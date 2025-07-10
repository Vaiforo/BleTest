// src/bleadvertiser.h — отправка BLE-рекламного пакета (AuroraOS, Qt 5 + QtDBus)
#pragma once

#include <QObject>
#include <QString>
#include <QPointer>

class QDBusInterface;

/**
 * BleAdvertiser
 *  C++‑обёртка для регистрации рекламного пакета через BlueZ D‑Bus
 *  Используется на экране «Студент» (StudentPage.qml).
 */
class BleAdvertiser : public QObject
{
    Q_OBJECT
public:
    explicit BleAdvertiser(QObject *parent = nullptr);

    /** Запустить рекламу с указанным идентификатором. */
    Q_INVOKABLE void advertiseStudent(const QString &id);

public slots:
    /** Остановить рекламу досрочно (можно вызвать из QML). */
    void stopAdvertising();

private:
    // Внутренние константы
    static constexpr quint16 kCompanyId = 0xFFFF;  // Manufacturer ID
    static const     int     kAdvMs     = 10 * 1000; // длительность рекламы (мс)

    // Состояние текущей рекламы
    QString                 m_currentPath;  // D‑Bus object path, напр. "/ble_adv"
    QPointer<QDBusInterface> m_mgr;         // org.bluez.LEAdvertisingManager1
};
