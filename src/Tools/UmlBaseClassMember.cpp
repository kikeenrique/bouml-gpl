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

/* !!!!!!!!!! Do not modify this file !!!!!!!!!! */

#include "UmlBaseClassMember.h"
#include "UmlCom.h"

void UmlBaseClassMember::read_uml_() {
  UmlBaseItem::read_uml_();
  _class_member = UmlCom::read_bool();
  _volatile = UmlCom::read_bool();
  _visibility = (aVisibility) UmlCom::read_char();
}

#ifdef WITHCPP
void UmlBaseClassMember::read_cpp_() {
  UmlBaseClassItem::read_cpp_();
  _cpp_visibility = (aVisibility) UmlCom::read_char();
}
#endif

#ifdef WITHJAVA
void UmlBaseClassMember::read_java_() {
  UmlBaseClassItem::read_java_();
  _java_annotation = UmlCom::read_string();
}
#endif

bool UmlBaseClassMember::isClassMember() {
  read_if_needed_();
  
  return _class_member;
}

bool UmlBaseClassMember::set_isClassMember(bool y) {
  return set_it_(_class_member, y, setIsClassMemberCmd);
}

bool UmlBaseClassMember::isVolatile() {
  read_if_needed_();
  
  return _volatile;
}

bool UmlBaseClassMember::set_isVolatile(bool y) {
  return set_it_(_volatile, y, setIsVolatileCmd);
}

aVisibility UmlBaseClassMember::visibility() {
  read_if_needed_();
  
  return _visibility;
}

bool UmlBaseClassMember::set_Visibility(aVisibility v) {
  aVisibility vi;
  
  if ((v != DefaultVisibility) && set_it_(vi, v, setVisibilityCmd)) {
    _visibility = v;
    return TRUE;
  }
  else
    return FALSE;
}

#ifdef WITHCPP
aVisibility UmlBaseClassMember::cppVisibility() {
  read_if_needed_();
  
  return _cpp_visibility;
}

bool UmlBaseClassMember::set_CppVisibility(aVisibility v) {
  aVisibility vi;
  
  if (set_it_(vi, v, setCppVisibilityCmd)) {
    _cpp_visibility = v;
    return TRUE;
  }
  else
    return FALSE;
}
#endif

#ifdef WITHJAVA
QCString UmlBaseClassMember::javaAnnotations() {
  read_if_needed_();
  
  return _java_annotation;
}
    
bool UmlBaseClassMember::set_JavaAnnotations(QCString s) {
  return set_it_(_java_annotation, s, setJavaAnnotationCmd);
}
#endif
