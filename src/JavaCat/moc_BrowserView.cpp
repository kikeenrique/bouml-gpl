/****************************************************************************
** BrowserView meta object code from reading C++ file 'BrowserView.h'
**
** Created: Sat Apr 5 23:46:37 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "BrowserView.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *BrowserView::className() const
{
    return "BrowserView";
}

QMetaObject *BrowserView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_BrowserView( "BrowserView", &BrowserView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString BrowserView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BrowserView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString BrowserView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BrowserView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* BrowserView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QListView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_0 = {"selected", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_1 = {"rightPressed", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "href", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"refer", 1, param_slot_2 };
    static const QMetaData slot_tbl[] = {
	{ "selected(QListViewItem*)", &slot_0, QMetaData::Protected },
	{ "rightPressed(QListViewItem*)", &slot_1, QMetaData::Protected },
	{ "refer(const QString&)", &slot_2, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"BrowserView", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_BrowserView.setMetaObject( metaObj );
    return metaObj;
}

void* BrowserView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "BrowserView" ) )
	return this;
    return QListView::qt_cast( clname );
}

bool BrowserView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: selected((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 1: rightPressed((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 2: refer((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool BrowserView::qt_emit( int _id, QUObject* _o )
{
    return QListView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool BrowserView::qt_property( int id, int f, QVariant* v)
{
    return QListView::qt_property( id, f, v);
}

bool BrowserView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
