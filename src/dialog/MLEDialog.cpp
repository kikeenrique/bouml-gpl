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

#include "MLEDialog.h"
#include "DialogUtil.h"

MLEDialog::MLEDialog(QString t) : QDialog(0, "Uml", TRUE, 0) {
  QVBoxLayout *vbox = new QVBoxLayout(this);

  e = new MultiLineEdit(this);
  vbox->addWidget(e);
  
  e->setText(t);
}

MLEDialog::~MLEDialog() {
}

void MLEDialog::get(QString & t, const QPoint & p)
{
  MLEDialog d(t);
  
  d.move(p);
  d.exec();
  t = d.e->text();
}

QString MLEDialog::text() {
  return e->text();
}

