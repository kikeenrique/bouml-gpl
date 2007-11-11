
#include "UmlTransition.h"

QCString UmlTransition::sKind() {
  return "transition";
}

void UmlTransition::html(QCString, unsigned int, unsigned int) {
  fw.write("<table><tr><td><div class=\"element\">Transition  <b>");
  writeq(name());
  fw.write("</div></td></tr></table>");
  
  if (parent() == target()) {
    fw.write("<p>Self relation of ");
    parent()->write();
    fw.write((isExternal()) ? ", external</p>" : ", internal</p>");
  }
  else {
    fw.write("<p>From ");
    parent()->write();
    fw.write(" To ");
    target()->write();
    fw.write("</p>");
  }

  QCString s = description();

  if (!s.isEmpty()) {
    fw.write("<p>");
    writeq(s);
    fw.write("<br /></p>");
  }

  fw.write("<p>Trigger :</p><ul>");
  
  s = trigger();

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

  fw.write("<p>Guard :</p><ul>");
  
  s = guard();

  if (!s.isEmpty()) {
    fw.write("<li>OCL : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = cppGuard();

  if (!s.isEmpty()) {
    fw.write("<li>C++ : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = javaGuard();

  if (!s.isEmpty()) {
    fw.write("<li>Java : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  fw.write("</ul>");

  fw.write("<p>Activity :</p><ul>");
  
  s = activity();

  if (!s.isEmpty()) {
    fw.write("<li>OCL : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = cppActivity();

  if (!s.isEmpty()) {
    fw.write("<li>C++ : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = javaActivity();

  if (!s.isEmpty()) {
    fw.write("<li>Java : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  fw.write("</ul>");
 
  write_properties();

  unload(FALSE, FALSE);
}

