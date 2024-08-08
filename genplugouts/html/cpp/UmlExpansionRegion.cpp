
#include "UmlExpansionRegion.h"

QByteArray UmlExpansionRegion::sKind() {
  return "expansion region";
}

void UmlExpansionRegion::html(QByteArray pfix, unsigned int rank, unsigned int level) {
  UmlItem::html(pfix, rank, "Expansion region", level, "expansionregion");

  unload(FALSE, FALSE);
}

