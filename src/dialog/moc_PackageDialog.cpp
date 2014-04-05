/****************************************************************************
** PackageDialog meta object code from reading C++ file 'PackageDialog.h'
**
** Created: Sat Apr 5 23:41:51 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "PackageDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *PackageDialog::className() const
{
    return "PackageDialog";
}

QMetaObject *PackageDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_PackageDialog( "PackageDialog", &PackageDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString PackageDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PackageDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString PackageDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PackageDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* PackageDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QUMethod slot_2 = {"edit_description", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"edStereotypeActivated", 1, param_slot_3 };
    static const QUMethod slot_4 = {"cppsrc_browse", 0, 0 };
    static const QUMethod slot_5 = {"cpph_browse", 0, 0 };
    static const QUMethod slot_6 = {"java_browse", 0, 0 };
    static const QUMethod slot_7 = {"php_browse", 0, 0 };
    static const QUMethod slot_8 = {"python_browse", 0, 0 };
    static const QUMethod slot_9 = {"idl_browse", 0, 0 };
    static const QUMethod slot_10 = {"cppsrc_relative", 0, 0 };
    static const QUMethod slot_11 = {"cpph_relative", 0, 0 };
    static const QUMethod slot_12 = {"java_relative", 0, 0 };
    static const QUMethod slot_13 = {"php_relative", 0, 0 };
    static const QUMethod slot_14 = {"python_relative", 0, 0 };
    static const QUMethod slot_15 = {"idl_relative", 0, 0 };
    static const QUParameter param_slot_16[] = {
	{ 0, &static_QUType_ptr, "QWidget", QUParameter::In }
    };
    static const QUMethod slot_16 = {"change_tabs", 1, param_slot_16 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected },
	{ "edit_description()", &slot_2, QMetaData::Protected },
	{ "edStereotypeActivated(const QString&)", &slot_3, QMetaData::Protected },
	{ "cppsrc_browse()", &slot_4, QMetaData::Protected },
	{ "cpph_browse()", &slot_5, QMetaData::Protected },
	{ "java_browse()", &slot_6, QMetaData::Protected },
	{ "php_browse()", &slot_7, QMetaData::Protected },
	{ "python_browse()", &slot_8, QMetaData::Protected },
	{ "idl_browse()", &slot_9, QMetaData::Protected },
	{ "cppsrc_relative()", &slot_10, QMetaData::Protected },
	{ "cpph_relative()", &slot_11, QMetaData::Protected },
	{ "java_relative()", &slot_12, QMetaData::Protected },
	{ "php_relative()", &slot_13, QMetaData::Protected },
	{ "python_relative()", &slot_14, QMetaData::Protected },
	{ "idl_relative()", &slot_15, QMetaData::Protected },
	{ "change_tabs(QWidget*)", &slot_16, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"PackageDialog", parentObject,
	slot_tbl, 17,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_PackageDialog.setMetaObject( metaObj );
    return metaObj;
}

void* PackageDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "PackageDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool PackageDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    case 2: edit_description(); break;
    case 3: edStereotypeActivated((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: cppsrc_browse(); break;
    case 5: cpph_browse(); break;
    case 6: java_browse(); break;
    case 7: php_browse(); break;
    case 8: python_browse(); break;
    case 9: idl_browse(); break;
    case 10: cppsrc_relative(); break;
    case 11: cpph_relative(); break;
    case 12: java_relative(); break;
    case 13: php_relative(); break;
    case 14: python_relative(); break;
    case 15: idl_relative(); break;
    case 16: change_tabs((QWidget*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool PackageDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool PackageDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool PackageDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
