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
#include "ClassData.h"
#include "ActualParamData.h"
#include "myio.h"
#include "ToolCom.h"

ActualParamData::ActualParamData(BrowserClass * p, unsigned r)
    : super(p), rank(r) {
}
  
QString ActualParamData::get_name() const {
  return super->full_name() + " : " +
    ((ClassData *) super->get_data())->get_formalparam_name(rank);
}

void ActualParamData::set_value(const AType & t) {
  value = t;
}

void ActualParamData::send_uml_def(ToolCom * com) {
  super->write_id(com);
  com->write_unsigned(rank);
  value.send_def(com);
}

void ActualParamData::save(QTextStream & st, QString & warning,
			   const QString & cl_name) const {
  nl_indent(st);
  st << "actual class ";
  super->save(st, TRUE, warning);
  nl_indent(st);
  st << "  rank " << rank << ' ';
  if (! value.save(st, warning, "value ", "explicit_value "))
    warning += QString("<p><b>") + cl_name + "</b>'s actual for <b>" +
      ((ClassData *) super->get_data())->get_formalparam_name(rank) +
	"<b> values the deleted class <b>" + value.type->full_name() + "</b>\n";
}

ActualParamData * ActualParamData::read(char * & st)
{
  read_keyword(st, "actual");
  read_keyword(st, "class");
  
  BrowserClass * p = BrowserClass::read_ref(st);
  
  read_keyword(st, "rank");
  
  ActualParamData * result = new ActualParamData(p, read_unsigned(st));
  
  result->value.read(st, "value", "explicit_value");
  
  return result;
}
