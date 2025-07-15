// src/bleadvertiser.h — заголовок без изменений, но с комментариями по логированию
#pragma once

#include <QObject>
#include <QString>
#include <QPointer>

class QDBusInterface;

/**
 * BleAdvertiser
 *  Обёртка для регистрации BLE-рекламы в BlueZ (DBus).
 *  Используется на экране «Студент» (StudentPage.qml).
 *
 *  Теперь в реализации (bleadvertiser.cpp) добавлено подробное логирование
 *  через qInfo/qDebug/qWarning — см. там.
 */
class BleAdvertiser : public QObject
{
    Q_OBJECT
public:
    explicit BleAdvertiser(QObject *parent = nullptr);

    /** Запустить рекламу с указанным идентификатором студента. */
    Q_INVOKABLE void advertiseStudent(const QString &id);

public slots:
    /** Досрочно остановить рекламу (можно вызвать из QML). */
    void stopAdvertising();

private:
    /* Внутренние константы */
    static constexpr quint16 kCompanyId = 0xFFFF;   ///< Manufacturer ID
    static const     int     kAdvMs     = 60 * 1000;///< длительность рекламы, мс

    /* Текущее состояние */
    QString                  m_currentPath;   ///< DBus object path ("/ble_adv")
    QPointer<QDBusInterface> m_mgr;           ///< org.bluez.LEAdvertisingManager1
};
