// src/bleadvertiser.cpp — с расширенным логированием
#include "bleadvertiser.h"

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusObjectPath>
#include <QtDBus/QDBusPendingCall>
#include <QtDBus/QDBusPendingCallWatcher>
#include <QtDBus/QDBusPendingReply>
#include <QTimer>
#include <QDateTime>
#include <QDebug>

static inline QString ts()       // короткий тайм-стемп для лога
{ return QDateTime::currentDateTime().toString("hh:mm:ss.zzz"); }

BleAdvertiser::BleAdvertiser(QObject *parent) : QObject(parent)
{
    qInfo().noquote() << ts() << "[Advertiser] ctor OK";
}

/*───────────────────── публичный QML-вызываемый метод ───────────────────────*/
void BleAdvertiser::advertiseStudent(const QString &id)
{
    qInfo().noquote() << ts() << "[Advertiser] advertiseStudent() called with id =" << id;

    if (id.isEmpty()) {
        qWarning().noquote() << ts() << "[Advertiser] empty student ID → abort";
        return;
    }
    if (!m_currentPath.isEmpty()) {
        qWarning().noquote() << ts() << "[Advertiser] advertising already active, ignore";
        return;
    }

    /* 1. Формируем свойства рекламы ---------------------------------------- */
    QVariantMap props;
    props["Type"]         = QStringLiteral("broadcast");
    props["Discoverable"] = false;

    QVariantMap mdata;
    mdata.insert(QString::number(kCompanyId),
                 QVariant::fromValue(QByteArray(id.toLatin1())));
    props["ManufacturerData"] = QVariant::fromValue(mdata);

    qDebug().noquote() << ts() << "[Advertiser] Properties prepared:"
                       << "CompanyId =" << kCompanyId
                       << ", raw =" << id.toLatin1().toHex();

    /* 2. Регистрируем объект с Properties interface ------------------------ */
    const QString path = "/ble_adv";
    const bool okObj = QDBusConnection::systemBus().registerObject(
                path, this, QDBusConnection::ExportAllProperties);

    if (!okObj) {
        qWarning().noquote() << ts() << "[Advertiser] cannot register DBus object" << path;
        return;
    }
    qDebug().noquote() << ts() << "[Advertiser] DBus object registered at" << path;

    /* 3. Создаём интерфейс менеджера рекламы ------------------------------- */
    auto *mgr = new QDBusInterface("org.bluez", "/org/bluez/hci0",
                                   "org.bluez.LEAdvertisingManager1",
                                   QDBusConnection::systemBus(), this);

    if (!mgr->isValid()) {
        qWarning().noquote() << ts() << "[Advertiser] BlueZ LEAdvertisingManager1 not found";
        QDBusConnection::systemBus().unregisterObject(path);
        mgr->deleteLater();
        return;
    }
    qDebug().noquote() << ts() << "[Advertiser] Got LEAdvertisingManager1";

    /* 4. Асинхронно вызываем RegisterAdvertisement ------------------------- */
    QDBusPendingCall pc =
        mgr->asyncCall(QStringLiteral("RegisterAdvertisement"),
                       QVariant::fromValue(QDBusObjectPath(path)),
                       QVariant::fromValue(props));

    auto *watcher = new QDBusPendingCallWatcher(pc, this);
    connect(watcher, &QDBusPendingCallWatcher::finished,
            this, [=] {
        QDBusPendingReply<> reply = *watcher;
        watcher->deleteLater();

        if (reply.isError()) {
            qWarning().noquote() << ts()
                << "[Advertiser] RegisterAdvertisement error:"
                << reply.error().name() << reply.error().message();
            QDBusConnection::systemBus().unregisterObject(path);
            mgr->deleteLater();
            return;
        }

        /* Реклама успешно запущена */
        m_currentPath = path;
        m_mgr         = mgr;

        qInfo().noquote() << ts()
                          << "[Advertiser] advertising STARTED, will stop in"
                          << kAdvMs << "ms";

        /* авто-остановка через kAdvMs */
        QTimer::singleShot(kAdvMs, this, &BleAdvertiser::stopAdvertising);
    });
}

/*──────────────────────────── публичный слот ────────────────────────────────*/
void BleAdvertiser::stopAdvertising()
{
    if (m_currentPath.isEmpty() || !m_mgr) {
        qDebug().noquote() << ts() << "[Advertiser] stopAdvertising(): nothing to stop";
        return;
    }

    qDebug().noquote() << ts() << "[Advertiser] Sending UnregisterAdvertisement";

    m_mgr->call(QDBus::NoBlock, "UnregisterAdvertisement",
                QVariant::fromValue(QDBusObjectPath(m_currentPath)));

    QDBusConnection::systemBus().unregisterObject(m_currentPath);

    qInfo().noquote() << ts() << "[Advertiser] advertising STOPPED";

    m_mgr->deleteLater();
    m_mgr = nullptr;
    m_currentPath.clear();
}
