/****************************************************************************
** Socket meta object code from reading C++ file 'Socket.h'
**
** Created: Sat Apr 5 23:42:07 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "Socket.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Socket::className() const
{
    return "Socket";
}

QMetaObject *Socket::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Socket( "Socket", &Socket::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Socket::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Socket", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Socket::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Socket", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Socket::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"data_received", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "data_received()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"Socket", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Socket.setMetaObject( metaObj );
    return metaObj;
}

void* Socket::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Socket" ) )
	return this;
    if ( !qstrcmp( clname, "QSocketDevice" ) )
	return (QSocketDevice*)this;
    return QObject::qt_cast( clname );
}

bool Socket::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: data_received(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Socket::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Socket::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool Socket::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
