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

#ifndef UMLCLASS_H
#define UMLCLASS_H

#include <qdict.h>

#include "UmlBaseClass.h"

class UmlPackage;
class UmlClass;
class UmlClassView;
class UmlArtifact;
class UmlDeploymentView;
class UmlOperation;
class UmlAttribute;
class UmlRelation;


// This class manages 'formal parameters' of a template class.
// For instance 'T' in 
//	template class Cl<class T> ...
// You can modify it as you want (except the constructor)

class UmlFormalParameter : public UmlBaseFormalParameter {
  public:
    UmlFormalParameter(){};
};

// This class manages 'actual parameters' of a template class.
// for instance T in
//	class Cl1 : public Cl2<T> ...
// You can modify it as you want (except the constructor)

class UmlActualParameter : public UmlBaseActualParameter {
  public:
    UmlActualParameter(){};
};

// This class manages 'classes'
// You can modify it as you want (except the constructor)

class UmlClass : public UmlBaseClass {
  public:
    UmlClass(void * id, const QCString & n) 
      : UmlBaseClass(id, n) {};
  
    void made(UmlClassView * base_class_view, UmlClassView * user_class_view,
	      UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
	      UmlClass * base_item, const QCString & s, UmlClass *& base,
	      UmlClass *& user, UmlClass * user_interf = 0);
    
    void made(UmlClassView * base_class_view, UmlClassView * user_class_view,
	      UmlArtifact * base_art, UmlArtifact * user_art,
	      UmlClass * base_item, const QCString & s, UmlClass *& base,
	      UmlClass *& user, UmlClass * user_interf = 0);
    
    static UmlClass * made(UmlClassView * class_view,
			   UmlDeploymentView * depl_view,
			   const QCString & s, bool interf = FALSE);
    
    static UmlClass * made(UmlClassView * class_view,
			   UmlArtifact * art,
			   const QCString & s, bool interf = FALSE);
    
    void add_kind(const char * k);
    void add_default_base_op(UmlClass * super, UmlClass * user,
			     UmlClass * parent, QCString s,
			     const char * k, bool unnamed = FALSE);
    void add_constr(UmlClass * super, aVisibility, bool unnamed = FALSE);
    UmlOperation * add_op(const char * name, aVisibility, 
			  UmlClass * return_type, bool excpt = FALSE);
    UmlOperation * add_op(const char * name, aVisibility, 
			  const char * return_type, bool excpt = FALSE);
    UmlOperation * add_op(const char * name, aVisibility, 
			  UmlTypeSpec & return_type, bool excpt = FALSE);
    UmlAttribute * add_attribute(const char * name, aVisibility v,
				 UmlClass * type,
				 const char * if_def, const char * end_if);
    UmlAttribute * add_attribute(const char * name, aVisibility v,
				 const char * type,
				 const char * if_def, const char * end_if);
    UmlAttribute * add_attribute(const char * name, aVisibility v,
				 UmlTypeSpec & type,
				 const char * if_def, const char * end_if);
    UmlAttribute * add_enum_item(const char * name);
    UmlRelation * add_relation(aRelationKind k, const char * name,
			       aVisibility v, UmlClass * type,
			       const char * if_def, const char * end_if);
    UmlOperation * get_operation(const char *);
};

#endif
