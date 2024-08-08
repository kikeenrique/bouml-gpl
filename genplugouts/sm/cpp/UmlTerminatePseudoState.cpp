
#include "UmlTerminatePseudoState.h"
#include "UmlClass.h"
#include "UmlState.h"

void UmlTerminatePseudoState::generate(UmlClass *, UmlClass *, UmlState *, QByteArray & body, QByteArray indent) {
  body += indent + "stm._final();\n";
}

