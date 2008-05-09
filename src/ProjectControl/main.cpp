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
#include <qtextstream.h>
#include <qdir.h>

#include "ControlWindow.h"
#include "Pixmap.h"
#include "myio.h"

int main(int argc, char ** argv)
{
  QApplication * app = new QApplication(argc, argv);
  
  QFile fp(QDir::home().absFilePath(".boumlrc"));
  int uid = -1;
  
  if (fp.open(IO_ReadOnly)) {
    QTextStream ts(&fp);
    
    ts.setEncoding(QTextStream::Latin1);
    
    QString ln;
    
    while (!(ln = ts.readLine()).isEmpty()) {
      const char * p = ln;

      if (!strncmp(p, "ID ", 3)) {
	sscanf(p+3, "%d", &uid);
	break;
      }
    }
  }
    
  if (uid == -1)
    QMessageBox::critical(0, "Control project", "Own identifier not defined");
  else if ((uid < 2) || (uid > 127))
    QMessageBox::critical(0, "Control project", "invalid Identifier");
  else {
    set_user_id(uid, QDir::home().dirName());
    app->connect(app, SIGNAL(lastWindowClosed()), SLOT(quit()) );
    init_pixmaps();
    
    ControlWindow * w = new ControlWindow();
    
    w->resize((QApplication::desktop()->width() * 3)/5,
	      (QApplication::desktop()->height() * 3)/5);
    
    w->show();
    
    app->exec();
  }

  return 0;
}
