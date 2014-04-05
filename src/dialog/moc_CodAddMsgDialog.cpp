/****************************************************************************
** CodAddMsgDialog meta object code from reading C++ file 'CodAddMsgDialog.h'
**
** Created: Sat Apr 5 23:41:57 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "CodAddMsgDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CodAddMsgDialog::className() const
{
    return "CodAddMsgDialog";
}

QMetaObject *CodAddMsgDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CodAddMsgDialog( "CodAddMsgDialog", &CodAddMsgDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CodAddMsgDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CodAddMsgDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CodAddMsgDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CodAddMsgDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CodAddMsgDialog::staticMetaObject()
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
	"CodAddMsgDialog", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CodAddMsgDialog.setMetaObject( metaObj );
    return metaObj;
}

void* CodAddMsgDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CodAddMsgDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool CodAddMsgDialog::qt_invoke( int _id, QUObject* _o )
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

bool CodAddMsgDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CodAddMsgDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool CodAddMsgDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
