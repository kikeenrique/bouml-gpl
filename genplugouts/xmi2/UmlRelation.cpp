
#include "UmlRelation.h"
#include "FileOut.h"

#include "UmlClass.h"
void UmlRelation::write(FileOut & out) {
  write(out, TRUE);
}

void UmlRelation::write(FileOut & out, bool inside) {
  switch (relationKind()) {
  case aGeneralisation:
    if (inside)
      write_generalization(out);
    break;
  case aRealization:
    if (inside)
      parent()->memo_relation(this);
    else
      write_realization(out);
    break;
  case aDependency:
    if (inside)
      parent()->memo_relation(this);
    else
      write_dependency(out);
    break;
  default:
    // don't generate them for actors
    {
      UmlItem * p = parent();
      
      if (p->stereotype() == "actor")
	return;
      do {
	p = p->parent();
      } while (p->kind() == aClass);
      if (p->kind() == aClassView)
	if (inside)
	  write_relation_as_attribute(out);
	else
      // note : it is the first side
	  write_relation(out);
    }
    break;
  }
}

void UmlRelation::write_generalization(FileOut & out) {
  out.indent();
  out << "<generalization xmi:type=\"uml:Generalization\"";
  out.id(this);
  out.ref(roleType(), "general");
  out << "/>\n";
}

void UmlRelation::write_dependency(FileOut & out) {
  const char * k = (_uml_20) ? "ownedElement" : "packagedElement";

  out.indent();
  out << '<' << k << " xmi:type=\"uml:Dependency\"";
  out.id(this);
  out.ref(parent(), "client");
  out.ref(roleType(), "supplier");
  out << ">\n";
  out.indent(+1); 
  write_description_properties(out);
  out.indent(-1);
  out.indent();
  out << "</" << k << ">\n";
}

void UmlRelation::write_realization(FileOut & out) {
  const char * k = (_uml_20) ? "ownedElement" : "packagedElement";

  out.indent();
  out << '<' << k << " xmi:type=\"uml:Realization\"";
  out.id(this);
  out.ref(parent(), "client");
  out.ref(roleType(), "supplier");
  out.ref(roleType(), "realizingClassifier");
  out << ">\n";
  out.indent(+1); 
  write_description_properties(out);
  out.indent(-1);
  out.indent();
  out << "</" << k << ">\n";
}

void UmlRelation::write_relation(FileOut & out) {
  // note : it is the first side
 
  const char * k = (_uml_20) ? "ownedElement" : "packagedElement";

  out.indent();
  out << '<' << k << " xmi:type=\"uml:Association\"";
  out.id_prefix(this, "ASSOC_");
  
  QCString s = name();
  int i1 = s.find("(");
  int i2 = s.findRev(")");
  
  if ((i1 != -1) && (i2 != -1) && (i2 > i1) && (s[i1+1] != '<')  && (s[i2-1] != '>')) {
    s = s.mid(i1 + 1, i2 - i1 - 1);
    
    if (!s.isEmpty()) {
      out << " name=\"";
      out.quote(s);
      out << '"';
    }
  }
  write_visibility(out);
  out << ">\n";
  
  out.indent();
  out << "\t<memberEnd";
  out.idref(this);
  out << "/>\n";
  
  UmlRelation * other = side(FALSE);
  
  out.indent();
  if (other != 0) {
    out << "\t<memberEnd";
    out.idref(other);
    out << "/>\n";
  }
  else {
    out << "\t<ownedEnd xmi:type=\"uml:Property\"";
    out.id_prefix(this, "REVERSE_");
    out.ref(this, "association", "ASSOC_");
    out << " visibility=\"" << ((_vis_prefix) ? "vis_private\"" : "private\"");
    out.ref(parent(), "type");
    out << " aggregation=\"";
    switch (relationKind()) {
    case anAggregation:
    case aDirectionalAggregation:
      out << "shared";
      break;
    case anAggregationByValue:
    case aDirectionalAggregationByValue:
      out << "composite";
      break;
    default:
      out << "none";
    }
    out << "\" isNavigable=\"false\"/>\n";

    out.indent();
    out << "\t<memberEnd ";
    out.idref_prefix(this, "REVERSE_");
    out << "/>\n";
  }
  
  out.indent();
  out << "</" << k << ">\n";

  UmlTypeSpec assoc = association();
  
  if (assoc.type != 0) {
    out.indent();
    out << '<' << k << " xmi:type=\"uml:AssociationClass\"";
    out.id_prefix(this, "ASSOC_CLASS_");
    out << ">\n";
    
    out.indent();
    out << "\t<memberEnd ";
    out.idref_prefix(this, "ASSOC_");
    out << "/>\n";
    
    out.indent();
    out << "\t<memberEnd ";
    out.idref(assoc.type);
    out << "/>\n";
    
    out.indent();
    out << "</" << k << ">\n";
  }
}

void UmlRelation::write_relation_as_attribute(FileOut & out) {
  QCString s;

  switch (_lang) {
  case Uml:
	s = roleName();
    break;
  case Cpp:
    if (cppDecl().isEmpty())
      return;
    s = true_name(roleName(), cppDecl());
    break;
  default: // Java
    if (javaDecl().isEmpty())
      return;
    s = true_name(roleName(), javaDecl());
    break;
  }
  out.indent();
  out << "<ownedAttribute xmi:type=\"uml:Property\" name=\"" << s << '"';
  out.id(this);

  write_visibility(out);
  write_scope(out);
  if (isReadOnly())
    out << " isReadOnly=\"true\"";
  
  UmlRelation * first = side(TRUE);

  out.ref(first, "association", "ASSOC_");

  out << " aggregation=\"";
  if (this == first) {
    parent()->memo_relation(this);
    out << "none";
  }
  else {
    switch (relationKind()) {
    case anAggregation:
    case aDirectionalAggregation:
      out << "shared";
      break;
    case anAggregationByValue:
    case aDirectionalAggregationByValue:
      out << "composite";
      break;
    default:
      out << "none";
    }
  }

  out << "\">\n";
  out.indent(+1);
  
  out.indent();
  out << "<type";
  out.idref(roleType());
  out << "/>\n";
  write_multiplicity(out, multiplicity());
  write_default_value(out, defaultValue());
  write_annotation(out);
  write_description_properties(out);

  out.indent(-1);
  out.indent();
  out << "</ownedAttribute>\n";

  unload();
}

