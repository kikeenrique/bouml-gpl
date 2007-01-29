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

#ifndef _UMLBASENCRELATION_H
#define _UMLBASENCRELATION_H


#include "UmlItem.h"

class UmlNcRelation;

// Manage the relations between non class objects, for instances
// dependency between packages and use case generalization
class UmlBaseNcRelation : public UmlItem {
  public:
    // returns a new non class relation of the given 'kind' from 'start' to 'end'
    //
    // In case it cannot be created ('parent' cannot contain it etc ...)
    // return 0 in C++ and produce a RuntimeException in Java
    static UmlNcRelation * create(aRelationKind kind, UmlItem * start, UmlItem * end);

    // returns the kind of the item
    virtual anItemKind kind();

    // returns the kind of the relation : aGeneralisation or a Dependency, the other are not allowed
    // for the non class relations
    aRelationKind relationKind();

    // returns the 'end' object (the 'start' object is the parent of the relation) no set !
    UmlItem * target();


  private:
    aRelationKind _rel_kind;

    UmlItem * _target;

    virtual void read_uml_();
    
  public:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseNcRelation(void * id, const QCString & n);

};

inline UmlBaseNcRelation::UmlBaseNcRelation(void * id, const QCString & n) : UmlItem(id, n) {
  _target = 0;
}

#endif
