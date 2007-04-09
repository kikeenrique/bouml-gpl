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

#include <stdio.h>

#include <qgrid.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qpushbutton.h> 
#include <qfiledialog.h> 

#include "PackageDialog.h"
#include "PackageData.h"
#include "GenerationSettings.h"
#include "BrowserPackage.h"
#include "KeyValueTable.h"
#include "UmlWindow.h"
#include "DialogUtil.h"
#include "BrowserView.h"
#include "UmlDesktop.h"
#include "BodyDialog.h"
#include "strutil.h"

static const char * Relative = "Set it relative";
static const char * Absolute = "Set it absolute";

QSize PackageDialog::previous_size;

PackageDialog::PackageDialog(PackageData * da)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), pa(da) {
  da->browser_node->edit_start();
  
  bool visit = !da->browser_node->saveable();
  
  if (!visit)
    setCancelButton();
  else {
    setOkButton(QString::null);
    setCancelButton("Close");
  }

  setCaption("Package dialog");
    
  QGrid * grid;
  QVBox * vtab;
  QHBox * htab;
  QPushButton * button;
  
  // general tab
  
  grid = new QGrid(2, this);
  umltab = grid;
  grid->setSpacing(5);
  grid->setMargin(5);
  
  new QLabel("name : ", grid);
  edname = new LineEdit(pa->name(), grid);
  edname->setReadOnly(!da->browser_node->is_writable() ||
		      (pa->get_browser_node() == BrowserView::get_project()));
  
  new QLabel("stereotype : ", grid);
  edstereotype = new QComboBox(!visit, grid);
  edstereotype->insertItem(toUnicode(pa->stereotype));
  if (! visit) {
    edstereotype->insertStringList(BrowserPackage::default_stereotypes());
    edstereotype->setAutoCompletion(TRUE);
  }
  edstereotype->setCurrentItem(0);
  QSizePolicy sp = edstereotype->sizePolicy();
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);
  
  vtab = new QVBox(grid);
  new QLabel("description :", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_description()));
  comment = new MultiLineEdit(grid);
  comment->setReadOnly(visit);
  comment->setText(pa->get_browser_node()->get_comment());
  QFont font = comment->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  comment->setFont(font);
  
  addTab(grid, "Uml");
  
  // C++
  
  vtab = new QVBox(this);
  cpptab = vtab;
  vtab->setMargin(5);
  
  htab = new QHBox(vtab);
  htab->setMargin(5);
  new QLabel("The generation directories may be relative in case the root directory\n\
is specified (through the project menu entry 'edit generation settings')\n\n", htab);
  
  htab = new QHBox(vtab);
  htab->setMargin(5);
  QLabel * lbl1 = new QLabel("headers directory : ", htab);
  edcpphdir = new LineEdit(pa->cpp_h_dir, htab);
  if (visit)
    edcpphdir->setReadOnly(TRUE);
  else {
    htab = new QHBox(vtab);
    new QLabel("", htab);
    button = new QPushButton("Browse", htab);
    connect(button, SIGNAL(clicked ()), this, SLOT(cpph_browse()));
    new QLabel("", htab);
    cpphbutton = new QPushButton((pa->cpp_h_dir.isEmpty() || 
				  QDir::isRelativePath(pa->cpp_h_dir))
				 ? Absolute : Relative, htab);
    if (GenerationSettings::get_cpp_root_dir().isEmpty())
      cpphbutton->setEnabled(FALSE); 
    connect(cpphbutton, SIGNAL(clicked ()), this, SLOT(cpph_relative()));
    new QLabel("", htab);
  }
  
  htab = new QHBox(vtab);
  htab->setMargin(5);
  new QLabel("", htab);
  
  htab = new QHBox(vtab);
  htab->setMargin(5);
  QLabel * lbl2 = new QLabel("sources directory : ", htab);
  edcppsrcdir = new LineEdit(pa->cpp_src_dir, htab);
  if (visit)
    edcppsrcdir->setReadOnly(TRUE);
  else {
    htab = new QHBox(vtab);
    new QLabel("", htab);
    button = new QPushButton("Browse", htab);
    connect(button, SIGNAL(clicked ()), this, SLOT(cppsrc_browse()));
    new QLabel("", htab);
    cppsrcbutton =
      new QPushButton((pa->cpp_src_dir.isEmpty() || 
		       QDir::isRelativePath(pa->cpp_src_dir))
		      ? Absolute : Relative, htab);
    if (GenerationSettings::get_cpp_root_dir().isEmpty())
      cppsrcbutton->setEnabled(FALSE); 
    connect(cppsrcbutton, SIGNAL(clicked ()), this, SLOT(cppsrc_relative()));
    new QLabel("", htab);
  }
  
  htab = new QHBox(vtab);
  htab->setMargin(5);
  new QLabel("", htab);
  
  htab = new QHBox(vtab);
  htab->setMargin(5);
  QLabel * lbl3 = new QLabel("namespace : ", htab);
  edcppnamespace = new LineEdit(pa->cpp_namespace, htab);
  edcppnamespace->setReadOnly(visit);
  
  same_width(lbl1, lbl2, lbl3);
  
  vtab->setStretchFactor(new QHBox(vtab), 1000);
  
  addTab(vtab, "C++");
  
  // Java
  
  vtab = new QVBox(this);
  javatab = vtab;
  vtab->setMargin(5);
  
  htab = new QHBox(vtab);
  htab->setMargin(5);
  new QLabel("The generation directory may be relative in case the root directory\n\
is specified (through the project menu entry 'edit generation settings')\n\n", htab);
  
  htab = new QHBox(vtab);
  htab->setMargin(5);
  lbl1 = new QLabel("directory : ", htab);
  edjavadir = new LineEdit(pa->java_dir, htab);
  if (visit) 
    edjavadir->setReadOnly(TRUE);
  else {
    htab = new QHBox(vtab);
    new QLabel("", htab);
    button = new QPushButton("Browse", htab);
    connect(button, SIGNAL(clicked ()), this, SLOT(java_browse()));
    new QLabel("", htab);
    javabutton = new QPushButton((pa->java_dir.isEmpty() || 
				  QDir::isRelativePath(pa->java_dir))
				 ? Absolute : Relative, htab);
    if (GenerationSettings::get_java_root_dir().isEmpty())
      javabutton->setEnabled(FALSE); 
    connect(javabutton, SIGNAL(clicked ()), this, SLOT(java_relative()));
    new QLabel("", htab);
  }
  
  htab = new QHBox(vtab);
  htab->setMargin(5);
  new QLabel("", htab);
  
  htab = new QHBox(vtab);
  htab->setMargin(5);
  lbl2 = new QLabel("package : ", htab);
  edjavapackage = new LineEdit(pa->java_package, htab);
  edjavapackage->setReadOnly(visit);
  
  same_width(lbl1, lbl2);
  
  vtab->setStretchFactor(new QHBox(vtab), 1000);
  
  addTab(vtab, "Java");
  
  // IDL
  
  vtab = new QVBox(this);
  idltab = vtab;
  vtab->setMargin(5);
  
  htab = new QHBox(vtab);
  htab->setMargin(5);
  new QLabel("The generation directory may be relative in case the root directory\n\
is specified (through the project menu entry 'edit generation settings')\n\n", htab);
  
  htab = new QHBox(vtab);
  htab->setMargin(5);
  lbl1 = new QLabel("directory : ", htab);
  edidldir = new LineEdit(pa->idl_dir, htab);
  if (visit)
    edidldir->setReadOnly(TRUE);
  else {
    htab = new QHBox(vtab);
    new QLabel("", htab);
    button = new QPushButton("Browse", htab);
    connect(button, SIGNAL(clicked ()), this, SLOT(idl_browse()));
    new QLabel("", htab);
    idlbutton = new QPushButton((pa->idl_dir.isEmpty() || 
				 QDir::isRelativePath(pa->idl_dir))
				? Absolute : Relative, htab);
    if (GenerationSettings::get_idl_root_dir().isEmpty())
      idlbutton->setEnabled(FALSE); 
    connect(idlbutton, SIGNAL(clicked ()), this, SLOT(idl_relative()));
    new QLabel("", htab);
  }
  
  htab = new QHBox(vtab);
  htab->setMargin(5);
  new QLabel("", htab);
  
  htab = new QHBox(vtab);
  htab->setMargin(5);
  lbl2 = new QLabel("module : ", htab);
  edidlmodule = new LineEdit(pa->idl_module, htab);
  edidlmodule->setReadOnly(visit);
  
  same_width(lbl1, lbl2);
  
  vtab->setStretchFactor(new QHBox(vtab), 1000);
  
  addTab(vtab, "IDL");
  
  // USER : list key - value
  
  vtab = new QVBox(this);
  kvtable = new KeyValuesTable(pa->get_browser_node(), vtab, visit);
  addTab(vtab, "Properties");
  
  //
    
  connect(this, SIGNAL(currentChanged(QWidget *)),
	  this, SLOT(change_tabs(QWidget *)));
}

void PackageDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

PackageDialog::~PackageDialog() {
  pa->browser_node->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
}
    
void PackageDialog::change_tabs(QWidget * w) {
  if (hasOkButton()) {
    if (w == umltab)
      edname->setFocus();
    else if (w == cpptab)
      edcpphdir->setFocus();
    else if (w == javatab)
      edjavadir->setFocus();
    else if (w == idltab)
      edidldir->setFocus();
  }
}

void PackageDialog::edit_description() {
  edit(comment->text(), edname->text().stripWhiteSpace() + "_description", 
       pa, TxtEdit, this, (post_edit) post_edit_description, edits);
}

void PackageDialog::post_edit_description(PackageDialog * d, QString s)
{
  d->comment->setText(s);
}

void PackageDialog::accept() {
  if (!check_edits(edits))
    return;
    
  BrowserNode * bn = pa->browser_node;
  QString s;
  
  s = edname->text().stripWhiteSpace();
  if ((s != pa->name()) &&
      ((BrowserNode *) bn->parent())->wrong_child_name(s, UmlPackage,
								     bn->allow_spaces(),
								     bn->allow_empty()))
    msg_critical("Error", s + "\n\nillegal name or already used");
  else {  
    bn->set_name(s);
    pa->set_stereotype(fromUnicode(edstereotype->currentText().simplifyWhiteSpace()));
    pa->cpp_h_dir = edcpphdir->text().simplifyWhiteSpace();
    pa->cpp_src_dir = edcppsrcdir->text().simplifyWhiteSpace();
    pa->cpp_namespace = edcppnamespace->text().simplifyWhiteSpace();
    pa->java_dir = edjavadir->text().simplifyWhiteSpace();
    pa->java_package = edjavapackage->text().simplifyWhiteSpace();
    pa->idl_dir = edidldir->text().simplifyWhiteSpace();
    pa->idl_module = edidlmodule->text().simplifyWhiteSpace();
    
    bn->set_comment(comment->text());
    UmlWindow::set_commented(bn);
    
    kvtable->update(bn);
    
    bn->package_modified();
    pa->modified();
    
    QTabDialog::accept();
  }
}

void PackageDialog::browse(LineEdit * ed, QPushButton * button,
			   const char * caption, QString root) {
  QString s = ed->text();
  bool rel = FALSE;
  
  if (s.isNull())
    s = root;
  else if (!root.isEmpty() && QDir::isRelativePath(s)) {
    rel = TRUE;
    s = root + s;
  }
	   
  const QString ns = QFileDialog::getExistingDirectory(s, this, 0, caption);

  
  if (! ns.isNull()) {
    unsigned len = root.length();
    
    if (rel && 
        (ns.find(root) == 0) &&
        (ns.length() >= len))
      ed->setText(ns.mid(len));
    else {
      ed->setText(ns);
      button->setText(Relative);
    }
  }
}

void PackageDialog::cpph_browse() {
  browse(edcpphdir, cpphbutton, "C++ header directory",
	 GenerationSettings::get_cpp_root_dir());
}

void PackageDialog::cppsrc_browse() {
  browse(edcppsrcdir, cppsrcbutton, "C++ source directory",
	 GenerationSettings::get_cpp_root_dir());
}

void PackageDialog::java_browse() {
  browse(edjavadir, javabutton, "Java directory",
	 GenerationSettings::get_java_root_dir());
}

void PackageDialog::idl_browse() {
  browse(edidldir, idlbutton, "Idl directory",
	 GenerationSettings::get_idl_root_dir());
}

void PackageDialog::relative(LineEdit * ed, QPushButton * button,
			     QString root) {
  const QString s = ed->text();
  if (QDir::isRelativePath(root))
    root = BrowserView::get_dir().filePath(root);
  
  if (button->text() == Relative) {
    unsigned len = root.length();
      
    if (
#ifdef WIN32
	(s.lower().find(root.lower()) == 0) &&
#else
	(s.find(root) == 0) &&
#endif
	(s.length() >= len)) {
      ed->setText(s.mid(len));
      button->setText(Absolute);
    }
  }
  else {
    ed->setText(root + s);
    button->setText(Relative);
  }
}

void PackageDialog::cpph_relative() {
  relative(edcpphdir, cpphbutton, GenerationSettings::get_cpp_root_dir());
}

void PackageDialog::cppsrc_relative() {
  relative(edcppsrcdir, cppsrcbutton, GenerationSettings::get_cpp_root_dir());
}

void PackageDialog::java_relative() {
  relative(edjavadir, javabutton, GenerationSettings::get_java_root_dir());
}

void PackageDialog::idl_relative() {
  relative(edidldir, idlbutton, GenerationSettings::get_idl_root_dir());
}
