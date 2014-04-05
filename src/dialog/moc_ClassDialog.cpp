/****************************************************************************
** ClassDialog meta object code from reading C++ file 'ClassDialog.h'
**
** Created: Sat Apr 5 23:41:46 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ClassDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ClassDialog::className() const
{
    return "ClassDialog";
}

QMetaObject *ClassDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ClassDialog( "ClassDialog", &ClassDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ClassDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ClassDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ClassDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ClassDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ClassDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QUMethod slot_2 = {"default_description", 0, 0 };
    static const QUMethod slot_3 = {"edit_description", 0, 0 };
    static const QUMethod slot_4 = {"edit_constraint", 0, 0 };
    static const QUParameter param_slot_5[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"edStereotypeActivated", 1, param_slot_5 };
    static const QUMethod slot_6 = {"icon_browse", 0, 0 };
    static const QUMethod slot_7 = {"icon_root_relative", 0, 0 };
    static const QUMethod slot_8 = {"icon_prj_relative", 0, 0 };
    static const QUParameter param_slot_9[] = {
	{ 0, &static_QUType_ptr, "QWidget", QUParameter::In }
    };
    static const QUMethod slot_9 = {"update_all_tabs", 1, param_slot_9 };
    static const QUMethod slot_10 = {"cpp_update_decl", 0, 0 };
    static const QUMethod slot_11 = {"cpp_default_decl", 0, 0 };
    static const QUMethod slot_12 = {"cpp_unmapped_decl", 0, 0 };
    static const QUMethod slot_13 = {"java_update_decl", 0, 0 };
    static const QUMethod slot_14 = {"java_default_decl", 0, 0 };
    static const QUMethod slot_15 = {"java_unmapped_decl", 0, 0 };
    static const QUMethod slot_16 = {"java_edit_annotation", 0, 0 };
    static const QUMethod slot_17 = {"php_update_decl", 0, 0 };
    static const QUMethod slot_18 = {"php_default_decl", 0, 0 };
    static const QUMethod slot_19 = {"php_unmapped_decl", 0, 0 };
    static const QUMethod slot_20 = {"python_update_decl", 0, 0 };
    static const QUMethod slot_21 = {"python_default_decl", 0, 0 };
    static const QUMethod slot_22 = {"python_unmapped_decl", 0, 0 };
    static const QUMethod slot_23 = {"idl_update_decl", 0, 0 };
    static const QUMethod slot_24 = {"idl_default_decl", 0, 0 };
    static const QUMethod slot_25 = {"idl_unmapped_decl", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected },
	{ "default_description()", &slot_2, QMetaData::Protected },
	{ "edit_description()", &slot_3, QMetaData::Protected },
	{ "edit_constraint()", &slot_4, QMetaData::Protected },
	{ "edStereotypeActivated(const QString&)", &slot_5, QMetaData::Protected },
	{ "icon_browse()", &slot_6, QMetaData::Protected },
	{ "icon_root_relative()", &slot_7, QMetaData::Protected },
	{ "icon_prj_relative()", &slot_8, QMetaData::Protected },
	{ "update_all_tabs(QWidget*)", &slot_9, QMetaData::Protected },
	{ "cpp_update_decl()", &slot_10, QMetaData::Protected },
	{ "cpp_default_decl()", &slot_11, QMetaData::Protected },
	{ "cpp_unmapped_decl()", &slot_12, QMetaData::Protected },
	{ "java_update_decl()", &slot_13, QMetaData::Protected },
	{ "java_default_decl()", &slot_14, QMetaData::Protected },
	{ "java_unmapped_decl()", &slot_15, QMetaData::Protected },
	{ "java_edit_annotation()", &slot_16, QMetaData::Protected },
	{ "php_update_decl()", &slot_17, QMetaData::Protected },
	{ "php_default_decl()", &slot_18, QMetaData::Protected },
	{ "php_unmapped_decl()", &slot_19, QMetaData::Protected },
	{ "python_update_decl()", &slot_20, QMetaData::Protected },
	{ "python_default_decl()", &slot_21, QMetaData::Protected },
	{ "python_unmapped_decl()", &slot_22, QMetaData::Protected },
	{ "idl_update_decl()", &slot_23, QMetaData::Protected },
	{ "idl_default_decl()", &slot_24, QMetaData::Protected },
	{ "idl_unmapped_decl()", &slot_25, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ClassDialog", parentObject,
	slot_tbl, 26,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ClassDialog.setMetaObject( metaObj );
    return metaObj;
}

void* ClassDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ClassDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool ClassDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    case 2: default_description(); break;
    case 3: edit_description(); break;
    case 4: edit_constraint(); break;
    case 5: edStereotypeActivated((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: icon_browse(); break;
    case 7: icon_root_relative(); break;
    case 8: icon_prj_relative(); break;
    case 9: update_all_tabs((QWidget*)static_QUType_ptr.get(_o+1)); break;
    case 10: cpp_update_decl(); break;
    case 11: cpp_default_decl(); break;
    case 12: cpp_unmapped_decl(); break;
    case 13: java_update_decl(); break;
    case 14: java_default_decl(); break;
    case 15: java_unmapped_decl(); break;
    case 16: java_edit_annotation(); break;
    case 17: php_update_decl(); break;
    case 18: php_default_decl(); break;
    case 19: php_unmapped_decl(); break;
    case 20: python_update_decl(); break;
    case 21: python_default_decl(); break;
    case 22: python_unmapped_decl(); break;
    case 23: idl_update_decl(); break;
    case 24: idl_default_decl(); break;
    case 25: idl_unmapped_decl(); break;
    default:
	return QTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ClassDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ClassDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool ClassDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *FormalParamsTable::className() const
{
    return "FormalParamsTable";
}

QMetaObject *FormalParamsTable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FormalParamsTable( "FormalParamsTable", &FormalParamsTable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FormalParamsTable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FormalParamsTable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FormalParamsTable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FormalParamsTable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FormalParamsTable::staticMetaObject()
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
	"FormalParamsTable", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FormalParamsTable.setMetaObject( metaObj );
    return metaObj;
}

void* FormalParamsTable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FormalParamsTable" ) )
	return this;
    return MyTable::qt_cast( clname );
}

bool FormalParamsTable::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: button_pressed((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+4))); break;
    case 1: value_changed((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return MyTable::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool FormalParamsTable::qt_emit( int _id, QUObject* _o )
{
    return MyTable::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool FormalParamsTable::qt_property( int id, int f, QVariant* v)
{
    return MyTable::qt_property( id, f, v);
}

bool FormalParamsTable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *ApplicableOnTable::className() const
{
    return "ApplicableOnTable";
}

QMetaObject *ApplicableOnTable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ApplicableOnTable( "ApplicableOnTable", &ApplicableOnTable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ApplicableOnTable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ApplicableOnTable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ApplicableOnTable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ApplicableOnTable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ApplicableOnTable::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = MyTable::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_0 = {"button_pressed", 4, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "button_pressed(int,int,int,const QPoint&)", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ApplicableOnTable", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ApplicableOnTable.setMetaObject( metaObj );
    return metaObj;
}

void* ApplicableOnTable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ApplicableOnTable" ) )
	return this;
    return MyTable::qt_cast( clname );
}

bool ApplicableOnTable::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: button_pressed((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+4))); break;
    default:
	return MyTable::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ApplicableOnTable::qt_emit( int _id, QUObject* _o )
{
    return MyTable::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ApplicableOnTable::qt_property( int id, int f, QVariant* v)
{
    return MyTable::qt_property( id, f, v);
}

bool ApplicableOnTable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
