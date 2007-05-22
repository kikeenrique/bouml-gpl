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

#ifndef ATYPE_H
#define ATYPE_H

#include "mystr.h"

class QTextStream;
class BrowserClass;
class ToolCom;

class AType {
  public:
    SharedStr explicit_type;
    BrowserClass * type;
    
  public:
    AType() : type(0) {};
    const char * get_type() const;
    QString get_full_type() const;
    
    void send_def(ToolCom * com) const;
    void save(QTextStream & st, QString & warning,
	      const char * t, const char * ex) const;
    void read(char * & st, const char * t, const char * ex);
    void read(char * & st, const char * t, const char * ex, const char * k);
};

#endif

