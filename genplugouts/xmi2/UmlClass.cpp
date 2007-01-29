
#include "UmlClass.h"
#include "FileOut.h"

#include "UmlRelation.h"
void UmlClass::write(FileOut & out) {
  QCString st = stereotype();
  bool is_actor = (stereotype() == "actor");
  
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
    : ((_uml_20) ? "ownedMember" : "packagedElement");
  
  out.indent();
  out << "<" << k << " xmi:type=\"uml:"
    << ((is_actor) ? "Actor" : ((st == "interface") ? "Interface" : "Class"))
      << "\" name=\"";
  out.quote(name());
  out << '"';
  out.id(this);
  write_visibility(out);
  out << " isAbstract=\""
    << ((isAbstract()) ? "true" : "false")
      << "\" >\n";
  out.indent(+1);
  write_annotation(out);
  write_description_properties(out);
  
  write_formals(out);
  write_actuals(out);
  
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->write(out);
  
  out.indent(-1);
  out.indent();
  out << "</" << k << ">\n";
  
  unload();
}

void UmlClass::write_formals(FileOut & out) {
  QValueList<UmlFormalParameter> formal_params = formals();
    
  if (!formal_params.isEmpty()) {
    out.indent();
    out << "<ownedTemplateSignature xmi:type=\"uml:TemplateSignature\"";
    out.id_prefix(this, "FORMALS_");
    out << ">\n";
    out.indent(+1);

    int rank;
    QValueList<UmlFormalParameter>::ConstIterator iter;

    for (iter = formal_params.begin(), rank = 0;
	 iter != formal_params.end();
	 ++iter, rank += 1)
      (*iter).write(out, this, rank);

    out.indent(-1);
    out.indent();
    out << "</ownedTemplateSignature>\n";
  }
}

void UmlClass::write_actuals(FileOut & out) {
  QValueList<UmlActualParameter> actual_params = actuals();

  if (! actual_params.isEmpty()) {
    out.indent();
    out << "<templateBinding xmi:type=\"uml:TemplateBinding\"";
    out.id_prefix(this, "ACTUALS_");
    out << ">\n";
    out.indent(+1);

    int rank;
    QValueList<UmlActualParameter>::ConstIterator iter;

    for (iter = actual_params.begin(), rank = 0;
	 iter != actual_params.end();
	 ++iter, rank += 1)
      (*iter).write(out, this, rank);

    out.indent(-1);
    out.indent();
    out << "</templateBinding>\n";
  }
}

