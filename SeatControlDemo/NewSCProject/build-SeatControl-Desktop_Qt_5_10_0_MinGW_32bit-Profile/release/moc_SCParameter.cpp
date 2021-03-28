/****************************************************************************
** Meta object code from reading C++ file 'SCParameter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SeatControlV1.1/SCParameter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SCParameter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SCParameter_t {
    QByteArrayData data[23];
    char stringdata0[280];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SCParameter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SCParameter_t qt_meta_stringdata_SCParameter = {
    {
QT_MOC_LITERAL(0, 0, 11), // "SCParameter"
QT_MOC_LITERAL(1, 12, 11), // "ShowMessage"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 8), // "uint16_t"
QT_MOC_LITERAL(4, 34, 13), // "ShowParameter"
QT_MOC_LITERAL(5, 48, 15), // "SendProgressBar"
QT_MOC_LITERAL(6, 64, 7), // "uint8_t"
QT_MOC_LITERAL(7, 72, 16), // "SendFirmwareInfo"
QT_MOC_LITERAL(8, 89, 14), // "SendButtonInfo"
QT_MOC_LITERAL(9, 104, 14), // "DownloadFinish"
QT_MOC_LITERAL(10, 119, 14), // "ConnectComPort"
QT_MOC_LITERAL(11, 134, 11), // "TimerUpdate"
QT_MOC_LITERAL(12, 146, 11), // "ReadRecData"
QT_MOC_LITERAL(13, 158, 10), // "WriteBytes"
QT_MOC_LITERAL(14, 169, 14), // "RecFileAddress"
QT_MOC_LITERAL(15, 184, 13), // "CalibrateMode"
QT_MOC_LITERAL(16, 198, 10), // "MotorSpeed"
QT_MOC_LITERAL(17, 209, 12), // "MotorReverse"
QT_MOC_LITERAL(18, 222, 20), // "MotorPositionReverse"
QT_MOC_LITERAL(19, 243, 8), // "ReadPara"
QT_MOC_LITERAL(20, 252, 8), // "SavePara"
QT_MOC_LITERAL(21, 261, 9), // "WritePara"
QT_MOC_LITERAL(22, 271, 8) // "LoadPara"

    },
    "SCParameter\0ShowMessage\0\0uint16_t\0"
    "ShowParameter\0SendProgressBar\0uint8_t\0"
    "SendFirmwareInfo\0SendButtonInfo\0"
    "DownloadFinish\0ConnectComPort\0TimerUpdate\0"
    "ReadRecData\0WriteBytes\0RecFileAddress\0"
    "CalibrateMode\0MotorSpeed\0MotorReverse\0"
    "MotorPositionReverse\0ReadPara\0SavePara\0"
    "WritePara\0LoadPara"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SCParameter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  109,    2, 0x06 /* Public */,
       4,    2,  114,    2, 0x06 /* Public */,
       5,    1,  119,    2, 0x06 /* Public */,
       7,    2,  122,    2, 0x06 /* Public */,
       8,    2,  127,    2, 0x06 /* Public */,
       9,    0,  132,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,  133,    2, 0x08 /* Private */,
      11,    0,  134,    2, 0x08 /* Private */,
      12,    0,  135,    2, 0x08 /* Private */,
      13,    1,  136,    2, 0x08 /* Private */,
      14,    2,  139,    2, 0x08 /* Private */,
      15,    0,  144,    2, 0x08 /* Private */,
      16,    0,  145,    2, 0x08 /* Private */,
      17,    0,  146,    2, 0x08 /* Private */,
      18,    0,  147,    2, 0x08 /* Private */,
      19,    0,  148,    2, 0x08 /* Private */,
      20,    0,  149,    2, 0x08 /* Private */,
      21,    0,  150,    2, 0x08 /* Private */,
      22,    0,  151,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 3,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, 0x80000000 | 6, QMetaType::QString,    2,    2,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 3,    2,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::ULongLong,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SCParameter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SCParameter *_t = static_cast<SCParameter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ShowMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< uint16_t(*)>(_a[2]))); break;
        case 1: _t->ShowParameter((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->SendProgressBar((*reinterpret_cast< uint8_t(*)>(_a[1]))); break;
        case 3: _t->SendFirmwareInfo((*reinterpret_cast< uint8_t(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->SendButtonInfo((*reinterpret_cast< uint8_t(*)>(_a[1])),(*reinterpret_cast< uint16_t(*)>(_a[2]))); break;
        case 5: _t->DownloadFinish(); break;
        case 6: _t->ConnectComPort(); break;
        case 7: _t->TimerUpdate(); break;
        case 8: _t->ReadRecData(); break;
        case 9: _t->WriteBytes((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 10: _t->RecFileAddress((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2]))); break;
        case 11: _t->CalibrateMode(); break;
        case 12: _t->MotorSpeed(); break;
        case 13: _t->MotorReverse(); break;
        case 14: _t->MotorPositionReverse(); break;
        case 15: _t->ReadPara(); break;
        case 16: _t->SavePara(); break;
        case 17: _t->WritePara(); break;
        case 18: _t->LoadPara(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SCParameter::*_t)(QString , uint16_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SCParameter::ShowMessage)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SCParameter::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SCParameter::ShowParameter)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (SCParameter::*_t)(uint8_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SCParameter::SendProgressBar)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (SCParameter::*_t)(uint8_t , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SCParameter::SendFirmwareInfo)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (SCParameter::*_t)(uint8_t , uint16_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SCParameter::SendButtonInfo)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (SCParameter::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SCParameter::DownloadFinish)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject SCParameter::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SCParameter.data,
      qt_meta_data_SCParameter,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SCParameter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SCParameter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SCParameter.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SCParameter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void SCParameter::ShowMessage(QString _t1, uint16_t _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SCParameter::ShowParameter(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SCParameter::SendProgressBar(uint8_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SCParameter::SendFirmwareInfo(uint8_t _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SCParameter::SendButtonInfo(uint8_t _t1, uint16_t _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SCParameter::DownloadFinish()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
