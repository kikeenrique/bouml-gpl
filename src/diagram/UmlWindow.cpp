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

#include <stdlib.h>
#include <qcursor.h>
#include <qworkspace.h>
#include <qsplitter.h>
#include <qvbox.h>
#include <qhbox.h>
#include <qmultilineedit.h>
#include <qstatusbar.h>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#ifndef QT_NO_PRINTER
#include <qprinter.h>
#include <qpainter.h>
#endif
#include <qwhatsthis.h>
#include <qapplication.h>
#include <qfiledialog.h>
#include <qfileinfo.h> 
#include <qwindowsstyle.h> 
#include <qmotifstyle.h> 
#include <qmotifplusstyle.h> 
//#include <qcdestyle.h> 
//#include <qsgistyle.h>

#include "UmlWindow.h"
#include "UmlDesktop.h"
#include "BrowserView.h"
#include "BrowserPackage.h"
#include "BrowserDiagram.h"
#include "BrowserClass.h"
#include "BrowserState.h"
#include "BrowserActivity.h"
#include "BrowserUseCase.h"
#include "BrowserComponent.h"
#include "BrowserArtifact.h"
#include "BrowserDeploymentNode.h"
#include "DiagramWindow.h"
#include "DiagramView.h"
#include "GenerationSettings.h"
#include "TraceDialog.h"
#include "Tool.h"
#include "ToolDialog.h"
#include "BrowserSearchDialog.h"
#include "ShortcutDialog.h"
#include "BasicData.h"
#include "ToolCom.h"
#include "About.h"
#include "UmlGlobal.h"
#include "UmlPixmap.h"
#include "DialogUtil.h"
#include "myio.h"
#include "mu.h"

#include "filesave.xpm"
#include "fileopen.xpm"
#include "fileprint.xpm"
#include "browsersearch.xpm"

UmlWindow * UmlWindow::the;

const char * projectNewText = "To create a <em>new project</em>.<br><br>"
  "You will ask to create a new directory having the name of the project "
  "where all the saving files will be placed.";
const char * projectOpenText = "To open a <em>project</em>.<br><br>"
  "You can also select the <b>Open command</b> from the Project menu.";
const char * projectSaveText = "To save the project.<br><br>"
  "You can also select the Save command from the Project menu.";
const char * projectSaveAsText = "To save all in a new project.";
const char * diagramPrintText = "To print the current diagram.<br><br>"
  "You can also select the Print command from the Project menu.";
const char * cppText = "To set or not the C++ definition/declaration "
  "to the default value when a class/operation/attribute/relation is created";
const char * javaText = "To set or not the Java definition/declaration "
  "to the default value when a class/operation/attribute/relation is created";
const char * idlText = "To set or not the IDL definition/declaration "
  "to the default value when a class/operation/attribute/relation is created";
const char * verboseText = "To ask or not for a verbose code generation";
const char * preserve_bodiesText = "To preserve or not the operations's body";
const char * add_operation_profileText = "To write the operation profile at the beginning of the temporary file when you edit an operation's body";
const char * viewBrowserStereotypeText = "To show or hide the <em>stereotypes</em> "
  "in the <b>Browser</b>.";
const char * browserUpText = "To select the <em>browser</em> current item's parent.";
const char * browserSearchText = "To search an item in the <em>browser</em>.";
const char * fontSizeMenuText = "To set the point size of the base font used in the "
  "<em>browser</em> and the diagrams.";
const char * formatMenuText = "To set the default format of the diagrams.";
static const char * prevText = "To select the previously selected element in the <em>browser</em>.";
static const char * nextText = "To select the next selected element in the <em>browser</em>.";

UmlWindow::UmlWindow() : QMainWindow(0, "Bouml", WDestructiveClose) {
  setCaption("Bouml");
  
  the = this;
  commented = 0;
  format = IsoA4;
  
  //
  // buttons and menus
  //
  
  QPixmap openIcon, saveIcon;
  
  projectTools = new QToolBar(this, "project operations");
  addToolBar(projectTools, "Project Operations", Top, TRUE);
  
  openIcon = QPixmap(fileopen);
  QToolButton * projectOpen
    = new QToolButton(openIcon, "Open Project", QString::null,
		      this, SLOT(load()), projectTools, "open project");
  
  saveIcon = QPixmap(filesave);
  QToolButton * projectSave
    = new QToolButton(saveIcon, "Save Project", QString::null,
		      this, SLOT(save()), projectTools, "save project");
  
#ifndef QT_NO_PRINTER
  QPixmap printIcon;
  
  printIcon = QPixmap(fileprint);
  QToolButton * diagramPrint
    = new QToolButton(printIcon, "Print diagram", QString::null,
		      this, SLOT(print()), projectTools, "print diagram");
  QWhatsThis::add(diagramPrint, diagramPrintText);
#endif
  
  QPixmap searchIcon = QPixmap(browsersearch);
  QToolButton * browserSearch
    = new QToolButton(searchIcon, "Browser search", QString::null,
		      this, SLOT(browser_search()), projectTools, "browser search");
  QWhatsThis::add(browserSearch, browserSearchText);
  
  prev = new QToolButton(*leftPixmap, "previous selected", QString::null,
			 this, SLOT(prev_select()), projectTools, "previous selected");
  QWhatsThis::add(prev, prevText);
  
  next = new QToolButton(*rightPixmap, "next selected", QString::null,
			 this, SLOT(next_select()), projectTools, "next selected");
  QWhatsThis::add(next, nextText);
  
  (void)QWhatsThis::whatsThisButton(projectTools);

  QWhatsThis::add(projectOpen, projectOpenText);
  QWhatsThis::add(projectSave, projectSaveText);
  
  projectMenu = new QPopupMenu(this);
  menuBar()->insertItem("&Project", projectMenu);
  connect(projectMenu, SIGNAL(aboutToShow()),
	  this, SLOT(projectMenuAboutToShow()));
    
  windowsMenu = new QPopupMenu(this);
  windowsMenu->setCheckable(TRUE);
  connect(windowsMenu, SIGNAL(aboutToShow()),
	  this, SLOT(windowsMenuAboutToShow()));
  menuBar()->insertItem("&Windows", windowsMenu);
  
  toolMenu = new QPopupMenu(this);
  connect(toolMenu, SIGNAL(aboutToShow()), this, SLOT(toolMenuAboutToShow()));
  menuBar()->insertItem("&Tools", toolMenu);

  miscMenu = new QPopupMenu(this);
  menuBar()->insertItem("&Miscellaneous", miscMenu);
  miscMenu->setCheckable(TRUE);
  connect(miscMenu, SIGNAL(aboutToShow()),
	  this, SLOT(miscMenuAboutToShow()));
  
  show_browser_stereotypes_id =
    miscMenu->insertItem("Show s&tereotypes in browser", this,
			 SLOT(show_stereotypes()));
  miscMenu->setItemChecked(show_browser_stereotypes_id, TRUE);
  miscMenu->setWhatsThis(show_browser_stereotypes_id, viewBrowserStereotypeText);
  
  QPopupMenu * pmstyle = new QPopupMenu(this);
  bool used = FALSE;

#ifndef QT_NO_STYLE_MOTIF
  pmstyle->insertItem("Motif", this, SLOT(motif_style()));
  used = TRUE;
#endif
#ifndef QT_NO_STYLE_MOTIFPLUS
  pmstyle->insertItem("MotifPlus", this, SLOT(motifplus_style()));
  used = TRUE;
#endif
  //pmstyle->insertItem("Sgi", this, SLOT(sgi_style()));
  //pmstyle->insertItem("Cde", this, SLOT(cde_style()));
#ifndef QT_NO_STYLE_WINDOWS
  pmstyle->insertItem("Windows", this, SLOT(windows_style()));
  used = TRUE;
#endif
  
  if (used)
    miscMenu->insertItem("Style", pmstyle);
  style = '?';
  
  fontSizeMenu = new QPopupMenu(this);
  fontSizeMenu->setCheckable(TRUE);
  connect(fontSizeMenu, SIGNAL(aboutToShow()),
	  this, SLOT(fontSizeMenuAboutToShow()));

#define FONTSIZEMIN 6
#define FONTSIZESUP 16
  
  for (int i = FONTSIZEMIN; i < FONTSIZESUP; i += 1)
    fontSizeMenu->insertItem(QString::number(i), this,
			     SLOT(setFontSize(int)), 0, i);

  int id = miscMenu->insertItem("Font size", fontSizeMenu);
  miscMenu->setWhatsThis(id, fontSizeMenuText);
  
  formatMenu = new QPopupMenu(this);
  formatMenu->setCheckable(TRUE);
  connect(formatMenu, SIGNAL(aboutToShow()),
	  this, SLOT(formatMenuAboutToShow()));
  for (int f = IsoA0; f != CanvasFormatSup; f += 1)
    formatMenu->insertItem(stringify((CanvasFormat) f), this,
			   SLOT(setFormat(int)), 0, f);

  id = miscMenu->insertItem("Diagram default format", formatMenu);
  miscMenu->setWhatsThis(id, formatMenuText);
  
  miscMenu->insertSeparator();
  use_cpp_id =
    miscMenu->insertItem("C++ default declaration/definition", this, SLOT(use_cpp()));
  miscMenu->setItemChecked(use_cpp_id, GenerationSettings::cpp_get_default_defs());
  miscMenu->setWhatsThis(use_cpp_id, cppText);
  use_java_id =
    miscMenu->insertItem("Java default definition", this, SLOT(use_java()));
  miscMenu->setItemChecked(use_java_id, GenerationSettings::java_get_default_defs());
  miscMenu->setWhatsThis(use_java_id, javaText);
  use_idl_id =
    miscMenu->insertItem("Idl default declaration", this, SLOT(use_idl()));
  miscMenu->setItemChecked(use_idl_id, GenerationSettings::idl_get_default_defs());
  miscMenu->setWhatsThis(use_idl_id, idlText);
  
  miscMenu->insertSeparator();
  verbose_gen_id =
    miscMenu->insertItem("Verbose code generation", this, SLOT(verbose()));
  miscMenu->setWhatsThis(verbose_gen_id, verboseText);

  preserve_bodies_id =
    miscMenu->insertItem("Preserve operations's body", this, SLOT(preserve()));
  miscMenu->setWhatsThis(preserve_bodies_id, preserve_bodiesText);

  add_operation_profile_id =
    miscMenu->insertItem("Add operation profile on body edition", this,
			 SLOT(addoperationprofile()));
  miscMenu->setWhatsThis(add_operation_profile_id, add_operation_profileText);
  
  miscMenu->insertSeparator();
  shortcut_id =
    miscMenu->insertItem("Edit shortcuts", this, SLOT(edit_shortcuts()));
  
  menuBar()->insertSeparator();
  QPopupMenu * help = new QPopupMenu(this);
  menuBar()->insertItem("&Help", help);
  
  help->insertItem("&About", this, SLOT(about()), Key_F1);
  help->insertItem("About&Qt", this, SLOT(aboutQt()));
  help->insertSeparator();
  help->insertItem("What's This", this, SLOT(whatsThis()), SHIFT+Key_F1);
    
  //
  // sub windows
  //
  
  spl1 = new QSplitter(QSplitter::Horizontal, this, "spl1");
  spl2 = new QSplitter(QSplitter::Vertical, spl1, "spl2");
  
  browser = new BrowserView(spl1);
  
  ws = new QWorkspace(spl2);
  
  comment = new MultiLineEdit(spl2);
  set_commented(0);
  connect(comment, SIGNAL(textChanged()), this, SLOT(comment_changed())); 
  
  spl1->moveToFirst(browser);
  spl2->moveToFirst(ws);
  
  int w = (UmlDesktop::width() * 15)/16;
  
  resize(w, (UmlDesktop::height() * 7)/8);
  UmlDesktop::tocenter(this);
  
  // browser width = total width/4
  QValueList<int> lsz = spl1->sizes();
  
  lsz.first() = w/4;
  lsz.last() = w - lsz.first();
  
  spl1->setSizes(lsz);
  
  // on resize try to resize only the diagrams
  spl1->setResizeMode(browser, QSplitter::KeepSize);
  spl2->setResizeMode(comment, QSplitter::KeepSize);

#ifdef _WS_QWS_
  // Qt/Embedded XOR drawing not yet implemented.
  spl1->setOpaqueResize(TRUE);
  spl2->setOpaqueResize(TRUE);
#endif
  
  setCentralWidget(spl1);  
  
  //
  // read historic
  //
  
  QFile fp(QDir::home().absFilePath(".bouml"));
  
  if (fp.open(IO_ReadOnly)) {
    QTextStream ts(&fp);
    
    ts.setEncoding(QTextStream::Latin1);
    
    QString ln;
    
    while (!(ln = ts.readLine()).isEmpty())
      historic.append(ln);
  }
  
  //
  
  clear_select_historic();
  
  //
  
  statusBar()->message("Ready", 2000);
}

UmlWindow::~UmlWindow() {
  BrowserView::remove_temporary_files();
  set_user_id(-1);
  exit(0);
}

void UmlWindow::projectMenuAboutToShow() {
  abort_line_construction();
  
  projectMenu->clear();
  
  if (!BrowserNode::edition_active()) {
    int id;
    bool enabled =  (browser->get_project() != 0);
    QPixmap openIcon = QPixmap(fileopen);
    QPixmap saveIcon = QPixmap(filesave);
    
    id = projectMenu->insertItem("&New", this, SLOT(newProject()));
    projectMenu->setWhatsThis(id, projectNewText);
    
    id = projectMenu->insertItem(openIcon, "&Open",
				 this, SLOT(load()));
    projectMenu->setWhatsThis(id, projectOpenText);
    
    id = projectMenu->insertItem(saveIcon, "&Save",
				 this, SLOT(save()), CTRL+Key_S);
    projectMenu->setWhatsThis(id, projectSaveText);
    projectMenu->setItemEnabled(id, enabled);
    
    id = projectMenu->insertItem("Save &as...", this, SLOT(saveAs()));
    projectMenu->setWhatsThis(id, projectSaveAsText);
    projectMenu->setItemEnabled(id, enabled);
    
    projectMenu->insertSeparator();
    projectMenu->insertItem("&Close", this, SLOT(close()));
    projectMenu->setItemEnabled(id, enabled);
    projectMenu->insertItem("&Quit", this, SLOT(quit()));
    
    // edit
    
    if (enabled) {
      bool writable = browser->get_project()->is_writable();
      QPopupMenu * ed = new QPopupMenu(this);
    
      projectMenu->insertSeparator();
      projectMenu->insertItem("Edit", ed);
      
      id = ed->insertItem("Edit generation settings", this, SLOT(edit_gen_settings()));
      ed->setWhatsThis(id, "to set how an Uml type is compiled in C++ etc..., \
to set the default parameter passing, to set the default code \
produced for an attribute etc..., and to set the root directories");
      ed->setItemEnabled(id, writable);
      
      id = ed->insertItem("Edit default stereotypes", this, SLOT(edit_stereotypes()));
      ed->setWhatsThis(id, "to set the default stereotypes list");
      ed->setItemEnabled(id, writable);
      
      ed->insertSeparator();
      
      id = ed->insertItem("Edit class settings", this, SLOT(edit_class_settings()));
      ed->setWhatsThis(id, "to set the sub classes settings");
      ed->setItemEnabled(id, writable);
      
      id = ed->insertItem("Edit drawing settings", this, SLOT(edit_drawing_settings()));
      ed->setWhatsThis(id, "to set how the sub <em>diagrams</em>'s items must be drawed");
      ed->setItemEnabled(id, writable);
    }
    
    // historic
    
    projectMenu->insertSeparator();
    QString whats = QString("to open this project.<br><br>The historic is saved in <i>")
      + QDir::home().absFilePath(".bouml") + "</i>";
    
    for (int i = 0; i < int(historic.count()); i += 1) {
      id = projectMenu->insertItem(*historic.at(i),
				   this, SLOT(historicActivated(int)));
      projectMenu->setItemParameter(id, i);
      projectMenu->setWhatsThis(id, whats);
    }
  }
  else
    msg_warning("Bouml",
		"Nothing available while a dialog is openned");
}

void UmlWindow::historicActivated(int id) {
  QStringList::Iterator it = historic.at(id);
  
  if (it != historic.end()) {
    close();
  
    if (browser->get_project() == 0)
      load(*it);
    else
      statusBar()->message("Loading aborted", 2000);
  }
}

void UmlWindow::clear()
{
  while (! the->ws->windowList().isEmpty()) {
    DiagramWindow * w = (DiagramWindow *) the->ws->windowList().at(0);
    
    w->dont_save();
    w->close(TRUE);
  }
}

void UmlWindow::toolMenuAboutToShow() {
  abort_line_construction();
  
  toolMenu->clear();
  toolMenu->insertItem("Show &Trace Window", this, SLOT(show_trace()));
  if (browser->get_project() != 0) {
    toolMenu->insertSeparator();
    toolMenu->insertItem("Generate C++", this, SLOT(cpp_generate()), CTRL+Key_G);
    toolMenu->insertItem("Generate Java", this, SLOT(java_generate()), CTRL+Key_J);
    toolMenu->insertItem("Generate Idl", this, SLOT(idl_generate()), CTRL+Key_I);
    if (!BrowserNode::edition_active()) {
      toolMenu->insertSeparator();
      toolMenu->insertItem("Reverse C++", this, SLOT(cpp_reverse()));
      toolMenu->insertItem("Reverse Java", this, SLOT(java_reverse()));
      toolMenu->insertSeparator();
      toolMenu->insertItem("Java Catalog", this, SLOT(java_catalog()));
      if (BrowserClass::find("UmlBaseItem") != 0) {
	toolMenu->insertSeparator();
	toolMenu->insertItem("Plug-out upgrade", this, SLOT(plug_out_upgrade()));
      }
      Tool::menu_insert(toolMenu, UmlProject, this, SLOT(run_tool(int)));
    }
    if (browser->get_project()->is_writable()) {
      toolMenu->insertSeparator();
      toolMenu->insertItem("Tools settings", this, SLOT(tool_settings()));
      toolMenu->insertItem("Import settings", this, SLOT(import_tool_settings()));
    }
  }
}
  
void UmlWindow::set_commented(BrowserNode * bn)
{
  if (the != 0) {
    // not on exit
    UmlWindow * him = the;
    bool same = (him->commented == bn);
    
    the = 0;	// to do nothing in comment_changed() which is called
    
    him->commented = bn;
    
    if (bn != 0) {
      him->comment->setText(bn->get_comment());
      him->comment->setReadOnly(!bn->is_writable());
      him->statusBar()->message(bn->get_data()->definition(TRUE));
      if (! same)
	him->is_selected(bn);
    }
    else {
      him->comment->clear();
      him->comment->setReadOnly(TRUE);
      him->statusBar()->clear();
    }
    the = him;
  }
}

void UmlWindow::comment_changed()
{
  if ((the != 0) && (the->commented != 0) &&
      (the->commented->get_comment() != the->comment->text())) {
    the->commented->set_comment(the->comment->text());
    the->commented->modified();
    the->commented->package_modified();
  }
}

void UmlWindow::set_message(const QString & m)
{
  the->statusBar()->message(m);
}

QWorkspace * UmlWindow::get_workspace()
{
  return the->ws;
}

CanvasFormat UmlWindow::default_format()
{
  return the->format;
}

void UmlWindow::newProject() {
  abort_line_construction();
  if (!BrowserNode::edition_active()) {
    close();
    
    if (browser->get_project() == 0) {
      QString f = QFileDialog::getSaveFileName(QString::null, "*", this,
					       0, "Select parent directory");
      
      if (!f.isEmpty()) {
	QDir di;
	
	if (di.mkdir(f)) {
	  di.cd(f);
	  browser->set_project(di);
	  Tool::defaults();
	  browser->get_project()->BrowserPackage::save_all(FALSE);
	  
	  msg_warning("New project",
		      "Do not forget to set the default target language(s)\n"
		      "through the 'Miscellaneous' menu\n"
		      "\n"
		      "If you program in Java, the Java Catalog plug-out\n"
		      "will help you, use it !");
	}
	else
	  msg_critical("New project", "Cannot create directory " + f);
      }
    }
  }
}

void UmlWindow::load() {
  abort_line_construction();
  if (!BrowserNode::edition_active()) {
    close();
    
    if (browser->get_project() == 0) {
      QString fn = QFileDialog::getOpenFileName(QString::null, "*.prj", this);
      
      if (!fn.isEmpty())
	load(fn);
      else
	statusBar()->message("Loading aborted", 2000);
    }
  }
}

void UmlWindow::historic_add(QString fn)
{
  QStringList::Iterator it = the->historic.find(fn);
  
  if (it != the->historic.end())
    the->historic.remove(it);
  
  the->historic.prepend(fn);
  
  QFile fp(QDir::home().absFilePath(".bouml"));
  
  if (fp.open(IO_WriteOnly)) {
    int rank;
    QTextStream ts(&fp);
    
    ts.setEncoding(QTextStream::Latin1);

    for (it = the->historic.begin(), rank = 0;
	 (it != the->historic.end()) && (rank != 10);
	 ++it, rank += 1) {
      ts.writeRawBytes((const char *) *it, (*it).length());
      ts.writeRawBytes("\n", 1);
    }
  }
}

void UmlWindow::load_it(QString fn) {
  // plug-out
  close_it();
  the->load(fn);
}

void UmlWindow::load(QString fn) {
  clear_select_historic();
  
  QFileInfo fi(fn);
  
  if (!fi.exists()) {
    return;
  }
  
  QDir di(fi.dirPath(TRUE));
  QString s = fi.fileName();
  
  s.truncate(s.length() - 4);	// QFileInfo::baseName remove all after first dot
  
  if (di.dirName() != s) {
    msg_critical("Uml",
		 "The name of the project and the name of\n"
		 "the directory containing it must be the same\n\n"
		 "Project name : " + s + "\n"
		 "Directory name : " + di.dirName());
    close_it();
    return;
  }
   
  historic_add(fi.absFilePath());
  
  QApplication::setOverrideCursor(Qt::waitCursor);
  set_in_import(FALSE);
  browser->set_project(di);
  BrowserNode::pre_load();
  
  unsigned format;
  
  try {
    format = browser->get_project()->load(TRUE);
  }
  catch (int) {
    QApplication::restoreOverrideCursor();
    close_it();
    clear_select_historic(); // in case selection are done by error
  
    return;
  }
  
  BrowserNode::post_load();
  browser->get_project()->setOpen(TRUE);
  QApplication::restoreOverrideCursor();
  setCaption("Bouml : " + fn);
  
  if (format < 20) {
    BrowserClass::plug_out_conversion();
    
    QString new_st = GenerationSettings::new_java_enums();
    
    BrowserClass::new_java_enums(new_st);
    
    browser->get_project()->package_modified();
    msg_warning("BOUML",
		"Project conversion done.<br><br>"
		"A <i>save-as</i> is forced now to save the result "
		"in a new project, then the project will be closed");
    saveAs();
    close_it();		// write access of added items not ok
  }
  else if (format < 22) {
    QString new_st = GenerationSettings::new_java_enums();
    bool must_save_as = BrowserClass::new_java_enums(new_st);

    QString m = ("Class stereotype <i>enum_pattern</i> added.<br><br>"
		 "The classes having the stereotype <i>enum</i> and a non empty "
		 "Java definition have now the stereotype <i>enum_pattern</i>.<br><br>");

    if (!new_st.isEmpty())
      m = "The class stereotype <i>enum_pattern</i> is renamed <i>" + new_st +
	"</i>and the classes having the stereotype <i>enum_pattern</i> have now the stereotype <i>" +
	  new_st + "</i><br><br>" + m;
  
    if (must_save_as)
      m += "A <i>save-as</i> is forced now to save the result in a new project";
    
    msg_warning("Bouml", m);
    
    if (must_save_as) {
      if (! saveas_it())
	close_it();
    }
    else
      browser->get_project()->package_modified();
  }
  else if ((user_id() != 0) && (fi.baseName() == "empty")) {
    set_user_id(-1);
    saveAs();
  }
  else {
    BrowserView::remove_temporary_files();
    read_session();
  }
  
  clear_select_historic(); // in case selection are done by error
}

void UmlWindow::save() {
  abort_line_construction();
  if (browser->get_project()) {
    if (!BrowserNode::edition_active()) {
      // hide the window else the user see that 
      // all the not-open diagram window will be open
      // to save their content
      QApplication::setOverrideCursor(Qt::waitCursor);
      ws->hide();
      BrowserPackage::save_all(TRUE);
      ws->show();
      QApplication::restoreOverrideCursor();
    }
    else
      msg_warning("Bouml",
		  "Saving can't be done while a dialog is openned");
  }
}

void UmlWindow::save_it()
{
  the->save();
}

void UmlWindow::saveAs() {
  abort_line_construction();
  saveas_it();
}

bool UmlWindow::saveas_it()
{
  if (the->browser->get_project() && !BrowserNode::edition_active()) {
    for (;;) {
      QString f = QFileDialog::getSaveFileName(QString::null, "*", the,
					       0, "Select parent directory");
      
      if (!f.isEmpty()) {
	QDir d(f);
	
	if (d.dirName() == "empty")
	  msg_critical("Error", "'empty' is reserved to the empty plug-out");
	else {
	  QDir di;
	  
	  while (!di.mkdir(f)) {
	    if (msg_critical("Error", QString("Cannot create directory\n") + f,
			     QMessageBox::Retry, QMessageBox::Abort)
		!= QMessageBox::Retry) {
	      if (!strcmp(the->browser->get_project()->get_name(), "empty"))
		exit(0);
	      return FALSE;
	    }
	  }
	  
	  QApplication::setOverrideCursor(Qt::waitCursor);
	  di.cd(f);
	  the->ws->hide();
	  if (the->browser->save_as(di))
	    BrowserPackage::save_all(FALSE);
	  the->ws->show();
	  QApplication::restoreOverrideCursor();
	  the->setCaption("Bouml : " + f);
	  return TRUE;
	}
      }
      else if (!strcmp(the->browser->get_project()->get_name(), "empty"))
	exit(0);
      else
	return FALSE;
    }
  }
  
  return FALSE;
}


bool UmlWindow::can_close() {
  if (browser->get_project()) {
    if (BrowserPackage::must_be_saved()) {
      switch (msg_warning("Bouml",
			  "The project is modified, save it ?\n",
			  QMessageBox::Yes, QMessageBox::No,
			  QMessageBox::Cancel)) {
      case QMessageBox::Yes:
	ws->hide();
	BrowserPackage::save_all(TRUE);
	ws->show();
	break;
      case QMessageBox::Cancel:
	statusBar()->message("Close aborted", 2000);
	return FALSE;
      }
    }
  }
  
  return TRUE;
}

void UmlWindow::close() {
  abort_line_construction();
  if (can_close())
    close_it();
}

void UmlWindow::closeEvent(QCloseEvent * e) {
  if (can_close()) {
    save_session();
    e->accept();
  }
}

void UmlWindow::close_it()
{
  clear_select_historic();
  
  ToolCom::close_all();
  
  the->save_session();
  
  QApplication::setOverrideCursor(Qt::waitCursor);
  
  // close all diagram windows
  // do not hide ws, else a future diagram window opening will crash !
  the->clear();
  
  // empty the browser
  the->browser->clear();
  
  // remove tools
  the->toolMenu->clear();
  
  QApplication::restoreOverrideCursor();
  the->setCaption("Bouml");

  Tool::init();
  GenerationSettings::init();
  BrowserPackage::init();
  BrowserClass::init();
  BrowserState::init();
  BrowserActivity::init();
  BrowserUseCase::init();
  BrowserComponent::init();
  BrowserArtifact::init();
  BrowserDeploymentNode::init();
  DiagramView::init();
}

void UmlWindow::quit() {
  abort_line_construction();
  if (!BrowserNode::edition_active() && can_close()) {
    if (browser->get_project() != 0) {
      save_session();
      BrowserView::remove_temporary_files();
      // delete lock
      set_user_id(-1);
    }
    QApplication::exit(0);
  }
}

void UmlWindow::save_session() {
  if ((browser->get_project() == 0) ||
      (user_id() == 0) ||
      (getenv("BOUML_ID") == 0))
    return;
  
  QDir d = BrowserView::get_dir();
  char fn[32];
  
  sprintf(fn, "%d.session", user_id());
  
  QFile fp(d.absFilePath(fn));
      
  if (open_file(fp, IO_WriteOnly, TRUE) != -1) {
    QTextStream st(&fp);
    
    st.setEncoding(QTextStream::Latin1);
    
    st << "window_sizes " << width() << " " << height() << " "
       << spl1->sizes().first() << " " << spl1->sizes().last() << " "
       << spl2->sizes().first() << " " << spl2->sizes().last() << '\n';

    switch (style) {
    case 'w': st << "windows_style\n"; break;
    case 'm' : st << "motif_style\n"; break;
    case '+' : st << "motifplus_style\n"; break;
    default: break;
    }
    
    QWidgetList windows = ws->windowList();
    
    if (! windows.isEmpty()) {
      st << "diagrams\n";    
      for (unsigned i = 0; i != windows.count(); i += 1) {
	if (ws->activeWindow() == windows.at(i))
	  st << "  active";
	((DiagramWindow *) windows.at(i))->save_session(st);
      }
      st << "end\n";
    }
    
    browser->get_project()->save_session(st);
    
    if (verbose_generation())
      st << "verbose\n";       
  }
}

void UmlWindow::read_session() {
  if ((user_id() == 0) || (getenv("BOUML_ID") == 0))
    return;
  
  QDir d = BrowserView::get_dir();
  char fn[32];
  
  sprintf(fn, "%d.session", user_id());
  
  QFile fp(d.absFilePath(fn));
  int size;
      
  if ((size = open_file(fp, IO_ReadOnly, TRUE)) != -1) {
    char * s = new char[size + 1];
    
    if (fp.readBlock(s, size) != -1) {
      s[size] = 0;
      
      try {
	char * st = s;
      
	read_keyword(st, "window_sizes");
	
	int w = read_unsigned(st);
	
	resize(w, read_unsigned(st));
	{
	  extern QApplication * theApp;
	  theApp->processEvents(/*500*/);
	}
	
	QValueList<int> lsz = spl1->sizes();
  
	lsz.first() = read_unsigned(st);
	lsz.last() = read_unsigned(st);
  
	spl1->setSizes(lsz);
	
	lsz = spl2->sizes();
  
	lsz.first() = read_unsigned(st);
	lsz.last() = read_unsigned(st);
  
	spl2->setSizes(lsz);
	{
	  extern QApplication * theApp;
	  theApp->processEvents(/*500*/);
	}
	
	const char * k = read_keyword(st);
  
	if (!strcmp(k, "windows_style")) {
	  windows_style();
	  k = read_keyword(st);
	}
	else if (!strcmp(k, "motif_style")) {
	  motif_style();
	  k = read_keyword(st);
	}
	else if (!strcmp(k, "motifplus_style")) {
	  motifplus_style();
	  k = read_keyword(st);
	}

	// diagrams managed in BrowserPackage::read_session
	browser->get_project()->read_session(st, k);
	
	if (at_end(st) == verbose_generation())
	  toggle_verbose_generation();
      }
      catch (...) {
      }
    }
    delete [] s;
  }
}

void UmlWindow::print() {
  abort_line_construction();
#ifndef QT_NO_PRINTER
  DiagramWindow * m = ((DiagramWindow *) ws->activeWindow());
  if (m) {
    QPrinter printer;

    if (printer.setup(this)) {
      QPainter paint(&printer);
      QApplication::setOverrideCursor(Qt::waitCursor);
      m->get_view()->print(&paint);
      QApplication::restoreOverrideCursor();
    }
  }
#endif
}

void UmlWindow::edit_gen_settings() {
  browser->get_project()->edit_gen_settings();
}

void UmlWindow::edit_stereotypes() {
  browser->get_project()->edit_stereotypes();
}

void UmlWindow::edit_class_settings() {
  browser->get_project()->edit_class_settings();
}

void UmlWindow::edit_drawing_settings() {
  browser->get_project()->edit_drawing_settings();
}

void UmlWindow::use_cpp() {
  GenerationSettings::cpp_set_default_defs(!GenerationSettings::cpp_get_default_defs());
}

void UmlWindow::use_java() {
  GenerationSettings::java_set_default_defs(!GenerationSettings::java_get_default_defs());
}

void UmlWindow::use_idl() {
  GenerationSettings::idl_set_default_defs(!GenerationSettings::idl_get_default_defs());
}

void UmlWindow::verbose() {
  toggle_verbose_generation();
}

void UmlWindow::preserve() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0) {
    if (!prj->is_writable())
      msg_critical("Bouml", "Unchanged : project is read-only");
    else {
      toggle_preserve_bodies();
      if (preserve_bodies() && add_operation_profile())
	toggle_add_operation_profile();
      prj->modified();
    }
  }
}

void UmlWindow::addoperationprofile() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0) {
    if (!prj->is_writable())
      msg_critical("Bouml", "Unchanged : project is read-only");
    else {
      toggle_add_operation_profile();
      if (add_operation_profile() && preserve_bodies()) {
	toggle_preserve_bodies();
	msg_critical("Bouml",
		     "Warning : <i>Preserve operations's body</i> toggle is cleared !"
		     "<br><br>"
		     "Next code generations will replace operations's body");
      }
      prj->modified();
    }
  }
}

void UmlWindow::edit_shortcuts() {
  ShortcutDialog d;
  
  d.exec();
}

void UmlWindow::miscMenuAboutToShow() {
  abort_line_construction();
  
  miscMenu->setItemChecked(use_cpp_id,
			   GenerationSettings::cpp_get_default_defs());
  miscMenu->setItemChecked(use_java_id,
			   GenerationSettings::java_get_default_defs());
  miscMenu->setItemChecked(use_idl_id,
			   GenerationSettings::idl_get_default_defs());
  miscMenu->setItemChecked(verbose_gen_id, verbose_generation());
  
  BrowserPackage * prj = browser->get_project();
  bool enabled = (prj != 0);
  
  miscMenu->setItemEnabled(use_cpp_id, enabled);
  miscMenu->setItemEnabled(use_java_id, enabled);
  miscMenu->setItemEnabled(use_idl_id, enabled);
  miscMenu->setItemEnabled(verbose_gen_id, enabled);
  if (enabled) {
    miscMenu->setItemChecked(preserve_bodies_id, preserve_bodies());
    miscMenu->setItemChecked(add_operation_profile_id, add_operation_profile());
  }
  miscMenu->setItemEnabled(preserve_bodies_id, enabled && prj->is_writable());
  miscMenu->setItemEnabled(add_operation_profile_id, enabled && prj->is_writable());
  miscMenu->setItemEnabled(shortcut_id, enabled);
  miscMenu->setItemEnabled(show_browser_stereotypes_id, enabled);
}

void UmlWindow::fontSizeMenuAboutToShow() {
  abort_line_construction();
  
  int ps = NormalFont.pointSize();

  for (int i = FONTSIZEMIN; i < FONTSIZESUP; i += 1) {
    fontSizeMenu->setItemChecked(i, i == ps);
    fontSizeMenu->setWhatsThis(i, fontSizeMenuText);
  }
}

void UmlWindow::setFontSize(int i) {
  if (i == NormalFont.pointSize())
    return;
  
  BrowserNode * prj = browser->get_project();
  
  if (prj != 0) {
    if (!prj->is_writable() &&
	(msg_warning("Bouml",
		     "Project file is read-only, new font "
		     "size will not be saved, continue ?\n",
		     QMessageBox::Yes, QMessageBox::No)
	 != QMessageBox::Yes))
      return;
    
    prj->package_modified();
    
    
    resize_font(i);
    
    // force browser redisplay
    
    bool is_open = prj->isOpen();
    
    prj->setOpen(!is_open);
    prj->setOpen(is_open);
    
    // force diagrams redisplay
    QWidgetList windows = ws->windowList();
    
    for (i = 0; i < int(windows.count()); ++i) {
      DiagramView * view = ((DiagramWindow *) windows.at(i))->get_view();
      double z = view->the_canvas()->zoom();
      
      view->set_zoom(z + 0.1);
      view->set_zoom(z);
    }
    
    DrawingSettings::modified();
  }
}

void UmlWindow::formatMenuAboutToShow() {
  abort_line_construction();
  
  for (int i = IsoA0; i != CanvasFormatSup; i += 1) {
    formatMenu->setItemChecked(i, i == (int) format);
    formatMenu->setWhatsThis(i, formatMenuText);
  }
}

void UmlWindow::setFormat(int i) {
  BrowserNode * prj = browser->get_project();
  
  if (prj != 0) {
    if (!prj->is_writable() &&
	(msg_warning("Bouml",
		     "Project file is read-only, default "
		     "format will not be saved, continue ?\n",
		     QMessageBox::Yes, QMessageBox::No)
	 != QMessageBox::Yes))
      return;
    
    format = (CanvasFormat) i;
    
    prj->package_modified();
  }
}

void UmlWindow::set_default_format(CanvasFormat f)
{
  the->format = f;
}


void UmlWindow::browser_search() {
  abort_line_construction();
  if (browser->get_project() != 0) {
    BrowserSearchDialog dialog;
    
    dialog.exec();
  }
}

void UmlWindow::show_stereotypes() {
  miscMenu->setItemChecked(show_browser_stereotypes_id,
			   BrowserNode::toggle_show_stereotypes());
}

void UmlWindow::motif_style() {
#ifndef QT_NO_STYLE_MOTIF
  QApplication::setStyle(new QMotifStyle);
  style = 'm';
#endif
}

void UmlWindow::motifplus_style() {
#ifndef QT_NO_STYLE_MOTIFPLUS
  QApplication::setStyle(new QMotifPlusStyle);
  style = '+';
#endif
}

//void UmlWindow::sgi_style() {
//  QApplication::setStyle(new QSGIStyle);
//}
//
//void UmlWindow::cde_style() {
//  QApplication::setStyle(new QCDEStyle);
//}

void UmlWindow::windows_style() {
#ifndef QT_NO_STYLE_WINDOWS
  QApplication::setStyle(new QWindowsStyle);
  style = 'w';
#endif
}


void UmlWindow::show_trace() {
  TraceDialog::show_it();
}

void UmlWindow::cpp_generate() {
  BrowserPackage * prj = browser->get_project();
  bool preserve = preserve_bodies();
  
  if (prj != 0)
    ToolCom::run((verbose_generation()) 
		 ? ((preserve) ? "cpp_generator -v -p" : "cpp_generator -v")
		 : ((preserve) ? "cpp_generator -p" : "cpp_generator"),
		 prj);
}

void UmlWindow::java_generate() {
  BrowserPackage * prj = browser->get_project();
  bool preserve = preserve_bodies();
  
  if (prj != 0)
    ToolCom::run((verbose_generation()) 
		 ? ((preserve) ? "java_generator -v -p" : "java_generator -v")
		 : ((preserve) ? "java_generator -p" : "java_generator"),
		 prj);
}

void UmlWindow::idl_generate() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run((verbose_generation()) ? "idl_generator -v" : "idl_generator",
		 prj);
}

void UmlWindow::java_catalog() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run("java_catalog", prj);
}

#if 0
void UmlWindow::doc_generate() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run("doc_generator", prj);
}
#endif

void UmlWindow::cpp_reverse() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run("cpp_reverse", prj);
}

void UmlWindow::java_reverse() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run("java_reverse", prj);
}

void UmlWindow::plug_out_upgrade() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run("plug_out_upgrade", prj);
}

void UmlWindow::run_tool(int param) {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run(Tool::command(param), prj);
}

void UmlWindow::tool_settings() {
  ToolDialog dlg;
  
  if (dlg.exec()== QDialog::Accepted) {
    browser->get_project()->modified();
  }
}

void UmlWindow::import_tool_settings() {
  if (Tool::import())
    browser->get_project()->modified();
}

void UmlWindow::about() {
  AboutDialog d;
  
  d.exec();
}

void UmlWindow::aboutQt() {
  QMessageBox::aboutQt(this, "Bouml");
}

void UmlWindow::windowsMenuAboutToShow() {
  abort_line_construction();
  
  windowsMenu->clear();
  
  int cascadeId = windowsMenu->insertItem("&Cascade", ws, SLOT(cascade()));
  int tileId = windowsMenu->insertItem("&Tile", ws, SLOT(tile()));
  int preferredId = windowsMenu->insertItem("&Preferred geometry", this, SLOT(preferred_geometry()));
  
  QWidgetList windows = ws->windowList();
  
  if (windows.isEmpty()) {
    windowsMenu->setItemEnabled(cascadeId, FALSE);
    windowsMenu->setItemEnabled(tileId, FALSE);
    windowsMenu->setItemEnabled(preferredId, FALSE);
  }
  
  windowsMenu->insertSeparator();
  
  for (int i = 0; i < int(windows.count()); ++i) {
    int id = windowsMenu->insertItem(windows.at(i)->caption(),
				     this, SLOT(windowsMenuActivated(int)));
    windowsMenu->setItemParameter(id, i);
    windowsMenu->setItemChecked(id, ws->activeWindow() == windows.at(i));
  }
}

void UmlWindow::preferred_geometry() {
  QWidgetList l = the->ws->windowList();
  QWidget * w;
  
  for (w = l.first(); w != 0; w = l.next())
    ((DiagramWindow *) w)->get_view()->preferred_size_zoom();
}

void UmlWindow::windowsMenuActivated(int id) {
  QWidget* w = ws->windowList().at(id);
  
  if (w) {
    w->showNormal();
    w->setFocus();
  }
}

void UmlWindow::abort_line_construction() {
  QWidgetList l = the->ws->windowList();
  QWidget * w;
  
  for (w = l.first(); w != 0; w = l.next())
    ((DiagramWindow *) w)->get_view()->abort_line_construction();
}

//

static bool OnHistoric = FALSE;
static QValueList<BrowserNode *>::Iterator HistoricIterator;

void UmlWindow::clear_select_historic()
{
  the->select_historic.clear();
  HistoricIterator = the->select_historic.end();
  
  the->prev->setPixmap(*leftUnavailablePixmap);
  the->next->setPixmap(*rightUnavailablePixmap);
}

void UmlWindow::is_selected(BrowserNode * bn) {
  if (OnHistoric)
    OnHistoric = FALSE;
  else if ((HistoricIterator == select_historic.end()) ||
	   (*HistoricIterator != bn)) {
    ++HistoricIterator;
    while (HistoricIterator != select_historic.end())
      HistoricIterator = select_historic.remove(HistoricIterator);
    
    if (select_historic.count() == 50)
      select_historic.remove(select_historic.first());
    
    select_historic.append(bn);
    HistoricIterator = select_historic.end();
    --HistoricIterator;
    
    prev->setPixmap((HistoricIterator != select_historic.begin())
		    ? *leftPixmap : *leftUnavailablePixmap);
    next->setPixmap(*rightUnavailablePixmap);
  }
}

void UmlWindow::next_select() {
  if (++HistoricIterator != select_historic.end()) {
    OnHistoric = TRUE;
    browser->select(*HistoricIterator);
    
    the->prev->setPixmap(*leftPixmap);
    ++HistoricIterator;
    the->next->setPixmap((HistoricIterator != select_historic.end())
			 ? *rightPixmap : *rightUnavailablePixmap);
  }
  
  --HistoricIterator; 
}

void UmlWindow::prev_select() {
  if (HistoricIterator != select_historic.begin()) {
    OnHistoric = TRUE;
    browser->select(*--HistoricIterator);
    
    the->prev->setPixmap((HistoricIterator != select_historic.begin())
			 ? *leftPixmap : *leftUnavailablePixmap);
    the->next->setPixmap(*rightPixmap);  
  }
}
