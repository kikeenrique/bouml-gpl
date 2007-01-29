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

#include "UmlBaseOperation.h"
#include "UmlBaseRelation.h"
#include "UmlClass.h"
#include "UmlCom.h"

UmlBaseOperation::UmlBaseOperation(void * id, const QCString & n)
    : UmlClassMember(id, n) {
  _get_of = 0;
  _set_of = 0;
}

anItemKind UmlBaseOperation::kind() {
  return anOperation;
}

void UmlBaseOperation::read_uml_() {
  UmlBaseClassMember::read_uml_();
  _return_type.type = (UmlClass *) UmlBaseItem::read_();
  if (_return_type.type == 0)
    _return_type.explicit_type = UmlCom::read_string();
  _abstract = UmlCom::read_bool();
  
  unsigned n;
  
  for (n = UmlCom::read_unsigned(); n; n -= 1) {
    UmlParameter param;
    
    param.dir = (aDirection) UmlCom::read_char();
    param.type.type = (UmlClass *) UmlBaseItem::read_();
    if (param.type.type == 0)
      param.type.explicit_type = UmlCom::read_string();
    param.name = UmlCom::read_string();
    param.default_value = UmlCom::read_string();
    _params.append(param);
  }
  
  for (n = UmlCom::read_unsigned(); n; n -= 1) {
    UmlTypeSpec exception;
    
    exception.type = (UmlClass *) UmlBaseItem::read_();
    if (exception.type == 0)
      exception.explicit_type = UmlCom::read_string();
    _exceptions.append(exception);
  }
  _get_of = (UmlClassMember *) UmlBaseItem::read_();
  _set_of = (UmlClassMember *) UmlBaseItem::read_();
}

#ifdef WITHCPP
void UmlBaseOperation::read_cpp_() {
  UmlBaseClassMember::read_cpp_();
  _cpp_const = UmlCom::read_bool();
  _cpp_friend = UmlCom::read_bool();
  _cpp_virtual = UmlCom::read_bool();
  _cpp_inline = UmlCom::read_bool();
  _cpp_def = UmlCom::read_string();
  _cpp_name_spec = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseOperation::read_java_() {
  UmlBaseClassMember::read_java_();
  _java_final = UmlCom::read_bool();
  _java_synchronized = UmlCom::read_bool();
  _java_name_spec = UmlCom::read_string();
}
#endif

#ifdef WITHIDL
void UmlBaseOperation::read_idl_() {
  UmlBaseClassMember::read_idl_();
  _idl_oneway = UmlCom::read_bool();
  _idl_name_spec = UmlCom::read_string();
}
#endif

void UmlBaseOperation::unload(bool rec, bool del) {
  _return_type.explicit_type = 0;
  _params.clear();
  _exceptions.clear();
#ifdef WITHCPP
  _cpp_def = 0;
  _cpp_name_spec = 0;
#endif
#ifdef WITHJAVA
  _java_name_spec = 0;
#endif
#ifdef WITHIDL
  _idl_name_spec = 0;
#endif
  UmlBaseClassMember::unload(rec, del);
}

bool UmlBaseOperation::isAbstract() {
  read_if_needed_();
    
  return _abstract;
}

bool UmlBaseOperation::set_isAbstract(bool y) {
  bool b;
  
  if (set_it_(b, y, setIsAbstractCmd)) {
    _abstract = y;
    if (y)
      ((UmlBaseClass *) parent())->set_isAbstract(y);
    return TRUE;
  }
  else
    return FALSE;
}

const UmlTypeSpec & UmlBaseOperation::returnType() {
  read_if_needed_();
    
  return _return_type;
}

bool UmlBaseOperation::set_ReturnType(const UmlTypeSpec & t) {
  return set_it_(_return_type, t, setReturnTypeCmd);
}

const QValueList<UmlParameter> UmlBaseOperation::params() {
  read_if_needed_();
    
  return _params;
}

bool UmlBaseOperation::removeParameter(unsigned rank) {
  UmlCom::send_cmd(_identifier, removeParameterCmd, rank);
  if (UmlCom::read_ack()) {
    if (_defined)
      _params.remove(_params.at(rank));
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseOperation::addParameter(unsigned rank, const UmlParameter & p) {
  UmlCom::send_cmd(_identifier, addParameterCmd, rank, p.dir, p.name, 
		   p.default_value, p.type);
  if (UmlCom::read_ack()) {
    if (_defined)
      _params.insert(_params.at(rank), p);
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseOperation::replaceParameter(unsigned rank, const UmlParameter & p) {
  UmlCom::send_cmd(_identifier, replaceParameterCmd, rank, p.dir, p.name, 
		   p.default_value, p.type);
  if (UmlCom::read_ack()) {
    if (_defined)
      _params[rank] = p;
    return TRUE;
  }
  else
    return FALSE;
}

const QValueList<UmlTypeSpec> UmlBaseOperation::exceptions() {
  read_if_needed_();
    
  return _exceptions;
}

bool UmlBaseOperation::removeException(unsigned rank) {
  UmlCom::send_cmd(_identifier, removeExceptionCmd, rank);
  if (UmlCom::read_ack()) {
    if (_defined)
      _exceptions.remove(_exceptions.at(rank));
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseOperation::addException(unsigned rank, const UmlTypeSpec & t) {
  UmlCom::send_cmd(_identifier, addExceptionCmd, rank, t);
  if (UmlCom::read_ack()) {
    if (_defined)
      _exceptions.insert(_exceptions.at(rank), t);
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseOperation::replaceException(unsigned rank, const UmlTypeSpec & t) {
  UmlCom::send_cmd(_identifier, replaceExceptionCmd, rank, t);
  if (UmlCom::read_ack()) {
    if (_defined)
      _exceptions[rank] = t;
    return TRUE;
  }
  else
    return FALSE;
}

UmlClassMember * UmlBaseOperation::getOf() {
  read_if_needed_();
    
  return _get_of;
}

UmlClassMember * UmlBaseOperation::setOf() {
  read_if_needed_();
    
  return _set_of;
}

#ifdef WITHCPP
bool UmlBaseOperation::isCppConst() {
  read_if_needed_();
    
  return _cpp_const;
}

bool UmlBaseOperation::set_isCppConst(bool y) {
  bool b;
  
  if (set_it_(b, y, setIsCppConstCmd)) {
    _cpp_const = y;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseOperation::isCppFriend() {
  read_if_needed_();
    
  return _cpp_friend;
}

bool UmlBaseOperation::set_isCppFriend(bool y) {
  bool b;
  
  if (set_it_(b, y, setIsCppFriendCmd)) {
    _cpp_friend = y;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseOperation::isCppVirtual() {
  read_if_needed_();
    
  return _cpp_virtual;
}

bool UmlBaseOperation::set_isCppVirtual(bool y) {
  bool b;
  
  if (set_it_(b, y, setIsCppVirtualCmd)) {
    _cpp_virtual = y;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseOperation::isCppInline() {
  read_if_needed_();
    
  return _cpp_inline;
}

bool UmlBaseOperation::set_isCppInline(bool y) {
  bool b;
  
  if (set_it_(b, y, setIsCppInlineCmd)) {
    _cpp_inline = y;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & UmlBaseOperation::cppDef() {
  read_if_needed_();
    
  return _cpp_def;
}

bool UmlBaseOperation::set_CppDef(const char * s) {
  return set_it_(_cpp_def, s, setCppDefCmd);
}

QCString UmlBaseOperation::cppBody() {
  // not memorized in the instance for memory size reason
  UmlCom::send_cmd(_identifier, cppBodyCmd);
  return UmlCom::read_string();
}

bool UmlBaseOperation::set_CppBody(const char * s) {
  // not memorized in the instance for memory size reason
  UmlCom::send_cmd(_identifier, setCppBodyCmd, s);
  return UmlCom::read_ack();
}

const QCString & UmlBaseOperation::cppNameSpec() {
  read_if_needed_();
    
  return _cpp_name_spec;
}

bool UmlBaseOperation::set_CppNameSpec(const char * s) {
  return set_it_(_cpp_name_spec, s, setCppNameSpecCmd);
}
#endif

#ifdef WITHJAVA
bool UmlBaseOperation::isJavaFinal() {
  read_if_needed_();
    
  return _java_final;
}

bool UmlBaseOperation::set_isJavaFinal(bool y) {
  bool b;
  
  if (set_it_(b, y, setJavaFinalCmd)) {
    _java_final = y;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseOperation::isJavaSynchronized() {
  read_if_needed_();
    
  return _java_synchronized;
}

bool UmlBaseOperation::set_isJavaSynchronized(bool y) {
  bool b;
  
  if (set_it_(b, y, setJavaSynchronizedCmd)) {
    _java_synchronized = y;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & UmlBaseOperation::javaDef() {
  return javaDecl();
}

bool UmlBaseOperation::set_JavaDef(const char * s) {
  return set_JavaDecl(s);
}

QCString UmlBaseOperation::javaBody() {
  // not memorized in the instance for memory size reason
  UmlCom::send_cmd(_identifier, javaBodyCmd);
  return UmlCom::read_string();
}

bool UmlBaseOperation::set_JavaBody(const char * s) {
  // not memorized in the instance for memory size reason
  UmlCom::send_cmd(_identifier, setJavaBodyCmd, s);
  return UmlCom::read_ack();
}

const QCString & UmlBaseOperation::javaNameSpec() {
  read_if_needed_();
    
  return _java_name_spec;
}

bool UmlBaseOperation::set_JavaNameSpec(const char * s) {
  return set_it_(_java_name_spec, s, setJavaNameSpecCmd);
}
#endif

#ifdef WITHIDL
bool UmlBaseOperation::isIdlOneway() {
  read_if_needed_();
    
  return _idl_oneway;
}

bool UmlBaseOperation::set_isIdlOneway(bool y) {
  bool b;
  
  if (set_it_(b, y, setIsIdlOnewayCmd)) {
    _idl_oneway = y;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & UmlBaseOperation::idlNameSpec() {
  read_if_needed_();
    
  return _idl_name_spec;
}

bool UmlBaseOperation::set_IdlNameSpec(const char * s) {
  return set_it_(_idl_name_spec, s, setIdlNameSpecCmd);
}
#endif

UmlOperation * UmlBaseOperation::create(UmlClass * parent, const char * s)
{
  return (UmlOperation *) parent->create_(anOperation, s);
}

// not in plug-outs managed through bouml
unsigned UmlBaseOperation::get_id() const {
  UmlCom::send_cmd(_identifier, getIdCmd);
  return UmlCom::read_unsigned();
}

