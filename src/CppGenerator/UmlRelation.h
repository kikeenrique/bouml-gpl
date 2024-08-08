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

#ifndef UMLRELATION_H
#define UMLRELATION_H

#include <qvaluelist.h>

#include "UmlBaseRelation.h"

class QTextStream;
class UmlActualParameter;

class UmlRelation : public UmlBaseRelation {
  public:
    UmlRelation(void * id, const QByteArray & n)
      : UmlBaseRelation(id, n) {
    };
  
    virtual void compute_dependency(QList<CppRefType> & dependency,
				    const QByteArray & cl_stereotype,
				    bool all_in_h);
    void generate_inherit(const char *& sep, QTextStream & f_h, 
			  const QValueList<UmlActualParameter> & actuals,
			  const QByteArray & cl_stereotype);
    virtual void generate_decl(aVisibility & current_visibility, QTextStream & f_h,
			       const QByteArray & cl_stereotype, QByteArray indent,
			       BooL & first, bool last);
    virtual void generate_def(QTextStream & f, QByteArray indent, bool h,
			      QByteArray templates, QByteArray cl_names,
			      QByteArray templates_tmplop, 
			      QByteArray cl_names_tmplop);
};

#endif
