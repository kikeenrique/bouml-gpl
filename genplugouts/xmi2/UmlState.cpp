
#include "UmlState.h"
#include "FileOut.h"
#include "UmlTransition.h"

void UmlState::write(FileOut & out) {
  anItemKind pkind = parent()->kind();
  bool mach = (pkind != aState) && (pkind != aRegion);
  const char * k;
  
  if (mach) {
    k = (_uml_20) ? "ownedMember" : "packagedElement";
    memo_incoming_trans();
  }
  else
    k = "subvertex";
  
  out.indent(); 
  out << "<" << k << " xmi:type=\"uml:"
    << ((mach || (stereotype() == "machine")) ? "StateMachine" : "State")
    << '"';
  out.id(this); 
  out << " name=\"";
  out.quote(name());
  out << "\">\n";
  out.indent(+1); 
  
  write_description_properties(out); 
     
  while (! _incoming_trans.isEmpty())
    _incoming_trans.take(0)->write_in(out);
  
  const QVector<UmlItem> ch = children(); 
  unsigned n = ch.size();
  unsigned i;
  bool need_region = FALSE;
     
  for (i = 0; i != n; i += 1) {
    if (ch[i]->kind() != aRegion) {
      need_region = TRUE;
      break;
    }
  }

  if (need_region) {
    out.indent();
    out << "<region xmi:type=\"uml:Region\"";
    out.id_prefix(this, "IMPLICIT_REGION_");
    out << " name=\"Bouml_Implicit_Region\">\n";
    out.indent(+1);
    
    for (i = 0; i != n; i += 1)
      if (ch[i]->kind() != aRegion)
	ch[i]->write(out);
    
    while (! _trans.isEmpty())
      _trans.take(0)->write_it(out);
    
    out.indent(-1);
    out.indent();
    out << "</region>\n";
  }

  for (i = 0; i != n; i += 1)
    if (ch[i]->kind() == aRegion)
      ch[i]->write(out);
       
  out.indent(-1); 
  out.indent(); 
  out << "</" << k << ">\n"; 
    
  unload(); 
}

void UmlState::memo_incoming_trans() {
  const QVector<UmlItem> ch = children(); 
  unsigned n = ch.size();
  unsigned i;
     
  for (i = 0; i != n; i += 1) {
    UmlStateItem * x = dynamic_cast<UmlStateItem *>(ch[i]);

    if (x != 0)
      x->memo_incoming_trans();
  }
}

void UmlState::memo_trans(UmlTransition * tr) {
  _trans.append(tr);
}

void UmlState::add_incoming_trans(UmlTransition * tr) {
  _incoming_trans.append(tr);
}

UmlState * UmlState::find(QCString s)
{
  QMap<QCString, UmlState *>::Iterator iter = _all.find(s);
  
  return (iter == _all.end()) ? 0 : *iter;
}

QMap<QCString, UmlState *> UmlState::_all;

