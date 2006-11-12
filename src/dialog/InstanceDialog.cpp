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

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include <qlayout.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qpushbutton.h>

#include "InstanceDialog.h"
#include "Instance.h"
#include "DialogUtil.h"
#include "UmlDesktop.h"

QSize InstanceDialog::previous_size;

InstanceDialog::InstanceDialog(Instance * i, const char * kind,
			       const char * caption)
    : QDialog(0, caption, TRUE), inst(i) {
  setCaption(caption);
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  
  QHBoxLayout * hbox;
  QLabel * lbl1;
  QLabel * lbl2;
  
  vbox->setMargin(5);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  hbox->addWidget(lbl1 = new QLabel("name : ", this));
  edname = new LineEdit(inst->get_name(), this);
  hbox->addWidget(edname);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  hbox->addWidget(lbl2 = new QLabel(kind, this));
  edtype = new QComboBox(FALSE, this);
  inst->get_types(nodes);
  nodes.full_names(list);
  edtype->insertStringList(list);
  edtype->setCurrentItem(nodes.find(inst->get_type()));
  hbox->addWidget(edtype);
  
  same_width(lbl1, lbl2);
  
  hbox = new QHBoxLayout(vbox); 
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

void InstanceDialog::accept() {
  inst->set_name(edname->text().stripWhiteSpace());
  inst->set_type(nodes.at(edtype->currentItem()));
  
  QDialog::accept();
}
