
#include "UmlActivityAction.h"

#include "UmlCom.h"
void UmlActivityAction::html() {
  UmlCom::message(name());

  QCString s = sKind();
  
  s.replace(0, 1, s.left(1).upper());
    
  UmlItem::html(s, (UmlDiagram *) associatedDiagram());
  
  fw.write("<p>Defined in ");
  parent()->write();
  fw.write("</p>");

  fw.write("<p>Pre Condition :</p><ul>");
  
  s = preCondition();

  if (!s.isEmpty()) {
    fw.write("<li>OCL : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = cppPreCondition();

  if (!s.isEmpty()) {
    fw.write("<li>C++ : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = javaPreCondition();

  if (!s.isEmpty()) {
    fw.write("<li>Java : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  fw.write("</ul>");

  fw.write("<p>Post Condition :</p><ul>");
  
  s = postCondition();

  if (!s.isEmpty()) {
    fw.write("<li>OCL : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = cppPostCondition();

  if (!s.isEmpty()) {
    fw.write("<li>C++ : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = javaPostCondition();

  if (!s.isEmpty()) {
    fw.write("<li>Java : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  fw.write("</ul>");

  if (! constraint().isEmpty()) {
    fw.write("<p> Constraint :<ul>");
    writeq(constraint());
    fw.write("</ul>");
  }
}

