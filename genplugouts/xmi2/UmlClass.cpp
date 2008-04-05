
#include "UmlClass.h"
#include "FileOut.h"

#include "UmlRelation.h"
#include "UmlCom.h"
void UmlClass::write(FileOut & out) {
  QCString st = stereotype();
  bool is_actor = (st == "actor");
  bool is_enum = (st == "enum");
  bool is_stereotype =
    (st == "stereotype") && 
    (parent()->parent()->kind() == aPackage) &&
    (parent()->parent()->stereotype() == "profile");
  
  if (!is_actor) {
    switch (_lang) {
    case Cpp:
      if (cppDecl().isEmpty())
	return;
      break;
    case Java:
      if (javaDecl().isEmpty())
	return;
    default:
      break;
    }
  }

  const char * k = (parent()->kind() == aClass)
    ? "nestedClassifier"
    : ((!_uml_20)
       ? "packagedElement"
       : ((is_stereotype) ? "ownedStereotype" : "ownedMember"));
  bool is_assoc_class = (_assoc != 0);
  
  out.indent();
  out << "<" << k << " xmi:type=\"uml:"
    << ((is_actor) 
	? "Actor"
	: ((is_assoc_class)
	   ? "AssociationClass"
	   : ((st == "interface")
	      ? "Interface" 
	      : ((is_enum)
		 ?"Enumeration"
		 : ((is_stereotype) ? "Stereotype" : "Class")))))
    << "\" name=\"";
  out.quote(name());
  out << '"';
  out.id(this);
  write_visibility(out);
  out << " isAbstract=\"" << ((isAbstract()) ? "true" : "false") << "\" >\n";
  
  if (is_assoc_class)
    _assoc->write_ends(out);
    
  out.indent(+1);
  
  write_annotation(out);
  write_description_properties(out);
  
  write_formals(out);
  write_actuals(out);
  
  QCString extending;
  QCString extname;
  
  if (is_stereotype) {
    propertyValue("stereotypeExtension", extending);
    if (extending.isEmpty())
      extending = (_uml_20) ? "http://schema.omg.org/spec/UML/2.0/uml.xml#Element"
			    : "http://schema.omg.org/spec/UML/2.1/uml.xml#Element";
    extname = extending.mid(extending.findRev('#') + 1);
    
    out.indent();
    out << "<ownedAttribute xmi:type=\"uml:Property\" name=\"base_";
    out.quote(extname);
    out << '"';
    out.id_prefix(this, "BASE_");
    out.ref(this, "association", "EXT_");
    out << ">\n";
    out.indent();
#ifndef WIN32
#warning have to add management of stereotype/class extension
#endif
    out << "\t<type xmi:type=\"uml:Class\" href=\"" << extending << "\"/>\n";
    out.indent();
    out << "</ownedAttribute>\n";
  }

  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->write(out);
  
  out.indent(-1);
  out.indent();
  out << "</" << k << ">\n";
  
  if (is_stereotype) {
    k = (_uml_20) ? "ownedMember" : "packagedElement";
    
    out.indent();
    out << "<" << k << " xmi:type=\"uml:Extension\" name=\"A_";
    out.quote(extname);
    out  << '_';
    out.quote(name());
    out << '"';
    out.id_prefix(this, "EXT_");
    out.ref(this, "memberEnd", "BASE_");
    out << ">\n";
    out.indent();
    out << "\t<ownedEnd xmi:type=\"uml:ExtensionEnd\" name=\"extension_";
    out.quote(name());
    out << '"';
    out.id_prefix(this, "EXTEND_");
    out.ref(this, "type");
    out << " aggregation=\"composite\"/>\n";
    out.indent();
    out << "</" << k << ">\n";
  }
  
  unload();
}

void UmlClass::write_formals(FileOut & out) {
  QValueList<UmlFormalParameter> formal_params = formals();
    
  if (!formal_params.isEmpty()) {
    out.indent();
    out << "<ownedTemplateSignature xmi:type=\"uml:";
    if (!_uml_20)
      out << "Redefinable";
    out << "TemplateSignature\"";
    out.id_prefix(this, "FORMALS_");
    out << ">\n";
    out.indent(+1);

    int rank;
    QValueList<UmlFormalParameter>::ConstIterator iter;

    for (iter = formal_params.begin(), rank = 0;
	 iter != formal_params.end();
	 ++iter, rank += 1)
      (*iter).write(out, this, rank, _uml_20);

    out.indent(-1);
    out.indent();
    out << "</ownedTemplateSignature>\n";
  }
}

void UmlClass::write_actuals(FileOut & out) {
  QValueList<UmlActualParameter> actual_params = actuals();
  QValueList<UmlActualParameter>::ConstIterator iter;
  int rank;
  UmlItem * idtb = 0;
  UmlClass * super = 0;

  for (iter = actual_params.begin(), rank = 0;
       iter != actual_params.end();
       ++iter, rank += 1) {
    if (super != (*iter).superClass()) {
      if (super != 0) {
	out.indent(-1);
	out.indent();
	out << "</templateBinding>\n";
      }
      
      super = (*iter).superClass();
      
      out.indent();
      out << "<templateBinding xmi:type=\"uml:TemplateBinding\"";
      out.id_prefix(++idtb, "ACTUALS_");
      out << ">\n";
      out.indent(+1);
      
      out.indent();
      out << "<boundElement";
      out.idref(this);
      out << " />\n";
      
      out.indent();
      out << "<signature";
      out.idref_prefix(super, "FORMALS_");
      out << " />\n";
    }
      
    (*iter).write(out, this, rank);
  }

  if (super != 0) {
    out.indent(-1);
    out.indent();
    out << "</templateBinding>\n";
  }

}

void UmlClass::search_class_assoc() {
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->search_class_assoc();
}

UmlClass * UmlClass::set_assoc(UmlRelation * rel) {
  if (_assoc == 0) {
    _assoc = rel;
    return this;
  }
  else {
    QCString msg = "warning : class '" +  name() +
      "' is an association class associated with several relations<br>";
    
    UmlCom::trace(msg);
    return 0;
  }
}

