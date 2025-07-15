TARGET = ru.template.BleTest

CONFIG += \
    auroraapp

PKGCONFIG += \

QT += core qml quick dbus

HEADERS += \
    src/bleadvertiser.h \
    src/blescanner.h

DISTFILES += \
    qml/BleTest.qml \
    qml/StudentPage.qml \
    qml/TeacherPage.qml \
    qml/cover/DefaultCoverPage.qml \
    qml/icons/BleTest.svg \
    qml/pages/AboutPage.qml \
    qml/pages/MainPage.qml \
    rpm/ru.template.BleTest.spec \

AURORAAPP_ICONS = 86x86 108x108 128x128 172x172

CONFIG += auroraapp_i18n

TRANSLATIONS += \
    translations/ru.template.BleTest.ts \
    translations/ru.template.BleTest-ru.ts \

SOURCES += \
    src/bleadvertiser.cpp \
    src/blescanner.cpp \
    src/main.cpp
