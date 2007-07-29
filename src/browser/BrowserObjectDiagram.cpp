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

#include "BrowserObjectDiagram.h"
#include "SimpleData.h"
#include "ObjectDiagramWindow.h"
#include "DiagramView.h"
#include "UmlPixmap.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "BrowserView.h"
#include "mu.h"

QList<BrowserObjectDiagram> BrowserObjectDiagram::imported;
QValueList<int> BrowserObjectDiagram::imported_ids;
QStringList BrowserObjectDiagram::its_default_stereotypes;	// unicode

BrowserObjectDiagram::BrowserObjectDiagram(QString s, BrowserNode * p, int id)
    : BrowserDiagram(s, p, id), window(0) {
  make();
  is_modified = (id == 0);
}

BrowserObjectDiagram::BrowserObjectDiagram(int id)
    : BrowserDiagram(id), window(0) {
  // not yet read
  make();
  is_modified = (id == 0);
}

BrowserObjectDiagram::BrowserObjectDiagram(BrowserObjectDiagram * model, BrowserNode * p)
    : BrowserDiagram(p->get_name(), p, 0), window(0) {
  def = new SimpleData(model->def);
  def->set_browser_node(this);
  comment = model->comment;
  settings = model->settings;
  note_color = model->note_color;
  fragment_color = model->fragment_color;
  package_color = model->package_color;
  class_instance_color = model->class_instance_color;
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

BrowserObjectDiagram::~BrowserObjectDiagram() {
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

void BrowserObjectDiagram::make() {
  def = new SimpleData;
  def->set_browser_node(this);
  note_color = UmlDefaultColor;
  fragment_color = UmlDefaultColor;
  package_color = UmlDefaultColor;
  class_instance_color = UmlDefaultColor;
}

BrowserObjectDiagram * BrowserObjectDiagram::add_object_diagram(BrowserNode * future_parent)
{
  QString name;
  
  if (future_parent->enter_child_name(name, "enter object diagram's name : ",
				      UmlObjectDiagram, TRUE, FALSE))
    return new BrowserObjectDiagram(name, future_parent);
  else
    return 0;
}

void BrowserObjectDiagram::set_name(const char * s) {
  BrowserDiagram::set_name(s);
  if (window != 0)
    window->setCaption(s);
}

void BrowserObjectDiagram::import()
{
  QValueList<int>::Iterator it = imported_ids.begin();
  
  while (!imported.isEmpty()) {
    QString warning;
    BrowserObjectDiagram * d = imported.take(0);
    
    (new ObjectDiagramWindow(d->full_name(), d, *it))->close(TRUE);
    it = imported_ids.remove(it);
    d->is_modified = TRUE;
  }
}

void BrowserObjectDiagram::renumber(int phase) {
  if (phase == -1)
    open(FALSE);
  else {
    new_ident(phase, all);
    window->get_view()->renumber(get_ident());
    is_modified = TRUE;
  }
}

void BrowserObjectDiagram::delete_it() {
  if (window)
    delete window;
  
  BrowserNode::delete_it();
}

BrowserNode * BrowserObjectDiagram::duplicate(BrowserNode * p, QString name) {
  BrowserObjectDiagram * result = new BrowserObjectDiagram(this, p);
  
  result->set_name((name.isEmpty()) ? get_name() : (const char *) name);
  result->update_stereotype();
  
  return result;
}

const QPixmap* BrowserObjectDiagram::pixmap(int) const {
  return (deletedp()) ? DeletedObjectDiagramIcon : ObjectDiagramIcon;
}

void BrowserObjectDiagram::draw_svg() const {
  fputs("\t<rect fill=\"white\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x=\"0\" y=\"0\" width=\"15\" height=\"15\" />\n"
	"\t<path fill=\"none\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" "
	"d=\"M 2 2 L 10 2 L 10 6 L 2 6 L 2 2 "
	"M 5 10 L 13 10 L 13 13 L 5 13 L 5 10 "
	"M 5 6 L 9 10\" />\n",
	svg());
}

void BrowserObjectDiagram::menu() {
  QPopupMenu m(0, name);
  QPopupMenu toolm(0);
  BrowserNode * item_above = 0;
  
  m.insertItem(new MenuTitle(name, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    m.setWhatsThis(m.insertItem("Show", 0),
		   "to show and edit the <em>object diagram</em>");
    if (!is_edited) {
      m.setWhatsThis(m.insertItem("Edit", 1), 
		     "to edit the <em>object diagram</em>");
      if (!is_read_only) {
	m.setWhatsThis(m.insertItem("Edit drawing settings", 2),
		       "to set how the <em>object diagram</em>'s items must be drawed");
	m.insertSeparator();
	m.setWhatsThis(m.insertItem("Duplicate", 3),
		       "to duplicate the <em>object diagram</em>");
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
	      if (!((BrowserNode *) item_above)->wrong_child_name(get_name(), UmlObjectDiagram, TRUE, FALSE)) {
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
	  m.setWhatsThis(m.insertItem("Delete", 4),
			 "to delete the <em>object diagram</em>. \
Note that you can undelete it after");
	}
      }
    }
    mark_menu(m, "object diagram", 90);
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("Tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0))
    m.setWhatsThis(m.insertItem("Undelete", 5),
		   "to undelete the <em>object diagram</em>");
  
  exec_menu_choice(m.exec(QCursor::pos()), item_above);
}

void BrowserObjectDiagram::exec_menu_choice(int rank,
					    BrowserNode * item_above) {
  switch (rank) {
  case 0:
    open(FALSE);
    break;
  case 1:
    edit("Object diagram", its_default_stereotypes);
    return;
  case 2:
    edit_settings();
    return;
  case 3:
    {
      QString name;
      
      if (((BrowserNode *)parent())->enter_child_name(name, "enter object diagram's name : ",
						      UmlObjectDiagram, TRUE, FALSE))
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

void BrowserObjectDiagram::apply_shortcut(QString s) {
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
  
  exec_menu_choice(choice, 0);
}

void BrowserObjectDiagram::open(bool) {
  if (window == 0)
    window = new ObjectDiagramWindow(full_name(TRUE), this);
  else
    window->raise();
  
  window->setFocus();
}

void BrowserObjectDiagram::edit_settings() {
  QArray<StateSpec> st;
  QArray<ColorSpec> co(4);
  
  settings.complete(st, TRUE);
  
  co[0].set("package color", &package_color);
  co[1].set("fragment color", &fragment_color);
  co[2].set("note color", &note_color);
  co[3].set("class instance \ncolor", &class_instance_color);
  
  SettingsDialog dialog(&st, &co, FALSE, FALSE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted) {
    DrawingSettings::modified();
    modified();
    package_modified();
  }
}

void BrowserObjectDiagram::on_close() {
  window = 0;
}

void BrowserObjectDiagram::read_session(char * & st) {
  window->read_session(st);
}

UmlCode BrowserObjectDiagram::get_type() const {
  return UmlObjectDiagram;
}

int BrowserObjectDiagram::get_identifier() const {
  return get_ident();
}

void BrowserObjectDiagram::get_objectdiagramsettings(ObjectDiagramSettings & r) const {
  if (! settings.complete(r))
    ((BrowserNode *) parent())->get_objectdiagramsettings(r);
}

void BrowserObjectDiagram::package_settings(bool & name_in_tab,
					    ShowContextMode & show_context) const {
  ObjectDiagramSettings st;
  
  get_objectdiagramsettings(st);
  name_in_tab = st.package_name_in_tab == UmlYes;
  show_context = st.show_context_mode;
}

UmlColor BrowserObjectDiagram::get_color(UmlCode who) const {
  UmlColor c;
  
  switch (who) {
  case UmlNote:
    c = note_color;
    break;
  case UmlPackage:
    c = package_color;
    break;
  case UmlFragment:
    c = fragment_color;
    break;
  default:
    c = class_instance_color;
  }
  
  return (c != UmlDefaultColor)
    ? c
    : ((BrowserNode *) parent())->get_color(who);
}

bool BrowserObjectDiagram::get_auto_label_position(UmlCode who) const {
  switch (settings.auto_label_position) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_auto_label_position(who);
  }
}

bool BrowserObjectDiagram::get_shadow() const {
  switch (settings.shadow) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_shadow(UmlObjectDiagram);
  }  
}

bool BrowserObjectDiagram::get_draw_all_relations() const {
  switch (settings.draw_all_relations) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_draw_all_relations(UmlObjectDiagram);
  }  
}

void BrowserObjectDiagram::dont_draw_all_relations() {
  settings.draw_all_relations = UmlNo;
}

bool BrowserObjectDiagram::get_classinstwritehorizontally(UmlCode) const {
  Uml3States h = settings.write_horizontally;
  
  return (h == UmlDefaultState)
    ? ((BrowserNode *) parent())->get_classinstwritehorizontally(UmlObjectDiagram)
    : (h == UmlYes);
}

BasicData * BrowserObjectDiagram::get_data() const {
  return def;
}

bool BrowserObjectDiagram::tool_cmd(ToolCom * com, const char * args) {
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
      ObjectDiagramWindow * w = new ObjectDiagramWindow(full_name(), this);
      
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

bool BrowserObjectDiagram::api_compatible(unsigned v) const {
  return v >= 24;
}

void BrowserObjectDiagram::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "objdiagram_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserObjectDiagram::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "objdiagram_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
}

void BrowserObjectDiagram::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "objectdiagram_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "objectdiagram " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    def->save(st, warning);
    settings.save(st);
    
    bool nl = FALSE;
    
    save_color(st, "class_instance_color", class_instance_color, nl);
    save_color(st, "note_color", note_color, nl);
    save_color(st, "fragment_color", fragment_color, nl);
    save_color(st, "package_color", package_color, nl);
    
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

BrowserObjectDiagram * BrowserObjectDiagram::read_ref(char * & st, char * k)
{
  if (strcmp(k, "objectdiagram_ref"))
    wrong_keyword(k, "objectdiagram_ref");
  
  int id = read_id(st);
  BrowserObjectDiagram * result = 
    (BrowserObjectDiagram *) all[id];
  
  return (result == 0)
    ? new BrowserObjectDiagram(id)
    : result;
}

BrowserObjectDiagram * BrowserObjectDiagram::read(char * & st, char * k,
					    BrowserNode * parent)
{
  BrowserObjectDiagram * r;
  int id;
  
  if (!strcmp(k, "objectdiagram_ref")) {
    if ((r = (BrowserObjectDiagram *) all[id = read_id(st)]) == 0)
      r = new BrowserObjectDiagram(id);
    return r;
  }
  else if (!strcmp(k, "objectdiagram")) {
    id = read_id(st);
    
    QString s = read_string(st);
    
    if ((r = (BrowserObjectDiagram *) all[id]) == 0)
      r = new BrowserObjectDiagram(s, parent, id);
    else if (r->is_defined) {
      BrowserObjectDiagram * already_exist = r;

      r = new BrowserObjectDiagram(s, parent, id);

      already_exist->must_change_id(all);
      already_exist->unconsistent_fixed("object diagram", r);
    }
    else {
      r->set_parent(parent);
      r->set_name(s);
    }
      
    r->is_defined = TRUE;

    r->is_read_only = (!in_import() && read_only_file()) || 
      (user_id() != 0) && r->is_api_base();
    
    QFileInfo fi(BrowserView::get_dir(), QString::number(id) + ".diagram");
    if (!in_import() && fi.exists() && !fi.isWritable())
      r->is_read_only = TRUE;
    
    k = read_keyword(st);
    
    r->def->read(st, k);				// updates k
    r->settings.read(st, k);				// updates k
    if (read_file_format() < 30)
      r->settings.draw_all_relations = UmlNo;
    read_color(st, "class_instance_color", r->class_instance_color, k); // updates k
    read_color(st, "note_color", r->note_color, k);	// updates k
    read_color(st, "fragment_color", r->fragment_color, k);	// updates k
    read_color(st, "package_color", r->package_color, k);	// updates k
    r->BrowserNode::read(st, k);			// updates k
    
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

BrowserNode * BrowserObjectDiagram::get_it(const char * k, int id)
{
  return (!strcmp(k, "objectdiagram_ref")) ? all[id] : 0;
}
