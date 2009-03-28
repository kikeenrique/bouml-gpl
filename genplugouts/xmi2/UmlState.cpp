
#include "UmlState.h"
#include "FileOut.h"
#include "UmlTransition.h"

#include "UmlOperation.h"
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
  out << '"';
  if (specification() != 0)
    out.ref(specification(), "specification");
  out << ">\n";
  out.indent(+1); 
  
  write_description_properties(out); 
  
  QCString doentry;
  QCString doactivity;
  QCString doexit;
  
  switch (_lang) {
  case Uml:
    doentry = entryBehavior();
    doactivity = doActivity();
    doexit = exitBehavior();
    break;
  case Cpp:
    doentry = cppEntryBehavior();
    doactivity = cppDoActivity();
    doexit = cppExitBehavior();
    break;
  default: // Java
    doentry = javaEntryBehavior();
    doactivity = javaDoActivity();
    doexit = javaExitBehavior();
    break;
  }
  
  if (! doentry.isEmpty()) {
    out.indent();
    out << "<entry xmi:type=\"uml:Activity\"";
    out.id_prefix(this, "ENTRY_");
    out << ">\n";
    out.indent();
    out << "\t<body>";
    out.quote(doentry);
    out << "</body>\n";
    out.indent();
    out << "</entry>\n";
  }
  
  if (! doactivity.isEmpty()) {
    out.indent();
    out << "<doActivity xmi:type=\"uml:Activity\"";
    out.id_prefix(this, "DOACTIVITY_");
    out << ">\n";
    out.indent();
    out << "\t<body>";
    out.quote(doactivity);
    out << "</body>\n";
    out.indent();
    out << "</doActivity>\n";
  }
  
  if (! doexit.isEmpty()) {
    out.indent();
    out << "<exit xmi:type=\"uml:Activity\"";
    out.id_prefix(this, "EXIT_");
    out << ">\n";
    out.indent();
    out << "\t<body>";
    out.quote(doexit);
    out << "</body>\n";
    out.indent();
    out << "</exit>\n";
  }
     
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
    
#if 0
    // to bypass Eclipse's bug
    while (! _trans.isEmpty())
      _trans.take(0)->write_it(out);
#endif
    
    out.indent(-1);
    out.indent();
    out << "</region>\n";
  }

  for (i = 0; i != n; i += 1)
    if (ch[i]->kind() == aRegion)
      ch[i]->write(out);
       
#if 1
    // to bypass Eclipse's bug
    while (! _trans.isEmpty())
      _trans.take(0)->write_it(out);
#endif
    
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

