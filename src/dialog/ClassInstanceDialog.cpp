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
#include <qcursor.h>

#include "ClassInstanceDialog.h"
#include "OdClassInstCanvas.h"
#include "BrowserClass.h"
#include "BrowserAttribute.h"
#include "BrowserView.h"
#include "MyTable.h"
#include "DialogUtil.h"
#include "UmlDesktop.h"

QSize ClassInstanceDialog::previous_size;

ClassInstanceDialog::ClassInstanceDialog(OdClassInstCanvas * i)
    : QDialog(0, "class instance dialog", TRUE), inst(i) {
  setCaption("Class instance dialog");
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  
  QHBoxLayout * hbox;
  
  vbox->setMargin(5);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  hbox->addWidget(new QLabel("name : ", this));
  edname = new LineEdit(inst->get_name(), this);
  hbox->addWidget(edname);
  edname->setFocus();
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  
  SmallPushButton *  b = new SmallPushButton("class :", this);
  
  connect(b, SIGNAL(clicked()), this, SLOT(menu_class()));
  hbox->addWidget(b);
  edtype = new QComboBox(FALSE, this);
  inst->get_types(nodes);
  nodes.full_names(list);
  edtype->insertStringList(list);
  edtype->setCurrentItem(nodes.find(inst->get_type()));
  connect(edtype, SIGNAL(activated(int)), this, SLOT(type_changed(int)));
  hbox->addWidget(edtype);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  hbox->addWidget(tbl = new MyTable(this));
  tbl->setNumCols(3);
  tbl->setSorting(FALSE);
  tbl->setSelectionMode(QTable::NoSelection);	// single does not work
  tbl->setRowMovingEnabled(TRUE);
  tbl->horizontalHeader()->setLabel(0, " Attribute ");
  tbl->horizontalHeader()->setLabel(1, " Class ");
  tbl->horizontalHeader()->setLabel(2, " Value ");
  
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
  
  type_changed(edtype->currentItem());
}

void ClassInstanceDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

ClassInstanceDialog::~ClassInstanceDialog() {
  previous_size = size();
}

void ClassInstanceDialog::menu_class() {
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

void ClassInstanceDialog::type_changed(int i) {
  // made attribute list
  BrowserClass * cl = (BrowserClass *) nodes.at(i);
  QList<BrowserClass> lcl;
  
  attributes.clear();
  cl->get_all_parents(lcl);
  lcl.append(cl);
  
  QListIterator<BrowserClass> it(lcl);
  
  for (; it.current(); ++it) {
    QListViewItem * child;
    
    for (child = it.current()->firstChild(); child; child = child->nextSibling())
      if (!((BrowserNode *) child)->deletedp() &&
	  (((BrowserNode *) child)->get_type() == UmlAttribute))
	attributes.append((BrowserNode *) child);
  }
  
  attributes.sort();
  
  // fill table
  tbl->setNumRows(0);
  tbl->setNumRows(attributes.count());
  
  BrowserNode * at;
  int index;
  
  for (at = attributes.first(), index = 0;
       at != 0;
       at = attributes.next(), index += 1) {
    tbl->setItem(index, 0, new TableItem(tbl, QTableItem::Never, at->get_name()));
    tbl->setItem(index, 1, new TableItem(tbl, QTableItem::Never, ((BrowserNode *) at->parent())->get_name()));
    
    int rank = inst->attributes.findRef((AttributeData *) at->get_data());
    
    if (rank != -1)
      tbl->setText(index, 2, *(inst->values.at(rank)));
    else
      tbl->setText(index, 2, QString::null);
  }
  
  tbl->setColumnStretchable (2, TRUE);
  tbl->adjustColumn(0);
  tbl->adjustColumn(1);
}

void ClassInstanceDialog::accept() {
  tbl->forceUpdateCells();
  
  inst->set_name(edname->text().stripWhiteSpace());
  inst->set_type((BrowserClass *) nodes.at(edtype->currentItem()));
  
  inst->attributes.clear();
  inst->values.clear();
  
  int n = tbl->numRows();
  int index;
  
  for (index = 0; index != n; index += 1) {
    QString s = tbl->text(index, 2).stripWhiteSpace();
    
    if (! s.isEmpty()) {
      inst->attributes.append((AttributeData *) attributes.at(index)->get_data());
      inst->values.append(s);
    }
  }
  
  QDialog::accept();
}
