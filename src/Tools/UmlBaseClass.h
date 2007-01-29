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

#ifndef UMLBASECLASS_H
#define UMLBASECLASS_H

/* !!!!!!!!!! Do not modify this file !!!!!!!!!! */

#include <qdict.h>
#include <qvaluelist.h>

#include "UmlClassMember.h"
#include "UmlTypeSpec.h"

class UmlPackage;
class UmlClass;
class UmlFormalParameter;
class UmlActualParameter;
class UmlClassDiagram;
class UmlArtifact;

// This class manages 'formal parameters' of a template class.
// For instance 'T' in 
//	template class Cl<class T> ...

class UmlBaseFormalParameter {
  friend class UmlBaseClass;
    
  protected:
    QCString _name;
    QCString _type;	// "class" ...
    UmlTypeSpec _default_value;
    UmlTypeSpec _extends;
    
    void read_();
    
  public:
    UmlBaseFormalParameter(){};
  
    // returns the name of the formal
    const QCString & name() const { return _name; };
    
    // returns the type of a formal, probably "class", C++
    const QCString & type() const { return _type; };
    
    // returns the default actual value, C++
    const UmlTypeSpec & defaultValue() const { return _default_value; };
    
    // returns the extends value, Java
    const UmlTypeSpec & extend() const { return _extends; };
};

// This class manages 'actual parameters' of a template class.
// for instance T in
//	class Cl1 : public Cl2<T> ...

class UmlBaseActualParameter {
  friend class UmlBaseClass;
  
  protected:
    UmlClass * _super;
    unsigned _rank;
    UmlTypeSpec _value;
  
    void read_();
    
  public:
    UmlBaseActualParameter(){};
  
    // returns the class of the corresponding formal
    UmlClass * superClass() const { return _super; };
    
    //returns the actual rank (0...)
    unsigned rank() const { return _rank; };
    
    // returns the actual's value
    const UmlTypeSpec & value() const { return _value; };
};

class UmlBaseClass : public UmlClassMember {
  friend class UmlBaseRelation;		// allow it to call reread_if_needed
  friend class UmlBaseArtifact;		// allow it to set _assoc_artifact
  private:
    static QDict<UmlClass> classes;	// key includes package/class-container
  
    bool _abstract;
#ifdef WITHCPP
    bool _cpp_external : 1;
#endif
#ifdef WITHJAVA
    bool _java_external : 1;
    bool _java_public : 1;
    bool _java_final : 1;
#endif
#ifdef WITHIDL
    bool _idl_external : 1;
    bool _idl_local : 1;
    bool _idl_custom : 1;
#endif
    UmlTypeSpec _base_type;
    UmlClassDiagram * _assoc_diagram;

#ifdef WITHIDL
    UmlTypeSpec _switch_type;		// union
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
    
    void reread_if_needed_();

  protected:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseClass(void * id, const QCString & n);
  
  public:
    // returns a new class named 'name' created under 'parent', or
    // the null pointer in case it cannot be created (the name is
    // already used or invalid, 'parent' cannot contain it etc ...)
    static UmlClass * create(UmlItem * parent, const char * name);
    
    // returns the kind of the item
    virtual anItemKind kind();
    
    // returns TRUE in case the class is abstract
    bool isAbstract();
    
    // set if the class is abstract, returns FALSE on error,
    // for instance you cannot set it to FALSE in case the class have
    // abstract operations
    bool set_isAbstract(bool y);
    
    // returns the type on which the class (in fact a typedef) is based
    const UmlTypeSpec & baseType();
    
    // to set the type on which the class (in fact a typedef) is based,
    // returns FALSE on error, does not check that the calss is (already)
    // a typedef
    bool set_BaseType(const UmlTypeSpec &);
    
    // returns the formals
    QValueList<UmlFormalParameter> formals();
    
    // remove the formal of the given rank (0...), returns 0 on error
    bool removeFormal(unsigned rank);
    
    // adds a formal at the given rank (0...), returns 0 on error
    bool addFormal(unsigned rank, const UmlFormalParameter &);
    
    // replace the formal at the given rank (0...), returns 0 on error
    bool replaceFormal(unsigned rank, const UmlFormalParameter &);
    
    // returns the actuals
    QValueList<UmlActualParameter> actuals();
    
    // replace the actual value at the given rank (0...), returns 0 on error
    bool replaceActual(unsigned rank, const UmlTypeSpec &);
    
    // returns the optional associated diagram
    UmlClassDiagram * associatedDiagram();
    
    // sets the associated diagram, arg may be 0
    bool set_AssociatedDiagram(UmlClassDiagram *);
    
    // returns the optional associated component,
    // to set it refer to the UmlBaseArtifact's operations
    // addAssociatedClass(), removeAssociatedClass() and
    // setAssociatedClasses()
    UmlArtifact * associatedArtifact();
    
#ifdef WITHCPP
    // returns TRUE if the class is external, its definition
    // must contains how the name is made on the first line
    // (${name} by default), the other lines if they exist
    // must contains the #include and may be using forms
    bool isCppExternal();
    
    // set if the class is external, returns FALSE on error
    bool set_isCppExternal(bool y);
#endif
    
#ifdef WITHJAVA
    // returns TRUE if the class is external, its definition
    // must contain how the name is made on the first line
    // (${name} by default), the other lines are ignored
    bool isJavaExternal();
    
    // set if the class is external, returns FALSE on error
    bool set_isJavaExternal(bool y);
    
    // returns TRUE if the class is public
    bool isJavaPublic();
    
    // set if the class is public, returns FALSE on error
    bool set_isJavaPublic(bool y);
    
    // returns TRUE is the class is final   
    bool isJavaFinal();
    
    // set if the class is final, returns FALSE on error
    bool set_isJavaFinal(bool y);
#endif
    
#ifdef WITHIDL
    // returns the switch's type, significant in case the class
    // is an union in IDL
    const UmlTypeSpec & switchType();
    
    // to set the switch type even the class is not (already) an IDL union
    // returns FALSE on error
    bool set_SwitchType(const UmlTypeSpec &);
    
    // returns TRUE if the class is external, its definition
    // must contain how the name is made on the first line
    // (${name} by default), the other lines are ignored
    bool isIdlExternal();
    
    // set if the class is external, returns FALSE on error
    bool set_isIdlExternal(bool y);
    
    // returns TRUE if the class is local, have sense for an interface
    bool isIdlLocal();
    
    // set if the class is local, returns FALSE on error, 
    // have sense for an interface
    bool set_isIdlLocal(bool y);
    
    // returns TRUE is the class is custom, have sense for a valuetype
    bool isIdlCustom();
    
    // set if the class is custom, returns FALSE on error, 
    // have sense for a valuetype
    bool set_isIdlCustom(bool y);
#endif
    
    // returns the class having the name given in argument in case it
    // exist, else 0. In case the package is specified, the class must
    // be defined in a sub-level of the package
    static UmlClass * get(const QCString & n, const UmlPackage * p = 0);
    
    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE. 
    // if 'del' is true the sub items are deleted, you will have to call
    // Children() to re-access to them
    virtual void unload(bool rec = FALSE, bool del = FALSE);
};

#endif
