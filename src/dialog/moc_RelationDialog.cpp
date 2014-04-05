/****************************************************************************
** RelationDialog meta object code from reading C++ file 'RelationDialog.h'
**
** Created: Sat Apr 5 23:41:52 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "RelationDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *RelationDialog::className() const
{
    return "RelationDialog";
}

QMetaObject *RelationDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_RelationDialog( "RelationDialog", &RelationDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString RelationDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RelationDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString RelationDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RelationDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* RelationDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QUMethod slot_2 = {"default_description_a", 0, 0 };
    static const QUMethod slot_3 = {"default_description_b", 0, 0 };
    static const QUMethod slot_4 = {"edit_description_a", 0, 0 };
    static const QUMethod slot_5 = {"edit_description_b", 0, 0 };
    static const QUMethod slot_6 = {"edit_constraint_a", 0, 0 };
    static const QUMethod slot_7 = {"edit_constraint_b", 0, 0 };
    static const QUMethod slot_8 = {"edit_init_a", 0, 0 };
    static const QUMethod slot_9 = {"edit_init_b", 0, 0 };
    static const QUParameter param_slot_10[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"edTypeActivated", 1, param_slot_10 };
    static const QUMethod slot_11 = {"menu_assoc", 0, 0 };
    static const QUParameter param_slot_12[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"derived_changed_a", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"derived_changed_b", 1, param_slot_13 };
    static const QUMethod slot_14 = {"cpp_update_a", 0, 0 };
    static const QUMethod slot_15 = {"cpp_update_b", 0, 0 };
    static const QUMethod slot_16 = {"cpp_default_a", 0, 0 };
    static const QUMethod slot_17 = {"cpp_include_in_header", 0, 0 };
    static const QUMethod slot_18 = {"cpp_unmapped_a", 0, 0 };
    static const QUMethod slot_19 = {"cpp_default_b", 0, 0 };
    static const QUMethod slot_20 = {"cpp_unmapped_b", 0, 0 };
    static const QUMethod slot_21 = {"java_update_a", 0, 0 };
    static const QUMethod slot_22 = {"java_update_b", 0, 0 };
    static const QUMethod slot_23 = {"java_default_a", 0, 0 };
    static const QUMethod slot_24 = {"java_unmapped_a", 0, 0 };
    static const QUMethod slot_25 = {"java_default_b", 0, 0 };
    static const QUMethod slot_26 = {"java_unmapped_b", 0, 0 };
    static const QUMethod slot_27 = {"java_edit_annotation_a", 0, 0 };
    static const QUMethod slot_28 = {"java_edit_annotation_b", 0, 0 };
    static const QUMethod slot_29 = {"php_update_a", 0, 0 };
    static const QUMethod slot_30 = {"php_update_b", 0, 0 };
    static const QUMethod slot_31 = {"php_default_a", 0, 0 };
    static const QUMethod slot_32 = {"php_unmapped_a", 0, 0 };
    static const QUMethod slot_33 = {"php_default_b", 0, 0 };
    static const QUMethod slot_34 = {"php_unmapped_b", 0, 0 };
    static const QUMethod slot_35 = {"python_update_a", 0, 0 };
    static const QUMethod slot_36 = {"python_update_b", 0, 0 };
    static const QUMethod slot_37 = {"python_default_a", 0, 0 };
    static const QUMethod slot_38 = {"python_unmapped_a", 0, 0 };
    static const QUMethod slot_39 = {"python_default_b", 0, 0 };
    static const QUMethod slot_40 = {"python_unmapped_b", 0, 0 };
    static const QUMethod slot_41 = {"idl_update_a", 0, 0 };
    static const QUMethod slot_42 = {"idl_update_b", 0, 0 };
    static const QUMethod slot_43 = {"idl_default_a", 0, 0 };
    static const QUMethod slot_44 = {"idl_unmapped_a", 0, 0 };
    static const QUMethod slot_45 = {"idl_default_b", 0, 0 };
    static const QUMethod slot_46 = {"idl_unmapped_b", 0, 0 };
    static const QUParameter param_slot_47[] = {
	{ 0, &static_QUType_ptr, "QWidget", QUParameter::In }
    };
    static const QUMethod slot_47 = {"update_all_tabs", 1, param_slot_47 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected },
	{ "default_description_a()", &slot_2, QMetaData::Protected },
	{ "default_description_b()", &slot_3, QMetaData::Protected },
	{ "edit_description_a()", &slot_4, QMetaData::Protected },
	{ "edit_description_b()", &slot_5, QMetaData::Protected },
	{ "edit_constraint_a()", &slot_6, QMetaData::Protected },
	{ "edit_constraint_b()", &slot_7, QMetaData::Protected },
	{ "edit_init_a()", &slot_8, QMetaData::Protected },
	{ "edit_init_b()", &slot_9, QMetaData::Protected },
	{ "edTypeActivated(int)", &slot_10, QMetaData::Protected },
	{ "menu_assoc()", &slot_11, QMetaData::Protected },
	{ "derived_changed_a(bool)", &slot_12, QMetaData::Protected },
	{ "derived_changed_b(bool)", &slot_13, QMetaData::Protected },
	{ "cpp_update_a()", &slot_14, QMetaData::Protected },
	{ "cpp_update_b()", &slot_15, QMetaData::Protected },
	{ "cpp_default_a()", &slot_16, QMetaData::Protected },
	{ "cpp_include_in_header()", &slot_17, QMetaData::Protected },
	{ "cpp_unmapped_a()", &slot_18, QMetaData::Protected },
	{ "cpp_default_b()", &slot_19, QMetaData::Protected },
	{ "cpp_unmapped_b()", &slot_20, QMetaData::Protected },
	{ "java_update_a()", &slot_21, QMetaData::Protected },
	{ "java_update_b()", &slot_22, QMetaData::Protected },
	{ "java_default_a()", &slot_23, QMetaData::Protected },
	{ "java_unmapped_a()", &slot_24, QMetaData::Protected },
	{ "java_default_b()", &slot_25, QMetaData::Protected },
	{ "java_unmapped_b()", &slot_26, QMetaData::Protected },
	{ "java_edit_annotation_a()", &slot_27, QMetaData::Protected },
	{ "java_edit_annotation_b()", &slot_28, QMetaData::Protected },
	{ "php_update_a()", &slot_29, QMetaData::Protected },
	{ "php_update_b()", &slot_30, QMetaData::Protected },
	{ "php_default_a()", &slot_31, QMetaData::Protected },
	{ "php_unmapped_a()", &slot_32, QMetaData::Protected },
	{ "php_default_b()", &slot_33, QMetaData::Protected },
	{ "php_unmapped_b()", &slot_34, QMetaData::Protected },
	{ "python_update_a()", &slot_35, QMetaData::Protected },
	{ "python_update_b()", &slot_36, QMetaData::Protected },
	{ "python_default_a()", &slot_37, QMetaData::Protected },
	{ "python_unmapped_a()", &slot_38, QMetaData::Protected },
	{ "python_default_b()", &slot_39, QMetaData::Protected },
	{ "python_unmapped_b()", &slot_40, QMetaData::Protected },
	{ "idl_update_a()", &slot_41, QMetaData::Protected },
	{ "idl_update_b()", &slot_42, QMetaData::Protected },
	{ "idl_default_a()", &slot_43, QMetaData::Protected },
	{ "idl_unmapped_a()", &slot_44, QMetaData::Protected },
	{ "idl_default_b()", &slot_45, QMetaData::Protected },
	{ "idl_unmapped_b()", &slot_46, QMetaData::Protected },
	{ "update_all_tabs(QWidget*)", &slot_47, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"RelationDialog", parentObject,
	slot_tbl, 48,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_RelationDialog.setMetaObject( metaObj );
    return metaObj;
}

void* RelationDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "RelationDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool RelationDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    case 2: default_description_a(); break;
    case 3: default_description_b(); break;
    case 4: edit_description_a(); break;
    case 5: edit_description_b(); break;
    case 6: edit_constraint_a(); break;
    case 7: edit_constraint_b(); break;
    case 8: edit_init_a(); break;
    case 9: edit_init_b(); break;
    case 10: edTypeActivated((int)static_QUType_int.get(_o+1)); break;
    case 11: menu_assoc(); break;
    case 12: derived_changed_a((bool)static_QUType_bool.get(_o+1)); break;
    case 13: derived_changed_b((bool)static_QUType_bool.get(_o+1)); break;
    case 14: cpp_update_a(); break;
    case 15: cpp_update_b(); break;
    case 16: cpp_default_a(); break;
    case 17: cpp_include_in_header(); break;
    case 18: cpp_unmapped_a(); break;
    case 19: cpp_default_b(); break;
    case 20: cpp_unmapped_b(); break;
    case 21: java_update_a(); break;
    case 22: java_update_b(); break;
    case 23: java_default_a(); break;
    case 24: java_unmapped_a(); break;
    case 25: java_default_b(); break;
    case 26: java_unmapped_b(); break;
    case 27: java_edit_annotation_a(); break;
    case 28: java_edit_annotation_b(); break;
    case 29: php_update_a(); break;
    case 30: php_update_b(); break;
    case 31: php_default_a(); break;
    case 32: php_unmapped_a(); break;
    case 33: php_default_b(); break;
    case 34: php_unmapped_b(); break;
    case 35: python_update_a(); break;
    case 36: python_update_b(); break;
    case 37: python_default_a(); break;
    case 38: python_unmapped_a(); break;
    case 39: python_default_b(); break;
    case 40: python_unmapped_b(); break;
    case 41: idl_update_a(); break;
    case 42: idl_update_b(); break;
    case 43: idl_default_a(); break;
    case 44: idl_unmapped_a(); break;
    case 45: idl_default_b(); break;
    case 46: idl_unmapped_b(); break;
    case 47: update_all_tabs((QWidget*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool RelationDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool RelationDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool RelationDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
