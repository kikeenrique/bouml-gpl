/****************************************************************************
** RelationCanvas meta object code from reading C++ file 'RelationCanvas.h'
**
** Created: Sat Apr 5 23:41:24 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "RelationCanvas.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *RelationCanvas::className() const
{
    return "RelationCanvas";
}

QMetaObject *RelationCanvas::metaObj = 0;
static QMetaObjectCleanUp cleanUp_RelationCanvas( "RelationCanvas", &RelationCanvas::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString RelationCanvas::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RelationCanvas", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString RelationCanvas::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RelationCanvas", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* RelationCanvas::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = ArrowCanvas::staticMetaObject();
    static const QUMethod slot_0 = {"modified", 0, 0 };
    static const QUMethod slot_1 = {"actuals_modified", 0, 0 };
    static const QUMethod slot_2 = {"deleted", 0, 0 };
    static const QUMethod slot_3 = {"drawing_settings_modified", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "modified()", &slot_0, QMetaData::Private },
	{ "actuals_modified()", &slot_1, QMetaData::Private },
	{ "deleted()", &slot_2, QMetaData::Private },
	{ "drawing_settings_modified()", &slot_3, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"RelationCanvas", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_RelationCanvas.setMetaObject( metaObj );
    return metaObj;
}

void* RelationCanvas::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "RelationCanvas" ) )
	return this;
    return ArrowCanvas::qt_cast( clname );
}

bool RelationCanvas::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: modified(); break;
    case 1: actuals_modified(); break;
    case 2: deleted(); break;
    case 3: drawing_settings_modified(); break;
    default:
	return ArrowCanvas::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool RelationCanvas::qt_emit( int _id, QUObject* _o )
{
    return ArrowCanvas::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool RelationCanvas::qt_property( int id, int f, QVariant* v)
{
    return ArrowCanvas::qt_property( id, f, v);
}

bool RelationCanvas::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
