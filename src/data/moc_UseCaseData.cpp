/****************************************************************************
** UseCaseData meta object code from reading C++ file 'UseCaseData.h'
**
** Created: Sat Apr 5 23:41:18 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "UseCaseData.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *UseCaseData::className() const
{
    return "UseCaseData";
}

QMetaObject *UseCaseData::metaObj = 0;
static QMetaObjectCleanUp cleanUp_UseCaseData( "UseCaseData", &UseCaseData::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString UseCaseData::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UseCaseData", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString UseCaseData::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UseCaseData", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* UseCaseData::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = SimpleData::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"UseCaseData", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_UseCaseData.setMetaObject( metaObj );
    return metaObj;
}

void* UseCaseData::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "UseCaseData" ) )
	return this;
    return SimpleData::qt_cast( clname );
}

bool UseCaseData::qt_invoke( int _id, QUObject* _o )
{
    return SimpleData::qt_invoke(_id,_o);
}

bool UseCaseData::qt_emit( int _id, QUObject* _o )
{
    return SimpleData::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool UseCaseData::qt_property( int id, int f, QVariant* v)
{
    return SimpleData::qt_property( id, f, v);
}

bool UseCaseData::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
