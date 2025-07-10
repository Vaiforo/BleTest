// src/bleadvertiser.cpp
#include "bleadvertiser.h"

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusObjectPath>
#include <QtDBus/QDBusPendingCall>
#include <QtDBus/QDBusPendingCallWatcher>
#include <QtDBus/QDBusPendingReply>
#include <QTimer>
#include <QDebug>

BleAdvertiser::BleAdvertiser(QObject *parent) : QObject(parent) {}

/*───────────────────── публичный QML-вызываемый метод ───────────────────────*/
void BleAdvertiser::advertiseStudent(const QString &id)
{
    if (id.isEmpty()) {
        qWarning() << "BleAdvertiser: empty student ID";
        return;
    }
    if (!m_currentPath.isEmpty()) {            // уже идёт реклама
        qWarning() << "BleAdvertiser: advertising already active";
        return;
    }

    /* 1. Формируем свойства рекламы */
    QVariantMap props;
    props["Type"]         = QStringLiteral("broadcast");
    props["Discoverable"] = false;

    QVariantMap mdata;
    mdata.insert(QString::number(kCompanyId),
                 QVariant::fromValue(QByteArray(id.toLatin1())));
    props["ManufacturerData"] = QVariant::fromValue(mdata);

    /* 2. Регистрируем «пустой» D-Bus-объект, BlueZ читает свойства через DBus.Properties */
    const QString path = "/ble_adv";
    if (!QDBusConnection::systemBus().registerObject(path, this,
            QDBusConnection::ExportAllProperties))
    {
        qWarning() << "BleAdvertiser: cannot register object path" << path;
        return;
    }

    /* 3. Создаём интерфейс менеджера рекламы */
    auto *mgr = new QDBusInterface("org.bluez", "/org/bluez/hci0",
                                   "org.bluez.LEAdvertisingManager1",
                                   QDBusConnection::systemBus(), this);
    if (!mgr->isValid()) {
        qWarning() << "BleAdvertiser: BlueZ LEAdvertisingManager1 not found";
        QDBusConnection::systemBus().unregisterObject(path);
        mgr->deleteLater();
        return;
    }

    /* 4. Асинхронно вызываем RegisterAdvertisement */
    QDBusPendingCall pc =
        mgr->asyncCall(QStringLiteral("RegisterAdvertisement"),
                       QVariant::fromValue(QDBusObjectPath(path)),
                       QVariant::fromValue(props));

    auto *watcher = new QDBusPendingCallWatcher(pc, this);
    connect(watcher, &QDBusPendingCallWatcher::finished,
            this, [=] {
        watcher->deleteLater();
        QDBusPendingReply<> reply = *watcher;
        if (reply.isError()) {
            qWarning() << "BleAdvertiser: RegisterAdvertisement error:"
                       << reply.error().message();
            QDBusConnection::systemBus().unregisterObject(path);
            mgr->deleteLater();
            return;
        }

        /* Реклама успешно запущена */
        m_currentPath = path;
        m_mgr         = mgr;
        qDebug() << "BleAdvertiser: advertising started for" << kAdvMs << "ms";

        /* авто-остановка через kAdvMs */
        QTimer::singleShot(kAdvMs, this, &BleAdvertiser::stopAdvertising);
    });
}

/*──────────────────────────── публичный слот ────────────────────────────────*/
void BleAdvertiser::stopAdvertising()
{
    if (m_currentPath.isEmpty() || !m_mgr)
        return;                                     // нечего останавливать

    m_mgr->call(QDBus::NoBlock, "UnregisterAdvertisement",
                QVariant::fromValue(QDBusObjectPath(m_currentPath)));

    QDBusConnection::systemBus().unregisterObject(m_currentPath);

    qDebug() << "BleAdvertiser: advertising stopped";

    m_mgr->deleteLater();
    m_mgr = nullptr;
    m_currentPath.clear();
}
