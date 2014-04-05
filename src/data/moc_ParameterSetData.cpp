/****************************************************************************
** ParameterSetData meta object code from reading C++ file 'ParameterSetData.h'
**
** Created: Sat Apr 5 23:41:16 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ParameterSetData.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ParameterSetData::className() const
{
    return "ParameterSetData";
}

QMetaObject *ParameterSetData::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ParameterSetData( "ParameterSetData", &ParameterSetData::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ParameterSetData::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ParameterSetData", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ParameterSetData::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ParameterSetData", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ParameterSetData::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = SimpleData::staticMetaObject();
    static const QUMethod slot_0 = {"check", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "check()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ParameterSetData", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ParameterSetData.setMetaObject( metaObj );
    return metaObj;
}

void* ParameterSetData::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ParameterSetData" ) )
	return this;
    return SimpleData::qt_cast( clname );
}

bool ParameterSetData::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: check(); break;
    default:
	return SimpleData::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ParameterSetData::qt_emit( int _id, QUObject* _o )
{
    return SimpleData::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ParameterSetData::qt_property( int id, int f, QVariant* v)
{
    return SimpleData::qt_property( id, f, v);
}

bool ParameterSetData::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
