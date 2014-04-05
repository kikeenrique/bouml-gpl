/****************************************************************************
** SdClassInstCanvas meta object code from reading C++ file 'SdClassInstCanvas.h'
**
** Created: Sat Apr 5 23:41:26 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "SdClassInstCanvas.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *SdClassInstCanvas::className() const
{
    return "SdClassInstCanvas";
}

QMetaObject *SdClassInstCanvas::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SdClassInstCanvas( "SdClassInstCanvas", &SdClassInstCanvas::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SdClassInstCanvas::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SdClassInstCanvas", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SdClassInstCanvas::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SdClassInstCanvas", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SdClassInstCanvas::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"modified", 0, 0 };
    static const QUMethod slot_1 = {"deleted", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "modified()", &slot_0, QMetaData::Private },
	{ "deleted()", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"SdClassInstCanvas", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SdClassInstCanvas.setMetaObject( metaObj );
    return metaObj;
}

void* SdClassInstCanvas::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SdClassInstCanvas" ) )
	return this;
    if ( !qstrcmp( clname, "SdObjCanvas" ) )
	return (SdObjCanvas*)this;
    if ( !qstrcmp( clname, "ClassInstCanvas" ) )
	return (ClassInstCanvas*)this;
    return QObject::qt_cast( clname );
}

bool SdClassInstCanvas::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: modified(); break;
    case 1: deleted(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool SdClassInstCanvas::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool SdClassInstCanvas::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool SdClassInstCanvas::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
