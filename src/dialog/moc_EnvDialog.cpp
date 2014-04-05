/****************************************************************************
** EnvDialog meta object code from reading C++ file 'EnvDialog.h'
**
** Created: Sat Apr 5 23:42:05 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "EnvDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *EnvDialog::className() const
{
    return "EnvDialog";
}

QMetaObject *EnvDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_EnvDialog( "EnvDialog", &EnvDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString EnvDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "EnvDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString EnvDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "EnvDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* EnvDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"accept", 0, 0 };
    static const QUMethod slot_1 = {"reject", 0, 0 };
    static const QUMethod slot_2 = {"doc_browse", 0, 0 };
    static const QUMethod slot_3 = {"navigator_browse", 0, 0 };
    static const QUMethod slot_4 = {"template_browse", 0, 0 };
    static const QUMethod slot_5 = {"editor_browse", 0, 0 };
    static const QUMethod slot_6 = {"lang_browse", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "accept()", &slot_0, QMetaData::Private },
	{ "reject()", &slot_1, QMetaData::Private },
	{ "doc_browse()", &slot_2, QMetaData::Private },
	{ "navigator_browse()", &slot_3, QMetaData::Private },
	{ "template_browse()", &slot_4, QMetaData::Private },
	{ "editor_browse()", &slot_5, QMetaData::Private },
	{ "lang_browse()", &slot_6, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"EnvDialog", parentObject,
	slot_tbl, 7,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_EnvDialog.setMetaObject( metaObj );
    return metaObj;
}

void* EnvDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "EnvDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool EnvDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: accept(); break;
    case 1: reject(); break;
    case 2: doc_browse(); break;
    case 3: navigator_browse(); break;
    case 4: template_browse(); break;
    case 5: editor_browse(); break;
    case 6: lang_browse(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool EnvDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool EnvDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool EnvDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
