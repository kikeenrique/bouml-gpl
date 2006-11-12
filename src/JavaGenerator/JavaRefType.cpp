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

/*
#include "JavaRefType.h"
#include "UmlClass.h"
#include "UmlSettings.h"
#include "JavaSettings.h"
#include "UmlPackage.h"
#include "UmlArtifact$.h"
#include "UmlCom.h"

bool JavaRefType::add(const UmlTypeSpec & t, QList<JavaRefType> & l)
{
  return (t.type)
    ? add(t.type, l)
    : add(t.explicit_type, l);
}

bool JavaRefType::add(UmlClass * cl, QList<JavaRefType> & l)
{
  JavaRefType * ref;
  QCString t = cl->name();
  
  for (ref = l.first(); ref; ref = l.next())
    if (ref->type.toString() == t)
      return TRUE;
  
  l.append(new JavaRefType(cl));
  return TRUE;
}

bool JavaRefType::add(const QCString & t, QList<JavaRefType> & l)
{
  if (t.isEmpty())
    return FALSE;

  static QDict<char> java_builtin_types;
  
  if (java_builtin_types.count() == 0) {
    java_builtin_types.insert("boolean", " ");
    java_builtin_types.insert("char", " ");
    java_builtin_types.insert("byte", " ");
    java_builtin_types.insert("short", " ");
    java_builtin_types.insert("int", " ");
    java_builtin_types.insert("long", " ");
    java_builtin_types.insert("float", " ");
    java_builtin_types.insert("double", " ");
    java_builtin_types.insert("Object", " ");
  }
  
  if (java_builtin_types[t] != 0)
    return TRUE;
  
  JavaRefType * ref;
  
  for (ref = l.first(); ref; ref = l.next())
    if (ref->type.toString() == t)
      return TRUE;
  
  l.append(new JavaRefType(t));
  return TRUE;
}

void JavaRefType::remove(const QCString & t, QList<JavaRefType> & l)
{
  QListIterator<JavaRefType> it(l);
  
  for (; it.current(); ++it) {
    if ((*it)->type.explicit_type == t) {
      delete *it;
      l.remove(it);
      return;
    }
  }
}

void JavaRefType::remove(UmlClass * cl, QList<JavaRefType> & l)
{
  QListIterator<JavaRefType> it(l);
  
  for (; it.current(); ++it) {
    if ((*it)->type.type == cl) {
      l.remove(it);
      return;
    }
  }
}

void JavaRefType::compute(QList<JavaRefType> & dependencies,
			  const QCString & filedef, QCString & incl,
			  UmlArtifact$ * who)
{
  incl = "";	// to not be QCString::null
  
  JavaRefType * ref;
  
  for (ref = dependencies.first(); ref != 0; ref = dependencies.next()) {
    UmlClass * cl = (ref->type.type != 0)
      ? ref->type.type
      : UmlBaseClass::get(ref->type.explicit_type);
    QCString in;
    
    if (cl == 0)
      in = JavaSettings::import(ref->type.explicit_type);
    else {
      in = who->package()->javaPackage();
      
      if (!in.isEmpty())
	in = "import " + in + ';';
    }
    
    if (!in.isEmpty() && (incl.find(in) == -1) && (filedef.find(in) == -1))
      incl += in + '\n';
  }
}
*/
