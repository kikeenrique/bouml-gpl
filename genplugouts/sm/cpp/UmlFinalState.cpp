
#include "UmlFinalState.h"
#include "UmlClass.h"
#include "UmlState.h"

#include "UmlCom.h"
#include "UmlOperation.h"
#include "UmlTransition.h"

void UmlFinalState::generate(UmlClass * machine, UmlClass * anystate, UmlState * state, QCString & body, QCString indent) {
  if (parent()->kind() != aRegion)
    body += indent + "stm._final();\n";
  else {
#warning A FAIRE
  }
}

