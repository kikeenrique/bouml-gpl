
#include "UmlClassMember.h"
#include "FileIn.h"
#include "Token.h"

void UmlClassMember::setVisibility(QCString v) {
  if ((v == "public") || (v == "vis_public"))
    set_Visibility(PublicVisibility);
  else if ((v == "protected") || (v == "vis_protected"))
    set_Visibility(ProtectedVisibility);
  else if ((v == "private") || (v == "vis_private"))
    set_Visibility(PrivateVisibility);
  else if ((v == "package") || (v == "vis_package"))
    set_Visibility(PackageVisibility);
}

QCString UmlClassMember::readConstraint(FileIn & in, Token & token)
{
  QCString constraint;
  
  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      QCString s = token.what();
      
      if ((s == "specification") && (token.xmiType() == "uml:OpaqueExpression")) {
	constraint = token.valueOf("body");
	
	if (! token.closed()) {
	  while (in.read(), !token.close("specification")) {
	    s = token.what();
	    
	    if (s == "body")
	      constraint = in.body("body");
	    else if (! token.closed())
	      in.finish(s);
	  }
	}
      }
      else
	in.bypass(token);
    }
  }

  return constraint;
}

