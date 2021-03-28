/****************************************************************************
** Meta object code from reading C++ file 'SCButton.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SeatControlV1.2/SCButton.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SCButton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SCButton_t {
    QByteArrayData data[23];
    char stringdata0[264];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SCButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SCButton_t qt_meta_stringdata_SCButton = {
    {
QT_MOC_LITERAL(0, 0, 8), // "SCButton"
QT_MOC_LITERAL(1, 9, 11), // "ShowMessage"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 8), // "uint16_t"
QT_MOC_LITERAL(4, 31, 11), // "SendCommand"
QT_MOC_LITERAL(5, 43, 13), // "MotorBackOpen"
QT_MOC_LITERAL(6, 57, 14), // "MotorBackClose"
QT_MOC_LITERAL(7, 72, 13), // "MotorBackStop"
QT_MOC_LITERAL(8, 86, 12), // "MotorLegOpen"
QT_MOC_LITERAL(9, 99, 13), // "MotorLegClose"
QT_MOC_LITERAL(10, 113, 12), // "MotorLegStop"
QT_MOC_LITERAL(11, 126, 13), // "MotorFootOpen"
QT_MOC_LITERAL(12, 140, 14), // "MotorFootClose"
QT_MOC_LITERAL(13, 155, 13), // "MotorFootStop"
QT_MOC_LITERAL(14, 169, 8), // "FastOpen"
QT_MOC_LITERAL(15, 178, 9), // "FastClose"
QT_MOC_LITERAL(16, 188, 9), // "OpenClose"
QT_MOC_LITERAL(17, 198, 4), // "Half"
QT_MOC_LITERAL(18, 203, 11), // "ReadCurrent"
QT_MOC_LITERAL(19, 215, 11), // "RecParaData"
QT_MOC_LITERAL(20, 227, 7), // "uint8_t"
QT_MOC_LITERAL(21, 235, 11), // "TimerUpdate"
QT_MOC_LITERAL(22, 247, 16) // "StartReadCurrent"

    },
    "SCButton\0ShowMessage\0\0uint16_t\0"
    "SendCommand\0MotorBackOpen\0MotorBackClose\0"
    "MotorBackStop\0MotorLegOpen\0MotorLegClose\0"
    "MotorLegStop\0MotorFootOpen\0MotorFootClose\0"
    "MotorFootStop\0FastOpen\0FastClose\0"
    "OpenClose\0Half\0ReadCurrent\0RecParaData\0"
    "uint8_t\0TimerUpdate\0StartReadCurrent"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SCButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  109,    2, 0x06 /* Public */,
       4,    1,  114,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,  117,    2, 0x08 /* Private */,
       6,    0,  118,    2, 0x08 /* Private */,
       7,    0,  119,    2, 0x08 /* Private */,
       8,    0,  120,    2, 0x08 /* Private */,
       9,    0,  121,    2, 0x08 /* Private */,
      10,    0,  122,    2, 0x08 /* Private */,
      11,    0,  123,    2, 0x08 /* Private */,
      12,    0,  124,    2, 0x08 /* Private */,
      13,    0,  125,    2, 0x08 /* Private */,
      14,    0,  126,    2, 0x08 /* Private */,
      15,    0,  127,    2, 0x08 /* Private */,
      16,    0,  128,    2, 0x08 /* Private */,
      17,    0,  129,    2, 0x08 /* Private */,
      18,    0,  130,    2, 0x08 /* Private */,
      19,    2,  131,    2, 0x08 /* Private */,
      21,    0,  136,    2, 0x08 /* Private */,
      22,    0,  137,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 3,    2,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20, 0x80000000 | 3,    2,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SCButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SCButton *_t = static_cast<SCButton *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ShowMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< uint16_t(*)>(_a[2]))); break;
        case 1: _t->SendCommand((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: _t->MotorBackOpen(); break;
        case 3: _t->MotorBackClose(); break;
        case 4: _t->MotorBackStop(); break;
        case 5: _t->MotorLegOpen(); break;
        case 6: _t->MotorLegClose(); break;
        case 7: _t->MotorLegStop(); break;
        case 8: _t->MotorFootOpen(); break;
        case 9: _t->MotorFootClose(); break;
        case 10: _t->MotorFootStop(); break;
        case 11: _t->FastOpen(); break;
        case 12: _t->FastClose(); break;
        case 13: _t->OpenClose(); break;
        case 14: _t->Half(); break;
        case 15: _t->ReadCurrent(); break;
        case 16: _t->RecParaData((*reinterpret_cast< uint8_t(*)>(_a[1])),(*reinterpret_cast< uint16_t(*)>(_a[2]))); break;
        case 17: _t->TimerUpdate(); break;
        case 18: _t->StartReadCurrent(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SCButton::*_t)(QString , uint16_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SCButton::ShowMessage)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SCButton::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SCButton::SendCommand)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject SCButton::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SCButton.data,
      qt_meta_data_SCButton,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SCButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SCButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SCButton.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SCButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void SCButton::ShowMessage(QString _t1, uint16_t _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SCButton::SendCommand(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
