/****************************************************************************
** ImageDialog meta object code from reading C++ file 'ImageDialog.h'
**
** Created: Sat Apr 5 23:42:07 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ImageDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ImageDialog::className() const
{
    return "ImageDialog";
}

QMetaObject *ImageDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ImageDialog( "ImageDialog", &ImageDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ImageDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ImageDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ImageDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ImageDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ImageDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"accept", 0, 0 };
    static const QUMethod slot_1 = {"browse", 0, 0 };
    static const QUMethod slot_2 = {"root_relative", 0, 0 };
    static const QUMethod slot_3 = {"prj_relative", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "accept()", &slot_0, QMetaData::Protected },
	{ "browse()", &slot_1, QMetaData::Protected },
	{ "root_relative()", &slot_2, QMetaData::Protected },
	{ "prj_relative()", &slot_3, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ImageDialog", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ImageDialog.setMetaObject( metaObj );
    return metaObj;
}

void* ImageDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ImageDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool ImageDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: accept(); break;
    case 1: browse(); break;
    case 2: root_relative(); break;
    case 3: prj_relative(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ImageDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ImageDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool ImageDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
