/****************************************************************************
** StringTable meta object code from reading C++ file 'StringTable.h'
**
** Created: Sat Apr 5 23:41:56 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "StringTable.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *StringTable::className() const
{
    return "StringTable";
}

QMetaObject *StringTable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_StringTable( "StringTable", &StringTable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString StringTable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StringTable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString StringTable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StringTable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* StringTable::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = MyTable::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In },
	{ "button", &static_QUType_int, 0, QUParameter::In },
	{ "mousePos", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_0 = {"button_pressed", 4, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"value_changed", 2, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "button_pressed(int,int,int,const QPoint&)", &slot_0, QMetaData::Protected },
	{ "value_changed(int,int)", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"StringTable", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_StringTable.setMetaObject( metaObj );
    return metaObj;
}

void* StringTable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "StringTable" ) )
	return this;
    return MyTable::qt_cast( clname );
}

bool StringTable::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: button_pressed((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+4))); break;
    case 1: value_changed((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return MyTable::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool StringTable::qt_emit( int _id, QUObject* _o )
{
    return MyTable::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool StringTable::qt_property( int id, int f, QVariant* v)
{
    return MyTable::qt_property( id, f, v);
}

bool StringTable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
