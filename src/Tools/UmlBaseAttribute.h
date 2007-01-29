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

#ifndef UMLBASEATTRIBUTE_H
#define UMLBASEATTRIBUTE_H

/* !!!!!!!!!! Do not modify this file !!!!!!!!!! */

#include "UmlClassMember.h"
#include "UmlTypeSpec.h"

class UmlOperation;
class UmlAttribute;
class UmlClass;

class UmlBaseAttribute : public UmlClassMember {
  private:
    bool _read_only;
#ifdef WITHCPP
    bool _cpp_mutable;
#endif
#ifdef WITHJAVA
    bool _java_transient;
#endif
    QCString _default_value;
    UmlTypeSpec _type;
    UmlOperation * _get_oper;
    UmlOperation * _set_oper;
#ifdef WITHIDL
    UmlAttribute * _idl_case;	// exclusive with idl_explicit_case
    QCString _idl_explicit_case;
#endif
    
    virtual void read_uml_();
#ifdef WITHCPP
    virtual void read_cpp_();
#endif
#ifdef WITHJAVA
    virtual void read_java_();
#endif
#ifdef WITHIDL
    virtual void read_idl_();
#endif
    
  public:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseAttribute(void * id, const QCString & n);
  
    // returns a new attribute named 'name' created under 'parent', or
    // the null pointer in case it cannot be created (the name is
    // already used or invalid, 'parent' cannot contain it etc ...)
    static UmlAttribute * create(UmlClass * parent, const char * name);
    
    // returns the kind of the item
    virtual anItemKind kind();
  
    // indicates if the attribute is read only, returns TRUE if yes
    bool isReadOnly();
    
    // to set the 'read only' state of the attribute, returns FALSE on error
    bool set_isReadOnly(bool);
    
    // returns the default attribute value, may be an empty string
    const QCString & defaultValue();
    
    // to set the default attribute value ("" allowed), returns FALSE on error
    bool set_DefaultValue(const char *);
    
    // returns the attribute UML type
    const UmlTypeSpec & type();
    
    // to set the attribute UML type, returns FALSE on error
    bool set_Type(const UmlTypeSpec &);
    
    // returns the 'get' operation of the attribute, or 0 if it does not exist
    UmlOperation * getOperation();
    
    // returns the 'set' operation of the attribute, or 0 if it does not exist
    UmlOperation * setOperation();
    
    // to generate an associated 'get' operation, returns FALSE on error
    bool addGetOperation();
    
    // to generate an associated 'set' operation, returns FALSE on error
    bool addSetOperation();
    
#ifdef WITHCPP
    // Indicate if the attribute is 'mutable'
    bool isCppMutable();
    
    // Set if the attribute is 'mutable'
    bool set_isCppMutable(bool y);
#endif
    
#ifdef WITHJAVA
    // indicates if the attribute is 'transient', returns TRUE if yes
    bool isJavaTransient();
    
    // to set the 'transient' state of the attribute, returns FALSE on error
    bool set_isJavaTransient(bool);
#endif    

#ifdef WITHIDL
    // in case the attribute is an IDL union's member returns the
    // corresponding 'case', an empty string in case it is not specified
    QCString idlCase();
    
    // to set the 'case' even the attribute is not (already) known as
    // an IDL union's member, returns FALSE on error
    bool set_IdlCase(UmlAttribute *);
    
    // to set the 'case' even the attribute is not (already) known as
    // an IDL union's member, returns FALSE on error
    bool set_IdlCase(const char *);
#endif
    
    // to unload the object to free memory, it will be reloaded
    // automatically if needed. args unused
    virtual void unload(bool = FALSE, bool = FALSE);
};

#endif
