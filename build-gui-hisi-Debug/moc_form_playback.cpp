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
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   15,   14,   14, 0x05,
      58,   53,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      91,   87,   14,   14, 0x08,
     128,  115,   14,   14, 0x08,
     175,  170,   14,   14, 0x08,
     213,  170,   14,   14, 0x08,
     259,  257,   14,   14, 0x08,
     281,   14,   14,   14, 0x08,
     302,   14,   14,   14, 0x08,
     330,   14,   14,   14, 0x08,
     356,   14,   14,   14, 0x08,
     383,   14,   14,   14, 0x08,
     409,   14,   14,   14, 0x08,
     433,   14,   14,   14, 0x08,
     457,   14,   14,   14, 0x08,
     479,   14,   14,   14, 0x08,
     501,   14,   14,   14, 0x08,
     529,  523,   14,   14, 0x08,
     552,  548,   14,   14, 0x08,
     572,  548,   14,   14, 0x08,
     590,  548,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_form_playback[] = {
    "form_playback\0\0value\0"
    "signalFileDownloadProgress(int)\0flag\0"
    "signalFileDownloadClose(int)\0dev\0"
    "refreshDevInfo(SGuiDev)\0playback_msg\0"
    "slotNotifyPlaybackInfo(SPlaybackNotify_t)\0"
    "item\0tableWidgetClicked(QTableWidgetItem*)\0"
    "tableWidgetDoubleClicked(QTableWidgetItem*)\0"
    "b\0showTableWidget(bool)\0on_btn_srh_clicked()\0"
    "on_btn_page_start_clicked()\0"
    "on_btn_page_pre_clicked()\0"
    "on_btn_page_next_clicked()\0"
    "on_btn_page_end_clicked()\0"
    "on_btn_to_dec_clicked()\0on_btn_backup_clicked()\0"
    "on_btn_stop_clicked()\0on_btn_play_clicked()\0"
    "on_btn_step_clicked()\0index\0"
    "set_play_rate(int)\0pos\0slider_pressed(int)\0"
    "slider_moved(int)\0slider_released(int)\0"
};

void form_playback::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        form_playback *_t = static_cast<form_playback *>(_o);
        switch (_id) {
        case 0: _t->signalFileDownloadProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->signalFileDownloadClose((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->refreshDevInfo((*reinterpret_cast< SGuiDev(*)>(_a[1]))); break;
        case 3: _t->slotNotifyPlaybackInfo((*reinterpret_cast< SPlaybackNotify_t(*)>(_a[1]))); break;
        case 4: _t->tableWidgetClicked((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 5: _t->tableWidgetDoubleClicked((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 6: _t->showTableWidget((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->on_btn_srh_clicked(); break;
        case 8: _t->on_btn_page_start_clicked(); break;
        case 9: _t->on_btn_page_pre_clicked(); break;
        case 10: _t->on_btn_page_next_clicked(); break;
        case 11: _t->on_btn_page_end_clicked(); break;
        case 12: _t->on_btn_to_dec_clicked(); break;
        case 13: _t->on_btn_backup_clicked(); break;
        case 14: _t->on_btn_stop_clicked(); break;
        case 15: _t->on_btn_play_clicked(); break;
        case 16: _t->on_btn_step_clicked(); break;
        case 17: _t->set_play_rate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->slider_pressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->slider_moved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->slider_released((*reinterpret_cast< int(*)>(_a[1]))); break;
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
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void form_playback::signalFileDownloadProgress(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void form_playback::signalFileDownloadClose(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
