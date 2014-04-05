/****************************************************************************
** ClassDiagramWindow meta object code from reading C++ file 'ClassDiagramWindow.h'
**
** Created: Sat Apr 5 23:41:20 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ClassDiagramWindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ClassDiagramWindow::className() const
{
    return "ClassDiagramWindow";
}

QMetaObject *ClassDiagramWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ClassDiagramWindow( "ClassDiagramWindow", &ClassDiagramWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ClassDiagramWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ClassDiagramWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ClassDiagramWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ClassDiagramWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ClassDiagramWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = DiagramWindow::staticMetaObject();
    static const QUMethod slot_0 = {"hit_class", 0, 0 };
    static const QUMethod slot_1 = {"hit_package", 0, 0 };
    static const QUMethod slot_2 = {"hit_fragment", 0, 0 };
    static const QUMethod slot_3 = {"hit_inherit", 0, 0 };
    static const QUMethod slot_4 = {"hit_dependency", 0, 0 };
    static const QUMethod slot_5 = {"hit_association", 0, 0 };
    static const QUMethod slot_6 = {"hit_directionalassociation", 0, 0 };
    static const QUMethod slot_7 = {"hit_aggregation", 0, 0 };
    static const QUMethod slot_8 = {"hit_aggregationbyvalue", 0, 0 };
    static const QUMethod slot_9 = {"hit_directionalaggregation", 0, 0 };
    static const QUMethod slot_10 = {"hit_directionalaggregationbyvalue", 0, 0 };
    static const QUMethod slot_11 = {"hit_realize", 0, 0 };
    static const QUMethod slot_12 = {"hit_note", 0, 0 };
    static const QUMethod slot_13 = {"hit_anchor", 0, 0 };
    static const QUMethod slot_14 = {"hit_text", 0, 0 };
    static const QUMethod slot_15 = {"hit_image", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "hit_class()", &slot_0, QMetaData::Protected },
	{ "hit_package()", &slot_1, QMetaData::Protected },
	{ "hit_fragment()", &slot_2, QMetaData::Protected },
	{ "hit_inherit()", &slot_3, QMetaData::Protected },
	{ "hit_dependency()", &slot_4, QMetaData::Protected },
	{ "hit_association()", &slot_5, QMetaData::Protected },
	{ "hit_directionalassociation()", &slot_6, QMetaData::Protected },
	{ "hit_aggregation()", &slot_7, QMetaData::Protected },
	{ "hit_aggregationbyvalue()", &slot_8, QMetaData::Protected },
	{ "hit_directionalaggregation()", &slot_9, QMetaData::Protected },
	{ "hit_directionalaggregationbyvalue()", &slot_10, QMetaData::Protected },
	{ "hit_realize()", &slot_11, QMetaData::Protected },
	{ "hit_note()", &slot_12, QMetaData::Protected },
	{ "hit_anchor()", &slot_13, QMetaData::Protected },
	{ "hit_text()", &slot_14, QMetaData::Protected },
	{ "hit_image()", &slot_15, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ClassDiagramWindow", parentObject,
	slot_tbl, 16,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ClassDiagramWindow.setMetaObject( metaObj );
    return metaObj;
}

void* ClassDiagramWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ClassDiagramWindow" ) )
	return this;
    return DiagramWindow::qt_cast( clname );
}

bool ClassDiagramWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: hit_class(); break;
    case 1: hit_package(); break;
    case 2: hit_fragment(); break;
    case 3: hit_inherit(); break;
    case 4: hit_dependency(); break;
    case 5: hit_association(); break;
    case 6: hit_directionalassociation(); break;
    case 7: hit_aggregation(); break;
    case 8: hit_aggregationbyvalue(); break;
    case 9: hit_directionalaggregation(); break;
    case 10: hit_directionalaggregationbyvalue(); break;
    case 11: hit_realize(); break;
    case 12: hit_note(); break;
    case 13: hit_anchor(); break;
    case 14: hit_text(); break;
    case 15: hit_image(); break;
    default:
	return DiagramWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ClassDiagramWindow::qt_emit( int _id, QUObject* _o )
{
    return DiagramWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ClassDiagramWindow::qt_property( int id, int f, QVariant* v)
{
    return DiagramWindow::qt_property( id, f, v);
}

bool ClassDiagramWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
