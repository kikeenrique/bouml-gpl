/****************************************************************************
** OperationDialog meta object code from reading C++ file 'OperationDialog.h'
**
** Created: Sat Apr 5 23:41:51 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "OperationDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *OperationDialog::className() const
{
    return "OperationDialog";
}

QMetaObject *OperationDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_OperationDialog( "OperationDialog", &OperationDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString OperationDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "OperationDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString OperationDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "OperationDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* OperationDialog::staticMetaObject()
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
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"classoper_toggled", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"abstract_toggled", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"forcegenbody_toggled", 1, param_slot_7 };
    static const QUMethod slot_8 = {"cpp_default_decl", 0, 0 };
    static const QUMethod slot_9 = {"cpp_decl_from_def", 0, 0 };
    static const QUMethod slot_10 = {"cpp_unmapped_decl", 0, 0 };
    static const QUMethod slot_11 = {"cpp_update_decl", 0, 0 };
    static const QUMethod slot_12 = {"cpp_edit_param_decl", 0, 0 };
    static const QUMethod slot_13 = {"cpp_def_from_decl", 0, 0 };
    static const QUMethod slot_14 = {"cpp_default_def", 0, 0 };
    static const QUMethod slot_15 = {"cpp_unmapped_def", 0, 0 };
    static const QUMethod slot_16 = {"cpp_update_def", 0, 0 };
    static const QUMethod slot_17 = {"cpp_edit_param_def", 0, 0 };
    static const QUMethod slot_18 = {"cpp_edit_body", 0, 0 };
    static const QUParameter param_slot_19[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_19 = {"const_volatile_toggled", 1, param_slot_19 };
    static const QUParameter param_slot_20[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_20 = {"friend_toggled", 1, param_slot_20 };
    static const QUParameter param_slot_21[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_21 = {"virtual_toggled", 1, param_slot_21 };
    static const QUParameter param_slot_22[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_22 = {"inline_toggled", 1, param_slot_22 };
    static const QUMethod slot_23 = {"java_default_def", 0, 0 };
    static const QUMethod slot_24 = {"java_unmapped_def", 0, 0 };
    static const QUMethod slot_25 = {"java_update_def", 0, 0 };
    static const QUMethod slot_26 = {"java_edit_body", 0, 0 };
    static const QUMethod slot_27 = {"java_edit_annotation", 0, 0 };
    static const QUParameter param_slot_28[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_28 = {"java_finalsynchronized_toggled", 1, param_slot_28 };
    static const QUMethod slot_29 = {"php_default_def", 0, 0 };
    static const QUMethod slot_30 = {"php_unmapped_def", 0, 0 };
    static const QUMethod slot_31 = {"php_update_def", 0, 0 };
    static const QUMethod slot_32 = {"php_edit_body", 0, 0 };
    static const QUParameter param_slot_33[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_33 = {"php_final_toggled", 1, param_slot_33 };
    static const QUMethod slot_34 = {"php_edit_param", 0, 0 };
    static const QUMethod slot_35 = {"python_default_def", 0, 0 };
    static const QUMethod slot_36 = {"python_unmapped_def", 0, 0 };
    static const QUMethod slot_37 = {"python_update_def", 0, 0 };
    static const QUMethod slot_38 = {"python_edit_body", 0, 0 };
    static const QUMethod slot_39 = {"python_edit_param", 0, 0 };
    static const QUMethod slot_40 = {"python_edit_decorator", 0, 0 };
    static const QUMethod slot_41 = {"idl_default_def", 0, 0 };
    static const QUMethod slot_42 = {"idl_unmapped_def", 0, 0 };
    static const QUMethod slot_43 = {"idl_update_decl", 0, 0 };
    static const QUParameter param_slot_44[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_44 = {"oneway_toggled", 1, param_slot_44 };
    static const QUParameter param_slot_45[] = {
	{ 0, &static_QUType_ptr, "QWidget", QUParameter::In }
    };
    static const QUMethod slot_45 = {"update_all_tabs", 1, param_slot_45 };
    static const QUMethod slot_46 = {"menu_returntype", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected },
	{ "default_description()", &slot_2, QMetaData::Protected },
	{ "edit_description()", &slot_3, QMetaData::Protected },
	{ "edit_constraint()", &slot_4, QMetaData::Protected },
	{ "classoper_toggled(bool)", &slot_5, QMetaData::Protected },
	{ "abstract_toggled(bool)", &slot_6, QMetaData::Protected },
	{ "forcegenbody_toggled(bool)", &slot_7, QMetaData::Protected },
	{ "cpp_default_decl()", &slot_8, QMetaData::Protected },
	{ "cpp_decl_from_def()", &slot_9, QMetaData::Protected },
	{ "cpp_unmapped_decl()", &slot_10, QMetaData::Protected },
	{ "cpp_update_decl()", &slot_11, QMetaData::Protected },
	{ "cpp_edit_param_decl()", &slot_12, QMetaData::Protected },
	{ "cpp_def_from_decl()", &slot_13, QMetaData::Protected },
	{ "cpp_default_def()", &slot_14, QMetaData::Protected },
	{ "cpp_unmapped_def()", &slot_15, QMetaData::Protected },
	{ "cpp_update_def()", &slot_16, QMetaData::Protected },
	{ "cpp_edit_param_def()", &slot_17, QMetaData::Protected },
	{ "cpp_edit_body()", &slot_18, QMetaData::Protected },
	{ "const_volatile_toggled(bool)", &slot_19, QMetaData::Protected },
	{ "friend_toggled(bool)", &slot_20, QMetaData::Protected },
	{ "virtual_toggled(bool)", &slot_21, QMetaData::Protected },
	{ "inline_toggled(bool)", &slot_22, QMetaData::Protected },
	{ "java_default_def()", &slot_23, QMetaData::Protected },
	{ "java_unmapped_def()", &slot_24, QMetaData::Protected },
	{ "java_update_def()", &slot_25, QMetaData::Protected },
	{ "java_edit_body()", &slot_26, QMetaData::Protected },
	{ "java_edit_annotation()", &slot_27, QMetaData::Protected },
	{ "java_finalsynchronized_toggled(bool)", &slot_28, QMetaData::Protected },
	{ "php_default_def()", &slot_29, QMetaData::Protected },
	{ "php_unmapped_def()", &slot_30, QMetaData::Protected },
	{ "php_update_def()", &slot_31, QMetaData::Protected },
	{ "php_edit_body()", &slot_32, QMetaData::Protected },
	{ "php_final_toggled(bool)", &slot_33, QMetaData::Protected },
	{ "php_edit_param()", &slot_34, QMetaData::Protected },
	{ "python_default_def()", &slot_35, QMetaData::Protected },
	{ "python_unmapped_def()", &slot_36, QMetaData::Protected },
	{ "python_update_def()", &slot_37, QMetaData::Protected },
	{ "python_edit_body()", &slot_38, QMetaData::Protected },
	{ "python_edit_param()", &slot_39, QMetaData::Protected },
	{ "python_edit_decorator()", &slot_40, QMetaData::Protected },
	{ "idl_default_def()", &slot_41, QMetaData::Protected },
	{ "idl_unmapped_def()", &slot_42, QMetaData::Protected },
	{ "idl_update_decl()", &slot_43, QMetaData::Protected },
	{ "oneway_toggled(bool)", &slot_44, QMetaData::Protected },
	{ "update_all_tabs(QWidget*)", &slot_45, QMetaData::Protected },
	{ "menu_returntype()", &slot_46, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"OperationDialog", parentObject,
	slot_tbl, 47,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_OperationDialog.setMetaObject( metaObj );
    return metaObj;
}

void* OperationDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "OperationDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool OperationDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    case 2: default_description(); break;
    case 3: edit_description(); break;
    case 4: edit_constraint(); break;
    case 5: classoper_toggled((bool)static_QUType_bool.get(_o+1)); break;
    case 6: abstract_toggled((bool)static_QUType_bool.get(_o+1)); break;
    case 7: forcegenbody_toggled((bool)static_QUType_bool.get(_o+1)); break;
    case 8: cpp_default_decl(); break;
    case 9: cpp_decl_from_def(); break;
    case 10: cpp_unmapped_decl(); break;
    case 11: cpp_update_decl(); break;
    case 12: cpp_edit_param_decl(); break;
    case 13: cpp_def_from_decl(); break;
    case 14: cpp_default_def(); break;
    case 15: cpp_unmapped_def(); break;
    case 16: cpp_update_def(); break;
    case 17: cpp_edit_param_def(); break;
    case 18: cpp_edit_body(); break;
    case 19: const_volatile_toggled((bool)static_QUType_bool.get(_o+1)); break;
    case 20: friend_toggled((bool)static_QUType_bool.get(_o+1)); break;
    case 21: virtual_toggled((bool)static_QUType_bool.get(_o+1)); break;
    case 22: inline_toggled((bool)static_QUType_bool.get(_o+1)); break;
    case 23: java_default_def(); break;
    case 24: java_unmapped_def(); break;
    case 25: java_update_def(); break;
    case 26: java_edit_body(); break;
    case 27: java_edit_annotation(); break;
    case 28: java_finalsynchronized_toggled((bool)static_QUType_bool.get(_o+1)); break;
    case 29: php_default_def(); break;
    case 30: php_unmapped_def(); break;
    case 31: php_update_def(); break;
    case 32: php_edit_body(); break;
    case 33: php_final_toggled((bool)static_QUType_bool.get(_o+1)); break;
    case 34: php_edit_param(); break;
    case 35: python_default_def(); break;
    case 36: python_unmapped_def(); break;
    case 37: python_update_def(); break;
    case 38: python_edit_body(); break;
    case 39: python_edit_param(); break;
    case 40: python_edit_decorator(); break;
    case 41: idl_default_def(); break;
    case 42: idl_unmapped_def(); break;
    case 43: idl_update_decl(); break;
    case 44: oneway_toggled((bool)static_QUType_bool.get(_o+1)); break;
    case 45: update_all_tabs((QWidget*)static_QUType_ptr.get(_o+1)); break;
    case 46: menu_returntype(); break;
    default:
	return QTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool OperationDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool OperationDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool OperationDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *ParamsTable::className() const
{
    return "ParamsTable";
}

QMetaObject *ParamsTable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ParamsTable( "ParamsTable", &ParamsTable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ParamsTable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ParamsTable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ParamsTable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ParamsTable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ParamsTable::staticMetaObject()
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
	"ParamsTable", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ParamsTable.setMetaObject( metaObj );
    return metaObj;
}

void* ParamsTable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ParamsTable" ) )
	return this;
    return MyTable::qt_cast( clname );
}

bool ParamsTable::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: button_pressed((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+4))); break;
    case 1: value_changed((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return MyTable::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ParamsTable::qt_emit( int _id, QUObject* _o )
{
    return MyTable::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ParamsTable::qt_property( int id, int f, QVariant* v)
{
    return MyTable::qt_property( id, f, v);
}

bool ParamsTable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *ExceptionsTable::className() const
{
    return "ExceptionsTable";
}

QMetaObject *ExceptionsTable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ExceptionsTable( "ExceptionsTable", &ExceptionsTable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ExceptionsTable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ExceptionsTable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ExceptionsTable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ExceptionsTable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ExceptionsTable::staticMetaObject()
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
	"ExceptionsTable", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ExceptionsTable.setMetaObject( metaObj );
    return metaObj;
}

void* ExceptionsTable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ExceptionsTable" ) )
	return this;
    return MyTable::qt_cast( clname );
}

bool ExceptionsTable::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: button_pressed((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+4))); break;
    case 1: value_changed((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return MyTable::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ExceptionsTable::qt_emit( int _id, QUObject* _o )
{
    return MyTable::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ExceptionsTable::qt_property( int id, int f, QVariant* v)
{
    return MyTable::qt_property( id, f, v);
}

bool ExceptionsTable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *CppParamsTable::className() const
{
    return "CppParamsTable";
}

QMetaObject *CppParamsTable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CppParamsTable( "CppParamsTable", &CppParamsTable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CppParamsTable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CppParamsTable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CppParamsTable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CppParamsTable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CppParamsTable::staticMetaObject()
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
    static const QUMethod slot_1 = {"setCurrentCell", 2, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "button_pressed(int,int,int,const QPoint&)", &slot_0, QMetaData::Protected },
	{ "setCurrentCell(int,int)", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"CppParamsTable", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CppParamsTable.setMetaObject( metaObj );
    return metaObj;
}

void* CppParamsTable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CppParamsTable" ) )
	return this;
    return MyTable::qt_cast( clname );
}

bool CppParamsTable::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: button_pressed((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+4))); break;
    case 1: setCurrentCell((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return MyTable::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CppParamsTable::qt_emit( int _id, QUObject* _o )
{
    return MyTable::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CppParamsTable::qt_property( int id, int f, QVariant* v)
{
    return MyTable::qt_property( id, f, v);
}

bool CppParamsTable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *CppParamsDialog::className() const
{
    return "CppParamsDialog";
}

QMetaObject *CppParamsDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CppParamsDialog( "CppParamsDialog", &CppParamsDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CppParamsDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CppParamsDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CppParamsDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CppParamsDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CppParamsDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"CppParamsDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CppParamsDialog.setMetaObject( metaObj );
    return metaObj;
}

void* CppParamsDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CppParamsDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool CppParamsDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CppParamsDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CppParamsDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool CppParamsDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *PhpParamsTable::className() const
{
    return "PhpParamsTable";
}

QMetaObject *PhpParamsTable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_PhpParamsTable( "PhpParamsTable", &PhpParamsTable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString PhpParamsTable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PhpParamsTable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString PhpParamsTable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PhpParamsTable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* PhpParamsTable::staticMetaObject()
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
    static const QUMethod slot_1 = {"setCurrentCell", 2, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "button_pressed(int,int,int,const QPoint&)", &slot_0, QMetaData::Protected },
	{ "setCurrentCell(int,int)", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"PhpParamsTable", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_PhpParamsTable.setMetaObject( metaObj );
    return metaObj;
}

void* PhpParamsTable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "PhpParamsTable" ) )
	return this;
    return MyTable::qt_cast( clname );
}

bool PhpParamsTable::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: button_pressed((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+4))); break;
    case 1: setCurrentCell((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return MyTable::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool PhpParamsTable::qt_emit( int _id, QUObject* _o )
{
    return MyTable::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool PhpParamsTable::qt_property( int id, int f, QVariant* v)
{
    return MyTable::qt_property( id, f, v);
}

bool PhpParamsTable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *PhpParamsDialog::className() const
{
    return "PhpParamsDialog";
}

QMetaObject *PhpParamsDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_PhpParamsDialog( "PhpParamsDialog", &PhpParamsDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString PhpParamsDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PhpParamsDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString PhpParamsDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PhpParamsDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* PhpParamsDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"PhpParamsDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_PhpParamsDialog.setMetaObject( metaObj );
    return metaObj;
}

void* PhpParamsDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "PhpParamsDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool PhpParamsDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool PhpParamsDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool PhpParamsDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool PhpParamsDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *PythonParamsTable::className() const
{
    return "PythonParamsTable";
}

QMetaObject *PythonParamsTable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_PythonParamsTable( "PythonParamsTable", &PythonParamsTable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString PythonParamsTable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PythonParamsTable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString PythonParamsTable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PythonParamsTable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* PythonParamsTable::staticMetaObject()
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
    static const QUMethod slot_1 = {"setCurrentCell", 2, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "button_pressed(int,int,int,const QPoint&)", &slot_0, QMetaData::Protected },
	{ "setCurrentCell(int,int)", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"PythonParamsTable", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_PythonParamsTable.setMetaObject( metaObj );
    return metaObj;
}

void* PythonParamsTable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "PythonParamsTable" ) )
	return this;
    return MyTable::qt_cast( clname );
}

bool PythonParamsTable::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: button_pressed((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+4))); break;
    case 1: setCurrentCell((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return MyTable::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool PythonParamsTable::qt_emit( int _id, QUObject* _o )
{
    return MyTable::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool PythonParamsTable::qt_property( int id, int f, QVariant* v)
{
    return MyTable::qt_property( id, f, v);
}

bool PythonParamsTable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *PythonParamsDialog::className() const
{
    return "PythonParamsDialog";
}

QMetaObject *PythonParamsDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_PythonParamsDialog( "PythonParamsDialog", &PythonParamsDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString PythonParamsDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PythonParamsDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString PythonParamsDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PythonParamsDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* PythonParamsDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"PythonParamsDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_PythonParamsDialog.setMetaObject( metaObj );
    return metaObj;
}

void* PythonParamsDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "PythonParamsDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool PythonParamsDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool PythonParamsDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool PythonParamsDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool PythonParamsDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
