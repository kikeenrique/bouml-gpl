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

#include "CodAddMsgDialog.h"
#include "ColDiagramView.h"
#include "CodMsgSupport.h"
#include "CodObjCanvas.h"
#include "ColMsg.h"
#include "BrowserClass.h"
#include "OperationData.h"
#include "DialogUtil.h"
#include "UmlDesktop.h"

QSize CodAddMsgDialog::previous_size;

CodAddMsgDialog::CodAddMsgDialog(CodObjCanvas * from, CodObjCanvas * to,
				 CodMsgSupport * i, ColDiagramView * v,
				 bool fo)
    : QDialog(0, "add msg dialog", TRUE), in(i), view(v), forward(fo) {
  setCaption("Add message dialog");
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  
  QHBoxLayout * hbox;
  QLabel * label1;
  QLabel * label2;
  
  vbox->setMargin(5);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(10);
  label1 = new QLabel(QString("Add message to ") + to->get_full_name(), this);
  label1->setAlignment(AlignCenter);
  hbox->addWidget(label1);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  hbox->addWidget(label1 = new QLabel("rank : ", this));
  cbrank = new QComboBox(FALSE, this);
  
  ColMsgList all_in;
  ColMsgList all_out;
  
  from->get_all_in_all_out(all_in, all_out);
  
  ColMsg * m;
  QStringList new_ones;
  QListIterator<ColMsg> itout(all_out);
  
  for (; (m = itout.current()) != 0; ++itout) {
    QString s = m->next_hierarchical_rank();
      
    if ((s.find('.') != - 1) && (ColMsg::find(s, all_out) == 0)) {
      cbrank->insertItem(QString::number(m->get_rank() + 1) + " : " + s);
      new_ones.append(s);
    }
  }
    
  QListIterator<ColMsg> itin(all_in);
  
  for (; (m = itin.current()) != 0; ++itin) {
    QString s = m->get_hierarchical_rank() + ".1";
    
    if ((ColMsg::find(s, all_out) == 0) && (new_ones.findIndex(s) == -1)) {
      cbrank->insertItem(QString::number(m->get_rank() + 1) + " : " + s);
      new_ones.append(s);
    }
  }

  // add a the rank for a new toplevel link
  cbrank->insertItem(QString::number(ColMsg::last_rank(view->get_msgs()) + 1)
		     + " : "
		     + QString::number(view->get_msgs().count() + 1));
  
  // the default new one follow the last input or output or view's last msg
  cbrank->setCurrentItem((cbrank->count() == 1) ? 0 : cbrank->count() - 2);
  
  QSizePolicy sp = cbrank->sizePolicy();
  
  sp.setHorData(QSizePolicy::Expanding);
  cbrank->setSizePolicy(sp);
  
  hbox->addWidget(cbrank);
  
  // the operations
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  hbox->addWidget(label2 = new QLabel("message : ", this));
  same_width(label1, label2);
  edoper = new QComboBox(TRUE, this);
  edoper->setAutoCompletion(TRUE);
  
  if (to->get_type() != 0) {
    to->get_type()->get_opers(opers, list);
    edoper->insertStringList(list);
  }
    
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

void CodAddMsgDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

CodAddMsgDialog::~CodAddMsgDialog() {
  previous_size = size();
}

void CodAddMsgDialog::accept() {
  QString s = edoper->currentText().stripWhiteSpace();
  
  if (s.isEmpty())
    return;
  
  int index = list.findIndex(s);
  const OperationData * d;
  QString e;
  
  if (index >= 0)
    d = opers[index];
  else {
    d = 0;
    e = s;
  }

  s = cbrank->currentText();
  
  ColMsg::new_one(d, e, forward, s.mid(s.find(':') + 2), in)
    ->place_in(view->get_msgs());
  
  view->update_msg_supports();
  
  QDialog::accept();
}
