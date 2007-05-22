// *************************************************************************
//
// Copyleft 2004-2007 Bruno PAGES  .
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

#include <qcursor.h>
#include <qlayout.h>
#include <qcombobox.h> 
#include <qpushbutton.h>

#include "ClassListDialog.h"
#include "BrowserClass.h"
#include "BasicData.h"
#include "UmlDesktop.h"

QSize ClassListDialog::previous_size;

ClassListDialog::ClassListDialog(const char * m,
				 const QValueList<BrowserClass *> & l)
    : QDialog(0, m, TRUE) {
  setCaption(m);
  move(QCursor::pos());
 
  QVBoxLayout * vbox = new QVBoxLayout(this);
  QHBoxLayout * hbox;
 
  vbox->setMargin(5);
 
  cb = new QComboBox(FALSE, this);
  vbox->addWidget(cb);
  
  QValueList<BrowserClass *>::ConstIterator end = l.end();
  QValueList<BrowserClass *>::ConstIterator it;
  
    for (it = l.begin(); it != end; ++it)
      if (!(*it)->deletedp())
	cb->insertItem((*it)->full_name(TRUE));
  
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
  
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

ClassListDialog::~ClassListDialog() {
  previous_size = size();
}

int ClassListDialog::choosen() {
  return cb->currentItem();
}
