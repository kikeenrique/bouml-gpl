
#include "UmlBaseDiagram.h"

#include "UmlCom.h"
bool UmlBaseDiagram::saveIn(const QByteArray & s) const {
  UmlCom::send_cmd(_identifier, saveInCmd, s);
  return UmlCom::read_bool();
}

