// *************************************************************************
//
// Copyright 2004-2009 Bruno PAGES  .
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
#include "UmlTypeSpec.h"

class Class;
class UmlClass;

class UmlRelation : public UmlBaseRelation {
  public:
    UmlRelation(void * id, const QCString & n)
      : UmlBaseRelation(id, n) {
    };  
  
    static bool new_one(Class * container, const QCString & name,
			UmlTypeSpec & dest, QCString str_actuals,
			aVisibility visibility,	bool staticp,
			bool constp, bool transientp, bool volatilep,
			const QCString & array,	const QCString & value,
			QCString comment, QCString description,
			QCString annotation
#ifdef ROUNDTRIP
			, bool roundtrip, QList<UmlItem> & expected_order
#endif
			);
    static bool new_one(Class * container, const QCString & name,
			UmlClass * type, QCString type_def,
			QCString genericname,
			aVisibility visibility, bool staticp,
			bool constp, bool transientp, bool volatilep,
			const QCString & array, const QCString & value,
			QCString comment, QCString description,
			QCString annotation
#ifdef ROUNDTRIP
			, bool roundtrip, QList<UmlItem> & expected_order
#endif
			);
#ifdef ROUNDTRIP
    void set_unidir();
#endif
};

#endif
