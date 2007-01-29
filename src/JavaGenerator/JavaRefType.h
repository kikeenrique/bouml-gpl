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

#ifndef CPPREFTYPE_H
#define CPPREFTYPE_H

/*
#include <qlist.h>

#include "UmlTypeSpec.h"

class QTextOStream;
class UmlArtifact;

class JavaRefType {
  protected:
    UmlTypeSpec type;
    
    JavaRefType(UmlClass * cl) { type.type = cl; };
    JavaRefType(const QCString & t) { type.explicit_type = t; };
    
  public:
    static bool add(UmlClass *, QList<JavaRefType> &);
    static bool add(const QCString &, QList<JavaRefType> &);
    static bool add(const UmlTypeSpec & t, QList<JavaRefType> & l);
    static void remove(UmlClass *, QList<JavaRefType> & l);
    static void remove(const QCString &, QList<JavaRefType> & l);
    static void compute(QList<JavaRefType> & dependencies,
			const QCString & filedef, QCString & incl,
			UmlArtifact * who);
};
*/

#endif
