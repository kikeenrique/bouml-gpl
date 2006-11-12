
#include "UmlRegion.h"
#include "UmlClass.h"

#include "UmlState.h"

void UmlRegion::init(UmlClass * mother, QCString path, UmlState *) {
  const QVector<UmlItem> ch = children();
  unsigned index;
  
  for (index = 0; index != ch.count(); index += 1)
    if (ch[index]->kind() == aState)
      ((UmlState *) ch[index])->init(mother, path, 0);

}

