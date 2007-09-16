
#include "UmlRelation.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

void UmlRelation::importAsAttribute(FileIn & in, Token & token, UmlItem * where)
{
  if (where->kind() == aClass)
    Association::get(token.valueOf("association")).import(in, token);
  else if (! token.closed())
    in.finish(token.what());
}

void UmlRelation::init()
{
  declareFct("ownedelement", "uml:Association", &importIt);
  declareFct("ownedmember", "uml:Association", &importIt);
  declareFct("packagedelement", "uml:Association", &importIt);

}

void UmlRelation::importIt(FileIn & in, Token & token, UmlItem *)
{
  Association & assoc = Association::get(token.xmiId(), token.valueOf("name"));
  
  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      QCString s = token.what();
      
      if ((s == "ownedend") && (token.xmiType() == "uml:Property"))
	assoc.import(in, token);
      else if (! token.closed())
	in.finish(s);
    }
  }
}

