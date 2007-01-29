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

#include <qlayout.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qpushbutton.h>
#include <qpopupmenu.h>
#include <qgrid.h>
#include <qcursor.h>

#include "InstanceDialog.h"
#include "Instance.h"
#include "DialogUtil.h"
#include "UmlDesktop.h"
#include "BrowserView.h"

QSize InstanceDialog::previous_size;

InstanceDialog::InstanceDialog(Instance * i, const char * kind,
			       const char * caption)
    : QDialog(0, caption, TRUE), inst(i) {
  setCaption(caption);
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  
  
  vbox->setMargin(5);
  
  QGrid * grid = new QGrid(2, this);
  
  vbox->addWidget(grid);
  new QLabel("name : ", grid);
  edname = new LineEdit(inst->get_name(), grid);
  
  new QLabel("", grid);
  new QLabel("", grid);
  
  SmallPushButton * b = new SmallPushButton(kind, grid);
  
  connect(b, SIGNAL(clicked()), this, SLOT(menu_type()));
  
  edtype = new QComboBox(FALSE, grid);
  inst->get_types(nodes);
  nodes.full_names(list);
  edtype->insertStringList(list);
  edtype->setCurrentItem(nodes.find(inst->get_type()));
  
  new QLabel("", grid);
  new QLabel("", grid);
  
  QHBoxLayout * hbox = new QHBoxLayout(vbox); 
  
  hbox->setMargin(5);
  QPushButton * accept = new QPushButton("&OK", this);
  QPushButton * cancel = new QPushButton("&Cancel", this);
  QSize bs(cancel->sizeHint());
  
  accept->setDefault(TRUE);
  accept->setFixedSize(bs);
  cancel->setFixedSize(bs);
  
  hbox->addWidget(accept);
  hbox->addWidget(cancel);
    
  connect(accept, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void InstanceDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

InstanceDialog::~InstanceDialog() {
  previous_size = size();
}

void InstanceDialog::menu_type() {
  QPopupMenu m(0);

  m.insertItem("Choose", -1);
  m.insertSeparator();
  
  int index = list.findIndex(edtype->currentText().stripWhiteSpace());
  
  if (index != -1)
    m.insertItem("Select in browser", 0);
  
  BrowserNode * bn = BrowserView::selected_item();
    
  if ((bn->get_type() == UmlClass) && !bn->deletedp())
    m.insertItem("Choose class selected in browser", 1);
  else
    bn = 0;
  
  if ((index != -1) || (bn != 0)) {
    switch (m.exec(QCursor::pos())) {
    case 0:
      nodes.at(index)->select_in_browser();
      break;
    case 1:
      edtype->setCurrentItem(list.findIndex(bn->full_name(TRUE)));
    }
  }
}

void InstanceDialog::accept() {
  inst->set_name(edname->text().stripWhiteSpace());
  inst->set_type(nodes.at(edtype->currentItem()));
  
  QDialog::accept();
}
