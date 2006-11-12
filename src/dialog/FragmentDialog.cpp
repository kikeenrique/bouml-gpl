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

#include "FragmentDialog.h"
#include "DialogUtil.h"
#include "UmlDesktop.h"

QSize FragmentDialog::previous_size;

FragmentDialog::FragmentDialog(const QStringList &defaults, QString & s)
    : QDialog(0, "Fragment dialog", TRUE), name(s) {
  setCaption("Fragment dialog");
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  
  QHBoxLayout * hbox;
  QLabel * label;
  
  vbox->setMargin(5);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  hbox->addWidget(label = new QLabel("name : ", this));
  cb = new QComboBox(TRUE, this);
  cb->insertItem(name);
  cb->setCurrentItem(0);
  cb->insertStringList(defaults);
  cb->setAutoCompletion(TRUE);
  hbox->addWidget(cb);
  
  QSizePolicy sp = cb->sizePolicy();
  
  sp.setHorData(QSizePolicy::Expanding);
  cb->setSizePolicy(sp);
  
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

void FragmentDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

FragmentDialog::~FragmentDialog() {
  previous_size = size();
}

void FragmentDialog::accept() {
  name = cb->currentText().stripWhiteSpace();

  QDialog::accept();
}
