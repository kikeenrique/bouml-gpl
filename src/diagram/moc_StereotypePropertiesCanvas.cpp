/****************************************************************************
** StereotypePropertiesCanvas meta object code from reading C++ file 'StereotypePropertiesCanvas.h'
**
** Created: Sat Apr 5 23:41:44 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "StereotypePropertiesCanvas.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *StereotypePropertiesCanvas::className() const
{
    return "StereotypePropertiesCanvas";
}

QMetaObject *StereotypePropertiesCanvas::metaObj = 0;
static QMetaObjectCleanUp cleanUp_StereotypePropertiesCanvas( "StereotypePropertiesCanvas", &StereotypePropertiesCanvas::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString StereotypePropertiesCanvas::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StereotypePropertiesCanvas", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString StereotypePropertiesCanvas::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StereotypePropertiesCanvas", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* StereotypePropertiesCanvas::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = NoteCanvas::staticMetaObject();
    static const QUMethod slot_0 = {"update", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "update()", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"StereotypePropertiesCanvas", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_StereotypePropertiesCanvas.setMetaObject( metaObj );
    return metaObj;
}

void* StereotypePropertiesCanvas::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "StereotypePropertiesCanvas" ) )
	return this;
    return NoteCanvas::qt_cast( clname );
}

bool StereotypePropertiesCanvas::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: update(); break;
    default:
	return NoteCanvas::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool StereotypePropertiesCanvas::qt_emit( int _id, QUObject* _o )
{
    return NoteCanvas::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool StereotypePropertiesCanvas::qt_property( int id, int f, QVariant* v)
{
    return NoteCanvas::qt_property( id, f, v);
}

bool StereotypePropertiesCanvas::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
