/****************************************************************************
** OperationData meta object code from reading C++ file 'OperationData.h'
**
** Created: Sat Apr 5 23:41:13 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "OperationData.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *OperationData::className() const
{
    return "OperationData";
}

QMetaObject *OperationData::metaObj = 0;
static QMetaObjectCleanUp cleanUp_OperationData( "OperationData", &OperationData::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString OperationData::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "OperationData", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString OperationData::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "OperationData", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* OperationData::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = ClassMemberData::staticMetaObject();
    static const QUMethod slot_0 = {"on_delete", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "on_delete()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"OperationData", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_OperationData.setMetaObject( metaObj );
    return metaObj;
}

void* OperationData::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "OperationData" ) )
	return this;
    if ( !qstrcmp( clname, "Labeled<OperationData>" ) )
	return (Labeled<OperationData>*)this;
    if ( !qstrcmp( clname, "MultipleDependency<BasicData>" ) )
	return (MultipleDependency<BasicData>*)this;
    return ClassMemberData::qt_cast( clname );
}

bool OperationData::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: on_delete(); break;
    default:
	return ClassMemberData::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool OperationData::qt_emit( int _id, QUObject* _o )
{
    return ClassMemberData::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool OperationData::qt_property( int id, int f, QVariant* v)
{
    return ClassMemberData::qt_property( id, f, v);
}

bool OperationData::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
