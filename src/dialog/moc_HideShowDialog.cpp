/****************************************************************************
** HideShowDialog meta object code from reading C++ file 'HideShowDialog.h'
**
** Created: Sat Apr 5 23:41:58 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "HideShowDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *HideShowDialog::className() const
{
    return "HideShowDialog";
}

QMetaObject *HideShowDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_HideShowDialog( "HideShowDialog", &HideShowDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString HideShowDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "HideShowDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString HideShowDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "HideShowDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* HideShowDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"hide_them", 0, 0 };
    static const QUMethod slot_2 = {"show_them", 0, 0 };
    static const QUMethod slot_3 = {"show_all", 0, 0 };
    static const QUMethod slot_4 = {"hide_all", 0, 0 };
    static const QUMethod slot_5 = {"hide_private", 0, 0 };
    static const QUMethod slot_6 = {"hide_private_protected", 0, 0 };
    static const QUMethod slot_7 = {"accept", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "hide_them()", &slot_1, QMetaData::Protected },
	{ "show_them()", &slot_2, QMetaData::Protected },
	{ "show_all()", &slot_3, QMetaData::Protected },
	{ "hide_all()", &slot_4, QMetaData::Protected },
	{ "hide_private()", &slot_5, QMetaData::Protected },
	{ "hide_private_protected()", &slot_6, QMetaData::Protected },
	{ "accept()", &slot_7, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"HideShowDialog", parentObject,
	slot_tbl, 8,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_HideShowDialog.setMetaObject( metaObj );
    return metaObj;
}

void* HideShowDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "HideShowDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool HideShowDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: hide_them(); break;
    case 2: show_them(); break;
    case 3: show_all(); break;
    case 4: hide_all(); break;
    case 5: hide_private(); break;
    case 6: hide_private_protected(); break;
    case 7: accept(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool HideShowDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool HideShowDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool HideShowDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
