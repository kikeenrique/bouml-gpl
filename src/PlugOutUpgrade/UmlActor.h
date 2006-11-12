#ifndef UMLACTOR_H
#define UMLACTOR_H

#include "UmlBaseActor.h"

// This class manages 'actors'
// You can modify it as you want (except the constructor)

class UmlActor : public UmlBaseActor {
  public:
    UmlActor(void * id, const QCString & n)
      : UmlBaseActor(id, n) {};
};

#endif
