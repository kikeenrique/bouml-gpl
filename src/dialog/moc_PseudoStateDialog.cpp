/****************************************************************************
** PseudoStateDialog meta object code from reading C++ file 'PseudoStateDialog.h'
**
** Created: Sat Apr 5 23:41:53 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "PseudoStateDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *PseudoStateDialog::className() const
{
    return "PseudoStateDialog";
}

QMetaObject *PseudoStateDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_PseudoStateDialog( "PseudoStateDialog", &PseudoStateDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString PseudoStateDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PseudoStateDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString PseudoStateDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PseudoStateDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* PseudoStateDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"accept", 0, 0 };
    static const QUMethod slot_2 = {"menu_reference", 0, 0 };
    static const QUMethod slot_3 = {"edit_description", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "accept()", &slot_1, QMetaData::Protected },
	{ "menu_reference()", &slot_2, QMetaData::Protected },
	{ "edit_description()", &slot_3, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"PseudoStateDialog", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_PseudoStateDialog.setMetaObject( metaObj );
    return metaObj;
}

void* PseudoStateDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "PseudoStateDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool PseudoStateDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    case 2: menu_reference(); break;
    case 3: edit_description(); break;
    default:
	return QTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool PseudoStateDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool PseudoStateDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool PseudoStateDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
