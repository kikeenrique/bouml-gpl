
#include "UmlActivityActionClasses.h"
#include "FileOut.h"

#include "UmlOperation.h"
#include "UmlActivity.h"
void UmlSendObjectAction::write(FileOut & out) {
  write_begin(out, "SendObjectAction");
  write_end(out);
}

void UmlUnmarshallAction::write(FileOut & out) {
  write_begin(out, "UnmarshallAction");
  write_end(out);
}

void UmlSendSignalAction::write(FileOut & out) {
  write_begin(out, "SendSignalAction");
  
  QCString sig;
  
  switch (_lang) {
  case Uml:
    sig = signal();
    break;
  case Cpp:
    sig = cppSignal();
    break;
  default:
    // java
    sig = javaSignal();
  }
  if (! sig.isEmpty()) {
    out << " signal=\"";
    out.quote(sig);
    out << '"';
  }

  write_end(out);
}

void UmlBroadcastSignalAction::write(FileOut & out) {
  write_begin(out, "BroadcastSignalAction");
  
  QCString sig;
  
  switch (_lang) {
  case Uml:
    sig = signal();
    break;
  case Cpp:
    sig = cppSignal();
    break;
  default:
    // java
    sig = javaSignal();
  }
  if (! sig.isEmpty()) {
    out << " signal=\"";
    out.quote(sig);
    out << '"';
  }

  write_end(out);
}

void UmlValueSpecificationAction::write(FileOut & out) {
  write_begin(out, "ValueSpecificationAction");
  
  QCString val;
  
  switch (_lang) {
  case Uml:
    val = value();
    break;
  case Cpp:
    val = cppValue();
    break;
  default:
    // Java
    val = javaValue();
  }
  if (! val.isEmpty())
    out.ref(activity()->add_opaque_expression(val), "value", "OPAQUE_EXPRESSION_");

  write_end(out);
}

void UmlOpaqueAction::write(FileOut & out) {
  write_begin(out, "OpaqueAction");
  write_end(out, TRUE);
  
  QCString body;
  
  switch(_lang) {
  case Uml:
    body = behavior();
    break;
  case Cpp:
    body = cppBehavior();
    break;
  default:
    // Java
    body = javaBehavior();
  }

  if (!body.isEmpty()) {
    out.indent();
    out << "<body>";
    out.quote(body);
    out << "</body>\n";
  }

  write_close(out);

}

void UmlAcceptEventAction::write(FileOut & out) {
  write_begin(out, "AcceptEventAction");
  if (isUnmarshall())
    out << " isUnmarshall=\"true\"";
  
  QCString trig;
  
  switch (_lang) {
  case Uml:
    trig = trigger();
    break;
  case Cpp:
    trig = cppTrigger();
    break;
  default:
    // java
    trig = javaTrigger();
  }

  if (! trig.isEmpty()) {
    out << " trigger=\"";
    out .quote(trig);
    out << '"';
  }

  write_end(out);
}

void UmlCallOperationAction::write(FileOut & out) {
  write_begin(out, "CallOperationAction");
  write_end(out, TRUE);
  
  UmlOperation * op = operation();
  
  if (op != 0) {
    out.indent();
    out << "<operation";
    out.idref(op);
    out << "/>\n";
  }

  write_close(out);

}

void UmlCallBehaviorAction::write(FileOut & out) {
  write_begin(out, "CallBehaviorAction");
  write_end(out, TRUE);
  
  UmlItem * b = behavior();
  
  if (b != 0) {
    out.indent();
    out << "<behavior";
    out.idref(b);
    out << "/>\n";
  }

  write_close(out);

}

void UmlClearVariableValueAction::write(FileOut & out) {
  write_begin(out, "ClearVariableAction");
  write_var_end(out);
}

void UmlReadVariableValueAction::write(FileOut & out) {
  write_begin(out, "ReadVariableAction");
  write_var_end(out);
}

void UmlWriteVariableValueAction::write(FileOut & out) {
  write_begin(out, "WriteVariableAction");
  write_var_end(out);

}

void UmlAddVariableValueAction::write(FileOut & out) {
  write_begin(out, "AddVariableValueAction");
  
  if (isReplaceAll())
    out << " isReplaceAll=\"true\"";
  
  write_var_end(out);
}

void UmlRemoveVariableValueAction::write(FileOut & out) {
  write_begin(out, "RemoveVariableValueAction");

  if (isRemoveDuplicates())
    out << " isRemoveDuplicates=\"true\"";

  write_var_end(out);
}

