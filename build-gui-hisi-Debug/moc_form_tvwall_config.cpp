/****************************************************************************
** Meta object code from reading C++ file 'form_tvwall_config.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gui/page/form_tvwall_config.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'form_tvwall_config.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_form_tvwall_config[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   20,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      56,   19,   19,   19, 0x08,
      77,   19,   19,   19, 0x08,
     101,   19,   19,   19, 0x08,
     121,   19,   19,   19, 0x08,
     147,  142,   19,   19, 0x08,
     168,  142,   19,   19, 0x08,
     193,  189,   19,   19, 0x08,
     230,  217,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_form_tvwall_config[] = {
    "form_tvwall_config\0\0data\0"
    "signal_tvwall_data(QByteArray)\0"
    "on_btn_clr_clicked()\0on_btn_cancel_clicked()\0"
    "on_btn_ok_clicked()\0btn_unbind_clicked()\0"
    "text\0rowsChanged(QString)\0"
    "colsChanged(QString)\0dev\0"
    "refreshDevInfo(SGuiDev)\0row,col,data\0"
    "slotDropEvent(int,int,QString)\0"
};

void form_tvwall_config::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        form_tvwall_config *_t = static_cast<form_tvwall_config *>(_o);
        switch (_id) {
        case 0: _t->signal_tvwall_data((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 1: _t->on_btn_clr_clicked(); break;
        case 2: _t->on_btn_cancel_clicked(); break;
        case 3: _t->on_btn_ok_clicked(); break;
        case 4: _t->btn_unbind_clicked(); break;
        case 5: _t->rowsChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->colsChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->refreshDevInfo((*reinterpret_cast< SGuiDev(*)>(_a[1]))); break;
        case 8: _t->slotDropEvent((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData form_tvwall_config::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject form_tvwall_config::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_form_tvwall_config,
      qt_meta_data_form_tvwall_config, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &form_tvwall_config::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *form_tvwall_config::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *form_tvwall_config::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_form_tvwall_config))
        return static_cast<void*>(const_cast< form_tvwall_config*>(this));
    return QWidget::qt_metacast(_clname);
}

int form_tvwall_config::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void form_tvwall_config::signal_tvwall_data(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
