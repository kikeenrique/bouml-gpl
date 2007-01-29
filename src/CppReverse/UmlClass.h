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

#ifndef UMLCLASS_H
#define UMLCLASS_H

#include "UmlBaseClass.h"

class ClassContainer;

class UmlFormalParameter : public UmlBaseFormalParameter {
  public:
    UmlFormalParameter(){};
    UmlFormalParameter(QCString t, QCString n, QCString v);
};

typedef QValueList<UmlFormalParameter> FormalParameterList;

class UmlActualParameter : public UmlBaseActualParameter {
  public:
    UmlActualParameter(){};
};

class UmlClass : public UmlBaseClass {
  public:
    UmlClass(void * id, const QCString & n);
  
    bool manage_inherit(ClassContainer * pack, 
			const QValueList<FormalParameterList> & tmplt
#ifdef REVERSE
			, bool libp
#endif
			);
    bool get_actuals(UmlClass * mother, ClassContainer * container, 
		     const QValueList<FormalParameterList> & formals);
    
    void set_under_construction(bool y, bool rec = FALSE);
    bool inside_its_definition();
    
    static void clear_usings() { Usings.clear(); }
    void using_it() { Usings.replace(name(), this); }
    static UmlClass * used(const char * n) { return Usings[n]; }
    static void save_using_scope() {
      UsingScope.prepend(Usings);
    }
    static void restore_using_scope();

#ifdef REVERSE
    void need_artifact(const QCString & nmsp);
    virtual bool need_source();
#endif
    
    private:
      UmlClass * auxilarily_typedef(const QCString & base
#ifdef REVERSE
				    , bool libp
#endif
				    );

      static QList<UmlClass> UnderConstruction;
      static QDict<UmlClass> Usings;
      static QValueList<QDict<UmlClass> > UsingScope;
};

#endif
