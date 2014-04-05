/****************************************************************************
** ClassInstanceData meta object code from reading C++ file 'ClassInstanceData.h'
**
** Created: Sat Apr 5 23:41:18 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ClassInstanceData.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ClassInstanceData::className() const
{
    return "ClassInstanceData";
}

QMetaObject *ClassInstanceData::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ClassInstanceData( "ClassInstanceData", &ClassInstanceData::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ClassInstanceData::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ClassInstanceData", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ClassInstanceData::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ClassInstanceData", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ClassInstanceData::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = SimpleData::staticMetaObject();
    static const QUMethod slot_0 = {"check", 0, 0 };
    static const QUMethod slot_1 = {"check_rels", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "check()", &slot_0, QMetaData::Public },
	{ "check_rels()", &slot_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ClassInstanceData", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ClassInstanceData.setMetaObject( metaObj );
    return metaObj;
}

void* ClassInstanceData::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ClassInstanceData" ) )
	return this;
    return SimpleData::qt_cast( clname );
}

bool ClassInstanceData::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: check(); break;
    case 1: check_rels(); break;
    default:
	return SimpleData::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ClassInstanceData::qt_emit( int _id, QUObject* _o )
{
    return SimpleData::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ClassInstanceData::qt_property( int id, int f, QVariant* v)
{
    return SimpleData::qt_property( id, f, v);
}

bool ClassInstanceData::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
