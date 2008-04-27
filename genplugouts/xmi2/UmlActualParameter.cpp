
#include "UmlActualParameter.h"
#include "FileOut.h"
#include "UmlClass.h"

void UmlActualParameter::write(FileOut & out, UmlClass * cl, int actualrank) const {
  char ps[32];
  char tp[32];

  sprintf(ps, "ACTUALPARAM%d_", actualrank);
  sprintf(tp, "TEMPLPARAM%d_", rank());

  out.indent();
  out << "<parameterSubstitution xmi:type=\"uml:TemplateParameterSubstitution\"";
  out.id_prefix(cl, ps);
  out << ">\n";
  out.indent();
  out << "\t<formal";
  out.idref_prefix(superClass(), tp);
  out << "/>\n";

  out.indent();
  if (value().type != 0) {
    out << "\t<actual";
    out.idref(value().type);
    out << "/>\n";
  }
  else {
    // produce an opaque expression, even in case of primitive type
    char oa[32];
    
    sprintf(oa, "OPAQUE_ACTUAL%d_", actualrank);
    
    out << "\t<actual";
    out.idref_prefix(cl, oa);
    out << "/>\n";
    
    out.indent();
    out << "\t<ownedActual xmi:type=\"uml:OpaqueExpression\"";
    out.id_prefix(cl, oa);
    out << " body=\"";
    out.quote(value().explicit_type);
    out << "\"/>\n";
  }

  out.indent();
  out << "</parameterSubstitution>\n";
}

