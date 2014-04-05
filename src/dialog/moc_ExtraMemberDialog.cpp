/****************************************************************************
** ExtraMemberDialog meta object code from reading C++ file 'ExtraMemberDialog.h'
**
** Created: Sat Apr 5 23:41:45 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ExtraMemberDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ExtraMemberDialog::className() const
{
    return "ExtraMemberDialog";
}

QMetaObject *ExtraMemberDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ExtraMemberDialog( "ExtraMemberDialog", &ExtraMemberDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ExtraMemberDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ExtraMemberDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ExtraMemberDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ExtraMemberDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ExtraMemberDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QUMethod slot_2 = {"edit_cpp_decl", 0, 0 };
    static const QUMethod slot_3 = {"edit_cpp_def", 0, 0 };
    static const QUMethod slot_4 = {"edit_java_decl", 0, 0 };
    static const QUMethod slot_5 = {"edit_php_decl", 0, 0 };
    static const QUMethod slot_6 = {"edit_python_decl", 0, 0 };
    static const QUMethod slot_7 = {"edit_idl_decl", 0, 0 };
    static const QUMethod slot_8 = {"edit_description", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected },
	{ "edit_cpp_decl()", &slot_2, QMetaData::Protected },
	{ "edit_cpp_def()", &slot_3, QMetaData::Protected },
	{ "edit_java_decl()", &slot_4, QMetaData::Protected },
	{ "edit_php_decl()", &slot_5, QMetaData::Protected },
	{ "edit_python_decl()", &slot_6, QMetaData::Protected },
	{ "edit_idl_decl()", &slot_7, QMetaData::Protected },
	{ "edit_description()", &slot_8, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ExtraMemberDialog", parentObject,
	slot_tbl, 9,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ExtraMemberDialog.setMetaObject( metaObj );
    return metaObj;
}

void* ExtraMemberDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ExtraMemberDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool ExtraMemberDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    case 2: edit_cpp_decl(); break;
    case 3: edit_cpp_def(); break;
    case 4: edit_java_decl(); break;
    case 5: edit_php_decl(); break;
    case 6: edit_python_decl(); break;
    case 7: edit_idl_decl(); break;
    case 8: edit_description(); break;
    default:
	return QTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ExtraMemberDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ExtraMemberDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool ExtraMemberDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
