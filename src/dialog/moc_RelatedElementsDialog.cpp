/****************************************************************************
** RelatedElementsDialog meta object code from reading C++ file 'RelatedElementsDialog.h'
**
** Created: Sat Apr 5 23:42:06 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "RelatedElementsDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *RelatedElementsDialog::className() const
{
    return "RelatedElementsDialog";
}

QMetaObject *RelatedElementsDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_RelatedElementsDialog( "RelatedElementsDialog", &RelatedElementsDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString RelatedElementsDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RelatedElementsDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString RelatedElementsDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RelatedElementsDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* RelatedElementsDialog::staticMetaObject()
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
	"RelatedElementsDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_RelatedElementsDialog.setMetaObject( metaObj );
    return metaObj;
}

void* RelatedElementsDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "RelatedElementsDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool RelatedElementsDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: accept(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool RelatedElementsDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool RelatedElementsDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool RelatedElementsDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
