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

#ifndef UMLBASEOPERATION_H
#define UMLBASEOPERATION_H

/* !!!!!!!!!! Do not modify this file !!!!!!!!!! */

#include <qvaluelist.h>

#include "UmlClassMember.h"
#include "UmlTypeSpec.h"

class UmlOperation;
class UmlClass;

struct UmlParameter {
  aDirection dir;
  UmlTypeSpec type;
  QCString name;
  QCString default_value;
};

class UmlBaseOperation : public UmlClassMember {    
  public:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseOperation(void * id, const QCString & n);
  
    // returns a new operation named 'name' created under 'parent', or
    // the null pointer in case it cannot be created (the name is
    // already used or invalid, 'parent' cannot contain it etc ...)
    static UmlOperation * create(UmlClass * parent, const char * name);
    
    // returns the kind of the item
    virtual anItemKind kind();
  
    // indicates if the operation is abstract, returns TRUE if yes
    bool isAbstract();
    
    // to set the 'abstract' flag, returns FALSE on error
    bool set_isAbstract(bool y);
    
    // returns the operation value type
    const UmlTypeSpec & returnType();
    
    // to set the operation value type, returns FALSE on error
    bool set_ReturnType(const UmlTypeSpec &);
    
    // returns the parameters
    const QValueList<UmlParameter> params();
    
    // remove the parameter of the given rank (0...), returns 0 on error
    bool removeParameter(unsigned rank);
    
    // adds a parameter at the given rank (0...), returns 0 on error
    bool addParameter(unsigned rank, const UmlParameter &);
    
    // replace the parameter at the given rank (0...), returns 0 on error
    bool replaceParameter(unsigned rank, const UmlParameter &);
    
    // returns the exceptions
    const QValueList<UmlTypeSpec> exceptions();
    
    //remove the exception of the given rank (0...), returns 0 on error
    bool removeException(unsigned rank);
    
    // adds the exception at the given rank (0...), returns 0 on error
    bool addException(unsigned rank, const UmlTypeSpec &);
    
    // replaces the exception at the given rank (0...), returns 0 on error
    bool replaceException(unsigned rank, const UmlTypeSpec &);
    
    // in case the operation is a 'get' operation, returns the associated
    // attribute or relation
    UmlClassMember * getOf();
    
    // in case the operation is a 'set' operation, returns the associated
    // attribute or relation
    UmlClassMember * setOf();
    
#ifdef WITHCPP
    // returns TRUE if the operation is declared const in C++
    bool isCppConst();
    
    // to set if the operation is declared const in C++, returns FALSE on error
    bool set_isCppConst(bool y);
    
    // returns TRUE if the operation is a friend in C++
    bool isCppFriend();
    
    // to set if the operation is a friend in C++, returns FALSE on error
    bool set_isCppFriend(bool y);
    
    // returns TRUE if the operation is declared virtual in C++
    bool isCppVirtual();
    
    // to set if the operation is declared virtual in C++, returns FALSE on error
    bool set_isCppVirtual(bool y);
    
    // returns TRUE if the operation is declared inline in C++
    bool isCppInline();
    
    // to set if the operation is declared inline in C++, returns FALSE on error
    bool set_isCppInline(bool y);
    
    // returns the operation's definition in C++, notes that the declaration
    // is returned by the inherited UmlBaseClassItem::CppDecl() operation
    const QCString & cppDef();
    
    // sets the operation's definition in C++, notes that the declaration
    // is set through the inherited UmlBaseClassItem::set_CppDecl() operation
    bool set_CppDef(const char *);
    
    // returns the operation's body in C++, useless if the def does not
    // contains ${body}. Note that the body is get each time from BOUML
    // for memory size reason
    QCString cppBody();
    
    // sets the operation's body in C++, useless if the def does not 
    // contains ${body}
    bool set_CppBody(const char *);
    
    // in case the operation is a 'get' or 'set' operation, returns how
    // the operation's C++ name must be generated
    const QCString & cppNameSpec();
    
    // in case the operation is a 'get' or 'set' operation, returns how
    // the operation's C++ name must be generated, returns FALSE on error
    bool set_CppNameSpec(const char *);
#endif
    
#ifdef WITHJAVA
    // returns TRUE if the operation is declared final in JAVA
    bool isJavaFinal();
    
    // to set if the operation is declared final in JAVA, returns FALSE on error
    bool set_isJavaFinal(bool y);
    
    // returns TRUE if the operation is declared synchronized in JAVA
    bool isJavaSynchronized();
    
    // to set if the operation is declared synchronized in JAVA,
    // returns FALSE on error
    bool set_isJavaSynchronized(bool y);
    
    // returns the operation's definition in Java, notes that it is
    // already made by the inherited JavaDecl operation
    const QCString & javaDef();
    
    // sets the operation's definition in Java, notes that it is
    // already made by the inherited set_JavaDecl operation
    bool set_JavaDef(const char *);
    
    // returns the operation's body in Java++, useless if the def does
    // not contains ${body} Note that the body is get each time from BOUML
    // for memory size reason
    QCString javaBody();
    
    // sets the operation's body in Java, useless if the def does not 
    // contains ${body}
    bool set_JavaBody(const char *);
    
    // in case the operation is a 'get' or 'set' operation, returns how
    // the operation's JAVA name must be generated
    const QCString & javaNameSpec();
    
    // in case the operation is a 'get' or 'set' operation, returns how
    // the operation's JAVA name must be generated, returns FALSE on error
    bool set_JavaNameSpec(const char *);
#endif
    
#ifdef WITHIDL
    // returns TRUE if the operation is declared oneway in IDL
    bool isIdlOneway();

    // to set if the operation is declared oneway in IDL
    bool set_isIdlOneway(bool y);
    
    // in case the operation is a 'get' or 'set' operation, returns how
    // the operation's IDL name must be generated
    const QCString & idlNameSpec();
    
    // in case the operation is a 'get' or 'set' operation, returns how
    // the operation's IDL name must be generated, returns FALSE on error
    bool set_IdlNameSpec(const char *);
#endif
    
    // to unload the object to free memory, it will be reloaded
    // automatically if needed. args unused
    virtual void unload(bool = FALSE, bool = FALSE);
    
    // not in plug-outs managed through bouml
    unsigned get_id() const;
    
  private:
    bool _abstract : 1;
#ifdef WITHCPP
    bool _cpp_const : 1;
    bool _cpp_friend : 1;
    bool _cpp_virtual : 1;
    bool _cpp_inline : 1;
#endif
#ifdef WITHJAVA
    bool _java_final : 1;
    bool _java_synchronized : 1;
#endif
#ifdef WITHIDL
    bool _idl_oneway;
#endif
    UmlTypeSpec _return_type;
    QValueList<UmlParameter> _params;
    QValueList<UmlTypeSpec> _exceptions;
#ifdef WITHCPP
    QCString _cpp_def;
    QCString _cpp_name_spec;
#endif
#ifdef WITHJAVA
    QCString _java_name_spec;
#endif
#ifdef WITHIDL
    QCString _idl_name_spec;
#endif
    UmlClassMember * _get_of;	// exclusive with set_of
    UmlClassMember * _set_of;	// exclusive with get_of
    
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
};

#endif
