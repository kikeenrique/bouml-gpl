/****************************************************************************
** SdMsgDialog meta object code from reading C++ file 'SdMsgDialog.h'
**
** Created: Sat Apr 5 23:41:55 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "SdMsgDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *SdMsgDialog::className() const
{
    return "SdMsgDialog";
}

QMetaObject *SdMsgDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SdMsgDialog( "SdMsgDialog", &SdMsgDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SdMsgDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SdMsgDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SdMsgDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SdMsgDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SdMsgDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"menu_op", 0, 0 };
    static const QUMethod slot_1 = {"polish", 0, 0 };
    static const QUMethod slot_2 = {"accept", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "menu_op()", &slot_0, QMetaData::Protected },
	{ "polish()", &slot_1, QMetaData::Protected },
	{ "accept()", &slot_2, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"SdMsgDialog", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SdMsgDialog.setMetaObject( metaObj );
    return metaObj;
}

void* SdMsgDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SdMsgDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool SdMsgDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: menu_op(); break;
    case 1: polish(); break;
    case 2: accept(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool SdMsgDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool SdMsgDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool SdMsgDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
