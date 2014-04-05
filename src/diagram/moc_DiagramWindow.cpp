/****************************************************************************
** DiagramWindow meta object code from reading C++ file 'DiagramWindow.h'
**
** Created: Sat Apr 5 23:41:21 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "DiagramWindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *DiagramWindow::className() const
{
    return "DiagramWindow";
}

QMetaObject *DiagramWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_DiagramWindow( "DiagramWindow", &DiagramWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString DiagramWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DiagramWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString DiagramWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DiagramWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* DiagramWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"new_scale", 1, param_slot_0 };
    static const QUMethod slot_1 = {"fit_scale", 0, 0 };
    static const QUMethod slot_2 = {"optimal_window_size", 0, 0 };
    static const QUMethod slot_3 = {"session_window_size", 0, 0 };
    static const QUMethod slot_4 = {"call_menu", 0, 0 };
    static const QUMethod slot_5 = {"hit_select", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "new_scale(int)", &slot_0, QMetaData::Public },
	{ "fit_scale()", &slot_1, QMetaData::Public },
	{ "optimal_window_size()", &slot_2, QMetaData::Public },
	{ "session_window_size()", &slot_3, QMetaData::Public },
	{ "call_menu()", &slot_4, QMetaData::Public },
	{ "hit_select()", &slot_5, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"DiagramWindow", parentObject,
	slot_tbl, 6,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_DiagramWindow.setMetaObject( metaObj );
    return metaObj;
}

void* DiagramWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "DiagramWindow" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool DiagramWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: new_scale((int)static_QUType_int.get(_o+1)); break;
    case 1: fit_scale(); break;
    case 2: optimal_window_size(); break;
    case 3: session_window_size(); break;
    case 4: call_menu(); break;
    case 5: hit_select(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool DiagramWindow::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool DiagramWindow::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool DiagramWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
