/****************************************************************************
** ColDiagramWindow meta object code from reading C++ file 'ColDiagramWindow.h'
**
** Created: Sat Apr 5 23:41:32 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ColDiagramWindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ColDiagramWindow::className() const
{
    return "ColDiagramWindow";
}

QMetaObject *ColDiagramWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ColDiagramWindow( "ColDiagramWindow", &ColDiagramWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ColDiagramWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ColDiagramWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ColDiagramWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ColDiagramWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ColDiagramWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = DiagramWindow::staticMetaObject();
    static const QUMethod slot_0 = {"hit_class", 0, 0 };
    static const QUMethod slot_1 = {"hit_classinstance", 0, 0 };
    static const QUMethod slot_2 = {"hit_package", 0, 0 };
    static const QUMethod slot_3 = {"hit_fragment", 0, 0 };
    static const QUMethod slot_4 = {"hit_link", 0, 0 };
    static const QUMethod slot_5 = {"hit_selflink", 0, 0 };
    static const QUMethod slot_6 = {"hit_note", 0, 0 };
    static const QUMethod slot_7 = {"hit_anchor", 0, 0 };
    static const QUMethod slot_8 = {"hit_text", 0, 0 };
    static const QUMethod slot_9 = {"hit_image", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "hit_class()", &slot_0, QMetaData::Protected },
	{ "hit_classinstance()", &slot_1, QMetaData::Protected },
	{ "hit_package()", &slot_2, QMetaData::Protected },
	{ "hit_fragment()", &slot_3, QMetaData::Protected },
	{ "hit_link()", &slot_4, QMetaData::Protected },
	{ "hit_selflink()", &slot_5, QMetaData::Protected },
	{ "hit_note()", &slot_6, QMetaData::Protected },
	{ "hit_anchor()", &slot_7, QMetaData::Protected },
	{ "hit_text()", &slot_8, QMetaData::Protected },
	{ "hit_image()", &slot_9, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ColDiagramWindow", parentObject,
	slot_tbl, 10,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ColDiagramWindow.setMetaObject( metaObj );
    return metaObj;
}

void* ColDiagramWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ColDiagramWindow" ) )
	return this;
    return DiagramWindow::qt_cast( clname );
}

bool ColDiagramWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: hit_class(); break;
    case 1: hit_classinstance(); break;
    case 2: hit_package(); break;
    case 3: hit_fragment(); break;
    case 4: hit_link(); break;
    case 5: hit_selflink(); break;
    case 6: hit_note(); break;
    case 7: hit_anchor(); break;
    case 8: hit_text(); break;
    case 9: hit_image(); break;
    default:
	return DiagramWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ColDiagramWindow::qt_emit( int _id, QUObject* _o )
{
    return DiagramWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ColDiagramWindow::qt_property( int id, int f, QVariant* v)
{
    return DiagramWindow::qt_property( id, f, v);
}

bool ColDiagramWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
