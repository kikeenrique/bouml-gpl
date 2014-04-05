/****************************************************************************
** TransitionDialog meta object code from reading C++ file 'TransitionDialog.h'
**
** Created: Sat Apr 5 23:41:54 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "TransitionDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *TransitionDialog::className() const
{
    return "TransitionDialog";
}

QMetaObject *TransitionDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TransitionDialog( "TransitionDialog", &TransitionDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString TransitionDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TransitionDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TransitionDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TransitionDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TransitionDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QUMethod slot_2 = {"edit_description", 0, 0 };
    static const QUMethod slot_3 = {"edit_uml_trigger", 0, 0 };
    static const QUMethod slot_4 = {"edit_uml_guard", 0, 0 };
    static const QUMethod slot_5 = {"edit_uml_expr", 0, 0 };
    static const QUMethod slot_6 = {"edit_cpp_trigger", 0, 0 };
    static const QUMethod slot_7 = {"edit_cpp_guard", 0, 0 };
    static const QUMethod slot_8 = {"edit_cpp_expr", 0, 0 };
    static const QUMethod slot_9 = {"edit_java_trigger", 0, 0 };
    static const QUMethod slot_10 = {"edit_java_guard", 0, 0 };
    static const QUMethod slot_11 = {"edit_java_expr", 0, 0 };
    static const QUParameter param_slot_12[] = {
	{ 0, &static_QUType_ptr, "QWidget", QUParameter::In }
    };
    static const QUMethod slot_12 = {"change_tabs", 1, param_slot_12 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected },
	{ "edit_description()", &slot_2, QMetaData::Protected },
	{ "edit_uml_trigger()", &slot_3, QMetaData::Protected },
	{ "edit_uml_guard()", &slot_4, QMetaData::Protected },
	{ "edit_uml_expr()", &slot_5, QMetaData::Protected },
	{ "edit_cpp_trigger()", &slot_6, QMetaData::Protected },
	{ "edit_cpp_guard()", &slot_7, QMetaData::Protected },
	{ "edit_cpp_expr()", &slot_8, QMetaData::Protected },
	{ "edit_java_trigger()", &slot_9, QMetaData::Protected },
	{ "edit_java_guard()", &slot_10, QMetaData::Protected },
	{ "edit_java_expr()", &slot_11, QMetaData::Protected },
	{ "change_tabs(QWidget*)", &slot_12, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"TransitionDialog", parentObject,
	slot_tbl, 13,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TransitionDialog.setMetaObject( metaObj );
    return metaObj;
}

void* TransitionDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TransitionDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool TransitionDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    case 2: edit_description(); break;
    case 3: edit_uml_trigger(); break;
    case 4: edit_uml_guard(); break;
    case 5: edit_uml_expr(); break;
    case 6: edit_cpp_trigger(); break;
    case 7: edit_cpp_guard(); break;
    case 8: edit_cpp_expr(); break;
    case 9: edit_java_trigger(); break;
    case 10: edit_java_guard(); break;
    case 11: edit_java_expr(); break;
    case 12: change_tabs((QWidget*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool TransitionDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool TransitionDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool TransitionDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
