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

#include <qapplication.h>

#include "UmlWindow.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "UmlDesktop.h"
#include "ToolCom.h"
#include "BrowserView.h"
#include "BrowserPackage.h"
#include "Shortcut.h"
#include "strutil.h"
#include "DialogUtil.h"
#include "mu.h"

QApplication * theApp;

int main(int argc, char **argv)
{
  theApp = new QApplication (argc, argv);
  
  initCodec();
  init_pixmaps();
  init_font();
  Shortcut::init();
  UmlDesktop::init();
  
  UmlWindow * uw = new UmlWindow();
  
  uw->show();
  
  if (argc > 1) {
    try {
      if ((argc == 3) &&
	  !strcmp(argv[2], "-root") &&
	  (msg_critical("DO NOT CONFIRM",
			"Root mode protection\n\n"
			"This mode allows me to develop BOUML\n\n"
			"do NOT confirm to avoid a disaster !!!\n\n"
			"confirm ?",
			QMessageBox::Yes, QMessageBox::No)
	   == QMessageBox::Yes)) {
	set_user_id(0);
	argc = 1;
      }
      
      uw->load(argv[1]);
    }
    catch (...) {
      // cannot read a file
      return 0;
    }
  }
  
  theApp->connect(theApp, SIGNAL(lastWindowClosed()), theApp, SLOT(quit()) );
    
  try {
    if (argc > 2) {
      if ((argc > 3) && !strcmp(argv[2], "-exec")) {
	bool with_exit = FALSE;
	
	if (!strcmp(argv[argc - 1], "-exit")) {
	  with_exit = TRUE;
	  argc -= 1;
	}
	
	QCString cmd = argv[3];
	QCString space = " ";
	int index;
	
	for (index = 4; index != argc; index += 1)
	  cmd += space + argv[index];
	
	ToolCom::run((const char *) cmd, BrowserView::get_project(), with_exit);
      }
      else
	msg_warning("Error", "Bouml was called with wrong parameters, ignore them");
    }
    
    theApp->exec();
  }
  catch (...) {
    // cannot read or write a file
    // exit after plug-out exec
    ;
  }
  
  return 0;
}
