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

#include <qpopupmenu.h> 
#include <qcursor.h>
#include <qfileinfo.h>

#include "BrowserStateDiagram.h"
#include "SimpleData.h"
#include "BrowserState.h"
#include "StateDiagramWindow.h"
#include "UmlPixmap.h"
#include "SettingsDialog.h"
#include "DiagramView.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "BrowserView.h"
#include "mu.h"

QList<BrowserStateDiagram> BrowserStateDiagram::imported;
QValueList<int> BrowserStateDiagram::imported_ids;
QStringList BrowserStateDiagram::its_default_stereotypes;	// unicode

BrowserStateDiagram::BrowserStateDiagram(QString s, BrowserNode * p, int id)
    : BrowserDiagram(s, p, id), window(0) {
  make();
  is_modified = (id == 0);
}

BrowserStateDiagram::BrowserStateDiagram(BrowserStateDiagram * model, BrowserNode * p)
    : BrowserDiagram(p->get_name(), p, 0), window(0) {
  def = new SimpleData(model->def);
  def->set_browser_node(this);
  comment = model->comment;
  settings = model->settings;
  state_color = model->state_color;
  stateaction_color = model->stateaction_color;
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

BrowserStateDiagram::BrowserStateDiagram(int id)
    : BrowserDiagram(id), window(0) {
  // not yet read
  make();
  is_modified = (id == 0);
}

BrowserStateDiagram::~BrowserStateDiagram() {
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

void BrowserStateDiagram::make() {
  def = new SimpleData;
  def->set_browser_node(this);
  state_color = UmlDefaultColor;
  stateaction_color = UmlDefaultColor;
  note_color = UmlDefaultColor;
  fragment_color = UmlDefaultColor;
  package_color = UmlDefaultColor;
}

BrowserStateDiagram * BrowserStateDiagram::add_state_diagram(BrowserNode * future_parent)
{
  QString name;
  
  if (future_parent->enter_child_name(name, "enter state diagram's name : ",
				      UmlStateDiagram, TRUE, FALSE))
    return new BrowserStateDiagram(name, future_parent);
  else
    return 0;
}

void BrowserStateDiagram::set_name(const char * s) {
  BrowserDiagram::set_name(s);
  if (window != 0)
    window->setCaption(s);
}

void BrowserStateDiagram::import()
{
  QValueList<int>::Iterator it = imported_ids.begin();
  
  while (!imported.isEmpty()) {
    QString warning;
    BrowserStateDiagram * d = imported.take(0);
    
    (new StateDiagramWindow(d->full_name(), d, *it))->close(TRUE);
    it = imported_ids.remove(it);
    d->is_modified = TRUE;
  }
}

void BrowserStateDiagram::renumber(int phase) {
  if (phase == -1)
    open(FALSE);
  else {
    new_ident(phase, all);
    window->get_view()->renumber(get_ident());
    is_modified = TRUE;
  }
}

void BrowserStateDiagram::delete_it() {
  if (window)
    delete window;
  BrowserNode::delete_it();
}

BrowserNode * BrowserStateDiagram::duplicate(BrowserNode * p, QString name) {
  BrowserStateDiagram * result = new BrowserStateDiagram(this, p);
  
  result->set_name((name.isEmpty()) ? get_name() : (const char *) name);
  result->update_stereotype();
  
  return result;
}

const QPixmap* BrowserStateDiagram::pixmap(int) const {
  return (deletedp()) ? DeletedStateDiagramIcon : StateDiagramIcon;
}

void BrowserStateDiagram::draw_svg() const {
  fputs("\t<rect fill=\"white\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x=\"0\" y=\"0\" width=\"15\" height=\"15\" />\n"
	"\t<path fill=\"none\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" "
	"d=\"M 6 1 L 8 1 L 8 2 L 6 2 L 6 1 "
	"M 7 2 L 7 7 "
	"M 4 8 L 10 8 "
	"M 2 10 L 4 8 "
	"M 2 11 L 4 13 "
	"M 12 10 L 10 8 "
	"M 12 11 L 10 13 "
	"M 4 13 L 10 13 "
	"M 2 10 L 2 11 "
	"M 12 10 L 12 11 "
	"M 5 5 L 7 7 L 9 5\" />\n",
	svg());
}

void BrowserStateDiagram::menu() {
  QPopupMenu m(0, name);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(name, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    m.setWhatsThis(m.insertItem("Show", 0),
		   "to show and edit the <em>state diagram</em>");
    if (!is_edited) {
      m.setWhatsThis(m.insertItem("Edit", 1),
		     "to edit the <em>state diagram</em>");
      if (!is_read_only) {
	m.setWhatsThis(m.insertItem("Edit drawing settings", 2),
		       "to set how the <em>state diagram</em>'s items must be drawed");
	m.insertSeparator();
	m.setWhatsThis(m.insertItem("Duplicate", 3),
		       "to duplicate the <em>state diagram</em>");
	if (edition_number == 0) {
	  m.insertSeparator();
	  m.setWhatsThis(m.insertItem("Delete", 4),
			 "to delete the <em>state diagram</em>. \
Note that you can undelete it after");
	}
      }
    }
    mark_menu(m, "state diagram", 90);
    if ((edition_number == 0) && 
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("Tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0))
    m.setWhatsThis(m.insertItem("Undelete", 5),
		   "to undelete the <em>state diagram</em>");
  
  exec_menu_choice(m.exec(QCursor::pos()));
}

void BrowserStateDiagram::exec_menu_choice(int rank) {
  switch (rank) {
  case 0:
    open(FALSE);
    return;
  case 1:
    edit("State diagram", its_default_stereotypes);
    return;
  case 2:
    edit_settings();
    return;
  case 3:
    {
      QString name;
      
      if (((BrowserNode *)parent())->enter_child_name(name, "enter state diagram's name : ",
						      UmlStateDiagram, TRUE, FALSE))
	duplicate((BrowserNode *) parent(), name)->select_in_browser();
      else
	return;
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

void BrowserStateDiagram::apply_shortcut(QString s) {
  int choice = -1;

  if (!deletedp()) {
    if (s == "Show")
      choice = 0;
    if (!is_edited) {
      if (s == "Edit")
	choice = 1;
      if (!is_read_only) {
	if (s == "Edit drawing settings")
	  choice = 2;
	else if (s == "Duplicate")
	  choice = 3;
	if (edition_number == 0) {
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

void BrowserStateDiagram::open(bool) {
  if (window == 0)
    window = new StateDiagramWindow(full_name(TRUE), this);
  else
    window->raise();
  
  window->setFocus();
}

void BrowserStateDiagram::edit_settings() {
  QArray<StateSpec> st;
  QArray<ColorSpec> co(5);
  
  settings.complete(st, TRUE);
  
  co[0].set("state color", &state_color);
  co[1].set("state action color", &stateaction_color);
  co[2].set("note color", &note_color);
  co[3].set("package color", &package_color);
  co[4].set("fragment color", &fragment_color);
  
  SettingsDialog dialog(&st, &co, FALSE, FALSE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted) {
    DrawingSettings::modified();
    package_modified();
  }
}

void BrowserStateDiagram::on_close() {
  window = 0;
}

void BrowserStateDiagram::read_session(char * & st) {
  window->read_session(st);
}

UmlCode BrowserStateDiagram::get_type() const {
  return UmlStateDiagram;
}

int BrowserStateDiagram::get_identifier() const {
  return get_ident();
}

void BrowserStateDiagram::get_statediagramsettings(StateDiagramSettings & r) const {
  if (! settings.complete(r))
    ((BrowserNode *) parent())->get_statediagramsettings(r);
}

void BrowserStateDiagram::get_statedrawingsettings(StateDrawingSettings & result) const {
  if (!settings.statedrawingsettings.complete(result))
    ((BrowserNode *) parent())->get_statedrawingsettings(result);
}

void BrowserStateDiagram::package_settings(bool & name_in_tab,
					   ShowContextMode & show_context) const {
  StateDiagramSettings st;
  
  get_statediagramsettings(st);
  name_in_tab = st.package_name_in_tab == UmlYes;
  show_context = st.show_context_mode;
}

bool BrowserStateDiagram::get_auto_label_position(UmlCode who) const {
  switch (settings.auto_label_position) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_auto_label_position(who);
  }
}

bool BrowserStateDiagram::get_write_label_horizontally(UmlCode who) const {
  switch (settings.write_label_horizontally) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_write_label_horizontally(who);
  }
}

bool BrowserStateDiagram::get_show_trans_definition(UmlCode who) const {
  switch (settings.show_trans_definition) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_show_trans_definition(who);
  }
}

DrawingLanguage BrowserStateDiagram::get_language(UmlCode who) const {
  DrawingLanguage v;
  
  switch (who) {
  case UmlStateDiagram:
    v = settings.statedrawingsettings.drawing_language;
    break;
  default:
    // error
    return UmlView;
  }
  
  return (v != DefaultDrawingLanguage)
    ? v
    : ((BrowserNode *) parent())->get_language(who);
}

UmlColor BrowserStateDiagram::get_color(UmlCode who) const {
  UmlColor c;
  
  switch (who) {
  case UmlState:
    c = state_color;
    break;
  case UmlStateAction:
    c = stateaction_color;
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

bool BrowserStateDiagram::get_shadow() const {
  switch (settings.shadow) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_shadow(UmlStateDiagram);
  }  
}

bool BrowserStateDiagram::get_draw_all_relations() const {
  switch (settings.draw_all_relations) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_draw_all_relations(UmlStateDiagram);
  }  
}

void BrowserStateDiagram::dont_draw_all_relations() {
  settings.draw_all_relations = UmlNo;
}

BasicData * BrowserStateDiagram::get_data() const {
  return def;
}

bool BrowserStateDiagram::tool_cmd(ToolCom * com, const char * args) {
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
      com->write_ack(window->get_view()->save_pict(args, TRUE, FALSE));
    else {
      StateDiagramWindow * w = new StateDiagramWindow(full_name(), this);
      
      com->write_ack(w->get_view()->save_pict(args, 
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

void BrowserStateDiagram::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  if (after == 0)
    ((BrowserNode *) parent())->DropAfterEvent(e, this);
  else
    e->ignore();
}

QString BrowserStateDiagram::drag_key() const {
  return QString::number(UmlStateDiagram)
    + "#" + QString::number((unsigned long) parent());
}

QString BrowserStateDiagram::drag_postfix() const {
  return "#" + QString::number((unsigned long) parent());
}

QString BrowserStateDiagram::drag_key(BrowserNode * p)
{
  return QString::number(UmlStateDiagram)
    + "#" + QString::number((unsigned long) p);
}

const QStringList & BrowserStateDiagram::default_stereotypes() {
  return its_default_stereotypes;
}

void BrowserStateDiagram::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "statediagram_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserStateDiagram::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "statediagram_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
}

void BrowserStateDiagram::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "statediagram_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "statediagram " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    def->save(st, warning);
    settings.save(st);
    
    bool nl = FALSE;
    
    save_color(st, "state_color", state_color, nl);
    save_color(st, "stateaction_color", stateaction_color, nl);
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

BrowserStateDiagram * BrowserStateDiagram::read_ref(char * & st, char * k)
{
  if (strcmp(k, "statediagram_ref"))
    wrong_keyword(k, "statediagram_ref");
  
  int id = read_id(st);
  BrowserStateDiagram * result = (BrowserStateDiagram *) all[id];
  
  return (result == 0)
    ? new BrowserStateDiagram(id)
    : result;
}

BrowserStateDiagram * BrowserStateDiagram::read(char * & st, char * k,
						BrowserNode * parent)
{
  BrowserStateDiagram * r;
  int id;
  
  if (!strcmp(k, "statediagram_ref")) {
    if ((r = (BrowserStateDiagram *) all[id = read_id(st)]) == 0)
      r = new BrowserStateDiagram(id);
    return r;
  }
  else if (!strcmp(k, "statediagram")) {
    id = read_id(st);
    
    QString s = read_string(st);
    
    if ((r = (BrowserStateDiagram *) all[id]) == 0)
      r = new BrowserStateDiagram(s, parent, id);
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
    if (read_file_format() < 30)
      r->settings.draw_all_relations = UmlNo;
    read_color(st, "state_color", r->state_color, k);		// updates k
    read_color(st, "stateaction_color", r->stateaction_color, k);		// updates k
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

BrowserNode * BrowserStateDiagram::get_it(const char * k, int id)
{
  return (!strcmp(k, "statediagram_ref")) ? all[id] : 0;
}
