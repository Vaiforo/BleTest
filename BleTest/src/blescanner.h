// src/blescanner.h
#pragma once

#include <QObject>
#include <QStringList>
#include <QHash>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>

class QTimer;

/**
 * @brief Приём BLE-рекламных пакетов (ManufacturerData 0xFFFF)
 *
 * Логика:
 *   • startRollCall()  ‒ включает Discovery в BlueZ, подписывается на сигнал
 *     InterfacesAdded и в течение 10 минут собирает ID студентов.
 *   • stopRollCall()   ‒ останавливает Discovery досрочно.
 *   • present()        ‒ список ФИО, которые уже отметились (для QML-ListView).
 *
 * Отображаемые имена берутся из словаря idMap  «id → ФИО».
 */
class BleScanner : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool         scanning READ scanning  NOTIFY scanningChanged)
    Q_PROPERTY(QStringList  present  READ present   NOTIFY presentChanged)

public:
    explicit BleScanner(QObject *parent = nullptr);

    Q_INVOKABLE void startRollCall();   ///< Запустить 10-минутное сканирование
    Q_INVOKABLE void stopRollCall();    ///< Досрочно остановить сканирование

    bool         scanning() const { return m_scanning; }
    QStringList  present()  const { return m_present; }

signals:
    void scanningChanged();
    void presentChanged();

private slots:
    void onInterfacesAdded(const QDBusMessage &msg);  ///< обработка DBus-сигнала

private:
    void connectBlueZSignals();
    void disconnectBlueZSignals();

    bool        m_scanning {false};
    QStringList m_present;
    QTimer     *m_scanTimer {nullptr};

    // Словарь "id" → "ФИО"
    QHash<QString, QString> m_idMap {
        {"111111", "Студент 1"},
        {"222222", "Студент 2"},
        {"333333", "Студент 3"}
    };

    static constexpr quint16 kCompanyId = 0xFFFF;          ///< Manufacturer ID
    static const     int     kScanMs    = 10 * 60 * 1000;  ///< 10 минут
};
