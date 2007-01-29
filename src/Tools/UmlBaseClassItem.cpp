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

/* !!!!!!!!!! Do not modify this file !!!!!!!!!! */

#include "UmlBaseClassItem.h"
#include "UmlCom.h"

#ifdef WITHCPP
void UmlBaseClassItem::read_cpp_() {
  _cpp_decl = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseClassItem::read_java_() {
  _java_decl = UmlCom::read_string();
}
#endif

#ifdef WITHIDL
void UmlBaseClassItem::read_idl_() {
  _idl_decl = UmlCom::read_string();
}
#endif

void UmlBaseClassItem::unload(bool rec, bool del) {
#ifdef WITHCPP
  _cpp_decl = 0;
#endif
#ifdef WITHJAVA
  _java_decl = 0;
#endif
#ifdef WITHIDL
  _idl_decl = 0;
#endif
  UmlBaseItem::unload(rec, del);
}

#ifdef WITHCPP
const QCString & UmlBaseClassItem::cppDecl() {
  read_if_needed_();
  
  return _cpp_decl;
}

bool UmlBaseClassItem::set_CppDecl(const char * s) {
  return set_it_(_cpp_decl, s, setCppDeclCmd);
}
#endif

#ifdef WITHJAVA
const QCString & UmlBaseClassItem::javaDecl() {
  read_if_needed_();
  
  return _java_decl;
}

bool UmlBaseClassItem::set_JavaDecl(const char * s) {
  return set_it_(_java_decl, s, setJavaDeclCmd);
}
#endif

#ifdef WITHIDL
const QCString & UmlBaseClassItem::idlDecl() {
  read_if_needed_();
  
  return _idl_decl;
}

bool UmlBaseClassItem::set_IdlDecl(const char * s) {
  return set_it_(_idl_decl, s, setIdlDeclCmd);
}
#endif
