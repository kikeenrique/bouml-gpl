/****************************************************************************
** StereotypeDialog meta object code from reading C++ file 'StereotypeDialog.h'
**
** Created: Sat Apr 5 23:41:55 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "StereotypeDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *StereotypeDialog::className() const
{
    return "StereotypeDialog";
}

QMetaObject *StereotypeDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_StereotypeDialog( "StereotypeDialog", &StereotypeDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString StereotypeDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StereotypeDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString StereotypeDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StereotypeDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* StereotypeDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"StereotypeDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_StereotypeDialog.setMetaObject( metaObj );
    return metaObj;
}

void* StereotypeDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "StereotypeDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool StereotypeDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool StereotypeDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool StereotypeDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool StereotypeDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
