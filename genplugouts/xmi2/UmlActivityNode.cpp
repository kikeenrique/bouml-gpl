
#include "UmlActivityNode.h"
#include "FileOut.h"

void UmlActivityNode::memo_incoming_flow() {
  const QVector<UmlItem> ch = children(); 
  unsigned n = ch.size();
  unsigned i;
     
  for (i = 0; i != n; i += 1)
    (dynamic_cast<UmlActivityItem *>(ch[i]))->memo_incoming_flow();
}

void UmlActivityNode::add_incoming_flow(UmlFlow * flow) {
  _incoming_flows.append(flow);
}

void UmlActivityNode::write_incoming_flows(FileOut & out) {
  while (!_incoming_flows.isEmpty()) {
    out.indent();
    out << "<incoming";
    out.idref(_incoming_flows.take(0));
    out << "/>\n";
  }
}

