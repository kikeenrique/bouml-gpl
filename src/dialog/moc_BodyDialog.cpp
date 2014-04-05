/****************************************************************************
** BodyDialog meta object code from reading C++ file 'BodyDialog.h'
**
** Created: Sat Apr 5 23:41:50 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "BodyDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *BodyDialog::className() const
{
    return "BodyDialog";
}

QMetaObject *BodyDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_BodyDialog( "BodyDialog", &BodyDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString BodyDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BodyDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString BodyDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BodyDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* BodyDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"BodyDialog", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_BodyDialog.setMetaObject( metaObj );
    return metaObj;
}

void* BodyDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "BodyDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool BodyDialog::qt_invoke( int _id, QUObject* _o )
{
    return QDialog::qt_invoke(_id,_o);
}

bool BodyDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool BodyDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool BodyDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
