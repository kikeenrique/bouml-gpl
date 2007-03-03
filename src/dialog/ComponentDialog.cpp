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

#include <qgrid.h> 
#include <qvbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h> 
#include <qcheckbox.h> 

#include "ComponentDialog.h"
#include "BrowserComponent.h"
#include "SimpleData.h"
#include "BrowserClass.h"
#include "UmlWindow.h"
#include "UmlDesktop.h"
#include "DialogUtil.h"
#include "KeyValueTable.h"
#include "ListBoxBrowserNode.h"
#include "strutil.h"
#include "UmlPixmap.h"
#include "BodyDialog.h"

QSize ComponentDialog::previous_size;

ComponentDialog::ComponentDialog(SimpleData * nd)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), data(nd) {
  nd->get_browser_node()->edit_start();
  
  if (nd->get_browser_node()->is_writable())
    setCancelButton();
  else {
    setOkButton(QString::null);
    setCancelButton("Close");
  }

  setCaption("component dialog");
    
  BrowserComponent * bn = (BrowserComponent *) nd->get_browser_node();
  const QValueList<BrowserClass *> & rq = bn->get_required_classes();
  const QValueList<BrowserClass *> & rz = bn->get_realized_classes();
  const QValueList<BrowserClass *> & pr = bn->get_provided_classes();
  QValueList<BrowserClass *>::ConstIterator it;
    
  d_associated.resize(47);
  
  for (it = rq.begin(); it != rq.end(); ++it)
    d_associated.insert(*it, *it);
  for (it = rz.begin(); it != rz.end(); ++it)
    d_associated.insert(*it, *it);
  for (it = pr.begin(); it != pr.end(); ++it)
    d_associated.insert(*it, *it);
    
  init_uml_tab();
  init_rq_tab();
  init_pr_rz_tab();
  
  // USER : list key - value
  
  QGrid * grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  kvtable = new KeyValuesTable((BrowserComponent *) data->get_browser_node(),
			       grid, !hasOkButton());
  addTab(grid, "Properties");
}

void ComponentDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::setsize_center(this, previous_size, 0.8, 0.8);
}

ComponentDialog::~ComponentDialog() {
  data->get_browser_node()->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
}
    
void ComponentDialog::init_uml_tab() {
  bool visit = !hasOkButton();  
  
  BrowserComponent * bn = (BrowserComponent *) data->get_browser_node();
  QVBox * vbox;
  QGrid * grid = new QGrid(2, this);
  
  grid->setMargin(5);
  grid->setSpacing(5);

  new QLabel("name : ", grid);
  edname = new LineEdit(bn->get_name(), grid);
  edname->setReadOnly(visit);
    
  new QLabel("stereotype : ", grid);
  edstereotype = new QComboBox(TRUE, grid);
  edstereotype->insertItem(toUnicode(data->get_stereotype()));
  if (! visit) {
    edstereotype->insertStringList(BrowserComponent::default_stereotypes());
    edstereotype->setAutoCompletion(TRUE);
  }
  edstereotype->setCurrentItem(0);
  QSizePolicy sp = edstereotype->sizePolicy();
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);
    
  vbox = new QVBox(grid);
  new QLabel("description :", vbox);
  if (! visit)
    connect(new SmallPushButton("Editor", vbox), SIGNAL(clicked()),
	    this, SLOT(edit_description()));
  comment = new MultiLineEdit(grid);
  comment->setReadOnly(visit);
  comment->setText(bn->get_comment());
  QFont font = comment->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  comment->setFont(font);
  
  addTab(grid, "Uml");
}

void ComponentDialog::init_rq_tab() {
  bool visit = !hasOkButton();  
  QHBox * hbox;
  QVBox * vbox;  
  QPushButton * button;
  BrowserComponent * bn = (BrowserComponent *) data->get_browser_node();
  
  rq_page = new QVBox(this);
  
  if (!visit) {
    hbox = new QHBox(rq_page);
    hbox->setMargin(5);
    new QLabel("Stereotype filtering  ", hbox);
    rq_stereotypefilter = new QComboBox(TRUE, hbox);
    rq_stereotypefilter->setAutoCompletion(TRUE);
    rq_stereotypefilter->insertItem("");
    rq_stereotypefilter->insertStringList(BrowserClass::default_stereotypes());
    rq_stereotypefilter->setCurrentItem(0);
    QSizePolicy sp = rq_stereotypefilter->sizePolicy();
    sp.setHorData(QSizePolicy::Expanding);
    rq_stereotypefilter->setSizePolicy(sp);
    connect(rq_stereotypefilter, SIGNAL(activated(const QString &)),
	    this, SLOT(rq_stereotypeFilterActivated(const QString &)));
    
    hbox = new QHBox(rq_page);
    vbox = new QVBox(hbox);
    vbox->setMargin(5);
    (new QLabel("Available classes", vbox))->setAlignment(AlignCenter);
    lb_rq_available = new QListBox(vbox);
    lb_rq_available->setSelectionMode(QListBox::Multi);
    
    rq_stereotypeFilterActivated(QString::null);
    
    vbox = new QVBox(hbox);
    vbox->setMargin(5);
    (new QLabel("", vbox))->setScaledContents(TRUE);
    button = new QPushButton(vbox);
    button->setPixmap(*rightPixmap);
    connect(button, SIGNAL(clicked()), this, SLOT(require_cls()));
    (new QLabel("", vbox))->setScaledContents(TRUE);
    button = new QPushButton(vbox);
    button->setPixmap(*leftPixmap);
    connect(button, SIGNAL(clicked()), this, SLOT(unrequire_cls()));
    (new QLabel("", vbox))->setScaledContents(TRUE);
    vbox = new QVBox(hbox);
  }
  else
    vbox = rq_page;
  
  vbox->setMargin(5);
  (new QLabel("Required classes", vbox))->setAlignment(AlignCenter);
  lb_rq = new QListBox(vbox);
  lb_rq->setSelectionMode((visit) ? QListBox::NoSelection
				  : QListBox::Multi);
  
  const QValueList<BrowserClass *> & rq = bn->get_required_classes();
  QValueList<BrowserClass *>::ConstIterator it;
  
  for (it = rq.begin(); it != rq.end(); ++it)
    if (!(*it)->deletedp())
      lb_rq->insertItem(new ListBoxBrowserNode(*it, (*it)->full_name(TRUE)));
  
  lb_rq->sort();
  
  addTab(rq_page, "Required classes");
}

void ComponentDialog::init_pr_rz_tab() {
  bool visit = !hasOkButton();  
  QHBox * hbox;
  QVBox * vbox;  
  QPushButton * button;
  BrowserComponent * bn = (BrowserComponent *) data->get_browser_node();
  
  pr_rz_page = new QVBox(this);
  
  if (!visit) {
    hbox = new QHBox(pr_rz_page);
    hbox->setMargin(5);
    new QLabel("Stereotype filtering  ", hbox);
    pr_rz_stereotypefilter = new QComboBox(TRUE, hbox);
    pr_rz_stereotypefilter->setAutoCompletion(TRUE);
    pr_rz_stereotypefilter->insertItem("");
    pr_rz_stereotypefilter->insertStringList(BrowserClass::default_stereotypes());
    pr_rz_stereotypefilter->setCurrentItem(0);
    QSizePolicy sp = pr_rz_stereotypefilter->sizePolicy();
    sp.setHorData(QSizePolicy::Expanding);
    pr_rz_stereotypefilter->setSizePolicy(sp);
    connect(pr_rz_stereotypefilter, SIGNAL(activated(const QString &)),
	    this, SLOT(pr_rz_stereotypeFilterActivated(const QString &)));
    
    hbox = new QHBox(pr_rz_page);
    vbox = new QVBox(hbox);
    vbox->setMargin(5);
    (new QLabel("Available classes", vbox))->setAlignment(AlignCenter);
    lb_pr_rz_available = new QListBox(vbox);
    lb_pr_rz_available->setSelectionMode(QListBox::Multi);
    
    pr_rz_stereotypeFilterActivated(QString::null);
    
    vbox = new QVBox(hbox);
    vbox->setMargin(5);
    (new QLabel("", vbox))->setScaledContents(TRUE);
    button = new QPushButton(vbox);
    button->setPixmap(*rightPixmap);
    connect(button, SIGNAL(clicked()), this, SLOT(provide_cls()));
    (new QLabel("", vbox))->setScaledContents(TRUE);
    button = new QPushButton(vbox);
    button->setPixmap(*leftPixmap);
    connect(button, SIGNAL(clicked()), this, SLOT(unprovide_cls()));
    (new QLabel("", vbox))->setScaledContents(TRUE);

    (new QLabel("", vbox))->setScaledContents(TRUE);
    button = new QPushButton(vbox);
    button->setPixmap(*rightPixmap);
    connect(button, SIGNAL(clicked()), this, SLOT(realize_cls()));
    (new QLabel("", vbox))->setScaledContents(TRUE);
    button = new QPushButton(vbox);
    button->setPixmap(*leftPixmap);
    connect(button, SIGNAL(clicked()), this, SLOT(unrealize_cls()));
    (new QLabel("", vbox))->setScaledContents(TRUE);

    vbox = new QVBox(hbox);
  }
  else
    vbox = pr_rz_page;
  
  QValueList<BrowserClass *>::ConstIterator it;
  
  vbox->setMargin(5);
  
  (new QLabel("Provided classes", vbox))->setAlignment(AlignCenter);
  lb_pr = new QListBox(vbox);
  lb_pr->setSelectionMode((visit) ? QListBox::NoSelection
				  : QListBox::Multi);
  
  const QValueList<BrowserClass *> & pr = bn->get_provided_classes();
  
  for (it = pr.begin(); it != pr.end(); ++it)
    if (!(*it)->deletedp())
      lb_pr->insertItem(new ListBoxBrowserNode(*it, (*it)->full_name(TRUE)));
  
  lb_pr->sort();
  
  if (!visit) {
    QHBox * hb = new QHBox(vbox);
    
    hb->setMargin(5);
    //(new QLabel("", hb))->setScaledContents(TRUE);
    hb->setStretchFactor(new QLabel("", hb), 100);
    button = new QPushButton(hb);
    button->setPixmap(*upPixmap);
    connect(button, SIGNAL(clicked()), this, SLOT(rz_to_pr_cls()));
    hb->setStretchFactor(button, 0);
    //(new QLabel("", hb))->setScaledContents(TRUE);
    hb->setStretchFactor(new QLabel("", hb), 100);
    button = new QPushButton(hb);
    button->setPixmap(*downPixmap);
    connect(button, SIGNAL(clicked()), this, SLOT(pr_to_rz_cls()));
    hb->setStretchFactor(button, 0);
    //(new QLabel("", hb))->setScaledContents(TRUE);
    hb->setStretchFactor(new QLabel("", hb), 100);
  }
  
  (new QLabel("Realized classes", vbox))->setAlignment(AlignCenter);
  lb_rz = new QListBox(vbox);
  lb_rz->setSelectionMode((visit) ? QListBox::NoSelection
				  : QListBox::Multi);
  
  const QValueList<BrowserClass *> & rz = bn->get_realized_classes();
  
  for (it = rz.begin(); it != rz.end(); ++it)
    if (!(*it)->deletedp())
      lb_rz->insertItem(new ListBoxBrowserNode(*it, (*it)->full_name(TRUE)));
  
  lb_rz->sort();
  
  addTab(pr_rz_page, "Provided/realized classes");
}

void ComponentDialog::rq_stereotypeFilterActivated(const QString & st) {
  lb_rq_available->clear();
  
  BrowserNodeList classes;
  BrowserNode * cl;

  BrowserClass::instances(classes, st, TRUE);
  
  for (cl = classes.first(); cl != 0; cl = classes.next())
    if (d_associated.find(cl) == 0)
      lb_rq_available->insertItem(new ListBoxBrowserNode(cl, cl->full_name(TRUE)));
  
  lb_rq_available->sort();
}

void ComponentDialog::pr_rz_stereotypeFilterActivated(const QString & st) {
  lb_pr_rz_available->clear();
  
  BrowserNodeList classes;
  BrowserNode * cl;

  BrowserClass::instances(classes, st, TRUE);
  
  for (cl = classes.first(); cl != 0; cl = classes.next())
    if (d_associated.find(cl) == 0)
      lb_pr_rz_available->insertItem(new ListBoxBrowserNode(cl, cl->full_name(TRUE)));
  
  lb_rq_available->sort();
}

void ComponentDialog::require_cls() {
  unsigned int i = 0;
  
  while (i != lb_rq_available->count()) {
    QListBoxItem * item = lb_rq_available->item(i);
    
    if (item->selected()) {
      lb_rq_available->takeItem(item);
      lb_rq->insertItem(item);
      
      BrowserNode * c =
	((ListBoxBrowserNode *) item)->browser_node;
      
      d_associated.insert(c, c);
      
      unsigned int j;
      unsigned int n = lb_pr_rz_available->count();
      
      for (j = 0; j != n; j += 1) {
	item = lb_pr_rz_available->item(j);
      
	if (((ListBoxBrowserNode *) item)->browser_node == c) {
	  delete item;
	  break;
	}
      }
    }
    else
      i += 1;
  }
  
  lb_rq->sort();
}

void ComponentDialog::unrequire_cls() {
  unsigned int i = 0;
  
  while (i != lb_rq->count()) {
    QListBoxItem * item = lb_rq->item(i);
    
    if (item->selected()) {
      BrowserNode * c =
	((ListBoxBrowserNode *) item)->browser_node;
      QString s = item->text();
      
      delete item;
      d_associated.remove(c);
      
      if (rq_stereotypefilter->currentText().isEmpty() ||
	  (c->get_stereotype() == rq_stereotypefilter->currentText()))
	lb_rq_available->insertItem(new ListBoxBrowserNode(c, s));
      if (pr_rz_stereotypefilter->currentText().isEmpty() ||
	  (c->get_stereotype() == pr_rz_stereotypefilter->currentText()))
	lb_pr_rz_available->insertItem(new ListBoxBrowserNode(c, s));
    }
    else
      i += 1;
  }
  
  lb_rq_available->sort();
  lb_pr_rz_available->sort();
}

void ComponentDialog::provide_cls() {
  unsigned int i = 0;
  
  while (i != lb_pr_rz_available->count()) {
    QListBoxItem * item = lb_pr_rz_available->item(i);
    
    if (item->selected()) {
      lb_pr_rz_available->takeItem(item);
      lb_pr->insertItem(item);
      
      BrowserNode * c =
	((ListBoxBrowserNode *) item)->browser_node;
      
      d_associated.insert(c, c);
      
      unsigned int j;
      unsigned int n = lb_rq_available->count();
      
      for (j = 0; j != n; j += 1) {
	item = lb_rq_available->item(j);
      
	if (((ListBoxBrowserNode *) item)->browser_node == c) {
	  delete item;
	  break;
	}
      }
    }
    else
      i += 1;
  }
  
  lb_pr->sort();
}

void ComponentDialog::unprovide_cls() {
  unsigned int i = 0;
  
  while (i != lb_pr->count()) {
    QListBoxItem * item = lb_pr->item(i);
    
    if (item->selected()) {
      BrowserNode * c =
	((ListBoxBrowserNode *) item)->browser_node;
      QString s = item->text();
      
      delete item;
      d_associated.remove(c);
      
      if (rq_stereotypefilter->currentText().isEmpty() ||
	  (c->get_stereotype() == rq_stereotypefilter->currentText()))
	lb_rq_available->insertItem(new ListBoxBrowserNode(c, s));
      if (pr_rz_stereotypefilter->currentText().isEmpty() ||
	  (c->get_stereotype() == pr_rz_stereotypefilter->currentText()))
	lb_pr_rz_available->insertItem(new ListBoxBrowserNode(c, s));
    }
    else
      i += 1;
  }
  
  lb_rq_available->sort();
  lb_pr_rz_available->sort();
}

void ComponentDialog::realize_cls() {
  unsigned int i = 0;
  
  while (i != lb_pr_rz_available->count()) {
    QListBoxItem * item = lb_pr_rz_available->item(i);
    
    if (item->selected()) {
      lb_pr_rz_available->takeItem(item);
      lb_rz->insertItem(item);
      
      BrowserNode * c =
	((ListBoxBrowserNode *) item)->browser_node;
      
      d_associated.insert(c, c);
      
      unsigned int j;
      unsigned int n = lb_rq_available->count();
      
      for (j = 0; j != n; j += 1) {
	item = lb_rq_available->item(j);
      
	if (((ListBoxBrowserNode *) item)->browser_node == c) {
	  delete item;
	  break;
	}
      }
    }
    else
      i += 1;
  }
  
  lb_rz->sort();
}

void ComponentDialog::unrealize_cls() {
  unsigned int i = 0;
  
  while (i != lb_rz->count()) {
    QListBoxItem * item = lb_rz->item(i);
    
    if (item->selected()) {
      BrowserNode * c =
	((ListBoxBrowserNode *) item)->browser_node;
      QString s = item->text();
      
      delete item;
      d_associated.remove(c);
      
      if (rq_stereotypefilter->currentText().isEmpty() ||
	  (c->get_stereotype() == rq_stereotypefilter->currentText()))
	lb_rq_available->insertItem(new ListBoxBrowserNode(c, s));
      if (pr_rz_stereotypefilter->currentText().isEmpty() ||
	  (c->get_stereotype() == pr_rz_stereotypefilter->currentText()))
	lb_pr_rz_available->insertItem(new ListBoxBrowserNode(c, s));
    }
    else
      i += 1;
  }
  
  lb_rq_available->sort();
  lb_pr_rz_available->sort();
}

void ComponentDialog::rz_to_pr_cls() {
  unsigned int i = 0;
  
  while (i != lb_rz->count()) {
    QListBoxItem * item = lb_rz->item(i);
    
    if (item->selected()) {
      lb_rz->takeItem(item);
      lb_pr->insertItem(item);
    }
    else
      i += 1;
  }
  
  lb_pr->sort();
}

void ComponentDialog::pr_to_rz_cls() {
  unsigned int i = 0;
  
  while (i != lb_pr->count()) {
    QListBoxItem * item = lb_pr->item(i);
    
    if (item->selected()) {
      lb_pr->takeItem(item);
      lb_rz->insertItem(item);
    }
    else
      i += 1;
  }
  
  lb_rz->sort();
}

void ComponentDialog::edit_description() {
  edit(comment->text(), edname->text().stripWhiteSpace() + "_description",
       data, TxtEdit, this, (post_edit) post_edit_description, edits);
}

void ComponentDialog::post_edit_description(ComponentDialog * d, QString s) {
  d->comment->setText(s);
}

void ComponentDialog::accept() {
  if (!check_edits(edits))
    return;
    
  QString s = edname->text().stripWhiteSpace();
  BrowserComponent * bn = (BrowserComponent *) data->get_browser_node();
  
  if ((s != bn->get_name()) &&
      ((BrowserNode *) bn->parent())->wrong_child_name(s, bn->get_type(),
						       bn->allow_spaces(),
						       bn->allow_empty()))
    msg_critical("Error", edname->text() + "\n\nillegal name or already used");
  else {  
    bn->set_name(s);
    bn->set_comment(comment->text());
    UmlWindow::set_commented(bn);
            
    data->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
    
    QValueList<BrowserClass *> rq;
    QValueList<BrowserClass *> rz;
    QValueList<BrowserClass *> pr;
    
    unsigned int i;
    unsigned int n;
    
    for (i = 0, n = lb_rq->count(); i != n; i += 1)
      rq.append((BrowserClass *) ((ListBoxBrowserNode *) lb_rq->item(i))->browser_node);
    for (i = 0, n = lb_pr->count(); i != n; i += 1)
      pr.append((BrowserClass *) ((ListBoxBrowserNode *) lb_pr->item(i))->browser_node);
    for (i = 0, n = lb_rz->count(); i != n; i += 1)
      rz.append((BrowserClass *) ((ListBoxBrowserNode *) lb_rz->item(i))->browser_node);
    
    bn->set_associated_classes(rz, pr, rq);
    
    kvtable->update(bn);
  
    bn->modified();
    bn->package_modified();
    data->modified();
    
    QTabDialog::accept();
  }
}
