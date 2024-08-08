
#include "UmlCom.h"
#include "UmlActivityControlNode.h"

void UmlActivityControlNode::html(QByteArray pfix, unsigned int rank, unsigned int level) {
  define();

  UmlCom::message(name());

  QByteArray s = sKind();
  
  s.replace(0, 1, s.left(1).upper());
  UmlItem::html(s, (UmlDiagram *) 0);
  
  fw.write("<p>Defined in ");
  parent()->write();
  fw.write("</p>");
  
  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

