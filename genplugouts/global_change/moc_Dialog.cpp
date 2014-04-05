/****************************************************************************
** Dialog meta object code from reading C++ file 'Dialog.h'
**
** Created: Sat Apr 5 23:59:52 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "Dialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#include <qvariant.h>
const char *Dialog::className() const
{
    return "Dialog";
}

QMetaObject *Dialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Dialog( "Dialog", &Dialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Dialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Dialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Dialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Dialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Dialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"do_replace", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_QVariant, "\x14", QUParameter::Out },
	{ "s", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"digest", 2, param_slot_1 };
    static const QUMethod slot_2 = {"polish", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "do_replace()", &slot_0, QMetaData::Protected },
	{ "digest(const QString)", &slot_1, QMetaData::Protected },
	{ "polish()", &slot_2, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"Dialog", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Dialog.setMetaObject( metaObj );
    return metaObj;
}

void* Dialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Dialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool Dialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: do_replace(); break;
    case 1: static_QUType_QVariant.set(_o,QVariant(digest((const QString)static_QUType_QString.get(_o+1)))); break;
    case 2: polish(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Dialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Dialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool Dialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
