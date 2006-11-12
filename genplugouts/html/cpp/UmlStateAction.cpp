
#include "UmlCom.h"
#include "UmlStateAction.h"

QCString UmlStateAction::sKind() {
  return "state action";
}

void UmlStateAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlCom::message(name());

  QCString s = stereotype();
  
  if (s.isEmpty())
    s = "State action";
  else
    s = "State action " + s;
    
  UmlItem::html(s, (UmlDiagram *) 0);
  
  fw.write("<p>Defined in ");
  if (parent()->kind() == aRegion)
    parent()->parent()->write();
  else
    parent()->write();
  fw.write("</p>");

  fw.write("<p>Expression :</p><ul>");
  
  s = expression();

  if (!s.isEmpty()) {
    fw.write("<li>OCL : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = cppExpression();

  if (!s.isEmpty()) {
    fw.write("<li>C++ : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = javaExpression();

  if (!s.isEmpty()) {
    fw.write("<li>Java : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  fw.write("</ul>");

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

