/****************************************************************************
** MLEDialog meta object code from reading C++ file 'MLEDialog.h'
**
** Created: Sat Apr 5 23:41:49 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MLEDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MLEDialog::className() const
{
    return "MLEDialog";
}

QMetaObject *MLEDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MLEDialog( "MLEDialog", &MLEDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MLEDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MLEDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MLEDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MLEDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MLEDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"MLEDialog", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MLEDialog.setMetaObject( metaObj );
    return metaObj;
}

void* MLEDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MLEDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool MLEDialog::qt_invoke( int _id, QUObject* _o )
{
    return QDialog::qt_invoke(_id,_o);
}

bool MLEDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MLEDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool MLEDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
