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

#include "UmlBasePackage.h"
#include "UmlDiagram.h"
#include "UmlPackage.h"
#include "UmlCom.h"

UmlBasePackage::UmlBasePackage(void * id, const QCString & n) : UmlItem(id, n) {
  _assoc_diagram = 0;
}

anItemKind UmlBasePackage::kind() {
  return aPackage;
}

void UmlBasePackage::read_uml_() {
  _assoc_diagram = (UmlDiagram *) UmlBaseItem::read_();
  UmlBaseItem::read_uml_();
}

#ifdef WITHCPP
void UmlBasePackage::read_cpp_() {
  _cpp_src_dir = UmlCom::read_string();
  _cpp_h_dir = UmlCom::read_string();
  _cpp_namespace = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBasePackage::read_java_() {
  _java_dir = UmlCom::read_string();
  _java_package = UmlCom::read_string();
}
#endif


#ifdef WITHIDL
void UmlBasePackage::read_idl_() {
  _idl_dir = UmlCom::read_string();
  _idl_module = UmlCom::read_string();
}
#endif

void UmlBasePackage::unload(bool rec, bool del) {
  _assoc_diagram = 0;
#ifdef WITHCPP
  _cpp_src_dir = 0;
  _cpp_h_dir = 0;
  _cpp_namespace = 0;
#endif
#ifdef WITHJAVA
  _java_dir = 0;
  _java_package = 0;
#endif
#ifdef WITHIDL
  _idl_dir = 0;
  _idl_module = 0;
#endif
  UmlBaseItem::unload(rec, del);
}

//

UmlDiagram * UmlBasePackage::associatedDiagram() {
  read_if_needed_();
  
  return _assoc_diagram;
}

bool UmlBasePackage::set_AssociatedDiagram(UmlDiagram * d) {
  UmlCom::send_cmd(_identifier, setAssocDiagramCmd, ((UmlBaseItem *) d)->_identifier);
  if (UmlCom::read_ack()) {
    _assoc_diagram = d;
    return TRUE;
  }
  else
    return FALSE;
}

#ifdef WITHCPP
const QCString & UmlBasePackage::cppSrcDir() {
  read_if_needed_();
  
  return _cpp_src_dir;
}

bool UmlBasePackage::set_CppSrcDir(const QCString & s) {
  return set_it_(_cpp_src_dir, s, setCppSrcDirCmd);
}

const QCString & UmlBasePackage::cppHDir() {
  read_if_needed_();
  
  return _cpp_h_dir;
}

bool UmlBasePackage::set_CppHDir(const QCString & s) {
  return set_it_(_cpp_h_dir, s, setCppHDirCmd);
}

const QCString & UmlBasePackage::cppNamespace() {
  read_if_needed_();
  
  return _cpp_namespace;
}

bool UmlBasePackage::set_CppNamespace(const QCString & s) {
  return set_it_(_cpp_namespace, s, setCppNamespaceCmd);
}

UmlPackage * UmlBasePackage::findNamespace(const QCString & n) const
{
  UmlCom::send_cmd(packageGlobalCmd, findNamespaceCmd, _identifier, n);
  
  return (UmlPackage *) UmlBaseItem::read_();  
}

#endif

#ifdef WITHJAVA
const QCString & UmlBasePackage::javaDir() {
  read_if_needed_();
  
  return _java_dir;
}

bool UmlBasePackage::set_JavaDir(const QCString & s) {
  return set_it_(_java_dir, s, setJavaDirCmd);
}

const QCString & UmlBasePackage::javaPackage() {
  read_if_needed_();
  
  return _java_package;
}

bool UmlBasePackage::set_JavaPackage(const QCString & s) {
  return set_it_(_java_package, s, setJavaPackageCmd);
}

UmlPackage * UmlBasePackage::findPackage(const QCString & n) const {
  UmlCom::send_cmd(packageGlobalCmd, findPackageCmd, _identifier, n);
  
  return (UmlPackage *) UmlBaseItem::read_();  
}

#endif

#ifdef WITHIDL
const QCString & UmlBasePackage::idlDir() {
  read_if_needed_();
  
  return _idl_dir;
}

bool UmlBasePackage::set_IdlDir(const QCString & s) {
  return set_it_(_idl_dir, s, setIdlDirCmd);
}

const QCString & UmlBasePackage::idlModule() {
  read_if_needed_();
  
  return _idl_module;
}

bool UmlBasePackage::set_IdlModule(const QCString & s) {
  return set_it_(_idl_module, s, setIdlModuleCmd);
}

UmlPackage * UmlBasePackage::findModule(const QCString & n) const {
  UmlCom::send_cmd(packageGlobalCmd, findModuleCmd, _identifier, n);
  
  return (UmlPackage *) UmlBaseItem::read_();  
}

#endif

UmlPackage * UmlBasePackage::create(UmlPackage * parent, const char * name)
{
  return (UmlPackage *) parent->create_(aPackage, name);
}

UmlPackage * UmlBasePackage::getProject() {
  UmlCom::send_cmd(packageGlobalCmd, getProjectCmd);
  
  return (UmlPackage *) UmlBaseItem::read_();  
}
