/****************************************************************************
** ActualParamData meta object code from reading C++ file 'ActualParamData.h'
**
** Created: Sat Apr 5 23:41:10 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ActualParamData.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ActualParamData::className() const
{
    return "ActualParamData";
}

QMetaObject *ActualParamData::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ActualParamData( "ActualParamData", &ActualParamData::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ActualParamData::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ActualParamData", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ActualParamData::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ActualParamData", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ActualParamData::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"on_delete", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "on_delete()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ActualParamData", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ActualParamData.setMetaObject( metaObj );
    return metaObj;
}

void* ActualParamData::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ActualParamData" ) )
	return this;
    return QObject::qt_cast( clname );
}

bool ActualParamData::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: on_delete(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ActualParamData::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ActualParamData::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool ActualParamData::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
