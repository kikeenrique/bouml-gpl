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

#include <qtextview.h> 
#include <qlayout.h>
#include <qpushbutton.h>

#include "TraceDialog.h"
#include "UmlDesktop.h"

TraceDialog * TraceDialog::the;
QTextView * TraceDialog::txt;

QSize TraceDialog::previous_size;

TraceDialog::TraceDialog() : QDialog(0, "", FALSE) {
  setCaption("Trace");
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  

  txt = new QTextView(this);
  vbox->add(txt);
  
  QHBoxLayout * hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  QPushButton * cl = new QPushButton("Clear", this);
  QPushButton * close = new QPushButton("Close", this);
  QSize bs(cl->sizeHint());
  
  close->setDefault(TRUE);
  close->setFixedSize(bs);
  cl->setFixedSize(bs);
  
  hbox->addWidget(close);
  hbox->addWidget(cl);
  
  connect(close, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cl, SIGNAL(clicked()), this, SLOT(clr()));
  
  // not done in polish else the initial size is too small
  UmlDesktop::setsize_center(this, previous_size, 0.5, 0.5);
}

TraceDialog::~TraceDialog() {
  delete txt;
  txt = 0;
  previous_size = size();
}

void TraceDialog::clr() {
  if (txt)
    txt->setText("");
}

void TraceDialog::clear()
{
  if (txt)
    txt->setText("");
}

void TraceDialog::add(const char * s) {
  if (txt == 0)
    the = new TraceDialog();
  the->show();
  txt->append(s);
  txt->update();
}

void TraceDialog::show_it()
{
  if (the)
    the->hide();
  add("");
}
