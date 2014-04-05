/****************************************************************************
** ComponentDialog meta object code from reading C++ file 'ComponentDialog.h'
**
** Created: Sat Apr 5 23:41:48 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ComponentDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ComponentDialog::className() const
{
    return "ComponentDialog";
}

QMetaObject *ComponentDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ComponentDialog( "ComponentDialog", &ComponentDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ComponentDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ComponentDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ComponentDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ComponentDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ComponentDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QUMethod slot_2 = {"edit_description", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "st", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"rq_stereotypeFilterActivated", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "st", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"pr_stereotypeFilterActivated", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "st", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"rz_stereotypeFilterActivated", 1, param_slot_5 };
    static const QUMethod slot_6 = {"require_cls", 0, 0 };
    static const QUMethod slot_7 = {"unrequire_cls", 0, 0 };
    static const QUMethod slot_8 = {"provide_cls", 0, 0 };
    static const QUMethod slot_9 = {"unprovide_cls", 0, 0 };
    static const QUMethod slot_10 = {"realize_cls", 0, 0 };
    static const QUMethod slot_11 = {"unrealize_cls", 0, 0 };
    static const QUParameter param_slot_12[] = {
	{ 0, &static_QUType_ptr, "QWidget", QUParameter::In }
    };
    static const QUMethod slot_12 = {"change_tabs", 1, param_slot_12 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected },
	{ "edit_description()", &slot_2, QMetaData::Protected },
	{ "rq_stereotypeFilterActivated(const QString&)", &slot_3, QMetaData::Protected },
	{ "pr_stereotypeFilterActivated(const QString&)", &slot_4, QMetaData::Protected },
	{ "rz_stereotypeFilterActivated(const QString&)", &slot_5, QMetaData::Protected },
	{ "require_cls()", &slot_6, QMetaData::Protected },
	{ "unrequire_cls()", &slot_7, QMetaData::Protected },
	{ "provide_cls()", &slot_8, QMetaData::Protected },
	{ "unprovide_cls()", &slot_9, QMetaData::Protected },
	{ "realize_cls()", &slot_10, QMetaData::Protected },
	{ "unrealize_cls()", &slot_11, QMetaData::Protected },
	{ "change_tabs(QWidget*)", &slot_12, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ComponentDialog", parentObject,
	slot_tbl, 13,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ComponentDialog.setMetaObject( metaObj );
    return metaObj;
}

void* ComponentDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ComponentDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool ComponentDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    case 2: edit_description(); break;
    case 3: rq_stereotypeFilterActivated((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: pr_stereotypeFilterActivated((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: rz_stereotypeFilterActivated((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: require_cls(); break;
    case 7: unrequire_cls(); break;
    case 8: provide_cls(); break;
    case 9: unprovide_cls(); break;
    case 10: realize_cls(); break;
    case 11: unrealize_cls(); break;
    case 12: change_tabs((QWidget*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ComponentDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ComponentDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool ComponentDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
