/****************************************************************************
** ClassMemberData meta object code from reading C++ file 'ClassMemberData.h'
**
** Created: Sat Apr 5 23:41:11 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ClassMemberData.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ClassMemberData::className() const
{
    return "ClassMemberData";
}

QMetaObject *ClassMemberData::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ClassMemberData( "ClassMemberData", &ClassMemberData::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ClassMemberData::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ClassMemberData", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ClassMemberData::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ClassMemberData", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ClassMemberData::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = BasicData::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"ClassMemberData", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ClassMemberData.setMetaObject( metaObj );
    return metaObj;
}

void* ClassMemberData::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ClassMemberData" ) )
	return this;
    return BasicData::qt_cast( clname );
}

bool ClassMemberData::qt_invoke( int _id, QUObject* _o )
{
    return BasicData::qt_invoke(_id,_o);
}

bool ClassMemberData::qt_emit( int _id, QUObject* _o )
{
    return BasicData::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ClassMemberData::qt_property( int id, int f, QVariant* v)
{
    return BasicData::qt_property( id, f, v);
}

bool ClassMemberData::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
