
#include "UmlRegion.h"
#include "UmlTransition.h"
#include "FileOut.h"

void UmlRegion::memo_incoming_trans() {
  const QVector<UmlItem> ch = children(); 
  unsigned n = ch.size();
  unsigned i;
     
  for (i = 0; i != n; i += 1)
    (dynamic_cast<UmlStateItem *>(ch[i]))->memo_incoming_trans();
}

void UmlRegion::memo_trans(UmlTransition * tr) {
  _trans.append(tr);
}

void UmlRegion::write(FileOut & out) {
  out.indent(); 
  out << "<region xmi:type=\"uml:Region\"";
  out.id(this); 
  out << " name=\"";
  out.quote(name());
  out << "\">\n";
  out.indent(+1); 
  
  write_description_properties(out); 
     
  const QVector<UmlItem> ch = children(); 
  unsigned n = ch.size();
  unsigned i;
  
  for (i = 0; i != n; i += 1)
    ch[i]->write(out);
    
  while (! _trans.isEmpty())
    _trans.take(0)->write(out);

  out.indent(-1); 
  out.indent(); 
  out << "</region>\n"; 
    
  unload(); 
}

