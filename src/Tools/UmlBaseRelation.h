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

#ifndef UMLBASERELATION_H
#define UMLBASERELATION_H

/* !!!!!!!!!! Do not modify this file !!!!!!!!!! */

#include "UmlClassMember.h"
#include "UmlTypeSpec.h"

class UmlClass;
class UmlRelation;
class UmlOperation;
class UmlAttribute;

// system base class managing the relations

class UmlBaseRelation : public UmlClassMember {
  private:
    bool _read_only;
#ifdef WITHCPP
    bool _cpp_virtual_inheritance : 1;
    bool _cpp_mutable : 1;
#endif
#ifdef WITHJAVA
    bool _java_transient : 1;
#endif
#ifdef WITHIDL
    bool _idl_truncatable : 1;
#endif
    aRelationKind _rel_kind : 8;
    QCString _default_value;
    UmlClass * _role_type;
    QCString _role_name;
    QCString _multiplicity;
    UmlTypeSpec _association;
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
    UmlBaseRelation(void * id, const QCString & n);
  
    // returns a new relation of the given 'kind' from 'start' to 'end',
    // or the null pointer in case it cannot be created.
    static UmlRelation * create(aRelationKind kind, 
				UmlClass * start, UmlClass * end);
  
    // returns the kind of the item
    virtual anItemKind kind();
    
    // indicates if the relation is read only, returns TRUE if yes
    bool isReadOnly();
    
    // to set the 'read only' state of the relation, returns FALSE on error
    bool set_isReadOnly(bool y);
    
    // returns the default relation value, may be an empty string
    const QCString & defaultValue();
    
    // to set the default relation value ("" allowed), returns FALSE on error
    bool set_DefaultValue(const char *);
    
    // returns the kind of the relation (see above)
    aRelationKind relationKind();
    
    // if 'first' is true returns the relation associated to the
    // first role, else the relation associated to the second
    // role or 0/null if the relation is uni directional
    UmlRelation * side(bool first);
    
#if 0
    //bool set_rel_kind(const char *); relire relation des 2 cotes (1 si mono dir)
#endif
      
    // to set the stereotype, returns FALSE on error
    // redefined in case the relation is bidirectional to set the stereotype
    // of the relation corresponding to the other direction
    virtual bool set_Stereotype(const QCString &);
    
    // returns the 'end' class (the 'start' class is the parent of the relation)
    UmlClass * roleType();	// no set !
    
    // return the associated class/type, may be an empty spec
    UmlTypeSpec association();
    
    // set the associated class/type, may be an empty spec
    bool set_Association(const UmlTypeSpec &);
    
    // returns the name of the role
    const QCString & roleName();
    
    // to set the name of the role, returns FALSE on error
    bool set_RoleName(const char *);
    
    // returns the multiplicity (may be an empty string)
    const QCString & multiplicity();
    
    // to set the multiplicity, returns FALSE on error
    bool set_Multiplicity(const char *);
    
    // returns the 'get' operation of the relation, or 0 if it does not exist
    UmlOperation * getOperation();
    
    // returns the 'set' operation of the relation, or 0 if it does not exist
    UmlOperation * setOperation();
    
    // to generate an associated 'get' operation, returns FALSE on error
    bool addGetOperation();
    
    // to generate an associated 'set' operation, returns FALSE on error
    bool addSetOperation();
    
#ifdef WITHCPP
    // indicates if the inheritance is virtual in C++, returns TRUE if yes
    bool cppVirtualInheritance();
    
    // to set if the inheritance is virtual in C++
    bool set_CppVirtualInheritance(bool y);

    // Indicate if the relation is 'mutable'
    bool isCppMutable();
    
    // Set if the relation is 'mutable'
    bool set_isCppMutable(bool y);
#endif
    
#ifdef WITHJAVA
    // indicates if the relation is 'transient', returns TRUE if yes
    bool isJavaTransient();
    
    // to set the 'transient' state of the relation, returns FALSE on error
    bool set_isJavaTransient(bool);
#endif    

#ifdef WITHIDL
    // indicates if the relation is 'truncatable', returns TRUE if yes
    // have sense only for a valuetype inheritance
    bool isIdlTruncatableInheritance();
    
    // to set the 'truncatable' state of the relation, returns FALSE on error
    // have sense only for a valuetype inheritance
    bool set_isIdlTruncatableInheritance(bool);
    
    // in case the relation is an IDL union's member returns the
    // corresponding 'case', an empty string in case it is not specified
    QCString idlCase();
    
    // to set the 'case' even the relation is not (already) known as
    // an IDL union's member, returns FALSE on error
    bool set_IdlCase(UmlAttribute *);
    
    // to set the 'case' even the relation is not (already) known as
    // an IDL union's member, returns FALSE on error
    bool set_IdlCase(const char *);
#endif
    
    // to unload the object to free memory, it will be reloaded
    // automatically if needed. args unused
    virtual void unload(bool = FALSE, bool = FALSE);
};

#endif
