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
    UmlOperation(void * id, const QByteArray & n);
    virtual ~UmlOperation();
  
    static bool new_one(Class * cl, const QByteArray & name,
			const QValueList<FormalParameterList> & tmplt,
			QByteArray type, const QByteArray & modifier,
			const QByteArray & pretype, aVisibility visibility,
			bool inlinep, bool virtualp, bool staticp, bool constp,
			bool volatilep, bool typenamep,	bool explicitp,
			bool friendp, QByteArray friend_template,
			QByteArray comment, QByteArray description, bool pfunc
#ifdef ROUNDTRIP
			, bool roundtrip, QList<UmlItem> & expected_order
#endif
			);
    static void reverse_definition(Package * pack, QByteArray name, QByteArray type,
				   QValueList<FormalParameterList> & tmplt,
				   bool inlinep, const QByteArray & comment,
				   const QByteArray & description);

#ifdef REVERSE
    virtual bool need_source();
#ifdef ROUNDTRIP
    static void force_defs();
# endif
#endif
  
    static void skip_body(int level = 0);
    static void skip_expr(QByteArray end, bool allow_templ = FALSE);
    static bool pfunc(BooL & func, QByteArray & name, QByteArray & type,
		      const char * namespec);
  private:
#ifdef ROUNDTRIP
    static QPtrDict<QByteArray> DefNotYetSet;
#endif
    static NDict< QList<UmlOperation> > friends;
    FormalParameterList * formals;
    QByteArray def0;	// for template operations
  
    static bool read_param(ClassContainer * container, unsigned rank, 
			   UmlParameter & param, QByteArray & decl,
			   const QValueList<FormalParameterList> & tmplt,
			   BooL & on_error, bool add_defaultvalue);
    static void friend_operations(QList<UmlOperation> & candidates,
				  const QValueList<FormalParameterList> & tmplt,
				  const QByteArray & name);
    static bool operations(QList<UmlOperation> & candidates, UmlClass * cl,
			   const QValueList<FormalParameterList> & tmplt,
			   const FormalParameterList *& oper_tmplt,
			   const QByteArray & name);
    static bool read_throw_elt(ClassContainer * container,
			       UmlTypeSpec & typespec,
			       const QValueList<FormalParameterList> & tmplts);
    bool reverse_if_def(Package * pack,
			QValueList<FormalParameterList> & tmplts,
			const FormalParameterList * oper_tmplt,
			bool inlinep, bool pfct, const QByteArray & comment,
			const QByteArray & description, BooL & on_error,
			unsigned & nargs, QByteArray oper_name);

    void update_param_names(QValueList<UmlParameter> & params);
#ifdef ROUNDTRIP
    void update_params(Class * cl, QValueList<UmlParameter> & params);
    void update_exceptions(Class * cl, const QValueList<UmlTypeSpec> & exceptions);
    
    static void clean_body(QByteArray & body);
    static UmlOperation * already_exist(Class * container, const QByteArray & name,
					QValueList<UmlParameter> & params,
					bool empty_decl);
#endif
};

#endif
