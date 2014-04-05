/****************************************************************************
** RelationData meta object code from reading C++ file 'RelationData.h'
**
** Created: Sat Apr 5 23:41:13 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "RelationData.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *RelationData::className() const
{
    return "RelationData";
}

QMetaObject *RelationData::metaObj = 0;
static QMetaObjectCleanUp cleanUp_RelationData( "RelationData", &RelationData::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString RelationData::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RelationData", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString RelationData::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RelationData", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* RelationData::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = ClassMemberData::staticMetaObject();
    static const QUMethod slot_0 = {"end_deleted", 0, 0 };
    static const QUMethod slot_1 = {"on_delete", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "end_deleted()", &slot_0, QMetaData::Protected },
	{ "on_delete()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"RelationData", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_RelationData.setMetaObject( metaObj );
    return metaObj;
}

void* RelationData::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "RelationData" ) )
	return this;
    if ( !qstrcmp( clname, "Labeled<RelationData>" ) )
	return (Labeled<RelationData>*)this;
    return ClassMemberData::qt_cast( clname );
}

bool RelationData::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: end_deleted(); break;
    case 1: on_delete(); break;
    default:
	return ClassMemberData::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool RelationData::qt_emit( int _id, QUObject* _o )
{
    return ClassMemberData::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool RelationData::qt_property( int id, int f, QVariant* v)
{
    return ClassMemberData::qt_property( id, f, v);
}

bool RelationData::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
