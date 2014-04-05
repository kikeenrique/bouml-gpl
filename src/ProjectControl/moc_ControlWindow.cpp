/****************************************************************************
** ControlWindow meta object code from reading C++ file 'ControlWindow.h'
**
** Created: Sat Apr 5 23:49:49 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ControlWindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ControlWindow::className() const
{
    return "ControlWindow";
}

QMetaObject *ControlWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ControlWindow( "ControlWindow", &ControlWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ControlWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ControlWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ControlWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ControlWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ControlWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"load", 0, 0 };
    static const QUMethod slot_1 = {"quit", 0, 0 };
    static const QUMethod slot_2 = {"browser_search", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"historicActivated", 1, param_slot_3 };
    static const QUMethod slot_4 = {"change_user", 0, 0 };
    static const QUMethod slot_5 = {"motif_style", 0, 0 };
    static const QUMethod slot_6 = {"motifplus_style", 0, 0 };
    static const QUMethod slot_7 = {"windows_style", 0, 0 };
    static const QUMethod slot_8 = {"about", 0, 0 };
    static const QUMethod slot_9 = {"aboutQt", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "load()", &slot_0, QMetaData::Private },
	{ "quit()", &slot_1, QMetaData::Private },
	{ "browser_search()", &slot_2, QMetaData::Private },
	{ "historicActivated(int)", &slot_3, QMetaData::Private },
	{ "change_user()", &slot_4, QMetaData::Private },
	{ "motif_style()", &slot_5, QMetaData::Private },
	{ "motifplus_style()", &slot_6, QMetaData::Private },
	{ "windows_style()", &slot_7, QMetaData::Private },
	{ "about()", &slot_8, QMetaData::Private },
	{ "aboutQt()", &slot_9, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"ControlWindow", parentObject,
	slot_tbl, 10,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ControlWindow.setMetaObject( metaObj );
    return metaObj;
}

void* ControlWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ControlWindow" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool ControlWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: load(); break;
    case 1: quit(); break;
    case 2: browser_search(); break;
    case 3: historicActivated((int)static_QUType_int.get(_o+1)); break;
    case 4: change_user(); break;
    case 5: motif_style(); break;
    case 6: motifplus_style(); break;
    case 7: windows_style(); break;
    case 8: about(); break;
    case 9: aboutQt(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ControlWindow::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ControlWindow::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool ControlWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
