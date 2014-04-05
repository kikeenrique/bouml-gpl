/****************************************************************************
** BrowserView meta object code from reading C++ file 'BrowserView.h'
**
** Created: Sat Apr 5 23:41:19 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "BrowserView.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *BrowserView::className() const
{
    return "BrowserView";
}

QMetaObject *BrowserView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_BrowserView( "BrowserView", &BrowserView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString BrowserView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BrowserView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString BrowserView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BrowserView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* BrowserView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QListView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_0 = {"selected", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_1 = {"rightPressed", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_2 = {"doubleClick", 1, param_slot_2 };
    static const QUMethod slot_3 = {"menu", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "e", &static_QUType_ptr, "QDragMoveEvent", QUParameter::In }
    };
    static const QUMethod slot_4 = {"contentsDragMoveEvent", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "e", &static_QUType_ptr, "QDropEvent", QUParameter::In }
    };
    static const QUMethod slot_5 = {"contentsDropEvent", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "e", &static_QUType_ptr, "QMouseEvent", QUParameter::In }
    };
    static const QUMethod slot_6 = {"contentsMouseMoveEvent", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "e", &static_QUType_ptr, "QMouseEvent", QUParameter::In }
    };
    static const QUMethod slot_7 = {"contentsMousePressEvent", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ "e", &static_QUType_ptr, "QMouseEvent", QUParameter::In }
    };
    static const QUMethod slot_8 = {"contentsMouseReleaseEvent", 1, param_slot_8 };
    static const QMetaData slot_tbl[] = {
	{ "selected(QListViewItem*)", &slot_0, QMetaData::Protected },
	{ "rightPressed(QListViewItem*)", &slot_1, QMetaData::Protected },
	{ "doubleClick(QListViewItem*)", &slot_2, QMetaData::Protected },
	{ "menu()", &slot_3, QMetaData::Protected },
	{ "contentsDragMoveEvent(QDragMoveEvent*)", &slot_4, QMetaData::Protected },
	{ "contentsDropEvent(QDropEvent*)", &slot_5, QMetaData::Protected },
	{ "contentsMouseMoveEvent(QMouseEvent*)", &slot_6, QMetaData::Protected },
	{ "contentsMousePressEvent(QMouseEvent*)", &slot_7, QMetaData::Protected },
	{ "contentsMouseReleaseEvent(QMouseEvent*)", &slot_8, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"BrowserView", parentObject,
	slot_tbl, 9,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_BrowserView.setMetaObject( metaObj );
    return metaObj;
}

void* BrowserView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "BrowserView" ) )
	return this;
    return QListView::qt_cast( clname );
}

bool BrowserView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: selected((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 1: rightPressed((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 2: doubleClick((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 3: menu(); break;
    case 4: contentsDragMoveEvent((QDragMoveEvent*)static_QUType_ptr.get(_o+1)); break;
    case 5: contentsDropEvent((QDropEvent*)static_QUType_ptr.get(_o+1)); break;
    case 6: contentsMouseMoveEvent((QMouseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 7: contentsMousePressEvent((QMouseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 8: contentsMouseReleaseEvent((QMouseEvent*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool BrowserView::qt_emit( int _id, QUObject* _o )
{
    return QListView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool BrowserView::qt_property( int id, int f, QVariant* v)
{
    return QListView::qt_property( id, f, v);
}

bool BrowserView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
