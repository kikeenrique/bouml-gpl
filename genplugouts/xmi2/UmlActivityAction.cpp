
#include "UmlActivityAction.h"
#include "FileOut.h"

void UmlActivityAction::write_begin(FileOut & out, QCString k) {
  out.indent();
  out << ((parent()->kind() == anActivity) ? "<node" : "<containedNode")
    << " xmi:type=\"" << k << '"';
  out.id(this);
  if (!name().isEmpty()){
    out << " name=\"";
    out.quote(name());
    out << '"';
  }
}

void UmlActivityAction::write_end(FileOut & out, bool dontclose) {
  out << ">\n";
  out.indent(+1);
  
  write_description_properties(out);

  switch (_lang) {
  case Uml:
    write_condition(out, preCondition(), postCondition());
    break;
  case Cpp:
    write_condition(out, cppPreCondition(), cppPostCondition());
  default:
    // java
    write_condition(out, javaPreCondition(), javaPostCondition());
  }

  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->write(out);

  write_incoming_flows(out);
  
  if (!dontclose)
    write_close(out);
}

void UmlActivityAction::write_close(FileOut & out) {
  out.indent(-1);
  out.indent();
  out << ((parent()->kind() == anActivity) ? "</node>\n" : "</containedNode>\n");
    
  unload();
}

void UmlActivityAction::write_condition(FileOut & out, QCString cond, bool pre) {
  if (! cond.isEmpty()) {
    const char * k;
    const char * K;
    const char * body;
    
    if (pre) {
      k = "pre";
      K = "PRE_";
      body = "PRE_BODY_";
    }
    else {
      k = "post";
      K = "POST_";
      body = "POST_BODY_";
    }
    
    out.indent();
    out << '<' << k << "condition xmi:type=\"uml:Constraint\"";
    out.id_prefix(this, K);
    out << ">\n";
    out.indent();
    out << "\t<specification xmi:type=\"uml:OpaqueExpression\"";
    out.id_prefix(this, body);
    out << " body=\"";
    out.quote(cond);
    out << "\"/>\n";
    out.indent();
    out << "</" << k << "condition>\n";
  }
}

