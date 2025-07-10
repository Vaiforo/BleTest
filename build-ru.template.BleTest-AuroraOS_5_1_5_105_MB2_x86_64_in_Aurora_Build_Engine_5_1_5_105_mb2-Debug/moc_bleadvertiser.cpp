/****************************************************************************
** Meta object code from reading C++ file 'bleadvertiser.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BleTest/src/bleadvertiser.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bleadvertiser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BleAdvertiser_t {
    QByteArrayData data[5];
    char stringdata0[51];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BleAdvertiser_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BleAdvertiser_t qt_meta_stringdata_BleAdvertiser = {
    {
QT_MOC_LITERAL(0, 0, 13), // "BleAdvertiser"
QT_MOC_LITERAL(1, 14, 15), // "stopAdvertising"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 16), // "advertiseStudent"
QT_MOC_LITERAL(4, 48, 2) // "id"

    },
    "BleAdvertiser\0stopAdvertising\0\0"
    "advertiseStudent\0id"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BleAdvertiser[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
       3,    1,   25,    2, 0x02 /* Public */,

 // slots: parameters
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,    4,

       0        // eod
};

void BleAdvertiser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BleAdvertiser *_t = static_cast<BleAdvertiser *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stopAdvertising(); break;
        case 1: _t->advertiseStudent((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject BleAdvertiser::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BleAdvertiser.data,
      qt_meta_data_BleAdvertiser,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BleAdvertiser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BleAdvertiser::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BleAdvertiser.stringdata0))
        return static_cast<void*>(const_cast< BleAdvertiser*>(this));
    return QObject::qt_metacast(_clname);
}

int BleAdvertiser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
