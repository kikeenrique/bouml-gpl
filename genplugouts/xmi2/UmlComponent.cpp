
#include "UmlComponent.h"
#include "FileOut.h"
#include "UmlClass.h"
#include "UmlItem.h"

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
    
  static UmlItem * prov_rank = 0;
  static UmlItem * req_rank = 0;
  static UmlItem * real_rank = 0;
  
  write(out, providedClasses(), prov_rank, "provided", "PROVIDED_");
  write(out, requiredClasses(), req_rank, "required", "REQUIRED_");
  write(out, realizedClasses(), real_rank, "realization", "REALIZATION_");

  out.indent(-1); 
  out.indent(); 
  out << "</" << k << ">\n"; 
 
  unload(); 
}

void UmlComponent::write(FileOut & out, const QVector< UmlClass > & v, UmlItem *& rank, const char * k1, const char * k2) {
  unsigned n = v.size();
  unsigned index;
  
  for (index = 0; index != n; index += 1) {
    out.indent();
    out << "<" << k1;
    out.id_prefix(++rank, k2);
    
    UmlClass * cl = v[index];
    
    out.idref(cl);
    out << " name=\"";
    out.quote(cl->name());
    out << "\"/>\n";
  }
}

