// *************************************************************************
//
// Copyleft 2004-2008 Bruno PAGES  .
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

#include "CppRefType.h"
#include "UmlClass.h"
#include "UmlSettings.h"
#include "CppSettings.h"
#include "UmlPackage.h"
#include "UmlArtifact.h"
#include "UmlCom.h"
#include "util.h"

bool CppRefType::add(const UmlTypeSpec & t, QList<CppRefType> & l, bool incl)
{
  return (t.type)
    ? add(t.type, l, incl)
    : add(t.explicit_type, l, incl);
}

bool CppRefType::add(UmlClass * cl, QList<CppRefType> & l, bool incl)
{
  CppRefType * ref;
  QCString t = cl->name();
  
  if (cl->parent()->kind() == aClass) {
    // nested => need include parent
    do
      cl = (UmlClass *) cl->parent();
    while (cl->parent()->kind() == aClass);
    
    incl = TRUE;
  }
  
  for (ref = l.first(); ref; ref = l.next()) {
    // don't use ref->type.toString() because of synonymous
    // in several namespaces
    if ((ref->type.type != 0)
	? (ref->type.type == cl)
	: (ref->type.explicit_type == t)) {
      if (incl)
	ref->included = TRUE;
      return TRUE;
    }
  }
  
  l.append(new CppRefType(cl, incl));
  return TRUE;
}

bool CppRefType::add(const QCString & t, QList<CppRefType> & l, bool incl)
{
  if (t.isEmpty())
    return FALSE;

  static QDict<char> cpp_builtin_types;
  
  if (cpp_builtin_types.count() == 0) {
    cpp_builtin_types.insert("unsigned", " ");
    cpp_builtin_types.insert("signed", " ");
    cpp_builtin_types.insert("char", " ");
    cpp_builtin_types.insert("short", " ");
    cpp_builtin_types.insert("int", " ");
    cpp_builtin_types.insert("long", " ");
    cpp_builtin_types.insert("float", " ");
    cpp_builtin_types.insert("double", " ");
    cpp_builtin_types.insert("bool", " ");
    cpp_builtin_types.insert("void", " ");
  }
  
  if (cpp_builtin_types[t] != 0)
    return TRUE;
  
  CppRefType * ref;
  
  for (ref = l.first(); ref; ref = l.next()) {
    if (ref->type.toString() == t) {
      if (incl)
	ref->included = TRUE;
      return TRUE;
    }
  }
  
  l.append(new CppRefType(t, incl));
  return TRUE;
}

void CppRefType::remove(const QCString & t, QList<CppRefType> & l)
{
  QListIterator<CppRefType> it(l);
  
  for (; it.current(); ++it) {
    if ((*it)->type.explicit_type == t) {
      delete *it;
      l.remove(it);
      return;
    }
  }
}

void CppRefType::remove(UmlClass * cl, QList<CppRefType> & l)
{
  QListIterator<CppRefType> it(l);
  
  for (; it.current(); ++it) {
    if ((*it)->type.type == cl) {
      l.remove(it);
      return;
    }
  }
}

void CppRefType::force_ref(UmlClass * cl, QList<CppRefType> & l)
{
  CppRefType * ref;
  QCString t = cl->name();
  
  for (ref = l.first(); ref; ref = l.next()) {
    // don't use ref->type.toString() because of synonymous
    // in several namespaces
    if ((ref->type.type != 0)
	? (ref->type.type == cl)
	: (ref->type.explicit_type == t)) {
      ref->included = FALSE;
      return;
    }
  }
}

void CppRefType::compute(QList<CppRefType> & dependencies,
			 const QCString & hdef, const QCString & srcdef,
			 QCString & h_incl,  QCString & decl, QCString & src_incl,
			 UmlArtifact * who)
{
  UmlPackage * pack = who->package();
  QCString hdir;
  QCString srcdir;
  
  if (CppSettings::isRelativePath()) {
    QCString empty;
    
    hdir = pack->header_path(empty);
    srcdir = pack->source_path(empty);
  }
  else if (CppSettings::isRootRelativePath())
    hdir =  srcdir = UmlPackage::rootDir();
  
  // aze.cpp includes aze.h
  src_incl += "#include \"";
  if (CppSettings::includeWithPath())
    src_incl += pack->header_path(who->name(), srcdir);
  else {
    src_incl += who->name();
    src_incl += '.';
    src_incl += CppSettings::headerExtension();
  }
  src_incl += "\"\n";
  h_incl = "";	// to not be QCString::null
  decl = "";	// to not be QCString::null
  
  CppRefType * ref;
  
  for (ref = dependencies.first(); ref != 0; ref = dependencies.next()) {
    UmlClass * cl = (ref->type.type)
      ? ref->type.type
      : UmlBaseClass::get(ref->type.explicit_type, 0);
    bool included = ref->included;
    QCString hform;	// form in header
    QCString srcform;	// form in source
    
    if (cl == 0) {
      QCString in = CppSettings::include(ref->type.explicit_type);
      
      if (!in.isEmpty()) 
	hform = srcform = in + '\n';
      else
	// doesn't know what it is
	continue;
    }
    else if (cl->isCppExternal()) {
      hform = cl->cppDecl();
      
      int index;
      
      if ((index = hform.find('\n')) == -1)
	// wrong form
	continue;
      
      hform = hform.mid(index + 1) + '\n';
      
      for (;;) {
	if ((index = hform.find("${name}")) != -1)
	  hform.replace(index, 7, cl->name());
	else if ((index = hform.find("${Name}")) != -1)
	  hform.replace(index, 7, capitalize(cl->name()));
	else if ((index = hform.find("${NAME}")) != -1)
	  hform.replace(index, 7, cl->name().upper());
	else
	  break;
      }
      srcform = hform;
    }
    else {
      QCString st = cl->cpp_stereotype();	
      
      if ((st == "enum") || (st == "typedef"))
	included = TRUE;
      
      UmlArtifact * art = cl->associatedArtifact();
      
      if (art != 0) {
	if (art == who)
	  // don't include itself
	  continue;
	if (CppSettings::includeWithPath()) {
	  UmlPackage * p = art->package();
	  
	  hform = "#include \"" + p->header_path(art->name(), hdir) + "\"\n";
	  srcform = "#include \"" + p->header_path(art->name(), srcdir) + "\"\n";
	}
	else
	  srcform = hform = "#include \"" + art->name() + '.' +
	    CppSettings::headerExtension() + "\"\n";
      }
      else if (cl->parent()->kind() != aClass) {
	write_trace_header();
	UmlCom::trace(QCString("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b> class<i> ") + cl->name() +
		      "</i> referenced but does not have associated <i>artifact</i></b></font><br>");
	incr_warning();
	continue;
      }
    }
    
    if (included) {
      // #include must be placed in the header file
      if ((h_incl.find(hform) == -1) && (hdef.find(hform) == -1))
	h_incl += hform;
    }
    else if ((cl != 0) &&
	     !cl->isCppExternal() &&
	     (cl->parent()->kind() != aClass)) {	// else too complicated
      // #include useless in header file, place it in the source file
      if ((src_incl.find(srcform) == -1) && (h_incl.find(hform) == -1) &&
	  (hdef.find(hform) == -1) && (srcdef.find(srcform) == -1))
	src_incl += srcform;
      
      // header file must contains the declaration
      hform = cl->decl();
      
      if (decl.find(hform) == -1)
	decl += hform;

      if ((cl->associatedArtifact() == 0) &&
	  (cl->parent()->kind() != aClass)) {
	write_trace_header();
      	UmlCom::trace(QCString("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b> class<i> ") + cl->name() +
		      "</i> referenced but does not have associated <i>artifact</i></b></font><br>");
	incr_warning();
      }
    }
    else if (!hform.isEmpty()) {
      // have the #include form but does not know if it is a class or other,
      // generate the #include in the header file EXCEPT if the #include is
      // already in the header/source file to allow to optimize the generated
      // code
      if ((src_incl.find(srcform) == -1) && (h_incl.find(hform) == -1) &&
	  (hdef.find(hform) == -1) && (srcdef.find(srcform) == -1))
	h_incl += hform;
    }
  }
  
  // trier !
}
