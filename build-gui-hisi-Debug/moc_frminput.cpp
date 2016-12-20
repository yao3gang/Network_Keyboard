/****************************************************************************
** Meta object code from reading C++ file 'frminput.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gui/input/frminput.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'frminput.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_frmInput[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      30,   10,    9,    9, 0x08,
      62,    9,    9,    9, 0x08,
     119,   76,    9,    9, 0x08,
     164,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_frmInput[] = {
    "frmInput\0\0oldWidget,nowWidget\0"
    "focusChanged(QWidget*,QWidget*)\0"
    "btn_clicked()\0topColor,bottomColor,borderColor,textColor\0"
    "changeStyle(QString,QString,QString,QString)\0"
    "reClicked()\0"
};

void frmInput::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        frmInput *_t = static_cast<frmInput *>(_o);
        switch (_id) {
        case 0: _t->focusChanged((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        case 1: _t->btn_clicked(); break;
        case 2: _t->changeStyle((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 3: _t->reClicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData frmInput::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject frmInput::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_frmInput,
      qt_meta_data_frmInput, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &frmInput::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *frmInput::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *frmInput::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_frmInput))
        return static_cast<void*>(const_cast< frmInput*>(this));
    return QWidget::qt_metacast(_clname);
}

int frmInput::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
