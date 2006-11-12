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
#include <qpushbutton.h>

#include "ObjectLinkDialog.h"
#include "MyTable.h"
#include "BrowserClass.h"
#include "RelationData.h"
#include "UmlDesktop.h"

QSize ObjectLinkDialog::previous_size;

ObjectLinkDialog::ObjectLinkDialog(QList<RelationData> & l,
				   RelationData * current)
    : QDialog(0, "object link dialog", TRUE), rels(l), choozen(0) {
  setCaption("Object link dialog");
  
  QVBoxLayout * vbox = new QVBoxLayout(this);
  QHBoxLayout * hbox;
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  
  hbox->addWidget(new QLabel("\n"
			     "To set the association select one clicking on the first column then press 'Ok'\n"
			     "To unset the association press 'Unset' then press 'Ok'\n",
			     this));
  
  init(current);	// update table
  
  vbox->addWidget(table);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  QPushButton * unset = new QPushButton("&Unset", this);
  QPushButton * accept = new QPushButton("&OK", this);
  QPushButton * cancel = new QPushButton("&Cancel", this);
  QSize bs(cancel->sizeHint());
  
  unset->setFixedSize(bs);
  accept->setFixedSize(bs);
  cancel->setFixedSize(bs);
  
  hbox->addWidget(unset);
  hbox->addWidget(accept);
  hbox->addWidget(cancel);
    
  connect(unset, SIGNAL(clicked()), this, SLOT(unselect()));
  connect(accept, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

ObjectLinkDialog::~ObjectLinkDialog() {
  previous_size = size();
}

void ObjectLinkDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

void ObjectLinkDialog::init(RelationData * current) {
  table = new MyTable(this);
  
  table->setNumRows(rels.count());
  table->setNumCols(5);
  table->setSelectionMode(QTable::Single);
  table->setSorting(FALSE);
  
  table->horizontalHeader()->setLabel(0, "Name");
  table->horizontalHeader()->setLabel(1, "Class A");
  table->horizontalHeader()->setLabel(2, "Role A");
  table->horizontalHeader()->setLabel(3, "Class B");
  table->horizontalHeader()->setLabel(4, "Role B");
  
  QListIterator<RelationData> iter(rels);
  int row;
  
  for (row = 0; iter.current(); ++iter, row += 1) {
    RelationData * d = *iter;
    const char * name = d->get_name();
    
    if ((name != 0) && (*name != 0) && (*name != '<'))
      table->setText(row, 0, name);    
    table->setItem(row, 1, new TableItem(table, QTableItem::Never, d->get_start_class()->full_name(TRUE)));    
    table->setItem(row, 2, new TableItem(table, QTableItem::Never, d->get_role_a()));
    table->setItem(row, 3, new TableItem(table, QTableItem::Never, d->get_end_class()->full_name(TRUE)));
    table->setItem(row, 4, new TableItem(table, QTableItem::Never, d->get_role_b()));
  }
    
  table->setColumnStretchable (1, TRUE);
  table->setColumnStretchable (3, TRUE);
  table->adjustColumn(0);
  table->adjustColumn(2);
  table->adjustColumn(4);
  
  if (current != 0) {
    // select the current relation
    QTableSelection sel;
    int row = rels.findRef(current);
    
    sel.init(row, 0);
    sel.expandTo(row, 4);
    table->addSelection(sel);
  }
}

void ObjectLinkDialog::unselect() {
  while (table->currentSelection() != -1)
    table->removeSelection(0);
}

void ObjectLinkDialog::accept() {
  if (table->currentSelection() == -1)
    choozen = 0;
  else {
    QTableSelection sel = table->selection(0);
    
    choozen = rels.at(sel.topRow());
  }
  
  QDialog::accept();
}
