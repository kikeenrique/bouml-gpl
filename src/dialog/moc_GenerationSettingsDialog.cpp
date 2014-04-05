/****************************************************************************
** GenerationSettingsDialog meta object code from reading C++ file 'GenerationSettingsDialog.h'
**
** Created: Sat Apr 5 23:41:49 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "GenerationSettingsDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GenerationSettingsDialog::className() const
{
    return "GenerationSettingsDialog";
}

QMetaObject *GenerationSettingsDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GenerationSettingsDialog( "GenerationSettingsDialog", &GenerationSettingsDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GenerationSettingsDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GenerationSettingsDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GenerationSettingsDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GenerationSettingsDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GenerationSettingsDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QUMethod slot_2 = {"cpproot_browse", 0, 0 };
    static const QUMethod slot_3 = {"javaroot_browse", 0, 0 };
    static const QUMethod slot_4 = {"phproot_browse", 0, 0 };
    static const QUMethod slot_5 = {"pythonroot_browse", 0, 0 };
    static const QUMethod slot_6 = {"idlroot_browse", 0, 0 };
    static const QUMethod slot_7 = {"cpp_relative", 0, 0 };
    static const QUMethod slot_8 = {"java_relative", 0, 0 };
    static const QUMethod slot_9 = {"php_relative", 0, 0 };
    static const QUMethod slot_10 = {"python_relative", 0, 0 };
    static const QUMethod slot_11 = {"idl_relative", 0, 0 };
    static const QUParameter param_slot_12[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"java_get_visi_changed", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"java_set_visi_changed", 1, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"php_get_visi_changed", 1, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"php_set_visi_changed", 1, param_slot_15 };
    static const QUMethod slot_16 = {"follow_cpp_get_name", 0, 0 };
    static const QUMethod slot_17 = {"follow_cpp_set_name", 0, 0 };
    static const QUMethod slot_18 = {"follow_java_get_name", 0, 0 };
    static const QUMethod slot_19 = {"follow_java_set_name", 0, 0 };
    static const QUMethod slot_20 = {"follow_php_get_name", 0, 0 };
    static const QUMethod slot_21 = {"follow_php_set_name", 0, 0 };
    static const QUMethod slot_22 = {"follow_python_get_name", 0, 0 };
    static const QUMethod slot_23 = {"follow_python_set_name", 0, 0 };
    static const QUMethod slot_24 = {"follow_idl_get_name", 0, 0 };
    static const QUMethod slot_25 = {"follow_idl_set_name", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected },
	{ "cpproot_browse()", &slot_2, QMetaData::Protected },
	{ "javaroot_browse()", &slot_3, QMetaData::Protected },
	{ "phproot_browse()", &slot_4, QMetaData::Protected },
	{ "pythonroot_browse()", &slot_5, QMetaData::Protected },
	{ "idlroot_browse()", &slot_6, QMetaData::Protected },
	{ "cpp_relative()", &slot_7, QMetaData::Protected },
	{ "java_relative()", &slot_8, QMetaData::Protected },
	{ "php_relative()", &slot_9, QMetaData::Protected },
	{ "python_relative()", &slot_10, QMetaData::Protected },
	{ "idl_relative()", &slot_11, QMetaData::Protected },
	{ "java_get_visi_changed(int)", &slot_12, QMetaData::Protected },
	{ "java_set_visi_changed(int)", &slot_13, QMetaData::Protected },
	{ "php_get_visi_changed(int)", &slot_14, QMetaData::Protected },
	{ "php_set_visi_changed(int)", &slot_15, QMetaData::Protected },
	{ "follow_cpp_get_name()", &slot_16, QMetaData::Protected },
	{ "follow_cpp_set_name()", &slot_17, QMetaData::Protected },
	{ "follow_java_get_name()", &slot_18, QMetaData::Protected },
	{ "follow_java_set_name()", &slot_19, QMetaData::Protected },
	{ "follow_php_get_name()", &slot_20, QMetaData::Protected },
	{ "follow_php_set_name()", &slot_21, QMetaData::Protected },
	{ "follow_python_get_name()", &slot_22, QMetaData::Protected },
	{ "follow_python_set_name()", &slot_23, QMetaData::Protected },
	{ "follow_idl_get_name()", &slot_24, QMetaData::Protected },
	{ "follow_idl_set_name()", &slot_25, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"GenerationSettingsDialog", parentObject,
	slot_tbl, 26,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GenerationSettingsDialog.setMetaObject( metaObj );
    return metaObj;
}

void* GenerationSettingsDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GenerationSettingsDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool GenerationSettingsDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    case 2: cpproot_browse(); break;
    case 3: javaroot_browse(); break;
    case 4: phproot_browse(); break;
    case 5: pythonroot_browse(); break;
    case 6: idlroot_browse(); break;
    case 7: cpp_relative(); break;
    case 8: java_relative(); break;
    case 9: php_relative(); break;
    case 10: python_relative(); break;
    case 11: idl_relative(); break;
    case 12: java_get_visi_changed((int)static_QUType_int.get(_o+1)); break;
    case 13: java_set_visi_changed((int)static_QUType_int.get(_o+1)); break;
    case 14: php_get_visi_changed((int)static_QUType_int.get(_o+1)); break;
    case 15: php_set_visi_changed((int)static_QUType_int.get(_o+1)); break;
    case 16: follow_cpp_get_name(); break;
    case 17: follow_cpp_set_name(); break;
    case 18: follow_java_get_name(); break;
    case 19: follow_java_set_name(); break;
    case 20: follow_php_get_name(); break;
    case 21: follow_php_set_name(); break;
    case 22: follow_python_get_name(); break;
    case 23: follow_python_set_name(); break;
    case 24: follow_idl_get_name(); break;
    case 25: follow_idl_set_name(); break;
    default:
	return QTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GenerationSettingsDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool GenerationSettingsDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool GenerationSettingsDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
