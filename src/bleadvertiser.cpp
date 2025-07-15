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

void BleAdvertiser::advertiseStudent(const QString &id)
{
    if (id.isEmpty()) {
        qWarning() << "BleAdvertiser: empty student ID";
        return;
    }
    if (!m_currentPath.isEmpty()) {
        qWarning() << "BleAdvertiser: advertising already active";
        return;
    }

    const QString path = "/ble_adv";

    // 1. Рекламные свойства
    QVariantMap props;
    props["Type"]         = QStringLiteral("broadcast");
    props["Discoverable"] = false;

    // Добавим UUID как "ServiceUUIDs", чтобы реклама выглядела стандартной
    props["ServiceUUIDs"] = QStringList{"1234"}; // можно заменить на val UUID

    // ManufacturerData
    QVariantMap mdata;
    mdata.insert(QString::number(kCompanyId),
                 QVariant::fromValue(QByteArray(id.toLatin1())));
    props["ManufacturerData"] = QVariant::fromValue(mdata);

    // 2. Регистрируем D-Bus объект
    if (!QDBusConnection::systemBus().registerObject(path, this,
            QDBusConnection::ExportAllProperties))
    {
        qWarning() << "BleAdvertiser: cannot register object path" << path;
        return;
    }

    // 3. Менеджер рекламы
    auto *mgr = new QDBusInterface("org.bluez", "/org/bluez/hci0",
                                   "org.bluez.LEAdvertisingManager1",
                                   QDBusConnection::systemBus(), this);
    if (!mgr->isValid()) {
        qWarning() << "BleAdvertiser: BlueZ LEAdvertisingManager1 not found";
        QDBusConnection::systemBus().unregisterObject(path);
        mgr->deleteLater();
        return;
    }

    // 4. Регистрируем рекламу
    QDBusPendingCall pc =
        mgr->asyncCall("RegisterAdvertisement",
                       QVariant::fromValue(QDBusObjectPath(path)),
                       QVariant::fromValue(props));

    auto *watcher = new QDBusPendingCallWatcher(pc, this);
    connect(watcher, &QDBusPendingCallWatcher::finished,
            this, [=] {
        watcher->deleteLater();
        QDBusPendingReply<> reply = *watcher;
        if (reply.isError()) {
            qWarning().noquote() << ts() << "[Advertiser] Register error:"
                                 << reply.error().message();
            QDBusConnection::systemBus().unregisterObject(path);
            mgr->deleteLater();
            return;
        }

        qInfo().noquote() << ts() << "[Advertiser] STARTED advertising ID ="
                          << id << "raw =" << id.toLatin1().toHex();

        m_currentPath = path;
        m_mgr         = mgr;

        QTimer::singleShot(kAdvMs, this, &BleAdvertiser::stopAdvertising);
    });
}


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
