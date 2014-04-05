/****************************************************************************
** OpaqueDialog meta object code from reading C++ file 'ActivityActionDialog.h'
**
** Created: Sat Apr 5 23:42:01 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ActivityActionDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *OpaqueDialog::className() const
{
    return "OpaqueDialog";
}

QMetaObject *OpaqueDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_OpaqueDialog( "OpaqueDialog", &OpaqueDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString OpaqueDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "OpaqueDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString OpaqueDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "OpaqueDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* OpaqueDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"edit_ocl", 0, 0 };
    static const QUMethod slot_1 = {"edit_cpp", 0, 0 };
    static const QUMethod slot_2 = {"edit_java", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "edit_ocl()", &slot_0, QMetaData::Protected },
	{ "edit_cpp()", &slot_1, QMetaData::Protected },
	{ "edit_java()", &slot_2, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"OpaqueDialog", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_OpaqueDialog.setMetaObject( metaObj );
    return metaObj;
}

void* OpaqueDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "OpaqueDialog" ) )
	return this;
    if ( !qstrcmp( clname, "AnyActionDialog" ) )
	return (AnyActionDialog*)this;
    return QObject::qt_cast( clname );
}

bool OpaqueDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: edit_ocl(); break;
    case 1: edit_cpp(); break;
    case 2: edit_java(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool OpaqueDialog::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool OpaqueDialog::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool OpaqueDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *AccessVariableValueDialog::className() const
{
    return "AccessVariableValueDialog";
}

QMetaObject *AccessVariableValueDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AccessVariableValueDialog( "AccessVariableValueDialog", &AccessVariableValueDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AccessVariableValueDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AccessVariableValueDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AccessVariableValueDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AccessVariableValueDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AccessVariableValueDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"classChanged", 1, param_slot_0 };
    static const QUMethod slot_1 = {"menu_var", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "classChanged(int)", &slot_0, QMetaData::Protected },
	{ "menu_var()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"AccessVariableValueDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AccessVariableValueDialog.setMetaObject( metaObj );
    return metaObj;
}

void* AccessVariableValueDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AccessVariableValueDialog" ) )
	return this;
    if ( !qstrcmp( clname, "AnyActionDialog" ) )
	return (AnyActionDialog*)this;
    return QObject::qt_cast( clname );
}

bool AccessVariableValueDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: classChanged((int)static_QUType_int.get(_o+1)); break;
    case 1: menu_var(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool AccessVariableValueDialog::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool AccessVariableValueDialog::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool AccessVariableValueDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *CallBehaviorDialog::className() const
{
    return "CallBehaviorDialog";
}

QMetaObject *CallBehaviorDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CallBehaviorDialog( "CallBehaviorDialog", &CallBehaviorDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CallBehaviorDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CallBehaviorDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CallBehaviorDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CallBehaviorDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CallBehaviorDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"menu_beh", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "menu_beh()", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"CallBehaviorDialog", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CallBehaviorDialog.setMetaObject( metaObj );
    return metaObj;
}

void* CallBehaviorDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CallBehaviorDialog" ) )
	return this;
    if ( !qstrcmp( clname, "WithBehaviorDialog" ) )
	return (WithBehaviorDialog*)this;
    return QObject::qt_cast( clname );
}

bool CallBehaviorDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: menu_beh(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CallBehaviorDialog::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CallBehaviorDialog::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool CallBehaviorDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *CallOperationDialog::className() const
{
    return "CallOperationDialog";
}

QMetaObject *CallOperationDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CallOperationDialog( "CallOperationDialog", &CallOperationDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CallOperationDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CallOperationDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CallOperationDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CallOperationDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CallOperationDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"classChanged", 1, param_slot_0 };
    static const QUMethod slot_1 = {"menu_oper", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "classChanged(int)", &slot_0, QMetaData::Protected },
	{ "menu_oper()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"CallOperationDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CallOperationDialog.setMetaObject( metaObj );
    return metaObj;
}

void* CallOperationDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CallOperationDialog" ) )
	return this;
    if ( !qstrcmp( clname, "AnyActionDialog" ) )
	return (AnyActionDialog*)this;
    return QObject::qt_cast( clname );
}

bool CallOperationDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: classChanged((int)static_QUType_int.get(_o+1)); break;
    case 1: menu_oper(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CallOperationDialog::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CallOperationDialog::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool CallOperationDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *ValueSpecificationDialog::className() const
{
    return "ValueSpecificationDialog";
}

QMetaObject *ValueSpecificationDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ValueSpecificationDialog( "ValueSpecificationDialog", &ValueSpecificationDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ValueSpecificationDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ValueSpecificationDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ValueSpecificationDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ValueSpecificationDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ValueSpecificationDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"edit_ocl", 0, 0 };
    static const QUMethod slot_1 = {"edit_cpp", 0, 0 };
    static const QUMethod slot_2 = {"edit_java", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "edit_ocl()", &slot_0, QMetaData::Protected },
	{ "edit_cpp()", &slot_1, QMetaData::Protected },
	{ "edit_java()", &slot_2, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ValueSpecificationDialog", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ValueSpecificationDialog.setMetaObject( metaObj );
    return metaObj;
}

void* ValueSpecificationDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ValueSpecificationDialog" ) )
	return this;
    if ( !qstrcmp( clname, "AnyActionDialog" ) )
	return (AnyActionDialog*)this;
    return QObject::qt_cast( clname );
}

bool ValueSpecificationDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: edit_ocl(); break;
    case 1: edit_cpp(); break;
    case 2: edit_java(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ValueSpecificationDialog::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ValueSpecificationDialog::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool ValueSpecificationDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *ReduceDialog::className() const
{
    return "ReduceDialog";
}

QMetaObject *ReduceDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ReduceDialog( "ReduceDialog", &ReduceDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ReduceDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ReduceDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ReduceDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ReduceDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ReduceDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"menu_beh", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "menu_beh()", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ReduceDialog", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ReduceDialog.setMetaObject( metaObj );
    return metaObj;
}

void* ReduceDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ReduceDialog" ) )
	return this;
    if ( !qstrcmp( clname, "WithBehaviorDialog" ) )
	return (WithBehaviorDialog*)this;
    return QObject::qt_cast( clname );
}

bool ReduceDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: menu_beh(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ReduceDialog::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ReduceDialog::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool ReduceDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *ActivityActionDialog::className() const
{
    return "ActivityActionDialog";
}

QMetaObject *ActivityActionDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ActivityActionDialog( "ActivityActionDialog", &ActivityActionDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ActivityActionDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ActivityActionDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ActivityActionDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ActivityActionDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ActivityActionDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"edTypeActivated", 1, param_slot_2 };
    static const QUMethod slot_3 = {"edit_description", 0, 0 };
    static const QUMethod slot_4 = {"edit_constraint", 0, 0 };
    static const QUParameter param_slot_5[] = {
	{ 0, &static_QUType_ptr, "QWidget", QUParameter::In }
    };
    static const QUMethod slot_5 = {"change_tabs", 1, param_slot_5 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected },
	{ "edTypeActivated(int)", &slot_2, QMetaData::Protected },
	{ "edit_description()", &slot_3, QMetaData::Protected },
	{ "edit_constraint()", &slot_4, QMetaData::Protected },
	{ "change_tabs(QWidget*)", &slot_5, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ActivityActionDialog", parentObject,
	slot_tbl, 6,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ActivityActionDialog.setMetaObject( metaObj );
    return metaObj;
}

void* ActivityActionDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ActivityActionDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool ActivityActionDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    case 2: edTypeActivated((int)static_QUType_int.get(_o+1)); break;
    case 3: edit_description(); break;
    case 4: edit_constraint(); break;
    case 5: change_tabs((QWidget*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ActivityActionDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ActivityActionDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool ActivityActionDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
