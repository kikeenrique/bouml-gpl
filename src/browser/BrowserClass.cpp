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
#include <qpainter.h>
#include <qcursor.h>
#include <qapplication.h>
#include <qfile.h>
#include <qdict.h>

#include "BrowserClass.h"
#include "BrowserAttribute.h"
#include "BrowserOperation.h"
#include "BrowserExtraMember.h"
#include "BrowserRelation.h"
#include "BrowserSimpleRelation.h"
#include "BrowserArtifact.h"
#include "BrowserComponent.h"
#include "ArtifactData.h"
#include "AttributeData.h"
#include "OperationData.h"
#include "RelationData.h"
#include "ClassData.h"
#include "BrowserClassView.h"
#include "BrowserClassDiagram.h"
#include "BrowserDeploymentView.h"
#include "UmlPixmap.h"
#include "UmlDrag.h"
#include "UmlGlobal.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "MenuItalic.h"
#include "ClassDialog.h"
#include "OperationListDialog.h"
#include "BrowserView.h"
#include "BrowserPackage.h"
#include "BrowserActivityObject.h"
#include "BrowserPin.h"
#include "BrowserParameter.h"
#include "BrowserExpansionNode.h"
#include "ReferenceDialog.h"
#include "mu.h"

IdDict<BrowserClass> BrowserClass::all(257);

QStringList BrowserClass::its_default_stereotypes;	// unicode
QStringList BrowserClass::relations_default_stereotypes[UmlRelations];	// unicode

BrowserClass::BrowserClass(QString s, BrowserNode * p, ClassData * d, int id)
    : BrowserNode(s, p), Labeled<BrowserClass>(all, id),
      def(d), associated_diagram(0), associated_artifact(0) {
}

BrowserClass::BrowserClass(const BrowserClass * model, BrowserNode * p)
    : BrowserNode(model->get_name(), p), Labeled<BrowserClass>(all, 0),
      associated_diagram(0), associated_artifact(0) {
  def = new ClassData(model->def, this);
  comment = model->comment;
}

BrowserClass::BrowserClass(int id)
    : BrowserNode(), Labeled<BrowserClass>(all, id),
      def(new ClassData), associated_diagram(0),
      associated_artifact(0) {
  // not yet read
  def->set_browser_node(this);
}

BrowserClass::~BrowserClass() {
  if (deletedp() && !modifiedp())
    QFile::remove(bodies_file());
  
  all.remove(get_ident());
  delete def;
}

void BrowserClass::delete_it() {
  BrowserNode::delete_it();
  
  if ((associated_artifact != 0) &&
      associated_artifact->is_writable())
    associated_artifact->remove_associated_class(this);
  
  QValueList<BrowserComponent *>::Iterator it;
  
  for (it = associated_components.begin();
       it != associated_components.end();
       it++)
    if ((*it)->is_writable())
      (*it)->remove_associated_class(this);
}

void BrowserClass::clear(bool old)
{
  all.clear(old);
  BrowserAttribute::clear(old);
  BrowserRelation::clear(old);
  BrowserOperation::clear(old);
  BrowserExtraMember::clear(old);
}

void BrowserClass::update_idmax_for_root()
{
  all.update_idmax_for_root();
  BrowserAttribute::update_idmax_for_root();
  BrowserRelation::update_idmax_for_root();
  BrowserOperation::update_idmax_for_root();
  BrowserExtraMember::update_idmax_for_root();
}
    
void BrowserClass::referenced_by(QList<BrowserNode> & l) {
  IdIterator<BrowserClass> it(all);
  
  while (it.current()) {
    if (!it.current()->deletedp() &&
	(it.current() != this) &&
	it.current()->def->reference(this))
      l.append(it.current());
    ++it;
  }
  
  BrowserNode::referenced_by(l);
  BrowserRelation::compute_referenced_by(l, this);
  BrowserSimpleRelation::compute_referenced_by(l, this);
  BrowserAttribute::compute_referenced_by(l, this);
  BrowserOperation::compute_referenced_by(l, this);
  BrowserArtifact::compute_referenced_by(l, this);
  BrowserComponent::compute_referenced_by(l, this);
  BrowserActivityObject::compute_referenced_by(l, this);
  BrowserPin::compute_referenced_by(l, this);
  BrowserParameter::compute_referenced_by(l, this);
  BrowserExpansionNode::compute_referenced_by(l, this);
}

void BrowserClass::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
  BrowserNode::renumber(phase);
}

bool BrowserClass::new_java_enums(QString new_st)
{
  IdIterator<BrowserClass> it(all);
  bool result = FALSE;
  
  while (it.current()) {
    ClassData * d = it.current()->def;
    const char * st = d->get_stereotype();
    
    if (*st != 0) {
      if (strcmp(st, "enum_pattern") == 0) {
	d->set_stereotype(new_st);
	if (it.current()->is_read_only)
	  result = TRUE;
	else
	  it.current()->package_modified();
      }
      else if ((strcmp(st, "enum") == 0) && (*(d->get_javadecl()) != 0)) {
	d->set_stereotype("enum_pattern");
	if (it.current()->is_read_only)
	  result = TRUE;
	else
	  it.current()->package_modified();
      }
    }
    
    ++it;
  }

  if (new_st.isEmpty())
    new_st = "enum_pattern";
  its_default_stereotypes.append(new_st);
  
  return result;
}

const QPixmap* BrowserClass::pixmap(int) const {
  bool actor = (strcmp(def->get_stereotype(), "actor") == 0);
  bool tmplt = def->get_n_formalparams();
  
  if (deletedp()) {
    if (actor)
      return DeletedActorIcon;
    else if (tmplt)
      return DeletedTemplateIcon;
    else
      return DeletedClassIcon;
  }
  
  if (actor)
    return ActorIcon;
  else if (! nestedp())
    return (tmplt) ? TemplateIcon : ClassIcon;
  else {
    switch (def->get_uml_visibility()) {
    case UmlPublic:
      return (tmplt) ? PublicEmbeddedTemplateIcon
		     : PublicEmbeddedClassIcon;
    case UmlProtected:
      return (tmplt) ? ProtectedEmbeddedTemplateIcon
		     : ProtectedEmbeddedClassIcon;
    case UmlPrivate:
      return (tmplt) ? PrivateEmbeddedTemplateIcon
		     : PrivateEmbeddedClassIcon;
    default:
      return (tmplt) ? PackageEmbeddedTemplateIcon
		     : PackageEmbeddedClassIcon;
    }
  }
}

void BrowserClass::paintCell(QPainter * p, const QColorGroup & cg, int column,
			     int width, int alignment) {
  const QColor & bg = p->backgroundColor();
  
  if (is_marked) {
    p->setBackgroundMode(OpaqueMode);
    p->setBackgroundColor(UmlRedColor);
  }
    
  p->setFont((def->get_is_abstract())
	     ? ((is_writable()) ? BoldItalicFont : ItalicFont)
	     : ((is_writable()) ? BoldFont : NormalFont));
  QListViewItem::paintCell(p, cg, column, width, alignment);
  
  if (is_marked) {
    p->setBackgroundMode(TransparentMode);
    p->setBackgroundColor(bg);
  }
}

QString BrowserClass::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);

  if (p.isEmpty()) 
    return QString((const char *) name);
  else if (rev)
    return name + "   [" + p + "]";
  else
    return p + "::" + name;
}

void BrowserClass::member_cpp_def(const QString &, const QString &, 
				  QString & s, bool) const {
  ClassDialog::cpp_generate_members_def(this, s);
}

void BrowserClass::menu() {
  QList<BrowserOperation> l = inherited_operations(21);
  QPopupMenu m(0);
  QPopupMenu gensubm(0);
  QPopupMenu inhopersubm(0);
  QPopupMenu compsubm(0);
  QPopupMenu toolm(0);
  int index;
  
  m.insertItem(new MenuTitle(name, m.font()), -1);
  if (!deletedp()) {
    if (!is_read_only) {
      if (edition_number == 0) {
	m.insertSeparator();
	
	const char * stereotype = def->get_stereotype();
	
	if (!strcmp(stereotype, "enum") || !strcmp(stereotype, "enum_pattern"))
	  m.setWhatsThis(m.insertItem("Add item", 8),
			 "to add an <em>item</em> to the <em>enum</em>");
	
	if (strcmp(stereotype, "typedef") && strcmp(stereotype, "enum_pattern")) {
	  m.setWhatsThis(m.insertItem("Add attribute", 0),
			 "to add an <em>attribute</em> to the <em>class</em>");

	  m.setWhatsThis(m.insertItem("Add operation", 1),
			 "to add an <em>operation</em> to the <em>class</em>");
	  if ((l.count() != 0) && strcmp(stereotype, "union")) {
	    if (l.count() > 20)
	      m.setWhatsThis(m.insertItem("Add inherited operation", 9999),
			     "to redefine an inherited <em>operation</em> in the <em>class</em>");
	    else {
	      BrowserOperation * oper;
	      
	      inhopersubm.insertItem(new MenuTitle("Choose operation to add it", m.font()), -1);
	      inhopersubm.insertSeparator();
	      
	      for (oper = l.first(), index = 10000;
		   oper;
		   oper = l.next(), index += 1) {
		if (((OperationData *) oper->get_data())->get_is_abstract())
		  inhopersubm.insertItem(new MenuItalic(((BrowserNode *) oper->parent())->get_name() +
							QString("::") + oper->get_data()->definition(TRUE),
							inhopersubm.font()),
					 index);
		else
		  inhopersubm.insertItem(((BrowserNode *) oper->parent())->get_name() +
					 QString("::") + oper->get_data()->definition(TRUE),
					 index);
	      }
	      
	      m.setWhatsThis(m.insertItem("Add inherited operation", &inhopersubm),
			     "to redefine an inherited <em>operation</em> in the <em>class</em>");
	    }
	  }
	  if (strcmp(stereotype, "enum") && strcmp(stereotype, "enum_pattern")) {
	    m.setWhatsThis(m.insertItem("Add nested class", 14),
			   "to add an <em>nested class</em> to the <em>class</em>");
	  }
	  m.setWhatsThis(m.insertItem("Add extra member", 2),
			 "to add an <em>extra member</em> to the <em>class</em>");
	}
	m.insertSeparator();
	m.setWhatsThis(m.insertItem("Edit", 3),
		       "to edit the <em>class</em>, \
a double click with the left mouse button does the same thing");
	m.setWhatsThis(m.insertItem("Duplicate", 13),
		       "to duplicate the <em>class</em>");
	m.insertSeparator();
	m.setWhatsThis(m.insertItem("Delete", 4),
		       "to delete the <em>class</em>. \
Note that you can undelete it after");
	
	if ((associated_artifact == 0) &&
	    (((BrowserNode *) parent())->get_type() == UmlClassView)) {
	  BrowserNode * bcv = ((BrowserClassView *) parent())->get_associated();
	  
	  if ((bcv != 0) && !bcv->deletedp()) {
	    QListViewItem * child;
	    
	    for (child = bcv->firstChild(); child; child = child->nextSibling())
	      if (!((BrowserNode *) child)->deletedp() &&
		  (((BrowserNode *) child)->get_type() == UmlArtifact) &&
		  (((BrowserNode *) child)->get_name() == name))
		break;
	    
	    if (child == 0) {
	      // no artifact having the same name, propose to create it
	      m.insertSeparator();
	      m.setWhatsThis(m.insertItem("Create source artifact", 5),
			     "to add a <em>&lt;&lt;source&gt;&gt; artifact</em> in the <em>deployment view</em> associated to \
the <em>class view</em>, this artifact will contain the generated code of the class");
	    }
	  }
	}
      }
    }
    else
      m.setWhatsThis(m.insertItem("Edit", 3),
		     "to edit the <em>class</em>, \
a double click with the left mouse button does the same thing");
    
    bool have_sep = FALSE;
    
    if ((associated_artifact != 0) && !associated_artifact->deletedp()) {
      m.insertSeparator();
      have_sep = TRUE;
      m.setWhatsThis(m.insertItem("Select associated artifact", 5),
		     "to select the associated <em>&lt;&lt;source&gt;&gt; artifact</em>");
    }
    
    if (! associated_components.isEmpty()) {
      if (! have_sep)
	m.insertSeparator();
      
      if (associated_components.first() == associated_components.last())
	// only one component
	m.setWhatsThis(m.insertItem("Select associated component", 100000),
		       "to select a <em>component</em> realizing or providing the <em>class</em>");
      else {
	compsubm.insertItem(new MenuTitle("Choose component", m.font()), -1);
	compsubm.insertSeparator();
	
	m.setWhatsThis(m.insertItem("Select an associated component", &compsubm),
		       "to select a <em>component</em> realizing or providing the <em>class</em>");
	
	index = 100000;
	
	QValueList<BrowserComponent *>::Iterator it;
	
	for (it = associated_components.begin(), index = 100000;
	     it != associated_components.end();
	     it++, index += 1)
	  compsubm.insertItem((*it)->full_name(TRUE), index);
      }
    }
    
    m.insertSeparator();
    m.setWhatsThis(m.insertItem("Referenced by", 15),
		   "to know who reference the <i>class</i>");
    mark_menu(m, "class", 90);
    if (! nestedp()) {
      m.insertSeparator();
      m.insertItem("Generate", &gensubm);    
      gensubm.insertItem("C++", 10);
      gensubm.insertItem("Java", 11);
      gensubm.insertItem("Idl", 12);
    }
    if ((edition_number == 0) && 
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("Tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0)) {
    m.setWhatsThis(m.insertItem("Undelete", 6),
		   "undelete the <em>class</em>. \
Do not undelete its <em>attributes</em>, <em>operations</em> and <em>relations</em>");
 
    QListViewItem * child;
  
    for (child = firstChild(); child != 0; child = child->nextSibling()) {
      if (((BrowserNode *) child)->deletedp()) {
	m.setWhatsThis(m.insertItem("Undelete recursively", 7),
		       "undelete the <em>class</em> and its \
nested <em>classes</em>, <em>attributes</em>, <em>operations</em> and \
<em>relations</em> (except if the class at the other side is also deleted)");
	break;
      }
    }
  }

  exec_menu_choice(m.exec(QCursor::pos()), l);
}

void BrowserClass::exec_menu_choice(int index,
				    QList<BrowserOperation> & l) {
  switch (index) {
  case 0:
  case 8:
    {
      BrowserNode * bn = add_attribute(0, index == 8);
      
      if (bn != 0)
	bn->open(TRUE);
    }
    return;
  case 1:
    {
      BrowserNode * bn = add_operation();
      
      if (bn != 0)
	bn->open(TRUE);
    }
    return;
  case 2:
    {
      BrowserNode * bn = add_extra_member();
      
      if (bn != 0)
	bn->open(TRUE);
    }
    return;
  case 3:
    // modal edition
    def->edit();
    return;
  case 4:
    delete_it();
    break;
  case 5:
    if (associated_artifact == 0) {
      BrowserArtifact * ba = 
	new BrowserArtifact(name,
			     ((BrowserClassView *) parent())->get_associated());
      
      ba->get_data()->set_stereotype("source");
      set_associated_artifact(ba);
    }
    else {
      associated_artifact->select_in_browser();
      return;
    }
    break;
  case 6:
    BrowserNode::undelete(FALSE);
    break;
  case 7:
    BrowserNode::undelete(TRUE);
    break;
  case 10:
    {
      bool preserve = preserve_bodies();
      
      ToolCom::run((verbose_generation()) 
		   ? ((preserve) ? "cpp_generator -v -p" : "cpp_generator -v")
		   : ((preserve) ? "cpp_generator -p" : "cpp_generator"),
		   this);
    }
    return;
  case 11:
    {
      bool preserve = preserve_bodies();
      
      ToolCom::run((verbose_generation()) 
		   ? ((preserve) ? "java_generator -v -p" : "java_generator -v")
		   : ((preserve) ? "java_generator -p" : "java_generator"), 
		   this);
    }
    return;
  case 12:
    ToolCom::run((verbose_generation()) ? "idl_generator -v" : "idl_generator", this);
    return;
  case 13:
    {
      QString name;
      
      if (((BrowserNode *) parent())->enter_child_name(name, "enter class's name : ",
						       UmlClass, FALSE, FALSE))
	duplicate((BrowserNode *) parent(), name)->select_in_browser();
    }
    break;
  case 14: 
    {
      BrowserClass * cl = add_class(this);
      
      if (cl != 0)
	cl->select_in_browser();
    }
    break;
  case 15:
    ReferenceDialog::show(this);
    return;
#if 0
  case 16:
    {
      AssocComponentDialog dialog(associated_components);
      
      dialog.raise();
      dialog.exec();
    }
    return;
#endif
  case 9999:
    {
      l = inherited_operations(~0u);
      
      OperationListDialog dialog("Choose inherited operation", l);
      
      dialog.raise();
      if (dialog.exec() != QDialog::Accepted)
	return;
      index = dialog.choosen() + 10000;
    }
    // no break
  default:
    if (index >= 100000)
      // assoc comp
      associated_components[index - 100000]->select_in_browser();
    else if (index >= 10000)
      // inherited operation
      add_inherited_operation(l.at(index - 10000));
    else if (index >= 100)
      ToolCom::run(Tool::command(index - 100), this);
    else
      mark_management(index - 90);
    return;
  }
  package_modified();
}

void BrowserClass::apply_shortcut(QString s) {
  int choice = -1;

  if (!deletedp()) {
    if (!is_read_only) {
      if (edition_number == 0) {
	
	const char * stereotype = def->get_stereotype();
	
	if (!strcmp(stereotype, "enum") || !strcmp(stereotype, "enum_pattern"))
	  if (s == "New item")
	    choice = 8;
	
	if (strcmp(stereotype, "typedef") && strcmp(stereotype, "enum_pattern")) {
	  if (s == "New attribute")
	    choice = 0;
	  else if (s == "New operation")
	    choice = 1;
	  if (strcmp(stereotype, "enum") && strcmp(stereotype, "enum_pattern")) {
	    if (s == "New class")
	      choice = 14;
	  }
	  if (s == "New extra member")
	    choice = 2;
	}
	if (s == "Edit")
	  choice = 3;
	else if (s == "Duplicate")
	  choice = 13;
	else if (s == "Delete")
	  choice = 4;
	
	if ((associated_artifact == 0) &&
	    (((BrowserNode *) parent())->get_type() == UmlClassView)) {
	  BrowserNode * bcv = ((BrowserClassView *) parent())->get_associated();
	  
	  if ((bcv != 0) && !bcv->deletedp()) {
	    QListViewItem * child;
	    
	    for (child = bcv->firstChild(); child; child = child->nextSibling())
	      if (!((BrowserNode *) child)->deletedp() &&
		  (((BrowserNode *) child)->get_type() == UmlArtifact) &&
		  (((BrowserNode *) child)->get_name() == name))
		break;
	    
	    if (child == 0) {
	      // no artifact having the same name, propose to create it
	      if (s == "Create source artifact")
		choice = 5;
	    }
	  }
	}
      }
    }
    else
      if (s == "Edit")
	choice = 3;
    
    if ((associated_artifact != 0) && !associated_artifact->deletedp()) {
      if (s == "Select associated artifact")
	choice = 5;
    }
    
    if (s == "Referenced by")
      choice = 15;
    mark_shortcut(s, choice, 90);
    if (edition_number == 0)
      Tool::shortcut(s, choice, get_type(), 100);
    if (! nestedp()) {
      if (s == "Generate C++")
	choice = 10;
      else if (s == "Generate Java")
	choice = 11;
      else if (s == "Generate Idl")
	choice = 12;
    }
  }
  else if (!is_read_only && (edition_number == 0)) {
    if (s == "Undelete")
      choice = 6;
 
    QListViewItem * child;
  
    for (child = firstChild(); child != 0; child = child->nextSibling()) {
      if (((BrowserNode *) child)->deletedp()) {
	if (s == "Undelete recursively")
	  choice = 7;
	break;
      }
    }
  }
    
  QList<BrowserOperation> l;
  
  exec_menu_choice(choice, l);
}

BrowserNode * BrowserClass::duplicate(BrowserNode * p, QString name) {
  BrowserClass * result = new BrowserClass(this, p);
  
  result->set_name((name.isEmpty()) ? get_name() : (const char *) name);
  
  // count the current sub elt to not take into account two time the
  // bi-dir self relations
  unsigned n = 0;
  QListViewItem * child;
  
  for (child = firstChild(); child != 0; child = child->nextSibling())
    n += 1;
  
  // duplicates the sub elts
  for (child = firstChild(); n != 0;child = child->nextSibling(), n -= 1) {
    if (!((BrowserNode *) child)->deletedp()) {
      if (IsaRelation(((BrowserNode *) child)->get_type())) {
	RelationData * rd = (RelationData *)
	  ((BrowserNode *) child)->get_data();
	
	if ((rd->get_start_class() != this) ||
	    (rd->get_end_class() != this) ||
	    rd->is_a((BrowserRelation*) child))
	  ((BrowserNode *) child)->duplicate(result);
      }
      else
	((BrowserNode *) child)->duplicate(result);
    }
  }
  
  result->update_stereotype(TRUE);
  
  return result;
}
	
void BrowserClass::set_name(const char * s) {
  if (name != s) {    
    bool firsttime = name.isEmpty();
    
    if (! firsttime) {
      // rename constructors and destructors
      QListViewItem * child;
      
      for (child = firstChild(); child != 0; child = child->nextSibling()) {
	if (!((BrowserNode *) child)->deletedp() &&
	    (((BrowserNode *) child)->get_type() == UmlOperation)) {
	  const char * op_name = ((BrowserNode *) child)->get_name();
	  
	  if (op_name == name)
	    // renamed
	    ((BrowserNode *) child)->set_name(s);
	  else if ((*((const char *) op_name) == '~') &&
		   ((((const char *) op_name) + 1) == name)) {
	    // destructor renamed
	    QCString d = "~";
			    
	    d += s;
	    ((BrowserNode *) child)->set_name(d);
	  }
	}
      }
      
      name = s;
    
      // to update inherit/realize/dependency names
      // else set by BrowserNode::post_load()
      BrowserView::get_project()->update_stereotype(TRUE);
      
      // rename constructors and destructors
      
    }
    else
      name = s;
  }
}

BrowserNode * BrowserClass::add_attribute(BrowserAttribute * attr,
					  bool enum_item) {
  QString name;
  
  if (enter_child_name(name, (enum_item) ? "enter item's name : "
					 : "enter attribute's name : ",
		       UmlAttribute, FALSE, FALSE)) {
    attr = (attr == 0) ? BrowserAttribute::new_one(name, this, enum_item)
		       : (BrowserAttribute *) attr->duplicate(this, name);
    
    setOpen(TRUE);
    def->modified();
    package_modified();
    attr->select_in_browser();
    if (enum_item)
      ((AttributeData *) attr->get_data())->set_visibility(UmlPublic);
    
    return attr;
  }
  
  return 0;
}

BrowserNode * BrowserClass::add_extra_member(BrowserExtraMember * em) {
  em = (em == 0) ? BrowserExtraMember::new_one(QString::null, this)
		 : (BrowserExtraMember *) em->duplicate(this, name);
  
  setOpen(TRUE);
  def->modified();
  package_modified();
  em->select_in_browser();
  
  return em;
}

BrowserNode * BrowserClass::add_relation(BrowserRelation * rel) {
  // the relation is an association, not a dependency/inheritance
  rel = (BrowserRelation *) rel->duplicate(this);
  
  def->modified();
  package_modified();
  rel->select_in_browser();
  
  return rel;
}

BrowserNode * BrowserClass::add_operation(BrowserOperation * oper) {
  QString name;
  
  if (enter_child_name(name, "enter operation's name : ",
		       UmlOperation, FALSE, FALSE)) {
    oper = (oper == 0) ? BrowserOperation::new_one(name, this)
		       : (BrowserOperation *) oper->duplicate(this);

    setOpen(TRUE);
    def->modified();
    package_modified();
    oper->set_name(name);
    oper->select_in_browser();
   
    return oper;
  }
  
  return 0;
}

BrowserNode * BrowserClass::add_inherited_operation(BrowserOperation * model) {
  BrowserOperation * oper = new BrowserOperation(model, this);
  
  ((OperationData *) oper->get_data())->set_is_abstract(0);
  setOpen(TRUE);
  def->modified();
  package_modified();
  oper->select_in_browser();
  
  return oper;
}

const char * BrowserClass::may_start(UmlCode l) const {
  const char * stereotype = def->get_stereotype();
  
  if (!strcmp(stereotype, "typedef")) {
    switch (l) {
    case UmlGeneralisation:
    case UmlRealize:
      if (is_read_only && !root_permission())
	return "read only";
      else {
	BrowserNodeList inh;
	
	children(inh, UmlGeneralisation, UmlRealize);
	return (inh.count() <= 1)
	  ? 0
	  : "typedef can't generalize / realize several times";
      }
    case UmlDependency:
      if (is_read_only && !root_permission())
	return "read only";
      // no break
    case UmlAnchor:
      return 0;
    default:
      return "a typedef can't have relation";
    }
  }
  
  switch (l) {
  case UmlGeneralisation:
  case UmlRealize:
    if (!strcmp(stereotype, "union"))
      return "an union can't generalize / realize";
    // no break;
  case UmlAssociation:
  case UmlDirectionalAssociation:
  case UmlAggregation:
  case UmlAggregationByValue:
  case UmlDirectionalAggregation:
  case UmlDirectionalAggregationByValue:
  case UmlDependency:
    return (!is_read_only || root_permission()) ? 0 : "read only";
  default:
    return 0;
  }
}

const char * BrowserClass::may_connect(UmlCode l, BrowserClass * other) {
  switch (l) {
  case UmlGeneralisation:
  case UmlRealize:
    return check_inherit(other);
  case UmlAssociation:
  case UmlAggregation:
  case UmlAggregationByValue:
    return (!other->is_read_only || root_permission()) ? 0 : "read only";
  default:
    return (l != UmlAnchor) ? 0 : "can't have anchor between classes";
  }
}

BasicData * BrowserClass::add_relation(UmlCode t, BrowserNode * end) {
  if (IsaRelation(t)) {
    RelationData * d = new RelationData(t);
  
    BrowserRelation * br1 = new BrowserRelation(this, d); 
    BrowserRelation * br2 = d->set_start_end(br1, (BrowserClass *) end);
  
    br1->modified();	// updates name
    br1->package_modified();
    if (br2 != 0) {
      br2->modified();	// updates name
      br2->package_modified();
    }
  
    switch (t) {
    case UmlGeneralisation:
    case UmlRealize:
      def->inherit_or_instantiate((BrowserClass *) end);
      break;
    default:
      break;
    }
  
    return d;
  }
  else
    return BrowserNode::add_relation(t, end);
}

void BrowserClass::open(bool force_edit) {
  if (!force_edit && 
      (associated_diagram != 0) &&
      !associated_diagram->deletedp())
    associated_diagram->open(FALSE);
  else if (edition_number == 0)
    // modal edition
    def->edit();
}

UmlCode BrowserClass::get_type() const {
  return UmlClass;
}

void BrowserClass::modified() {
  repaint();
  def->modified();
  ((BrowserNode *) parent())->modified();
}

BasicData * BrowserClass::get_data() const {
  return def;
}

bool BrowserClass::allow_spaces() const {
  return FALSE;
}


// compute all parents, grand parents ...
void BrowserClass::get_all_parents(QList<BrowserClass> & l) const {
  QList<BrowserNode> notyet = parents();

  while (! notyet.isEmpty()) {
    BrowserClass * cl = (BrowserClass *) notyet.getFirst();
    notyet.removeFirst();
    
    if ((cl != this) && (l.findRef(cl) == -1)) {
      l.append(cl);
      
      QList<BrowserNode> grand_parents = cl->parents();
      BrowserNode * gp;
      
      for (gp = grand_parents.first(); gp; gp = grand_parents.next())
	if (notyet.findRef(gp) == -1)
	  notyet.append(gp);
    }
  }
}

// returns all operations included the inherited (except the redefined ones
// and the contructor and destructor of parents)

void BrowserClass::get_opers(QValueList<const OperationData *> & opers,
			     QStringList & list) {
  QList<BrowserClass> all_parents;
  
  get_all_parents(all_parents);
  
  // own operations
  QListViewItem * child;
  QDict<void> already(499);
  
  for (child = firstChild(); child; child = child->nextSibling()) {
    if ((((BrowserNode *) child)->get_type() == UmlOperation) &&
	!((BrowserNode *) child)->deletedp()) {
      OperationData * od =
	(OperationData *) ((BrowserNode *) child)->get_data();
      QString msg = od->definition(TRUE);
	
      opers.append(od);
      list.append(msg);
      already.insert(msg, (void *) 1);
    }
  }
  
  // inherited
  
  BrowserClass * cl;
  
  for (cl = all_parents.first(); cl != 0; cl = all_parents.next()) {
    for (child = cl->firstChild(); child; child = child->nextSibling()) {
      if ((((BrowserNode *) child)->get_type() == UmlOperation) &&
	  (((BrowserNode *) child)->get_name() != cl->name) &&		// not a constructor
	  (((BrowserNode *) child)->get_name()[0] != '~')) {// not a destructor
	OperationData * data = (OperationData *)
	  ((BrowserOperation *) child)->get_data();
	QString profile = data->definition(TRUE);
	
	if (already.find(profile) == 0) {
	  // not yet added
	  opers.append(data);
	  list.append(profile);
	  already.insert(profile, (void *) 1);
	}
      }
    }
  }
}
  

void BrowserClass::DragMoveEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, UmlClass) ||
      UmlDrag::canDecode(e, UmlAttribute) ||
      UmlDrag::canDecode(e, UmlOperation) ||
      UmlDrag::canDecode(e, UmlExtraMember) ||
      UmlDrag::canDecode(e, BrowserOperation::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserRelation::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserSimpleRelation::drag_key(this)))
    e->accept();
  else
    ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserClass::DragMoveInsideEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, UmlClass) ||
      UmlDrag::canDecode(e, UmlAttribute) ||
      UmlDrag::canDecode(e, UmlOperation) ||
      UmlDrag::canDecode(e, UmlExtraMember) ||
      UmlDrag::canDecode(e, BrowserOperation::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserRelation::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserSimpleRelation::drag_key(this)))
    e->accept();
  else
    e->ignore();
}

bool BrowserClass::may_contains_them(const QList<BrowserNode> & l,
				     bool & duplicable) const {
  QListIterator<BrowserNode> it(l);
  
  for (; it.current(); ++it) {
    switch (it.current()->get_type()) {
    case UmlOperation:
      if (((BrowserOperation *) it.current())->get_get_of())
	if (l.containsRef(((BrowserOperation *) it.current())->get_get_of())
	    == 0)
	  return FALSE;
      if (((BrowserOperation *) it.current())->get_set_of())
	if (l.containsRef(((BrowserOperation *) it.current())->get_set_of())
	    == 0)
	  return FALSE;
      break;
    case UmlAttribute:
      if (((BrowserAttribute *) it.current())->get_get_oper())
	if (l.containsRef(((BrowserAttribute *) it.current())->get_get_oper())
	    == 0)
	  return FALSE;
      if (((BrowserAttribute *) it.current())->get_set_oper())
	if (l.containsRef(((BrowserAttribute *) it.current())->get_set_oper())
	    == 0)
	  return FALSE;
      break;
    case UmlClass:
    case UmlExtraMember:
      break;
    case UmlDependOn:
      if (((const BrowserNode *) it.current()->parent()) != this)
	return FALSE;
      duplicable = FALSE;
      return TRUE;
    default:
      if (!IsaRelation(it.current()->get_type()) ||
	  (((const BrowserNode *) it.current()->parent()) != this))
	return FALSE;
      duplicable = FALSE;
      return TRUE;
    }
    
    if (! may_contains(it.current(), FALSE))
      return FALSE;
    
    duplicable = may_contains_it(it.current());
  }
  
  return TRUE;
}

void BrowserClass::move(BrowserNode * bn, BrowserNode * after) {
  if ((bn->get_type() == UmlClass) && !bn->nestedp()) {
    ((BrowserClass *) bn)->def->set_uml_visibility(UmlProtected);
    ((BrowserClass *) bn)->def->set_cpp_visibility(UmlDefaultVisibility);
    ((BrowserClass *) bn)->set_associated_artifact(0);
  }

  BrowserNode * old = ((BrowserNode *) bn->parent());
  char * cpp;
  char * java;
  
  if ((old != this) && (bn->get_type() == UmlOperation)) {
    OperationData * d = (OperationData *) bn->get_data();
    
    cpp = d->get_body(TRUE);
    java = d->get_body(FALSE);
    d->create_modified_body_file();
  }
  else
    cpp = java = 0;
  
  if (after)
    bn->moveItem(after);
  else {
    bn->parent()->takeItem(bn);
    insertItem(bn);
  }
  
  if (old != this) {
    old->modified();
    old->package_modified();
    
    if (bn->get_type() == UmlOperation) {
      OperationData * d = (OperationData *) bn->get_data();
      
      if (cpp) {
	d->new_body(cpp, TRUE);
	delete [] cpp;
      }
      if (java) {
	d->new_body(java, FALSE);
	delete [] java;
      }
      
      if (d->get_is_abstract())
	def->set_is_abstract(TRUE);
    }
  }
  
  modified();
  package_modified();
}

void BrowserClass::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserClass::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  BrowserNode * bn;
  
  if ((((bn = UmlDrag::decode(e, UmlClass)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlAttribute)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlOperation)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlExtraMember)) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserOperation::drag_key(this))) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserRelation::drag_key(this))) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserSimpleRelation::drag_key(this))) != 0)) &&
      (bn != after) && (bn != this)) {
    if (may_contains(bn, bn->get_type() == UmlClass))  {
      if ((after == 0) &&
	  (bn->get_type() == UmlClass) &&
	  ((BrowserNode *) parent())->may_contains(bn, TRUE)) {
	// have choice
	QPopupMenu m(0);
  
	m.insertItem(new MenuTitle(bn->get_name() + QString(" moving"),
				   m.font()), -1);
	m.insertSeparator();
	m.insertItem("In " + QString(get_name()), 1);
	m.insertItem("After " + QString(get_name()), 2);
	
	switch (m.exec(QCursor::pos())) {
	case 1:
	  break;
	case 2:
	  ((BrowserNode *) parent())->DropAfterEvent(e, this);
	  return;
	default:
	  e->ignore();
	  return;
	}
      }
      move(bn, after);
    }
    else if (after == 0)
      ((BrowserNode *) parent())->DropAfterEvent(e, this);
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

BrowserClass * BrowserClass::get_class(BrowserNode * future_parent,
				       const char * stereotype)
{
  BrowserNode * old;
  QString name;
  BrowserNodeList nodes;
  
  if (!future_parent->enter_child_name(name, "enter class's name : ",
				       UmlClass, instances(nodes), &old,
				       FALSE, FALSE))
    return 0;
    
  if (old != 0)
    return ((BrowserClass *) old);
  
  ClassData * cd = new ClassData();
  
  BrowserClass * r = new BrowserClass(name, future_parent, cd);
  
  cd->set_browser_node(r);
  if (stereotype != 0)
    cd->set_stereotype("actor");  
  future_parent->setOpen(TRUE);
  future_parent->package_modified();
  
  return r;
}

BrowserClass * BrowserClass::add_class(BrowserNode * future_parent, QString name)
{
  if (name.isEmpty() &&
      !future_parent->enter_child_name(name, "enter class's name : ",
				       UmlClass, FALSE, FALSE))
    
    return 0;
  
  ClassData * cd = new ClassData();
  BrowserClass * r = new BrowserClass(name, future_parent, cd);
  
  cd->set_browser_node(r);
  return r;
}

QList<BrowserNode> BrowserClass::parents() const {
  QList<BrowserNode> l;
  QListViewItem * child;
    
  for (child = firstChild(); child != 0; child = child->nextSibling()) {
    BrowserNode * ch = ((BrowserNode *) child);
    
    switch (ch->get_type()) {
    case UmlGeneralisation:
    case UmlRealize:
      if (! ch->deletedp())
	l.append(((RelationData *) ch->get_data())->get_end_class());
    default:
      break;
    }
  }
  
  return l;
}

const char * BrowserClass::check_inherit(const BrowserNode * new_parent) const {
#if 0
  // To allows to inherit sereval times on the same template class
  // there are many problems. The only way is probably to memorize
  // the inheritance relation more/rather than the parent class in
  // the actuals, else it is not possible to update the actuals when 
  // the number of formals of an inherited class is changed
  if ((new_parent->get_type() == UmlClass) &&
      (((ClassData *) new_parent->get_data())->get_n_formalparams() != 0))
    // don't check more templates inheritance
    return TRUE;
#endif
  
  const char * parent_stereotype = 
    new_parent->get_data()->get_stereotype();
  
  return (!strcmp(parent_stereotype, "union"))
    ? "union can't generalize / realize"
    : BrowserNode::check_inherit(new_parent);
}
  
QList<BrowserOperation> BrowserClass::inherited_operations(unsigned limit) const {
  QApplication::setOverrideCursor(Qt::waitCursor);
  
  QList<BrowserClass> all_parents;
  
  get_all_parents(all_parents);

  // computes own operations profile list
  QListViewItem * child;
  QDict<BrowserOperation> already(499);
  
  for (child = firstChild(); child; child = child->nextSibling())
    if ((((BrowserNode *) child)->get_type() == UmlOperation) &&
	!((BrowserNode *) child)->deletedp())
      already.insert(((BrowserOperation *) child)->get_data()->definition(TRUE),
		     (BrowserOperation *) child);
	
  // computes undefined inherited operations list
  QList<BrowserOperation> l;
  BrowserClass * cl;

  for (cl = all_parents.first(); cl != 0; cl = all_parents.next()) {
    for (child = cl->firstChild(); child; child = child->nextSibling()) {
      if ((((BrowserNode *) child)->get_type() == UmlOperation) &&
	  !((BrowserNode *) child)->deletedp() &&
	  (((BrowserNode *) child)->get_name() != cl->name) &&		// not a constructor
	  (((BrowserNode *) child)->get_name()[0] != '~')) {// not a destructor
	OperationData * data = (OperationData *)
	  ((BrowserOperation *) child)->get_data();
	
	if ((data->get_uml_visibility() != UmlPrivate) &&
	    !data->get_isa_class_operation()) {
	  // may be refined
	  QString profile = data->definition(TRUE);
	  BrowserOperation * other = already.find(profile);
	  
	  if (other == 0) {
	    l.append((BrowserOperation *) child);
	    already.insert(profile, (BrowserOperation *) child);
	    if (l.count() >= limit) {
	      QApplication::restoreOverrideCursor();
	      
	      return l;
	    }
	  }
	  else if (! data->get_is_abstract() &&
		   ((OperationData *) other->get_data())
		   ->get_is_abstract()) {
	    // to know later if the oper is abstract or not
	    already.replace(profile, (BrowserOperation *) child);
	    l.removeRef(other);
	    l.append((BrowserOperation *) child);
	  }
	}
      }
    }
  }
  
  QApplication::restoreOverrideCursor();
  
  return l;
}

bool BrowserClass::have_abstract_operation() {
  QListViewItem * child;
    
  for (child = firstChild(); child != 0; child = child->nextSibling()) {
    BrowserNode * ch = ((BrowserNode *) child);
    
    if ((ch->get_type() == UmlOperation) &&
	((OperationData *) ch->get_data())->get_is_abstract())
      return TRUE;
  }
  
  return FALSE;
}

// unicode
const QStringList & BrowserClass::default_stereotypes()
{
  return its_default_stereotypes;
}

// unicode
const QStringList & BrowserClass::default_stereotypes(UmlCode arrow) const {
  if (IsaRelation(arrow))
    return relations_default_stereotypes[arrow];
  else {
    static QStringList empty;
  
    return empty;
  }
}

BrowserNodeList & BrowserClass::instances(BrowserNodeList & result, 
					  const char * st, bool non_nested)
{
  IdIterator<BrowserClass> it(all);
  BrowserClass * cl;
  
  if (non_nested) {
    if ((st == 0) || (*st == 0)) {
      while ((cl = it.current()) != 0) {
	if (!cl->deletedp() && !cl->nestedp())
	  result.append(cl);
	++it;
      }
    }
    else {
      while ((cl = it.current()) != 0) {
	if (!cl->deletedp() &&
	    !cl->nestedp() &&
	    !strcmp(cl->get_data()->get_stereotype(), st))
	  result.append(cl);
	++it;
      }
    }
  }
  else {
    if ((st == 0) || (*st == 0)) {
      while ((cl = it.current()) != 0) {
	if (!cl->deletedp())
	  result.append(cl);
	++it;
      }
    }
    else {
      while ((cl = it.current()) != 0) {
	if (!cl->deletedp() && !strcmp(cl->get_data()->get_stereotype(), st))
	  result.append(cl);
	++it;
      }
    }
  }
  
  result.sort();
  
  return result;
}

BrowserNode * BrowserClass::get_associated() const {
  return associated_diagram;
}

void BrowserClass::set_associated_diagram(BrowserClassDiagram * dg,
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

BrowserArtifact * BrowserClass::get_associated_artifact() const {
  return associated_artifact;
}

void BrowserClass::set_associated_artifact(BrowserArtifact * a,
					   bool on_read) {
  if (associated_artifact != a) {
    if (associated_artifact != 0) {
      BrowserArtifact * old = associated_artifact;
    
      associated_artifact = 0;	// to not loop until end of days
      if (! old->deletedp())
	old->remove_associated_class(this, on_read);
    }
  
    if ((associated_artifact = a) != 0)
      a->add_associated_class(this, on_read);

    def->modified();	// to upgrade drawing context
  }
}

const QValueList<BrowserComponent *> & BrowserClass::get_associated_components() const {
  return associated_components;
}

void BrowserClass::add_associated_component(BrowserComponent * c) {
  if (associated_components.findIndex(c) == -1) {
    associated_components.append(c);
    QObject::connect(c->get_data(), SIGNAL(deleted()),
		     def, SLOT(on_delete()));
  }
}

void BrowserClass::remove_associated_component(BrowserComponent * c) {
  QValueList<BrowserComponent *>::Iterator it = associated_components.find(c);
  
  if (it != associated_components.end()) {
    associated_components.remove(it);
    QObject::disconnect(c->get_data(), SIGNAL(deleted()),
			def, SLOT(on_delete()));
  }
}

void BrowserClass::on_delete() {
  if (associated_diagram && associated_diagram->deletedp())
    associated_diagram = 0;

  QValueList<BrowserComponent *>::Iterator it = associated_components.begin();
  
  while (it != associated_components.end()) {
    if ((*it)->deletedp())
      it = associated_components.remove(it);
    else
      it++;
  }
}

void BrowserClass::init()
{
  its_default_stereotypes.clear();
  its_default_stereotypes.append("actor");
  its_default_stereotypes.append("auxiliary");
  its_default_stereotypes.append("boundary");
  its_default_stereotypes.append("control");
  its_default_stereotypes.append("entity");
  its_default_stereotypes.append("enum");
  its_default_stereotypes.append("enum_pattern");
  its_default_stereotypes.append("exception");
  its_default_stereotypes.append("focus");
  its_default_stereotypes.append("implementation class");
  its_default_stereotypes.append("interface");
  its_default_stereotypes.append("@interface");
  its_default_stereotypes.append("metaclass");
  its_default_stereotypes.append("struct");
  its_default_stereotypes.append("type");
  its_default_stereotypes.append("typedef");
  its_default_stereotypes.append("union");
  its_default_stereotypes.append("utility");
  
  int r;
  
  for (r = 0; r != UmlRelations; r += 1) {
    relations_default_stereotypes[r].clear();
    
    switch (r) {
    case UmlGeneralisation: 
      break;
    case UmlDependency:
      relations_default_stereotypes[r].append("friend");
      break;
    case UmlRealize:
      relations_default_stereotypes[r].append("bind");
      break;
    default:
      // relations corresponding to attribute
      relations_default_stereotypes[r].append("vector");
      relations_default_stereotypes[r].append("list");
      relations_default_stereotypes[r].append("set");
    }
  }
}

bool BrowserClass::tool_cmd(ToolCom * com, const char * args) {
  switch ((unsigned char) args[-1]) {
  case createCmd:
    {
      bool ok = TRUE;
      
      if (is_read_only && !root_permission())
	ok = FALSE;
      else {
	switch (com->get_kind(args)) {
	case UmlAttribute:
	  if (wrong_child_name(args, UmlAttribute, FALSE, FALSE))
	    ok = FALSE;
	  else
	    (BrowserAttribute::new_one(args, this,
				       !strcmp(def->get_stereotype(), "enum") ||
				       !strcmp(def->get_stereotype(), "enum_pattern")))
	      ->write_id(com);
	  break;
	case UmlOperation:
	  if (wrong_child_name(args, UmlOperation, FALSE, FALSE))
	    ok = FALSE;
	  else
	    (BrowserOperation::new_one(args, this))->write_id(com);
	  break;
	case UmlRelations:
	  {
	    UmlCode c;
	    
	    if (!com->get_relation_kind(c, args))
	      ok = FALSE;
	    else {
	      BrowserClass * end = (BrowserClass *) com->get_id(args);
	      
	      if ((may_start(c) == 0) && (may_connect(c, end) == 0))
		((RelationData *) add_relation(c, end))->get_start()->write_id(com);
	      else
		ok = FALSE;
	    }
	  }
	  break;
	case UmlSimpleRelations:
	  {
	    UmlCode c;
	    
	    if (!com->get_relation_kind(c, args))
	      ok = FALSE;
	    else {
	      BrowserNode * end = (BrowserNode *) com->get_id(args);
	      
	      if (end->get_type() == UmlPackage)
		add_relation(UmlDependOn, end)->get_browser_node()->write_id(com);
	      else
		ok = FALSE;
	    }
	  }
	  break;
	case UmlExtraMember:
	  (BrowserExtraMember::new_one(args, this))->write_id(com);
	  break;
	case UmlClass:
	  if (wrong_child_name(args, UmlClass, FALSE, FALSE))
	    ok = FALSE;
	  else {
	    (BrowserClass::add_class(this, args))->write_id(com);
	    package_modified();
	    // def unmodified
	    return TRUE;
	  }
	  break;
	default:
	  ok = FALSE;
	}
      }
      
      if (! ok)
	com->write_id(0);
      else {
	def->modified();
	package_modified();
      }
      
      return TRUE;
    }
  case setAssocDiagramCmd:
    if (is_read_only && !root_permission())
      com->write_ack(FALSE);
    else {
      set_associated_diagram((BrowserClassDiagram *) com->get_id(args));
      com->write_ack(TRUE);
    }
    return TRUE;
  case assocArtifactCmd:
    if ((associated_artifact == 0) || associated_artifact->deletedp())
      com->write_id(0);
    else
      associated_artifact->write_id(com);
    return TRUE;
  case assocComponentCmd:
    if (com->api_format() < 17) {
      if (!associated_components.isEmpty())
	associated_components.first()->write_id(com);
      else
	com->write_id(0);
    }
    else {
      QValueList<BrowserComponent *>::Iterator it;
      
      com->write_unsigned(associated_components.count());
      
      for (it = associated_components.begin();
	   it != associated_components.end();
	   it++)
	(*it)->write_id(com);
    }
    return TRUE;
  default:
    {
      QString s = name;
      bool result = (def->tool_cmd(com, args, this, comment) ||
		     BrowserNode::tool_cmd(com, args));
      
      if (s != name)
	// to update inherit/realize/dependency names
	BrowserView::get_project()->update_stereotype(TRUE);
      
      return result;
    }
  }
}

bool BrowserClass::tool_global_cmd(ToolCom * com, const char * args)
{
  switch ((unsigned char) args[-1]) {
  case findClassCmd:
    {
      BrowserNode * p = (BrowserNode *) com->get_id(args);	// package
      
      if (p != 0) {
	QListViewItem * child;
	
	for (child = p->firstChild(); child != 0; child = child->nextSibling()) {
	  // search a class view
	  if (!((BrowserNode *) child)->deletedp() &&
	      (((BrowserNode *) child)->get_type() == UmlClassView)) {
	    QListViewItem * ch;
	    
	    for (ch = child->firstChild(); ch != 0; ch = ch->nextSibling()) {
	      // search the class
	      if (!((BrowserNode *) ch)->deletedp() &&
		  (((BrowserNode *) ch)->get_type() == UmlClass) &&
		  (((BrowserClass *) ch)->name == args)) {
		((BrowserClass *) ch)->write_id(com);
		return TRUE;
	      }
	    }
	  }
	}
      }
      else {
	IdIterator<BrowserClass> it(all);
	BrowserClass * cl;
	
	while ((cl = it.current()) != 0) {
	  if (!cl->deletedp() && cl->name == args) {
	    cl->write_id(com);
	    return TRUE;
	  }
	  ++it;
	}
      }
      
      com->write_id(0);
      return TRUE;
    }
  default:
    return FALSE;
  }
}

void BrowserClass::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "class_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
  BrowserNode::save_stereotypes(st, relations_default_stereotypes);
}

void BrowserClass::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "class_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    BrowserNode::read_stereotypes(st, relations_default_stereotypes);
    k = read_keyword(st);
  }
  else
    init();
}

void BrowserClass::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "class_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "class " << get_ident() << ' ';
    save_string(name, st);
    
    indent(+1);
    def->save(st, warning);
    
    if (associated_diagram != 0) {
      nl_indent(st);
      st << "associated_diagram ";
      associated_diagram->save(st, TRUE, warning);
    }
    
    BrowserNode::save(st);
    
    // bodies file
    QFile qf;
    char * modified_bodies = (def->get_bodies_modified())
      ? read_definition(get_ident(), "b")
      : 0;
    
    // saves the sub elts & operations body
      
    QListViewItem * child = firstChild();
    
    if (child != 0) {
      for (;;) {
	if (! ((BrowserNode *) child)->deletedp()) {
	  ((BrowserNode *) child)->save(st, FALSE, warning);
	  
	  if ((((BrowserNode *) child)->get_type() == UmlOperation) &&
	      (modified_bodies != 0)) {
	    OperationData * od =
	      (OperationData *) ((BrowserNode *) child)->get_data();
	    
	    od->save_body(qf, modified_bodies, TRUE);
	    od->save_body(qf, modified_bodies, FALSE);
	  }
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
    
    delete [] modified_bodies;
    
    if (def->get_bodies_modified()) {
      delete_definition(get_ident(), "b");
      def->set_bodies_modified(FALSE);
    }
    
    indent(-1);
    nl_indent(st);
    st << "end";
    
    // for saveAs
    if (! is_api_base())
      is_read_only = FALSE;
  }
}
  
bool BrowserClass::get_bodies_read() {
  return def->get_bodies_read();
}

void BrowserClass::set_bodies_read(bool y) {
  def->set_bodies_read(y);
}

bool BrowserClass::get_bodies_modified() {
  return def->get_bodies_modified();
}

void BrowserClass::set_bodies_modified(bool y) {
  def->set_bodies_modified(y);
}

QString BrowserClass::bodies_file() const {
  QString fn;
      
  fn.sprintf("%d.bodies", get_ident());

  QDir d = BrowserView::get_dir();
      
  return d.absFilePath(fn);
}

BrowserClass * BrowserClass::find(const char * s)
{
  IdIterator<BrowserClass> it(all);
  BrowserClass * cl;
  
  while ((cl = it.current()) != 0) {
    if (strcmp(cl->name, s) == 0)
      return cl;
    
    ++it;
  }
  
  return 0;
}

// translate plug out release < 2.0 ... if it is a plug out

static const char * component2artifact(const char * b)
{
  static const char * t[][2] = {
    { "ComponentView", "DeploymentView" },
    { "ComponentDiagram", "DeploymentDiagram" },
    { "aComponent", "anArtifact" },
    { "Component", "Artifact" }
  };
  static QCString s;
  
  s = b;
  
  for (int index1 = 0; index1 != sizeof(t)/sizeof(*t); index1 += 1) {
    const char * o = t[index1][0];
    const char * n = t[index1][1];
    int l = strlen(o);
    int index2 = 0;
    int index3;

    while ((index3 = s.find(o, index2)) != -1) {
      s.replace(index3, l, n);
      index2 = index3;
    }
  }

  return s;
}

void BrowserClass::plug_out_conversion()
{
  BrowserClass * baseArtifact;
  BrowserClass * artifact;
  BrowserArtifact * baseArtifactArtifact;
  BrowserArtifact * artifactArtifact;
  BrowserClass * deploymentView;
  BrowserClass * deploymentDiagram;
  BrowserClass * baseClass;
  BrowserClass * baseClassView;
  BrowserClass * baseItem;
  BrowserClass * item;
  BrowserArtifact * baseItemArtifact;
  BrowserClass * onInstanceCmd;
  BrowserClass * anItemKind;
  BrowserClass * com;
  BrowserArtifact * comArtifact;
  
  if (((baseArtifact = find("UmlBaseComponent")) != 0) &&
      (find("UmlBaseArtifact") == 0) &&
      ((artifact = find("UmlComponent")) != 0) && 
      ((baseArtifactArtifact = BrowserArtifact::find("UmlBaseComponent")) != 0) &&
      ((artifactArtifact = BrowserArtifact::find("UmlComponent")) != 0) && 
      ((deploymentView = find("UmlDeploymentView")) != 0) && 
      ((deploymentDiagram = find("UmlDeploymentDiagram")) != 0) && 
      ((baseClass = find("UmlBaseClass")) != 0) && 
      ((baseClassView = find("UmlBaseClassView")) != 0) && 
      ((item = find("UmlItem")) != 0) && 
      ((baseItem = find("UmlBaseItem")) != 0) && 
      ((baseItemArtifact = BrowserArtifact::find("UmlBaseItem")) != 0) && 
      ((onInstanceCmd = find("OnInstanceCmd")) != 0) &&
      ((anItemKind = find("anItemKind")) != 0) && 
      ((com = find("UmlCom")) != 0) && 
      ((comArtifact = BrowserArtifact::find("UmlCom")) != 0)) {
    // do conversion
    int uid = user_id();
    QListViewItem * child;
    QCString s;
    int index;
    AType t_deplview;
    AType t_depldiag;
    AType t_artifact;
    char * b;
    OperationData * op1;
    OperationData * op2;
    
    if (uid == 0)
      uid = 1;
    
    BrowserPackage::update_idmax_for_root();
  
    t_deplview.type = deploymentView;
    t_depldiag.type = deploymentDiagram;
    t_artifact.type = artifact;
    
    // create UmlBaseComponent class and artifact
    set_user_id(0);
    
    BrowserClass * baseComponent =
      new BrowserClass(baseArtifact, (BrowserNode *) baseArtifact->parent());
    BrowserArtifact * baseComponentArtifact =
      new BrowserArtifact(baseArtifactArtifact,
			  (BrowserNode *) baseArtifactArtifact->parent());

    baseComponent->moveItem(baseArtifact);
    baseComponentArtifact->moveItem(baseArtifactArtifact);
    
    baseComponent->set_comment(baseArtifact->get_comment());
    baseComponentArtifact->set_comment(baseArtifactArtifact->get_comment());
    baseComponentArtifact->get_data()->set_stereotype("source");
    baseComponentArtifact->add_associated_class(baseComponent, TRUE);
    
    // create UmlComponent class and artifact
    set_user_id(uid);
    
    BrowserClass * component =
      new BrowserClass(artifact, (BrowserNode *) artifact->parent());
    BrowserArtifact * componentArtifact =
      new BrowserArtifact(artifactArtifact,
			  (BrowserNode *) artifactArtifact->parent());
    
    component->moveItem(artifact);
    componentArtifact->moveItem(artifactArtifact);

    component->set_comment(artifact->get_comment());
    componentArtifact->set_comment(artifactArtifact->get_comment());
    componentArtifact->get_data()->set_stereotype("source");
    componentArtifact->add_associated_class(component, TRUE);

    AType t_component;

    t_component.type = component;
    
    // manage UmlBaseArtifact
    set_user_id(0);
	  
    baseArtifact->set_name("UmlBaseArtifact");
    baseArtifact->package_modified();
    baseArtifact->set_comment("Manage the artifacts");
    for (child = baseArtifact->firstChild(); child != 0; child = child->nextSibling()) {
      if (((BrowserNode *) child)->get_type() == UmlOperation) {
	if (strcmp(((BrowserNode *) child)->get_name(), "UmlBaseComponent") == 0) {	  
	  // UmlBaseComponent constructor
	  BrowserOperation * baseComponentCreation = 
	    new BrowserOperation((BrowserOperation *) child, baseComponent);
	  
	  set_user_id(uid);	// to create saved  *_<n>.b file name
	  op1 = (OperationData *) ((BrowserNode *) child)->get_data();
	  op2 = (OperationData *) baseComponentCreation->get_data();
	  if ((b = op1->get_body(TRUE)) != 0) {
	    op2->new_body(b, TRUE);
	    delete [] b;
	  }
	  if ((b = op1->get_body(FALSE)) != 0) {
	    op2->new_body(b, FALSE);
	    delete [] b;
	  }	  
	  baseComponentCreation->set_comment(((BrowserNode *) child)->get_comment());
	  set_user_id(0);
	  
	  // UmlBaseArtifact constructor
	  ((BrowserNode *) child)->set_name("UmlBaseArtifact");
	}
	else if (strcmp(((BrowserNode *) child)->get_name(), "create") == 0) {
	  // UmlBaseComponent create operation
	  BrowserOperation * baseComponentCreate = 
	    new BrowserOperation((BrowserOperation *) child, baseComponent);
	  
	  set_user_id(uid);	// to create saved  *_<n>.b file name
	  op1 = (OperationData *) baseComponentCreate->get_data();
	  op2 = (OperationData *) ((BrowserNode *) child)->get_data();

	  if ((b = op2->get_body(TRUE)) != 0) {
	    op1->new_body(b, TRUE);
	    op2->new_body(component2artifact(b), TRUE);
	    delete [] b;
	  }
	  if ((b = op2->get_body(FALSE)) != 0) {
	    op1->new_body(b, FALSE);
	    op2->new_body(component2artifact(b), FALSE);
	    delete [] b;
	  }	  
	  baseComponentCreate->set_comment(((BrowserNode *) child)->get_comment());
	  op1->set_return_type(t_component);
	  op2->set_param_type(0, t_deplview);
	  set_user_id(0);
	}
	else if (strcmp(((BrowserNode *) child)->get_name(), "kind") == 0) {
	  // UmlBaseComponent kind operation
	  BrowserOperation * baseComponentKind = 
	    new BrowserOperation((BrowserOperation *) child, baseComponent);
	  
	  set_user_id(uid);	// to create saved  *_<n>.b file name
	  op1 = (OperationData *) baseComponentKind->get_data();
	  op2 = (OperationData *) ((BrowserNode *) child)->get_data();

	  if ((b = op2->get_body(TRUE)) != 0) {
	    op1->new_body(b, TRUE);
	    op2->new_body(component2artifact(b), TRUE);
	    delete [] b;
	  }
	  if ((b = op2->get_body(FALSE)) != 0) {
	    op1->new_body(b, FALSE);
	    op2->new_body(component2artifact(b), FALSE);
	    delete [] b;
	  }	  
	  baseComponentKind->set_comment(((BrowserNode *) child)->get_comment());
	  set_user_id(0);
	}
	else if (strcmp(((BrowserNode *) child)->get_name(), "read_uml_") == 0) {
	  // UmlBaseComponent read_uml_ operation
	  BrowserOperation * baseComponentRead_uml_ = 
	    new BrowserOperation((BrowserOperation *) child, baseComponent);
	  
	  set_user_id(uid);	// to create saved  *_<n>.b file name
	  op1 = (OperationData *) baseComponentRead_uml_->get_data();
	  op1->new_body("  _assoc_diagram = (UmlComponentDiagram *) UmlBaseItem::read_();\n\
  UmlBaseItem::read_uml_();\n", TRUE);
	  op1->new_body("  _assoc_diagram = (UmlComponentDiagram) UmlBaseItem.read_();\n\
  super.read_uml_();\n", FALSE);
	  baseComponentRead_uml_->set_comment(((BrowserNode *) child)->get_comment());
	  
	  // UmlBaseArtifact read_uml_ operation
	  op2 = (OperationData *) ((BrowserNode *) child)->get_data();
	  if ((b = op2->get_body(TRUE)) != 0) {
	    op2->new_body(component2artifact(b), TRUE);
	    delete [] b;
	  }
	  if ((b = op2->get_body(FALSE)) != 0) {
	    op2->new_body(component2artifact(b), FALSE);
	    delete [] b;
	  }	  
	  set_user_id(0);
	}
	else if (strcmp(((BrowserNode *) child)->get_name(), "associatedDiagram") == 0) {
	  // UmlBaseComponent associatedDiagram operation
	  BrowserOperation * baseComponentAssociatedDiagram = 
	    new BrowserOperation((BrowserOperation *) child, baseComponent);
	  
	  set_user_id(uid);	// to create saved  *_<n>.b file name
	  op1 = (OperationData *) baseComponentAssociatedDiagram->get_data();
	  op2 = (OperationData *) ((BrowserNode *) child)->get_data();
	  if ((b = op2->get_body(TRUE)) != 0) {
	    op1->new_body(b, TRUE);
	    delete [] b;
	  }
	  if ((b = op2->get_body(FALSE)) != 0) {
	    op1->new_body(b, FALSE);
	    delete [] b;
	  }	  
	  baseComponentAssociatedDiagram->set_comment(((BrowserNode *) child)->get_comment());

	  op2->set_return_type(t_depldiag);
	  set_user_id(0);
	}
	else if (strcmp(((BrowserNode *) child)->get_name(), "set_AssociatedDiagram") == 0) {
	  // UmlBaseComponent set_AssociatedDiagram operation
	  BrowserOperation * baseComponentSet_AssociatedDiagram = 
	    new BrowserOperation((BrowserOperation *) child, baseComponent);
	  
	  set_user_id(uid);	// to create saved  *_<n>.b file name
	  op1 = (OperationData *) baseComponentSet_AssociatedDiagram->get_data();
	  op2 = (OperationData *) ((BrowserNode *) child)->get_data();
	  if ((b = op2->get_body(TRUE)) != 0) {
	    op1->new_body(b, TRUE);
	    delete [] b;
	  }
	  if ((b = op2->get_body(FALSE)) != 0) {
	    op1->new_body(b, FALSE);
	    delete [] b;
	  }	  
	  baseComponentSet_AssociatedDiagram->set_comment(((BrowserNode *) child)->get_comment());

	  op2->set_param_type(0, t_depldiag);
	  set_user_id(0);
	}
	else if (strcmp(((BrowserNode *) child)->get_name(), "associatedComponents") == 0) {
	  ((BrowserNode *) child)->set_name("associatedArtifacts");
	  set_user_id(uid);	// to create saved  *_<n>.b file name
	  op1 = (OperationData *) ((BrowserNode *) child)->get_data();
	  if ((b = op1->get_body(TRUE)) != 0) {
	    op1->new_body(component2artifact(b), TRUE);
	    delete [] b;
	  }
	  if ((b = op1->get_body(FALSE)) != 0) {
	    op1->new_body(component2artifact(b), FALSE);
	    delete [] b;
	  }	  
	  set_user_id(0);
	}
	else if (strcmp(((BrowserNode *) child)->get_name(), "addAssociatedComponent") == 0) {
	  ((BrowserNode *) child)->set_name("addAssociatedArtifact");
	  set_user_id(uid);	// to create saved  *_<n>.b file name
	  op1 = (OperationData *) ((BrowserNode *) child)->get_data();
	  op1->set_param_type(0, t_artifact);
	  if ((b = op1->get_body(TRUE)) != 0) {
	    op1->new_body(component2artifact(b), TRUE);
	    delete [] b;
	  }
	  if ((b = op1->get_body(FALSE)) != 0) {
	    op1->new_body(component2artifact(b), FALSE);
	    delete [] b;
	  }	  
	  set_user_id(0);
	}
	else if (strcmp(((BrowserNode *) child)->get_name(), "removeAssociatedComponent") == 0) {
	  ((BrowserNode *) child)->set_name("removeAssociatedArtifact");
	  set_user_id(uid);	// to create saved  *_<n>.b file name
	  op1 = (OperationData *) ((BrowserNode *) child)->get_data();
	  op1->set_param_type(0, t_artifact);
	  if ((b = op1->get_body(TRUE)) != 0) {
	    op1->new_body(component2artifact(b), TRUE);
	    delete [] b;
	  }
	  if ((b = op1->get_body(FALSE)) != 0) {
	    op1->new_body(component2artifact(b), FALSE);
	    delete [] b;
	  }	  
	  set_user_id(0);
	}
	else if (strcmp(((BrowserNode *) child)->get_name(), "removeAllAssociatedComponents") == 0) {
	  ((BrowserNode *) child)->set_name("removeAllAssociatedArtifacts");
	  set_user_id(uid);	// to create saved  *_<n>.b file name
	  op1 = (OperationData *) ((BrowserNode *) child)->get_data();
	  if ((b = op1->get_body(TRUE)) != 0) {
	    op1->new_body(component2artifact(b), TRUE);
	    delete [] b;
	  }
	  if ((b = op1->get_body(FALSE)) != 0) {
	    op1->new_body(component2artifact(b), FALSE);
	    delete [] b;
	  }	  
	  set_user_id(0);
	}
	
	// update operation comment
	s = ((BrowserNode *) child)->get_comment();
	index = 0;
	
	while ((index = s.find("component", index)) != -1) {
	  s.replace(index, 9, "artifact");
	  index += 8;
	}
	
	index = 0;
	
	while ((index = s.find("componant", index)) != -1) {	// this fault exist
	  s.replace(index, 9, "artifact");
	  index += 8;
	}
	
	((BrowserNode *) child)->set_comment(s);
      }
      else if (((BrowserNode *) child)->get_type() == UmlGeneralisation) {
	RelationData * r = (RelationData *)
	  ((BrowserNode *) child)->get_data();
	
	if (r->get_end_class() == item)
	  ((RelationData *) baseComponent->add_relation(UmlGeneralisation, item))
	    ->copy(r);
      }
      else if (((BrowserNode *) child)->get_type() == UmlDirectionalAssociation) {
	RelationData * r = (RelationData *) ((BrowserNode *) child)->get_data();
	
	if (strcmp(r->get_role_a(), "_assoc_diagram") == 0) {
	  ((RelationData *) baseComponent->add_relation(UmlDirectionalAssociation, 
							r->get_end_class()))
	    ->copy(r);
	  r->unidir_change_dest(deploymentDiagram);
	}
      }
    }
    baseArtifactArtifact->package_modified();
    baseArtifactArtifact->set_name("UmlBaseArtifact");
    
    // manage UmlBaseClass
    baseClass->package_modified();
    for (child = baseClass->firstChild(); child != 0; child = child->nextSibling()) {
      if (((BrowserNode *) child)->get_type() == UmlOperation) {
	if (strcmp(((BrowserNode *) child)->get_name(), "associatedComponent") == 0) {
	  ((BrowserNode *) child)->set_name("associatedArtifact");
	  ((BrowserNode *) child)->set_comment(" returns the optional associated artifact,\n\
 to set it refer to the UmlBaseArtifact's operations\n\
 addAssociatedClass(), removeAssociatedClass() and\n\
 setAssociatedClasses()");
	  
	  set_user_id(uid);	// to create saved  *_<n>.b file name
	  op1 = (OperationData *) ((BrowserNode *) child)->get_data();
	  if ((b = op1->get_body(TRUE)) != 0) {
	    op1->new_body(component2artifact(b), TRUE);
	    delete [] b;
	  }
	  if ((b = op1->get_body(FALSE)) != 0) {
	    op1->new_body(component2artifact(b), FALSE);
	    delete [] b;
	  }	  
	}
      }
    }
    
    // manage UmlBaseClassView
    baseClassView->package_modified();
    for (child = baseClassView->firstChild(); child != 0; child = child->nextSibling()) {
      if (((BrowserNode *) child)->get_type() == UmlOperation) {
	if (strcmp(((BrowserNode *) child)->get_name(), "associatedComponentView") == 0) {
	  ((BrowserNode *) child)->set_name("associatedDeploymentView");
	  ((BrowserNode *) child)->set_comment(" returns the optional associated deployment view");
	  
	  op1 = (OperationData *) ((BrowserNode *) child)->get_data();
	  op1->set_return_type(t_deplview);
	}
	else if (strcmp(((BrowserNode *) child)->get_name(), "set_AssociatedComponentView") == 0) {
	  ((BrowserNode *) child)->set_name("set_AssociatedDeploymentView");
	  ((BrowserNode *) child)->set_comment(" sets the associated deployment view, arg may be null to unset it\n\
\n\
 On error return FALSE in C++, produce a RuntimeException in Java\n");
	  
	  op1 = (OperationData *) ((BrowserNode *) child)->get_data();
	  op1->set_param_type(0, t_deplview);
	}
	else if (strcmp(((BrowserNode *) child)->get_name(), "read_uml_") == 0) {
	  set_user_id(uid);	// to create saved  *_<n>.b file name
	  op1 = (OperationData *) ((BrowserNode *) child)->get_data();
	  if ((b = op1->get_body(TRUE)) != 0) {
	    op1->new_body(component2artifact(b), TRUE);
	    delete [] b;
	  }
	  if ((b = op1->get_body(FALSE)) != 0) {
	    op1->new_body(component2artifact(b), FALSE);
	    delete [] b;
	  }	  
	}
      }
      else if (((BrowserNode *) child)->get_type() == UmlDirectionalAssociation) {
	RelationData * r = (RelationData *) ((BrowserNode *) child)->get_data();
	
	if (strcmp(r->get_role_a(), "_assoc_view") == 0)
	  r->unidir_change_dest(deploymentView);
      }
    }
    
    // manage UmlBaseItem
    baseItem->package_modified();
    for (child = baseItem->firstChild(); child != 0; child = child->nextSibling()) {
      if (((BrowserNode *) child)->get_type() == UmlOperation) {
	if (strcmp(((BrowserNode *) child)->get_name(), "read_") == 0) {
	  set_user_id(uid);	// to create saved  *_<n>.b file name
	  op1 = (OperationData *) ((BrowserNode *) child)->get_data();
	  if ((b = op1->get_body(TRUE)) != 0) {
	    s = b;
	    delete [] b;
	    s.insert(s.find("case aNode:"),
		     "case anArtifact:\n\
      result = new UmlArtifact(id, name);\n\
      break;\n    ");
	    op1->new_body(s, TRUE);
	  }
	  if ((b = op1->get_body(FALSE)) != 0) {
	    s = b;
	    delete [] b;
	    s.insert(s.find("case anItemKind._aNode:"),
		   "case anItemKind._anArtifact:\n\
      return new UmlArtifact(id, name);\n    ");
	    op1->new_body(s, FALSE);
	  }
	  set_user_id(0);
	}
      }
      else if (((BrowserNode *) child)->get_type() == UmlDependency) {
	RelationData * r = (RelationData *)
	  ((BrowserNode *) child)->get_data();
	
	if (r->get_end_class() == baseArtifact)
	  ((RelationData *) baseItem->add_relation(UmlDependency, baseComponent))
	    ->copy(r);
      }
    }
    baseItemArtifact->package_modified();
    ((ArtifactData *) baseItemArtifact->get_data())->convert_add_include_artifact();
    
    // manage onInstanceCmd
    BrowserAttribute * a = 0;
    
    onInstanceCmd->package_modified();
    for (child = onInstanceCmd->firstChild(); child != 0; child = child->nextSibling()) {
      if (((BrowserNode *) child)->get_type() == UmlAttribute) {
	if (strcmp(((BrowserNode *) child)->get_name(), "assocComponentCmd") == 0) {
	  a = (BrowserAttribute *) 
	    ((BrowserAttribute *) child)->duplicate(onInstanceCmd, "assocArtifactCmd");
	  ((AttributeData *) a->get_data())->set_visibility(UmlPublic);
	  a->moveItem(child);
	}
	else if (strcmp(((BrowserNode *) child)->get_name(), "addAssocComponentCmd") == 0)
	  ((BrowserNode *) child)->set_name("addAssocArtifactCmd");
	else if (strcmp(((BrowserNode *) child)->get_name(), "removeAssocComponentCmd") == 0)
	  ((BrowserNode *) child)->set_name("removeAssocArtifactCmd");
	else if (strcmp(((BrowserNode *) child)->get_name(), "removeAllAssocComponentsCmd") == 0)
	  ((BrowserNode *) child)->set_name("removeAllAssocArtifactsCmd");
      }
    }
    
    if (a != 0) {
      // manage anItemKind
      a = (BrowserAttribute *) a->duplicate(anItemKind, "anArtifact");
      ((AttributeData *) a->get_data())->set_visibility(UmlPublic);

      for (child = anItemKind->firstChild(); child != 0; child = child->nextSibling()) {
	if (strcmp(((BrowserNode *) child)->get_name(), "aNode") == 0) {
	  a->moveItem(child);
	  break;
	}
      }
    }
    
    // manage UmlCom
    com->package_modified();
    for (child = com->firstChild(); child != 0; child = child->nextSibling()) {
      if ((((BrowserNode *) child)->get_type() == UmlOperation) && 
	  (strcmp(((BrowserNode *) child)->get_name(), "connect") == 0)) {
	set_user_id(uid);	// to create saved  *_<n>.b file name
	op1 = (OperationData *) ((BrowserNode *) child)->get_data();
	if ((b = op1->get_body(TRUE)) != 0) {
	  s = b;
	  delete [] b;
	  s.insert(s.find("write_unsigned(")+15, "1");
	  op1->new_body(s, TRUE);
	}
	
	if ((b = op1->get_body(FALSE)) != 0) {
	  s = b;
	  delete [] b;
	  s.insert(s.find("write_unsigned(")+15, "1");
	  op1->new_body(s, FALSE);
	}
	set_user_id(0);
	break;
      }
    }
    
    // manage UmlArtifact
    set_user_id(uid);
    
    artifact->set_name("UmlArtifact");
    artifact->package_modified();
    s = artifact->get_comment();
    if ((index = s.find("'components'")) != -1) {
      s.replace(index+1, 9, "artifact");
      artifact->set_comment(s);
    }
    for (child = artifact->firstChild(); child != 0; child = child->nextSibling()) {
      if (((BrowserNode *) child)->get_type() == UmlOperation) {
	if (strcmp(((BrowserNode *) child)->get_name(), "UmlComponent") == 0) {
	  BrowserOperation * componentCreate = 
	    new BrowserOperation((BrowserOperation *) child, component);
	  
	  ((BrowserNode *) child)->set_name("UmlArtifact");
	  componentCreate->set_comment(((BrowserNode *) child)->get_comment());
	  	  
	  OperationData::convert((OperationData *) componentCreate->get_data(),
				 (OperationData *) ((BrowserNode *) child)->get_data());
	}
	else if (strcmp(((BrowserNode *) child)->get_name(), "~UmlComponent") == 0)
	  ((BrowserNode *) child)->set_name("~UmlArtifact");
      }
      else if (((BrowserNode *) child)->get_type() == UmlGeneralisation) {
	RelationData * r = (RelationData *)
	  ((BrowserNode *) child)->get_data();
	
	if (r->get_end_class() == baseArtifact)
	  ((RelationData *) component->add_relation(UmlGeneralisation, baseComponent))
	    ->copy(r);
      }
    }
    artifactArtifact->package_modified();
    artifactArtifact->set_name("UmlArtifact");

    // rename component views -> deployment views
    BrowserNode * bn;
    
    bn = (BrowserNode *) baseArtifactArtifact->parent();
    bn->set_name("API BASE Deployment view");
    bn->set_comment("The API system artifacts\n\nDO NOT MODIFY THEM !!!\n");
    
    bn = (BrowserNode *) comArtifact->parent();
    if (strcmp(bn->get_name(), "Com Component view") == 0) {
      bn->set_name("Com Deployment view");
      bn->set_comment("The API system artifacts concerning the communication with BOUML\n\nDO NOT MODIFY THEM !!!\n");
    }
    
    BrowserArtifact * settingsArtifact = BrowserArtifact::find("CppSettings");
    
    if (settingsArtifact != 0) {
      bn = (BrowserNode *) settingsArtifact->parent();
      if (strcmp(bn->get_name(), "Settings Component view") == 0) {
	bn->set_name("Settings Deployment view");
	bn->set_comment("The API system artifacts concerning the settings\n\nDO NOT MODIFY THEM !!!\n");
      }
    }
    
    bn = (BrowserNode *) artifactArtifact->parent();
    if (strcmp(bn->get_name(), "API USER component view") == 0) {
      bn->set_name("API USER Deployment view");
      s = bn->get_comment();
      index = 0;
      
      while ((index = s.find("component", index)) != -1) {
	s.replace(index, 9, "artifact");
	index += 8;
      }
      
      bn->set_comment(s);
    }
  }
}

BrowserClass * BrowserClass::read_ref(char * & st, const char * k)
{
  if (k == 0)
    read_keyword(st, "class_ref");
  else if (strcmp(k, "class_ref"))
    wrong_keyword(k, "class_ref");
  
  int id = read_id(st);
  BrowserClass * result = all[id];
  
  return (result == 0)
    ? new BrowserClass(id)
    : result;
}

BrowserClass * BrowserClass::read(char * & st, char * k, BrowserNode * parent)
{
  BrowserClass * result;
  int id;
  
  if (!strcmp(k, "class_ref")) {
    if ((result = all[id = read_id(st)]) == 0)
      result = new BrowserClass(id);
    return result;
  }
  else if (!strcmp(k, "class")) {
    id = read_id(st);
    
    QString s = read_string(st);
    
    if ((result = all[id]) == 0)
      result = new BrowserClass(s, parent, new ClassData, id);
    else {
      result->set_parent(parent);
      result->set_name(s);
    }
    
    result->is_read_only = (!in_import() && read_only_file()) ||
      (user_id() != 0) && result->is_api_base();
    
    QFileInfo fi(BrowserView::get_dir(), QString::number(id) + ".bodies");
    if (!in_import() && fi.exists() && !fi.isWritable())
      result->is_read_only = TRUE;	
    
    k = read_keyword(st);
    
    result->def->read(st, k);	// updates k
    result->def->set_browser_node(result);
    
    if (!strcmp(k, "associated_class_diagram")) {
      // old format
      result->set_associated_diagram(BrowserClassDiagram::read_ref(st, "classdiagram_ref"),
				     TRUE);
      k = read_keyword(st);
    }
    else if (!strcmp(k, "associated_diagram")) {
      result->set_associated_diagram(BrowserClassDiagram::read_ref(st, read_keyword(st)),
				     TRUE);
      k = read_keyword(st);
    }
    
    if ((read_file_format() < 20) && !strcmp(k, "associated_component")) {
      // old formats or component -> artifact
      if (read_file_format() < 3)
	// old format
	result->set_associated_artifact(BrowserArtifact::read_ref(st, "artifact_ref"),
					 TRUE);
      else
	result->set_associated_artifact(BrowserArtifact::read_ref(st, read_keyword(st)),
					 TRUE);
      k = read_keyword(st);
    }
    
    result->BrowserNode::read(st, k);	// updates k
    
    if (strcmp(k, "end")) {
      while (BrowserAttribute::read(st, k, result) ||
	     BrowserOperation::read(st, k, result) ||
	     BrowserExtraMember::read(st, k, result) ||
	     BrowserRelation::read(st, k, result) ||
	     BrowserSimpleRelation::read(st, k, result) ||
	     BrowserClass::read(st, k, result))
	k = read_keyword(st);

      if (in_import()) {
	result->set_bodies_modified(TRUE);
	OperationData::import(result, id);
      }
      
      if (strcmp(k, "end"))
	wrong_keyword(k, "end");
    }
    
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserClass::get_it(const char * k, int id)
{
  if (!strcmp(k, "class_ref"))
    return all[id];
  
  BrowserNode * r;
  
  if (((r = BrowserAttribute::get_it(k, id)) == 0) && 
      ((r = BrowserOperation::get_it(k, id)) == 0) && 
      ((r = BrowserExtraMember::get_it(k, id)) == 0))
    r = BrowserRelation::get_it(k, id);
  
  return r;
}
