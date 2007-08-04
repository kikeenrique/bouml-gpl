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

#ifdef DEBUG_BOUML
#include <iostream>

using namespace std;
#endif

#include <qfiledialog.h> 
#include <qapplication.h>
#include <qmessagebox.h>
#include <qdir.h>

#include "Package.h"
#include "Class.h"
#include "UmlPackage.h"
#include "UmlClass.h"
#include "UmlAttribute.h"
#include "UmlRelation.h"
#include "UmlOperation.h"
#include "Lex.h"
#include "UmlCom.h"
#include "CppSettings.h"
#include "CppCatWindow.h"
#include "Statistic.h"
#include "Progress.h"

// create all packages under it
Package * Package::Root;

// the packages selected by the user to reverse them
QList<Package> Package::Choozen;

// all the classes presents in the environment, the
// key is the full name including namespaces
NDict<Class> Package::Declared(511);
NDict<Class> Package::Defined(511);

// to place unknown classes
//Package * Package::Unknown;

// to know if it is the scan step or the reverse step
bool Package::Scan;

// current template forms
QValueList<FormalParameterList> Package::Formals;

// to show a progress bar
Progress * Package::ProgressBar;
QApplication * Package::app;
int Package::Nfiles;

// memorise the name of the header currently reversed
// to create artifact having the right name and respect
// the classes defined in a same header file
QCString Package::fname;


#ifndef REVERSE
Package::Package(BrowserView * parent, UmlPackage * u)
    : BrowserNode(parent, u->Name()) {
  uml = u;
}
#endif

Package::Package(Package * parent, QString p, QString n)
    : BrowserNode(parent, n) {
  path = p;
  uml = 0;
}

// to not have warning
Package::~Package() {
}

bool Package::isa_package() const {
  return TRUE;
}

#ifndef REVERSE
QString Package::get_path() const {
  return path;
}
#endif

void Package::init(UmlPackage * r, QApplication * a)
{
#ifdef REVERSE
  Root = new Package(0, 0, r->name());
  Root->uml = r;
#else
  root = new Package(BrowserView::instance(), r);
#endif
  app = a;
}

bool Package::scan_dirs() {
  QStringList dirs;
  QString path = QDir::currentDirPath();

  // get input C++ source dirs
  while (!(path = QFileDialog::getExistingDirectory(path, 0, 0,
						   "select a directory to reverse, press cancel to finish"))
						   .isEmpty()) {

    int pathlen = path.length();
    
    if (((const char *) path)[pathlen - 1] != '/') {
      path += "/";
      pathlen += 1;
    }
    
    QString err = 0;
    
    for (QStringList::Iterator it = dirs.begin(); it != dirs.end(); ++it) {
      int dlen = (*it).length();
      
      if (dlen >= pathlen) {
	if ((*it).left(pathlen) == path) {
	  err = (dlen == pathlen)
	    ? path + " is already selected\n"
	    : "An already selected directory is a subdirectory of " + path;
	  break;
	}
      }
      else if (path.left(dlen) == *it) {
	err = path + " is a subdirectory of an already selected directory\n";
	break;
      }
    }
		   
    if (!err.isEmpty())
      QMessageBox::warning(0, "C++ reverse",
			    err + "Press 'cancel' to reverse selected directories");
    else {
      QDir d(path);
      QCString s;
      
      dirs.append(path);
      s.sprintf("<font face=helvetica>%dth directory to reverse : <b>", dirs.count());
      s += QCString(path) + "</b><br></font>\n";
      UmlCom::trace(s);

      Choozen.append(new Package(Root, path, d.dirName()));
      
      d.cdUp();
      path = d.absPath();
    }
  }
  
  if (dirs.isEmpty())
    return FALSE;

  UmlCom::trace("<hr>\n");
  
  dirs.clear();

#ifndef REVERSE
  QApplication::setOverrideCursor(Qt::waitCursor);
  CppCatWindow::clear_trace();
#endif

  // count files
  Package * p;

  Nfiles = 0;

  for (p = Choozen.first(); p != 0; p = Choozen.next()) {
    QDir d(p->path);

    Nfiles += file_number(d, TRUE, 
			  CppSettings::headerExtension(),
			  CppSettings::sourceExtension());
  }
    
  // scanning phase
  ProgressBar = new Progress(Nfiles, "Scanning in progress, please wait ...");
  Scan = TRUE;

  for (p = Choozen.first(); p != 0; p = Choozen.next()) {
    QDir d(p->path);

    p->reverse_directory(d, TRUE, CppSettings::headerExtension(), TRUE);
    p->reverse_directory(d, TRUE, CppSettings::sourceExtension(), FALSE);
  }
  Scan = FALSE;

  if (ProgressBar != 0)
    delete ProgressBar;
#ifndef REVERSE
  QApplication::restoreOverrideCursor();
    
  Root->setOpen(TRUE);
#endif
    
  return TRUE;
}
    
void Package::progress_closed()
{
  ProgressBar = 0;
}

int Package::file_number(QDir & d, bool rec, const char * h, const char * cpp)
{
  int result = 0;
  const QFileInfoList * list = d.entryInfoList(QDir::Files | QDir::Readable);
  QFileInfoListIterator it(*list);
  QFileInfo * fi;
  
  while ((fi = it.current()) != 0) {
    if ((fi->extension() == h) || (fi->extension() == cpp))
      result += 1;
    ++it;
  }

  if (rec) {
    // sub directories
    list = d.entryInfoList(QDir::Dirs | QDir::NoSymLinks);
    QFileInfoListIterator itd(*list);
    QFileInfo * di;
    
    while ((di = itd.current()) != 0) {
      if (((const char *) di->fileName())[0] != '.') {
	QDir sd(di->filePath());
	
	result += file_number(sd, rec, h, cpp);
      }
      ++itd;
    }
  }
  
  return result;
}

void Package::send_dirs(bool rec) {
  // reverse phase
#ifdef REVERSE
  UmlItem * prj = Root->uml;
#else
  UmlItem * prj = root;
#endif

  while (prj->parent() != 0)
    prj = prj->parent();
  
  prj->set_childrenVisible(FALSE);
  
#ifndef REVERSE
  QApplication::setOverrideCursor(Qt::waitCursor);
#endif
  ProgressBar = new Progress(Nfiles, "Reverse in progress, please wait ...");

  Package * p;

  for (p = Choozen.first(); p != 0; p = Choozen.next()) {
    QDir d(p->path);
  
    p->reverse_directory(d, rec, CppSettings::headerExtension(), TRUE);
    p->reverse_directory(d, rec, CppSettings::sourceExtension(), FALSE);
  }

  if (ProgressBar != 0)
    delete ProgressBar;

#ifndef REVERSE
  QApplication::restoreOverrideCursor();
#endif
  
#ifdef REVERSE
  Root->uml->set_childrenVisible(TRUE);
#else
  root->set_childrenVisible(TRUE);
#endif
}

void Package::reverse_directory(QDir & d, bool rec, QString ext, bool h) {
  // reads files
  const QFileInfoList * list =
    d.entryInfoList("*." + ext, QDir::Files | QDir::Readable);
  QFileInfoListIterator it(*list);
  
  while (it.current() != 0) {
    if (h)
      fname = it.current()->baseName();
    reverse_file(QCString(it.current()->filePath()));
    if (ProgressBar)
      ProgressBar->tic();
    app->processEvents();
    ++it;
  }

  if (rec) {
    // sub directories
    list = d.entryInfoList(QDir::Dirs | QDir::NoSymLinks);
    QFileInfoListIterator itd(*list);
    QFileInfo * di;
    
    while ((di = itd.current()) != 0) {
      if (((const char *) di->fileName())[0] != '.') {
	QDir sd(di->filePath());
	Package * p = find(QCString(sd.dirName()));
	
	if (p != 0)
	  p->reverse_directory(sd, TRUE, ext, h);
      }
      ++itd;
    }
  }
  
  fname = "";
}

void Package::reverse_file(QCString f) {
  if (! Lex::open(f))
    // very strange !
    CppCatWindow::trace(QCString("<font face=helvetica><b>cannot open <i>")
			+ f + "</i></b></font><br><hr><br>");  
  else {    
    UmlCom::message(((Scan) ? "scan " : "reverse ") + f);
    
    reverse_toplevel_forms(f, FALSE);
    
    Formals.clear();
    UmlClass::clear_usings();
    Namespace::clear_usings();
    Namespace::clear_aliases();
    Lex::close();
  }
}

void Package::reverse_toplevel_forms(QCString f, bool sub_block) {
  QCString pretype;
  QCString s;
  
  while (! (s = Lex::read_word()).isEmpty()) {
    if (s == "template") {
      FormalParameterList fmt;
      
      Formals.append(fmt);
      get_template(Formals.last());
    }
    else {
      if ((s == "class") || (s == "struct") || (s == "union")) {
	Lex::mark();
	
	QCString s2 = Lex::read_word();
	
	if ((strncmp(s2, "Q_EXPORT", 8) == 0) ||
	    (strncmp(s2, "QM_EXPORT", 9) == 0) ||
	    (strncmp(s2, "Q_PNGEXPORT", 11) == 0))
	  s2 = Lex::read_word();
	
	if (Lex::identifierp(s2, TRUE) &&
	    ((s2 = Lex::read_word()) != "{") && (s2 != ":") && (s2 != ";")) {
	  // form like 'class X Y'
	  pretype = s;
	  Lex::come_back();
	}
	else {
	  Lex::come_back();
	  // do not manage 'struct {..} var;' or 'struct {..} f()' ...
	  Class::reverse(this, s, Formals, f);
	}
      }
      else if (s == "enum") {
	Lex::mark();
	
	QCString s2 = Lex::read_word();
	
	if (Lex::identifierp(s2, TRUE) && (Lex::read_word() != "{")) {
	  // form like 'enum X Y'
	  pretype = s;
	  Lex::come_back();
	}
	else {
	  Lex::come_back();
	  // do not manage 'enum {..} var;' or 'enum {..} f()' ...
	  Class::reverse_enum(this, f);
	}
      }
      else if (s == "typedef")
	Class::reverse_typedef(this, f, Formals);
      else if (s == "namespace") {
	s = Lex::read_word();
	
	QCString s2 = Lex::read_word();
	
	if (s2.isEmpty()) {
	  if (!Scan)
	    Lex::premature_eof();
	  return;
	}
	else if (s2 == "{") {
	  Namespace::enter(s);
	  UmlClass::save_using_scope();
	  reverse_toplevel_forms(f, TRUE);
	  UmlClass::restore_using_scope();
	  Namespace::exit();
	}
	else if (Scan)
	  UmlOperation::skip_body();
	else if (s2 != "=") {
	  Lex::error_near(s2);
	  UmlOperation::skip_body();
	}
	else if ((s2 = Lex::read_word()).isEmpty()) {
	  Lex::premature_eof();
	  return;
	}
	else
	  Namespace::add_alias(s, s2);
      }
      else if (s == "using") {
	if ((s = Lex::read_word()).isEmpty()) {
	  if (!Scan)
	    Lex::premature_eof();
	  return;
	}
	else if (s == "namespace") {
	  if (Scan)
	    UmlOperation::skip_body(0);
	  else if ((s = Lex::read_word()).isEmpty()) {
	    Lex::premature_eof();
	    return;
	  }
	  else
	    Namespace::add_using(s);
	}
	else if (!Scan) {
	  s = Lex::complete_template_type(s);
	  
	  UmlTypeSpec typespec;
	  
	  if (!find_type(s, typespec)) {
#if 0
	    int index = s.findRev("::");
	    
	    if ((index == -1) ||
		(!find_type(s = s.left(index), typespec) &&
		 !s.isEmpty()))
	      Lex::warn("unknown class <font color =\"red\">" +
			Lex::quote(s) + "</font>");
	    else
	      Lex::warn("using form lost");
#endif
	  }
	  else
	    typespec.type->using_it();
	}
	else
	  UmlOperation::skip_body(0);
      }
      else if (s == ";")
	;
      else if (sub_block && (s == "}")) {
	Formals.clear();
	break;
      }
      else if ((s == "QM_TEMPLATE_EXTERN_CANVAS") ||
	       (s == "Q_TEMPLATE_EXTERN_CANVAS") ||
	       (s == "Q_TYPENAME"))
	; // lost
      else if (Scan && (s != "extern") && (s != "typename")) {
#ifdef DEBUG_BOUML
	cout << "reverse_toplevel_forms skip_body sur '" << s << "'\n";
#endif
	UmlOperation::skip_body();
      }
      else
	reverse_toplevel_form(f, s);
      
      Formals.clear();
    }
  }
#ifdef DEBUG_BOUML
  cout << "exit Package::reverse_toplevel_forms avec '" << s << "'\n";
#endif
}

void Package::reverse_toplevel_form(QCString f, QCString s) {
  QCString comment = Lex::get_comments();
  QCString description = Lex::get_description();
  QCString q_modifier;	// not yet used
  bool inlinep = FALSE;
  QCString type;
  QCString name;
  QCString array;

#ifdef DEBUG_BOUML
  cout << "Package::reverse_toplevel_form(" << s << ")\n";
#endif
  
  for (;;) {
    if ((s == "static") || (s == "const") ||
	(s == "typename")|| (s == "volatile"))
      // note that for an operation 'const' or 'typename' are
      // useless to find the corresponding declaration
      ;
    else if ((s == "class") || (s == "struct") ||
	     (s == "enum") || (s == "union"))
      // pre-type useless to find the corresponding declaration
      ;
    else if (s == "inline")
      inlinep = TRUE;
    else if ((s == "unsigned") || (s == "signed") ||
	     (s == "void") || (s == "bool"))
      type = s;
    else if ((s == "char") || (s == "short") || (s == "int") || 
	     (s == "long") || (s == "float") || (s == "double")) {
      type = (type.isEmpty()) ? s : type + ' ' + s;
    }
    else if (Lex::star(s) || (s == "&"))
      ;
    else if (s == "(") {
      // suppose a function or an operation
      if (Scan)
	UmlOperation::skip_body(0);
      else if (name == "main")
	get_uml()->reverse_main(type, comment);
      else
	UmlOperation::reverse_definition(this, name, type, Formals,
					 inlinep, comment, description);
      return;
    }
    else if (s == "=") {
      // initialized variable
      reverse_variable(name);
      return;
    }
    else if (s == ";") {
      // not initialized variable, does nothing
      return;
    }
    else if ((s == "extern") || (s == "Q_TEMPLATE_EXTERN")) {
      if (!(s = Lex::read_word()).isEmpty()) {
	if (*s == '"') {
	  // extern "language" {
	  if (! Scan)
	    Lex::warn("sorry, <font color =\"red\">extern " + Lex::quote(s)
		      + "</font> is lost");
	  if ((s = Lex::read_word()) == "{")
	    reverse_toplevel_forms(f, TRUE);
	  else {
	    if (! Scan)
	      Lex::error_near(s);
	    UmlOperation::skip_body(0);
	  }
	}
	else
	  UmlOperation::skip_body(0);
      }

      return;
    }
    else if ((strncmp(s, "Q_EXPORT", 8) == 0) ||
	     (strncmp(s, "QM_EXPORT", 9) == 0) ||
	     (strncmp(s, "Q_PNGEXPORT", 11) == 0))
      q_modifier = s;
    else if ((s == "Q_INLINE_TEMPLATES") ||
	     (s == "Q_TYPENAME"))
      ; // lost
    else if (Lex::identifierp(s, FALSE)) {
      if (type.isEmpty()) {
	type = s = Lex::complete_template_type(s);

#ifdef DEBUG_BOUML
	cout << "type = '" << type << "'\n";
#endif
      }
      else if (name.isEmpty()) {
	if (type.right(3) == "::~") {
	  type += s;
#ifdef DEBUG_BOUML
	  cout << "type => '" << type << "'\n";
#endif
	}
	else {
	  name = Lex::complete_template_type(s);

#ifdef DEBUG_BOUML
	  cout << "name = '" << name << "'\n";
#endif
	}
      }
      else {
#ifdef DEBUG_BOUML
	cout << "ERROR '" << s << "' alors qu a deja le type '" << type << "' et le nom '" << name << "'\n";
#endif
	break;
      }
    }
    else if (*((const char *) s) == '[')
      array += s;
    else {
#ifdef DEBUG_BOUML
      cout << "ERROR : '" << s << "'\n";
#endif
      break;
    }
    
    QCString s2 = Lex::read_word();
    
    if (s2.isEmpty())
      break;
    
    s = s2;
  }
  
  // here we are in error
  Lex::error_near(s);
  UmlOperation::skip_body((s == "{") ? 1 : 0);
  Lex::finish_line();
  Lex::clear_comments();
}

void Package::reverse_variable(const QCString & name) {
  // '=' or '(' read
  QCString init;
  
  Lex::mark();
  
  while ((init = Lex::read_word()) != ";")
    if (init.isEmpty())
      return;

  init = Lex::region();
  
  if (name.isEmpty()) {
    Lex::syntax_error();
#ifdef DEBUG_BOUML
    cout << "ERROR ';' et name.isEmpty() || type.isEmpty()\n";
#endif
    return;
  }
  
  UmlTypeSpec typespec;
  int index = name.findRev("::");
  QCString varname;
  
  if ((index <= 0) ||
      !find_type(Lex::normalize(name.left(index)), typespec)) {
    Lex::warn(QCString("<font color =\"red\"> ") + Lex::quote(name) +
	      "</font> is lost");
#ifdef DEBUG_BOUML
    cout << "ERROR " << name << " lost";
#endif
    return;
  }
  else
    varname = name.mid(index + 2);
  
  init = QCString("=") + init.left(init.length() - 1);
  
  // search the corresponding attribute
  QVector<UmlItem> children = typespec.type->children();
  unsigned rank = children.size();
  
  while (rank--) {
    UmlItem * it = children[rank];
    
    switch (it->kind()) {
    case anAttribute:
      if (it->name() == varname) {
	if (((UmlClassMember *) it)->isClassMember()) {
	  ((UmlAttribute *) it)->set_DefaultValue(init);
	  
	  QCString decl = ((UmlAttribute *) it)->cppDecl();
	  int index = decl.find("${h_value}");
	  
	  if (index != -1) {
	    decl.remove(index + 2,  2);
	    ((UmlAttribute *) it)->set_CppDecl(decl);
	  }
	}
	return;
      }
      break;
    case aRelation:
      if (((UmlRelation *) it)->roleName() == varname) {
	if (((UmlRelation *) it)->isClassMember()) {
	  ((UmlRelation *) it)->set_DefaultValue(init);
	  
	  QCString decl = ((UmlRelation *) it)->cppDecl();
	  int index = decl.find("${h_value}");
	  
	  if (index != -1) {
	    decl.remove(index + 2,  2);
	    ((UmlRelation *) it)->set_CppDecl(decl);
	  }
	}
	return;
      }
    default:	// to avoid compiler warning
      break;
    }
  }
  
  // no compatible variable
  Lex::warn(QCString("<font color =\"red\"> ") + Lex::quote(name)
	    + "</font> is not a static attribute of <font color =\"red\"> " +
	    Lex::quote(typespec.type->name()) + "</font>");
#ifdef DEBUG_BOUML
  cout << "ERROR " << name << " undeclared static attribute\n";
#endif
}

Class * Package::new_class(const QCString & name,
			   const QCString & stereotype,
			   bool declaration) {
  Class * cl = new Class(this, name, stereotype);
        
  if (! name.isEmpty())
    ((declaration) ? Declared : Defined).insert(name, cl);
  
  return cl;
}

bool Package::find_type(QCString type, UmlTypeSpec & typespec) {
  return ClassContainer::find_type(type, typespec, Defined);
}

Class * Package::declare_if_needed(const QCString & name,
					 QCString stereotype) {
  FormalParameterList l;
  
  return ClassContainer::declare_if_needed(name, stereotype, l,
					   Declared, Defined);
}

void Package::declare_if_needed(QCString name, Class * cl) {
  if (Defined[name] == 0)
    Declared.replace(name, cl);
}

Class * Package::define(const QCString & name, QCString stereotype) {
  return ClassContainer::define(name, stereotype, Declared, Defined);
}

void Package::define(QCString name, Class * cl) {
  if (! name.isEmpty()) {
    if (Declared[name] != 0)
      Declared.remove(name);
    
    Defined.replace(name, cl);
  }
}

void Package::declaration(const QCString &, const QCString &,
			  const QCString &) {
  // lost it
}

UmlPackage * Package::get_uml(bool mandatory) {
  if (uml == 0) {
    const char * name = text(0);
    Package * pa = (Package *) parent();
    UmlPackage * uml_pa = pa->get_uml();	// will end on project
	
    QVector<UmlItem> ch = uml_pa->children();
    
    for (unsigned index = 0; index != ch.size(); index += 1) {
      UmlItem * it = ch[index];
      
      if ((it->kind() == aPackage) && (it->name() == name))
	return uml = (UmlPackage *) it;
    }
    
    if ((uml = UmlBasePackage::create(uml_pa, name)) == 0) {
      if (!mandatory)
	return 0;
      
#ifdef REVERSE
      UmlCom::trace(QCString("<font face=helvetica><b>cannot create package <i>")
		    + name + "</i> under package <i>" + uml_pa->name() +
		    "</b></font><br>");
      UmlCom::message("");
      throw 0;
#else
      QMessageBox::critical(0, "Fatal Error", 
			    QString("<font face=helvetica><b>cannot create package <i>")
			    + name + "</i> under package <i>"
			    + ((const char *) uml_pa->name()) + "</b></font><br>");
      QApplication::exit(1);
#endif
    }

    if (! namespace_.isEmpty())
      uml->set_CppNamespace(namespace_);
    
#ifdef DEBUG_BOUML
#warning A FAIRE
#endif
    /*uml->set_CppSrcDir(?);
    uml->set_CppHDir(?);*/
  }
  
  return uml;
}

Package * Package::find(QCString s) {
  TreeItem * child;
    
  for (child = firstChild(); child != 0; child = child->nextSibling())
    if (((BrowserNode *) child)->isa_package() &&
	(child->text(0) == (const char *) s))
      return (Package *) child;
  
  return new Package(this, path + "/" + s, s);
}

/*
Package * Package::unknown()
{
  if (Unknown != 0)
    return Unknown;
  
  Unknown = new Package(Root, "<unknown>", "unknown");
  
  if (Unknown->get_uml(FALSE) == 0) {
    int i = 1;
    
    do {
      Unknown->setText(0, "unknown" + QString::number(i));
    } while (Unknown->get_uml(FALSE) == 0);
  }
  
  return Unknown;
}
*/
