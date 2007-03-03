
#include "UmlActivityPin.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

void UmlActivityPin::init()
{
  declareFct("argument", "uml:InputPin", &importIt);
  declareFct("input", "uml:InputPin", &importIt);
  declareFct("insertat", "uml:InputPin", &importIt);
  declareFct("object", "uml:InputPin", &importIt);
  declareFct("removeat", "uml:InputPin", &importIt);
  declareFct("request", "uml:InputPin", &importIt);
  declareFct("target", "uml:InputPin", &importIt);
  declareFct("value", "uml:InputPin", &importIt);
  
  declareFct("output", "uml:OutputPin", &importIt);
  declareFct("result", "uml:OutputPin", &importIt);
}

void UmlActivityPin::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(anActivityPin, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlActivityPin * a = create((UmlActivityAction *) where, s);
    
    if (a == 0)
      in.error("cannot create activity pin '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);

    a->set_Direction((token.xmiType() == "uml:InputPin")
		     ? InputDirection : OutputDirection);
    if (token.valueOf("isunique") == "true")
      a->set_IsUnique(TRUE);
    if (token.valueOf("isexception") == "true")
      a->set_IsException(TRUE);
    if (token.valueOf("isstream") == "true")
      a->set_IsStream(TRUE);

    a->import_it(in, token);
  }
}

