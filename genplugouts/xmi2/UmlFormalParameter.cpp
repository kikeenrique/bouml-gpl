
#include "UmlFormalParameter.h"
#include "FileOut.h"
#include "UmlClass.h"

void UmlFormalParameter::write(FileOut & out, const UmlClass * cl, int rank) const {
  char tp[32];
  char te[32];

  sprintf(tp, "TEMPLPARAM%d_", rank);
  sprintf(te, "TEMPLELEM%d_", rank);
  
  out.indent();
  out << "<parameter";
  out.idref_prefix(cl, tp);
  out << "/>\n";
  out.indent();
  out << "<ownedParameter xmi:type=\"uml:ClassifierTemplateParameter\"";
  out.id_prefix(cl, tp);
  out << ">\n";
  out.indent(+1);

  out.indent();
  out << "<ownedParameteredElement xmi:type=\"uml:Class\"";
  out.id_prefix(cl, te);
  out << " name=\"";
  out.quote(name());
  out << '"';
  out.ref(cl, "templateParameter", tp);
  out << "/>\n";

  if (defaultValue().type != 0)
    UmlItem::write_default_value(out, defaultValue().type->name());
  else
    UmlItem::write_default_value(out, defaultValue().explicit_type);

  out.indent(-1);
  out.indent();
  out << "</ownedParameter>\n";
}

