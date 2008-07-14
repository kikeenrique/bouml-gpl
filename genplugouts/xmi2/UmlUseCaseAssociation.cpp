
#include "UmlUseCaseAssociation.h"
#include "FileOut.h"
#include "UmlItem.h"

#include "UmlClass.h"
#include "UmlUseCase.h"
#include "UmlUseCaseReference.h"
void UmlUseCaseAssociation::write(FileOut & out, UmlItem * d, int rank, const char * k) {
  char s[16];
  
  out.indent();
  out << '<' << k << " xmi:type=\"uml:Association\"";
  sprintf(s, "UCAC%d_", rank);
  out.id_prefix(d, s);
  if (! name().isEmpty()){
    out << " name=\"";
    out.quote(name());
    out << "\">\n";
  }
  else
    out << ">\n";
  out.indent(+1); 
  
  out.indent();
  out << "<ownedEnd xmi:type=\"uml:Property\"";
  out.id_prefix(d, s + 2);
  out.ref(actor(), "type");
  out.ref(d, "association", s);
  out << "/>\n";
  
  out.indent();
  out << "<ownedEnd xmi:type=\"uml:Property\"";
  s[2] = 'U';
  out.id_prefix(d, s + 2);
  s[2] = 'A';
  out.ref(useCase()->useCase(), "type");
  out.ref(d, "association", s);
  out << "/>\n";

  out.indent(-1);
  out.indent();
  out << "</" << k << ">\n";
}

