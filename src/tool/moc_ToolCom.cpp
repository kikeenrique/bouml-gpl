/****************************************************************************
** ToolCom meta object code from reading C++ file 'ToolCom.h'
**
** Created: Sat Apr 5 23:42:08 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ToolCom.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ToolCom::className() const
{
    return "ToolCom";
}

QMetaObject *ToolCom::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ToolCom( "ToolCom", &ToolCom::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ToolCom::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ToolCom", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ToolCom::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ToolCom", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ToolCom::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"connexion_timeout", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "connexion_timeout()", &slot_0, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"closed", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "closed()", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ToolCom", parentObject,
	slot_tbl, 1,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ToolCom.setMetaObject( metaObj );
    return metaObj;
}

void* ToolCom::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ToolCom" ) )
	return this;
    return QObject::qt_cast( clname );
}

// SIGNAL closed
void ToolCom::closed()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool ToolCom::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: connexion_timeout(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ToolCom::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: closed(); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ToolCom::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool ToolCom::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
