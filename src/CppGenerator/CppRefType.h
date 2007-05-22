// *************************************************************************
//
// Copyleft 2004-2007 Bruno PAGES  .
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

#include <qlist.h>

#include "UmlTypeSpec.h"

class QTextOStream;
class UmlArtifact;

class CppRefType {
  protected:
    UmlTypeSpec type;
    bool included;
    
    CppRefType(UmlClass * cl, bool i) : included(i) { type.type = cl; };
    CppRefType(const QCString & t, bool i) : included(i) { type.explicit_type = t; };
    
  public:
    static bool add(UmlClass *, QList<CppRefType> &, bool incl);
    static bool add(const QCString &, QList<CppRefType> &, bool incl);
    static bool add(const UmlTypeSpec & t, QList<CppRefType> & l, bool incl);
    static void remove(UmlClass *, QList<CppRefType> & l);
    static void remove(const QCString &, QList<CppRefType> & l);
    static void force_ref(UmlClass * cl, QList<CppRefType> & l);
    static void compute(QList<CppRefType> & dependencies,
			const QCString & hdef, const QCString & srcdef,
			QCString & h_incl,  QCString & decl, QCString & src_incl,
			UmlArtifact * who);
};

#endif
