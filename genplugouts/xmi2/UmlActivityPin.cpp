
#include "UmlActivityPin.h"
#include "FileOut.h"

void UmlActivityPin::write(FileOut & out) {
  QCString s = name();
  const char * k;
  const char * K;
  
  if (direction() == InputDirection) {
    switch (parent()->kind()) {
    case aSendObjectAction:
      k = ((s == "target") || (s == "request"))
	? ((const char *) s)
	: "argument";
      break;
    case anUnmarshallAction:
      k = (s == "object") ? ((const char *) s) : "input";
      break;
    case aSendSignalAction:
    case aCallOperationAction:
    case aCallBehaviorAction:
      k = (s == "target") ? ((const char *) s) : "argument";
      break;
    case anAddVariableValueAction:
      k = (s == "insertAt") ? ((const char *) s) : "value";
      break;
    case aRemoveVariableValueAction:
      k = (s == "removeAt") ? ((const char *) s) : "value";
      break;
    case aBroadcastSignalAction:
      k = "argument";
      break;
    case aWriteVariableValueAction:
      k = "value";
      break;
    default:
      k = "input";
    }
    K = "Input";
  }
  else {
    switch (parent()->kind()) {
    case anUnmarshallAction:
    case anAcceptEventAction:
    case aCallOperationAction:
    case aCallBehaviorAction:
    case aReadVariableValueAction:
      k = "result";
      break;
    default:
      k = "output";
    }
    K = "Output";
  }

  out.indent();
  out << '<' << k << " xmi:type=\"uml:" << K << "Pin\" name=\"";
  out.quote(name());
  out << '"';
  out.id(this);
  write_flags(out);
  if (isControlType())
    out << " isControlType=\"true\"";
  write_ordering(out);
  write_selection(out);
  write_in_state(out);
  out << ">\n";
  
  out.indent(+1);
  
  write_description_properties(out);
  write_multiplicity(out, multiplicity(), this);
  UmlItem::write_type(out, type());

  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->write(out);
  
  write_incoming_flows(out);
  
  out.indent(-1);

  out.indent();
  out << "</" << k << ">\n";

  unload();
}

