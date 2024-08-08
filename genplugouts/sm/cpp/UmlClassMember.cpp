
#include "UmlClassMember.h"

void UmlClassMember::setComment(QByteArray s) {
  set_Description(((cppDecl().find("${description}") != -1) ? "// " : " ")
		  + s);

}

