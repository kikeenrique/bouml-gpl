#ifndef _PHPSETTINGS_H
#define _PHPSETTINGS_H

#ifdef WITHPHP


#include <qcstring.h>
#include "aVisibility.h"
#include "UmlSettings.h"
#include <qdict.h>

// This class manages settings concerning PHP, configured through
// the 'Generation settings' dialog.
//
// This class may be defined as a 'singleton', but I prefer to use static 
// members allowing to just write 'PhpSettings::member' rather than
// 'PhpSettings::instance()->member' or other long sentence like this.
class PhpSettings : public UmlSettings {
  public:
    // returns TRUE when the created Php objects are initialized
    // with the default declaration/definition
    static bool useDefaults();

    // if y is TRUE the future created Php objects will be initialized
    // with the default declaration/definition
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_UseDefaults(bool y);

    // returns the PHP stereotype corresponding to the 'UML' stereotype given
    // in argument
    static QCString classStereotype(const QCString & s);

    // set the PHP stereotype corresponding to the 'UML' stereotype given
    // in argument
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_ClassStereotype(QCString s, QCString v);

    // reverse of the ClassStereotype() operation, returns the 'UML' 
    // stereotype corresponding to the PHP one given in argument
    static QCString classUmlStereotype(const QCString & s);

    //  return the 'root' directory
    static const QCString & rootDir();

    //  set the 'root' directory
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_RootDir(QCString v);

    // returns the default source file content
    static const QCString & sourceContent();

    // set the default source file content
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_SourceContent(QCString v);

    // returns the extension of the file produced by the PHP code generator
    static const QCString & sourceExtension();

    // set the extension of the file produced by the PHP code generator
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_SourceExtension(QCString v);

    // returns the default definition of a class
    static const QCString & classDecl();

    // set the default definition of a class
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_ClassDecl(QCString v);

    // returns the default specification for an 'external' class
    static const QCString & externalClassDecl();

    // set the default specification for an 'external' class
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_ExternalClassDecl(QCString v);

    // returns the default definition of an enumeration
    static QCString enumDecl();

    // set the default definition of an enumeration
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_EnumDecl(QCString v);

    // returns the default definition of an interface
    static const QCString & interfaceDecl();

    // set the default definition of an interface
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_InterfaceDecl(QCString v);

    // returns the default definition of an attribute
    static const QCString & attributeDecl();

    // set the default definition of an attribute
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_AttributeDecl(QCString v);

    // returns the default definition of an enumeration item
    static QCString enumItemDecl();

    // set the default definition of an enumeration item
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_EnumItemDecl(QCString v);

    // returns the default definition of a relation
    static const QCString & relationDecl();

    // set the default definition of a relation
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_RelationDecl(QCString v);

    // returns the default definition of an operation
    static const QCString & operationDef();

    // set the default definition of an operation
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_OperationDef(QCString v);

    // returns the default visibility of a 'get' operation generated
    // through the attribute and relation 'add get operation'
    // note : visibility shared with Java
    static aVisibility getVisibility();

    // set the default visibility of a 'get' operation generated
    // through the attribute and relation 'add get operation'
    // note : visibility shared with Java
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_GetVisibility(aVisibility v);

    // returns the default name of a 'get' operation generated 
    // through the attribute and relation 'add get operation' menu
    static const QCString & getName();

    // set the default name of a 'get' operation generated 
    // through the attribute and relation 'add get operation' menu
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_GetName(QCString v);

    // returns the default name of a 'set' operation generated 
    // through the attribute and relation 'add set operation' menu
    static const QCString & setName();

    // set the default name of a 'set' operation generated 
    // through the attribute and relation 'add set operation' menu
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_SetName(QCString v);

    //  return if a 'get' operation generated through the attribute
    //  and relation 'add get operation' menu is final by default
    static bool isGetFinal();

    //  set if a 'get' operation generated through the attribute
    //  and relation 'add get operation' menu is final by default
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_IsGetFinal(bool v);

    // returns if a 'set' operation generated through the attribute
    // and relation 'add set operation' menu is final by default
    static bool isSetFinal();

    // set if a 'set' operation generated through the attribute
    // and relation 'add set operation' menu is final by default
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_IsSetFinal(bool v);

    // returns the default visibility of a 'set' operation generated
    // through the attribute and relation 'add set operation'
    // note : visibility shared with Java
    static aVisibility setVisibility();

    // set the default visibility of a 'set' operation generated
    // through the attribute and relation 'add set operation'
    // note : visibility shared with Java
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_SetVisibility(aVisibility v);

  private:
    // never called !
    PhpSettings() {};

    static bool _defined;

    static QCString _root;

    static QCString _class_decl;

    static QCString _external_class_decl;

    static QCString _enum_decl;

    static QCString _interface_decl;

    static QCString _attr_decl;

    static QCString _enum_item_decl;

    static QCString _rel_decl;

    static QCString _oper_def;

#ifndef WITHJAVA
    static aVisibility _get_visibility;
#endif

    static QCString _get_name;

    static bool _is_get_final;

#ifndef WITHJAVA
    static aVisibility _set_visibility;
#endif

    static QCString _set_name;

    static bool _is_set_final;

    static QCString _src_content;

    static QCString _ext;

  protected:
    static void read_();

    static void read_if_needed_();

};


#endif

#endif
