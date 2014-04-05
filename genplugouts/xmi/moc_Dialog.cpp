/****************************************************************************
** Dialog meta object code from reading C++ file 'Dialog.h'
**
** Created: Sat Apr 5 23:53:30 2014
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
    static const QUMethod slot_0 = {"browse", 0, 0 };
    static const QUMethod slot_1 = {"accept_cpp", 0, 0 };
    static const QUMethod slot_2 = {"accept_uml", 0, 0 };
    static const QUMethod slot_3 = {"accept_java", 0, 0 };
    static const QUMethod slot_4 = {"accept", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "browse()", &slot_0, QMetaData::Protected },
	{ "accept_cpp()", &slot_1, QMetaData::Protected },
	{ "accept_uml()", &slot_2, QMetaData::Protected },
	{ "accept_java()", &slot_3, QMetaData::Protected },
	{ "accept()", &slot_4, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"Dialog", parentObject,
	slot_tbl, 5,
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
    case 0: browse(); break;
    case 1: accept_cpp(); break;
    case 2: accept_uml(); break;
    case 3: accept_java(); break;
    case 4: accept(); break;
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
