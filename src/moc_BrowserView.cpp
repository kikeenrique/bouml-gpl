/****************************************************************************
** Meta object code from reading C++ file 'BrowserView.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "diagram/BrowserView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BrowserView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BrowserView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x09,
      39,   12,   12,   12, 0x09,
      69,   12,   12,   12, 0x09,
      98,   12,   12,   12, 0x09,
     107,  105,   12,   12, 0x09,
     146,  105,   12,   12, 0x09,
     177,  105,   12,   12, 0x09,
     214,  105,   12,   12, 0x09,
     252,  105,   12,   12, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_BrowserView[] = {
    "BrowserView\0\0selected(Q3ListViewItem*)\0"
    "rightPressed(Q3ListViewItem*)\0"
    "doubleClick(Q3ListViewItem*)\0menu()\0"
    "e\0contentsDragMoveEvent(QDragMoveEvent*)\0"
    "contentsDropEvent(QDropEvent*)\0"
    "contentsMouseMoveEvent(QMouseEvent*)\0"
    "contentsMousePressEvent(QMouseEvent*)\0"
    "contentsMouseReleaseEvent(QMouseEvent*)\0"
};

void BrowserView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BrowserView *_t = static_cast<BrowserView *>(_o);
        switch (_id) {
        case 0: _t->selected((*reinterpret_cast< Q3ListViewItem*(*)>(_a[1]))); break;
        case 1: _t->rightPressed((*reinterpret_cast< Q3ListViewItem*(*)>(_a[1]))); break;
        case 2: _t->doubleClick((*reinterpret_cast< Q3ListViewItem*(*)>(_a[1]))); break;
        case 3: _t->menu(); break;
        case 4: _t->contentsDragMoveEvent((*reinterpret_cast< QDragMoveEvent*(*)>(_a[1]))); break;
        case 5: _t->contentsDropEvent((*reinterpret_cast< QDropEvent*(*)>(_a[1]))); break;
        case 6: _t->contentsMouseMoveEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 7: _t->contentsMousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 8: _t->contentsMouseReleaseEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BrowserView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BrowserView::staticMetaObject = {
    { &Q3ListView::staticMetaObject, qt_meta_stringdata_BrowserView,
      qt_meta_data_BrowserView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BrowserView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BrowserView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BrowserView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BrowserView))
        return static_cast<void*>(const_cast< BrowserView*>(this));
    return Q3ListView::qt_metacast(_clname);
}

int BrowserView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Q3ListView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
