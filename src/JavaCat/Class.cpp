// *************************************************************************
//
// Copyleft 2004-2008 Bruno PAGES  .
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

#include <qdir.h>
#include <qdatastream.h> 
#ifndef REVERSE
#include <qapplication.h>
#include <qpopupmenu.h> 
#include <qcursor.h>
#include <qpainter.h>
#include <qmessagebox.h>
#endif

#include "Class.h"
#include "UmlClass.h"
#include "Lex.h"
#include "Package.h"
#include "UmlPackage.h"
#include "UmlOperation.h"
#include "UmlRelation.h"
#include "UmlAttribute.h"
#include "UmlCom.h"
#include "JavaSettings.h"
#include "JavaCatWindow.h"
#ifdef REVERSE
#include "Statistic.h"
#include "UmlExtraClassMember.h"

UmlArtifact * Class::CurrentArtifact;
#else
#include "BrowserView.h"
#include "Pixmap.h"
#include "ShowFileDialog.h"

QList<Class> Class::Historic;
#endif

Class::Class(BrowserNode * p, const char * n, char st)
    : BrowserNode(p, n),
      uml(0), stereotype(st), abstractp(FALSE), reversedp(FALSE),
#ifdef REVERSE
      from_lib(FALSE)
#else
      description_updatedp(FALSE)
#endif
{
  // in case the package is java[.*] the (sub)class is exported
  while (!p->isa_package())
    p = (BrowserNode *) p->parent();
  
  ((Package *) p)->new_class(this);
}

UmlClass * Class::get_uml() {
  if (uml != 0)
    return uml;

  UmlItem * p = (((BrowserNode *) parent())->isa_package())
    ? (UmlItem *) ((Package *) parent())->get_uml()->get_classview()
    : (UmlItem *) ((Class *) parent())->get_uml();
  QCString str = QCString(text(0));
			  
  uml = UmlBaseClass::create(p, str);
    
  if (uml == 0) {
    // probably already exist
    QVector<UmlItem> ch = p->children();
    UmlItem * x;
    
    for (unsigned chindex = 0; chindex != ch.size(); chindex += 1) {
      if (((x = ch[chindex])->kind() == aClass) && (x->name() == str)) {
	uml = (UmlClass *) x;
	break;
      }
    }
    
    if (uml == 0) {
#ifdef REVERSE
      UmlCom::message("");
      UmlCom::trace(QString("<font face=helvetica><b>cannot create class <i>")
		    + text(0) + "</i> under <i>"
		    + parent()->text(0) + "</b></font><br>");
      throw 0;
#else
      QMessageBox::critical(0, "Fatal Error", 
			    QString("<font face=helvetica><b>cannot create class <i>")
			    + text(0) + "</i> under <i>"
			    + parent()->text(0) + "</b></font><br>");
      QApplication::exit(1);
#endif
    }
  }
  
  switch (stereotype) {
  case 'i':
    uml->set_Stereotype("interface");
    uml->set_JavaDecl(JavaSettings::interfaceDecl());
    break;
  case '@':
    {
      QCString s = JavaSettings::interfaceDecl();
      int index = s.find("interface");
      
      if (index != -1)
	s.insert(index, '@');
      
      uml->set_Stereotype("@interface");
      uml->set_JavaDecl(s);
    }
    break;
  case 'e':
    uml->set_Stereotype("enum");
    uml->set_JavaDecl(JavaSettings::enumDecl());
    break;
  default:
    // class
    break;
  }
  
  if (abstractp)
    uml->set_isAbstract(TRUE);
  
  unsigned rank;
  FormalParameterList::ConstIterator it;
  
  for (rank = 0, it = formals.begin(); it != formals.end(); it++, rank += 1)
    uml->addFormal(rank, *it);
  
  formals.clear();
  
  return uml;
}

bool Class::already_in_bouml() {
  QVector<UmlItem> ch = get_uml()->children();
  
  for (unsigned index = 0; index != ch.size(); index += 1)
    if (ch[index]->kind() != aClass)
      return TRUE;

  return FALSE;
}

// note : 'tmplts' must be given by value to not have to
// remove the may be added formals in all the return cases

bool Class::reverse(ClassContainer * container, QCString stereotype,
		    QCString annotation, bool abstractp, bool finalp,
		    aVisibility visibility, QCString & path, 
		    QValueList<FormalParameterList> tmplts)
{
  QCString comment = Lex::get_comments();
  QCString description = Lex::get_description();
  QCString name;
  
  if ((name = Lex::read_word()).isEmpty())
    return FALSE;
  
  Class * cl = container->define(name, *((const char *) stereotype));
  UmlClass * cl_uml = 0;
    
  if ((cl == 0) || cl->reversedp)
    return FALSE;
  
  if (Package::scanning()) {
    cl->abstractp = abstractp;
    cl->filename = path;
#ifndef REVERSE
    cl->description = comment;
#endif
  }
  else {
#ifdef REVERSE
    Statistic::one_class_more();
#endif
    cl->reversedp = TRUE;
    
    cl_uml = cl->get_uml();
    
    if (cl->already_in_bouml()) {
      cl_uml->unload();
      return FALSE;
    }
    
    if (! comment.isEmpty())
      cl_uml->set_Description((cl_uml->javaDecl().find("${description}") != -1)
			      ? description : comment);
    
    cl_uml->set_Visibility(visibility);
    
    if (! annotation.isEmpty())
      cl_uml->set_JavaAnnotations(annotation);
    
    if (abstractp)
      cl_uml->set_isAbstract(abstractp);
    
    if (finalp)
      cl_uml->set_isJavaFinal(finalp);
  }
  
  QCString s = Lex::read_word();
  
  if (s == "<") {
    if (!cl->get_formals(cl->formals, FALSE, tmplts))
      return FALSE;
    
    tmplts.append(cl->formals);
    
    // note : formals set on cl_uml by get_uml()
    
    s = Lex::read_word();
  }
  
  if (s.isEmpty()) {
    if (! Package::scanning()) {
      Lex::premature_eof();
      cl_uml->unload();
    }
    return FALSE;
  }
  
  if (Package::scanning()) {
    while (s != "{") {
      if ((s = Lex::read_word()).isEmpty())
	return FALSE;
    }
  }
  else {
    if (s == "extends") {
      if (cl->stereotype == 'e') {
	Lex::syntax_error("an enum can't extend");
	if (! Package::scanning())
	  cl_uml->unload();
	return FALSE;
      }
      else if ((cl->stereotype != 'i')
	       ? !cl->manage_extends(container, tmplts)
	       : !cl->manage_implements(container, aGeneralisation, tmplts))
	return FALSE;
      s = Lex::read_word();
    }
    
    if (s == "implements") {
      if (! cl->manage_implements(container, aRealization, tmplts))
	return FALSE;
      s = Lex::read_word();
    }
  
    if (s != "{") {
      if (! Package::scanning()) {
	Lex::syntax_error("<font color =\"red\">{</font> is missing");
	cl_uml->unload();
      }
      return FALSE;
    }
  }
  
  if ((cl->stereotype == 'e') && !cl->manage_enum_items()) {
    if (! Package::scanning())
      cl_uml->unload();
    return FALSE;
  }
  
  while ((s = Lex::read_word()) != "}") {
    if (s.isEmpty()) {
      if (! Package::scanning()) {
	Lex::premature_eof();
	cl_uml->unload();
      }
      return FALSE;
    }
#ifdef TRACE
    cout << "in class def a lu '" << s << "'\n";
#endif
    if (s == ";")
      ;
    else if (! cl->manage_member(s, path)) {
      if (! Package::scanning())
	cl_uml->unload();
      return FALSE;
    }
  }
  
  if (! Package::scanning()) {
    if (cl->stereotype == 'e')
      // may be not abstract even an oper is abstract
      cl_uml->set_isAbstract(abstractp);
    
#ifdef REVERSE
    if (!cl->from_libp() && ((BrowserNode *) cl->parent())->isa_package()) {
      Package * pack = (Package *) cl->parent();
      
      cl_uml->need_artifact(pack->get_imports(), pack->is_java_lang_added(),
			    pack->get_static_imports(), CurrentArtifact);
    }
#endif
    
    cl_uml->unload();
  }
  
  return TRUE;
}

bool Class::manage_extends(ClassContainer * container, 
			   const QValueList<FormalParameterList> & tmplts) {
  // out of scanning
#ifdef TRACE
  cout << name() << "->manage_extends()\n";
#endif
  
  UmlTypeSpec typespec;
  Class * cl = 0;
  QValueList<UmlTypeSpec> actuals;
  QCString str_actuals;
  QCString dummy;
  
  if (! container->read_type(typespec, &cl, tmplts, &actuals, str_actuals,
			     dummy, 0, dummy, dummy))
    return FALSE;
  
  if (typespec.type == 0) {
#ifdef TRACE
    cout << "cannot create mother\n";
#endif
    return FALSE;
  }
  
  if (cl != 0) 
    inherit(cl);
  else
    inherit(typespec.type);
  
  return add_inherit(aGeneralisation, typespec, actuals, str_actuals);
}

bool Class::manage_implements(ClassContainer * container, aRelationKind k,
			      const QValueList<FormalParameterList> & tmplts) {
  // out of scanning
#ifdef TRACE
  cout << name() << "->manage_implements()\n";
#endif
  
  for (;;) {
    UmlTypeSpec typespec;
    Class * cl = 0;
    QValueList<UmlTypeSpec> actuals;
    QCString str_actuals;
    QCString dummy;
    
    if (! container->read_type(typespec, &cl, tmplts, &actuals, str_actuals,
			       dummy, 0, dummy, dummy))
      return FALSE;
    
    if (typespec.type == 0) {
#ifdef TRACE
      cout << "cannot create mother\n";
#endif
      return FALSE;
    }
    
    if (cl != 0)
      inherit(cl);
    else
      inherit(typespec.type);
  
    if (!add_inherit(k, typespec, actuals, str_actuals))
      return FALSE;
    
    QCString s = Lex::read_word();
    
    if (s == "{") {
      Lex::unread_word(s);
      return TRUE;
    }
    
    if (s != ",") {
      Lex::error_near(s);
      return FALSE;
    }
  }
}

bool Class::add_inherit(aRelationKind k, UmlTypeSpec & typespec,
			QValueList<UmlTypeSpec> & actuals, 
			QCString & str_actuals) {
  unsigned actual_rank = 
    (typespec.explicit_type.isEmpty() && !actuals.isEmpty())
      ? uml->actuals().count()
      : 0;
    
  UmlRelation * rel =
    UmlRelation::create((actuals.isEmpty()) ? k : aRealization,
			uml, typespec.type);
  
  if (rel == 0) {
#ifdef TRACE
    cout << "cannot create <|---\n";
#endif
    return FALSE;
  }
  
#ifdef REVERSE
  Statistic::one_relation_more();
#endif

  if (!typespec.explicit_type.isEmpty())
    rel->set_JavaDecl(typespec.explicit_type);
  else if (actuals.count() > typespec.type->formals().count())
    rel->set_JavaDecl("${type}" + str_actuals);
  else {
    QValueList<UmlTypeSpec>::ConstIterator iter;
    
    for (iter = actuals.begin(); iter != actuals.end(); iter++)
      uml->replaceActual(actual_rank++, *iter);
    
    rel->set_JavaDecl("${type}");
    if (! actuals.isEmpty())
      rel->set_Stereotype("bind");
  }
  
  return TRUE;
}

// this inherits cl => it knowns cl's sub-classes
void Class::inherit(Class * cl) {
  if (! cl->reversed()) {
    QString f = cl->get_path();
    
    if (!f.isEmpty()) {
      while (!((BrowserNode *) cl->parent())->isa_package())
	cl = (Class *) cl->parent();
      
#ifdef REVERSE
      UmlArtifact * cp = CurrentArtifact;
#endif
      
      Lex::push_context();
      Package::push_context();
      ((Package *) cl->parent())->reverse_file(QCString(f));
      Lex::pop_context();
      Package::pop_context();
#ifdef REVERSE
      CurrentArtifact = cp;
#endif
    }
  }
  
  QDictIterator<Class> it(cl->Defined);
  
  while (it.current()) {
    Defined.replace(it.currentKey(), it.current());
    ++it;
  }
  
  QDictIterator<UmlClass> it_uml(cl->user_defined);
  
  while (it_uml.current()) {
    user_defined.replace(it_uml.currentKey(), it_uml.current());
    ++it_uml;
  }
}

// this inherits uml_cl => it knowns uml_cl's sub-classes
void Class::inherit(UmlClass * uml_cl, QCString header) {
  QVector<UmlItem> ch = uml_cl->children();
  UmlItem * x;
  
  for (unsigned chindex = 0; chindex != ch.size(); chindex += 1) {
    if ((x = ch[chindex])->kind() == aClass) {
      QCString s = (header.isEmpty()) ? x->name() : header + x->name();
      
      user_defined.replace(s, (UmlClass *) x);
      s += ".";
      inherit((UmlClass *) x, s);
    }
  }
}

bool Class::get_formals(FormalParameterList & tmplt, bool name_only,
			QValueList<FormalParameterList> & tmplts) {
  // '<' already read
  tmplt.clear();
  
  QCString s;
  
  do {
    QCString n = Lex::read_word();
    
    if (n == ">")
      break;
    
    if (!Lex::identifierp(n)) {
      if (! Package::scanning())
	Lex::error_near(n);
      return FALSE;
    }
    
    s = Lex::read_word();
    
    if ((s == ",") || (s == ">"))
      ;
    else if (s != "extends") {
      if (! Package::scanning())
	Lex::error_near(n);
      return FALSE;
    }
    else {
      int level = 0;
    
      if (!name_only)
	Lex::mark();
    
      for (;;) {
	s = Lex::read_word(TRUE);
	
	if (s == ",") {
	  if (level == 0)
	    break;
	}
	else if (s == ">") {
	  if (level-- == 0)
	    break;
	}
	else if (s == "]")
	  level -= 1;
	else if ((s == "<") || (s == "["))
	  level += 1;
	else if (s.isEmpty()) {
	  if (! Package::scanning())
	    Lex::premature_eof();
	  return FALSE;
	}
      }
    }
    
    UmlTypeSpec typespec;
    
    if (!name_only && !Package::scanning()) {
      QCString e = Lex::region();
      
      e.resize(e.length()); // remove , or >
      e = e.stripWhiteSpace();
      
      if (! e.isEmpty())
	compute_type(e, typespec, tmplts);
    }
    
    tmplt.append(UmlFormalParameter(n, typespec));
    
  } while (s == ",");
  
  return TRUE;
}

bool Class::manage_member(QCString s, QCString & path) {
  aVisibility visibility;
  
  switch (stereotype) {
  case 'i':
  case '@':
    visibility = PublicVisibility;
    break;
  default:
    visibility = PackageVisibility;
  }

  QCString comment = Lex::get_comments();
  QCString description = Lex::get_description();
  bool m_staticp = FALSE;
  bool m_finalp = FALSE;
  bool m_abstractp = FALSE;
  bool m_nativep = FALSE;
  bool m_strictfp = FALSE;
  bool m_synchronizedp = FALSE;
  bool m_transientp = FALSE;
  bool m_volatilep = FALSE;
  bool first_var = TRUE;
  QCString array_before_name;	// type [] var, ...;
  QCString array;
  UmlTypeSpec type;
  bool type_read = FALSE;
  QValueList<FormalParameterList> tmplts;
  QValueList<UmlTypeSpec> actuals;
  QCString str_actuals;
  QCString name;
  QCString value;
  QCString annotation;
  QCString oper_templ;
  UmlClass * first_actual_class = 0;
  QCString type_def;
  QCString genericname;
  
#ifdef TRACE
  cout << "Class::manage_member(" << s << ")\n";
#endif
  
  for (;;) {
    if (s == "public")
      visibility = PublicVisibility;
    else if (s == "protected")
      visibility = ProtectedVisibility;
    else if (s == "private")
      visibility = PrivateVisibility;
    else if (s == "static") {
      if ((s = Lex::read_word()) == "{") {
#ifdef REVERSE
	if (Package::scanning() || from_libp())
	  UmlOperation::skip_body(1);
	else {
	  Lex::mark();
	  UmlOperation::skip_body(1);
	  UmlExtraClassMember::create(get_uml(), "initialization")->
	    set_JavaDecl("  static {" + Lex::region());
	}
#else
	UmlOperation::skip_body(1);
#endif
	return TRUE;
      }
      
      m_staticp = TRUE;
      continue;
    }
    else if (s == "abstract")
      m_abstractp = TRUE;
    else if (s == "final")
      m_finalp = TRUE;
    else if (s == "native")
      m_nativep = TRUE;
    else if (s == "strictfp")
      m_strictfp = TRUE;
    else if (s == "synchronized") 
      m_synchronizedp = TRUE;
    else if (s == "transient")
      m_transientp = TRUE;
    else if (s == "volatile")
      m_volatilep = TRUE;
    else if (*s == '@'){
      if (annotation.isEmpty())
	annotation = s;
      else
	annotation += "\n" + s;
    }
    else if (s == "{") {
#ifdef REVERSE
      if (Package::scanning() || from_libp())
	UmlOperation::skip_body(1);
      else {
	Lex::mark();
	UmlOperation::skip_body(1);
	UmlExtraClassMember::create(get_uml(), "initialization")->
	  set_JavaDecl("  {" + Lex::region());
      }
#else
      UmlOperation::skip_body(1);
#endif
      return TRUE;
    }
    else 
      break;
    
    s = Lex::read_word();
  }
  
  for (;;) {
    if (s.isEmpty()) {
      if (!Package::scanning())
	Lex::premature_eof();
      return FALSE;
    }
    else if (s == "(") {
      // an operation
      if (Package::scanning()) {
	UmlOperation::skip_body(0);
	return TRUE;
      }
      
      if (!type_read) {
	  Lex::error_near("(");
#ifdef TRACE
	  cout << "ERROR '(' and type not yet read\n";
#endif
	  return FALSE;
      }
      else if (name.isEmpty()) {
	if (type.toString() == QCString(text(0))) {
	  // constructor
	  name = text(0);
	  type.type = 0;
	}
	else {
	  Lex::error_near("(");
#ifdef TRACE
	  cout << "ERROR '(' no name not constructor\n";
#endif
	  return FALSE;
	}
      }
      
      return UmlOperation::new_one(this, name, tmplts, oper_templ,
				   type, str_actuals, first_actual_class, type_def,
				   visibility, m_finalp, m_abstractp,
				   m_staticp, m_nativep, m_strictfp, m_synchronizedp,
				   array, comment, description, annotation);
    }
    else if (s == "=") {
      // initialized variable, by pass value
      Lex::mark();
      UmlOperation::skip_expr(0);
      if (Package::scanning())
	return TRUE;
      
      value = Lex::region();
      
      char c = ((const char *) value)[value.length() - 1];
      
      if ((c == ';') || (c == ',')) {
	value.truncate(value.length() - 1); // remove ';' or ','
	s = (c == ';') ? ";" : ",";
      }
      else if (((s = Lex::read_word()) != ";") && (s != ","))
	Lex::error_near(s);
      value.insert(0, "= ");
      continue;
    }
    else if ((s == ";") || (s == ",")) {
      if (Package::scanning())
	return TRUE;
      
      if (name.isEmpty() || !type_read || !oper_templ.isEmpty()) {
	Lex::error_near(s);
#ifdef TRACE
	cout << "ERROR ';' et name.isEmpty() || type not yet read\n";
#endif
	return FALSE;
      }
      
      if (type.type != 0) {
	if (!UmlRelation::new_one(this, name, type, str_actuals, visibility,
				  m_staticp, m_finalp, m_transientp, m_volatilep,
				  array, value, comment, description, annotation))
	  return FALSE;
      }
      else if (first_actual_class != 0) {
	if (!UmlRelation::new_one(this, name, first_actual_class,
				  type_def, genericname, visibility,
				  m_staticp, m_finalp, m_transientp, m_volatilep,
				  array, value, comment, description, annotation))
	  return FALSE;
      }
      else if (!UmlAttribute::new_one(this, name, type, visibility,
				      m_staticp, m_finalp, m_transientp,
				      m_volatilep, array, value, 
				      comment, description, annotation))
	return FALSE;
      
      if (s == ";")
	return TRUE;
      
      // type var1, var2 ...;
      name = 0;
      array = array_before_name;
      value = 0;
    }
    else if (Lex::identifierp(s)) {
      if ((s == "class") || (s == "enum") ||
	  (s == "interface") || (s == "@interface"))
	return reverse(this, s, annotation, m_abstractp, m_finalp,
		       visibility, path, tmplts);
      
      if (!type_read) {
	if (! ((Package::scanning()) 
	       ? bypass_type(s)
	       : read_type(type, 0, tmplts, 0, str_actuals, s, 
			   &first_actual_class, type_def, genericname)))
	  return FALSE;
	
#ifdef TRACE
	cout << "type = '" << s << "...'\n";
#endif
	type_read = TRUE;
      }
      else if (name.isEmpty()) {
	name = s;
	if (first_var) {
	  first_var = FALSE;
	  array_before_name = array;
	}
#ifdef TRACE
	cout << "name = '" << name << "'\n";
#endif
      }
      else {
	if (! Package::scanning())
	  Lex::error_near(s);
#ifdef TRACE
	cout << "ERROR '" << s << "' alors qu a deja le type et le nom '" << name << "'\n";
#endif
	return FALSE;
      }
    }
    else if (*((const char *) s) == '[')
      array += s;
    else if (*((const char *) s) == '<') {
      // <...> behind an operation
      FormalParameterList tmplt;
      
      tmplts.append(tmplt);
      Lex::mark();
      if (!get_formals(tmplts.last(), TRUE, tmplts))
	return FALSE;
      oper_templ = "<" + Lex::region();
    }
    else {
      if (! Package::scanning())
	Lex::error_near(s);
#ifdef TRACE
      cout << "ERROR : '" << s << "'\n";
#endif
      UmlOperation::skip_expr(*((const char *) s) == '{');
      return TRUE;
    }
    
    s = Lex::read_word();
  }
}

bool Class::manage_enum_items() {
  UmlClass * cl_uml = (Package::scanning()) ? 0 : get_uml();
  QCString s;
  
  for (;;) {
    if ((s = Lex::read_word()).isEmpty()) {
      if (! Package::scanning())
	Lex::premature_eof();
      return FALSE;
    }
    else if (s == ";")
      return TRUE;
    else if (s == "}") {
      Lex::unread_word("}");
      return TRUE;
    }
    else if (! Lex::identifierp(s)) {
      if (! Package::scanning())
	Lex::error_near(s);
      return FALSE;
    }
    else if (! UmlAttribute::manage_enum_item(s, cl_uml))
      return FALSE;
  }
}

Class * Class::define(const QCString & name, char st) {
  Class * cl = Defined[name];
  
  if (!Package::scanning() || (cl != 0))
    return cl;
  
  cl = new Class(this, name, st);
  
  declare(name, cl);
  
  return cl;
}

void Class::compute_type(QCString name, UmlTypeSpec & typespec,
			 const QValueList<FormalParameterList> & tmplts,
			 Class ** need_object) {
  const QValueList<UmlFormalParameter> formals = get_uml()->formals();
  QValueList<UmlFormalParameter>::ConstIterator it;
  
  for (it = formals.begin(); it != formals.end(); it++) {
    if ((*it).name() == name) {
      // a formal
      if (need_object == 0) {
	typespec.type = 0;
	typespec.explicit_type = name;
	
	return;
      }
      
      BrowserNode * p = (BrowserNode *) this;
      
      do {
	p = (BrowserNode *) parent();
      } while (! p->isa_package());
      
      ((Package *) p)->force_class(name, typespec, need_object);
      return;
    }
  }
  
  QCString s = name;
  Class * cl = this;
  
  for (;;) {
    Class * cl2 = cl->Defined[s];
    
    if (cl2 != 0) {
      // a sub class
      typespec.type = cl2->get_uml();
      typespec.explicit_type = 0;
      
      if (need_object != 0)
	*need_object = cl2;
      
      return;
    }
    
    UmlClass * uml_cl2 = cl->user_defined[s];
    
    if (uml_cl2 != 0) {
      // a sub class
      typespec.type = uml_cl2;
      typespec.explicit_type = 0;
      
      return;
    }
    
    int index = s.find('.');
  
    if ((index != -1) && (s.left(index) == (const char *) cl->text(0)))
      s = s.mid(index + 1);
    else
      break;
  }

  // not defined in the current class scope, goes up
  BrowserNode * p = (BrowserNode *) parent();
  
  if (! p->isa_package())
    ((Class *) p)->compute_type(name, typespec, tmplts, need_object);
  else
    ((Package *) p)->compute_type(name, typespec, tmplts, need_object);
}

void Class::declare(const QCString & name, Class * cl) {
  Defined.insert(name, cl);
  
  QCString s = QCString(text(0)) + '.' + name;
  ((BrowserNode *) parent())->declare(s, cl);
}

bool Class::isa_package() const {
  return FALSE;
}

QString Class::get_path() const {
  return filename;
}

#ifndef REVERSE

const QPixmap* Class::pixmap(int) const {
  return ((stereotype == 'i') || (stereotype == '@'))
    ? InterfaceIcon : ClassIcon;
}
 
void Class::paintCell(QPainter * p, const QColorGroup & cg, int column,
		      int width, int alignment) {
  if (abstractp) {
    QFont fnt = p->font();
    
    fnt.setItalic(TRUE);
    p->setFont(fnt);
    BrowserNode::paintCell(p, cg, column, width, alignment);
    fnt.setItalic(FALSE);
    p->setFont(fnt);
  }
  else
    BrowserNode::paintCell(p, cg, column, width, alignment);
}

// called when the user click on the node
 
void Class::activate() {
  manage_historic();
  // Qt will call activated()
}

// called when the node is selected through comment reference

void Class::selected() {
  manage_historic();
  activated();
}

void Class::manage_historic() {
  // manage historic
  while (Historic.getLast() != Historic.current())
    Historic.removeLast();
  
  Historic.append(this);
}

// called when the node is activated for any reason
// for instance because the user clink on it, or
// select it using keyboard arrows
 
void Class::activated() {
  JavaCatWindow::clear_trace();

  if (! description_updatedp) {
    // modify description, made only now to take into account
    // future modifications on old backup
    
    const char * p = description;
    
    if (p == 0)
      return;
    
    while (*p == '\n')
      p += 1;
    
    QString s = p;
    
    if (s.left(3) != "<p>")
      s.insert(0, "<p>");
    
    int index = 0;
    
    while ((index = s.find("\n@", index)) != -1) {
      if (s[index + 2].isLetter()) {
	int index2 = index;
	
	s.replace(index + 1, 1, "<p><b>");
	
	for (index += 7; !s[index].isSpace(); index += 1)
	  ;
	s.insert(index, "</b>");
	
	if ((index == index2 + 10) && (s.mid(index2 + 7, 3) == "see")) {
	  for (index2 = index + 4; s[index2].isSpace(); index2 += 1)
	    ;
	  s.insert(index2, "{@link ");
	  for (index2 += 7; !s[index2].isSpace(); index2 += 1)
	    ;
	  s.insert(index2, '}');
	  index = index2 + 1;
	}
      }
      else
	s.replace(index + 1, 1, "<p>");
    }
    
    index = 0;
    
    while ((index = s.find("{@link ", index)) != -1) {
      int index2 = s.find("}", index + 7);
      
      if (index2 == -1)
	index += 7;
      else {
	int index3 = index2;
	
	while (!s[--index3].isSpace())
	  ;
	if (index3 == index + 6) {
	  if (s[index3 + 1] == QChar('#')) {
	    // does not have class sub artifacts
	    // {@link #X} => X
	    s.remove(index2, 1);
	    s.remove(index, 8);
	  }
	  else {
	    // {@link X} => <a href="X">X</a>
	    QString X = s.mid(index + 7, index2 - index - 7);
	    int index4 = X.find('#');
	    
	    if (index4 != -1) {
	      // does not have class sub artifacts
	      // {@link X#Y} => <a href="X">X</a>.Y
	      X.truncate(index4);
	      s.replace(s.find('#', index), 1, "\">" + X + "</a>.");
	      s.replace(index, 7, "<a href=\"");
	    }
	    else {
	      s.replace(index2, 1, "\">" + X + "</a>");
	      s.replace(index, 7, "<a href=\"");
	    }
	  }
	}
	else {
	  QString X = s.mid(index + 7, index3 - index - 7);
	  int index4 = X.find('#');
	  
	  if (index4 != -1) {
	    // does not have class sub artifacts
	    // {@link X#Y Y} => <a href="X">X</a>.Y
	    X.truncate(index4);
	    s.remove(index3, index2 - index3 + 1);
	    s.replace(s.find('#', index), 1, "\">" + X + "</a>.");
	    s.replace(index, 7, "<a href=\"");
	  }
	  else {
	    // {@link X Y} => <a href="X">Y</a>
	    s.replace(index2, 1, "</a>");
	    s.replace(index3, 1, "\">");
	    s.replace(index, 7, "<a href=\"");
	  }
	}
      }
    }
    
    description = s;
    description_updatedp = TRUE;
  }
  
  JavaCatWindow::trace(description);
}

void Class::historic_back() {
  if (Historic.current() != Historic.getFirst())
    BrowserView::select(Historic.prev());
}

void Class::historic_forward() {
  if (Historic.current() != Historic.getLast())
    BrowserView::select(Historic.next());
}

void Class::menu() {
  QPopupMenu m(0);
  
  m.insertItem(text(0), -1);
  m.insertSeparator();
  
  if (! reversed())
    m.insertItem("Send it", 0);
  m.insertItem("Show file", 1);
  
  switch (m.exec(QCursor::pos())) {
  case 0:
    {  
      Class * cl = this;
      BrowserNode * b;
      
      while (!(b = (BrowserNode *) cl->parent())->isa_package())
	cl = (Class *) b;
      
      JavaSettings::set_UseDefaults(TRUE);
      UmlCom::message("reverse " + cl->filename);
      JavaCatWindow::clear_trace();
      ((Package *) b)->reverse_file(filename);
      UmlCom::message("");
    }
    break;
  case 1:
    (new ShowFileDialog(filename))->show();
    break;
  }
}

void Class::refer(const QString & href) {
  ((BrowserNode *) parent())->refer(href);
}

// note : 'QDataStream >> char *' bugged on windows

void Class::backup(QDataStream  & dt) const {
  switch (stereotype) {
  case 'i':
    dt << ((Q_INT8) ((abstractp) ? 'i' : 'I'));
    break;
  case 'e':
    dt << ((Q_INT8) 'E');
    break;
  default:
    // class
    dt << ((Q_INT8) ((abstractp) ? 'c' : 'C'));
    break;
  }
  
  Q_INT32 len;
  
  len = text(0).length();
  dt << len;
  dt.writeRawBytes(text(0), len);
  
  len = filename.length();
  dt << len;
  dt.writeRawBytes(filename, len);
  
  len = description.length();
  dt << len;
  if (!description.isEmpty())
    dt.writeRawBytes(description, len);
  
  // sub classes
  
  QListViewItem * child;
    
  for (child = firstChild(); child != 0; child = child->nextSibling())
    ((BrowserNode *) child)->backup(dt);
    
  dt << ((Q_INT8) '@');
}

#endif // REVERSE

void Class::restore(QDataStream  & dt, char c, BrowserNode * parent)
{
  Q_INT32 len;
  
  dt >> len;
  char * n = new char[len + 1];
  dt.readRawBytes(n, len);
  n[len] = 0;
  
  dt >> len;
  char * fn = new char[len + 1];
  dt.readRawBytes(fn, len);
  fn[len] = 0;
  
  dt >> len;
  char * d;
  
  if (len == 0)
    d = 0;
  else {
    d = new char[len + 1];
    dt.readRawBytes(d, len);
    d[len] = 0;
  }

  Class * cl = new Class(parent, n, ((c == 'c') || (c == 'i')) ? c : (c - 'A' + 'a'));
  
#ifdef REVERSE
  cl->from_lib = TRUE;
#endif
  
  QCString name(n);
  
  parent->declare(n, cl);
  
  if ((c == 'i') || (c == 'c'))
    cl->abstractp = TRUE;
  
  cl->filename = fn;
#ifndef REVERSE
  cl->description = d;
#endif
  
  delete [] n;
  delete [] fn;
  if (d) delete [] d;
  
  // sub classes
  
  Q_INT8 k;
  
  while (dt >> k, k != '@')
    restore(dt, (char) k, cl);
}
