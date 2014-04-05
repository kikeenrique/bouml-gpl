/****************************************************************************
** StateDiagramView meta object code from reading C++ file 'StateDiagramView.h'
**
** Created: Sat Apr 5 23:41:38 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "StateDiagramView.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *StateDiagramView::className() const
{
    return "StateDiagramView";
}

QMetaObject *StateDiagramView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_StateDiagramView( "StateDiagramView", &StateDiagramView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString StateDiagramView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StateDiagramView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString StateDiagramView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StateDiagramView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* StateDiagramView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = DiagramView::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"StateDiagramView", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_StateDiagramView.setMetaObject( metaObj );
    return metaObj;
}

void* StateDiagramView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "StateDiagramView" ) )
	return this;
    return DiagramView::qt_cast( clname );
}

bool StateDiagramView::qt_invoke( int _id, QUObject* _o )
{
    return DiagramView::qt_invoke(_id,_o);
}

bool StateDiagramView::qt_emit( int _id, QUObject* _o )
{
    return DiagramView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool StateDiagramView::qt_property( int id, int f, QVariant* v)
{
    return DiagramView::qt_property( id, f, v);
}

bool StateDiagramView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
