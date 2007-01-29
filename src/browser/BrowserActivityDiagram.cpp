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
#include <qcursor.h>
#include <qfileinfo.h>

#include "BrowserActivityDiagram.h"
#include "SimpleData.h"
#include "BrowserActivity.h"
#include "ActivityDiagramWindow.h"
#include "UmlPixmap.h"
#include "SettingsDialog.h"
#include "DiagramView.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "BrowserView.h"
#include "mu.h"

QList<BrowserActivityDiagram> BrowserActivityDiagram::imported;
QValueList<int> BrowserActivityDiagram::imported_ids;
QStringList BrowserActivityDiagram::its_default_stereotypes;	// unicode

BrowserActivityDiagram::BrowserActivityDiagram(QString s, BrowserNode * p, int id)
    : BrowserDiagram(s, p, id), window(0) {
  make();
  is_modified = (id == 0);
}

BrowserActivityDiagram::BrowserActivityDiagram(BrowserActivityDiagram * model, BrowserNode * p)
    : BrowserDiagram(p->child_random_name("Diagram"), p, 0), window(0) {
  def = new SimpleData(model->def);
  def->set_browser_node(this);
  comment = model->comment;
  settings = model->settings;
  activity_color = model->activity_color;
  activityregion_color = model->activityregion_color;
  activityaction_color = model->activityaction_color;
  parameterpin_color = model->parameterpin_color;
  note_color = model->note_color;
  fragment_color = model->fragment_color;
  package_color = model->package_color;
  canvas_size = model->canvas_size;
  is_modified = TRUE;
  
  if (model->window != 0)
    model->window->duplicate(get_ident(), "diagram");
  else {
    char * diagram;
    
    if ((diagram = read_definition(model->get_ident(), "diagram")) != 0) {
      save_definition(get_ident(), "diagram", diagram, is_new);
      delete [] diagram;
    }
  }
}

bool BrowserActivityDiagram::api_compatible(unsigned v) const {
  return (v > 24);
}

BrowserActivityDiagram::BrowserActivityDiagram(int id)
    : BrowserDiagram(id), window(0) {
  // not yet read
  make();
  is_modified = (id == 0);
}

BrowserActivityDiagram::~BrowserActivityDiagram() {
  if (deletedp() && !modifiedp()) {
    QString fn;
    
    fn.sprintf("%d.diagram", get_ident());
    
    QDir d = BrowserView::get_dir();
    
    QFile::remove(d.absFilePath(fn));
  }
#if 0
  // already done before browser->clear
  if (window)
    delete window;
#endif
  all.remove(get_ident());
  delete def;
}

void BrowserActivityDiagram::make() {
  def = new SimpleData;
  def->set_browser_node(this);
  activity_color = UmlDefaultColor;
  activityregion_color = UmlDefaultColor;
  activityaction_color = UmlDefaultColor;
  parameterpin_color = UmlDefaultColor;
  note_color = UmlDefaultColor;
  fragment_color = UmlDefaultColor;
  package_color = UmlDefaultColor;
}

void BrowserActivityDiagram::set_name(const char * s) {
  BrowserDiagram::set_name(s);
  if (window != 0)
    window->setCaption(s);
}

void BrowserActivityDiagram::import()
{
  QValueList<int>::Iterator it = imported_ids.begin();
  
  while (!imported.isEmpty()) {
    QString warning;
    BrowserActivityDiagram * d = imported.take(0);
    
    (new ActivityDiagramWindow(d->full_name(), d, *it))->close(TRUE);
    it = imported_ids.remove(it);
    d->is_modified = TRUE;
  }
}

void BrowserActivityDiagram::renumber(int phase) {
  if (phase == -1)
    open(FALSE);
  else {
    new_ident(phase, all);
    window->get_view()->renumber(get_ident());
    is_modified = TRUE;
  }
}

void BrowserActivityDiagram::delete_it() {
  if (window)
    delete window;
  BrowserNode::delete_it();
}

BrowserNode * BrowserActivityDiagram::duplicate(BrowserNode * p, QString name) {
  BrowserActivityDiagram * result = new BrowserActivityDiagram(this, p);
  
  result->set_name((name.isEmpty()) ? get_name() : (const char *) name);
  result->update_stereotype();
  
  return result;
}

const QPixmap* BrowserActivityDiagram::pixmap(int) const {
  return (deletedp()) ? DeletedActivityDiagramIcon : ActivityDiagramIcon;
}

void BrowserActivityDiagram::menu() {
  QPopupMenu m(0, name);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(name, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    m.setWhatsThis(m.insertItem("Show", 0),
		   "to show and edit the <em>activity diagram</em>");
    if (!is_edited) {
      m.setWhatsThis(m.insertItem("Edit", 1),
		     "to edit the <em>activity diagram</em>");
      if (!is_read_only) {
	m.setWhatsThis(m.insertItem("Edit drawing settings", 2),
		       "to set how the <em>activity diagram</em>'s items must be drawed");
	m.insertSeparator();
	m.setWhatsThis(m.insertItem("Duplicate", 3),
		       "to duplicate the <em>activity diagram</em>");
	if (edition_number == 0) {
	  m.insertSeparator();
	  m.setWhatsThis(m.insertItem("Delete", 4),
			 "to delete the <em>activity diagram</em>. \
Note that you can undelete it after");
	}
      }
    }
    mark_menu(m, "activity diagram", 90);
    if ((edition_number == 0) && 
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("Tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0))
    m.setWhatsThis(m.insertItem("Undelete", 5),
		   "to undelete the <em>activity diagram</em>");
  
  exec_menu_choice(m.exec(QCursor::pos()));
}
  
void BrowserActivityDiagram::exec_menu_choice(int rank) {
  switch (rank) {
  case 0:
    open(FALSE);
    return;
  case 1:
    edit("Activity diagram", its_default_stereotypes);
    return;
  case 2:
    edit_settings();
    return;
  case 3:
    {
      BrowserActivityDiagram * cd =
	new BrowserActivityDiagram(this, (BrowserNode *) parent());
      
      cd->select_in_browser();
      cd->edit("Activity diagram", its_default_stereotypes);
    }
    break;
  case 4:
    delete_it();
    break;
  case 5:
    undelete(FALSE);
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

void BrowserActivityDiagram::apply_shortcut(QString s) {
  int choice = -1;
  
  if (!deletedp()) {
    if (s == "Show")
      choice = 0;;
    if (!is_edited) {
      if (s == "Edit")
	choice = 1;
      if (!is_read_only) {
	if (s == "Edit drawing settings")
	  choice = 2;
	else if (s == "Duplicate")
	  choice = 3;
	else if (edition_number == 0) {
	  if (s == "Delete")
	    choice = 4;
	}
      }
    }
    mark_shortcut(s, choice, 90);
    if (edition_number == 0)
      Tool::shortcut(s, choice, get_type(), 100);
  }
  else if (!is_read_only && (edition_number == 0))
    if (s == "Undelete")
      choice = 5;
  
  exec_menu_choice(choice);
}

void BrowserActivityDiagram::open(bool) {
  if (window == 0)
    window = new ActivityDiagramWindow(full_name(TRUE), this);
  else
    window->raise();
  
  window->setFocus();
}

void BrowserActivityDiagram::edit_settings() {
  QArray<StateSpec> st;
  QArray<ColorSpec> co(7);
  
  settings.complete(st, TRUE);
  
  co[0].set("activity color", &activity_color);
  co[1].set("activity region color", &activityregion_color);
  co[2].set("activity action color", &activityaction_color);
  co[3].set("parameter and pin color", &parameterpin_color);
  co[4].set("note color", &note_color);
  co[5].set("package color", &package_color);
  co[6].set("fragment color", &fragment_color);
  
  SettingsDialog dialog(&st, &co, FALSE, FALSE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted) {
    DrawingSettings::modified();
    package_modified();
  }
}

void BrowserActivityDiagram::on_close() {
  window = 0;
}

void BrowserActivityDiagram::read_session(char * & st) {
  window->read_session(st);
}

UmlCode BrowserActivityDiagram::get_type() const {
  return UmlActivityDiagram;
}

void BrowserActivityDiagram::get_activitydiagramsettings(ActivityDiagramSettings & r) const {
  if (! settings.complete(r))
    ((BrowserNode *) parent())->get_activitydiagramsettings(r);
}

void BrowserActivityDiagram::get_activitydrawingsettings(ActivityDrawingSettings & result) const {
  if (!settings.activitydrawingsettings.complete(result))
    ((BrowserNode *) parent())->get_activitydrawingsettings(result);
}

void BrowserActivityDiagram::package_settings(bool & name_in_tab,
					   ShowContextMode & show_context) const {
  ActivityDiagramSettings st;
  
  get_activitydiagramsettings(st);
  name_in_tab = st.package_name_in_tab == UmlYes;
  show_context = st.show_context_mode;
}

bool BrowserActivityDiagram::get_auto_label_position(UmlCode who) const {
  switch (settings.auto_label_position) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_auto_label_position(who);
  }
}

bool BrowserActivityDiagram::get_write_label_horizontally(UmlCode who) const {
  switch (settings.write_label_horizontally) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_write_label_horizontally(who);
  }
}

bool BrowserActivityDiagram::get_show_opaque_action_definition(UmlCode who) const {
  switch (settings.show_opaque_action_definition) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_show_opaque_action_definition(who);
  }
}

DrawingLanguage BrowserActivityDiagram::get_language(UmlCode who) const {
  DrawingLanguage v;
  
  switch (who) {
  case UmlActivityDiagram:
    v = settings.activitydrawingsettings.drawing_language;
    break;
  default:
    // error
    return UmlView;
  }
  
  return (v != DefaultDrawingLanguage)
    ? v
    : ((BrowserNode *) parent())->get_language(who);
}

UmlColor BrowserActivityDiagram::get_color(UmlCode who) const {
  UmlColor c;
  
  switch (who) {
  case UmlActivity:
    c = activity_color;
    break;
  case UmlInterruptibleActivityRegion:
  case UmlExpansionRegion:
    c = activityregion_color;
    break;
  case UmlActivityAction:
    c = activityaction_color;
    break;
  case UmlExpansionNode:
  case UmlParameter:
  case UmlActivityPin:
    c = parameterpin_color;
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

bool BrowserActivityDiagram::get_shadow() const {
  switch (settings.shadow) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_shadow(UmlActivityDiagram);
  }  
}

BasicData * BrowserActivityDiagram::get_data() const {
  return def;
}

bool BrowserActivityDiagram::tool_cmd(ToolCom * com, const char * args) {
  switch ((unsigned char) args[-1]) {
  case supportFileCmd:
    {
      QString fn;
      
      fn.sprintf("%d.diagram", get_ident());

      QDir d = BrowserView::get_dir();
      
      com->write_string(d.absFilePath(fn));
    }
    return TRUE;
  case saveInCmd:
    if (window != 0)
      com->write_ack(window->get_view()->save_in(args, TRUE, FALSE));
    else {
      ActivityDiagramWindow * w = new ActivityDiagramWindow(full_name(), this);
      
      com->write_ack(w->get_view()->save_in(args, 
					    !w->get_view()->has_preferred_size_zoom(),
					    TRUE));
      w->dont_save();
      w->close(TRUE);
    }
    return TRUE;
  default:
    return (def->tool_cmd(com, args, this, comment) ||
	    BrowserNode::tool_cmd(com, args));
  }
}

void BrowserActivityDiagram::DragMoveEvent(QDragMoveEvent * e) {
  ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserActivityDiagram::DropEvent(QDropEvent * e) {
  ((BrowserNode *) parent())->DropAfterEvent(e, this);
}

void BrowserActivityDiagram::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  if (after == 0)
    ((BrowserNode *) parent())->DropAfterEvent(e, this);
  else
    e->ignore();
}

QString BrowserActivityDiagram::drag_key() const {
  return QString::number(UmlActivityDiagram)
    + "#" + QString::number((unsigned long) parent());
}

QString BrowserActivityDiagram::drag_postfix() const {
  return "#" + QString::number((unsigned long) parent());
}

QString BrowserActivityDiagram::drag_key(BrowserNode * p)
{
  return QString::number(UmlActivityDiagram)
    + "#" + QString::number((unsigned long) p);
}

const QStringList & BrowserActivityDiagram::default_stereotypes() {
  return its_default_stereotypes;
}

void BrowserActivityDiagram::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "activitydiagram_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserActivityDiagram::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "activitydiagram_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
}

void BrowserActivityDiagram::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "activitydiagram_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "activitydiagram " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    def->save(st, warning);
    settings.save(st);
    
    bool nl = FALSE;
    
    save_color(st, "activity_color", activity_color, nl);
    save_color(st, "activityregion_color", activityregion_color, nl);
    save_color(st, "activityaction_color", activityaction_color, nl);
    save_color(st, "parameterpin_color", parameterpin_color, nl);
    save_color(st, "note_color", note_color, nl);
    save_color(st, "package_color", package_color, nl);
    save_color(st, "fragment_color", fragment_color, nl);
    
    BrowserNode::save(st);

    if (is_modified) {
      is_modified = FALSE;
      
      if (window)
	window->save("diagram", warning, is_new);
      else
	BrowserDiagram::save();
    }
    
    nl_indent(st);
    st << "size " << stringify(get_format());
    
    indent(-1);
    nl_indent(st);
    st << "end";
    
    // for saveAs
    if (! is_api_base())
      is_read_only = FALSE;
  }
}

BrowserActivityDiagram * BrowserActivityDiagram::read_ref(char * & st, char * k)
{
  if (strcmp(k, "activitydiagram_ref"))
    wrong_keyword(k, "activitydiagram_ref");
  
  int id = read_id(st);
  BrowserActivityDiagram * result = (BrowserActivityDiagram *) all[id];
  
  return (result == 0)
    ? new BrowserActivityDiagram(id)
    : result;
}

BrowserActivityDiagram * BrowserActivityDiagram::read(char * & st, char * k,
						      BrowserNode * parent)
{
  BrowserActivityDiagram * r;
  int id;
  
  if (!strcmp(k, "activitydiagram_ref")) {
    if ((r = (BrowserActivityDiagram *) all[id = read_id(st)]) == 0)
      r = new BrowserActivityDiagram(id);
    return r;
  }
  else if (!strcmp(k, "activitydiagram")) {
    id = read_id(st);
    
    QString s = read_string(st);
    
    if ((r = (BrowserActivityDiagram *) all[id]) == 0)
      r = new BrowserActivityDiagram(s, parent, id);
    else {
      r->set_parent(parent);
      r->set_name(s);
    }
    
    r->is_read_only = (!in_import() && read_only_file()) || 
      (user_id() != 0) && r->is_api_base();
    
    QFileInfo fi(BrowserView::get_dir(), QString::number(id) + ".diagram");
    if (!in_import() && fi.exists() && !fi.isWritable())
      r->is_read_only = TRUE;	
    
    k = read_keyword(st);
    
    r->def->read(st, k);					// updates k
    r->settings.read(st, k);					// updates k
    read_color(st, "activity_color", r->activity_color, k);		// updates k
    read_color(st, "activityregion_color", r->activityregion_color, k);		// updates k
    read_color(st, "activityaction_color", r->activityaction_color, k);		// updates k
    read_color(st, "parameterpin_color", r->parameterpin_color, k);		// updates k
    read_color(st, "note_color", r->note_color, k);		// updates k
    read_color(st, "package_color", r->package_color, k);	// updates k
    read_color(st, "fragment_color", r->fragment_color, k);	// updates k
    r->BrowserNode::read(st, k);				// updates k
    
    if (!strcmp(k, "size")) {
      r->set_format(canvas_format(read_keyword(st)));
      k = read_keyword(st);
    }
    else
      r->set_format(IsoA4);
    
    if (strcmp(k, "end"))
      wrong_keyword(k, "end");
    
    if (in_import()) {
      imported.append(r);
      imported_ids.append(id);
    }
    
    return r;
  }
  else
    return 0;
}

BrowserNode * BrowserActivityDiagram::get_it(const char * k, int id)
{
  return (!strcmp(k, "activitydiagram_ref")) ? all[id] : 0;
}
