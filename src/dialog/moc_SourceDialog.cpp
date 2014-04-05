/****************************************************************************
** NumberedMultiLineEdit meta object code from reading C++ file 'SourceDialog.h'
**
** Created: Sat Apr 5 23:41:50 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "SourceDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *NumberedMultiLineEdit::className() const
{
    return "NumberedMultiLineEdit";
}

QMetaObject *NumberedMultiLineEdit::metaObj = 0;
static QMetaObjectCleanUp cleanUp_NumberedMultiLineEdit( "NumberedMultiLineEdit", &NumberedMultiLineEdit::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString NumberedMultiLineEdit::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "NumberedMultiLineEdit", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString NumberedMultiLineEdit::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "NumberedMultiLineEdit", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* NumberedMultiLineEdit::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = MultiLineEdit::staticMetaObject();
    static const QUParameter param_signal_0[] = {
	{ "l", &static_QUType_int, 0, QUParameter::In },
	{ "c", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"cursorMoved", 2, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "cursorMoved(int,int)", &signal_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"NumberedMultiLineEdit", parentObject,
	0, 0,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_NumberedMultiLineEdit.setMetaObject( metaObj );
    return metaObj;
}

void* NumberedMultiLineEdit::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "NumberedMultiLineEdit" ) )
	return this;
    return MultiLineEdit::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL cursorMoved
void NumberedMultiLineEdit::cursorMoved( int t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

bool NumberedMultiLineEdit::qt_invoke( int _id, QUObject* _o )
{
    return MultiLineEdit::qt_invoke(_id,_o);
}

bool NumberedMultiLineEdit::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: cursorMoved((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return MultiLineEdit::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool NumberedMultiLineEdit::qt_property( int id, int f, QVariant* v)
{
    return MultiLineEdit::qt_property( id, f, v);
}

bool NumberedMultiLineEdit::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *SourceDialog::className() const
{
    return "SourceDialog";
}

QMetaObject *SourceDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SourceDialog( "SourceDialog", &SourceDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SourceDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SourceDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SourceDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SourceDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SourceDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"edit", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "l", &static_QUType_int, 0, QUParameter::In },
	{ "c", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"updateCursorPos", 2, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "edit()", &slot_0, QMetaData::Protected },
	{ "updateCursorPos(int,int)", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"SourceDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SourceDialog.setMetaObject( metaObj );
    return metaObj;
}

void* SourceDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SourceDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool SourceDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: edit(); break;
    case 1: updateCursorPos((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool SourceDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool SourceDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool SourceDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
