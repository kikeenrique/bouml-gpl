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

#include "BrowserDeploymentNode.h"
#include "BrowserSimpleRelation.h"
#include "SimpleData.h"
#include "BrowserDeploymentDiagram.h"
#include "UmlPixmap.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "UmlDrag.h"
#include "ReferenceDialog.h"
#include "mu.h"

IdDict<BrowserDeploymentNode> BrowserDeploymentNode::all;
QStringList BrowserDeploymentNode::its_default_stereotypes;	// unicode

BrowserDeploymentNode::BrowserDeploymentNode(QString s, BrowserNode * p, int id)
    : BrowserNode(s, p), Labeled<BrowserDeploymentNode>(all, id) {
  def = new SimpleData;
  def->set_browser_node(this);
  
  associated_diagram = 0;
}

BrowserDeploymentNode::BrowserDeploymentNode(const BrowserDeploymentNode * model,
					     BrowserNode * p)
    : BrowserNode(model->name, p), Labeled<BrowserDeploymentNode>(all, 0) {
  def = new SimpleData(model->def);
  def->set_browser_node(this);
  comment = model->comment;
  associated_diagram = 0;  
}

BrowserDeploymentNode::BrowserDeploymentNode(int id)
    : BrowserNode(), Labeled<BrowserDeploymentNode>(all, id) {
  def = new SimpleData;
  def->set_browser_node(this);
  
  associated_diagram = 0;
}

BrowserDeploymentNode::~BrowserDeploymentNode() {
  all.remove(get_ident());
  delete def;
}

BrowserNode * BrowserDeploymentNode::duplicate(BrowserNode * p, QString name) {
  BrowserDeploymentNode * result = new BrowserDeploymentNode(this, p);
  
  result->set_name((name.isEmpty()) ? get_name() : (const char *) name);
  result->update_stereotype();
  
  return result;
}

void BrowserDeploymentNode::clear(bool old)
{
  all.clear(old);
}

void BrowserDeploymentNode::update_idmax_for_root()
{
  all.update_idmax_for_root();
}
    
void BrowserDeploymentNode::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
}

const QPixmap* BrowserDeploymentNode::pixmap(int) const {
  return (deletedp()) ? DeletedDeploymentNodeIcon : DeploymentNodeIcon;
}

QString BrowserDeploymentNode::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);

  if (p.isEmpty()) 
    return QString((const char *) name);
  else if (rev)
    return name + "   [" + p + "]";
  else
    return p + "::" + name;
}

void BrowserDeploymentNode::menu() {
  QPopupMenu m(0, name);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(name, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!is_edited) {
      m.setWhatsThis(m.insertItem("Edit", 0),
		     "to edit the <em>node</em>, \
a double click with the left mouse button does the same thing");
      if (!is_read_only && (edition_number == 0)) {
	m.insertSeparator();
	m.setWhatsThis(m.insertItem("Delete", 1),
		       "to delete the <em>node</em>. \
Note that you can undelete it after");
      }
    }
    m.insertSeparator();
    m.setWhatsThis(m.insertItem("Referenced by", 3),
		   "to know who reference the <i>node</i> \
through a relation");
    mark_menu(m, "node", 90);
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("Tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0))
    m.setWhatsThis(m.insertItem("Undelete", 2),
		   "to undelete the <em>node</em>");
  
  int rank = m.exec(QCursor::pos());
  
  switch (rank) {
  case 0:
    edit("Node", its_default_stereotypes);
    return;
  case 1:
    delete_it();
    break;
  case 2:
    undelete(FALSE);
    break;
  case 3:
    ReferenceDialog::show(this);
    return;
  default:
    if (rank >= 100)
      ToolCom::run(Tool::command(rank - 100), this);
    else
      mark_management(rank - 90);
    return;
  }
  
  package_modified();
}

void BrowserDeploymentNode::open(bool force_edit) {
  if (!force_edit && 
      (associated_diagram != 0) &&
      !associated_diagram->deletedp())
    associated_diagram->open(FALSE);
  else if (!is_edited)
    edit("Node", its_default_stereotypes);
}

void BrowserDeploymentNode::DragMoveEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, BrowserSimpleRelation::drag_key(this)))
    e->accept();
  else
    ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserDeploymentNode::DragMoveInsideEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, BrowserSimpleRelation::drag_key(this)))
    e->accept();
  else
    e->ignore();
}

void BrowserDeploymentNode::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserDeploymentNode::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
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

UmlCode BrowserDeploymentNode::get_type() const {
  return UmlDeploymentNode;
}

BrowserDeploymentNode * BrowserDeploymentNode::get_deploymentnode(BrowserNode * future_parent)
{
  BrowserNode * old;
  QString name;
  BrowserNodeList nodes;
  
  if (!future_parent->enter_child_name(name, "enter node's name : ",
				       UmlDeploymentNode, instances(nodes),
				       &old, TRUE, FALSE))
    return 0;

  if (old != 0)
    return ((BrowserDeploymentNode *) old);
  
  future_parent->setOpen(TRUE);
  future_parent->package_modified();
  
  return new BrowserDeploymentNode(name, future_parent);
}

BrowserDeploymentNode * BrowserDeploymentNode::add_deploymentnode(BrowserNode * future_parent)
{
  QString name;
  
  if (! future_parent->enter_child_name(name, "enter node's name : ",
					UmlDeploymentNode, TRUE, FALSE))
    return 0;
  
  future_parent->setOpen(TRUE);
  future_parent->package_modified();
  return new BrowserDeploymentNode(name, future_parent);
}

BasicData * BrowserDeploymentNode::get_data() const {
  return def;
}

BrowserNodeList & BrowserDeploymentNode::instances(BrowserNodeList & result,
						   const char * st) {
  IdIterator<BrowserDeploymentNode> it(all);
  
  if ((st == 0) || (*st == 0)) {
    while (it.current() != 0) {
      if (!it.current()->deletedp())
	result.append(it.current());
      ++it;
    }
  }
  else {
    while (it.current() != 0) {
      if (!it.current()->deletedp() &&
	  !strcmp(it.current()->get_data()->get_stereotype(), st))
	result.append(it.current());
      ++it;
    }
  }
  
  result.sort();
  
  return result;
}

BrowserNode * BrowserDeploymentNode::get_associated() const {
  return associated_diagram;
}

void BrowserDeploymentNode::set_associated_diagram(BrowserDeploymentDiagram * dg,
						   bool on_read) {
  if (associated_diagram != dg) {
    if (associated_diagram != 0)
      QObject::disconnect(associated_diagram->get_data(), SIGNAL(deleted()),
			  def, SLOT(on_delete()));
    if ((associated_diagram = dg) != 0)
      QObject::connect(associated_diagram->get_data(), SIGNAL(deleted()),
		       def, SLOT(on_delete()));
    
    if (! on_read)
      package_modified();
  }
}

void BrowserDeploymentNode::on_delete() {
  if (associated_diagram && associated_diagram->deletedp())
    associated_diagram = 0;
}

void BrowserDeploymentNode::init()
{
  its_default_stereotypes.clear();
  its_default_stereotypes.append("cpu");
  its_default_stereotypes.append("device");
}

// unicode
const QStringList & BrowserDeploymentNode::default_stereotypes()
{
  return its_default_stereotypes;
}

bool BrowserDeploymentNode::tool_cmd(ToolCom * com, const char * args) {
  switch ((unsigned char) args[-1]) {
  case setAssocDiagramCmd:
    if (is_read_only && !root_permission())
      com->write_ack(FALSE);
    else {
      set_associated_diagram((BrowserDeploymentDiagram *) com->get_id(args));
      com->write_ack(TRUE);
    }
    break;
  case createCmd:
    {
      bool ok = TRUE;
      
      if (is_read_only && !root_permission())
	ok = FALSE;
      else {
	switch (com->get_kind(args)) {
	case UmlSimpleRelations:
	  {
	    UmlCode c;
	    
	    if (!com->get_relation_kind(c, args) ||
		(c != UmlDependency))
	      ok = FALSE;
	    else {
	      BrowserNode * end = (BrowserNode *) com->get_id(args);
	      
	      if (end->get_type() == UmlDeploymentNode)
		add_relation(UmlDependOn, end)->get_browser_node()->write_id(com);
	      else
		ok = FALSE;
	    }
	  }
	  break;
	default:
	  ok = FALSE;
	}
      }
      
      if (! ok)
	com->write_id(0);
      else
	package_modified();
      break;
    }
  default:
    return (def->tool_cmd(com, args, this, comment) ||
	    BrowserNode::tool_cmd(com, args));
  }
  
  return TRUE;
}

void BrowserDeploymentNode::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "deploymentnode_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserDeploymentNode::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "deploymentnode_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
  else
    init();
}

void BrowserDeploymentNode::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "deploymentnode_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "deploymentnode " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    def->save(st, warning);
    if (associated_diagram != 0) {
      if (associated_diagram->deletedp()) {
	warning += QString("<p>deployment node <b>") + full_name() +
	  "</b>'s associated diagram <b>" +
	    associated_diagram->full_name() + "</b> is deleted\n";
      }
      else {
	nl_indent(st);
	st << "associated_diagram ";
	associated_diagram->save(st, TRUE, warning);
      }
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

BrowserDeploymentNode * BrowserDeploymentNode::read_ref(char * & st)
{
  read_keyword(st, "deploymentnode_ref");
  
  int id = read_id(st);
  BrowserDeploymentNode * result = all[id];
  
  return (result == 0) 
    ? new BrowserDeploymentNode(id)
    : result;
}

BrowserDeploymentNode * BrowserDeploymentNode::read(char * & st, char * k, 
						    BrowserNode * parent)
{
  BrowserDeploymentNode * result;
  int id;
  
  if (!strcmp(k, "deploymentnode_ref")) {
    id = read_id(st);
    result = all[id];
    
    return (result == 0)
      ? new BrowserDeploymentNode(id)
      : result;
  }
  else if (!strcmp(k, "deploymentnode")) {
    id = read_id(st);
    result = all[id];
    
    if (result == 0)
      result = 
	new BrowserDeploymentNode(read_string(st), parent, id);
    else {
      result->set_parent(parent);
      result->set_name(read_string(st));
    }
    
    result->is_read_only = !in_import() && read_only_file() || 
      (user_id() != 0) && result->is_api_base();
    
    k = read_keyword(st);
    
    result->def->read(st, k);	// updates k
    if (!strcmp(k, "associated_deployment_diagram")) {
      // old format
      result->set_associated_diagram(BrowserDeploymentDiagram::read_ref(st, "deploymentdiagram_ref"),
				     TRUE);
      k = read_keyword(st);
    }
    else if (!strcmp(k, "associated_diagram")) {
      result->set_associated_diagram(BrowserDeploymentDiagram::read_ref(st, read_keyword(st)),
				     TRUE);
      k = read_keyword(st);
    }
    
    result->BrowserNode::read(st, k);	// updates k
    
    if (strcmp(k, "end")) {
      while (BrowserSimpleRelation::read(st, k, result))
	k = read_keyword(st);
      
      if (strcmp(k, "end"))
	wrong_keyword(k, "end");
    }
    
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserDeploymentNode::get_it(const char * k, int id)
{
  return (!strcmp(k, "deploymentnode_ref")) ? all[id] : 0;
}
