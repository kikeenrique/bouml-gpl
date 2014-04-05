/****************************************************************************
** CommentView meta object code from reading C++ file 'CommentView.h'
**
** Created: Sat Apr 5 23:46:38 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "CommentView.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CommentView::className() const
{
    return "CommentView";
}

QMetaObject *CommentView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CommentView( "CommentView", &CommentView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CommentView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CommentView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CommentView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CommentView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CommentView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTextBrowser::staticMetaObject();
    static const QUParameter param_signal_0[] = {
	{ "name", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"refer", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "refer(const QString&)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"CommentView", parentObject,
	0, 0,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CommentView.setMetaObject( metaObj );
    return metaObj;
}

void* CommentView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CommentView" ) )
	return this;
    return QTextBrowser::qt_cast( clname );
}

// SIGNAL refer
void CommentView::refer( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

bool CommentView::qt_invoke( int _id, QUObject* _o )
{
    return QTextBrowser::qt_invoke(_id,_o);
}

bool CommentView::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: refer((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QTextBrowser::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool CommentView::qt_property( int id, int f, QVariant* v)
{
    return QTextBrowser::qt_property( id, f, v);
}

bool CommentView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
