
#include "UmlActualParameter.h"
#include "FileOut.h"
#include "UmlClass.h"

void UmlActualParameter::write(FileOut & out, const UmlClass * cl, int actualrank) const {
  char ps[32];
  char tp[32];

  sprintf(ps, "ACTUALPARAM%d_", actualrank);
  sprintf(tp, "TEMPLPARAM%d_", rank());

  out.indent();
  out << "<parameterSubstitution xmi:type=\"TemplateParameterSubstitution\"";
  out.id_prefix(cl, ps);
  out << ">\n";
  out.indent();
  out << "\t<formal";
  out.idref_prefix(superClass(), tp);
  out << "/>\n";

  // how to produce actual ????
  out.indent();
  out << "\t<!-- how to indicate the actual is ";
  out.quote((value().type != 0) ? value().type->name() : value().explicit_type);
  out << " ? -->\n";

  out.indent();
  out << "</parameterSubstitution>\n";
}

