/****************************************************************************
** IconCanvas meta object code from reading C++ file 'IconCanvas.h'
**
** Created: Sat Apr 5 23:41:28 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "IconCanvas.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *IconCanvas::className() const
{
    return "IconCanvas";
}

QMetaObject *IconCanvas::metaObj = 0;
static QMetaObjectCleanUp cleanUp_IconCanvas( "IconCanvas", &IconCanvas::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString IconCanvas::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "IconCanvas", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString IconCanvas::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "IconCanvas", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* IconCanvas::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"deleted", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "deleted()", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"IconCanvas", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_IconCanvas.setMetaObject( metaObj );
    return metaObj;
}

void* IconCanvas::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "IconCanvas" ) )
	return this;
    if ( !qstrcmp( clname, "DiagramCanvas" ) )
	return (DiagramCanvas*)this;
    return QObject::qt_cast( clname );
}

bool IconCanvas::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: deleted(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool IconCanvas::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool IconCanvas::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool IconCanvas::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
