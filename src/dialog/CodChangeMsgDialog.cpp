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
#include <qhbox.h>
#include <qpushbutton.h>

#include "CodChangeMsgDialog.h"
#include "ColMsg.h"
#include "CodMsgSupport.h"
#include "CodObjCanvas.h"
#include "BrowserClass.h"
#include "OperationData.h"
#include "UmlDesktop.h"
//#include "DialogUtil.h"

QSize CodChangeMsgDialog::previous_size;

CodChangeMsgDialog::CodChangeMsgDialog(ColMsg * m)
    : QDialog(0, "Collaboration message dialog", TRUE), msg(m) {
  setCaption("Collaboration message dialog");
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  
  QHBoxLayout * hbox;
  
  vbox->setMargin(5);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  hbox->addWidget(new QLabel("message : ", this));
  edoper = new QComboBox(TRUE, this);
  edoper->setAutoCompletion(TRUE);
  if (msg->operation == 0)
    edoper->insertItem(msg->explicit_operation);
  else
    edoper->insertItem(msg->operation->definition(TRUE));
  
  CodObjCanvas * from;
  CodObjCanvas * to;
    
  msg->in->get_from_to(from, to, msg->is_forward);
  
  if (to->get_type() != 0) {
    to->get_type()->get_opers(opers, list);
    edoper->insertStringList(list);
  }
  edoper->setCurrentItem(0);
    
  QSizePolicy sp = edoper->sizePolicy();
  
  sp.setHorData(QSizePolicy::Expanding);
  edoper->setSizePolicy(sp);
  
  hbox->addWidget(edoper);
  
  // ok & cancel
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  QPushButton * ok = new QPushButton("&OK", this);
  QPushButton * cancel = new QPushButton("&Cancel", this);
  QSize bs(cancel->sizeHint());
  
  ok->setDefault(TRUE);
  ok->setFixedSize(bs);
  cancel->setFixedSize(bs);
  
  hbox->addWidget(ok);
  hbox->addWidget(cancel);
    
  connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void CodChangeMsgDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

CodChangeMsgDialog::~CodChangeMsgDialog() {
  previous_size = size();
}


void CodChangeMsgDialog::accept() {
  QString s = edoper->currentText().stripWhiteSpace();
  
  if (s.isEmpty())
    return;
  
  int index = list.findIndex(s);
  
  if (index >= 0) {
    msg->operation = opers[index];
    msg->explicit_operation = QString::null;
  }
  else {
    msg->operation = 0;
    msg->explicit_operation = s;
  }
  
  QDialog::accept();
}
