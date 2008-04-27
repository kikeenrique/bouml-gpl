
#include "UmlComponent.h"
#include "FileOut.h"
#include "UmlClass.h"

void UmlComponent::write(FileOut & out) {
  const char * k = (parent()->kind() == anUseCase)
    ? "ownedUseCase"
    : ((_uml_20) ? "ownedMember" : "packagedElement");
  
  out.indent(); 
  out << "<" << k << " xmi:type=\"uml:Component\"";
  out.id(this); 
  out << " name=\"";
  out.quote(name());
  out << "\">\n";
  out.indent(+1); 
  write_description_properties(out); 
     
  const QVector<UmlItem> ch = children(); 
  unsigned n = ch.size(); 
  unsigned index;
     
  for (index = 0; index != n; index += 1) 
    ch[index]->write(out); 
    
  write(out, providedClasses(), "provided", "PROVIDED");
  write(out, requiredClasses(), "required", "REQUIRED");
  write(out, realizingClasses(), "realization", "REALIZATION");

  out.indent(-1); 
  out.indent(); 
  out << "</" << k << ">\n"; 
 
  unload(); 
}

void UmlComponent::write(FileOut & out, const QVector< UmlClass > & v, const char * k1, const char * k2) {
  unsigned n = v.size();
  unsigned index;
  char s[64];
  
  strcpy(s, k2);
  
  char * p = s + strlen(s);
  
  for (index = 0; index != n; index += 1) {
    sprintf(p, "%u_", index);
    
    out.indent();
    out << "<" << k1;
    out.id_prefix(this, s);
    
    UmlClass * cl = v[index];
    
    out.idref(cl);
    out << " name=\"";
    out.quote(cl->name());
    out << "\"/>\n";
  }
}

