// *************************************************************************
//
// Copyright (C) 2004-2007 Bruno PAGES  All rights reserved.
//
// This file is part of the BOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : bouml@free.fr
// home   : http://bouml.free.fr
//
// *************************************************************************

#ifndef JAVASETTINGS
#define JAVASETTINGS

/* !!!!!!!!!! Do not modify this file !!!!!!!!!! */

#ifdef WITHJAVA
#include <qdict.h>

#include "UmlSettings.h"
#include "api.h"

// This class manages settings concerning JAVA, configured through
// the 'Generation settings' dialog.
//
// This class may be defined as a 'singleton', but I prefer to use static 
// members allowing to just write 'JavaSettings::member' rather than
// 'JavaSettings::instance()->member' or other long sentence like this.

class JavaSettings : public UmlSettings {
  public:
    // returns TRUE when the created Java objects are initialized
    // with the default declaration/definition
    static bool useDefaults();

    // if y is TRUE the future created Java objects will be initialized
    // with the default declaration/definition
    static bool set_UseDefaults(bool y);

    // returns the JAVA type corresponding to the 'UML' type given in
    // argument, as it is configured in the first 'Generation settings'
    // dialog's tab
    static QCString type(const QCString & s);

    // set the JAVA type corresponding to the 'UML' type given in
    // argument, as it is configured in the first 'Generation settings'
    // dialog's tab
    static bool set_Type(QCString s, QCString v);

    // reverse of the Type() operation, returns the 'UML' type corresponding
    // to the JAVA type given in argument
    static QCString umlType(const QCString & s);

    // returns the JAVA stereotype corresponding to the 'UML' stereotype given
    // in argument
    static QCString relationStereotype(const QCString & s);

    // set the JAVA stereotype corresponding to the 'UML' stereotype given
    // in argument
    static bool set_RelationStereotype(QCString s, QCString v);

    // reverse of the RelationStereotype() operation, returns the 'UML' 
    // stereotype corresponding to the JAVA one given in argument
    static QCString relationUmlStereotype(const QCString & s);

    // returns the JAVA stereotype corresponding to the 'UML' stereotype given
    // in argument
    static QCString classStereotype(const QCString & s);

    // set the JAVA stereotype corresponding to the 'UML' stereotype given
    // in argument
    static bool set_ClassStereotype(QCString s, QCString v);

    // reverse of the ClassStereotype() operation, returns the 'UML' 
    // stereotype corresponding to the JAVA one given in argument
    static QCString classUmlStereotype(const QCString & s);

    //returns the import or other form specified in the last
    //'Generation settings' tab for the Java type given in argument.
    static QCString import(const QCString & s);

    //  set the import or other form specified in the last
    //  'Generation settings' tab for the Java type given in argument.
    static bool set_Import(QCString s, QCString v);

    //  return the 'root' directory
    static const QCString & rootDir();

    //  set the 'root' directory
    static bool set_RootDir(QCString v);

    // returns the default source file content
    static const QCString & sourceContent();

    // set the default source file content
    static bool set_SourceContent(QCString v);

    // returns the extension of the file produced by the JAVA code generator
    static const QCString & sourceExtension();

    // set the extension of the file produced by the JAVA code generator
    static bool set_SourceExtension(QCString v);

    // returns the default definition of a class
    static const QCString & classDecl();

    // set the default definition of a class
    static bool set_ClassDecl(QCString v);

    // returns the default specification for an 'external' class
    static const QCString & externalClassDecl();

    // set the default specification for an 'external' class
    static bool set_ExternalClassDecl(QCString v);

    // returns the default definition of an enumeration
    static const QCString & enumDecl();

    // returns the default definition of an enumeration implemented
    // through an abstract class having constant attributes
    static const QCString & enumPatternDecl();

    // set the default definition of an enumeration implemented
    // through an abstract class having constant attributes
    static bool set_EnumPatternDecl(QCString v);

    // returns the default definition of an interface
    static const QCString & interfaceDecl();

    // set the default definition of an interface
    static bool set_InterfaceDecl(QCString v);

    // returns the default definition of an attribute
    static const QCString & attributeDecl();

    // set the default definition of an attribute
    static bool set_AttributeDecl(QCString v);

    // returns the default definition of an enumeration item
    static const QCString & enumPatternItemDecl();

    // returns the default definition of an enumeration item
    static const QCString & enumItemDecl();

    // set the default definition of an enumeration item
    // following enum pattern
    static bool set_EnumPatternItemDecl(QCString v);

    //Returns the 'case' form produced in the fromInt operation
    //for each enumeration item

    static const QCString & enumPatternItemCase();

    //  set the 'case' form produced in the fromInt operation
    //  for each enumeration item

    static bool set_EnumPatternItemCase(QCString v);

    // returns the default definition of a relation depending on the
    // multiplicity given in argument.
    static const QCString & relationDecl(const char * multiplicity);

    // set the default definition of a relation depending on the
    // multiplicity given in argument.
    static bool set_RelationDecl(const char * multiplicity, QCString v);

    // returns the default definition of an operation
    static const QCString & operationDef();

    // set the default definition of an operation
    static bool set_OperationDef(QCString v);

    // returns the default visibility of a 'get' operation generated
    // through the attribute and relation 'add get operation'
    static aVisibility getVisibility();

    // set the default visibility of a 'get' operation generated
    // through the attribute and relation 'add get operation'
    static bool set_GetVisibility(aVisibility v);

    // returns the default name of a 'get' operation generated 
    // through the attribute and relation 'add get operation' menu
    static const QCString & getName();

    // set the default name of a 'get' operation generated 
    // through the attribute and relation 'add get operation' menu
    static bool set_GetName(QCString v);

    // returns the default name of a 'set' operation generated 
    // through the attribute and relation 'add set operation' menu
    static const QCString & setName();

    // set the default name of a 'set' operation generated 
    // through the attribute and relation 'add set operation' menu
    static bool set_SetName(QCString v);

    //  return if a 'get' operation generated through the attribute
    //  and relation 'add get operation' menu is final by default
    static bool isGetFinal();

    //  set if a 'get' operation generated through the attribute
    //  and relation 'add get operation' menu is final by default
    static bool set_IsGetFinal(bool v);

    // returns if a 'set' operation generated through the attribute
    // and relation 'add set operation' menu is final by default
    static bool isSetFinal();

    // set if a 'set' operation generated through the attribute
    // and relation 'add set operation' menu is final by default
    static bool set_IsSetFinal(bool v);

    // returns the default visibility of a 'set' operation generated
    // through the attribute and relation 'add set operation'
    static aVisibility setVisibility();

    // set the default visibility of a 'set' operation generated
    // through the attribute and relation 'add set operation'
    static bool set_SetVisibility(aVisibility v);

    // returns if the parameters of a 'set' operation generated through the
    // attribute and relation 'add set operation' menu are final by default
    static bool isSetParamFinal();

    // set if the parameters of a 'set' operation generated through the
    // attribute and relation 'add set operation' menu are final by default
    static bool set_IsSetParamFinal(bool v);

  private:
    // never called !
    JavaSettings() {};

    static bool _defined;
    static QCString _root;
    static QCString _class_decl;
    static QCString _external_class_decl;
    static QCString _enum_decl;
    static QCString _enum_pattern_decl;
    static QCString _interface_decl;
    static QCString _attr_decl;
    static QCString _enum_item_decl;
    static QCString _enum_pattern_item_decl;
    static QCString _enum_pattern_item_case;
    static QCString _rel_decl[3/*multiplicity*/];
    static QCString _oper_def;
    static aVisibility _get_visibility;
    static QCString _get_name;
    static bool _is_get_final;
    static aVisibility _set_visibility;
    static QCString _set_name;
    static bool _is_set_final;
    static bool _is_set_param_final;
    static QCString _src_content;
    static QCString _ext;
    static QDict<QCString> _map_imports;

  protected:
    static void read_();
    static void read_if_needed_();
};
#endif

#endif
