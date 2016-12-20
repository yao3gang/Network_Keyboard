/****************************************************************************
** Meta object code from reading C++ file 'page_dev_mgt.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gui/page/page_dev_mgt.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'page_dev_mgt.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_page_dev_mgt[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      47,   13,   13,   13, 0x08,
      68,   13,   13,   13, 0x08,
      89,   13,   13,   13, 0x08,
     111,   13,   13,   13, 0x08,
     138,  132,   13,   13, 0x08,
     156,  132,   13,   13, 0x08,
     185,  174,   13,   13, 0x08,
     218,  174,   13,   13, 0x08,
     251,   14,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_page_dev_mgt[] = {
    "page_dev_mgt\0\0dev\0signalDevInfoChange(SGuiDev)\0"
    "on_btn_srh_clicked()\0on_btn_add_clicked()\0"
    "on_btn_info_clicked()\0on_btn_del_clicked()\0"
    "index\0cmbSrhChange(int)\0cmbAddChange(int)\0"
    "row,column\0tableWidgetSrhDBClicked(int,int)\0"
    "tableWidgetDevDBClicked(int,int)\0"
    "refreshDevInfo(SGuiDev)\0"
};

void page_dev_mgt::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        page_dev_mgt *_t = static_cast<page_dev_mgt *>(_o);
        switch (_id) {
        case 0: _t->signalDevInfoChange((*reinterpret_cast< SGuiDev(*)>(_a[1]))); break;
        case 1: _t->on_btn_srh_clicked(); break;
        case 2: _t->on_btn_add_clicked(); break;
        case 3: _t->on_btn_info_clicked(); break;
        case 4: _t->on_btn_del_clicked(); break;
        case 5: _t->cmbSrhChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->cmbAddChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->tableWidgetSrhDBClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->tableWidgetDevDBClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->refreshDevInfo((*reinterpret_cast< SGuiDev(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData page_dev_mgt::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject page_dev_mgt::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_page_dev_mgt,
      qt_meta_data_page_dev_mgt, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &page_dev_mgt::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *page_dev_mgt::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *page_dev_mgt::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_page_dev_mgt))
        return static_cast<void*>(const_cast< page_dev_mgt*>(this));
    return QWidget::qt_metacast(_clname);
}

int page_dev_mgt::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void page_dev_mgt::signalDevInfoChange(SGuiDev _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
