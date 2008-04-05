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





#include <stdlib.h>

#include <qapplication.h>
#include <qmessagebox.h>
#include <qdir.h>

#include "SynchroWindow.h"
#include "Pixmap.h"
#include "myio.h"

int main(int argc, char ** argv)
{
  QApplication * app = new QApplication(argc, argv);
  
  char * v = getenv("BOUML_ID");
    
  if (v == 0)
    QMessageBox::critical(0, "Synchro project", "BOUML_ID is not defined");
  else {
    int uid = atoi(v);
    
    if ((uid < 2) || (uid > 127))
      QMessageBox::critical(0, "Synchro project", "invalid BOUML_ID");
    else {
      set_user_id(uid, QDir::home().dirName());
      app->connect(app, SIGNAL(lastWindowClosed()), SLOT(quit()) );
      init_pixmaps();
	
      SynchroWindow * w = new SynchroWindow();
      
      w->resize((QApplication::desktop()->width() * 3)/5,
		(QApplication::desktop()->height() * 3)/5);
  
      w->show();
      if (argc > 1)
	w->load(argc - 1, argv + 1);
      
      app->exec();
    }
  }

  return 0;
}
