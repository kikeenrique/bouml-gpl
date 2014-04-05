/****************************************************************************
** DiagramView meta object code from reading C++ file 'DiagramView.h'
**
** Created: Sat Apr 5 23:41:21 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "DiagramView.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *DiagramView::className() const
{
    return "DiagramView";
}

QMetaObject *DiagramView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_DiagramView( "DiagramView", &DiagramView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString DiagramView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DiagramView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString DiagramView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DiagramView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* DiagramView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QCanvasView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "e", &static_QUType_ptr, "QKeyEvent", QUParameter::In }
    };
    static const QUMethod slot_0 = {"keyPressEvent", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_ptr, "QKeyEvent", QUParameter::In }
    };
    static const QUMethod slot_1 = {"keyReleaseEvent", 1, param_slot_1 };
    static const QUMethod slot_2 = {"optimal_window_size", 0, 0 };
    static const QUMethod slot_3 = {"restore_window_size", 0, 0 };
    static const QUMethod slot_4 = {"update", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "keyPressEvent(QKeyEvent*)", &slot_0, QMetaData::Protected },
	{ "keyReleaseEvent(QKeyEvent*)", &slot_1, QMetaData::Private },
	{ "optimal_window_size()", &slot_2, QMetaData::Private },
	{ "restore_window_size()", &slot_3, QMetaData::Private },
	{ "update()", &slot_4, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"DiagramView", parentObject,
	slot_tbl, 5,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_DiagramView.setMetaObject( metaObj );
    return metaObj;
}

void* DiagramView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "DiagramView" ) )
	return this;
    return QCanvasView::qt_cast( clname );
}

bool DiagramView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: keyPressEvent((QKeyEvent*)static_QUType_ptr.get(_o+1)); break;
    case 1: keyReleaseEvent((QKeyEvent*)static_QUType_ptr.get(_o+1)); break;
    case 2: optimal_window_size(); break;
    case 3: restore_window_size(); break;
    case 4: update(); break;
    default:
	return QCanvasView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool DiagramView::qt_emit( int _id, QUObject* _o )
{
    return QCanvasView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool DiagramView::qt_property( int id, int f, QVariant* v)
{
    return QCanvasView::qt_property( id, f, v);
}

bool DiagramView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
