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

#ifndef UMLOPERATION_H
#define UMLOPERATION_H

#include "UmlBaseOperation.h"
#include "UmlClass.h"
#include "Namespace.h"

class ClassContainer;
class Package;
class Class;

class UmlOperation : public UmlBaseOperation {
  public:
    UmlOperation(void * id, const QCString & n);
    virtual ~UmlOperation();
  
    static bool new_one(Class * cl, const QCString & name,
			const QValueList<FormalParameterList> & tmplt,
			QCString type, const QCString & modifier,
			const QCString & pretype, aVisibility visibility,
			bool inlinep, bool virtualp, bool staticp, bool constp,
			bool volatilep, bool typenamep,	bool explicitp,
			bool friendp, QCString friend_template,
			QCString comment, QCString description, bool pfunc);
    static void reverse_definition(Package * pack, QCString name, QCString type,
				   QValueList<FormalParameterList> & tmplt,
				   bool inlinep, const QCString & comment,
				   const QCString & description);

#ifdef REVERSE
    virtual bool need_source();
#endif
  
    static void skip_body(int level = 0);
    static void skip_expr(QCString end);
    static bool pfunc(bool & func, QCString & name, QCString & type,
		      const char * namespec);
  private:
    static NDict< QList<UmlOperation> > friends;
    FormalParameterList * formals;
    QCString def0;	// for template operations
  
    static bool read_param(ClassContainer * container, unsigned rank, 
			   UmlParameter & param, QCString & decl,
			   const QValueList<FormalParameterList> & tmplt,
			   bool & on_error);
    static void friend_operations(QList<UmlOperation> & candidates,
				  const QValueList<FormalParameterList> & tmplt,
				  const QCString & name);
    static bool operations(QList<UmlOperation> & candidates, UmlClass * cl,
			   const QValueList<FormalParameterList> & tmplt,
			   const FormalParameterList *& oper_tmplt,
			   const QCString & name);
    static bool read_throw_elt(ClassContainer * container,
			       UmlTypeSpec & typespec,
			       const QValueList<FormalParameterList> & tmplts);
    bool reverse_if_def(Package * pack,
			QValueList<FormalParameterList> & tmplts,
			const FormalParameterList * oper_tmplt,
			bool inlinep, bool pfct, const QCString & comment,
			const QCString & description, bool & on_error,
			unsigned & nargs, QCString oper_name);
};

#endif
