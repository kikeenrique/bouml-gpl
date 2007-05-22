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

#include <qgrid.h> 
#include <qvbox.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qradiobutton.h> 
#include <qpushbutton.h> 
#include <qpopupmenu.h> 
#include <qcursor.h> 

#include "AttributeDialog.h"
#include "AttributeData.h"
#include "ClassData.h"
#include "BrowserView.h"
#include "BrowserClass.h"
#include "BrowserAttribute.h"
#include "KeyValueTable.h"
#include "UmlWindow.h"
#include "UmlDesktop.h"
#include "DialogUtil.h"
#include "GenerationSettings.h"
#include "strutil.h"
#include "BodyDialog.h"
#include "AnnotationDialog.h"

QSize AttributeDialog::previous_size;

AttributeDialog::AttributeDialog(AttributeData * a)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), att(a) {
  a->browser_node->edit_start();
  
  if (a->browser_node->is_writable())
    setCancelButton();
  else {
    setOkButton(QString::null);
    setCancelButton("Close");
  }
  
  visit = !hasOkButton();
  ClassData * cld = (ClassData *) 
    ((BrowserNode *) a->browser_node->parent())->get_data();
  QString stereotype = cld->get_stereotype();
  QString lang_st;
  
  in_enum = (stereotype == "enum");
  
  lang_st = GenerationSettings::cpp_class_stereotype(stereotype);
  cpp_in_enum = in_enum || (lang_st == "enum");
  cpp_in_typedef = !cpp_in_enum && (lang_st == "typedef");
  
  lang_st = GenerationSettings::java_class_stereotype(stereotype);
  java_in_enum = in_enum || (lang_st == "enum");
  java_in_enum_pattern = !java_in_enum && (lang_st == "enum_pattern");
  java_in_typedef = !java_in_enum && (lang_st == "typedef");
  
  lang_st = GenerationSettings::idl_class_stereotype(stereotype);
  idl_in_enum = in_enum || (lang_st == "enum");
  idl_in_typedef = !idl_in_enum && (lang_st == "typedef");
  idl_in_struct = !idl_in_enum && ((lang_st == "struct") || (lang_st == "exception"));
  idl_in_union = !idl_in_enum && (lang_st == "union");
  
  setCaption((in_enum || java_in_enum_pattern) ? "Enum item dialog" : "Attribute dialog");
  
  QGrid * grid;
  QHBox * htab;
  QString s;
    
  // general tab
  
  grid = new QGrid(2, this);
  umltab = grid;
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel("class : ", grid);
  new QLabel(((BrowserNode *) a->get_browser_node()->parent())->full_name(TRUE),
	     grid);
  
  new QLabel("name :", grid);
  edname = new LineEdit(a->name(), grid);
  edname->setReadOnly(visit);

  QFont font = edname->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  
  if (!java_in_enum_pattern) {
    new QLabel("stereotype :", grid);
    edstereotype = new QComboBox(!visit, grid);
    edstereotype->insertItem(toUnicode(a->get_stereotype()));
    if (!visit) {
      edstereotype->insertStringList(BrowserAttribute::default_stereotypes());
      if (java_in_enum) {
	int n = edstereotype->count();
	
	for (attribute_st_rank = 0; attribute_st_rank != n; attribute_st_rank += 1)
	  if (edstereotype->text(attribute_st_rank) == "attribute")
	    break;
	     
	if (attribute_st_rank == n) {
	  edstereotype->insertItem("Attribute");
	  n += 1;
	}
	
	for (empty_st_rank = 0; empty_st_rank != n; empty_st_rank += 1)
	  if (edstereotype->text(empty_st_rank).isEmpty())
	    break;
	     
	if (empty_st_rank == n)
	  edstereotype->insertItem("");
      }
      edstereotype->setAutoCompletion(TRUE);
    }
    edstereotype->setCurrentItem(0);
    
    QSizePolicy sp = edstereotype->sizePolicy();
    
    sp.setHorData(QSizePolicy::Expanding);
    edstereotype->setSizePolicy(sp);
    
    connect(new SmallPushButton("type :", grid), SIGNAL(clicked()),
	    this, SLOT(menu_type()));
    
    edtype = new QComboBox(!visit, grid);
    edtype->insertItem(a->get_type().get_full_type());
    if (!visit) {
      BrowserClass::instances(nodes);
      nodes.full_names(list);
        
      edtype->insertStringList(GenerationSettings::basic_types());
      offset = edtype->count();
      edtype->insertStringList(list);
      edtype->setAutoCompletion(TRUE);
      view = a->browser_node->container(UmlClass);
    }
    edtype->setCurrentItem(0);
    
    edtype->setSizePolicy(sp);

    new QLabel("initial value :", grid);
  }
  else
    new QLabel("value :", grid);
  
  htab = new QHBox(grid);
  edinit = new LineEdit(a->get_init_value(), htab);
  if (visit)
    edinit->setReadOnly(TRUE);
  else
    connect(new SmallPushButton("Editor", htab), SIGNAL(clicked()),
	    this, SLOT(edit_init()));

  QButtonGroup * bg;
  
  if (!java_in_enum_pattern) {  
    new QLabel(grid);
    
    htab = new QHBox(grid);
    bg = uml_visibility.init(htab, a->get_uml_visibility(), TRUE);
    if (visit)
      bg->setEnabled(FALSE);
    htab->setStretchFactor(bg, 1000);
    
    htab->setStretchFactor(new QLabel("      ", htab), 0);
    
    bg = new QButtonGroup(3, QGroupBox::Horizontal, QString::null, htab);
    htab->setStretchFactor(bg, 1000);
    bg->setExclusive(FALSE);
    classattribute_cb = new QCheckBox("class attribute", bg);
    if (a->get_isa_class_attribute())
      classattribute_cb->setChecked(TRUE);
    volatile_cb = new QCheckBox("volatile", bg);
    if (a->isa_volatile_attribute)
      volatile_cb->setChecked(TRUE);
    constattribute_cb = new QCheckBox("read-only", bg);
    if (a->get_isa_const_attribute())
      constattribute_cb->setChecked(TRUE);
    classattribute_cb->setDisabled(visit);
    volatile_cb->setDisabled(visit);
    constattribute_cb->setDisabled(visit);
  }
  
  QVBox * vtab = new QVBox(grid);
  
  new QLabel("description :", vtab);
  if (! visit) {
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_description()));
    connect(new SmallPushButton("Default", vtab), SIGNAL(clicked()),
	    this, SLOT(default_description()));
  }
  comment = new MultiLineEdit(grid);
  comment->setReadOnly(visit);
  comment->setText(a->browser_node->get_comment());
  comment->setFont(font);
  
  vtab = new QVBox(grid);
  new QLabel("constraint :", vtab);
  if (! visit) {
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_constraint()));
  }
  constraint = new MultiLineEdit(grid);
  constraint->setReadOnly(visit);
  constraint->setText(a->constraint);
  constraint->setFont(font);
  
  addTab(grid, "Uml");
  
  // C++
  
  if (! cpp_in_typedef) {
    grid = new QGrid(2, this);
    cpptab = grid;
    grid->setMargin(5);
    grid->setSpacing(5);
    
    if (!cpp_in_enum) {  
      new QLabel("Visibility :", grid);
      htab = new QHBox(grid);
      
      QButtonGroup * bg =
	cpp_visibility.init(htab, a->get_cpp_visibility(), FALSE, 0, "follow uml");
      
      if (visit)
	bg->setEnabled(FALSE);
      
      new QLabel(" ", htab);
      
      mutable_cb = new QCheckBox("mutable", htab);
      if (a->cpp_mutable)
	mutable_cb->setChecked(TRUE);
      if (visit)
	mutable_cb->setDisabled(TRUE);
      else
	connect(mutable_cb, SIGNAL(toggled(bool)), this, SLOT(cpp_update()));
    }
    
    new QLabel("Declaration :", grid);
    edcppdecl = new MultiLineEdit(grid);
    edcppdecl->setText(a->get_cppdecl());
    edcppdecl->setFont(font);
    if (visit)
      edcppdecl->setReadOnly(TRUE);
    else
      connect(edcppdecl, SIGNAL(textChanged()), this, SLOT(cpp_update()));
    
    new QLabel("Result after\nsubstitution :", grid);
    showcppdecl = new MultiLineEdit(grid);
    showcppdecl->setReadOnly(TRUE);
    showcppdecl->setFont(font);
    
    if (! visit) {
      new QLabel(grid);
      htab = new QHBox(grid);
      connect(new QPushButton("Default declaration", htab), SIGNAL(pressed ()),
	      this, SLOT(cpp_default()));
      connect(new QPushButton("Not generated in C++", htab), SIGNAL(pressed ()),
	      this, SLOT(cpp_unmapped()));
    }
    
    addTab(grid, "C++");
  }
  else
    cpptab = 0;
  
  // Java
  
  if (! java_in_typedef) {
    grid = new QGrid(2, this);
    javatab = grid;
    grid->setMargin(5);
    grid->setSpacing(5);
    
    if (!java_in_enum && !java_in_enum_pattern) {
      new QLabel("", grid);
      transient_cb = new QCheckBox("transient", grid);
      if (a->java_transient)
	transient_cb->setChecked(TRUE);
      if (visit)
	transient_cb->setDisabled(TRUE);
      else
	connect(transient_cb, SIGNAL(toggled(bool)), this, SLOT(java_update()));
    }
  
    new QLabel("Declaration :", grid);
    edjavadecl = new MultiLineEdit(grid);
    edjavadecl->setText(a->get_javadecl());
    edjavadecl->setFont(font);
    if (visit)
      edjavadecl->setReadOnly(TRUE);
    else
      connect(edjavadecl, SIGNAL(textChanged()), this, SLOT(java_update()));
    
    new QLabel("Result after\nsubstitution :", grid);
    showjavadecl = new MultiLineEdit(grid);
    showjavadecl->setReadOnly(TRUE);
    showjavadecl->setFont(font);
    
    new QLabel(grid);
    htab = new QHBox(grid);

    if (! visit) {
      if (java_in_enum) {
	connect(new QPushButton("Default item declaration", htab), SIGNAL(pressed ()),
		this, SLOT(java_default_item()));
	connect(new QPushButton("Default attribute declaration", htab), SIGNAL(pressed ()),
		this, SLOT(java_default()));
      }
      else
	connect(new QPushButton("Default declaration", htab), SIGNAL(pressed ()),
		this, SLOT(java_default()));
      connect(new QPushButton("Not generated in Java", htab), SIGNAL(pressed ()),
	      this, SLOT(java_unmapped()));
      
    }
    javaannotation = (const char *) a->java_annotation;
    editjavaannotation =
      new QPushButton((visit) ? "Show annotation" : "Edit annotation",
		      htab);
    connect(editjavaannotation, SIGNAL(clicked()),
	    this, SLOT(java_edit_annotation()));
    
    addTab(grid, "Java");
  }
  else
    javatab = 0;
  
  // IDL
  
  if (! idl_in_typedef) {
    grid = new QGrid(2, this);
    idltab = grid;
    grid->setMargin(5);
    grid->setSpacing(5);
    
    if (idl_in_union) {
      new QLabel("Case :", grid);
      edcase = new QComboBox(TRUE, grid);
      edcase->insertItem(a->get_idlcase());
      
      if (! visit) {
	AType switch_type = cld->get_switch_type();
	
	if (switch_type.type != 0) {
	  switch_type.type->children(enums, UmlAttribute);
	  enums.names(enum_names);
	  edcase->insertStringList(enum_names);
	  edcase->setAutoCompletion(TRUE);
	}
      }
      edcase->setCurrentItem(0);
      
      QSizePolicy sp = edcase->sizePolicy();
      
      sp.setHorData(QSizePolicy::Expanding);
      edcase->setSizePolicy(sp);
      if (! visit)
	connect(edcase, SIGNAL(activated(int)), this, SLOT(idl_update()));      
    }
    
    new QLabel("Declaration :", grid);
    edidldecl = new MultiLineEdit(grid);
    edidldecl->setText(a->get_idldecl());
    edidldecl->setFont(font);
    if (visit)
      edidldecl->setReadOnly(TRUE);
    else
      connect(edidldecl, SIGNAL(textChanged()), this, SLOT(idl_update()));
    
    new QLabel("Result after\nsubstitution :", grid);
    showidldecl = new MultiLineEdit(grid);
    showidldecl->setReadOnly(TRUE);
    showidldecl->setFont(font);
    
    if (! visit) {
      new QLabel(grid);
      htab = new QHBox(grid);
      connect(new QPushButton((idl_in_enum) ? "Default declaration"
					    : "Default Attribute declaration",
			      htab),
	      SIGNAL(pressed ()), this, SLOT(idl_default()));
      if (!idl_in_enum && !idl_in_union && !idl_in_struct)
	connect(new QPushButton("Default State declaration", htab), SIGNAL(pressed ()),
		this, SLOT(idl_default_state()));
      connect(new QPushButton("Not generated in Idl", htab), SIGNAL(pressed ()),
	      this, SLOT(idl_unmapped()));
    }
    
    addTab(grid, "Idl");
  }
  else
    idltab = 0;
  
  // USER : list key - value
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  kvtable = new KeyValuesTable(a->browser_node, grid, visit);
  addTab(grid, "Properties");
  
  //
    
  connect(this, SIGNAL(currentChanged(QWidget *)),
	  this, SLOT(update_all_tabs(QWidget *)));
}

AttributeDialog::~AttributeDialog() {
  att->browser_node->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
}

void AttributeDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

void AttributeDialog::menu_type() {
  QPopupMenu m(0);

  m.insertItem("Choose", -1);
  m.insertSeparator();
  
  int index = list.findIndex(edtype->currentText().stripWhiteSpace());
  
  if (index != -1)
    m.insertItem("Select in browser", 0);
  
  BrowserNode * bn = 0;
  
  if (! visit) {
    bn = BrowserView::selected_item();
    
    if ((bn != 0) && (bn->get_type() == UmlClass) && !bn->deletedp())
      m.insertItem("Choose class selected in browser", 1);
    else
      bn = 0;
    
    m.insertItem("Create class and choose it", 2);
  }
  
  if (!visit || (index != -1) || (bn != 0)) {
    switch (m.exec(QCursor::pos())) {
    case 0:
      nodes.at(index)->select_in_browser();
      break;
    case 2:
      bn = BrowserClass::add_class(view);
      if (bn == 0)
	return;
      bn->select_in_browser();
      // no break
    case 1:
      {
	QString s = bn->full_name(TRUE);
	
	if ((index = list.findIndex(s)) == -1) {
	  // new class, may be created through an other dialog
	  index = 0;
	  QStringList::Iterator iter = list.begin();
	  QStringList::Iterator iter_end = list.end();
	  
	  while ((iter != iter_end) && (*iter < s)) {
	    ++iter;
	    index += 1;
	  }
	  nodes.insert((unsigned) index, bn);
	  list.insert(iter, s);
	  edtype->insertItem(s, index + offset);
	}
      }
      edtype->setCurrentItem(index + offset);
      break;
    default:
      break;
    }
  }
}

void AttributeDialog::accept() {
  if (!check_edits(edits))
    return;
    
  BrowserNode * bn = att->browser_node;
  QString s;
  
  s = edname->text().stripWhiteSpace();
  if ((s != att->name()) &&
      ((BrowserNode *) bn->parent())->wrong_child_name(s, UmlAttribute,
						       bn->allow_spaces(),
						       bn->allow_empty()))
    msg_critical("Error", s + "\n\nillegal name or already used");
  else {  
    bn->set_name(s);
    
    if (!java_in_enum_pattern) {
      AType t;
      
      s = edtype->currentText().stripWhiteSpace();
      if (!s.isEmpty()) {
	int index = list.findIndex(s);
	
	if (index >= 0)
	  t.type = (BrowserClass *) nodes.at(index);
	else
	  t.explicit_type = s;
      }
      att->set_type(t);
      
      att->uml_visibility = uml_visibility.value();
      
      att->isa_class_attribute = classattribute_cb->isChecked();
      att->isa_volatile_attribute = volatile_cb->isChecked();
    
      att->isa_const_attribute = constattribute_cb->isChecked();

      att->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
    }
    
    att->init_value = edinit->text();
    
    if (cpp_in_typedef)
      att->cpp_decl = QString::null;
    else {
      att->cpp_decl = edcppdecl->text();
      if (!cpp_in_enum)
	att->cpp_visibility = cpp_visibility.value();
    }
    
    att->cpp_mutable = (cpp_in_enum || cpp_in_typedef)
      ? FALSE
      : mutable_cb->isChecked();
    
    att->java_transient = (java_in_enum || java_in_typedef || java_in_enum_pattern)
      ? FALSE
      : transient_cb->isChecked();
    att->java_decl = (java_in_typedef) ? QString::null : edjavadecl->text();
    att->java_annotation = javaannotation;
    
    att->idl_decl = (idl_in_typedef) ? QString::null : edidldecl->text();
    if (idl_in_union) {
      int index;
      s = edcase->currentText().stripWhiteSpace();
      
      if (!s.isEmpty() && ((index = enum_names.findIndex(s)) != -1))
	att->set_idlcase((BrowserAttribute *) enums.at(index), "");
      else
	att->set_idlcase(0, s);
    }
    
    bn->set_comment(comment->text());
    UmlWindow::set_commented(bn);
  
    att->constraint = constraint->stripWhiteSpaceText();
    
    kvtable->update(bn);
    
    bn->modified();
    bn->package_modified();
    att->modified();
    
    QDialog::accept();
  }
}

void AttributeDialog::update_all_tabs(QWidget * w) {
  edname->setText(edname->text().stripWhiteSpace());
  
  if (w == umltab) {
    if (!visit)
      edname->setFocus();
  }
  else if (w == cpptab) {
    cpp_update();
    if (!visit)
      edcppdecl->setFocus();
  }
  else if (w == javatab) {
    java_update();
    if (!visit)
      edjavadecl->setFocus();
  }
  else if (w == idltab) {
    idl_update();
    if (!visit)
      edidldecl->setFocus();
  }
}

void AttributeDialog::default_description() {
  comment->setText(GenerationSettings::default_attribute_description());
}

void AttributeDialog::edit_description() {
  edit(comment->text(), edname->text().stripWhiteSpace() + "_description",
       att, TxtEdit, this, (post_edit) post_edit_description, edits);
}

void AttributeDialog::post_edit_description(AttributeDialog * d, QString s)
{
  d->comment->setText(s);
}

void AttributeDialog::edit_constraint() {
  edit(constraint->text(), edname->text().stripWhiteSpace() + "_constraint",
       att, TxtEdit, this, (post_edit) post_edit_constraint, edits);
}

void AttributeDialog::post_edit_constraint(AttributeDialog * d, QString s)
{
  d->constraint->setText(s);
}

void AttributeDialog::edit_init() {
  edit(edinit->text(), edname->text().stripWhiteSpace() + "_initialization",
       att, TxtEdit, this, (post_edit) post_edit_init, edits);
}

void AttributeDialog::post_edit_init(AttributeDialog * d, QString s)
{
  d->edinit->setText(s);
}

void AttributeDialog::cpp_default() {
  edcppdecl->setText((cpp_in_enum) ? GenerationSettings::cpp_default_enum_item_decl()
				   : GenerationSettings::cpp_default_attr_decl());
  cpp_update();
}

void AttributeDialog::cpp_unmapped() {
  edcppdecl->setText(QString::null);
  showcppdecl->setText(QString::null);
}

void AttributeDialog::cpp_update() {
  if (!cpp_in_enum)
    cpp_visibility.update_default(uml_visibility);
  
  // do NOT write
  //	const char * p = edcppdecl->text();
  // because the QString is immediatly destroyed !
  QString def = edcppdecl->text();
  const char * p = def;
  const char * pp = 0;
  QString indent = "";
  QString s;

  while ((*p == ' ') || (*p == '\t'))
    indent += *p++;
  
  if (*p != '#')
    s = indent;
  
  for (;;) {
    if (*p == 0) {
      if (pp == 0)
	break;
      
      // comment management done
      p = pp;
      pp = 0;
      if (*p == 0)
	break;
      if (*p != '#')
	s += indent;
    }
      
    if (!strncmp(p, "${comment}", 10))
      manage_comment(comment->text(), p, pp,
		     GenerationSettings::cpp_javadoc_style());
    else if (!strncmp(p, "${description}", 14))
      manage_description(comment->text(), p, pp);
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += edname->text();
    }
    else if ((cpp_in_enum && !strncmp(p, "${value}", 8)) ||
	     !strncmp(p, "${h_value}", 10)) {
      p += (p[2] == 'h') ? 10 : 8;
      if (!edinit->text().stripWhiteSpace().isEmpty() &&
	  (cpp_in_enum || classattribute_cb->isChecked())) {
	if (edinit->text().stripWhiteSpace().at(0) == QChar('='))
	  s += ' ';
	s += edinit->text();
      }
    }
    else if (!strncmp(p, "${value}", 8))
      p += 8;
    else if (*p == '\n') {
      s += *p++;
      if (*p && (*p != '#'))
	s += indent;
    }
    else if (cpp_in_enum)
      s += *p++;
    else if (!strncmp(p, "${static}", 9)) {
      p += 9;
      if (classattribute_cb->isChecked())
	s += "static ";
    }
    else if (!strncmp(p, "${const}", 8)) {
      p += 8;
      if (constattribute_cb->isChecked())
	s += "const ";
    }
    else if (!strncmp(p, "${mutable}", 10)) {
      p += 10;
      if (mutable_cb->isChecked())
	s += "mutable ";
    }
    else if (!strncmp(p, "${volatile}", 11)) {
      p += 11;
      if (volatile_cb->isChecked())
	s += "volatile ";
    }
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += GenerationSettings::cpp_type(type(edtype->currentText().stripWhiteSpace(),
					     list, nodes));
    }
    else if (*p == '@')
      manage_alias(att->browser_node, p, s, kvtable);
    else
      s += *p++;
  }

  showcppdecl->setText(s);
}

QString AttributeDialog::cpp_decl(const BrowserAttribute * at) 
{
  QString s;
  AttributeData * d = (AttributeData *) at->get_data();
  QCString decl = d->cpp_decl;
  
  remove_comments(decl);
  remove_preprocessor(decl);
  
  const char * p = decl;
  
  while ((*p == ' ') || (*p == '\t'))
    p += 1;
  
  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += at->get_name();
    }
    else if (!strncmp(p, "${value}", 8) || !strncmp(p, "${h_value}", 10))
      break;
    else if (!strncmp(p, "${static}", 9))
      p += 9;
    else if (!strncmp(p, "${const}", 8))
      p += 8;
    else if (!strncmp(p, "${mutable}", 10))
      p += 10;
    else if (!strncmp(p, "${volatile}", 11))
      p += 11;
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += GenerationSettings::cpp_type(d->type.get_type());
    }
    else if (*p == '\n') {
      s += ' ';
      do
	p+= 1;
      while ((*p == ' ') || (*p == '\t'));
    }
    else if (*p == ';')
      break;
    else if (*p == '@')
      manage_alias(at, p, s, 0);
    else
      s += *p++;
  }

  return s;
}

void AttributeDialog::java_default_item() {
  edjavadecl->setText(GenerationSettings::java_default_enum_item_decl());
  if (edstereotype->currentText().stripWhiteSpace() == "attribute")
    edstereotype->setCurrentItem(empty_st_rank);
  java_update();
}

void AttributeDialog::java_default() {
  if (java_in_enum_pattern)
    edjavadecl->setText(GenerationSettings::java_default_enum_pattern_item_decl());
  else {
    if (java_in_enum)
      edstereotype->setCurrentItem(attribute_st_rank);
    edjavadecl->setText(GenerationSettings::java_default_attr_decl());
  }
  java_update();
}

void AttributeDialog::java_unmapped() {
  edjavadecl->setText(QString::null);
  showjavadecl->setText(QString::null);
}

void AttributeDialog::java_update() {
  // do NOT write
  //	const char * p = edjavadecl->text();
  // because the QString is immediatly destroyed !
  QString def = edjavadecl->text();
  const char * p = def;
  const char * pp = 0;
  QString indent = "";
  QString s;

  while ((*p == ' ') || (*p == '\t'))
    indent += *p++;
  
  s = indent;
  
  for (;;) {
    if (*p == 0) {
      if (pp == 0)
	break;
      
      // comment management done
      p = pp;
      pp = 0;
      if (*p == 0)
	break;
      s += indent;
    }
      
    if (!strncmp(p, "${comment}", 10))
      manage_comment(comment->text(), p, pp,
		     GenerationSettings::java_javadoc_style());
    else if (!strncmp(p, "${description}", 14))
      manage_description(comment->text(), p, pp);
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += edname->text();
    }
    else if (!strncmp(p, "${value}", 8)) {
      p += 8;
      
      if (!edinit->text().stripWhiteSpace().isEmpty()) {
	if (edinit->text().at(0) == QChar('='))
	  s += ' ';
	s += edinit->text();
      }
      else if (java_in_enum_pattern)
	s += " = ...";
    }
    else if (*p == '\n') {
      s += *p++;
      if (*p)
	s += indent;
    }
    else if (!strncmp(p, "${@}", 4)) {
      p += 4;
      if (pp != 0)
	s += "${@}";
      else if (!javaannotation.isEmpty()) {
	pp = p;
	p = javaannotation;
      }
    }
    else if (*p == '@')
      manage_alias(att->browser_node, p, s, kvtable);
    else if (java_in_enum_pattern) {
      if (!strncmp(p, "${class}", 8)) {
	p += 8;
	s += ((BrowserNode *) att->browser_node->parent())->get_name();
      }
      else
	s += *p++;
    }
    else if (!strncmp(p, "${visibility}", 13)) {
      p += 13;
      if (uml_visibility.value() != UmlPackageVisibility)
	s += uml_visibility.state() + ' ';
    }
    else if (!strncmp(p, "${static}", 9)) {
      p += 9;
      if (classattribute_cb->isChecked())
	s += "static ";
    }
    else if (!strncmp(p, "${transient}", 12)) {
      p += 12;
      if (!java_in_enum && !java_in_enum_pattern && !java_in_typedef && transient_cb->isChecked())
	s += "transient ";
    }
    else if (!strncmp(p, "${volatile}", 11)) {
      p += 11;
      if (volatile_cb->isChecked())
	s += "volatile ";
    }
    else if (!strncmp(p, "${final}", 8)) {
      p += 8;
      if (constattribute_cb->isChecked())
	s += "final ";
    }
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += GenerationSettings::java_type(type(edtype->currentText().stripWhiteSpace(),
					      list, nodes));
    }
    else
      s += *p++;
  }

  editjavaannotation->setEnabled(def.find("${@}") != -1);
  
  showjavadecl->setText(s);
}

QString AttributeDialog::java_decl(const BrowserAttribute * at)
{
  QString s;
  AttributeData * d = (AttributeData *) at->get_data();
  QCString decl = d->java_decl;
  
  remove_comments(decl);
  
  const char * p = decl;
  
  while ((*p == ' ') || (*p == '\t'))
    p += 1;
  
  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += at->get_name();
    }
    else if (!strncmp(p, "${value}", 8))
      break;
    else if (!strncmp(p, "${class}", 8)) {
      p += 8;
      s += ((BrowserNode *) at->parent())->get_name();
    }
    else if (!strncmp(p, "${visibility}", 13))
      p += 13;
    else if (!strncmp(p, "${static}", 9))
      p += 9;
    else if (!strncmp(p, "${transient}", 12))
      p += 12;
    else if (!strncmp(p, "${volatile}", 11))
      p += 11;
    else if (!strncmp(p, "${final}", 8)) 
      p += 8;
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += GenerationSettings::java_type(d->type.get_type());
    }
    else if (!strncmp(p, "${@}", 4))
      p += 4;
    else if (*p == '\n') {
      s += ' ';
      do
	p+= 1;
      while ((*p == ' ') || (*p == '\t'));
    }
    else if (*p == ';')
      break;
    else if (*p == '@')
      manage_alias(at, p, s, 0);
    else
      s += *p++;
  }

  return s;
}

void AttributeDialog::java_edit_annotation() {  
  AnnotationDialog dialog(javaannotation, !hasOkButton());
  
  if (dialog.exec() == QDialog::Accepted)
    java_update();
}
  
void AttributeDialog::idl_default() {
  if (idl_in_enum)
    edidldecl->setText(GenerationSettings::idl_default_enum_item_decl());
  else if (idl_in_union)
    edidldecl->setText(GenerationSettings::idl_default_union_item_decl());
  else
    edidldecl->setText((constattribute_cb->isChecked() &&
			!edinit->text().stripWhiteSpace().isEmpty())
		       ? GenerationSettings::idl_default_const_decl()
		       : GenerationSettings::idl_default_attr_decl());
  idl_update();
}

void AttributeDialog::idl_default_state() {
  edidldecl->setText(GenerationSettings::idl_default_valuetype_attr_decl());
  idl_update();
}

void AttributeDialog::idl_unmapped() {
  edidldecl->setText(QString::null);
  showidldecl->setText(QString::null);
}

void AttributeDialog::idl_update() {
  // do NOT write
  //	const char * p = edidldecl->text();
  // because the QString is immediatly destroyed !
  QString def = edidldecl->text();
  const char * p = def;
  const char * pp = 0;
  QString indent = "";
  QString s;

  while ((*p == ' ') || (*p == '\t'))
    indent += *p++;
  
  if (*p != '#')
    s = indent;
  
  for (;;) {
    if (*p == 0) {
      if (pp == 0)
	break;
      
      // comment management done
      p = pp;
      pp = 0;
      if (*p == 0)
	break;
      s += indent;
    }
      
    if (!strncmp(p, "${comment}", 10))
      manage_comment(comment->text(), p, pp, FALSE);
    else if (!strncmp(p, "${description}", 14))
      manage_description(comment->text(), p, pp);
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += edname->text();
    }
    else if (!strncmp(p, "${value}", 8)) {
      p += 8;
      s += ' ';
      s += edinit->text();
    }
    else if (*p == '\n') {
      s += *p++;
      if (*p && (*p != '#'))
	s += indent;
    }
    else if (idl_in_enum)
      s += *p++;
    else if (idl_in_union && !strncmp(p, "${case}", 7)) {
      p += 7;
      s += edcase->currentText().stripWhiteSpace();
    }
    else if (!strncmp(p, "${attribut}", 11)) {
      // old version
      p += 11;
      if (!idl_in_struct)
	s += "attribute ";
    }
    else if (!strncmp(p, "${attribute}", 12)) {
      p += 12;
      if (!idl_in_struct)
	s += "attribute ";
    }
    else if (!strncmp(p, "${visibility}", 13)) {
      p += 13;
      switch (uml_visibility.value()) {
      case UmlPublic:
      case UmlPackageVisibility:
	s += "public ";
      default:
	s += "private ";
      }
    }
    else if (!strncmp(p, "${readonly}", 11)) {
      p += 11;
      if (constattribute_cb->isChecked())
	s += "readonly ";
    }
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += GenerationSettings::idl_type(type(edtype->currentText().stripWhiteSpace(),
					     list, nodes));
    }
    else if (*p == '@')
      manage_alias(att->browser_node, p, s, kvtable);
    else
      s += *p++;
  }

  showidldecl->setText(s);
}

QString AttributeDialog::idl_decl(const BrowserAttribute * at) 
{
  QString s;
  AttributeData * d = (AttributeData *) at->get_data();
  QCString decl = d->idl_decl;
  QString stereotype = ((BrowserNode *) at->parent())->get_data()->get_stereotype();
  bool in_enum = (stereotype == "enum") ||
    (GenerationSettings::idl_class_stereotype(stereotype) == "enum");
  
  remove_comments(decl);
  
  const char * p = decl;
  
  while ((*p == ' ') || (*p == '\t'))
    p += 1;
  
  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += at->get_name();
    }
    else if (!strncmp(p, "${value}", 8))
      break;
    else if (!strncmp(p, "${case}", 7)) {
      p += 7;
      s += d->get_idlcase();
    }
    else if (!strncmp(p, "${attribut}", 11))
      // old version
      p += 11;
    else if (!strncmp(p, "${attribute}", 12))
      p += 12;
    else if (!strncmp(p, "${visibility}", 13))
      p += 13;
    else if (!strncmp(p, "${readonly}", 11))
      p += 11;
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += GenerationSettings::idl_type(d->type.get_type());
    }
    else if (*p == '\n') {
      s += ' ';
      do
	p+= 1;
      while ((*p == ' ') || (*p == '\t'));
    }
    else if ((*p == ';') || (in_enum && (*p == ',')))
      break;
    else if (*p == '@')
      manage_alias(at, p, s, 0);
    else
      s += *p++;
  }

  return s;
}

