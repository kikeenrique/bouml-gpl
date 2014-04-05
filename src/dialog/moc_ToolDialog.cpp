/****************************************************************************
** ToolDialog meta object code from reading C++ file 'ToolDialog.h'
**
** Created: Sat Apr 5 23:41:56 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ToolDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ToolDialog::className() const
{
    return "ToolDialog";
}

QMetaObject *ToolDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ToolDialog( "ToolDialog", &ToolDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ToolDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ToolDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ToolDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ToolDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ToolDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"accept", 0, 0 };
    static const QUMethod slot_1 = {"polish", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "accept()", &slot_0, QMetaData::Protected },
	{ "polish()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ToolDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ToolDialog.setMetaObject( metaObj );
    return metaObj;
}

void* ToolDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ToolDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool ToolDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: accept(); break;
    case 1: polish(); break;
    default:
	return QTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ToolDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ToolDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool ToolDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *ToolTable::className() const
{
    return "ToolTable";
}

QMetaObject *ToolTable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ToolTable( "ToolTable", &ToolTable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ToolTable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ToolTable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ToolTable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ToolTable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ToolTable::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = StringTable::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In },
	{ "button", &static_QUType_int, 0, QUParameter::In },
	{ "mousePos", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_0 = {"button_pressed", 4, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "button_pressed(int,int,int,const QPoint&)", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ToolTable", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ToolTable.setMetaObject( metaObj );
    return metaObj;
}

void* ToolTable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ToolTable" ) )
	return this;
    return StringTable::qt_cast( clname );
}

bool ToolTable::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: button_pressed((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+4))); break;
    default:
	return StringTable::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ToolTable::qt_emit( int _id, QUObject* _o )
{
    return StringTable::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ToolTable::qt_property( int id, int f, QVariant* v)
{
    return StringTable::qt_property( id, f, v);
}

bool ToolTable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
