
#include "UmlClassMember.h"

void UmlClassMember::annotation() {
  if (! javaAnnotations().isEmpty()) {
    fw.write("<p> Java annotation(s) :<ul>");
    writeq(javaAnnotations());
    fw.write("</ul>");
  }
}

