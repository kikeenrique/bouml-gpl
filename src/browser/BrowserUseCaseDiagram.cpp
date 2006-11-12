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
#include <qcursor.h>
#include <qfileinfo.h>

#include "BrowserUseCaseDiagram.h"
#include "SimpleData.h"
#include "UseCaseDiagramWindow.h"
#include "BrowserUseCaseView.h"
#include "DiagramView.h"
#include "UmlPixmap.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "BrowserView.h"
#include "mu.h"

QList<BrowserUseCaseDiagram> BrowserUseCaseDiagram::imported;
QValueList<int> BrowserUseCaseDiagram::imported_ids;
QStringList BrowserUseCaseDiagram::its_default_stereotypes;	// unicode

BrowserUseCaseDiagram::BrowserUseCaseDiagram(QString s, BrowserNode * p, int id)
    : BrowserDiagram(s, p, id), window(0) {
  make();
  is_modified = (id == 0);
}

BrowserUseCaseDiagram::BrowserUseCaseDiagram(int id)
    : BrowserDiagram(id), window(0) {
  // not yet read
  make();
  is_modified = (id == 0);
}

BrowserUseCaseDiagram::BrowserUseCaseDiagram(BrowserUseCaseDiagram * model, BrowserNode * p)
    : BrowserDiagram(p->child_random_name("Diagram"), p, 0), window(0) {
  def = new SimpleData(model->def);
  def->set_browser_node(this);
  comment = model->comment;
  settings = model->settings;
  usecase_color = model->usecase_color;
  note_color = model->note_color;
  package_color = model->package_color;
  fragment_color = model->fragment_color;
  subject_color = model->subject_color;
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

BrowserUseCaseDiagram::~BrowserUseCaseDiagram() {
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

void BrowserUseCaseDiagram::make() {
  def = new SimpleData;
  def->set_browser_node(this);
  usecase_color = UmlDefaultColor;
  note_color = UmlDefaultColor;
  package_color = UmlDefaultColor;
  fragment_color = UmlDefaultColor;
  subject_color = UmlDefaultColor;
}

void BrowserUseCaseDiagram::set_name(const char * s) {
  BrowserDiagram::set_name(s);
  if (window != 0)
    window->setCaption(s);
}

void BrowserUseCaseDiagram::import()
{
  QValueList<int>::Iterator it = imported_ids.begin();
  
  while (!imported.isEmpty()) {
    QString warning;
    BrowserUseCaseDiagram * d = imported.take(0);
    
    (new UseCaseDiagramWindow(d->full_name(), d, *it))->close(TRUE);
    it = imported_ids.remove(it);
    d->is_modified = TRUE;
  }
}

void BrowserUseCaseDiagram::renumber(int phase) {
  if (phase == -1)
    open(FALSE);
  else {
    new_ident(phase, all);
    window->get_view()->renumber(get_ident());
    is_modified = TRUE;
  }
}

void BrowserUseCaseDiagram::delete_it() {
  if (window)
    delete window;
  
  BrowserNode::delete_it();
}

BrowserNode * BrowserUseCaseDiagram::duplicate(BrowserNode * p, QString name) {
  BrowserUseCaseDiagram * result = new BrowserUseCaseDiagram(this, p);
  
  result->set_name((name.isEmpty()) ? get_name() : (const char *) name);
  result->update_stereotype();
  
  return result;
}

const QPixmap* BrowserUseCaseDiagram::pixmap(int) const {
  return (deletedp()) ? DeletedUseCaseDiagramIcon : UseCaseDiagramIcon;
}

void BrowserUseCaseDiagram::menu() {
  QPopupMenu m(0, name);
  QPopupMenu toolm(0);
  BrowserNode * item_above = 0;
  
  m.insertItem(new MenuTitle(name, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    m.setWhatsThis(m.insertItem("show", 0),
		   "to show and edit the <em>use case diagram</em>");
    if (!is_edited) {
      m.setWhatsThis(m.insertItem("edit", 1),
		     "to edit the <em>use case diagram</em>");
      if (!is_read_only) {
	m.setWhatsThis(m.insertItem("edit drawing settings", 2),
		       "to set how the <em>use case diagram</em>'s items must be drawed");
	m.insertSeparator();
	m.setWhatsThis(m.insertItem("duplicate", 3),
		       "to duplicate the <em>use case diagram</em>");
	if (edition_number == 0) {
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
	  
	  if (item_above != 0) {
	    switch (item_above->get_type()) {
	    case UmlUseCase:
	    case UmlUseCaseView:
	      if (!((BrowserNode *) item_above)->wrong_child_name(get_name(), UmlUseCaseDiagram, TRUE, FALSE)) {
		m.insertSeparator();
		m.setWhatsThis(m.insertItem(QString("set it nested in ")
					    + item_above->get_name(),
					    6),
			       "to set it nested in the item above");
	      }
	    default:
	      break;
	    }
	  }
	  m.insertSeparator();
	  m.setWhatsThis(m.insertItem("delete", 4),
			 "to delete the <em>use case diagram</em>. \
Note that you can undelete it after");
	}
      }
    }
    mark_menu(m, "use case diagram", 90);
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0))
    m.setWhatsThis(m.insertItem("undelete", 5),
		   "to undelete the <em>use case diagram</em>");
  
  int rank = m.exec(QCursor::pos());
  
  switch (rank) {
  case 0:
    open(FALSE);
    return;
  case 1:
    edit("Use Case diagram", its_default_stereotypes);
    return;
  case 2:
    edit_settings();
    return;
  case 3:
    {
      BrowserUseCaseDiagram * ucd
	= new BrowserUseCaseDiagram(this, (BrowserNode *) parent());
      
      ucd->select_in_browser();
      ucd->edit("Use Case diagram", its_default_stereotypes);
    }
    break;
  case 4:
    delete_it();
    break;
  case 5:
    undelete(FALSE);
    break;
  case 6:
    parent()->takeItem(this);
    item_above->insertItem(this);
    item_above->setOpen(TRUE);
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

void BrowserUseCaseDiagram::open(bool) {
  if (window == 0)
    window = new UseCaseDiagramWindow(full_name(TRUE), this);
  else
    window->raise();
  
  window->setFocus();
}

void BrowserUseCaseDiagram::edit_settings() {
  QArray<StateSpec> st;
  QArray<ColorSpec> co(5);
  
  settings.complete(st, TRUE);
  
  co[0].set("note color", &note_color);
  co[1].set("use case color", &usecase_color);
  co[2].set("package color", &package_color);
  co[3].set("fragment color", &fragment_color);
  co[4].set("subject color", &subject_color);

  SettingsDialog dialog(&st, &co, FALSE, FALSE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted) {
    DrawingSettings::modified();
    package_modified();
  }
}

void BrowserUseCaseDiagram::on_close() {
  window = 0;
}

void BrowserUseCaseDiagram::read_session(char * & st) {
  window->read_session(st);
}

UmlCode BrowserUseCaseDiagram::get_type() const {
  return UmlUseCaseDiagram;
}

void BrowserUseCaseDiagram::get_usecasediagramsettings(UseCaseDiagramSettings & r) const {
  if (! settings.complete(r))
    ((BrowserNode *) parent())->get_usecasediagramsettings(r);
}


void BrowserUseCaseDiagram::package_settings(bool & name_in_tab,
					     ShowContextMode & show_context) const {
  UseCaseDiagramSettings st;
  
  get_usecasediagramsettings(st);
  name_in_tab = st.package_name_in_tab == UmlYes;
  show_context = st.show_context_mode;
}

UmlColor BrowserUseCaseDiagram::get_color(UmlCode who) const {
  UmlColor c;
  
  switch (who) {
  case UmlUseCase:
    c = usecase_color;
    break;
  case UmlNote:
    c = note_color;
    break;
  case UmlFragment:
    c = fragment_color;
    break;
  case UmlSubject:
    c = subject_color;
    break;
  default:
    c = package_color;
  }
  
  return (c != UmlDefaultColor)
    ? c
    : ((BrowserNode *) parent())->get_color(who);
}

bool BrowserUseCaseDiagram::get_shadow() const {
  switch (settings.shadow) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_shadow(UmlUseCaseDiagram);
  }  
}

bool BrowserUseCaseDiagram::get_auto_label_position(UmlCode who) const {
  switch (settings.auto_label_position) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_auto_label_position(who);
  }
}

BasicData * BrowserUseCaseDiagram::get_data() const {
  return def;
}

bool BrowserUseCaseDiagram::tool_cmd(ToolCom * com, const char * args) {
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
      UseCaseDiagramWindow * w = new UseCaseDiagramWindow(full_name(), this);
      
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

void BrowserUseCaseDiagram::DragMoveEvent(QDragMoveEvent * e) {
  ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserUseCaseDiagram::DropEvent(QDropEvent * e) {
  ((BrowserNode *) parent())->DropAfterEvent(e, this);
}

void BrowserUseCaseDiagram::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "usecasediagram_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserUseCaseDiagram::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "usecasediagram_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
}

void BrowserUseCaseDiagram::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "usecasediagram_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "usecasediagram " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    def->save(st, warning);
    settings.save(st);
    
    bool nl = FALSE;
    
    save_color(st, "note_color", note_color, nl);
    save_color(st, "usecase_color", usecase_color, nl);
    save_color(st, "package_color", package_color, nl);
    save_color(st, "fragment_color", fragment_color, nl);
    save_color(st, "subject_color", subject_color, nl);
    
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


BrowserUseCaseDiagram * BrowserUseCaseDiagram::read_ref(char * & st, char * k)
{
  if (strcmp(k, "usecasediagram_ref"))
    wrong_keyword(k, "usecasediagram_ref");
  
  int id = read_id(st);
  BrowserUseCaseDiagram * result = 
    (BrowserUseCaseDiagram *) all[id];
  
  return (result == 0)
    ? new BrowserUseCaseDiagram(id)
    : result;
}

BrowserUseCaseDiagram * BrowserUseCaseDiagram::read(char * & st, char * k,
						    BrowserNode * parent)
{
  BrowserUseCaseDiagram * r;
  int id;
  
  if (!strcmp(k, "usecasediagram_ref")) {
    if ((r = (BrowserUseCaseDiagram *) all[id = read_id(st)]) == 0)
      r = new BrowserUseCaseDiagram(id);
    return r;
  }
  else if (!strcmp(k, "usecasediagram")) {
    id = read_id(st);
    
    QString s = read_string(st);
    
    if ((r = (BrowserUseCaseDiagram *) all[id]) == 0)
      r = new BrowserUseCaseDiagram(s, parent, id);
    else {
      r->set_parent(parent);
      r->set_name(s);
    }
    
    r->is_read_only = !in_import() && read_only_file() || 
      (user_id() != 0) && r->is_api_base();
    
    QFileInfo fi(BrowserView::get_dir(), QString::number(id) + ".diagram");
    if (!in_import() && fi.exists() && !fi.isWritable())
      r->is_read_only = TRUE;

    k = read_keyword(st);
    
    r->def->read(st, k);					// updates k
    r->settings.read(st, k);					// updates k
    read_color(st, "note_color", r->note_color, k);		// updates k
    read_color(st, "usecase_color", r->usecase_color, k);	// updates k
    read_color(st, "package_color", r->package_color, k);	// updates k
    read_color(st, "fragment_color", r->fragment_color, k);	// updates k
    read_color(st, "subject_color", r->subject_color, k);	// updates k
    r->BrowserNode::read(st, k);	// updates k
    
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

BrowserNode * BrowserUseCaseDiagram::get_it(const char * k, int id)
{
  return (!strcmp(k, "usecasediagram_ref")) ? all[id] : 0;
}
