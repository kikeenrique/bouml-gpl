
#include "UmlActivityActionClasses.h"

#include "UmlOperation.h"
QCString UmlSendObjectAction::sKind() {
  return "send object activity action";
}

void UmlSendObjectAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlUnmarshallAction::sKind() {
  return "unmarshall activity action";
}

void UmlUnmarshallAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlSendSignalAction::sKind() {
  return "send signal activity action";
}

QCString UmlBroadcastSignalAction::sKind() {
  return "broadcast signal activity action";
}

QCString UmlValueSpecificationAction::sKind() {
  return "value specification activity action";
}

void UmlValueSpecificationAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  fw.write("<p>Value :</p><ul>");
  
  QCString s = value();

  if (!s.isEmpty()) {
    fw.write("<li>OCL : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = cppValue();

  if (!s.isEmpty()) {
    fw.write("<li>C++ : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = javaValue();

  if (!s.isEmpty()) {
    fw.write("<li>Java : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  fw.write("</ul>");

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlOpaqueAction::sKind() {
  return "opaque activity action";
}

void UmlOpaqueAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  fw.write("<p>Behavior :</p><ul>");
  
  QCString s = behavior();

  if (!s.isEmpty()) {
    fw.write("<li>OCL : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = cppBehavior();

  if (!s.isEmpty()) {
    fw.write("<li>C++ : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = javaBehavior();

  if (!s.isEmpty()) {
    fw.write("<li>Java : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  fw.write("</ul>");

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlAcceptEventAction::sKind() {
  return "accept event activity action";
}

void UmlAcceptEventAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isUnmarshall()) {
    if (isTimeEvent())
      fw.write("<p>Unmarshall, event is a time event</p>");
    else
      fw.write("<p>Unmarshall</p>");
  }
  else if (isTimeEvent())
    fw.write("<p>Event is a time event</p>");
  
  QCString s = trigger();

  fw.write("<p>Trigger :</p><ul>");
  
  if (!s.isEmpty()) {
    fw.write("<li>OCL : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = cppTrigger();

  if (!s.isEmpty()) {
    fw.write("<li>C++ : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = javaTrigger();

  if (!s.isEmpty()) {
    fw.write("<li>Java : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  fw.write("</ul>");

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlCallOperationAction::sKind() {
  return "call operation activity action";
}

void UmlCallOperationAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isSynchronous())
    fw.write("<p>Is synchronous</p>");
    
  if (operation() != 0){
    fw.write("<p>Operation : ");
    operation()->write();
    fw.write("</p>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlCallBehaviorAction::sKind() {
  return "call behavior activity action";
}

void UmlCallBehaviorAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isSynchronous())
    fw.write("<p>Is synchronous</p>");
    
  if (behavior() != 0){
    fw.write("<p>Behavior : ");
    behavior()->write();
    fw.write("</p>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlClearVariableValueAction::sKind() {
  return "clear variable value activity action";
}

QCString UmlReadVariableValueAction::sKind() {
  return "read variable value activity action";
}

QCString UmlWriteVariableValueAction::sKind() {
  return "write variable value activity action";
}

QCString UmlAddVariableValueAction::sKind() {
  return "add variable value activity action";
}

void UmlAddVariableValueAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isReplaceAll())
    fw.write("<p>Replace all</p>");
    
  if (variable() != 0){
    fw.write("<p>Variable : ");
    variable()->write();
    fw.write("</p>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlRemoveVariableValueAction::sKind() {
  return "remove variable value activity action";
}

void UmlRemoveVariableValueAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isRemoveDuplicates())
    fw.write("<p>Remove duplicates</p>");
    
  if (variable() != 0){
    fw.write("<p>Variable : ");
    variable()->write();
    fw.write("</p>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

