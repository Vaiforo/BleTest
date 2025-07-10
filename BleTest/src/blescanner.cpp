// src/blescanner.cpp
#include "blescanner.h"

#include <QTimer>
#include <QDebug>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusVariant>
#include <QtDBus/QDBusPendingCall>
#include <QtDBus/QDBusPendingReply>

BleScanner::BleScanner(QObject *parent) : QObject(parent)
{
    m_scanTimer = new QTimer(this);
    m_scanTimer->setSingleShot(true);
    connect(m_scanTimer, &QTimer::timeout, this, &BleScanner::stopRollCall);
}

/*───────────────────────────── public API ───────────────────────────────────*/
void BleScanner::startRollCall()
{
    if (m_scanning)
        return;

    /* Очистить прошлый список */
    m_present.clear();
    emit presentChanged();

    /* Подключаемся к сигналу ObjectManager::InterfacesAdded */
    connectBlueZSignals();

    /* Запускаем Discovery */
    QDBusInterface adapter("org.bluez", "/org/bluez/hci0",
                           "org.bluez.Adapter1",
                           QDBusConnection::systemBus());
    adapter.call("StartDiscovery");

    m_scanning = true;
    emit scanningChanged();

    m_scanTimer->start(kScanMs);
    qDebug() << "BleScanner: roll-call started";
}

void BleScanner::stopRollCall()
{
    if (!m_scanning)
        return;

    /* Останов Discovery и отписка от сигналов */
    QDBusInterface adapter("org.bluez", "/org/bluez/hci0",
                           "org.bluez.Adapter1",
                           QDBusConnection::systemBus());
    adapter.call("StopDiscovery");

    disconnectBlueZSignals();

    m_scanning = false;
    emit scanningChanged();

    qDebug() << "BleScanner: roll-call stopped";
}

/*────────────────────────── DBus signal handler ─────────────────────────────*/
void BleScanner::onInterfacesAdded(const QDBusMessage &msg)
{
    /* Параметры: (object_path, dict{interface -> dict{prop -> val}}) */
    if (msg.arguments().size() < 2)
        return;

    QVariantMap ifaces =
        qdbus_cast<QVariantMap>(msg.arguments().at(1));

    if (!ifaces.contains("org.bluez.Device1"))
        return;

    QVariantMap props =
        qdbus_cast<QVariantMap>(ifaces.value("org.bluez.Device1"));

    if (!props.contains("ManufacturerData"))
        return;

    /* ManufacturerData приходит как dict<uint16, variant<ay>>  */
    QVariant mdVar = props.value("ManufacturerData");
    QHash<quint16, QVariant> mdict =
        qdbus_cast<QHash<quint16, QVariant>>(mdVar);

    if (!mdict.contains(kCompanyId))
        return;

    QByteArray idBytes = mdict.value(kCompanyId).toByteArray();
    QString    id      = QString::fromLatin1(idBytes);

    if (!m_idMap.contains(id))
        return;

    const QString &fio = m_idMap.value(id);
    if (m_present.contains(fio))
        return;

    m_present << fio;
    emit presentChanged();

    qDebug() << "BleScanner: detected" << fio;
}

/*──────────────────────── internal helpers ─────────────────────────────────*/
void BleScanner::connectBlueZSignals()
{
    QDBusConnection::systemBus().connect("org.bluez",
                                         "/",
                                         "org.freedesktop.DBus.ObjectManager",
                                         "InterfacesAdded",
                                         this,
                                         SLOT(onInterfacesAdded(QDBusMessage)));
}

void BleScanner::disconnectBlueZSignals()
{
    QDBusConnection::systemBus().disconnect("org.bluez",
                                            "/",
                                            "org.freedesktop.DBus.ObjectManager",
                                            "InterfacesAdded",
                                            this,
                                            SLOT(onInterfacesAdded(QDBusMessage)));
}
