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

#include "UmlAttribute.h"
#include "UmlClass.h"
#include "UmlCom.h"

UmlBaseAttribute::UmlBaseAttribute(void * id, const QCString & n)
    :  UmlClassMember(id, n) {
  _get_oper = 0;
  _set_oper = 0;
  
#ifdef WITHIDL
  _idl_case = 0;
#endif
}

anItemKind UmlBaseAttribute::kind() {
  return anAttribute;
}

void UmlBaseAttribute::read_uml_() {
  UmlBaseClassMember::read_uml_();
  _type.type = (UmlClass *) UmlBaseItem::read_();
  if (_type.type == 0)
    _type.explicit_type = UmlCom::read_string();
  _default_value = UmlCom::read_string();
  _read_only = UmlCom::read_bool();
  _get_oper = (UmlOperation *) UmlBaseItem::read_();
  _set_oper = (UmlOperation *) UmlBaseItem::read_();
}

#ifdef WITHCPP
void UmlBaseAttribute::read_cpp_() {
  UmlBaseClassMember::read_cpp_();
  _cpp_mutable = UmlCom::read_bool();
}
#endif

#ifdef WITHJAVA
void UmlBaseAttribute::read_java_() {
  UmlBaseClassMember::read_java_();
  _java_transient = UmlCom::read_bool();
}
#endif

#ifdef WITHIDL
void UmlBaseAttribute::read_idl_() {
  UmlBaseClassItem::read_idl_();
  _idl_case = (UmlAttribute *) UmlBaseItem::read_();
  if (_idl_case == 0)
    _idl_explicit_case = UmlCom::read_string();
}
#endif

void UmlBaseAttribute::unload(bool rec, bool del) {
  _type.explicit_type = 0;
  _default_value = 0;
#ifdef WITHIDL
  _idl_explicit_case = 0;
#endif
  UmlBaseClassMember::unload(rec, del);
}

bool UmlBaseAttribute::isReadOnly() {
  read_if_needed_();
  
  return _read_only;
}

bool UmlBaseAttribute::set_isReadOnly(bool y) {
  return set_it_(_read_only, y, setIsReadOnlyCmd);
}

const QCString & UmlBaseAttribute::defaultValue() {
  read_if_needed_();
  
  return _default_value;
}

bool UmlBaseAttribute::set_DefaultValue(const char * s) {
  return set_it_(_default_value, s, setDefaultValueCmd);
}

const UmlTypeSpec & UmlBaseAttribute::type() {
  read_if_needed_();
  
  return _type;
}
   
bool UmlBaseAttribute::set_Type(const UmlTypeSpec & t) {
  return set_it_(_type, t, setTypeCmd);
}

UmlOperation * UmlBaseAttribute::getOperation() {
  read_if_needed_();
  
  return _get_oper;
}
 
bool UmlBaseAttribute::addGetOperation() {
  UmlCom::send_cmd(_identifier, addGetOperCmd);
  if (UmlCom::read_ack()) {
    reread_children_if_needed_();
    return TRUE;
  }
 else
   return FALSE;
}

UmlOperation * UmlBaseAttribute::setOperation() {
  read_if_needed_();
  
  return _set_oper;
}

bool UmlBaseAttribute::addSetOperation() {
  UmlCom::send_cmd(_identifier, addSetOperCmd);
  if (UmlCom::read_ack()) {
    reread_children_if_needed_();
    return TRUE;
  }
 else
   return FALSE;
}

#ifdef WITHCPP
bool UmlBaseAttribute::isCppMutable() {
  read_if_needed_();
  
  return _cpp_mutable;
}
    
bool UmlBaseAttribute::set_isCppMutable(bool y) {
  return set_it_(_cpp_mutable, y, setIsCppMutableCmd);
}
#endif

#ifdef WITHJAVA
bool UmlBaseAttribute::isJavaTransient() {
  read_if_needed_();
  
  return _java_transient;
}
    
bool UmlBaseAttribute::set_isJavaTransient(bool y) {
  return set_it_(_java_transient, y, setIsJavaTransientCmd);
}
#endif

#ifdef WITHIDL
QCString UmlBaseAttribute::idlCase() {
  read_if_needed_();
  
  return (_idl_case != 0) ? _idl_case->name() : _idl_explicit_case;
}

bool UmlBaseAttribute::set_IdlCase(UmlAttribute * a) {
  UmlCom::send_cmd(_identifier, setIdlCaseCmd, a->_identifier, "");
  if (UmlCom::read_ack()) {
    _idl_case = a;
    _idl_explicit_case = 0;
    return TRUE;
  }
 else
   return FALSE;
}

bool UmlBaseAttribute::set_IdlCase(const char * s) {
  UmlCom::send_cmd(_identifier, setIdlCaseCmd, (void *) 0, s);
  if (UmlCom::read_ack()) {
    _idl_case = 0;
    _idl_explicit_case = s;
    return TRUE;
  }
 else
   return FALSE;
}
#endif

UmlAttribute * UmlBaseAttribute::create(UmlClass * parent, const char * s)
{
  return (UmlAttribute *) parent->create_(anAttribute, s);
}
