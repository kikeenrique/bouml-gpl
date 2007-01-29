
#include "UmlPseudoState.h"
#include "FileOut.h"

void UmlPseudoState::memo_incoming_trans() {
  const QVector<UmlItem> ch = children(); 
  unsigned n = ch.size();
  unsigned i;
     
  for (i = 0; i != n; i += 1)
    (dynamic_cast<UmlStateItem *>(ch[i]))->memo_incoming_trans();
}

void UmlPseudoState::add_incoming_trans(UmlTransition * tr) {
  _incoming_trans.append(tr);
}

void UmlPseudoState::write(FileOut & out) {
  out.indent();
  out << "<connectionPoint xmi:type=\"uml:Pseudostate\"";
  out.id(this);
  if (! name().isEmpty()) {
    out << " name=\"";
    out.quote(name());
    out << '"';
  }
  out << " kind=\"" << sKind() << "\">\n";

  out.indent(+1);
  
  write_description_properties(out); 
     
  while (! _incoming_trans.isEmpty())
    _incoming_trans.take(0)->write_in(out);
  
  const QVector<UmlItem> ch = children(); 
  unsigned n = ch.size();
  unsigned i;
     
  for (i = 0; i != n; i += 1)
    ch[i]->write(out);

  out.indent(-1);
  out.indent();
  out << "</connectionPoint>\n";
    
  unload(); 
}

