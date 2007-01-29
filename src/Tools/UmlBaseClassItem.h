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

#ifndef UMLBASECLASSITEM_H
#define UMLBASECLASSITEM_H

/* !!!!!!!!!! Do not modify this file !!!!!!!!!! */

#include "UmlItem.h"

class UmlBaseClassItem : public UmlItem {
  friend class UmlBaseExtraClassMember;
  friend class UmlBaseClassMember;
  friend class UmlBaseAttribute;
  friend class UmlBaseOperation;
  friend class UmlBaseRelation;
  friend class UmlBaseClass;
    
  public:
    UmlBaseClassItem(void * id, const QCString & n)
      : UmlItem(id, n) {};
  
#ifdef WITHCPP
    const QCString & cppDecl();
    bool set_CppDecl(const char *);
#endif
#ifdef WITHJAVA
    const QCString & javaDecl();
    bool set_JavaDecl(const char *);
#endif
#ifdef WITHIDL
    const QCString & idlDecl();
    bool set_IdlDecl(const char *);
#endif
  
    virtual void unload(bool rec = FALSE, bool del = FALSE);
    
  private:
#ifdef WITHCPP
    QCString _cpp_decl;
    virtual void read_cpp_();
#endif
#ifdef WITHJAVA
    QCString _java_decl;
    virtual void read_java_();
#endif
#ifdef WITHIDL
    QCString _idl_decl;
    virtual void read_idl_();
#endif
};

#endif
