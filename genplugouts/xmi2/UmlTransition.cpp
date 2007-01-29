
#include "UmlTransition.h"
#include "FileOut.h"

void UmlTransition::memo_incoming_trans() {
  (dynamic_cast<UmlStateItem *>(target()))->add_incoming_trans(this);
}

void UmlTransition::write(FileOut & out) {
  (dynamic_cast<UmlStateItem *>(parent()->parent()))->memo_trans(this);
  
  out.indent();
  out << "<outgoing";
  out.idref(this);
  out << "/>\n";
}

void UmlTransition::write_it(FileOut & out) {
  out.indent();
  out << "<transition xmi:type=\"uml:Transition\"";
  out.id(this);
  out.ref(parent(), "source");
  out.ref(target(), "target");
  out << ">\n";
  out.indent(+1);
  
  QCString trig;
  QCString grd;
  QCString effect;
  
  switch (_lang) {
  case Uml:
    trig = trigger();
    grd = guard();
    effect = activity();
    break;
  case Cpp:
    trig = cppTrigger();
    grd = cppGuard();
    effect = cppActivity();
    break;
  default: // Java
    trig = javaTrigger();
    grd = javaGuard();
    effect = javaActivity();
    break;
  }

  if (! trig.isEmpty()) {
    out.indent();
    out << "<trigger xmi:type=\"uml:Trigger\"";
    out.id_prefix(this, "TRIGGER_");
    out << " name=\"";
    out.quote(trig);
    out << "\"/>\n";
  }

  if (! grd.isEmpty()) {
    out.indent();
    out << "<guard xmi:type=\"uml:Constraint\"";
    out.id_prefix(this, "GUARD_");
    out << ">\n";
    out.indent();
    out << "\t<specification xmi:type=\"uml:OpaqueExpression\"";
    out.id_prefix(this, "GUARD_EXPR_");
    out << ">\n";
    out.indent();
    out << "\t\t<body>";
    out.quote(grd);
    out << "</body>\n";
    out.indent();
    out << "\t</specification>\n";
    out.indent();
    out << "</guard>\n";
  }

  if (! effect.isEmpty()) {
    out.indent();
    out << "<effect xmi:type=\"uml:Activity\"";
    out.id_prefix(this, "EFFECT_");
    out << ">\n";
    out.indent();
    out << "\t<body>";
    out.quote(effect);
    out << "</body>\n";
    out.indent();
    out << "</effect>\n";
  }
  
  out.indent(-1);
  out.indent();
  out << "</transition>\n";
  
  unload(); 
}

void UmlTransition::write_in(FileOut & out) {
  out.indent();
  out << "<incoming";
  out.idref(this);
  out << "/>\n";
}

