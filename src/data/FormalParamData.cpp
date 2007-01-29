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

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include "BrowserClass.h"
#include "FormalParamData.h"
#include "myio.h"
#include "ToolCom.h"

void FormalParamData::set_default_value(BrowserClass * t, const QString & e) {
  default_value.type = t;
  default_value.explicit_type = e;
}

void FormalParamData::set_extends(BrowserClass * t, const QString & e) {
  extends.type = t;
  extends.explicit_type = e;
}

void FormalParamData::send_uml_def(ToolCom * com) {
  com->write_string(name);
  com->write_string(type);
  default_value.send_def(com);
  if (com->api_format() >= 20)
    extends.send_def(com);
}
      
void FormalParamData::read(ToolCom * com, const char *& args) {
  name = com->get_string(args);
  type = com->get_string(args);
  com->get_type(default_value, args);
  if (com->api_format() >= 20)
    com->get_type(extends, args);
}

void FormalParamData::skip(ToolCom * com, const char *& args)
{
  com->get_string(args);
  com->get_string(args);
  com->skip_type(args);
  if (com->api_format() >= 20)
    com->skip_type(args);
}

void FormalParamData::save(QTextStream & st, QString & warning,
			   const QString & cl_name) const {
  nl_indent(st);
  st << "formal name ";
  save_string(name, st);
  st << " type ";
  save_string(type, st);
  if (!default_value.save(st, warning, " default_value ", " explicit_default_value "))
    warning += QString("<p><b>") + cl_name + "</b>'s formal <b>" + name +
      "</b> default values the deleted class <b>" +
	default_value.type->full_name() + "</b>\n";
  nl_indent(st);	// default_value may produce a comment
  if (!extends.save(st, warning, "  extends ", "  explicit_extends "))
    warning += QString("<p><b>") + cl_name + "</b>'s formal <b>" + name +
      "</b> extends values the deleted class <b>" +
	extends.type->full_name() + "</b>\n";
}

void FormalParamData::read(char * & st) {
  read_keyword(st, "formal");
  read_keyword(st, "name");
  name = read_string(st);
  read_keyword(st, "type");
  type = read_string(st);
  default_value.read(st, "default_value", "explicit_default_value");
  if (read_file_format() >= 24)
    extends.read(st, "extends", "explicit_extends");
}
