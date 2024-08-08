
#include <qtextstream.h> 

#include "UmlTypeSpec.h"
#include "UmlClass.h"

void UmlTypeSpec::generate_import(QTextStream & f, UmlArtifact * using_art, 
				  bool from, QByteArray & made) const {
  if (type != 0)
    type->generate_import(f, using_art, from, made);
}


