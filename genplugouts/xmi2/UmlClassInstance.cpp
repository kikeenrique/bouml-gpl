
#include "UmlClassInstance.h"
#include "FileOut.h"

#include "UmlClass.h"
#include "UmlAttribute.h"
#include "UmlRelation.h"
void UmlClassInstance::write(FileOut & out) {
  const char * k = (_uml_20) ? "ownedMember" : "packagedElement";

  out.indent(); 
  out << "<" << k << " xmi:type=\"uml:InstanceSpecification\"";
  out.id(this); 
  out << " name=\"";
  out.quote(name());
  out << "\">\n"; 
  out.indent(+1); 
  
  out.indent();
  out << "<classifier ";
  out.idref(type());
  out << "/>\n"; 
    
  QValueList<SlotAttribute> attrs;
  QValueList<SlotAttribute>::Iterator a_iter;
  char sslot[32];
  char svalue[32];
  unsigned rank;
  
  attributesValue(attrs);
  for (a_iter = attrs.begin(), rank = 0;
       a_iter != attrs.end();
       ++a_iter, rank += 1) {
    SlotAttribute & slot = *a_iter;
    
    sprintf(sslot, "ASLOT%u_", rank);
    sprintf(svalue, "ASLOT_VALUE%u_", rank);
    
    out.indent();
    out << "<slot";
    out.ref(slot.attribute, "definingFeature");
    out.id_prefix(this, sslot);
    out << " xmi:type=\"uml:Slot\">\n";

    out.indent();
    out << "\t<value xmi:type=\"uml:LiteralString\"";
    out.id_prefix(this, svalue);
    out << " value=\"";
    out.quote(slot.value);
    out << "\"/>\n";
    
    out.indent();
    out << "</slot>\n";
  }

  QValueList<SlotRelation> rels;
  QValueList<SlotRelation>::Iterator r_iter;
  
  relationsValue(rels);
  for (r_iter = rels.begin(), rank = 0;
       r_iter != rels.end();
       ++r_iter, rank += 1) {
    SlotRelation & slot = *r_iter;
    
    sprintf(sslot, "RSLOT%u_", rank);
    
    out.indent();
    out << "<slot";
    out.ref(slot.relation, "definingFeature");
    out.id_prefix(this, sslot);
    out << " xmi:type=\"uml:Slot\"";
    out.ref(slot.value, "value");
    out << "/>\n";
  }

  write_description_properties(out);

  out.indent(-1);
  out.indent();
  out << "</" << k << ">\n"; 

  unload();
}

