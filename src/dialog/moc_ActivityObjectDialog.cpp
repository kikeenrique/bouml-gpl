/****************************************************************************
** ActivityObjectDialog meta object code from reading C++ file 'ActivityObjectDialog.h'
**
** Created: Sat Apr 5 23:42:02 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ActivityObjectDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ActivityObjectDialog::className() const
{
    return "ActivityObjectDialog";
}

QMetaObject *ActivityObjectDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ActivityObjectDialog( "ActivityObjectDialog", &ActivityObjectDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ActivityObjectDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ActivityObjectDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ActivityObjectDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ActivityObjectDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ActivityObjectDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QUMethod slot_2 = {"edit_description", 0, 0 };
    static const QUMethod slot_3 = {"edit_uml_selection", 0, 0 };
    static const QUMethod slot_4 = {"edit_cpp_selection", 0, 0 };
    static const QUMethod slot_5 = {"edit_java_selection", 0, 0 };
    static const QUMethod slot_6 = {"menu_type", 0, 0 };
    static const QUParameter param_slot_7[] = {
	{ 0, &static_QUType_ptr, "QWidget", QUParameter::In }
    };
    static const QUMethod slot_7 = {"change_tabs", 1, param_slot_7 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected },
	{ "edit_description()", &slot_2, QMetaData::Protected },
	{ "edit_uml_selection()", &slot_3, QMetaData::Protected },
	{ "edit_cpp_selection()", &slot_4, QMetaData::Protected },
	{ "edit_java_selection()", &slot_5, QMetaData::Protected },
	{ "menu_type()", &slot_6, QMetaData::Protected },
	{ "change_tabs(QWidget*)", &slot_7, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ActivityObjectDialog", parentObject,
	slot_tbl, 8,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ActivityObjectDialog.setMetaObject( metaObj );
    return metaObj;
}

void* ActivityObjectDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ActivityObjectDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool ActivityObjectDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    case 2: edit_description(); break;
    case 3: edit_uml_selection(); break;
    case 4: edit_cpp_selection(); break;
    case 5: edit_java_selection(); break;
    case 6: menu_type(); break;
    case 7: change_tabs((QWidget*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ActivityObjectDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ActivityObjectDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool ActivityObjectDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
