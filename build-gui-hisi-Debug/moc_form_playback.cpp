/****************************************************************************
** Meta object code from reading C++ file 'form_playback.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gui/page/form_playback.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'form_playback.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_form_playback[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   15,   14,   14, 0x08,
      48,   43,   14,   14, 0x08,
      86,   43,   14,   14, 0x08,
     132,  130,   14,   14, 0x08,
     154,   14,   14,   14, 0x08,
     175,   14,   14,   14, 0x08,
     203,   14,   14,   14, 0x08,
     229,   14,   14,   14, 0x08,
     256,   14,   14,   14, 0x08,
     282,   14,   14,   14, 0x08,
     306,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_form_playback[] = {
    "form_playback\0\0dev\0refreshDevInfo(SGuiDev)\0"
    "item\0tableWidgetClicked(QTableWidgetItem*)\0"
    "tableWidgetDoubleClicked(QTableWidgetItem*)\0"
    "b\0showTableWidget(bool)\0on_btn_srh_clicked()\0"
    "on_btn_page_start_clicked()\0"
    "on_btn_page_pre_clicked()\0"
    "on_btn_page_next_clicked()\0"
    "on_btn_page_end_clicked()\0"
    "on_btn_to_dec_clicked()\0on_btn_stop_clicked()\0"
};

void form_playback::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        form_playback *_t = static_cast<form_playback *>(_o);
        switch (_id) {
        case 0: _t->refreshDevInfo((*reinterpret_cast< SGuiDev(*)>(_a[1]))); break;
        case 1: _t->tableWidgetClicked((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 2: _t->tableWidgetDoubleClicked((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->showTableWidget((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_btn_srh_clicked(); break;
        case 5: _t->on_btn_page_start_clicked(); break;
        case 6: _t->on_btn_page_pre_clicked(); break;
        case 7: _t->on_btn_page_next_clicked(); break;
        case 8: _t->on_btn_page_end_clicked(); break;
        case 9: _t->on_btn_to_dec_clicked(); break;
        case 10: _t->on_btn_stop_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData form_playback::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject form_playback::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_form_playback,
      qt_meta_data_form_playback, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &form_playback::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *form_playback::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *form_playback::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_form_playback))
        return static_cast<void*>(const_cast< form_playback*>(this));
    return QWidget::qt_metacast(_clname);
}

int form_playback::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
