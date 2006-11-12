// *************************************************************************
//
// Copyright (C) 2004-2006 Bruno PAGES  All rights reserved.
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

#ifndef _UMLBASEEXTRACLASSMEMBER_H
#define _UMLBASEEXTRACLASSMEMBER_H

#include "UmlClassItem.h"

class UmlExtraClassMember;
class UmlClass;

class UmlBaseExtraClassMember : public UmlClassItem {
  public:
    UmlBaseExtraClassMember(void * id, const QCString & n)
      : UmlClassItem(id, n) {};
  
    // returns a new extra class member named 'name'  (the name may be empty)
    // created under the class 'parent', or the null pointer in case it cannot be created
    static UmlExtraClassMember * create(UmlClass * parent, const char * name);
    
    // returns the kind of the item
    virtual anItemKind kind();
  
#ifdef WITHCPP
    // returns TRUE if the extra member is managed as an inline operation in C++
    bool isCppInline();
    
    // to set if the extra member is managed as an inline operation in C++,
    // returns FALSE on error
    bool set_isCppInline(bool y);
    
    // returns the operation's definition in C++, notes that the declaration
    // is returned by the inherited ClassItemBase::CppDecl() operation
    const QCString & cppDef();

    // sets the operation's definition in C++, notes that the declaration
    // is set through the inherited ClassItemBase::set_CppDecl() operation
    bool set_CppDef(const char * s);
#endif
    
    // to unload the object to free memory, it will be reloaded
    // automatically if needed. args unused
    virtual void unload(bool = FALSE, bool = FALSE);
    
  private:
#ifdef WITHCPP
    bool _cpp_inline;
    QCString _cpp_def;

    virtual void read_cpp_();
#endif
};

#endif
