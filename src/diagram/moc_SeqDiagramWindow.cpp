/****************************************************************************
** SeqDiagramWindow meta object code from reading C++ file 'SeqDiagramWindow.h'
**
** Created: Sat Apr 5 23:41:28 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "SeqDiagramWindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *SeqDiagramWindow::className() const
{
    return "SeqDiagramWindow";
}

QMetaObject *SeqDiagramWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SeqDiagramWindow( "SeqDiagramWindow", &SeqDiagramWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SeqDiagramWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SeqDiagramWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SeqDiagramWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SeqDiagramWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SeqDiagramWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = DiagramWindow::staticMetaObject();
    static const QUMethod slot_0 = {"hit_class", 0, 0 };
    static const QUMethod slot_1 = {"hit_classinstance", 0, 0 };
    static const QUMethod slot_2 = {"hit_continuation", 0, 0 };
    static const QUMethod slot_3 = {"hit_fragment", 0, 0 };
    static const QUMethod slot_4 = {"hit_syncmsg", 0, 0 };
    static const QUMethod slot_5 = {"hit_asyncmsg", 0, 0 };
    static const QUMethod slot_6 = {"hit_foundsyncmsg", 0, 0 };
    static const QUMethod slot_7 = {"hit_foundasyncmsg", 0, 0 };
    static const QUMethod slot_8 = {"hit_lostsyncmsg", 0, 0 };
    static const QUMethod slot_9 = {"hit_lostasyncmsg", 0, 0 };
    static const QUMethod slot_10 = {"hit_syncselfmsg", 0, 0 };
    static const QUMethod slot_11 = {"hit_asyncselfmsg", 0, 0 };
    static const QUMethod slot_12 = {"hit_returnmsg", 0, 0 };
    static const QUMethod slot_13 = {"hit_selfreturnmsg", 0, 0 };
    static const QUMethod slot_14 = {"hit_note", 0, 0 };
    static const QUMethod slot_15 = {"hit_anchor", 0, 0 };
    static const QUMethod slot_16 = {"hit_text", 0, 0 };
    static const QUMethod slot_17 = {"hit_image", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "hit_class()", &slot_0, QMetaData::Protected },
	{ "hit_classinstance()", &slot_1, QMetaData::Protected },
	{ "hit_continuation()", &slot_2, QMetaData::Protected },
	{ "hit_fragment()", &slot_3, QMetaData::Protected },
	{ "hit_syncmsg()", &slot_4, QMetaData::Protected },
	{ "hit_asyncmsg()", &slot_5, QMetaData::Protected },
	{ "hit_foundsyncmsg()", &slot_6, QMetaData::Protected },
	{ "hit_foundasyncmsg()", &slot_7, QMetaData::Protected },
	{ "hit_lostsyncmsg()", &slot_8, QMetaData::Protected },
	{ "hit_lostasyncmsg()", &slot_9, QMetaData::Protected },
	{ "hit_syncselfmsg()", &slot_10, QMetaData::Protected },
	{ "hit_asyncselfmsg()", &slot_11, QMetaData::Protected },
	{ "hit_returnmsg()", &slot_12, QMetaData::Protected },
	{ "hit_selfreturnmsg()", &slot_13, QMetaData::Protected },
	{ "hit_note()", &slot_14, QMetaData::Protected },
	{ "hit_anchor()", &slot_15, QMetaData::Protected },
	{ "hit_text()", &slot_16, QMetaData::Protected },
	{ "hit_image()", &slot_17, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"SeqDiagramWindow", parentObject,
	slot_tbl, 18,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SeqDiagramWindow.setMetaObject( metaObj );
    return metaObj;
}

void* SeqDiagramWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SeqDiagramWindow" ) )
	return this;
    return DiagramWindow::qt_cast( clname );
}

bool SeqDiagramWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: hit_class(); break;
    case 1: hit_classinstance(); break;
    case 2: hit_continuation(); break;
    case 3: hit_fragment(); break;
    case 4: hit_syncmsg(); break;
    case 5: hit_asyncmsg(); break;
    case 6: hit_foundsyncmsg(); break;
    case 7: hit_foundasyncmsg(); break;
    case 8: hit_lostsyncmsg(); break;
    case 9: hit_lostasyncmsg(); break;
    case 10: hit_syncselfmsg(); break;
    case 11: hit_asyncselfmsg(); break;
    case 12: hit_returnmsg(); break;
    case 13: hit_selfreturnmsg(); break;
    case 14: hit_note(); break;
    case 15: hit_anchor(); break;
    case 16: hit_text(); break;
    case 17: hit_image(); break;
    default:
	return DiagramWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool SeqDiagramWindow::qt_emit( int _id, QUObject* _o )
{
    return DiagramWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool SeqDiagramWindow::qt_property( int id, int f, QVariant* v)
{
    return DiagramWindow::qt_property( id, f, v);
}

bool SeqDiagramWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
