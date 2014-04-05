/****************************************************************************
** ArrowCanvas meta object code from reading C++ file 'ArrowCanvas.h'
**
** Created: Sat Apr 5 23:41:19 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ArrowCanvas.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ArrowCanvas::className() const
{
    return "ArrowCanvas";
}

QMetaObject *ArrowCanvas::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ArrowCanvas( "ArrowCanvas", &ArrowCanvas::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ArrowCanvas::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ArrowCanvas", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ArrowCanvas::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ArrowCanvas", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ArrowCanvas::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"modified", 0, 0 };
    static const QUMethod slot_1 = {"drawing_settings_modified", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "modified()", &slot_0, QMetaData::Protected },
	{ "drawing_settings_modified()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ArrowCanvas", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ArrowCanvas.setMetaObject( metaObj );
    return metaObj;
}

void* ArrowCanvas::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ArrowCanvas" ) )
	return this;
    if ( !qstrcmp( clname, "QCanvasPolygon" ) )
	return (QCanvasPolygon*)this;
    if ( !qstrcmp( clname, "DiagramItem" ) )
	return (DiagramItem*)this;
    return QObject::qt_cast( clname );
}

bool ArrowCanvas::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: modified(); break;
    case 1: drawing_settings_modified(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ArrowCanvas::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ArrowCanvas::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool ArrowCanvas::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
