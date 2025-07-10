/****************************************************************************
** Meta object code from reading C++ file 'blescanner.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BleTest/src/blescanner.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'blescanner.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BleScanner_t {
    QByteArrayData data[11];
    char stringdata0[122];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BleScanner_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BleScanner_t qt_meta_stringdata_BleScanner = {
    {
QT_MOC_LITERAL(0, 0, 10), // "BleScanner"
QT_MOC_LITERAL(1, 11, 15), // "scanningChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 14), // "presentChanged"
QT_MOC_LITERAL(4, 43, 17), // "onInterfacesAdded"
QT_MOC_LITERAL(5, 61, 12), // "QDBusMessage"
QT_MOC_LITERAL(6, 74, 3), // "msg"
QT_MOC_LITERAL(7, 78, 13), // "startRollCall"
QT_MOC_LITERAL(8, 92, 12), // "stopRollCall"
QT_MOC_LITERAL(9, 105, 8), // "scanning"
QT_MOC_LITERAL(10, 114, 7) // "present"

    },
    "BleScanner\0scanningChanged\0\0presentChanged\0"
    "onInterfacesAdded\0QDBusMessage\0msg\0"
    "startRollCall\0stopRollCall\0scanning\0"
    "present"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BleScanner[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       2,   46, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   41,    2, 0x08 /* Private */,

 // methods: name, argc, parameters, tag, flags
       7,    0,   44,    2, 0x02 /* Public */,
       8,    0,   45,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       9, QMetaType::Bool, 0x00495001,
      10, QMetaType::QStringList, 0x00495001,

 // properties: notify_signal_id
       0,
       1,

       0        // eod
};

void BleScanner::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BleScanner *_t = static_cast<BleScanner *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->scanningChanged(); break;
        case 1: _t->presentChanged(); break;
        case 2: _t->onInterfacesAdded((*reinterpret_cast< const QDBusMessage(*)>(_a[1]))); break;
        case 3: _t->startRollCall(); break;
        case 4: _t->stopRollCall(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QDBusMessage >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BleScanner::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BleScanner::scanningChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (BleScanner::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BleScanner::presentChanged)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        BleScanner *_t = static_cast<BleScanner *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->scanning(); break;
        case 1: *reinterpret_cast< QStringList*>(_v) = _t->present(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject BleScanner::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BleScanner.data,
      qt_meta_data_BleScanner,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BleScanner::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BleScanner::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BleScanner.stringdata0))
        return static_cast<void*>(const_cast< BleScanner*>(this));
    return QObject::qt_metacast(_clname);
}

int BleScanner::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void BleScanner::scanningChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void BleScanner::presentChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
