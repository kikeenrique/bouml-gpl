/****************************************************************************
** MyTable meta object code from reading C++ file 'MyTable.h'
**
** Created: Sat Apr 5 23:41:48 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MyTable.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MyTable::className() const
{
    return "MyTable";
}

QMetaObject *MyTable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MyTable( "MyTable", &MyTable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MyTable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MyTable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MyTable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MyTable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MyTable::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTable::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"MyTable", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MyTable.setMetaObject( metaObj );
    return metaObj;
}

void* MyTable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MyTable" ) )
	return this;
    return QTable::qt_cast( clname );
}

bool MyTable::qt_invoke( int _id, QUObject* _o )
{
    return QTable::qt_invoke(_id,_o);
}

bool MyTable::qt_emit( int _id, QUObject* _o )
{
    return QTable::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MyTable::qt_property( int id, int f, QVariant* v)
{
    return QTable::qt_property( id, f, v);
}

bool MyTable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
