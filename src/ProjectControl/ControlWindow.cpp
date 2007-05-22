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

#include <qapplication.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qstatusbar.h>
#include <qwindowsstyle.h> 
#include <qmotifstyle.h> 
#include <qmotifplusstyle.h> 
#include <qwhatsthis.h>
#include <qfiledialog.h> 
#include <qfile.h>
#include <qdir.h>
#include <qtextstream.h>

#include "ControlWindow.h"
#include "BrowserView.h"
#include "BrowserSearchDialog.h"
#include "UserDialog.h"
#include "myio.h"

#include "fileopen.xpm"
#include "browsersearch.xpm"
#include "actor.xpm"

ControlWindow * ControlWindow::the;

const char * OpenText = "To load a project";
const char * SearchText = "To search a <em>package</em> in the <em>browser</em>.";
const char * ChangeUserText = "To be an other user.";

ControlWindow::ControlWindow() : QMainWindow(0, "Project control", WDestructiveClose) {
  the = this;
  setCaption("Project control");
  
  QPopupMenu * menu;
  QPixmap pixmap;
  
  QToolBar * tools = new QToolBar(this, "operations");
  
  addToolBar(tools, "Operations", Top, TRUE);

  menu = new QPopupMenu(this);
  menuBar()->insertItem("P&roject", menu);

  // open
  
  pixmap = QPixmap(fileopen);
  QWhatsThis::add(new QToolButton(pixmap, "Open", QString::null,
				  this, SLOT(load()), tools, "open"),
		  OpenText);
  menu->setWhatsThis(menu->insertItem(pixmap, "&Open", this,
				      SLOT(load()), CTRL+Key_O),
		     OpenText);
  
  // change user
  
  pixmap = QPixmap(actor);
  QWhatsThis::add(new QToolButton(pixmap, "Who", QString::null,
				  this, SLOT(change_user()), tools, "Change user"),
		  ChangeUserText);
  menu->setWhatsThis(menu->insertItem(pixmap, "Change &user", this,
				      SLOT(change_user()), CTRL+Key_C),
		     ChangeUserText);

  // search
  
  pixmap = QPixmap(browsersearch);
  QWhatsThis::add(new QToolButton(pixmap, "Search", QString::null,
				  this, SLOT(browser_search()), tools, "search"),
		  SearchText);
  menu->setWhatsThis(menu->insertItem(pixmap, "&Search", this,
				      SLOT(browser_search()), CTRL+Key_S),
		     SearchText);

  // quit & what
  
  menu->insertItem("&Quit", this, SLOT(quit()), CTRL+Key_Q);

  (void)QWhatsThis::whatsThisButton(tools);
  
  // historic

  QFile fp(QDir::home().absFilePath(".bouml"));
  
  if (fp.open(IO_ReadOnly)) {
    QTextStream ts(&fp);
    
    ts.setEncoding(QTextStream::Latin1);
    
    QString ln;
    
    while (!(ln = ts.readLine()).isEmpty())
      historic.append(ln);
  }
  
  menu->insertSeparator();
  QString whats = QString("to open this project.<br><br>The historic is saved in <i>")
    + QDir::home().absFilePath(".bouml") + "</i>";
  
  for (int i = 0; i < int(historic.count()); i += 1) {
    int id = menu->insertItem(*historic.at(i),
			      this, SLOT(historicActivated(int)));
    menu->setItemParameter(id, i);
    menu->setWhatsThis(id, whats);
  }
  
  // style
  
  menu = new QPopupMenu(this);
  menuBar()->insertItem("&Style", menu);
  
#if !defined(QT_NO_STYLE_MOTIF)
  menu->insertItem("Motif", this, SLOT(motif_style()));
#endif
#if !defined(QT_NO_STYLE_MOTIFPLUS)
  menu->insertItem("MotifPlus", this, SLOT(motifplus_style()));
#endif
  menu->insertItem("Windows", this, SLOT(windows_style()));
  
  // help & about
  
  menuBar()->insertSeparator();
  menu = new QPopupMenu(this);
  menuBar()->insertItem("&Help", menu);
  
  menu->insertItem("&About", this, SLOT(about()), Key_F1);
  menu->insertItem("About&Qt", this, SLOT(aboutQt()));
  menu->insertSeparator();
  menu->insertItem("What's This", this, SLOT(whatsThis()), SHIFT+Key_F1);
    
  //

  browser = new BrowserView(this);
  setCentralWidget(browser); 
  
  //
  
  show_identity();
}

ControlWindow::~ControlWindow() {
  browser->close();
}

void ControlWindow::historicActivated(int id) {
  QStringList::Iterator it = historic.at(id);
  
  if (it != historic.end())
    load(*it);
}

void ControlWindow::show_identity() {
  statusBar()->message("User is " + user_name(user_id()) +
		       " [id " + QString::number(user_id()) + "]");
}

void ControlWindow::change_user() {
  UserDialog d(QCursor::pos());
  
  if (d.exec() == QDialog::Accepted) {
    set_user_id(d.id(), d.name());
    show_identity();
    browser->hide();
    browser->show();
  }
}

void ControlWindow::load(QString path) {
  browser->close();
  
  QFileInfo fi(path);
  
  if (!fi.exists())
    return;
 
  QDir dir(fi.dirPath(TRUE));
  
  if (! dir.mkdir("all.lock"))
    QMessageBox::critical(0, "Control project", 
			  (dir.exists("all.lock"))
			  ? "\
The project is already locked by 'Project control' or 'Project syncho'\n\
(the directory 'all.lock' exists)"
			  : "Can't create directory 'all.lock'");
  else {
    const QFileInfoList * l = dir.entryInfoList("*.lock");
    QListIterator<QFileInfo> it(*l);
    QFileInfo * fi;
    QString ids;
    
    while ((fi = it.current()) != 0) {
      if (fi->isDir() && (fi->baseName() != "all"))
	ids += " " + fi->baseName();
      ++it;
    }
    
    if (! ids.isEmpty()) {
      QMessageBox::critical(0, "Control project", 
			    "The project is edited by the users having these BOUML_ID :" + ids);
      dir.rmdir("all.lock");
    }
    else {
      QApplication::setOverrideCursor(Qt::waitCursor);
      browser->set_project(dir);
      browser->get_project()->setOpen(TRUE);
      
      bool r = browser->get_project()->load(dir);
      
      QApplication::restoreOverrideCursor();
      
      if (! r)
	browser->close();
      // note : all.lock will be deleted by BrowserView
    }
  }
}

void ControlWindow::load() {
  QString path = QFileDialog::getOpenFileName(QString::null, "*.prj", this);
  
  if (! path.isEmpty())
    load(path);
}

void ControlWindow::quit() {
  browser->close();
  QApplication::exit(0);
}

void ControlWindow::browser_search() {
  BrowserSearchDialog dialog(QCursor::pos());
  
  dialog.exec();
}

void ControlWindow::motif_style() {
#if !defined(QT_NO_STYLE_MOTIF)
  QApplication::setStyle(new QMotifStyle);
#endif
}

void ControlWindow::motifplus_style() {
#if !defined(QT_NO_STYLE_MOTIFPLUS)
  QApplication::setStyle(new QMotifPlusStyle);
#endif
}

void ControlWindow::windows_style() {
#ifndef QT_NO_STYLE_WINDOWS
  QApplication::setStyle(new QWindowsStyle);
#endif
}

void ControlWindow::about() {
  QMessageBox::about(this, "Project control", "<p>Version <b>1.1</b></p>" );
}

void ControlWindow::aboutQt() {
  QMessageBox::aboutQt(this, "Project control");
}
