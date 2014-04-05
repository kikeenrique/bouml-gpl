/****************************************************************************
** RelTable meta object code from reading C++ file 'ClassInstanceDialog.h'
**
** Created: Sat Apr 5 23:41:54 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ClassInstanceDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *RelTable::className() const
{
    return "RelTable";
}

QMetaObject *RelTable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_RelTable( "RelTable", &RelTable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString RelTable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RelTable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString RelTable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RelTable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* RelTable::staticMetaObject()
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
    static const QMetaData slot_tbl[] = {
	{ "button_pressed(int,int,int,const QPoint&)", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"RelTable", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_RelTable.setMetaObject( metaObj );
    return metaObj;
}

void* RelTable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "RelTable" ) )
	return this;
    return MyTable::qt_cast( clname );
}

bool RelTable::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: button_pressed((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+4))); break;
    default:
	return MyTable::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool RelTable::qt_emit( int _id, QUObject* _o )
{
    return MyTable::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool RelTable::qt_property( int id, int f, QVariant* v)
{
    return MyTable::qt_property( id, f, v);
}

bool RelTable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *ClassInstanceDialog::className() const
{
    return "ClassInstanceDialog";
}

QMetaObject *ClassInstanceDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ClassInstanceDialog( "ClassInstanceDialog", &ClassInstanceDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ClassInstanceDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ClassInstanceDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ClassInstanceDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ClassInstanceDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ClassInstanceDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QUMethod slot_2 = {"edit_description", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"type_changed", 1, param_slot_3 };
    static const QUMethod slot_4 = {"menu_class", 0, 0 };
    static const QUParameter param_slot_5[] = {
	{ 0, &static_QUType_ptr, "QWidget", QUParameter::In }
    };
    static const QUMethod slot_5 = {"update_all_tabs", 1, param_slot_5 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected },
	{ "edit_description()", &slot_2, QMetaData::Protected },
	{ "type_changed(int)", &slot_3, QMetaData::Protected },
	{ "menu_class()", &slot_4, QMetaData::Protected },
	{ "update_all_tabs(QWidget*)", &slot_5, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ClassInstanceDialog", parentObject,
	slot_tbl, 6,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ClassInstanceDialog.setMetaObject( metaObj );
    return metaObj;
}

void* ClassInstanceDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ClassInstanceDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool ClassInstanceDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    case 2: edit_description(); break;
    case 3: type_changed((int)static_QUType_int.get(_o+1)); break;
    case 4: menu_class(); break;
    case 5: update_all_tabs((QWidget*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ClassInstanceDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ClassInstanceDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool ClassInstanceDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
