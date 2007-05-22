// *************************************************************************cr();
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

#ifndef ACTIVITYDATA_H
#define ACTIVITIDATA_H

#include "SimpleData.h"
#include "InfoData.h"

class QTextStream;

class BrowserActivity;
class ActivityDialog;

class ActivityData : public SimpleData {
  friend class ActivityDialog;
    
  protected:
    InfoData uml_condition;
    InfoData cpp_condition;
    InfoData java_condition;
    bool read_only;
    bool single_execution;
    
    virtual void send_uml_def(ToolCom * com, BrowserNode * bn,
			      const QString & comment);
    virtual void send_cpp_def(ToolCom * com);
    virtual void send_java_def(ToolCom * com);
    
  public:
    ActivityData();
    ActivityData(ActivityData * model, BrowserNode * br);
    virtual ~ActivityData();
    
    QString get_precond(DrawingLanguage) const;
    QString get_postcond(DrawingLanguage) const;
    bool get_is_read_only() const { return read_only; }
    bool get_is_single_execution() const { return single_execution; }
    
    void edit();
    
    virtual bool tool_cmd(ToolCom * com, const char * args,
			  BrowserNode * bn, const QString & comment);
    
    void save(QTextStream &, QString & warning) const;
    void read(char * & st, char * & k);
};
  
#endif
