/****************************************************************************
** CdClassCanvas meta object code from reading C++ file 'CdClassCanvas.h'
**
** Created: Sat Apr 5 23:41:20 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "CdClassCanvas.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CdClassCanvas::className() const
{
    return "CdClassCanvas";
}

QMetaObject *CdClassCanvas::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CdClassCanvas( "CdClassCanvas", &CdClassCanvas::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CdClassCanvas::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CdClassCanvas", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CdClassCanvas::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CdClassCanvas", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CdClassCanvas::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"modified", 0, 0 };
    static const QUMethod slot_1 = {"deleted", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "modified()", &slot_0, QMetaData::Private },
	{ "deleted()", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"CdClassCanvas", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CdClassCanvas.setMetaObject( metaObj );
    return metaObj;
}

void* CdClassCanvas::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CdClassCanvas" ) )
	return this;
    if ( !qstrcmp( clname, "DiagramCanvas" ) )
	return (DiagramCanvas*)this;
    if ( !qstrcmp( clname, "MultipleDependency<BasicData>" ) )
	return (MultipleDependency<BasicData>*)this;
    return QObject::qt_cast( clname );
}

bool CdClassCanvas::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: modified(); break;
    case 1: deleted(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CdClassCanvas::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CdClassCanvas::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool CdClassCanvas::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
