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

#include <qmultilineedit.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qapplication.h>
 
#include "ShowFileDialog.h"
 
ShowFileDialog::ShowFileDialog(const QCString & filename)
    : QDialog(0, filename) {
  QVBoxLayout * vbox = new QVBoxLayout(this);
 
  vbox->addWidget(new QLabel("You can specify the editor through the environment variable BOUML_EDITOR",
                             this));
 
  e = new QMultiLineEdit(this);
  
  QFile f(filename);
  
  if (f.open(IO_ReadOnly)) {
    QTextStream t(&f);
    
    e->setText(t.read());
  }
 
  QFont font = e->font();
 
  font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  e->setFont(font);
 
  vbox->addWidget(e);
  
  resize(QApplication::desktop()->width() /2,
	 QApplication::desktop()->height() /2);
}

ShowFileDialog::~ShowFileDialog() {
} 
