/****************************************************************************
** Meta object code from reading C++ file 'qthesismainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qthesismainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qthesismainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QThesisMainWindow_t {
    QByteArrayData data[11];
    char stringdata[228];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_QThesisMainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_QThesisMainWindow_t qt_meta_stringdata_QThesisMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 17),
QT_MOC_LITERAL(1, 18, 20),
QT_MOC_LITERAL(2, 39, 0),
QT_MOC_LITERAL(3, 40, 20),
QT_MOC_LITERAL(4, 61, 7),
QT_MOC_LITERAL(5, 69, 21),
QT_MOC_LITERAL(6, 91, 20),
QT_MOC_LITERAL(7, 112, 24),
QT_MOC_LITERAL(8, 137, 29),
QT_MOC_LITERAL(9, 167, 29),
QT_MOC_LITERAL(10, 197, 29)
    },
    "QThesisMainWindow\0onStartButtonClicked\0"
    "\0onWeatherAllSelected\0checked\0"
    "onWeatherRainSelected\0onWeatherDrySelected\0"
    "onSpecialDaysAllSelected\0"
    "onSpecialDaysWeekdaysSelected\0"
    "onSpecialDaysWeekendsSelected\0"
    "onSpecialDaysHolidaysSelected\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QThesisMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08,
       3,    1,   55,    2, 0x08,
       5,    1,   58,    2, 0x08,
       6,    1,   61,    2, 0x08,
       7,    1,   64,    2, 0x08,
       8,    1,   67,    2, 0x08,
       9,    1,   70,    2, 0x08,
      10,    1,   73,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,

       0        // eod
};

void QThesisMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QThesisMainWindow *_t = static_cast<QThesisMainWindow *>(_o);
        switch (_id) {
        case 0: _t->onStartButtonClicked(); break;
        case 1: _t->onWeatherAllSelected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->onWeatherRainSelected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->onWeatherDrySelected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->onSpecialDaysAllSelected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->onSpecialDaysWeekdaysSelected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->onSpecialDaysWeekendsSelected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->onSpecialDaysHolidaysSelected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject QThesisMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QThesisMainWindow.data,
      qt_meta_data_QThesisMainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *QThesisMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QThesisMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QThesisMainWindow.stringdata))
        return static_cast<void*>(const_cast< QThesisMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QThesisMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
