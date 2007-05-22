
#include "UmlClassMember.h"

void UmlClassMember::annotation_constraint() {
  if (! javaAnnotations().isEmpty()) {
    fw.write("<p> Java annotation(s) :<ul>");
    writeq(javaAnnotations());
    fw.write("</ul>");
  }
  
  if (! constraint().isEmpty()) {
    fw.write("<p> Constraint :<ul>");
    writeq(constraint());
    fw.write("</ul>");
  }
}

