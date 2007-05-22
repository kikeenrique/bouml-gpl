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

#include <stdlib.h>
#include <stdio.h>
#include <qdatetime.h>
#include <qapplication.h>

#include "mu.h"
#include "myio.h"
#include "BrowserView.h"
#include "DialogUtil.h"

static int Uid = -1;
static int RootPermission;	// old Uid or 0

void set_user_id(int i)
{
  if (Uid > 0) {
    QDir dir = BrowserView::get_dir();
    
    dir.rmdir(QString::number(Uid) + ".lock");
  }
  
  Uid = i;
}

int user_id()
{
  if (Uid == -1) {
    QDir dir = BrowserView::get_dir();
    
    if (dir.exists("all.lock")) {
      msg_critical("Bouml", 
			    "\
The project is open in read-only mode because it is\n\
under the control of 'Project control' or 'Project merge'\n\
(the directory 'all.lock' exists)");
      force_read_only(TRUE);
    }
    else
      force_read_only(FALSE);
    
    char * v = getenv("BOUML_ID");
    
    if ((v == 0) ||
	(sscanf(v, "%d", &Uid) != 1) ||
	(Uid < 2) ||
	(Uid > 127)) {
      Uid = QTime::currentTime().msec();
      
      for (;;) {
	Uid = Uid % 125 + 2;
	if (dir.mkdir(QString::number(Uid) + ".lock"))
	  break;
	if (!dir.exists(QString::number(Uid) + ".lock")) {
	  msg_critical("BOUML_ID", "Can't create directory "
		       + QString::number(Uid) +
		       ".lock,\nthe project is open in read-only mode");
	  force_read_only(TRUE);
	  break;
	}
      }
      
      msg_critical("BOUML_ID", 
		   "The BOUML_ID environment variable is not or wrong defined.\n\
\n\
This one allows several users to work on the same project, this time your\n\
identifier is " + QString::number(Uid) +
		   " but BOUML declines all the responsabilies of this random choice.\n\
\n\
You must to define the environment variable BOUML_ID valuing between 2 up to 127\n\
not used by an other person working at the same time on a project with you.");
    }
    else if (! dir.mkdir(QString::number(Uid) + ".lock")) {
      if (!dir.exists(QString::number(Uid) + ".lock")) {
	msg_critical("BOUML_ID", "Can't create directory "
		     + QString::number(Uid) +
		     ".lock,\nthe project is open in read-only mode");
	force_read_only(TRUE);
      }
      else {
	msg_critical("BOUML_ID", 
		     "\
It seems that you are already editing the project.\n\
If you're SURE that this is not the case\n\
and another user does not have an identifier\n\
equal to yours, remove the directory "
		     + QString::number(Uid) + ".lock\n\
an restart BOUML");
	exit(1);
      }
    }
  }
  
  return Uid;
}

void set_root_permission(bool y)
{
  RootPermission = (y) ? Uid : 0;
}

int root_permission()
{
  return RootPermission;
}

//

const char * user_name()
{
  static bool done = FALSE;
  static QCString name;
  
  if (! done) {
    name = QDir::home().dirName();
    done = TRUE;
  }
  
  return name;
}
