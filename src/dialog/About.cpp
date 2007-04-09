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
#include <qpushbutton.h>

#include "About.h"
#include "bp_xpm.xpm"
#include "UmlDesktop.h"

AboutDialog::AboutDialog() : QDialog(0, "About BOUML", TRUE) {
  setCaption("About BOUML");
  //move(p);
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  
  QHBoxLayout * hbox; 
  
  vbox->setMargin(5);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);

  QPixmap bp((const char **) bp_xpm);
  QLabel * lbp = new QLabel(this);
  
  lbp->setPixmap(bp);
  hbox->addWidget(lbp);
  hbox->addWidget(new QLabel("  ", this));
  hbox->addWidget(new QLabel("BOUML release 2.23.1\n\ne-mail : bouml@free.fr\nhome : http://bouml.free.fr",
			     this));
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  QPushButton * ok = new QPushButton("&OK", this);
  
  ok->setDefault( TRUE );
  
  hbox->addWidget(new QLabel(this));
  hbox->addWidget(ok);
  hbox->addWidget(new QLabel(this));
  
  if (UmlDesktop::fixed())
    UmlDesktop::tocenter(this);
  
  connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
}

