/****************************************************************************
** GreetingsDialog meta object code from reading C++ file 'GreetingsDialog.h'
**
** Created: Sat Apr 5 23:42:06 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "GreetingsDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GreetingsDialog::className() const
{
    return "GreetingsDialog";
}

QMetaObject *GreetingsDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GreetingsDialog( "GreetingsDialog", &GreetingsDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GreetingsDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GreetingsDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GreetingsDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GreetingsDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GreetingsDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "e", &static_QUType_ptr, "QCloseEvent", QUParameter::In }
    };
    static const QUMethod slot_0 = {"closeEvent", 1, param_slot_0 };
    static const QUMethod slot_1 = {"valid_ok", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "closeEvent(QCloseEvent*)", &slot_0, QMetaData::Protected },
	{ "valid_ok()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"GreetingsDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GreetingsDialog.setMetaObject( metaObj );
    return metaObj;
}

void* GreetingsDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GreetingsDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool GreetingsDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: closeEvent((QCloseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 1: valid_ok(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GreetingsDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool GreetingsDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool GreetingsDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
