/****************************************************************************
** Meta object code from reading C++ file 'SCParameter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../SCParameter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
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
    QByteArrayData data[47];
    char stringdata0[825];
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
QT_MOC_LITERAL(10, 119, 17), // "SendMotorPosition"
QT_MOC_LITERAL(11, 137, 16), // "QVector<quint16>"
QT_MOC_LITERAL(12, 154, 14), // "ConnectComPort"
QT_MOC_LITERAL(13, 169, 11), // "TimerUpdate"
QT_MOC_LITERAL(14, 181, 11), // "ReadRecData"
QT_MOC_LITERAL(15, 193, 10), // "WriteBytes"
QT_MOC_LITERAL(16, 204, 14), // "RecFileAddress"
QT_MOC_LITERAL(17, 219, 13), // "CalibrateMode"
QT_MOC_LITERAL(18, 233, 10), // "MotorSpeed"
QT_MOC_LITERAL(19, 244, 16), // "MotorBackReverse"
QT_MOC_LITERAL(20, 261, 15), // "MotorLegReverse"
QT_MOC_LITERAL(21, 277, 16), // "MotorFootReverse"
QT_MOC_LITERAL(22, 294, 24), // "MotorBackPositionReverse"
QT_MOC_LITERAL(23, 319, 23), // "MotorLegPositionReverse"
QT_MOC_LITERAL(24, 343, 24), // "MotorFootPositionReverse"
QT_MOC_LITERAL(25, 368, 26), // "ShowMotorBackClosePosition"
QT_MOC_LITERAL(26, 395, 25), // "ShowMotorBackOpenPosition"
QT_MOC_LITERAL(27, 421, 25), // "ShowMotorLegClosePosition"
QT_MOC_LITERAL(28, 447, 24), // "ShowMotorLegOpenPosition"
QT_MOC_LITERAL(29, 472, 26), // "ShowMotorFootClosePosition"
QT_MOC_LITERAL(30, 499, 25), // "ShowMotorFootOpenPosition"
QT_MOC_LITERAL(31, 525, 22), // "ShowLinkMotorBackStart"
QT_MOC_LITERAL(32, 548, 20), // "ShowLinkMotorBackEnd"
QT_MOC_LITERAL(33, 569, 28), // "ShowLinkMotorBackMotorLegEnd"
QT_MOC_LITERAL(34, 598, 21), // "ShowLinkMotorLegStart"
QT_MOC_LITERAL(35, 620, 19), // "ShowLinkMotorLegEnd"
QT_MOC_LITERAL(36, 640, 21), // "ShowLinkMotorBackHalf"
QT_MOC_LITERAL(37, 662, 20), // "ShowLinkMotorLegHalf"
QT_MOC_LITERAL(38, 683, 21), // "ShowLinkMotorFootHalf"
QT_MOC_LITERAL(39, 705, 8), // "ReadPara"
QT_MOC_LITERAL(40, 714, 8), // "SavePara"
QT_MOC_LITERAL(41, 723, 9), // "WritePara"
QT_MOC_LITERAL(42, 733, 8), // "LoadPara"
QT_MOC_LITERAL(43, 742, 23), // "SetClearCalibrationFlag"
QT_MOC_LITERAL(44, 766, 18), // "TimerCloseResponse"
QT_MOC_LITERAL(45, 785, 19), // "FrameRecDataProcess"
QT_MOC_LITERAL(46, 805, 19) // "SelectCurrentWidget"

    },
    "SCParameter\0ShowMessage\0\0uint16_t\0"
    "ShowParameter\0SendProgressBar\0uint8_t\0"
    "SendFirmwareInfo\0SendButtonInfo\0"
    "DownloadFinish\0SendMotorPosition\0"
    "QVector<quint16>\0ConnectComPort\0"
    "TimerUpdate\0ReadRecData\0WriteBytes\0"
    "RecFileAddress\0CalibrateMode\0MotorSpeed\0"
    "MotorBackReverse\0MotorLegReverse\0"
    "MotorFootReverse\0MotorBackPositionReverse\0"
    "MotorLegPositionReverse\0"
    "MotorFootPositionReverse\0"
    "ShowMotorBackClosePosition\0"
    "ShowMotorBackOpenPosition\0"
    "ShowMotorLegClosePosition\0"
    "ShowMotorLegOpenPosition\0"
    "ShowMotorFootClosePosition\0"
    "ShowMotorFootOpenPosition\0"
    "ShowLinkMotorBackStart\0ShowLinkMotorBackEnd\0"
    "ShowLinkMotorBackMotorLegEnd\0"
    "ShowLinkMotorLegStart\0ShowLinkMotorLegEnd\0"
    "ShowLinkMotorBackHalf\0ShowLinkMotorLegHalf\0"
    "ShowLinkMotorFootHalf\0ReadPara\0SavePara\0"
    "WritePara\0LoadPara\0SetClearCalibrationFlag\0"
    "TimerCloseResponse\0FrameRecDataProcess\0"
    "SelectCurrentWidget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SCParameter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      42,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  224,    2, 0x06 /* Public */,
       4,    2,  229,    2, 0x06 /* Public */,
       5,    1,  234,    2, 0x06 /* Public */,
       7,    2,  237,    2, 0x06 /* Public */,
       8,    3,  242,    2, 0x06 /* Public */,
       9,    0,  249,    2, 0x06 /* Public */,
      10,    1,  250,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,  253,    2, 0x08 /* Private */,
      13,    0,  254,    2, 0x08 /* Private */,
      14,    0,  255,    2, 0x08 /* Private */,
      15,    1,  256,    2, 0x08 /* Private */,
      16,    2,  259,    2, 0x08 /* Private */,
      17,    0,  264,    2, 0x08 /* Private */,
      18,    0,  265,    2, 0x08 /* Private */,
      19,    0,  266,    2, 0x08 /* Private */,
      20,    0,  267,    2, 0x08 /* Private */,
      21,    0,  268,    2, 0x08 /* Private */,
      22,    0,  269,    2, 0x08 /* Private */,
      23,    0,  270,    2, 0x08 /* Private */,
      24,    0,  271,    2, 0x08 /* Private */,
      25,    0,  272,    2, 0x08 /* Private */,
      26,    0,  273,    2, 0x08 /* Private */,
      27,    0,  274,    2, 0x08 /* Private */,
      28,    0,  275,    2, 0x08 /* Private */,
      29,    0,  276,    2, 0x08 /* Private */,
      30,    0,  277,    2, 0x08 /* Private */,
      31,    0,  278,    2, 0x08 /* Private */,
      32,    0,  279,    2, 0x08 /* Private */,
      33,    0,  280,    2, 0x08 /* Private */,
      34,    0,  281,    2, 0x08 /* Private */,
      35,    0,  282,    2, 0x08 /* Private */,
      36,    0,  283,    2, 0x08 /* Private */,
      37,    0,  284,    2, 0x08 /* Private */,
      38,    0,  285,    2, 0x08 /* Private */,
      39,    0,  286,    2, 0x08 /* Private */,
      40,    0,  287,    2, 0x08 /* Private */,
      41,    0,  288,    2, 0x08 /* Private */,
      42,    0,  289,    2, 0x08 /* Private */,
      43,    0,  290,    2, 0x08 /* Private */,
      44,    0,  291,    2, 0x08 /* Private */,
      45,    0,  292,    2, 0x08 /* Private */,
      46,    1,  293,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 3,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, 0x80000000 | 6, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar, QMetaType::UShort,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,    2,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

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
        case 4: _t->SendButtonInfo((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        case 5: _t->DownloadFinish(); break;
        case 6: _t->SendMotorPosition((*reinterpret_cast< QVector<quint16>(*)>(_a[1]))); break;
        case 7: _t->ConnectComPort(); break;
        case 8: _t->TimerUpdate(); break;
        case 9: _t->ReadRecData(); break;
        case 10: _t->WriteBytes((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 11: _t->RecFileAddress((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2]))); break;
        case 12: _t->CalibrateMode(); break;
        case 13: _t->MotorSpeed(); break;
        case 14: _t->MotorBackReverse(); break;
        case 15: _t->MotorLegReverse(); break;
        case 16: _t->MotorFootReverse(); break;
        case 17: _t->MotorBackPositionReverse(); break;
        case 18: _t->MotorLegPositionReverse(); break;
        case 19: _t->MotorFootPositionReverse(); break;
        case 20: _t->ShowMotorBackClosePosition(); break;
        case 21: _t->ShowMotorBackOpenPosition(); break;
        case 22: _t->ShowMotorLegClosePosition(); break;
        case 23: _t->ShowMotorLegOpenPosition(); break;
        case 24: _t->ShowMotorFootClosePosition(); break;
        case 25: _t->ShowMotorFootOpenPosition(); break;
        case 26: _t->ShowLinkMotorBackStart(); break;
        case 27: _t->ShowLinkMotorBackEnd(); break;
        case 28: _t->ShowLinkMotorBackMotorLegEnd(); break;
        case 29: _t->ShowLinkMotorLegStart(); break;
        case 30: _t->ShowLinkMotorLegEnd(); break;
        case 31: _t->ShowLinkMotorBackHalf(); break;
        case 32: _t->ShowLinkMotorLegHalf(); break;
        case 33: _t->ShowLinkMotorFootHalf(); break;
        case 34: _t->ReadPara(); break;
        case 35: _t->SavePara(); break;
        case 36: _t->WritePara(); break;
        case 37: _t->LoadPara(); break;
        case 38: _t->SetClearCalibrationFlag(); break;
        case 39: _t->TimerCloseResponse(); break;
        case 40: _t->FrameRecDataProcess(); break;
        case 41: _t->SelectCurrentWidget((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<quint16> >(); break;
            }
            break;
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
            typedef void (SCParameter::*_t)(quint8 , quint8 , quint16 );
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
        {
            typedef void (SCParameter::*_t)(QVector<quint16> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SCParameter::SendMotorPosition)) {
                *result = 6;
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
        if (_id < 42)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 42;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 42)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 42;
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
void SCParameter::SendButtonInfo(quint8 _t1, quint8 _t2, quint16 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SCParameter::DownloadFinish()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void SCParameter::SendMotorPosition(QVector<quint16> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
