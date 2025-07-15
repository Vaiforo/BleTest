// src/blescanner.cpp — расширенное логирование сканера
#include "blescanner.h"

#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusVariant>
#include <QtDBus/QDBusPendingCall>
#include <QtDBus/QDBusPendingReply>

static inline QString ts() { return QDateTime::currentDateTime().toString("hh:mm:ss.zzz"); }

BleScanner::BleScanner(QObject *parent) : QObject(parent)
{
    m_scanTimer = new QTimer(this);
    m_scanTimer->setSingleShot(true);
    connect(m_scanTimer, &QTimer::timeout, this, &BleScanner::stopRollCall);

    qInfo().noquote() << ts() << "[Scanner] ctor OK";
}

void BleScanner::startRollCall()
{
    if (m_scanning) {
        qWarning().noquote() << ts() << "[Scanner] startRollCall() while already scanning";
        return;
    }

    /* Очистить прошлый список */
    m_present.clear();
    emit presentChanged();

    /* Подключаемся к сигналу ObjectManager::InterfacesAdded */
    connectBlueZSignals();

    /* Запускаем Discovery */
    QDBusInterface adapter("org.bluez", "/org/bluez/hci0",
                           "org.bluez.Adapter1",
                           QDBusConnection::systemBus());
    QDBusMessage reply = adapter.call("StartDiscovery");
    if (reply.type() == QDBusMessage::ErrorMessage) {
        qWarning().noquote() << ts()
            << "[Scanner] StartDiscovery error:" << reply.errorMessage();
        disconnectBlueZSignals();
        return;
    }

    m_scanning = true;
    emit scanningChanged();

    m_scanTimer->start(kScanMs);
    qInfo().noquote() << ts() << "[Scanner] roll-call STARTED for" << kScanMs << "ms";

    qDebug().noquote() << ts() << "[Scanner] Discovery started, waiting for interfaces...";
}

void BleScanner::stopRollCall()
{
    if (!m_scanning) {
        qDebug().noquote() << ts() << "[Scanner] stopRollCall(): not active";
        return;
    }

    /* Останов Discovery и отписка от сигналов */
    QDBusInterface adapter("org.bluez", "/org/bluez/hci0",
                           "org.bluez.Adapter1",
                           QDBusConnection::systemBus());
    QDBusMessage reply = adapter.call("StopDiscovery");
    if (reply.type() == QDBusMessage::ErrorMessage) {
        qWarning().noquote() << ts()
            << "[Scanner] StopDiscovery error:" << reply.errorMessage();
    }

    disconnectBlueZSignals();

    m_scanning = false;
    emit scanningChanged();

    qInfo().noquote() << ts() << "[Scanner] roll-call STOPPED";
}

void BleScanner::onInterfacesAdded(const QDBusMessage &msg)
{
    qDebug().noquote() << ts() << "[Scanner] InterfacesAdded raw args:"
                       << msg.arguments();

    if (msg.arguments().size() < 2) {
        qDebug().noquote() << ts() << "[Scanner] InterfacesAdded: no args";
        return;
    }

    const QVariantMap ifaces = qdbus_cast<QVariantMap>(msg.arguments().at(1));
    if (!ifaces.contains("org.bluez.Device1")) {
        qDebug().noquote() << ts() << "[Scanner] Not a Device1 interface";
        return;
    }

    const QVariantMap props = qdbus_cast<QVariantMap>(ifaces.value("org.bluez.Device1"));
    qDebug().noquote() << ts() << "[Scanner] Device1 props:" << props.keys();

    if (!props.contains("ManufacturerData")) {
        qDebug().noquote() << ts() << "[Scanner] No ManufacturerData";
        return;
    }

    const QVariant mdVar = props.value("ManufacturerData");
    const QHash<quint16, QVariant> mdict = qdbus_cast<QHash<quint16, QVariant>>(mdVar);
    qDebug().noquote() << ts() << "[Scanner] ManufacturerData keys:"
                       << QList<quint16>(mdict.keys());

//    if (!mdict.contains(kCompanyId)) {
    qDebug().noquote() << ts() << "[Scanner] No entry for Company ID 0x"
                       << QString::number(kCompanyId, 16);
//        return;
//    }

    const QByteArray idBytes = mdict.value(kCompanyId).toByteArray();
    const QString id = QString::fromLatin1(idBytes);

    qDebug().noquote() << ts()
                       << "[Scanner] RX adv id =" << id
                       << "raw =" << idBytes.toHex();

    if (!m_idMap.contains(id)) {
        qWarning().noquote() << ts() << "[Scanner] Unknown ID:" << id;
        return;
    }

    const QString &fio = m_idMap.value(id);
    if (m_present.contains(fio)) {
        qDebug().noquote() << ts() << "[Scanner] duplicate ID =" << id;
        return;
    }

    m_present << fio;
    emit presentChanged();

    qInfo().noquote() << ts() << "[Scanner] DETECTED" << fio;
}


void BleScanner::connectBlueZSignals()
{
    const bool ok = QDBusConnection::systemBus().connect("org.bluez",
                                                         "/",
                                                         "org.freedesktop.DBus.ObjectManager",
                                                         "InterfacesAdded",
                                                         this,
                                                         SLOT(onInterfacesAdded(QDBusMessage)));

    qDebug().noquote() << ts() << "[Scanner] connectBlueZSignals() =" << ok;
}

void BleScanner::disconnectBlueZSignals()
{
    const bool ok = QDBusConnection::systemBus().disconnect("org.bluez",
                                                            "/",
                                                            "org.freedesktop.DBus.ObjectManager",
                                                            "InterfacesAdded",
                                                            this,
                                                            SLOT(onInterfacesAdded(QDBusMessage)));

    qDebug().noquote() << ts() << "[Scanner] disconnectBlueZSignals() =" << ok;
}
