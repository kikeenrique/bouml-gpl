// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
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
#include <qlist.h>

#include "UmlBaseClass.h"
#include "UmlTypeSpec.h"
#include "UmlFormalParameter.h"
#include "UmlActualParameter.h"

class QTextStream;

class UmlPackage;
class UmlClass;
class CppRefType;

class UmlClass : public UmlBaseClass {
  private:
    bool managed;
    
    static QList<UmlClass> context;
    
    static QValueList<UmlActualParameter> noactuals;
    
  public:
    UmlClass(void * id, const QByteArray & n)
      : UmlBaseClass(id, n), managed(FALSE) {};
  
    QByteArray cpp_stereotype();
    
    void compute_dependencies(QList<CppRefType> &, bool all_in_h);
    
    virtual void compute_dependency(QList<CppRefType> & dependency,
				    const QByteArray & cl_stereotype,
				    bool all_in_h);
    virtual void generate_decl(aVisibility & current_visibility,
			       QTextStream & f_h,
			       const QByteArray & cl_stereotype,
			       QByteArray indent,
			       BooL & first, bool last);
    void generate_def(QTextStream & f, QByteArray indent, bool h);
    virtual void generate_def(QTextStream & f, QByteArray indent, bool h,
			      QByteArray templates, QByteArray cl_names,
			      QByteArray templates_tmplop, 
			      QByteArray cl_names_tmplop);
				         
    virtual void generate();
    void generate_decl(QTextStream &, QByteArray indent);
    
    void get_template_prefixes(QByteArray & template1, QByteArray & template2);
    void spec(QByteArray & templates, QByteArray & names,
	      QByteArray & templates_tmplop, QByteArray & names_tmplop);
    QByteArray decl();
    
    void write(QTextStream &, bool with_formals = TRUE,
	       BooL * is_template = 0,
	       const QValueList<UmlActualParameter> & actuals = noactuals);
    static void write(QTextStream &, const UmlTypeSpec &,
		      bool with_formals = TRUE,
		      BooL * is_template = 0);
};

#endif
