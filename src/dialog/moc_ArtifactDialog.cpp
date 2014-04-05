/****************************************************************************
** ArtifactDialog meta object code from reading C++ file 'ArtifactDialog.h'
**
** Created: Sat Apr 5 23:41:48 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ArtifactDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ArtifactDialog::className() const
{
    return "ArtifactDialog";
}

QMetaObject *ArtifactDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ArtifactDialog( "ArtifactDialog", &ArtifactDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ArtifactDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ArtifactDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ArtifactDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ArtifactDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ArtifactDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QUMethod slot_2 = {"default_description", 0, 0 };
    static const QUMethod slot_3 = {"edit_description", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"edStereotypeActivated", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ 0, &static_QUType_ptr, "QWidget", QUParameter::In }
    };
    static const QUMethod slot_5 = {"update_tab", 1, param_slot_5 };
    static const QUMethod slot_6 = {"cpp_edit_h", 0, 0 };
    static const QUMethod slot_7 = {"cpp_update_h", 0, 0 };
    static const QUMethod slot_8 = {"cpp_default_h", 0, 0 };
    static const QUMethod slot_9 = {"cpp_unmapped_h", 0, 0 };
    static const QUMethod slot_10 = {"cpp_edit_src", 0, 0 };
    static const QUMethod slot_11 = {"cpp_update_src", 0, 0 };
    static const QUMethod slot_12 = {"cpp_default_src", 0, 0 };
    static const QUMethod slot_13 = {"cpp_unmapped_src", 0, 0 };
    static const QUMethod slot_14 = {"java_edit", 0, 0 };
    static const QUMethod slot_15 = {"java_update_src", 0, 0 };
    static const QUMethod slot_16 = {"java_default_src", 0, 0 };
    static const QUMethod slot_17 = {"java_unmapped_src", 0, 0 };
    static const QUMethod slot_18 = {"php_edit", 0, 0 };
    static const QUMethod slot_19 = {"php_update_src", 0, 0 };
    static const QUMethod slot_20 = {"php_default_src", 0, 0 };
    static const QUMethod slot_21 = {"php_unmapped_src", 0, 0 };
    static const QUMethod slot_22 = {"python_edit", 0, 0 };
    static const QUMethod slot_23 = {"python_update_src", 0, 0 };
    static const QUMethod slot_24 = {"python_default_src", 0, 0 };
    static const QUMethod slot_25 = {"python_unmapped_src", 0, 0 };
    static const QUMethod slot_26 = {"idl_edit", 0, 0 };
    static const QUMethod slot_27 = {"idl_update_src", 0, 0 };
    static const QUMethod slot_28 = {"idl_default_src", 0, 0 };
    static const QUMethod slot_29 = {"idl_unmapped_src", 0, 0 };
    static const QUMethod slot_30 = {"associate_cls", 0, 0 };
    static const QUMethod slot_31 = {"unassociate_cls", 0, 0 };
    static const QUMethod slot_32 = {"move_cls_up", 0, 0 };
    static const QUMethod slot_33 = {"move_cls_down", 0, 0 };
    static const QUMethod slot_34 = {"associate_arts", 0, 0 };
    static const QUMethod slot_35 = {"unassociate_arts", 0, 0 };
    static const QUParameter param_slot_36[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_36 = {"stereotypeFilterActivated", 1, param_slot_36 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected },
	{ "default_description()", &slot_2, QMetaData::Protected },
	{ "edit_description()", &slot_3, QMetaData::Protected },
	{ "edStereotypeActivated(const QString&)", &slot_4, QMetaData::Protected },
	{ "update_tab(QWidget*)", &slot_5, QMetaData::Protected },
	{ "cpp_edit_h()", &slot_6, QMetaData::Protected },
	{ "cpp_update_h()", &slot_7, QMetaData::Protected },
	{ "cpp_default_h()", &slot_8, QMetaData::Protected },
	{ "cpp_unmapped_h()", &slot_9, QMetaData::Protected },
	{ "cpp_edit_src()", &slot_10, QMetaData::Protected },
	{ "cpp_update_src()", &slot_11, QMetaData::Protected },
	{ "cpp_default_src()", &slot_12, QMetaData::Protected },
	{ "cpp_unmapped_src()", &slot_13, QMetaData::Protected },
	{ "java_edit()", &slot_14, QMetaData::Protected },
	{ "java_update_src()", &slot_15, QMetaData::Protected },
	{ "java_default_src()", &slot_16, QMetaData::Protected },
	{ "java_unmapped_src()", &slot_17, QMetaData::Protected },
	{ "php_edit()", &slot_18, QMetaData::Protected },
	{ "php_update_src()", &slot_19, QMetaData::Protected },
	{ "php_default_src()", &slot_20, QMetaData::Protected },
	{ "php_unmapped_src()", &slot_21, QMetaData::Protected },
	{ "python_edit()", &slot_22, QMetaData::Protected },
	{ "python_update_src()", &slot_23, QMetaData::Protected },
	{ "python_default_src()", &slot_24, QMetaData::Protected },
	{ "python_unmapped_src()", &slot_25, QMetaData::Protected },
	{ "idl_edit()", &slot_26, QMetaData::Protected },
	{ "idl_update_src()", &slot_27, QMetaData::Protected },
	{ "idl_default_src()", &slot_28, QMetaData::Protected },
	{ "idl_unmapped_src()", &slot_29, QMetaData::Protected },
	{ "associate_cls()", &slot_30, QMetaData::Protected },
	{ "unassociate_cls()", &slot_31, QMetaData::Protected },
	{ "move_cls_up()", &slot_32, QMetaData::Protected },
	{ "move_cls_down()", &slot_33, QMetaData::Protected },
	{ "associate_arts()", &slot_34, QMetaData::Protected },
	{ "unassociate_arts()", &slot_35, QMetaData::Protected },
	{ "stereotypeFilterActivated(const QString&)", &slot_36, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ArtifactDialog", parentObject,
	slot_tbl, 37,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ArtifactDialog.setMetaObject( metaObj );
    return metaObj;
}

void* ArtifactDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ArtifactDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool ArtifactDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    case 2: default_description(); break;
    case 3: edit_description(); break;
    case 4: edStereotypeActivated((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: update_tab((QWidget*)static_QUType_ptr.get(_o+1)); break;
    case 6: cpp_edit_h(); break;
    case 7: cpp_update_h(); break;
    case 8: cpp_default_h(); break;
    case 9: cpp_unmapped_h(); break;
    case 10: cpp_edit_src(); break;
    case 11: cpp_update_src(); break;
    case 12: cpp_default_src(); break;
    case 13: cpp_unmapped_src(); break;
    case 14: java_edit(); break;
    case 15: java_update_src(); break;
    case 16: java_default_src(); break;
    case 17: java_unmapped_src(); break;
    case 18: php_edit(); break;
    case 19: php_update_src(); break;
    case 20: php_default_src(); break;
    case 21: php_unmapped_src(); break;
    case 22: python_edit(); break;
    case 23: python_update_src(); break;
    case 24: python_default_src(); break;
    case 25: python_unmapped_src(); break;
    case 26: idl_edit(); break;
    case 27: idl_update_src(); break;
    case 28: idl_default_src(); break;
    case 29: idl_unmapped_src(); break;
    case 30: associate_cls(); break;
    case 31: unassociate_cls(); break;
    case 32: move_cls_up(); break;
    case 33: move_cls_down(); break;
    case 34: associate_arts(); break;
    case 35: unassociate_arts(); break;
    case 36: stereotypeFilterActivated((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ArtifactDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ArtifactDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool ArtifactDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
