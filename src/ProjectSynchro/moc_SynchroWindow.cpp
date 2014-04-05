/****************************************************************************
** SynchroWindow meta object code from reading C++ file 'SynchroWindow.h'
**
** Created: Sat Apr 5 23:49:56 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "SynchroWindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *SynchroWindow::className() const
{
    return "SynchroWindow";
}

QMetaObject *SynchroWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SynchroWindow( "SynchroWindow", &SynchroWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SynchroWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SynchroWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SynchroWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SynchroWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SynchroWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"load", 0, 0 };
    static const QUMethod slot_1 = {"quit", 0, 0 };
    static const QUMethod slot_2 = {"synchronize", 0, 0 };
    static const QUMethod slot_3 = {"motif_style", 0, 0 };
    static const QUMethod slot_4 = {"motifplus_style", 0, 0 };
    static const QUMethod slot_5 = {"windows_style", 0, 0 };
    static const QUMethod slot_6 = {"about", 0, 0 };
    static const QUMethod slot_7 = {"aboutQt", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "load()", &slot_0, QMetaData::Private },
	{ "quit()", &slot_1, QMetaData::Private },
	{ "synchronize()", &slot_2, QMetaData::Private },
	{ "motif_style()", &slot_3, QMetaData::Private },
	{ "motifplus_style()", &slot_4, QMetaData::Private },
	{ "windows_style()", &slot_5, QMetaData::Private },
	{ "about()", &slot_6, QMetaData::Private },
	{ "aboutQt()", &slot_7, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"SynchroWindow", parentObject,
	slot_tbl, 8,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SynchroWindow.setMetaObject( metaObj );
    return metaObj;
}

void* SynchroWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SynchroWindow" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool SynchroWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: load(); break;
    case 1: quit(); break;
    case 2: synchronize(); break;
    case 3: motif_style(); break;
    case 4: motifplus_style(); break;
    case 5: windows_style(); break;
    case 6: about(); break;
    case 7: aboutQt(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool SynchroWindow::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool SynchroWindow::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool SynchroWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
