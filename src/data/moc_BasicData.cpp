/****************************************************************************
** BasicData meta object code from reading C++ file 'BasicData.h'
**
** Created: Sat Apr 5 23:41:11 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "BasicData.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *BasicData::className() const
{
    return "BasicData";
}

QMetaObject *BasicData::metaObj = 0;
static QMetaObjectCleanUp cleanUp_BasicData( "BasicData", &BasicData::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString BasicData::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BasicData", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString BasicData::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BasicData", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* BasicData::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"on_delete", 0, 0 };
    static const QUMethod slot_1 = {"modified", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "on_delete()", &slot_0, QMetaData::Protected },
	{ "modified()", &slot_1, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"changed", 0, 0 };
    static const QUMethod signal_1 = {"deleted", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "changed()", &signal_0, QMetaData::Public },
	{ "deleted()", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"BasicData", parentObject,
	slot_tbl, 2,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_BasicData.setMetaObject( metaObj );
    return metaObj;
}

void* BasicData::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "BasicData" ) )
	return this;
    return QObject::qt_cast( clname );
}

// SIGNAL changed
void BasicData::changed()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL deleted
void BasicData::deleted()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

bool BasicData::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: on_delete(); break;
    case 1: modified(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool BasicData::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: changed(); break;
    case 1: deleted(); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool BasicData::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool BasicData::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
