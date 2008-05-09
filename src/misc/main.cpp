// *************************************************************************
//
// Copyleft 2004-2008 Bruno PAGES  .
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





#include <qapplication.h>

#include "UmlWindow.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "UmlDesktop.h"
#include "ToolCom.h"
#include "BrowserView.h"
#include "BrowserPackage.h"
#include "Shortcut.h"
//#include "strutil.h"
#include "DialogUtil.h"
#include "mu.h"
#include "err.h"
#include "EnvDialog.h"

bool ExitOnError = FALSE;
QApplication * theApp;

//

int main(int argc, char **argv)
{
  ExitOnError = FALSE;

  theApp = new QApplication (argc, argv);

  UmlDesktop::init();
  
  bool conv_env = !QDir::home().exists(".boumlrc");
  
  if (conv_env)
    EnvDialog::edit(TRUE);
  
  init_pixmaps();
  init_font();
  Shortcut::init(conv_env);
  
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
	set_editor(getenv("BOUML_EDITOR")); // no environment file
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
    

    ExitOnError = TRUE;
    theApp->exec();
  }
  catch (...) {
    ;
  }
  
  return 0;
}
