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

#include <qpopupmenu.h> 
#include <qmessagebox.h>
#include <qcursor.h>

#include "BrowserComponentView.h"
#include "SimpleData.h"
#include "BrowserComponentDiagram.h"
#include "BrowserComponent.h"
#include "UmlPixmap.h"
#include "UmlDrag.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "mu.h"

IdDict<BrowserComponentView> BrowserComponentView::all;
QStringList BrowserComponentView::its_default_stereotypes;	// unicode

BrowserComponentView::BrowserComponentView(QString s, BrowserNode * p, int id)
    : BrowserNode(s, p), Labeled<BrowserComponentView>(all, id) {
  make();
  is_modified = (id == 0);
}

BrowserComponentView::BrowserComponentView(const BrowserComponentView * model,
					   BrowserNode * p)
    : BrowserNode(model->name, p), Labeled<BrowserComponentView>(all, 0) {
  def = new SimpleData(model->def);
  def->set_browser_node(this);
  comment = model->comment;
  componentdiagram_settings = model->componentdiagram_settings;
  component_color = model->component_color;
  note_color = model->note_color;
  fragment_color = model->fragment_color;
  package_color = model->package_color;
}

BrowserComponentView::BrowserComponentView(int id)
    : BrowserNode(), Labeled<BrowserComponentView>(all, id) {
  // not yet read
  make();
  is_modified = (id == 0);
}

BrowserComponentView::~BrowserComponentView() {
  all.remove(get_ident());
}

BrowserNode * BrowserComponentView::duplicate(BrowserNode * p, QString name) {
  BrowserComponentView * result = new BrowserComponentView(this, p);
  
  result->set_name((name.isEmpty()) ? get_name() : (const char *) name);
  result->update_stereotype();
  
  return result;
}

void BrowserComponentView::make() {
  def = new SimpleData;
  def->set_browser_node(this);
  
  component_color = UmlDefaultColor;
  note_color = UmlDefaultColor;
  package_color = UmlDefaultColor;
  fragment_color = UmlDefaultColor;
}

void BrowserComponentView::clear(bool old)
{
  all.clear(old);
  BrowserComponent::clear(old);
}

void BrowserComponentView::update_idmax_for_root()
{
  all.update_idmax_for_root();
  BrowserComponent::update_idmax_for_root();
}

void BrowserComponentView::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
  BrowserNode::renumber(phase);
}

const QPixmap* BrowserComponentView::pixmap(int) const {
  return (deletedp()) ? DeletedComponentViewIcon : ComponentViewIcon;
}

QString BrowserComponentView::full_name(bool rev, bool itself) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);

  if (!itself)
    return p;
  else if (p.isEmpty()) 
    return QString((const char *) name);
  else if (rev)
    return name + "   [" + p + "]";
  else
    return p + "::" + name;
}

void BrowserComponentView::menu() {
  QPopupMenu m(0);
  QPopupMenu subm(0);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(name, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!is_read_only && (edition_number == 0)) {
      m.setWhatsThis(m.insertItem("New component diagram", 0),
		     "to add a <em>component diagram</em>");
      m.setWhatsThis(m.insertItem("New component", 1),
		     "to add a <em>component</em>");
      m.insertSeparator();
    }
    if (!is_edited) {
      m.setWhatsThis(m.insertItem("Edit", 3),
		     "to edit the <em>component view</em>");
      m.insertSeparator();
      if (!is_read_only) {
	/*m.setWhatsThis(m.insertItem("Edit component settings", 4),
		       "to set the sub components's settings");*/
	m.setWhatsThis(m.insertItem("Edit drawing settings", 5),
		       "to set how the sub <em>component diagrams</em>'s items must be drawed");
	if (edition_number == 0) {
	  m.insertSeparator();
	  m.setWhatsThis(m.insertItem("Delete", 6),
			 "to delete the <em>component view</em> and its sub items. \
Note that you can undelete them after");
	}
      }
    }
    mark_menu(m, "component view", 90);
    
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("Tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0)) {
    m.setWhatsThis(m.insertItem("Undelete", 7),
		   "undelete the <em>component view</em>. \
Do not undelete its sub items");
    m.setWhatsThis(m.insertItem("Undelete recursively", 8),
		   "undelete the <em>component view</em> and its sub items");
  }
  
  exec_menu_choice(m.exec(QCursor::pos()));
}

void BrowserComponentView::exec_menu_choice(int rank) {
  switch (rank) {
  case 0:
    add_component_diagram();
    break;
  case 1:
    {
      BrowserComponent * c = BrowserComponent::add_component(this);
      
      if (c != 0)
	c->select_in_browser();
    }
    break;
  case 3:
    edit("Component view", its_default_stereotypes);
    return;
  /*case 4:
    if (! component_settings.edit(UmlComponentView))
      return;
    break;*/
  case 5:
    {
      QArray<StateSpec> st;
      QArray<ColorSpec> co(4);
      
      componentdiagram_settings.complete(st, FALSE);
      
      co[0].set("component color", &component_color);
      co[1].set("note color", &note_color);
      co[2].set("package color", &package_color);
      co[3].set("fragment color", &fragment_color);

      SettingsDialog dialog(&st, &co, FALSE, FALSE);
      
      dialog.raise();
      if (dialog.exec() != QDialog::Accepted)
	return;
      DrawingSettings::modified();
    }
    break;
  case 6:
    delete_it();
    break;
  case 7:
    undelete(FALSE);
    break;
  case 8:
    undelete(TRUE);
    break;
  default:
    if (rank >= 100)
      ToolCom::run(Tool::command(rank - 100), this);
    else
      mark_management(rank - 90);
    return;
  }
  
  package_modified();
}

void BrowserComponentView::apply_shortcut(QString s) {
  int choice = -1;
  
  if (!deletedp()) {
    if (!is_read_only && (edition_number == 0)) {
      if (s == "New component diagram")
	choice = 0;
      else if (s == "New component")
	choice = 1;
    }
    if (!is_edited) {
      if (s == "Edit")
	choice = 3;
      if (!is_read_only) {
	/*m.setWhatsThis(m.insertItem("Edit component settings", 4),
		       "to set the sub components's settings");*/
	if (s == "Edit drawing settings")
	  choice = 5;
	if (edition_number == 0) {
	  if (s == "Delete")
	    choice = 6;
	}
      }
    }
    mark_shortcut(s, choice, 90);
    if (edition_number == 0)
      Tool::shortcut(s, choice, get_type(), 100);
  }
  else if (!is_read_only && (edition_number == 0)) {
    if (s == "Undelete")
      choice = 7;
    else if (s == "Undelete recursively")
      choice = 8;
  }
  
  exec_menu_choice(choice);
}

void BrowserComponentView::open(bool) {
  if (!is_edited)
    edit("Component view", its_default_stereotypes);
}

void BrowserComponentView::add_component_diagram() {
  (new BrowserComponentDiagram(child_random_name("Diagram"), this))
    ->select_in_browser();
}

UmlCode BrowserComponentView::get_type() const {
  return UmlComponentView;
}

BasicData * BrowserComponentView::get_data() const {
  return def;
}

void BrowserComponentView::get_componentdiagramsettings(ComponentDiagramSettings & r) const {
  if (! componentdiagram_settings.complete(r))
    ((BrowserNode *) parent())->get_componentdiagramsettings(r);
}

UmlColor BrowserComponentView::get_color(UmlCode who) const {
  UmlColor c;
  
  switch (who) {
  case UmlComponent:
    c = component_color;
    break;
  case UmlNote:
    c = note_color;
    break;
  case UmlFragment:
    c = fragment_color;
    break;
  default:
    c = package_color;
  }
  
  return (c != UmlDefaultColor)
    ? c
    : ((BrowserNode *) parent())->get_color(who);
}

bool BrowserComponentView::get_shadow(UmlCode who) const {
  switch (componentdiagram_settings.shadow) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_shadow(who);
  }
}

bool BrowserComponentView::get_auto_label_position(UmlCode who) const {
  Uml3States v;
  
  switch (who) {
  case UmlComponentDiagram:
    v = componentdiagram_settings.auto_label_position;
    break;
  default:
    // error
    return FALSE;
  }
  
  switch (v) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_auto_label_position(who);
  }
}

void BrowserComponentView::get_componentdrawingsettings(bool, ComponentDrawingSettings & result) const {
  if (!componentdiagram_settings.componentdrawingsettings.complete(result))
    ((BrowserNode *) parent())->get_componentdrawingsettings(FALSE, result);
}

BrowserNodeList & BrowserComponentView::instances(BrowserNodeList & result)
{
  IdIterator<BrowserComponentView> it(all);
  
  while (it.current() != 0) {
    if (!it.current()->deletedp())
      result.append(it.current());
    ++it;
  }

  result.sort();
  
  return result;
}

bool BrowserComponentView::tool_cmd(ToolCom * com, const char * args) {
  switch ((unsigned char) args[-1]) {
  case createCmd:
    {
      bool ok = TRUE;
      
      if (is_read_only && !root_permission())
	ok = FALSE;
      else {
	switch (com->get_kind(args)) {
	case UmlComponentDiagram:
	  if (wrong_child_name(args, UmlComponentDiagram, TRUE, FALSE))
	    ok = FALSE;
	  else
	    (new BrowserComponentDiagram(args, this))->write_id(com);
	  break;
	case UmlComponent:
	  if (wrong_child_name(args, UmlComponent, FALSE, FALSE))
	    ok = FALSE;
	  else
	    (new BrowserComponent(args, this))->write_id(com);
	  break;
	default:
	  ok = FALSE;
	}
      }
      
      if (! ok)
	com->write_id(0);
      else {
	package_modified();
      }
      
      return TRUE;
   }
  default:
    return (def->tool_cmd(com, args, this, comment) ||
	    BrowserNode::tool_cmd(com, args));
  }
}

void BrowserComponentView::DragMoveEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, UmlComponent) ||
      UmlDrag::canDecode(e, UmlComponentDiagram))
    e->accept();
  else
    ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserComponentView::DragMoveInsideEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, UmlComponent) ||
      UmlDrag::canDecode(e, UmlComponentDiagram))
    e->accept();
  else
    e->ignore();
}

bool BrowserComponentView::may_contains_them(const QList<BrowserNode> & l,
					     bool & duplicable) const {
  QListIterator<BrowserNode> it(l);
  
  for (; it.current(); ++it) {
    switch (it.current()->get_type()) {
    case UmlComponent:
      duplicable = FALSE;
      // no break
    case UmlComponentDiagram:
      if (! may_contains(it.current(), FALSE))
	return FALSE;
      break;
    default:
      return FALSE;
    }
    
    duplicable = may_contains_it(it.current());
  }
  
  return TRUE;
}

void BrowserComponentView::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserComponentView::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  BrowserNode * bn;
  
  if ((((bn = UmlDrag::decode(e, UmlComponent)) != 0) ||
       (((bn = UmlDrag::decode(e, UmlComponentDiagram)) != 0))) &&
      (bn != after) && (bn != this)) {
    if (may_contains(bn, FALSE)) {
      BrowserNode * old_parent = (BrowserNode *) bn->parent();
      
      if (after)
	bn->moveItem(after);
      else {
	bn->parent()->takeItem(bn);
	insertItem(bn);
      }
      package_modified();
      if (old_parent != this)
	old_parent->package_modified();
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

void BrowserComponentView::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "componentview_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserComponentView::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "componentview_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
}

void BrowserComponentView::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "componentview_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "componentview " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    
    def->save(st, warning);
    nl_indent(st);
    st << "//component diagram settings";
    componentdiagram_settings.save(st);
    //nl_indent(st);
    //st << "//component settings";
    //component_settings.save(st);
    
    bool nl = FALSE;
    
    save_color(st, "component_color", component_color, nl);
    save_color(st, "note_color", note_color, nl);
    save_color(st, "package_color", package_color, nl);
    save_color(st, "fragment_color", fragment_color, nl);
    
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
	else {
	  // set 'not modified' to delete the associated file on exit
	  ((BrowserNode *) child)->unmodified();
	  
	  if ((child = child->nextSibling()) == 0)
	    break;
	}
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

BrowserComponentView * BrowserComponentView::read_ref(char * & st, char * k)
{
  if (strcmp(k, "componentview_ref"))
    wrong_keyword(k, "componentview_ref");
  
  int id = read_id(st);
  BrowserComponentView * result = all[id];
  
  return (result == 0)
    ? new BrowserComponentView(id)
    : result;
}

BrowserComponentView * BrowserComponentView::read(char * & st, char * k,
						  BrowserNode * parent,
						  bool /*recursive*/)
{
  if (!strcmp(k, "componentview")) {
    int id = read_id(st);
    QString s = read_string(st);
    BrowserComponentView * r = all[id];
    
    if (r == 0)
      r = new BrowserComponentView(s, parent, id);
    else {
      r->set_parent(parent);
      r->set_name(s);
    }
    
    r->is_read_only = !in_import() && read_only_file() || 
      (user_id() != 0) && r->is_api_base();
    
    k = read_keyword(st);
    
    r->def->read(st, k);					// updates k
    r->componentdiagram_settings.read(st, k);			// updates k
    //r->component_settings.read(st, k);			// updates k
    read_color(st, "component_color", r->component_color, k);	// updates k
    read_color(st, "note_color", r->note_color, k);		// updates k
    read_color(st, "package_color", r->package_color, k);	// updates k
    read_color(st, "fragment_color", r->fragment_color, k);	// updates k
    r->BrowserNode::read(st, k);				// updates k
    
    if (strcmp(k, "end")) {
      while (BrowserComponentDiagram::read(st, k, r) ||
	     BrowserComponent::read(st, k, r))
	k = read_keyword(st);
      
      if (strcmp(k, "end"))
	wrong_keyword(k, "end");
    }
    
    return r;
  }
  else
    return 0;
}

BrowserNode * BrowserComponentView::get_it(const char * k, int id)
{
  if (!strcmp(k, "classview_ref"))
    return all[id];
  
  BrowserNode * r;
  
  if ((r = BrowserComponentDiagram::get_it(k, id)) == 0)
    r = BrowserComponent::get_it(k, id);
  
  return r;
}
