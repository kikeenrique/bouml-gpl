/****************************************************************************
** StateDiagramWindow meta object code from reading C++ file 'StateDiagramWindow.h'
**
** Created: Sat Apr 5 23:41:39 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "StateDiagramWindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *StateDiagramWindow::className() const
{
    return "StateDiagramWindow";
}

QMetaObject *StateDiagramWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_StateDiagramWindow( "StateDiagramWindow", &StateDiagramWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString StateDiagramWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StateDiagramWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString StateDiagramWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StateDiagramWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* StateDiagramWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = DiagramWindow::staticMetaObject();
    static const QUMethod slot_0 = {"hit_state", 0, 0 };
    static const QUMethod slot_1 = {"hit_package", 0, 0 };
    static const QUMethod slot_2 = {"hit_fragment", 0, 0 };
    static const QUMethod slot_3 = {"hit_entryPoint", 0, 0 };
    static const QUMethod slot_4 = {"hit_exitPoint", 0, 0 };
    static const QUMethod slot_5 = {"hit_initial", 0, 0 };
    static const QUMethod slot_6 = {"hit_final", 0, 0 };
    static const QUMethod slot_7 = {"hit_terminate", 0, 0 };
    static const QUMethod slot_8 = {"hit_deepHistory", 0, 0 };
    static const QUMethod slot_9 = {"hit_shallowHistory", 0, 0 };
    static const QUMethod slot_10 = {"hit_junction", 0, 0 };
    static const QUMethod slot_11 = {"hit_choice", 0, 0 };
    static const QUMethod slot_12 = {"hit_fork", 0, 0 };
    static const QUMethod slot_13 = {"hit_join", 0, 0 };
    static const QUMethod slot_14 = {"hit_region", 0, 0 };
    static const QUMethod slot_15 = {"hit_action", 0, 0 };
    static const QUMethod slot_16 = {"hit_signalin", 0, 0 };
    static const QUMethod slot_17 = {"hit_signalout", 0, 0 };
    static const QUMethod slot_18 = {"hit_transition", 0, 0 };
    static const QUMethod slot_19 = {"hit_note", 0, 0 };
    static const QUMethod slot_20 = {"hit_anchor", 0, 0 };
    static const QUMethod slot_21 = {"hit_text", 0, 0 };
    static const QUMethod slot_22 = {"hit_image", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "hit_state()", &slot_0, QMetaData::Protected },
	{ "hit_package()", &slot_1, QMetaData::Protected },
	{ "hit_fragment()", &slot_2, QMetaData::Protected },
	{ "hit_entryPoint()", &slot_3, QMetaData::Protected },
	{ "hit_exitPoint()", &slot_4, QMetaData::Protected },
	{ "hit_initial()", &slot_5, QMetaData::Protected },
	{ "hit_final()", &slot_6, QMetaData::Protected },
	{ "hit_terminate()", &slot_7, QMetaData::Protected },
	{ "hit_deepHistory()", &slot_8, QMetaData::Protected },
	{ "hit_shallowHistory()", &slot_9, QMetaData::Protected },
	{ "hit_junction()", &slot_10, QMetaData::Protected },
	{ "hit_choice()", &slot_11, QMetaData::Protected },
	{ "hit_fork()", &slot_12, QMetaData::Protected },
	{ "hit_join()", &slot_13, QMetaData::Protected },
	{ "hit_region()", &slot_14, QMetaData::Protected },
	{ "hit_action()", &slot_15, QMetaData::Protected },
	{ "hit_signalin()", &slot_16, QMetaData::Protected },
	{ "hit_signalout()", &slot_17, QMetaData::Protected },
	{ "hit_transition()", &slot_18, QMetaData::Protected },
	{ "hit_note()", &slot_19, QMetaData::Protected },
	{ "hit_anchor()", &slot_20, QMetaData::Protected },
	{ "hit_text()", &slot_21, QMetaData::Protected },
	{ "hit_image()", &slot_22, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"StateDiagramWindow", parentObject,
	slot_tbl, 23,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_StateDiagramWindow.setMetaObject( metaObj );
    return metaObj;
}

void* StateDiagramWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "StateDiagramWindow" ) )
	return this;
    return DiagramWindow::qt_cast( clname );
}

bool StateDiagramWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: hit_state(); break;
    case 1: hit_package(); break;
    case 2: hit_fragment(); break;
    case 3: hit_entryPoint(); break;
    case 4: hit_exitPoint(); break;
    case 5: hit_initial(); break;
    case 6: hit_final(); break;
    case 7: hit_terminate(); break;
    case 8: hit_deepHistory(); break;
    case 9: hit_shallowHistory(); break;
    case 10: hit_junction(); break;
    case 11: hit_choice(); break;
    case 12: hit_fork(); break;
    case 13: hit_join(); break;
    case 14: hit_region(); break;
    case 15: hit_action(); break;
    case 16: hit_signalin(); break;
    case 17: hit_signalout(); break;
    case 18: hit_transition(); break;
    case 19: hit_note(); break;
    case 20: hit_anchor(); break;
    case 21: hit_text(); break;
    case 22: hit_image(); break;
    default:
	return DiagramWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool StateDiagramWindow::qt_emit( int _id, QUObject* _o )
{
    return DiagramWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool StateDiagramWindow::qt_property( int id, int f, QVariant* v)
{
    return DiagramWindow::qt_property( id, f, v);
}

bool StateDiagramWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
