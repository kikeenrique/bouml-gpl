/****************************************************************************
** BrowserSearchDialog meta object code from reading C++ file 'BrowserSearchDialog.h'
**
** Created: Sat Apr 5 23:46:37 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "BrowserSearchDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *BrowserSearchDialog::className() const
{
    return "BrowserSearchDialog";
}

QMetaObject *BrowserSearchDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_BrowserSearchDialog( "BrowserSearchDialog", &BrowserSearchDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString BrowserSearchDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BrowserSearchDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString BrowserSearchDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BrowserSearchDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* BrowserSearchDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"search", 0, 0 };
    static const QUMethod slot_1 = {"select", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "search()", &slot_0, QMetaData::Protected },
	{ "select()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"BrowserSearchDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_BrowserSearchDialog.setMetaObject( metaObj );
    return metaObj;
}

void* BrowserSearchDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "BrowserSearchDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool BrowserSearchDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: search(); break;
    case 1: select(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool BrowserSearchDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool BrowserSearchDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool BrowserSearchDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
