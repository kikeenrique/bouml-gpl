/****************************************************************************
** StateDialog meta object code from reading C++ file 'StateDialog.h'
**
** Created: Sat Apr 5 23:41:53 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "StateDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *StateDialog::className() const
{
    return "StateDialog";
}

QMetaObject *StateDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_StateDialog( "StateDialog", &StateDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString StateDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StateDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString StateDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StateDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* StateDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QUMethod slot_2 = {"menu_specification", 0, 0 };
    static const QUMethod slot_3 = {"menu_reference", 0, 0 };
    static const QUMethod slot_4 = {"edit_description", 0, 0 };
    static const QUMethod slot_5 = {"edit_uml_entry", 0, 0 };
    static const QUMethod slot_6 = {"edit_uml_exit", 0, 0 };
    static const QUMethod slot_7 = {"edit_uml_activity", 0, 0 };
    static const QUMethod slot_8 = {"edit_cpp_entry", 0, 0 };
    static const QUMethod slot_9 = {"edit_cpp_exit", 0, 0 };
    static const QUMethod slot_10 = {"edit_cpp_activity", 0, 0 };
    static const QUMethod slot_11 = {"edit_java_entry", 0, 0 };
    static const QUMethod slot_12 = {"edit_java_exit", 0, 0 };
    static const QUMethod slot_13 = {"edit_java_activity", 0, 0 };
    static const QUParameter param_slot_14[] = {
	{ 0, &static_QUType_ptr, "QWidget", QUParameter::In }
    };
    static const QUMethod slot_14 = {"change_tabs", 1, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"ed_ref_activated", 1, param_slot_15 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected },
	{ "menu_specification()", &slot_2, QMetaData::Protected },
	{ "menu_reference()", &slot_3, QMetaData::Protected },
	{ "edit_description()", &slot_4, QMetaData::Protected },
	{ "edit_uml_entry()", &slot_5, QMetaData::Protected },
	{ "edit_uml_exit()", &slot_6, QMetaData::Protected },
	{ "edit_uml_activity()", &slot_7, QMetaData::Protected },
	{ "edit_cpp_entry()", &slot_8, QMetaData::Protected },
	{ "edit_cpp_exit()", &slot_9, QMetaData::Protected },
	{ "edit_cpp_activity()", &slot_10, QMetaData::Protected },
	{ "edit_java_entry()", &slot_11, QMetaData::Protected },
	{ "edit_java_exit()", &slot_12, QMetaData::Protected },
	{ "edit_java_activity()", &slot_13, QMetaData::Protected },
	{ "change_tabs(QWidget*)", &slot_14, QMetaData::Protected },
	{ "ed_ref_activated(int)", &slot_15, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"StateDialog", parentObject,
	slot_tbl, 16,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_StateDialog.setMetaObject( metaObj );
    return metaObj;
}

void* StateDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "StateDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool StateDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    case 2: menu_specification(); break;
    case 3: menu_reference(); break;
    case 4: edit_description(); break;
    case 5: edit_uml_entry(); break;
    case 6: edit_uml_exit(); break;
    case 7: edit_uml_activity(); break;
    case 8: edit_cpp_entry(); break;
    case 9: edit_cpp_exit(); break;
    case 10: edit_cpp_activity(); break;
    case 11: edit_java_entry(); break;
    case 12: edit_java_exit(); break;
    case 13: edit_java_activity(); break;
    case 14: change_tabs((QWidget*)static_QUType_ptr.get(_o+1)); break;
    case 15: ed_ref_activated((int)static_QUType_int.get(_o+1)); break;
    default:
	return QTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool StateDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool StateDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool StateDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
