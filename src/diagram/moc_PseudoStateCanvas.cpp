/****************************************************************************
** PseudoStateCanvas meta object code from reading C++ file 'PseudoStateCanvas.h'
**
** Created: Sat Apr 5 23:41:39 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "PseudoStateCanvas.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *PseudoStateCanvas::className() const
{
    return "PseudoStateCanvas";
}

QMetaObject *PseudoStateCanvas::metaObj = 0;
static QMetaObjectCleanUp cleanUp_PseudoStateCanvas( "PseudoStateCanvas", &PseudoStateCanvas::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString PseudoStateCanvas::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PseudoStateCanvas", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString PseudoStateCanvas::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PseudoStateCanvas", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* PseudoStateCanvas::staticMetaObject()
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
	"PseudoStateCanvas", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_PseudoStateCanvas.setMetaObject( metaObj );
    return metaObj;
}

void* PseudoStateCanvas::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "PseudoStateCanvas" ) )
	return this;
    if ( !qstrcmp( clname, "DiagramCanvas" ) )
	return (DiagramCanvas*)this;
    return QObject::qt_cast( clname );
}

bool PseudoStateCanvas::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: modified(); break;
    case 1: deleted(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool PseudoStateCanvas::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool PseudoStateCanvas::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool PseudoStateCanvas::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
