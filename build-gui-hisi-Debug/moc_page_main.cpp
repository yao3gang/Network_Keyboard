/****************************************************************************
** Meta object code from reading C++ file 'page_main.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gui/page/page_main.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'page_main.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_page_main[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      31,   28,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_page_main[] = {
    "page_main\0\0button_clicked()\0dt\0"
    "update_time(SDateTime)\0"
};

void page_main::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        page_main *_t = static_cast<page_main *>(_o);
        switch (_id) {
        case 0: _t->button_clicked(); break;
        case 1: _t->update_time((*reinterpret_cast< SDateTime(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData page_main::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject page_main::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_page_main,
      qt_meta_data_page_main, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &page_main::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *page_main::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *page_main::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_page_main))
        return static_cast<void*>(const_cast< page_main*>(this));
    return QDialog::qt_metacast(_clname);
}

int page_main::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
