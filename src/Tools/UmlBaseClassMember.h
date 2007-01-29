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

#ifndef UMLBASECLASSMEMBER_H
#define UMLBASECLASSMEMBER_H

/* !!!!!!!!!! Do not modify this file !!!!!!!!!! */

#include "UmlClassItem.h"

// This class manages 'true' class's items : attributes, relation,
// operation and sub-classes

class UmlBaseClassMember : public UmlClassItem {
  public:
    UmlBaseClassMember(void * id, const QCString & n)
      : UmlClassItem(id, n) {};
  
    // Indicate if the member is a 'class member' (static)
    // Always false in case of a class
    bool isClassMember();
    
    // Set if the member is a 'class member' (static)
    // not significant for a class
    bool set_isClassMember(bool y);
    
    // Indicate if the member is 'volatile'
    // Always false in case of a class
    bool isVolatile();
    
    // Set if the member is 'volatile'
    // not significant for a class
    bool set_isVolatile(bool y);
    
    // The member's visibility
    aVisibility visibility();
    
    // Set the member visibility,  DefaultVisibility is not legal
    bool set_Visibility(aVisibility v);
    
#ifdef WITHCPP
    // Special case for C++, thank to the 'friend class X' it may be
    // usefull to have a C++ visibility != other visibility
    // In case the C++ visibility must follow the other, return
    // DefaultVisibility
    aVisibility cppVisibility();
    
    // Set the visibility for C++, DefaultVisibility is legal
    bool set_CppVisibility(aVisibility v);
#endif
    
#ifdef WITHJAVA
    // returns the Java annotations
    QCString javaAnnotations();
    
    // Set the Java annotations, return FALSE on error
    bool set_JavaAnnotations(QCString s);
#endif
  
  private:
    friend class UmlBaseAttribute;
    friend class UmlBaseOperation;
    friend class UmlBaseRelation;
    friend class UmlBaseClass;
    
    bool _class_member;
    bool _volatile;
    aVisibility _visibility : 8;
#ifdef WITHCPP
    aVisibility _cpp_visibility : 8;
#endif
    
#ifdef WITHJAVA
    QCString _java_annotation;
#endif
    
    virtual void read_uml_();
#ifdef WITHCPP
    virtual void read_cpp_();
#endif
    
#ifdef WITHJAVA
    virtual void read_java_();
#endif
};

#endif
