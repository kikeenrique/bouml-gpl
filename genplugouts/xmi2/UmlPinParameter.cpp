
#include "UmlPinParameter.h"
#include "FileOut.h"

void UmlPinParameter::write_dir(FileOut & out) {
  out << " direction =\"";
  if (_pk_prefix)
    out << "pk_";
  switch (direction()) {
  case InputOutputDirection: out << "inout\""; break;
  case OutputDirection: out << "out\""; break;
  default: out << "in\"";
  }
}

void UmlPinParameter::write_flags(FileOut & out) {
  if (isUnique())
    out << " isUnique=\"true\"";
  if (isException())
    out << " isException=\"true\"";
  if (isStream())
    out << " isStream=\"true\"";
}

