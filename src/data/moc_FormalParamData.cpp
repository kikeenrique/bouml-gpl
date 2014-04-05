/****************************************************************************
** FormalParamData meta object code from reading C++ file 'FormalParamData.h'
**
** Created: Sat Apr 5 23:41:12 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "FormalParamData.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *FormalParamData::className() const
{
    return "FormalParamData";
}

QMetaObject *FormalParamData::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FormalParamData( "FormalParamData", &FormalParamData::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FormalParamData::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FormalParamData", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FormalParamData::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FormalParamData", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FormalParamData::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"on_delete_default", 0, 0 };
    static const QUMethod slot_1 = {"on_delete_extends", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "on_delete_default()", &slot_0, QMetaData::Protected },
	{ "on_delete_extends()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"FormalParamData", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FormalParamData.setMetaObject( metaObj );
    return metaObj;
}

void* FormalParamData::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FormalParamData" ) )
	return this;
    return QObject::qt_cast( clname );
}

bool FormalParamData::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: on_delete_default(); break;
    case 1: on_delete_extends(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool FormalParamData::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool FormalParamData::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool FormalParamData::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
