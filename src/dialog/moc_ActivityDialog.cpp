/****************************************************************************
** ActivityDialog meta object code from reading C++ file 'ActivityDialog.h'
**
** Created: Sat Apr 5 23:41:59 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ActivityDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ActivityDialog::className() const
{
    return "ActivityDialog";
}

QMetaObject *ActivityDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ActivityDialog( "ActivityDialog", &ActivityDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ActivityDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ActivityDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ActivityDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ActivityDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ActivityDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QUMethod slot_2 = {"menu_specification", 0, 0 };
    static const QUMethod slot_3 = {"edit_description", 0, 0 };
    static const QUMethod slot_4 = {"edit_constraint", 0, 0 };
    static const QUParameter param_slot_5[] = {
	{ 0, &static_QUType_ptr, "QWidget", QUParameter::In }
    };
    static const QUMethod slot_5 = {"change_tabs", 1, param_slot_5 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected },
	{ "menu_specification()", &slot_2, QMetaData::Protected },
	{ "edit_description()", &slot_3, QMetaData::Protected },
	{ "edit_constraint()", &slot_4, QMetaData::Protected },
	{ "change_tabs(QWidget*)", &slot_5, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ActivityDialog", parentObject,
	slot_tbl, 6,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ActivityDialog.setMetaObject( metaObj );
    return metaObj;
}

void* ActivityDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ActivityDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool ActivityDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    case 2: menu_specification(); break;
    case 3: edit_description(); break;
    case 4: edit_constraint(); break;
    case 5: change_tabs((QWidget*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ActivityDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ActivityDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool ActivityDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
