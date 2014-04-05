/****************************************************************************
** AttributeDialog meta object code from reading C++ file 'AttributeDialog.h'
**
** Created: Sat Apr 5 23:41:45 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "AttributeDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *AttributeDialog::className() const
{
    return "AttributeDialog";
}

QMetaObject *AttributeDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AttributeDialog( "AttributeDialog", &AttributeDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AttributeDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AttributeDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AttributeDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AttributeDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AttributeDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QUMethod slot_2 = {"edit_init", 0, 0 };
    static const QUMethod slot_3 = {"default_description", 0, 0 };
    static const QUMethod slot_4 = {"edit_description", 0, 0 };
    static const QUMethod slot_5 = {"edit_constraint", 0, 0 };
    static const QUMethod slot_6 = {"menu_type", 0, 0 };
    static const QUParameter param_slot_7[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"derived_changed", 1, param_slot_7 };
    static const QUMethod slot_8 = {"cpp_default", 0, 0 };
    static const QUMethod slot_9 = {"cpp_unmapped", 0, 0 };
    static const QUMethod slot_10 = {"cpp_update", 0, 0 };
    static const QUMethod slot_11 = {"java_default", 0, 0 };
    static const QUMethod slot_12 = {"java_default_item", 0, 0 };
    static const QUMethod slot_13 = {"java_unmapped", 0, 0 };
    static const QUMethod slot_14 = {"java_update", 0, 0 };
    static const QUMethod slot_15 = {"java_edit_annotation", 0, 0 };
    static const QUMethod slot_16 = {"php_default", 0, 0 };
    static const QUMethod slot_17 = {"php_unmapped", 0, 0 };
    static const QUMethod slot_18 = {"php_update", 0, 0 };
    static const QUMethod slot_19 = {"python_default", 0, 0 };
    static const QUMethod slot_20 = {"python_unmapped", 0, 0 };
    static const QUMethod slot_21 = {"python_update", 0, 0 };
    static const QUMethod slot_22 = {"idl_default", 0, 0 };
    static const QUMethod slot_23 = {"idl_default_state", 0, 0 };
    static const QUMethod slot_24 = {"idl_default_constant", 0, 0 };
    static const QUMethod slot_25 = {"idl_unmapped", 0, 0 };
    static const QUMethod slot_26 = {"idl_update", 0, 0 };
    static const QUParameter param_slot_27[] = {
	{ 0, &static_QUType_ptr, "QWidget", QUParameter::In }
    };
    static const QUMethod slot_27 = {"update_all_tabs", 1, param_slot_27 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected },
	{ "edit_init()", &slot_2, QMetaData::Protected },
	{ "default_description()", &slot_3, QMetaData::Protected },
	{ "edit_description()", &slot_4, QMetaData::Protected },
	{ "edit_constraint()", &slot_5, QMetaData::Protected },
	{ "menu_type()", &slot_6, QMetaData::Protected },
	{ "derived_changed(bool)", &slot_7, QMetaData::Protected },
	{ "cpp_default()", &slot_8, QMetaData::Protected },
	{ "cpp_unmapped()", &slot_9, QMetaData::Protected },
	{ "cpp_update()", &slot_10, QMetaData::Protected },
	{ "java_default()", &slot_11, QMetaData::Protected },
	{ "java_default_item()", &slot_12, QMetaData::Protected },
	{ "java_unmapped()", &slot_13, QMetaData::Protected },
	{ "java_update()", &slot_14, QMetaData::Protected },
	{ "java_edit_annotation()", &slot_15, QMetaData::Protected },
	{ "php_default()", &slot_16, QMetaData::Protected },
	{ "php_unmapped()", &slot_17, QMetaData::Protected },
	{ "php_update()", &slot_18, QMetaData::Protected },
	{ "python_default()", &slot_19, QMetaData::Protected },
	{ "python_unmapped()", &slot_20, QMetaData::Protected },
	{ "python_update()", &slot_21, QMetaData::Protected },
	{ "idl_default()", &slot_22, QMetaData::Protected },
	{ "idl_default_state()", &slot_23, QMetaData::Protected },
	{ "idl_default_constant()", &slot_24, QMetaData::Protected },
	{ "idl_unmapped()", &slot_25, QMetaData::Protected },
	{ "idl_update()", &slot_26, QMetaData::Protected },
	{ "update_all_tabs(QWidget*)", &slot_27, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"AttributeDialog", parentObject,
	slot_tbl, 28,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AttributeDialog.setMetaObject( metaObj );
    return metaObj;
}

void* AttributeDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AttributeDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool AttributeDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    case 2: edit_init(); break;
    case 3: default_description(); break;
    case 4: edit_description(); break;
    case 5: edit_constraint(); break;
    case 6: menu_type(); break;
    case 7: derived_changed((bool)static_QUType_bool.get(_o+1)); break;
    case 8: cpp_default(); break;
    case 9: cpp_unmapped(); break;
    case 10: cpp_update(); break;
    case 11: java_default(); break;
    case 12: java_default_item(); break;
    case 13: java_unmapped(); break;
    case 14: java_update(); break;
    case 15: java_edit_annotation(); break;
    case 16: php_default(); break;
    case 17: php_unmapped(); break;
    case 18: php_update(); break;
    case 19: python_default(); break;
    case 20: python_unmapped(); break;
    case 21: python_update(); break;
    case 22: idl_default(); break;
    case 23: idl_default_state(); break;
    case 24: idl_default_constant(); break;
    case 25: idl_unmapped(); break;
    case 26: idl_update(); break;
    case 27: update_all_tabs((QWidget*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool AttributeDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool AttributeDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool AttributeDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
