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

#include <stdio.h>
#include <qpopupmenu.h> 

#include "KeyValueTable.h"
#include "HaveKeyValueData.h"
#include "KeyValueData.h"
#include "MLinesItem.h"
#include "strutil.h"

KeyValuesTable::KeyValuesTable(HaveKeyValueData * hv, QWidget * parent, bool visit)
    : StringTable(((hv == 0) ? 0 : hv->get_n_keys()) + ((visit) ? 0 : 1),
		  (visit) ? 2 : 3, parent, visit) {
  horizontalHeader()->setLabel(0, "Key");
  horizontalHeader()->setLabel(1, "Value");
  if (! visit)
    horizontalHeader()->setLabel(2, "do");
  
  int index;
  int sup = (hv == 0) ? 0 : hv->get_n_keys();
  
  if (visit) {
    for (index = 0; index < sup; index += 1) {
      TableItem * ti;
      
      ti = new TableItem(this, QTableItem::Never,
			 toUnicode(hv->get_key(index)));
      ti->setReplaceable(FALSE);
      setItem(index, 0, ti);
      
      ti = new TableItem(this, QTableItem::Never,
			 toUnicode(hv->get_value(index)));
      ti->setReplaceable(FALSE);
      setItem(index, 1, ti);
    }
  }
  else {
    for (index = 0; index < sup; index += 1){
      setText(index, 0, toUnicode(hv->get_key(index)));
      setItem(index, 1, new MLinesItem(this, toUnicode(hv->get_value(index))));
      setText(index, 2, QString::null);
      setRowStretchable(index, TRUE);
    }
    
    setText(index, 0, QString::null);
    setItem(index, 1, new MLinesItem(this, QString::null));
    setText(index, 2, QString::null);
    setRowStretchable(index, TRUE);
  }
  
  horizontalHeader()->setResizeEnabled(TRUE, 0);  //setColumnStretchable (0, TRUE);
  setColumnStretchable (1, TRUE);
  if (! visit) {
    adjustColumn(2);
    setColumnStretchable (2, FALSE);
  }
}

void KeyValuesTable::init_row(int index) {
  setText(index, 0, QString::null);
  setItem(index, 1, new MLinesItem(this, QString::null));
  setText(index, 2, QString::null);
  setRowStretchable(index, TRUE);
}

void KeyValuesTable::update(HaveKeyValueData * oper) {
  forceUpdateCells();
  
  unsigned n = numRows();
  unsigned index;
  
  if (text(n - 1, 0).isEmpty())
    n -= 1;
  
  oper->set_n_keys(n);
  
  for (index = 0; index != n; index += 1) {
    oper->set_key(index, fromUnicode(text(index, 0)));
    oper->set_value(index, fromUnicode(text(index, 1)));
  }
}

bool KeyValuesTable::get_value(const char * key, QString & value) {
  int index;
  
  for (index = 0; index != numRows(); index += 1) {
    if (text(index, 0) == toUnicode(key)) {
      value = text(index, 1);
      return TRUE;
    }
  }
  
  return FALSE;
}
