/****************************************************************************
** ReferenceDialog meta object code from reading C++ file 'ReferenceDialog.h'
**
** Created: Sat Apr 5 23:41:50 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ReferenceDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ReferenceDialog::className() const
{
    return "ReferenceDialog";
}

QMetaObject *ReferenceDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ReferenceDialog( "ReferenceDialog", &ReferenceDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ReferenceDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ReferenceDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ReferenceDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ReferenceDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ReferenceDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"compute", 0, 0 };
    static const QUMethod slot_2 = {"select", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"selected", 1, param_slot_3 };
    static const QUMethod slot_4 = {"mark_unmark", 0, 0 };
    static const QUMethod slot_5 = {"mark_them", 0, 0 };
    static const QUMethod slot_6 = {"unmark_all", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Protected },
	{ "compute()", &slot_1, QMetaData::Protected },
	{ "select()", &slot_2, QMetaData::Protected },
	{ "selected(int)", &slot_3, QMetaData::Protected },
	{ "mark_unmark()", &slot_4, QMetaData::Protected },
	{ "mark_them()", &slot_5, QMetaData::Protected },
	{ "unmark_all()", &slot_6, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ReferenceDialog", parentObject,
	slot_tbl, 7,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ReferenceDialog.setMetaObject( metaObj );
    return metaObj;
}

void* ReferenceDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ReferenceDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool ReferenceDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: compute(); break;
    case 2: select(); break;
    case 3: selected((int)static_QUType_int.get(_o+1)); break;
    case 4: mark_unmark(); break;
    case 5: mark_them(); break;
    case 6: unmark_all(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ReferenceDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ReferenceDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool ReferenceDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
