
#include "UmlNode.h"

#include "UmlCom.h"
QByteArray UmlNode::sKind() {
  return "node";
}

void UmlNode::html(QByteArray, unsigned int, unsigned int) {
  UmlCom::message(name());

  UmlItem::html("Node", (UmlDiagram *) associatedDiagram());

  unload(FALSE, FALSE);
}

