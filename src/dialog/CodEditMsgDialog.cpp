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

#include <qdialog.h>
#include <qlayout.h>

#include "CodEditMsgDialog.h"
#include "ColDiagramView.h"
#include "ColMsgTable.h"
#include "UmlDesktop.h"

void CodEditMsgDialog::exec(ColDiagramView * view, ColMsgList & msgs)
{
  QDialog d(0, "Collaboration diagram message dialog", TRUE, 0);
  
  d.setCaption("Collaboration diagram message dialog");
  
  QVBoxLayout vbox(&d);
  
  vbox.addWidget(new ColMsgTable(&d, view, msgs));
  
  static QSize previous_size;
  
  UmlDesktop::limitsize_center(&d, previous_size, 0.8, 0.8);
  
  d.exec();
  
  previous_size = d.size();
}

