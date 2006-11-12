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

#ifndef ACTUALPARAMDATA_H
#define ACTUALPARAMDATA_H

#include "AType.h"

class ToolCom;

class ActualParamData {
  protected:
    BrowserClass * super;
    unsigned rank;
    AType value;
    
  public:
    ActualParamData(BrowserClass * p, unsigned r);
  
    BrowserClass * get_class() const { return super; };
    
    QString get_name() const;
    
    const AType & get_value() const { return value; };
    void set_value(const AType & t);
    
    void send_uml_def(ToolCom * com);
    
    void save(QTextStream &, QString & warning, const QString & cl_name) const;
    static ActualParamData * read(char * &);
};


#endif
