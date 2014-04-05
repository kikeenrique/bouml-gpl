/****************************************************************************
** ConstraintDialog meta object code from reading C++ file 'ConstraintDialog.h'
**
** Created: Sat Apr 5 23:42:04 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ConstraintDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ConstraintDialog::className() const
{
    return "ConstraintDialog";
}

QMetaObject *ConstraintDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ConstraintDialog( "ConstraintDialog", &ConstraintDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ConstraintDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ConstraintDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ConstraintDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ConstraintDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ConstraintDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"show_all", 0, 0 };
    static const QUMethod slot_1 = {"hide_all", 0, 0 };
    static const QUMethod slot_2 = {"hide_inherited", 0, 0 };
    static const QUMethod slot_3 = {"accept", 0, 0 };
    static const QUMethod slot_4 = {"polish", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "show_all()", &slot_0, QMetaData::Protected },
	{ "hide_all()", &slot_1, QMetaData::Protected },
	{ "hide_inherited()", &slot_2, QMetaData::Protected },
	{ "accept()", &slot_3, QMetaData::Protected },
	{ "polish()", &slot_4, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ConstraintDialog", parentObject,
	slot_tbl, 5,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ConstraintDialog.setMetaObject( metaObj );
    return metaObj;
}

void* ConstraintDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ConstraintDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool ConstraintDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: show_all(); break;
    case 1: hide_all(); break;
    case 2: hide_inherited(); break;
    case 3: accept(); break;
    case 4: polish(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ConstraintDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ConstraintDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool ConstraintDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *ConstraintTable::className() const
{
    return "ConstraintTable";
}

QMetaObject *ConstraintTable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ConstraintTable( "ConstraintTable", &ConstraintTable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ConstraintTable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ConstraintTable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ConstraintTable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ConstraintTable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ConstraintTable::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTable::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In },
	{ "button", &static_QUType_int, 0, QUParameter::In },
	{ "mousePos", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_0 = {"button_pressed", 4, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "button_pressed(int,int,int,const QPoint&)", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ConstraintTable", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ConstraintTable.setMetaObject( metaObj );
    return metaObj;
}

void* ConstraintTable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ConstraintTable" ) )
	return this;
    return QTable::qt_cast( clname );
}

bool ConstraintTable::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: button_pressed((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+4))); break;
    default:
	return QTable::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ConstraintTable::qt_emit( int _id, QUObject* _o )
{
    return QTable::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ConstraintTable::qt_property( int id, int f, QVariant* v)
{
    return QTable::qt_property( id, f, v);
}

bool ConstraintTable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
