/****************************************************************************
** JavaCatWindow meta object code from reading C++ file 'JavaCatWindow.h'
**
** Created: Sat Apr 5 23:46:37 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "JavaCatWindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *JavaCatWindow::className() const
{
    return "JavaCatWindow";
}

QMetaObject *JavaCatWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_JavaCatWindow( "JavaCatWindow", &JavaCatWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString JavaCatWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JavaCatWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString JavaCatWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JavaCatWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* JavaCatWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"load", 0, 0 };
    static const QUMethod slot_1 = {"save", 0, 0 };
    static const QUMethod slot_2 = {"quit", 0, 0 };
    static const QUMethod slot_3 = {"scan", 0, 0 };
    static const QUMethod slot_4 = {"browser_search", 0, 0 };
    static const QUMethod slot_5 = {"historic_back", 0, 0 };
    static const QUMethod slot_6 = {"historic_forward", 0, 0 };
    static const QUMethod slot_7 = {"motif_style", 0, 0 };
    static const QUMethod slot_8 = {"motifplus_style", 0, 0 };
    static const QUMethod slot_9 = {"windows_style", 0, 0 };
    static const QUMethod slot_10 = {"about", 0, 0 };
    static const QUMethod slot_11 = {"aboutQt", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "load()", &slot_0, QMetaData::Private },
	{ "save()", &slot_1, QMetaData::Private },
	{ "quit()", &slot_2, QMetaData::Private },
	{ "scan()", &slot_3, QMetaData::Private },
	{ "browser_search()", &slot_4, QMetaData::Private },
	{ "historic_back()", &slot_5, QMetaData::Private },
	{ "historic_forward()", &slot_6, QMetaData::Private },
	{ "motif_style()", &slot_7, QMetaData::Private },
	{ "motifplus_style()", &slot_8, QMetaData::Private },
	{ "windows_style()", &slot_9, QMetaData::Private },
	{ "about()", &slot_10, QMetaData::Private },
	{ "aboutQt()", &slot_11, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"JavaCatWindow", parentObject,
	slot_tbl, 12,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_JavaCatWindow.setMetaObject( metaObj );
    return metaObj;
}

void* JavaCatWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "JavaCatWindow" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool JavaCatWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: load(); break;
    case 1: save(); break;
    case 2: quit(); break;
    case 3: scan(); break;
    case 4: browser_search(); break;
    case 5: historic_back(); break;
    case 6: historic_forward(); break;
    case 7: motif_style(); break;
    case 8: motifplus_style(); break;
    case 9: windows_style(); break;
    case 10: about(); break;
    case 11: aboutQt(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool JavaCatWindow::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool JavaCatWindow::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool JavaCatWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
