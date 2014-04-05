/****************************************************************************
** ActivityDiagramWindow meta object code from reading C++ file 'ActivityDiagramWindow.h'
**
** Created: Sat Apr 5 23:41:40 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ActivityDiagramWindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ActivityDiagramWindow::className() const
{
    return "ActivityDiagramWindow";
}

QMetaObject *ActivityDiagramWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ActivityDiagramWindow( "ActivityDiagramWindow", &ActivityDiagramWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ActivityDiagramWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ActivityDiagramWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ActivityDiagramWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ActivityDiagramWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ActivityDiagramWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = DiagramWindow::staticMetaObject();
    static const QUMethod slot_0 = {"hit_activity", 0, 0 };
    static const QUMethod slot_1 = {"hit_interruptibleactivityregion", 0, 0 };
    static const QUMethod slot_2 = {"hit_expansionregion", 0, 0 };
    static const QUMethod slot_3 = {"hit_activitypartition", 0, 0 };
    static const QUMethod slot_4 = {"hit_initial", 0, 0 };
    static const QUMethod slot_5 = {"hit_activityfinal", 0, 0 };
    static const QUMethod slot_6 = {"hit_flowfinal", 0, 0 };
    static const QUMethod slot_7 = {"hit_decision", 0, 0 };
    static const QUMethod slot_8 = {"hit_merge", 0, 0 };
    static const QUMethod slot_9 = {"hit_fork", 0, 0 };
    static const QUMethod slot_10 = {"hit_join", 0, 0 };
    static const QUMethod slot_11 = {"hit_action", 0, 0 };
    static const QUMethod slot_12 = {"hit_object", 0, 0 };
    static const QUMethod slot_13 = {"hit_flow", 0, 0 };
    static const QUMethod slot_14 = {"hit_dependency", 0, 0 };
    static const QUMethod slot_15 = {"hit_package", 0, 0 };
    static const QUMethod slot_16 = {"hit_fragment", 0, 0 };
    static const QUMethod slot_17 = {"hit_note", 0, 0 };
    static const QUMethod slot_18 = {"hit_anchor", 0, 0 };
    static const QUMethod slot_19 = {"hit_text", 0, 0 };
    static const QUMethod slot_20 = {"hit_image", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "hit_activity()", &slot_0, QMetaData::Protected },
	{ "hit_interruptibleactivityregion()", &slot_1, QMetaData::Protected },
	{ "hit_expansionregion()", &slot_2, QMetaData::Protected },
	{ "hit_activitypartition()", &slot_3, QMetaData::Protected },
	{ "hit_initial()", &slot_4, QMetaData::Protected },
	{ "hit_activityfinal()", &slot_5, QMetaData::Protected },
	{ "hit_flowfinal()", &slot_6, QMetaData::Protected },
	{ "hit_decision()", &slot_7, QMetaData::Protected },
	{ "hit_merge()", &slot_8, QMetaData::Protected },
	{ "hit_fork()", &slot_9, QMetaData::Protected },
	{ "hit_join()", &slot_10, QMetaData::Protected },
	{ "hit_action()", &slot_11, QMetaData::Protected },
	{ "hit_object()", &slot_12, QMetaData::Protected },
	{ "hit_flow()", &slot_13, QMetaData::Protected },
	{ "hit_dependency()", &slot_14, QMetaData::Protected },
	{ "hit_package()", &slot_15, QMetaData::Protected },
	{ "hit_fragment()", &slot_16, QMetaData::Protected },
	{ "hit_note()", &slot_17, QMetaData::Protected },
	{ "hit_anchor()", &slot_18, QMetaData::Protected },
	{ "hit_text()", &slot_19, QMetaData::Protected },
	{ "hit_image()", &slot_20, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ActivityDiagramWindow", parentObject,
	slot_tbl, 21,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ActivityDiagramWindow.setMetaObject( metaObj );
    return metaObj;
}

void* ActivityDiagramWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ActivityDiagramWindow" ) )
	return this;
    return DiagramWindow::qt_cast( clname );
}

bool ActivityDiagramWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: hit_activity(); break;
    case 1: hit_interruptibleactivityregion(); break;
    case 2: hit_expansionregion(); break;
    case 3: hit_activitypartition(); break;
    case 4: hit_initial(); break;
    case 5: hit_activityfinal(); break;
    case 6: hit_flowfinal(); break;
    case 7: hit_decision(); break;
    case 8: hit_merge(); break;
    case 9: hit_fork(); break;
    case 10: hit_join(); break;
    case 11: hit_action(); break;
    case 12: hit_object(); break;
    case 13: hit_flow(); break;
    case 14: hit_dependency(); break;
    case 15: hit_package(); break;
    case 16: hit_fragment(); break;
    case 17: hit_note(); break;
    case 18: hit_anchor(); break;
    case 19: hit_text(); break;
    case 20: hit_image(); break;
    default:
	return DiagramWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ActivityDiagramWindow::qt_emit( int _id, QUObject* _o )
{
    return DiagramWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ActivityDiagramWindow::qt_property( int id, int f, QVariant* v)
{
    return DiagramWindow::qt_property( id, f, v);
}

bool ActivityDiagramWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
