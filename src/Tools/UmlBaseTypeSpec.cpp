
#include "UmlBaseTypeSpec.h"
#include "UmlClass.h"

const QByteArray & UmlBaseTypeSpec::toString() const {
  return (type) ? type->name() : explicit_type;
}

