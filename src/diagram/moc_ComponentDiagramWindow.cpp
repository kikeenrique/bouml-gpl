/****************************************************************************
** ComponentDiagramWindow meta object code from reading C++ file 'ComponentDiagramWindow.h'
**
** Created: Sat Apr 5 23:41:35 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ComponentDiagramWindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ComponentDiagramWindow::className() const
{
    return "ComponentDiagramWindow";
}

QMetaObject *ComponentDiagramWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ComponentDiagramWindow( "ComponentDiagramWindow", &ComponentDiagramWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ComponentDiagramWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ComponentDiagramWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ComponentDiagramWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ComponentDiagramWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ComponentDiagramWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = DiagramWindow::staticMetaObject();
    static const QUMethod slot_0 = {"hit_component", 0, 0 };
    static const QUMethod slot_1 = {"hit_package", 0, 0 };
    static const QUMethod slot_2 = {"hit_fragment", 0, 0 };
    static const QUMethod slot_3 = {"hit_inherit", 0, 0 };
    static const QUMethod slot_4 = {"hit_dependency", 0, 0 };
    static const QUMethod slot_5 = {"hit_required", 0, 0 };
    static const QUMethod slot_6 = {"hit_provided", 0, 0 };
    static const QUMethod slot_7 = {"hit_note", 0, 0 };
    static const QUMethod slot_8 = {"hit_anchor", 0, 0 };
    static const QUMethod slot_9 = {"hit_text", 0, 0 };
    static const QUMethod slot_10 = {"hit_image", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "hit_component()", &slot_0, QMetaData::Protected },
	{ "hit_package()", &slot_1, QMetaData::Protected },
	{ "hit_fragment()", &slot_2, QMetaData::Protected },
	{ "hit_inherit()", &slot_3, QMetaData::Protected },
	{ "hit_dependency()", &slot_4, QMetaData::Protected },
	{ "hit_required()", &slot_5, QMetaData::Protected },
	{ "hit_provided()", &slot_6, QMetaData::Protected },
	{ "hit_note()", &slot_7, QMetaData::Protected },
	{ "hit_anchor()", &slot_8, QMetaData::Protected },
	{ "hit_text()", &slot_9, QMetaData::Protected },
	{ "hit_image()", &slot_10, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ComponentDiagramWindow", parentObject,
	slot_tbl, 11,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ComponentDiagramWindow.setMetaObject( metaObj );
    return metaObj;
}

void* ComponentDiagramWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ComponentDiagramWindow" ) )
	return this;
    return DiagramWindow::qt_cast( clname );
}

bool ComponentDiagramWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: hit_component(); break;
    case 1: hit_package(); break;
    case 2: hit_fragment(); break;
    case 3: hit_inherit(); break;
    case 4: hit_dependency(); break;
    case 5: hit_required(); break;
    case 6: hit_provided(); break;
    case 7: hit_note(); break;
    case 8: hit_anchor(); break;
    case 9: hit_text(); break;
    case 10: hit_image(); break;
    default:
	return DiagramWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ComponentDiagramWindow::qt_emit( int _id, QUObject* _o )
{
    return DiagramWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ComponentDiagramWindow::qt_property( int id, int f, QVariant* v)
{
    return DiagramWindow::qt_property( id, f, v);
}

bool ComponentDiagramWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
