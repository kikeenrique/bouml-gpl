/****************************************************************************
** ParameterData meta object code from reading C++ file 'ParameterData.h'
**
** Created: Sat Apr 5 23:41:15 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ParameterData.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ParameterData::className() const
{
    return "ParameterData";
}

QMetaObject *ParameterData::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ParameterData( "ParameterData", &ParameterData::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ParameterData::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ParameterData", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ParameterData::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ParameterData", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ParameterData::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = SimpleData::staticMetaObject();
    static const QUMethod slot_0 = {"on_delete", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "on_delete()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ParameterData", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ParameterData.setMetaObject( metaObj );
    return metaObj;
}

void* ParameterData::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ParameterData" ) )
	return this;
    if ( !qstrcmp( clname, "PinParamData" ) )
	return (PinParamData*)this;
    return SimpleData::qt_cast( clname );
}

bool ParameterData::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: on_delete(); break;
    default:
	return SimpleData::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ParameterData::qt_emit( int _id, QUObject* _o )
{
    return SimpleData::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ParameterData::qt_property( int id, int f, QVariant* v)
{
    return SimpleData::qt_property( id, f, v);
}

bool ParameterData::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
