/****************************************************************************
** Meta object code from reading C++ file 'wzd_tvwall_layout.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gui/page/wzd_tvwall_layout.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wzd_tvwall_layout.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wzd_tvWall_layout[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_wzd_tvWall_layout[] = {
    "wzd_tvWall_layout\0"
};

void wzd_tvWall_layout::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData wzd_tvWall_layout::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject wzd_tvWall_layout::staticMetaObject = {
    { &QWizard::staticMetaObject, qt_meta_stringdata_wzd_tvWall_layout,
      qt_meta_data_wzd_tvWall_layout, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wzd_tvWall_layout::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wzd_tvWall_layout::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wzd_tvWall_layout::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wzd_tvWall_layout))
        return static_cast<void*>(const_cast< wzd_tvWall_layout*>(this));
    return QWizard::qt_metacast(_clname);
}

int wzd_tvWall_layout::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizard::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
