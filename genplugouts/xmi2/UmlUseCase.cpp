
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
     
  const QVector<UmlItem> ch = children(); 
  unsigned n = ch.size(); 
     
  for (unsigned i = 0; i != n; i += 1) 
    ch[i]->write(out); 
       
  out.indent(-1); 
  out.indent(); 
  out << "</" << k << ">\n"; 
 
  unload(); 
}

