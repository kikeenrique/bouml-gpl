/****************************************************************************
** ActivityDiagramView meta object code from reading C++ file 'ActivityDiagramView.h'
**
** Created: Sat Apr 5 23:41:40 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ActivityDiagramView.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ActivityDiagramView::className() const
{
    return "ActivityDiagramView";
}

QMetaObject *ActivityDiagramView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ActivityDiagramView( "ActivityDiagramView", &ActivityDiagramView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ActivityDiagramView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ActivityDiagramView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ActivityDiagramView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ActivityDiagramView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ActivityDiagramView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = DiagramView::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"ActivityDiagramView", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ActivityDiagramView.setMetaObject( metaObj );
    return metaObj;
}

void* ActivityDiagramView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ActivityDiagramView" ) )
	return this;
    return DiagramView::qt_cast( clname );
}

bool ActivityDiagramView::qt_invoke( int _id, QUObject* _o )
{
    return DiagramView::qt_invoke(_id,_o);
}

bool ActivityDiagramView::qt_emit( int _id, QUObject* _o )
{
    return DiagramView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ActivityDiagramView::qt_property( int id, int f, QVariant* v)
{
    return DiagramView::qt_property( id, f, v);
}

bool ActivityDiagramView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
