/****************************************************************************
** ClassData meta object code from reading C++ file 'ClassData.h'
**
** Created: Sat Apr 5 23:41:12 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ClassData.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ClassData::className() const
{
    return "ClassData";
}

QMetaObject *ClassData::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ClassData( "ClassData", &ClassData::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ClassData::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ClassData", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ClassData::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ClassData", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ClassData::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = BasicData::staticMetaObject();
    static const QUMethod slot_0 = {"update_actuals", 0, 0 };
    static const QUMethod slot_1 = {"on_delete", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "update_actuals()", &slot_0, QMetaData::Private },
	{ "on_delete()", &slot_1, QMetaData::Private }
    };
    static const QUMethod signal_0 = {"actuals_changed", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "actuals_changed()", &signal_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"ClassData", parentObject,
	slot_tbl, 2,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ClassData.setMetaObject( metaObj );
    return metaObj;
}

void* ClassData::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ClassData" ) )
	return this;
    return BasicData::qt_cast( clname );
}

// SIGNAL actuals_changed
void ClassData::actuals_changed()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool ClassData::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: update_actuals(); break;
    case 1: on_delete(); break;
    default:
	return BasicData::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ClassData::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: actuals_changed(); break;
    default:
	return BasicData::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ClassData::qt_property( int id, int f, QVariant* v)
{
    return BasicData::qt_property( id, f, v);
}

bool ClassData::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
