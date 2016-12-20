/****************************************************************************
** Meta object code from reading C++ file 'page_config.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gui/page/page_config.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'page_config.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_page_config[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      36,   30,   12,   12, 0x08,
      55,   30,   12,   12, 0x08,
      74,   12,   12,   12, 0x08,
     100,   12,   12,   12, 0x08,
     128,   12,   12,   12, 0x08,
     150,   12,   12,   12, 0x08,
     175,   12,   12,   12, 0x08,
     202,   12,   12,   12, 0x08,
     230,   12,   12,   12, 0x08,
     258,   12,   12,   12, 0x08,
     294,  289,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_page_config[] = {
    "page_config\0\0button_clicked()\0index\0"
    "setDateFormat(int)\0setTimeFormat(int)\0"
    "on_btn_date_def_clicked()\0"
    "on_btn_date_apply_clicked()\0"
    "on_btn_sync_clicked()\0on_btn_net_def_clicked()\0"
    "on_btn_net_apply_clicked()\0"
    "on_btn_tvWall_add_clicked()\0"
    "on_btn_tvWall_del_clicked()\0"
    "on_btn_tvWall_modify_clicked()\0data\0"
    "accept_tvwall_data(QByteArray)\0"
};

void page_config::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        page_config *_t = static_cast<page_config *>(_o);
        switch (_id) {
        case 0: _t->button_clicked(); break;
        case 1: _t->setDateFormat((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setTimeFormat((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_btn_date_def_clicked(); break;
        case 4: _t->on_btn_date_apply_clicked(); break;
        case 5: _t->on_btn_sync_clicked(); break;
        case 6: _t->on_btn_net_def_clicked(); break;
        case 7: _t->on_btn_net_apply_clicked(); break;
        case 8: _t->on_btn_tvWall_add_clicked(); break;
        case 9: _t->on_btn_tvWall_del_clicked(); break;
        case 10: _t->on_btn_tvWall_modify_clicked(); break;
        case 11: _t->accept_tvwall_data((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData page_config::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject page_config::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_page_config,
      qt_meta_data_page_config, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &page_config::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *page_config::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *page_config::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_page_config))
        return static_cast<void*>(const_cast< page_config*>(this));
    return QWidget::qt_metacast(_clname);
}

int page_config::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
