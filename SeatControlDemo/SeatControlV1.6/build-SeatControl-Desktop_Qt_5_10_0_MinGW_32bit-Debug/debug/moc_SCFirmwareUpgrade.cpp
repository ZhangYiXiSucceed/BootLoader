/****************************************************************************
** Meta object code from reading C++ file 'SCFirmwareUpgrade.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SCFirmwareUpgrade.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SCFirmwareUpgrade.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SCFirmwareUpgrade_t {
    QByteArrayData data[19];
    char stringdata0[249];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SCFirmwareUpgrade_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SCFirmwareUpgrade_t qt_meta_stringdata_SCFirmwareUpgrade = {
    {
QT_MOC_LITERAL(0, 0, 17), // "SCFirmwareUpgrade"
QT_MOC_LITERAL(1, 18, 11), // "ShowMessage"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 8), // "uint16_t"
QT_MOC_LITERAL(4, 40, 11), // "SendCommand"
QT_MOC_LITERAL(5, 52, 19), // "TransmitFileAddress"
QT_MOC_LITERAL(6, 72, 8), // "OpenFile"
QT_MOC_LITERAL(7, 81, 11), // "ReadFirmwre"
QT_MOC_LITERAL(8, 93, 8), // "Download"
QT_MOC_LITERAL(9, 102, 14), // "RecProgressBar"
QT_MOC_LITERAL(10, 117, 7), // "uint8_t"
QT_MOC_LITERAL(11, 125, 8), // "EnterAPP"
QT_MOC_LITERAL(12, 134, 16), // "ReturnBootLoader"
QT_MOC_LITERAL(13, 151, 18), // "ReadProgramerState"
QT_MOC_LITERAL(14, 170, 15), // "RecFirmwareInfo"
QT_MOC_LITERAL(15, 186, 12), // "AutoDownload"
QT_MOC_LITERAL(16, 199, 11), // "TimerUpdate"
QT_MOC_LITERAL(17, 211, 17), // "RecDownloadFinish"
QT_MOC_LITERAL(18, 229, 19) // "SelectCurrentWidget"

    },
    "SCFirmwareUpgrade\0ShowMessage\0\0uint16_t\0"
    "SendCommand\0TransmitFileAddress\0"
    "OpenFile\0ReadFirmwre\0Download\0"
    "RecProgressBar\0uint8_t\0EnterAPP\0"
    "ReturnBootLoader\0ReadProgramerState\0"
    "RecFirmwareInfo\0AutoDownload\0TimerUpdate\0"
    "RecDownloadFinish\0SelectCurrentWidget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SCFirmwareUpgrade[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   89,    2, 0x06 /* Public */,
       4,    1,   94,    2, 0x06 /* Public */,
       5,    2,   97,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,  102,    2, 0x08 /* Private */,
       7,    0,  103,    2, 0x08 /* Private */,
       8,    0,  104,    2, 0x08 /* Private */,
       9,    1,  105,    2, 0x08 /* Private */,
      11,    0,  108,    2, 0x08 /* Private */,
      12,    0,  109,    2, 0x08 /* Private */,
      13,    0,  110,    2, 0x08 /* Private */,
      14,    2,  111,    2, 0x08 /* Private */,
      15,    0,  116,    2, 0x08 /* Private */,
      16,    0,  117,    2, 0x08 /* Private */,
      17,    0,  118,    2, 0x08 /* Private */,
      18,    1,  119,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 3,    2,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::ULongLong,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10, QMetaType::QString,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void SCFirmwareUpgrade::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SCFirmwareUpgrade *_t = static_cast<SCFirmwareUpgrade *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ShowMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< uint16_t(*)>(_a[2]))); break;
        case 1: _t->SendCommand((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: _t->TransmitFileAddress((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2]))); break;
        case 3: _t->OpenFile(); break;
        case 4: _t->ReadFirmwre(); break;
        case 5: _t->Download(); break;
        case 6: _t->RecProgressBar((*reinterpret_cast< uint8_t(*)>(_a[1]))); break;
        case 7: _t->EnterAPP(); break;
        case 8: _t->ReturnBootLoader(); break;
        case 9: _t->ReadProgramerState(); break;
        case 10: _t->RecFirmwareInfo((*reinterpret_cast< uint8_t(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 11: _t->AutoDownload(); break;
        case 12: _t->TimerUpdate(); break;
        case 13: _t->RecDownloadFinish(); break;
        case 14: _t->SelectCurrentWidget((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SCFirmwareUpgrade::*_t)(QString , uint16_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SCFirmwareUpgrade::ShowMessage)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SCFirmwareUpgrade::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SCFirmwareUpgrade::SendCommand)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (SCFirmwareUpgrade::*_t)(QString , quint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SCFirmwareUpgrade::TransmitFileAddress)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject SCFirmwareUpgrade::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SCFirmwareUpgrade.data,
      qt_meta_data_SCFirmwareUpgrade,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SCFirmwareUpgrade::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SCFirmwareUpgrade::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SCFirmwareUpgrade.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SCFirmwareUpgrade::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void SCFirmwareUpgrade::ShowMessage(QString _t1, uint16_t _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SCFirmwareUpgrade::SendCommand(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SCFirmwareUpgrade::TransmitFileAddress(QString _t1, quint64 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
