/****************************************************************************
** Meta object code from reading C++ file 'bond.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gui/bond/bond.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bond.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Cbond[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    7,    6,    6, 0x05,
      48,   44,    6,    6, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_Cbond[] = {
    "Cbond\0\0dt\0signalNotifyUpdateTime(SDateTime)\0"
    "dev\0signalNotifyDevInfo(SGuiDev)\0"
};

void Cbond::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Cbond *_t = static_cast<Cbond *>(_o);
        switch (_id) {
        case 0: _t->signalNotifyUpdateTime((*reinterpret_cast< SDateTime(*)>(_a[1]))); break;
        case 1: _t->signalNotifyDevInfo((*reinterpret_cast< SGuiDev(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Cbond::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Cbond::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Cbond,
      qt_meta_data_Cbond, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Cbond::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Cbond::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Cbond::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Cbond))
        return static_cast<void*>(const_cast< Cbond*>(this));
    return QObject::qt_metacast(_clname);
}

int Cbond::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Cbond::signalNotifyUpdateTime(SDateTime _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Cbond::signalNotifyDevInfo(SGuiDev _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
