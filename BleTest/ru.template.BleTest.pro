TARGET = ru.template.BleTest

CONFIG += \
    auroraapp

PKGCONFIG += \

QT += core qml quick dbus

SOURCES += \
    src/bleadvertiser.cpp \
    src/blescanner.cpp \
    src/main.cpp \

HEADERS += \
    src/bleadvertiser.h \
    src/blescanner.h

DISTFILES += \
    qml/StudentPage.qml \
    qml/TeacherPage.qml \
    rpm/ru.template.BleTest.spec \

AURORAAPP_ICONS = 86x86 108x108 128x128 172x172

CONFIG += auroraapp_i18n

TRANSLATIONS += \
    translations/ru.template.BleTest.ts \
    translations/ru.template.BleTest-ru.ts \
