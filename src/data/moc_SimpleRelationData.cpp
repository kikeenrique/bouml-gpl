/****************************************************************************
** SimpleRelationData meta object code from reading C++ file 'SimpleRelationData.h'
**
** Created: Sat Apr 5 23:41:13 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "SimpleRelationData.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *SimpleRelationData::className() const
{
    return "SimpleRelationData";
}

QMetaObject *SimpleRelationData::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SimpleRelationData( "SimpleRelationData", &SimpleRelationData::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SimpleRelationData::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SimpleRelationData", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SimpleRelationData::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SimpleRelationData", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SimpleRelationData::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = SimpleData::staticMetaObject();
    static const QUMethod slot_0 = {"end_deleted", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "end_deleted()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"SimpleRelationData", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SimpleRelationData.setMetaObject( metaObj );
    return metaObj;
}

void* SimpleRelationData::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SimpleRelationData" ) )
	return this;
    return SimpleData::qt_cast( clname );
}

bool SimpleRelationData::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: end_deleted(); break;
    default:
	return SimpleData::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool SimpleRelationData::qt_emit( int _id, QUObject* _o )
{
    return SimpleData::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool SimpleRelationData::qt_property( int id, int f, QVariant* v)
{
    return SimpleData::qt_property( id, f, v);
}

bool SimpleRelationData::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
