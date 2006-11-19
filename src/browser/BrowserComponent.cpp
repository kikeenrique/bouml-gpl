// *************************************************************************
//
// Copyright (C) 2004-2006 Bruno PAGES  All rights reserved.
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

#include <qpopupmenu.h> 
#include <qmessagebox.h>
#include <qcursor.h>
#include <qdir.h>

#include "BrowserComponent.h"
#include "SimpleData.h"
#include "BrowserComponentDiagram.h"
#include "BrowserSimpleRelation.h"
#include "SimpleRelationData.h"
#include "BrowserClass.h"
#include "PackageData.h"
#include "UmlPixmap.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "ClassListDialog.h"
#include "GenerationSettings.h"
#include "ReferenceDialog.h"
#include "UmlDrag.h"
#include "mu.h"
#include "ComponentDialog.h"

IdDict<BrowserComponent> BrowserComponent::all(257);
QStringList BrowserComponent::its_default_stereotypes;	// unicode

BrowserComponent::BrowserComponent(QString s, BrowserNode * p, int id)
    : BrowserNode(s, p), Labeled<BrowserComponent>(all, id) {
  def = new SimpleData;
  def->set_browser_node(this);
  
  associated_diagram = 0;
}

BrowserComponent::BrowserComponent(const BrowserComponent * model,
				   BrowserNode * p)
    : BrowserNode(model->name, p), Labeled<BrowserComponent>(all, 0) {
  def = new SimpleData(model->def);
  def->set_browser_node(this);
  comment = model->comment;
  associated_diagram = 0;  
}

BrowserComponent::BrowserComponent(int id)
    : BrowserNode(), Labeled<BrowserComponent>(all, id) {
  // not yet read
  def = new SimpleData;
  def->set_browser_node(this);
  
  associated_diagram = 0;
}

BrowserComponent::~BrowserComponent() {
  all.remove(get_ident());
  delete def;
}

BrowserNode * BrowserComponent::duplicate(BrowserNode * p, QString name) {
  BrowserComponent * result = new BrowserComponent(this, p);
  
  result->set_name((name.isEmpty()) ? get_name() : (const char *) name);
  result->update_stereotype();
  
  return result;
}

void BrowserComponent::clear(bool old)
{
  all.clear(old);
}

void BrowserComponent::update_idmax_for_root()
{
  all.update_idmax_for_root();
}
    
void BrowserComponent::referenced_by(QList<BrowserNode> & l) {
  BrowserNode::referenced_by(l);
  BrowserSimpleRelation::compute_referenced_by(l, this);
}

void BrowserComponent::compute_referenced_by(QList<BrowserNode> & l,
					     BrowserClass * target)
{
  IdIterator<BrowserComponent> it(all);
  
  while (it.current()) {
    if (!it.current()->deletedp() &&
	((it.current()->realized_classes.findIndex(target) != -1) ||
	 (it.current()->provided_classes.findIndex(target) != -1) ||
	 (it.current()->required_classes.findIndex(target) != -1)))
      l.append(it.current());
    ++it;
  }
}

void BrowserComponent::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
}

const QPixmap* BrowserComponent::pixmap(int) const {
  return (deletedp()) ? DeletedComponentIcon : ComponentIcon;
}

QString BrowserComponent::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);

  if (p.isEmpty()) 
    return QString((const char *) name);
  else if (rev)
    return name + "   [" + p + "]";
  else
    return p + "::" + name;
}

// just check if the inheritance already exist
const char * BrowserComponent::check_inherit(const BrowserNode * new_parent) const {
  QListViewItem * child;
    
  for (child = firstChild(); child != 0; child = child->nextSibling()) {
    BrowserNode * ch = ((BrowserNode *) child);
    
    if ((ch->get_type() == UmlInherit) && 
	((((SimpleRelationData *) ch->get_data())->get_end_node())
	 == new_parent))
      return "already exist";
  }
  
  return (new_parent != this) ? 0 : "circular inheritance";
}

static void make_clsubm(QPopupMenu & m, QPopupMenu & sm,
			QValueList<BrowserClass *> & l, int bias,
			bool & need_sep, QString s)
{
  int n = 0;
  QValueList<BrowserClass *>::ConstIterator it;
  
  for (it = l.begin(); it != l.end(); ++it)
    if (!(*it)->deletedp())
      n += 1;
  
  if (n == 1) {
    if (need_sep) {
      m.insertSeparator();
      need_sep = FALSE;
    }
    m.setWhatsThis(m.insertItem("Select the " + s + " class", bias+1),
		   "to select the " + s + " <em>class</em>");
  }
  else if (n > 20) {
    if (need_sep) {
      m.insertSeparator();
      need_sep = FALSE;
    }
    m.setWhatsThis(m.insertItem("Select a " + s + " class", bias),
		   "to select a " + s + " <em>class</em>");
  }
  else if (n != 0) {
    if (need_sep) {
      m.insertSeparator();
      need_sep = FALSE;
    }
    sm.insertItem(new MenuTitle("Choose class", m.font()), -1);
    sm.insertSeparator();

    for (it = l.begin(), n = bias+1; it != l.end(); ++it)
      if (!(*it)->deletedp())
	sm.insertItem((*it)->full_name(TRUE), n++);
	    
    m.setWhatsThis(m.insertItem("Select a " + s + " class", &sm),
		   "to select a " + s + " <em>class</em>");
  }
}

static bool select_associated(int rank, int bias,
			      QValueList<BrowserClass *> & l)
{
  if (rank < bias)
    return FALSE;
  
  if (rank == bias) {
    ClassListDialog dialog("Choose class", l);
    
    dialog.raise();
    if (dialog.exec() != QDialog::Accepted)
      return TRUE;
    rank = dialog.choosen() + bias+1;
  }

  QValueList<BrowserClass *>::ConstIterator it;
  int n;
  
  for (it = l.begin(), n = bias+1;
       (*it)->deletedp() || (n++ != rank);
       ++it)
    ;
  
  (*it)->select_in_browser();

  return TRUE;
}

				   
void BrowserComponent::menu() {
  QPopupMenu m(0, name);
  QPopupMenu rqsubm(0);
  QPopupMenu prsubm(0);
  QPopupMenu rzsubm(0);
  QPopupMenu toolm(0);
  BrowserNode * item_above = 0;
  
  m.insertItem(new MenuTitle(name, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!is_edited) {
      m.setWhatsThis(m.insertItem("Add nested component", 4),
		     "to add an <em>nested component</em> to the <em>component</em>");
      m.insertSeparator();
      m.setWhatsThis(m.insertItem("Edit", 0),
		     "to edit the <em>component</em>, \
a double click with the left mouse button does the same thing");
      
      item_above = (BrowserNode *) parent()->firstChild();
      if (item_above == this)
	item_above = 0;
      else {
	for (;;) {
	  BrowserNode * next = (BrowserNode *) item_above->nextSibling();
	  
	  if (next == this)
	    break;
	  item_above = (BrowserNode *) next;
	}
      }
      
      if ((item_above != 0) && 
	  (item_above->get_type() == UmlComponent) &&
	  !((BrowserNode *) item_above)->wrong_child_name(get_name(), UmlClass, TRUE, FALSE))
	m.setWhatsThis(m.insertItem(QString("Set it nested in ")
				    + item_above->get_name(),
				    5),
		       "to set it a <em>sub component</em> of the <em>component</em> above");
      if (nestedp() &&
	  !((BrowserNode *) parent()->parent())->wrong_child_name(get_name(), UmlClass, TRUE, FALSE))
	m.setWhatsThis(m.insertItem(QString("Extract it from ")
				    + ((BrowserNode *) parent())->get_name(),
				    6),
		       "to stop to be a <em>sub component</em> of its <em>super component</em>");

      if (!is_read_only && (edition_number == 0)) {
	m.insertSeparator();
	m.setWhatsThis(m.insertItem("Delete", 1),
		       "to delete the <em>component</em>. \
Note that you can undelete it after");
      }
    }
    m.insertSeparator();
    m.setWhatsThis(m.insertItem("Referenced by", 3),
		   "to know who reference the <i>component</i> \
through a relation");
    mark_menu(m, "component", 90);
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("Tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0)) {
    m.setWhatsThis(m.insertItem("Undelete", 2),
		   "to undelete the <em>component</em>");
   
    QListViewItem * child;
    
    for (child = firstChild(); child != 0; child = child->nextSibling()) {
      if (((BrowserNode *) child)->deletedp()) {
	m.setWhatsThis(m.insertItem("Undelete recursively", 22),
		       "undelete the <em>component</em> and its \
nexted <em>components</em> and <em>relations</em> \
(except if the component at the other side is also deleted)");
	break;
      }
    }
  }

  bool need_sep = TRUE;
  
  make_clsubm(m, rqsubm, required_classes, 9999, need_sep, "required");
  make_clsubm(m, prsubm, provided_classes, 19999, need_sep, "provided");
  make_clsubm(m, rzsubm, realized_classes, 29999, need_sep, "realized");
    
  int rank = m.exec(QCursor::pos());
  
  switch (rank) {
  case 0:
    open(TRUE);
    return;
  case 1:
    delete_it();
    break;
  case 2:
    undelete(FALSE);
    break;
  case 22:
    undelete(TRUE);
    break;
  case 3:
    ReferenceDialog::show(this);
    return;
  case 4:
    {
      BrowserComponent * cp = add_component(this);
      
      if (cp != 0)
	cp->select_in_browser();
    }
    break;
  case 5:
    parent()->takeItem(this);
    item_above->insertItem(this);
    item_above->setOpen(TRUE);
    break;
  case 6:
    {
      QListViewItem * p = parent();
      
      p->takeItem(this);
      moveItem(p);
    }
    break;
  default:
    if (select_associated(rank, 29999, realized_classes) ||
	select_associated(rank, 19999, provided_classes) ||
	select_associated(rank, 9999, required_classes))
      return;
    if (rank >= 100)
      ToolCom::run(Tool::command(rank - 100), this);
    else
      mark_management(rank - 90);
    return;
  }
  
  package_modified();
}

void BrowserComponent::open(bool force_edit) {
  if (!force_edit &&
      (associated_diagram != 0) &&
      !associated_diagram->deletedp())
    associated_diagram->open(FALSE);
  else if (!is_edited) {
    def->setName(get_name());
    (new ComponentDialog(def))->show();
  }
}

UmlCode BrowserComponent::get_type() const {
  return UmlComponent;
}

void BrowserComponent::DragMoveEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, BrowserSimpleRelation::drag_key(this)))
    e->accept();
  else
    ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserComponent::DragMoveInsideEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, BrowserSimpleRelation::drag_key(this)))
    e->accept();
  else
    e->ignore();
}

void BrowserComponent::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserComponent::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  BrowserNode * bn;
  
  if (((bn = UmlDrag::decode(e, BrowserSimpleRelation::drag_key(this))) != 0) &&
      (bn != after) && (bn != this)) {
    if (may_contains(bn, FALSE)) {
      BrowserNode * old = ((BrowserNode *) bn->parent());
      
      if (after)
	bn->moveItem(after);
      else {
	bn->parent()->takeItem(bn);
	insertItem(bn);
      }
      
      if (old != this) {
	old->modified();
	old->package_modified();
      }
      
      modified();
      package_modified();
    }
    else {
      QMessageBox::critical(0, "Error", "Forbiden");
      e->ignore();
    }
  }
  else if ((bn == 0) && (after == 0))
    ((BrowserNode *) parent())->DropAfterEvent(e, this);
  else
    e->ignore();
}

BrowserComponent * BrowserComponent::get_component(BrowserNode * future_parent,
						   bool existing)
{
  BrowserNode * old;
  QString name;
  BrowserNodeList nodes;
  
  if (!future_parent->enter_child_name(name, 
				       (existing) ? "choose existing component"
						  : "enter component's name : ",
				       UmlComponent, instances(nodes), &old,
				       TRUE, FALSE, existing))
    return 0;

  if (old != 0)
    return ((BrowserComponent *) old);
  
  future_parent->setOpen(TRUE);
  future_parent->package_modified();
  
  return new BrowserComponent(name, future_parent);
}

BrowserComponent * BrowserComponent::add_component(BrowserNode * future_parent)
{
  QString name;
  
  if (!future_parent->enter_child_name(name, "enter component's name : ",
				       UmlComponent, TRUE, FALSE))
    return 0;
  
  future_parent->setOpen(TRUE);
  future_parent->package_modified();
  return new BrowserComponent(name, future_parent);
}

BasicData * BrowserComponent::get_data() const {
  return def;
}

BrowserNodeList & BrowserComponent::instances(BrowserNodeList & result,
		  			      const char * st, bool non_nested) {
  IdIterator<BrowserComponent> it(all);
  
  if ((st == 0) || (*st == 0)) {
    while (it.current() != 0) {
      if (!it.current()->deletedp() &&
	  (!non_nested || !it.current()->nestedp()))
	result.append(it.current());
      ++it;
    }
  }
  else {
    while (it.current() != 0) {
      if (!it.current()->deletedp() &&
	  (!non_nested || !it.current()->nestedp()) &&
	  !strcmp(it.current()->get_data()->get_stereotype(), st))
	result.append(it.current());
      ++it;
    }
  }
  
  result.sort();
  
  return result;
}

BrowserNode * BrowserComponent::get_associated() const {
  return associated_diagram;
}

void BrowserComponent::set_associated_diagram(BrowserComponentDiagram * dg,
					      bool on_read) {
  if (associated_diagram != dg) {
    if (associated_diagram != 0)
      QObject::disconnect(associated_diagram->get_data(), SIGNAL(deleted()),
			  def, SLOT(on_delete()));
    if ((associated_diagram = dg) != 0)
      QObject::connect(associated_diagram->get_data(), SIGNAL(deleted()),
		       def, SLOT(on_delete()));
    
    if (!on_read)
      package_modified();
  }
}

void BrowserComponent::on_delete() {
  if (associated_diagram && associated_diagram->deletedp())
    associated_diagram = 0;
  
  QValueList<BrowserClass *>::Iterator it;
  
  it = realized_classes.begin();
  while (it != realized_classes.end()) {
    if ((*it)->deletedp())
      it = realized_classes.remove(it);
    else
      ++it;
  }
  
  it = provided_classes.begin();
  while (it != provided_classes.end()) {
    if ((*it)->deletedp())
      it = provided_classes.remove(it);
    else
      ++it;
  }
  
  it = required_classes.begin();
  while (it != required_classes.end()) {
    if ((*it)->deletedp())
      it = required_classes.remove(it);
    else
      ++it;
  }
}

void BrowserComponent::remove_associated_class(BrowserClass * c) {
  QValueList<BrowserClass *>::Iterator it;
  
  if ((it = realized_classes.find(c)) != realized_classes.end()) {
    realized_classes.remove(it);
    QObject::disconnect(c->get_data(), SIGNAL(deleted()),
			def, SLOT(on_delete()));
    package_modified();
  }
  else if ((it = provided_classes.find(c)) != provided_classes.end()) {
    provided_classes.remove(it);
    QObject::disconnect(c->get_data(), SIGNAL(deleted()),
			def, SLOT(on_delete()));
    package_modified();
  } 
  else if ((it = required_classes.find(c)) != required_classes.end()) {
    required_classes.remove(it);
    QObject::disconnect(c->get_data(), SIGNAL(deleted()),
			def, SLOT(on_delete()));
    package_modified();
  }
}

QValueList<BrowserClass *> 
BrowserComponent::get_all_provided_classes(bool sorted) const {
  QValueList<BrowserClass *> r = provided_classes;
  QListViewItem * child;
    
  for (child = firstChild(); child != 0; child = child->nextSibling())
    if (((BrowserNode *) child)->get_type() == UmlComponent)
      r += ((BrowserComponent *) child)->provided_classes;

  if (sorted) {
    BrowserNodeList nl;
    QValueList<BrowserClass *>::Iterator it;

    for (it = r.begin() ; it != r.end(); it++)
      nl.append(*it);

    r.clear();
    nl.sort();

    while (! nl.isEmpty())
      r.append((BrowserClass *) nl.take(0));
  }

  return r;
}

QValueList<BrowserClass *>
BrowserComponent::get_all_required_classes(bool sorted) const {
  QValueList<BrowserClass *> pr = get_all_provided_classes(FALSE);
  QValueList<BrowserClass *> r = required_classes;
  QListViewItem * child;
    
  for (child = firstChild(); child != 0; child = child->nextSibling())
    if (((BrowserNode *) child)->get_type() == UmlComponent)
      r += ((BrowserComponent *) child)->required_classes;

  QValueList<BrowserClass *>::Iterator it;

  if (sorted) {
    BrowserNodeList nl;

    for (it = r.begin() ; it != r.end(); it++)
      if (pr.findIndex(*it) == -1)
	nl.append(*it);

    r.clear();
    nl.sort();

    while (! nl.isEmpty())
      r.append((BrowserClass *) nl.take(0));
  }
  else {
    it = r.begin();

    while (it != r.end()) {
      if (pr.findIndex(*it) == -1)
	it++;
      else
	it = r.remove(it);
    }
  }

  return r;
}

void BrowserComponent::set_associated_classes(const QValueList<BrowserClass *> & rz,
					      const QValueList<BrowserClass *> & pr,
					      const QValueList<BrowserClass *> & rq,
					      bool on_read) {
  QValueList<BrowserClass *>::Iterator it;
    
  if (! on_read) {
    // manage removed classes
    it = realized_classes.begin();
    
    while (it != realized_classes.end()) {
      if (rz.findIndex(*it) == -1) {
	// stop to be realized
	BrowserClass * cl = *it;
	
	it = realized_classes.remove(it);
	if (pr.findIndex(*it) == -1) {
	  cl->remove_associated_component(this);
	  
	  if (rq.findIndex(*it) == -1)
	    QObject::disconnect(cl->get_data(), SIGNAL(deleted()),
				def, SLOT(on_delete()));
	}
      }
      else
	++it;
    }
    
    it = provided_classes.begin();
    
    while (it != provided_classes.end()) {
      if (pr.findIndex(*it) == -1) {
	// stop to be provided
	BrowserClass * cl = *it;
	
	it = provided_classes.remove(it);
	if (rz.findIndex(*it) == -1) {
	  cl->remove_associated_component(this);
	  
	  if (rq.findIndex(*it) == -1)
	    QObject::disconnect(cl->get_data(), SIGNAL(deleted()),
				def, SLOT(on_delete()));
	}
      }
      else
	++it;
    }
    
    it = required_classes.begin();
    
    while (it != required_classes.end()) {
      if (rq.findIndex(*it) == -1) {
	// stop to be required
	BrowserClass * cl = *it;
	
	it = required_classes.remove(it);
	if ((rz.findIndex(*it) == -1) && (pr.findIndex(*it) == -1))
	  QObject::disconnect(cl->get_data(), SIGNAL(deleted()),
			      def, SLOT(on_delete()));
      }
      else
	++it;
    }
  }
  
  // manage added classes
  realized_classes = rz;
  for (it = realized_classes.begin(); it != realized_classes.end(); ++it) {
    BrowserClass * c = *it;
    
    if (c->get_associated_components().findIndex(this) == -1) {
      c->add_associated_component(this);
      QObject::connect(c->get_data(), SIGNAL(deleted()),
		       def, SLOT(on_delete()));
    }
  }
  provided_classes = pr;
  for (it = provided_classes.begin(); it != provided_classes.end(); ++it) {
    BrowserClass * c = *it;
    
    if (c->get_associated_components().findIndex(this) == -1) {
      c->add_associated_component(this);
      QObject::connect(c->get_data(), SIGNAL(deleted()),
		       def, SLOT(on_delete()));
    }
  }
  required_classes = rq;
  for (it = required_classes.begin(); it != required_classes.end(); ++it) {
    BrowserClass * c = *it;
    
    if (c->get_associated_components().findIndex(this) == -1)
      QObject::connect(c->get_data(), SIGNAL(deleted()),
		       def, SLOT(on_delete()));
  }
  
  if (!on_read)
    package_modified();
}

void BrowserComponent::init()
{
  its_default_stereotypes.clear();
  its_default_stereotypes.append("build component");
  its_default_stereotypes.append("entity");
  its_default_stereotypes.append("implement");
  its_default_stereotypes.append("process");
  its_default_stereotypes.append("service");
  its_default_stereotypes.append("subsystem");
}

// unicode
const QStringList & BrowserComponent::default_stereotypes()
{
  return its_default_stereotypes;
}

bool BrowserComponent::tool_cmd(ToolCom * com, const char * args) {
  switch ((unsigned char) args[-1]) {
  case getDefCmd:
  case getUmlDefCmd:
  case getCppDefCmd:
  case getJavaDefCmd:
  case getIdlDefCmd:
    BrowserNode::tool_cmd(com, args);
    if (com->api_format() > 13) {
      QValueList<BrowserClass *>::Iterator it;
      
      com->write_unsigned(realized_classes.count());
      
      for (it = realized_classes.begin(); it != realized_classes.end(); ++it)
	(*it)->write_id(com);
      
      com->write_unsigned(provided_classes.count());
      
      for (it = provided_classes.begin(); it != provided_classes.end(); ++it)
	(*it)->write_id(com);

      com->write_unsigned(required_classes.count());
      
      for (it = required_classes.begin(); it != required_classes.end(); ++it)
	(*it)->write_id(com);
    }
    
    return TRUE;
  default:
    if (((unsigned char) args[-1]) >= firstSetCmd) {
      if (is_read_only && !root_permission())
	com->write_ack(FALSE);
      else {
	switch ((unsigned char) args[-1]) {
	case setAssocDiagramCmd:
	  set_associated_diagram((BrowserComponentDiagram *) com->get_id(args));
	  break;
	case createCmd:
	  {
	    bool ok = TRUE;
	    
	    switch (com->get_kind(args)) {
	    case UmlSimpleRelations:
	      {
		UmlCode c;
		
		if (!com->get_relation_kind(c, args))
		  ok = FALSE;
		else {
		  BrowserNode * end = (BrowserNode *) com->get_id(args);
		  
		  switch (c) {
		  case UmlDependency:
		    switch (end->get_type()) {
		    case UmlPackage:
		    case UmlComponent:
		      add_relation(UmlDependOn, end)->get_browser_node()->write_id(com);
		      break;
		    default:
		      ok = FALSE;
		    }
		    break;
		  case UmlGeneralisation:
		    if ((end->get_type() == UmlComponent) &&
			(check_inherit(end) == 0))
		      add_relation(UmlInherit, end)->get_browser_node()->write_id(com);
		    else
		      ok = FALSE;
		    break;
		  default:
		    ok = FALSE;
		  }
		}
	      }
	      break;
	    default:
	      ok = FALSE;
	    }
	    
	    if (! ok)
	      com->write_id(0);
	    else
	      package_modified();
	    
	    break;
	  }
	case setAssocClassesCmd:
	  {
	    // check redondency
	    QValueList<BrowserClass *> rz;
	    QValueList<BrowserClass *> pr;
	    QValueList<BrowserClass *> rq;
	    QValueList<BrowserClass *>::ConstIterator it;
	    unsigned n;
	    
	    n = com->get_unsigned(args);
	    while (n--)
	      rz.append((BrowserClass *) com->get_id(args));
	    
	    n = com->get_unsigned(args);
	    while (n--)
	      pr.append((BrowserClass *) com->get_id(args));
	    
	    n = com->get_unsigned(args);
	    while (n--)
	      rq.append((BrowserClass *) com->get_id(args));
	    
	    it = rz.begin();
	    while (it != rz.end()) {
	      BrowserClass * cl = *it;
	      
	      if ((pr.find(it, cl) != pr.end()) ||
		  (rq.find(it, cl) != rq.end()) ||
		  (rz.find(++it, cl) != rz.end())) {
		com->write_ack(FALSE);
		return TRUE;
	      }
	    }
	    
	    it = pr.begin();
	    while (it != pr.end()) {
	      BrowserClass * cl = *it;
	      
	      if ((rz.find(it, cl) != rz.end()) ||
		  (rq.find(it, cl) != rq.end()) ||
		  (pr.find(++it, cl) != pr.end())) {
		com->write_ack(FALSE);
		return TRUE;
	      }
	    }
	    
	    it = rq.begin();
	    while (it != rq.end()) {
	      BrowserClass * cl = *it;
	      
	      if ((rz.find(it, cl) != rz.end()) ||
		  (pr.find(it, cl) != pr.end()) ||
		  (rq.find(++it, cl) != rq.end())) {
		com->write_ack(FALSE);
		return TRUE;
	      }
	    }
	    
	    set_associated_classes(rz, pr, rq);
	    com->write_ack(TRUE);
	  }
	  return TRUE;
	default:
	  return (def->tool_cmd(com, args, this, comment) ||
		  BrowserNode::tool_cmd(com, args));
	}
      }
    }
    else
      return (def->tool_cmd(com, args, this, comment) ||
	      BrowserNode::tool_cmd(com, args));
  }
  
  return TRUE;
}

void BrowserComponent::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "component_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserComponent::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "component_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
  else
    init();
}

void BrowserComponent::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "component_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "component " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    def->save(st, warning);
    
    if (associated_diagram != 0) {
      if (associated_diagram->deletedp()) {
	warning += QString("<p>component <b>") + full_name() +
	  "</b>'s associated diagram <b>" +
	    associated_diagram->full_name() + "</b> is deleted\n";
      }
      else {
	nl_indent(st);
	st << "associated_diagram ";
	associated_diagram->save(st, TRUE, warning);
      }
    }
    
    QValueList<BrowserClass *>::ConstIterator it;

    if (! realized_classes.isEmpty()) {
      nl_indent(st);
      st << "realized_classes";
      indent(+1);
      
      for (it = realized_classes.begin(); it != realized_classes.end(); ++it) {
	if ((*it)->deletedp())
	  warning += QString("<p>component <b>") + full_name() +
	    "</b>'s realized class <b>" +
	      (*it)->full_name() + "</b> is deleted\n";
	else {
	  nl_indent(st);
	  (*it)->save(st, TRUE, warning);
	}
      }
      indent(-1);
      nl_indent(st);
      st << "end";
    }
    
    if (! provided_classes.isEmpty()) {
      nl_indent(st);
      st << "provided_classes";
      indent(+1);
      
      for (it = provided_classes.begin(); it != provided_classes.end(); ++it) {
	if ((*it)->deletedp())
	  warning += QString("<p>component <b>") + full_name() +
	    "</b>'s provided class <b>" +
	      (*it)->full_name() + "</b> is deleted\n";
	else {
	  nl_indent(st);
	  (*it)->save(st, TRUE, warning);
	}
      }
      indent(-1);
      nl_indent(st);
      st << "end";
    }
    
    if (! required_classes.isEmpty()) {
      nl_indent(st);
      st << "required_classes";
      indent(+1);
      
      for (it = required_classes.begin(); it != required_classes.end(); ++it) {
	if ((*it)->deletedp())
	  warning += QString("<p>component <b>") + full_name() +
	    "</b>'s required class <b>" +
	      (*it)->full_name() + "</b> is deleted\n";
	else {
	  nl_indent(st);
	  (*it)->save(st, TRUE, warning);
	}
      }
      indent(-1);
      nl_indent(st);
      st << "end";
    }
    
    BrowserNode::save(st);
    
    // saves the sub elts
    
    QListViewItem * child = firstChild();
    
    if (child != 0) {
      for (;;) {
	if (! ((BrowserNode *) child)->deletedp()) {
	  ((BrowserNode *) child)->save(st, FALSE, warning);
	  child = child->nextSibling();
	  if (child != 0)
	    st << '\n';
	  else
	    break;
	}
	else if ((child = child->nextSibling()) == 0)
	  break;
      }
    }

    indent(-1);
    nl_indent(st);
    st << "end";
    
    // for saveAs
    if (! is_api_base())
      is_read_only = FALSE;
  }
}

BrowserComponent * BrowserComponent::read_ref(char * & st, const char * k)
{
  if (strcmp(k, "component_ref"))
    wrong_keyword(k, "component_ref");
  
  int id = read_id(st);
  BrowserComponent * result = all[id];
  
  return (result == 0)
    ? new BrowserComponent(id)
    : result;
}

BrowserComponent * BrowserComponent::read(char * & st, char * k, 
					  BrowserNode * parent)
{
  BrowserComponent * result;
  int id;
  
  if (!strcmp(k, "component_ref")) {
    if ((result = all[id = read_id(st)]) == 0)
      result = new BrowserComponent(id);
    return result;
  }
  else if (!strcmp(k, "component")) {
    id = read_id(st);
    
    QString s = read_string(st);
    
    k = read_keyword(st);
    
    if ((result = all[id]) == 0) {
      result = new BrowserComponent(s, parent, id);
      result->def->read(st, k);	// updates k
    }
    else {
      result->def->read(st, k);	// updates k
      result->set_parent(parent);
      result->set_name(s);
    }
    
    result->is_read_only = !in_import() && read_only_file() || 
      (user_id() != 0) && result->is_api_base();
    
    if (!strcmp(k, "associated_diagram")) {
      result->set_associated_diagram(BrowserComponentDiagram::read_ref(st, read_keyword(st)),
				     TRUE);
      k = read_keyword(st);
    }
    
    QValueList<BrowserClass *> rz;
    QValueList<BrowserClass *> pr;
    QValueList<BrowserClass *> rq;
    
    if (!strcmp(k, "realized_classes")) {
      while (strcmp((k = read_keyword(st)), "end"))
	rz.append(BrowserClass::read_ref(st, k));
      
      k = read_keyword(st);
    }
    
    if (!strcmp(k, "provided_classes")) {
      while (strcmp((k = read_keyword(st)), "end"))
	pr.append(BrowserClass::read_ref(st, k));
      
      k = read_keyword(st);
    }
    
    if (!strcmp(k, "required_classes")) {
      while (strcmp((k = read_keyword(st)), "end"))
	rq.append(BrowserClass::read_ref(st, k));
      
      k = read_keyword(st);
    }
    
    result->set_associated_classes(rz, pr, rq, TRUE);
    
    result->BrowserNode::read(st, k);	// updates k
    
    if (strcmp(k, "end")) {
      while (BrowserComponent::read(st, k, result) ||
	     BrowserSimpleRelation::read(st, k, result))
	k = read_keyword(st);
      
      if (strcmp(k, "end"))
	wrong_keyword(k, "end");
    }
    
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserComponent::get_it(const char * k, int id)
{
  // simple relation managed in use case
  return (!strcmp(k, "component_ref")) ? all[id] : 0;
}
