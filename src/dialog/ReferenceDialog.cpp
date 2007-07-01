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

#include <qlabel.h>
#include <qlayout.h>
#include <qcombobox.h> 
#include <qpushbutton.h>

#include "ReferenceDialog.h"
#include "UmlDesktop.h"

void ReferenceDialog::show(BrowserNode * target)
{
  ReferenceDialog d(target);
  
  d.exec();
}

QSize ReferenceDialog::previous_size;

ReferenceDialog::ReferenceDialog(BrowserNode * target)
    : QDialog(0, "Referenced By dialog", TRUE) {
  setCaption("Referenced By dialog");
  
  QList<BrowserNode> l;
  BrowserNode * bn;
  
  target->referenced_by(l);
  for (bn = l.first(); bn; bn = l.next())
    nodes.append(bn);
  
  nodes.sort();
  
  // remove duplicats
  nodes.first();
  while ((bn = nodes.current()) != 0)
    if (bn == nodes.next())
      nodes.remove();
  
  QStringList names;
  
  nodes.full_names(names);
  
  QVBoxLayout * vbox = new QVBoxLayout(this);
  QHBoxLayout * hbox;
 
  vbox->setMargin(5);
 
  QString s = target->get_name();
  
  if (nodes.isEmpty())
    s += (nodes.isEmpty()) ? " is not referenced" : " is referenced by :";
  
  vbox->addWidget(new QLabel(s, this));
  
  QPushButton * cancel;
  
  if (!nodes.isEmpty()) {
    items = new QComboBox(FALSE, this);
    vbox->addWidget(items);
    
    QStringList::Iterator it;
    
    for (bn = nodes.first(), it = names.begin();
	 bn;
	 bn = nodes.next(), ++it)
      items->insertItem(*(bn->pixmap(0)), *it);
  
    hbox = new QHBoxLayout(vbox); 
    hbox->setMargin(5);
    QPushButton * select = new QPushButton("&Select", this);
    
    select->setDefault(TRUE);
    hbox->addWidget(select);
    connect(select, SIGNAL(clicked()), this, SLOT(select()));
  }
  else {
    hbox = new QHBoxLayout(vbox); 
    hbox->setMargin(5);
  }
  
  cancel = new QPushButton("&Close", this);
  hbox->addWidget(cancel);
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

ReferenceDialog::~ReferenceDialog() {
  previous_size = size();
}

void ReferenceDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

void ReferenceDialog::select() {
  nodes.at(items->currentItem())->select_in_browser();
}
