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

#include <qintdict.h>

#include "UmlBaseOperation.h"

class QTextStream;

class UmlOperation : public UmlBaseOperation {
  private:
    static QIntDict<char> bodies;

  public:
    UmlOperation(void * id, const QByteArray & n)
      : UmlBaseOperation(id, n) {};
  
    virtual void generate(QTextStream & f, const QByteArray & cl_stereotype,
			  QByteArray indent);
    virtual void generate_enum_pattern_item(QTextStream &, int &,
					    const QByteArray &, QByteArray);
    virtual void generate_enum_pattern_case(QTextStream &, QByteArray);
    virtual void generate_enum_member(QTextStream &, QByteArray);
    
    QByteArray compute_name();
    
    static void read_bodies(const char * path);
    
  private:
    const char * generate_body(QTextStream & f, QByteArray indent, const char * p);
};

#endif
