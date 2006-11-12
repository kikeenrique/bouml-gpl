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

#include "UmlBaseExtraClassMember.h"
#include "UmlClass.h"
#include "UmlCom.h"

anItemKind  UmlBaseExtraClassMember::kind() {
  return anExtraClassMember;
}

#ifdef WITHCPP
void UmlBaseExtraClassMember::read_cpp_() {
  UmlBaseClassItem::read_cpp_();
  _cpp_def = UmlCom::read_string();
  _cpp_inline = UmlCom::read_bool();
}
#endif

void UmlBaseExtraClassMember::unload(bool rec, bool del) {
#ifdef WITHCPP
  _cpp_def = 0;
#endif
  UmlBaseClassItem::unload(rec, del);
}

UmlExtraClassMember *
  UmlBaseExtraClassMember::create(UmlClass * parent, const char * name)
{
  return (UmlExtraClassMember *) parent->create_(anExtraClassMember, name);
}
    
#ifdef WITHCPP
bool UmlBaseExtraClassMember::isCppInline() {
  read_if_needed_();
    
  return _cpp_inline;
}

bool UmlBaseExtraClassMember::set_isCppInline(bool y) {
  return set_it_(_cpp_inline, y, setIsCppInlineCmd);
}

const QCString & UmlBaseExtraClassMember::cppDef() {
  read_if_needed_();
    
  return _cpp_def;
}

bool UmlBaseExtraClassMember::set_CppDef(const char * s) {
  return set_it_(_cpp_def, s, setCppDefCmd);
}
#endif
