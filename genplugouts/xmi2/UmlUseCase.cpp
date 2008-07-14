
#include "UmlUseCase.h"
#include "FileOut.h"

void UmlUseCase::write(FileOut & out) {
  const char * k = (parent()->kind() == anUseCase)
    ? "ownedUseCase"
    : ((_uml_20) ? "ownedMember" : "packagedElement");
  
  out.indent(); 
  out << "<" << k << " xmi:type=\"uml:UseCase\"";
  out.id(this); 
  out << " name=\"";
  out.quote(name());
  out << "\">\n";
  out.indent(+1); 
  write_description_properties(out);
  
  QCString eps = extensionPoints().stripWhiteSpace();
  
  if (! eps.isEmpty()) {
    int index0 = 0;
    char s[16];
    int rank = 0;
    
    strcpy(s, "EXTPOINT");
    
    for (;;) {
      int index1 = eps.find('\n', index0);
      QCString ep = eps.mid(index0, index1);
      
      if (! ep.isEmpty()) {
	out.indent();
	out << "<extensionPoint xmi:type=\"uml:ExtensionPoint\"";
	sprintf(s+8, "%d_", ++rank);
	out.id_prefix(this, s);
	out << " name=\"";
	out.quote(ep);
	out << "\">\n";
      }
      
      if (index1 == -1)
	break;
      else
	index0 = index1 + 1;
    }
  }
     
  const QVector<UmlItem> ch = children(); 
  unsigned n = ch.size(); 
     
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->write(out); 
       
  out.indent(-1); 
  out.indent(); 
  out << "</" << k << ">\n"; 
 
  unload(); 
}

