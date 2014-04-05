/****************************************************************************
** UmlWindow meta object code from reading C++ file 'UmlWindow.h'
**
** Created: Sat Apr 5 23:41:30 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "UmlWindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *UmlWindow::className() const
{
    return "UmlWindow";
}

QMetaObject *UmlWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_UmlWindow( "UmlWindow", &UmlWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString UmlWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UmlWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString UmlWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UmlWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* UmlWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"newProject", 0, 0 };
    static const QUMethod slot_1 = {"newFromTemplate", 0, 0 };
    static const QUMethod slot_2 = {"load", 0, 0 };
    static const QUMethod slot_3 = {"save", 0, 0 };
    static const QUMethod slot_4 = {"saveAs", 0, 0 };
    static const QUMethod slot_5 = {"print", 0, 0 };
    static const QUMethod slot_6 = {"close", 0, 0 };
    static const QUMethod slot_7 = {"quit", 0, 0 };
    static const QUMethod slot_8 = {"browser_search", 0, 0 };
    static const QUMethod slot_9 = {"next_select", 0, 0 };
    static const QUMethod slot_10 = {"prev_select", 0, 0 };
    static const QUMethod slot_11 = {"edit_gen_settings", 0, 0 };
    static const QUMethod slot_12 = {"edit_stereotypes", 0, 0 };
    static const QUMethod slot_13 = {"edit_class_settings", 0, 0 };
    static const QUMethod slot_14 = {"edit_drawing_settings", 0, 0 };
    static const QUMethod slot_15 = {"use_cpp", 0, 0 };
    static const QUMethod slot_16 = {"use_java", 0, 0 };
    static const QUMethod slot_17 = {"use_php", 0, 0 };
    static const QUMethod slot_18 = {"use_python", 0, 0 };
    static const QUMethod slot_19 = {"use_idl", 0, 0 };
    static const QUMethod slot_20 = {"verbose", 0, 0 };
    static const QUMethod slot_21 = {"preserve", 0, 0 };
    static const QUMethod slot_22 = {"addoperationprofile", 0, 0 };
    static const QUMethod slot_23 = {"edit_shortcuts", 0, 0 };
    static const QUMethod slot_24 = {"edit_env", 0, 0 };
    static const QUMethod slot_25 = {"edit_image_root_dir", 0, 0 };
    static const QUMethod slot_26 = {"motif_style", 0, 0 };
    static const QUMethod slot_27 = {"motifplus_style", 0, 0 };
    static const QUMethod slot_28 = {"windows_style", 0, 0 };
    static const QUMethod slot_29 = {"show_stereotypes", 0, 0 };
    static const QUMethod slot_30 = {"do_completion", 0, 0 };
    static const QUMethod slot_31 = {"about", 0, 0 };
    static const QUMethod slot_32 = {"aboutQt", 0, 0 };
    static const QUMethod slot_33 = {"help", 0, 0 };
    static const QUMethod slot_34 = {"show_trace", 0, 0 };
    static const QUMethod slot_35 = {"cpp_generate", 0, 0 };
    static const QUMethod slot_36 = {"java_generate", 0, 0 };
    static const QUMethod slot_37 = {"php_generate", 0, 0 };
    static const QUMethod slot_38 = {"python_generate", 0, 0 };
    static const QUMethod slot_39 = {"idl_generate", 0, 0 };
    static const QUMethod slot_40 = {"java_catalog", 0, 0 };
    static const QUMethod slot_41 = {"cpp_reverse", 0, 0 };
    static const QUMethod slot_42 = {"cpp_roundtrip", 0, 0 };
    static const QUMethod slot_43 = {"java_reverse", 0, 0 };
    static const QUMethod slot_44 = {"java_roundtrip", 0, 0 };
    static const QUMethod slot_45 = {"php_reverse", 0, 0 };
    static const QUMethod slot_46 = {"python_reverse", 0, 0 };
    static const QUMethod slot_47 = {"cpp_roundtripbody", 0, 0 };
    static const QUMethod slot_48 = {"java_roundtripbody", 0, 0 };
    static const QUMethod slot_49 = {"php_roundtripbody", 0, 0 };
    static const QUMethod slot_50 = {"python_roundtripbody", 0, 0 };
    static const QUParameter param_slot_51[] = {
	{ "param", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_51 = {"run_tool", 1, param_slot_51 };
    static const QUMethod slot_52 = {"tool_settings", 0, 0 };
    static const QUMethod slot_53 = {"import_tool_settings", 0, 0 };
    static const QUMethod slot_54 = {"plug_out_upgrade", 0, 0 };
    static const QUMethod slot_55 = {"comment_changed", 0, 0 };
    static const QUMethod slot_56 = {"preferred_geometry", 0, 0 };
    static const QUMethod slot_57 = {"close_all_windows", 0, 0 };
    static const QUMethod slot_58 = {"toolMenuAboutToShow", 0, 0 };
    static const QUMethod slot_59 = {"projectMenuAboutToShow", 0, 0 };
    static const QUParameter param_slot_60[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_60 = {"historicActivated", 1, param_slot_60 };
    static const QUMethod slot_61 = {"langMenuAboutToShow", 0, 0 };
    static const QUMethod slot_62 = {"miscMenuAboutToShow", 0, 0 };
    static const QUMethod slot_63 = {"fontSizeMenuAboutToShow", 0, 0 };
    static const QUParameter param_slot_64[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_64 = {"setFontSize", 1, param_slot_64 };
    static const QUMethod slot_65 = {"formatMenuAboutToShow", 0, 0 };
    static const QUParameter param_slot_66[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_66 = {"setFormat", 1, param_slot_66 };
    static const QUMethod slot_67 = {"windowsMenuAboutToShow", 0, 0 };
    static const QUParameter param_slot_68[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_68 = {"windowsMenuActivated", 1, param_slot_68 };
    static const QUParameter param_slot_69[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_69 = {"dialogsMenuActivated", 1, param_slot_69 };
    static const QUParameter param_slot_70[] = {
	{ "e", &static_QUType_ptr, "QKeyEvent", QUParameter::In }
    };
    static const QUMethod slot_70 = {"keyPressEvent", 1, param_slot_70 };
    static const QMetaData slot_tbl[] = {
	{ "newProject()", &slot_0, QMetaData::Private },
	{ "newFromTemplate()", &slot_1, QMetaData::Private },
	{ "load()", &slot_2, QMetaData::Private },
	{ "save()", &slot_3, QMetaData::Private },
	{ "saveAs()", &slot_4, QMetaData::Private },
	{ "print()", &slot_5, QMetaData::Private },
	{ "close()", &slot_6, QMetaData::Private },
	{ "quit()", &slot_7, QMetaData::Private },
	{ "browser_search()", &slot_8, QMetaData::Private },
	{ "next_select()", &slot_9, QMetaData::Private },
	{ "prev_select()", &slot_10, QMetaData::Private },
	{ "edit_gen_settings()", &slot_11, QMetaData::Private },
	{ "edit_stereotypes()", &slot_12, QMetaData::Private },
	{ "edit_class_settings()", &slot_13, QMetaData::Private },
	{ "edit_drawing_settings()", &slot_14, QMetaData::Private },
	{ "use_cpp()", &slot_15, QMetaData::Private },
	{ "use_java()", &slot_16, QMetaData::Private },
	{ "use_php()", &slot_17, QMetaData::Private },
	{ "use_python()", &slot_18, QMetaData::Private },
	{ "use_idl()", &slot_19, QMetaData::Private },
	{ "verbose()", &slot_20, QMetaData::Private },
	{ "preserve()", &slot_21, QMetaData::Private },
	{ "addoperationprofile()", &slot_22, QMetaData::Private },
	{ "edit_shortcuts()", &slot_23, QMetaData::Private },
	{ "edit_env()", &slot_24, QMetaData::Private },
	{ "edit_image_root_dir()", &slot_25, QMetaData::Private },
	{ "motif_style()", &slot_26, QMetaData::Private },
	{ "motifplus_style()", &slot_27, QMetaData::Private },
	{ "windows_style()", &slot_28, QMetaData::Private },
	{ "show_stereotypes()", &slot_29, QMetaData::Private },
	{ "do_completion()", &slot_30, QMetaData::Private },
	{ "about()", &slot_31, QMetaData::Private },
	{ "aboutQt()", &slot_32, QMetaData::Private },
	{ "help()", &slot_33, QMetaData::Private },
	{ "show_trace()", &slot_34, QMetaData::Private },
	{ "cpp_generate()", &slot_35, QMetaData::Private },
	{ "java_generate()", &slot_36, QMetaData::Private },
	{ "php_generate()", &slot_37, QMetaData::Private },
	{ "python_generate()", &slot_38, QMetaData::Private },
	{ "idl_generate()", &slot_39, QMetaData::Private },
	{ "java_catalog()", &slot_40, QMetaData::Private },
	{ "cpp_reverse()", &slot_41, QMetaData::Private },
	{ "cpp_roundtrip()", &slot_42, QMetaData::Private },
	{ "java_reverse()", &slot_43, QMetaData::Private },
	{ "java_roundtrip()", &slot_44, QMetaData::Private },
	{ "php_reverse()", &slot_45, QMetaData::Private },
	{ "python_reverse()", &slot_46, QMetaData::Private },
	{ "cpp_roundtripbody()", &slot_47, QMetaData::Private },
	{ "java_roundtripbody()", &slot_48, QMetaData::Private },
	{ "php_roundtripbody()", &slot_49, QMetaData::Private },
	{ "python_roundtripbody()", &slot_50, QMetaData::Private },
	{ "run_tool(int)", &slot_51, QMetaData::Private },
	{ "tool_settings()", &slot_52, QMetaData::Private },
	{ "import_tool_settings()", &slot_53, QMetaData::Private },
	{ "plug_out_upgrade()", &slot_54, QMetaData::Private },
	{ "comment_changed()", &slot_55, QMetaData::Private },
	{ "preferred_geometry()", &slot_56, QMetaData::Private },
	{ "close_all_windows()", &slot_57, QMetaData::Private },
	{ "toolMenuAboutToShow()", &slot_58, QMetaData::Private },
	{ "projectMenuAboutToShow()", &slot_59, QMetaData::Private },
	{ "historicActivated(int)", &slot_60, QMetaData::Private },
	{ "langMenuAboutToShow()", &slot_61, QMetaData::Private },
	{ "miscMenuAboutToShow()", &slot_62, QMetaData::Private },
	{ "fontSizeMenuAboutToShow()", &slot_63, QMetaData::Private },
	{ "setFontSize(int)", &slot_64, QMetaData::Private },
	{ "formatMenuAboutToShow()", &slot_65, QMetaData::Private },
	{ "setFormat(int)", &slot_66, QMetaData::Private },
	{ "windowsMenuAboutToShow()", &slot_67, QMetaData::Private },
	{ "windowsMenuActivated(int)", &slot_68, QMetaData::Private },
	{ "dialogsMenuActivated(int)", &slot_69, QMetaData::Private },
	{ "keyPressEvent(QKeyEvent*)", &slot_70, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"UmlWindow", parentObject,
	slot_tbl, 71,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_UmlWindow.setMetaObject( metaObj );
    return metaObj;
}

void* UmlWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "UmlWindow" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool UmlWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: newProject(); break;
    case 1: newFromTemplate(); break;
    case 2: load(); break;
    case 3: save(); break;
    case 4: saveAs(); break;
    case 5: print(); break;
    case 6: close(); break;
    case 7: quit(); break;
    case 8: browser_search(); break;
    case 9: next_select(); break;
    case 10: prev_select(); break;
    case 11: edit_gen_settings(); break;
    case 12: edit_stereotypes(); break;
    case 13: edit_class_settings(); break;
    case 14: edit_drawing_settings(); break;
    case 15: use_cpp(); break;
    case 16: use_java(); break;
    case 17: use_php(); break;
    case 18: use_python(); break;
    case 19: use_idl(); break;
    case 20: verbose(); break;
    case 21: preserve(); break;
    case 22: addoperationprofile(); break;
    case 23: edit_shortcuts(); break;
    case 24: edit_env(); break;
    case 25: edit_image_root_dir(); break;
    case 26: motif_style(); break;
    case 27: motifplus_style(); break;
    case 28: windows_style(); break;
    case 29: show_stereotypes(); break;
    case 30: do_completion(); break;
    case 31: about(); break;
    case 32: aboutQt(); break;
    case 33: help(); break;
    case 34: show_trace(); break;
    case 35: cpp_generate(); break;
    case 36: java_generate(); break;
    case 37: php_generate(); break;
    case 38: python_generate(); break;
    case 39: idl_generate(); break;
    case 40: java_catalog(); break;
    case 41: cpp_reverse(); break;
    case 42: cpp_roundtrip(); break;
    case 43: java_reverse(); break;
    case 44: java_roundtrip(); break;
    case 45: php_reverse(); break;
    case 46: python_reverse(); break;
    case 47: cpp_roundtripbody(); break;
    case 48: java_roundtripbody(); break;
    case 49: php_roundtripbody(); break;
    case 50: python_roundtripbody(); break;
    case 51: run_tool((int)static_QUType_int.get(_o+1)); break;
    case 52: tool_settings(); break;
    case 53: import_tool_settings(); break;
    case 54: plug_out_upgrade(); break;
    case 55: comment_changed(); break;
    case 56: preferred_geometry(); break;
    case 57: close_all_windows(); break;
    case 58: toolMenuAboutToShow(); break;
    case 59: projectMenuAboutToShow(); break;
    case 60: historicActivated((int)static_QUType_int.get(_o+1)); break;
    case 61: langMenuAboutToShow(); break;
    case 62: miscMenuAboutToShow(); break;
    case 63: fontSizeMenuAboutToShow(); break;
    case 64: setFontSize((int)static_QUType_int.get(_o+1)); break;
    case 65: formatMenuAboutToShow(); break;
    case 66: setFormat((int)static_QUType_int.get(_o+1)); break;
    case 67: windowsMenuAboutToShow(); break;
    case 68: windowsMenuActivated((int)static_QUType_int.get(_o+1)); break;
    case 69: dialogsMenuActivated((int)static_QUType_int.get(_o+1)); break;
    case 70: keyPressEvent((QKeyEvent*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool UmlWindow::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool UmlWindow::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool UmlWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
