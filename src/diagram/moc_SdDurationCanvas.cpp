/****************************************************************************
** SdDurationCanvas meta object code from reading C++ file 'SdDurationCanvas.h'
**
** Created: Sat Apr 5 23:41:26 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "SdDurationCanvas.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *SdDurationCanvas::className() const
{
    return "SdDurationCanvas";
}

QMetaObject *SdDurationCanvas::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SdDurationCanvas( "SdDurationCanvas", &SdDurationCanvas::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SdDurationCanvas::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SdDurationCanvas", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SdDurationCanvas::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SdDurationCanvas", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SdDurationCanvas::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"modified", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "modified()", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"SdDurationCanvas", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SdDurationCanvas.setMetaObject( metaObj );
    return metaObj;
}

void* SdDurationCanvas::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SdDurationCanvas" ) )
	return this;
    if ( !qstrcmp( clname, "SdMsgSupport" ) )
	return (SdMsgSupport*)this;
    if ( !qstrcmp( clname, "SdDurationSupport" ) )
	return (SdDurationSupport*)this;
    return QObject::qt_cast( clname );
}

bool SdDurationCanvas::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: modified(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool SdDurationCanvas::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool SdDurationCanvas::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool SdDurationCanvas::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
