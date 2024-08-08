
#include "UmlAccessVariableValueAction.h"
#include "FileIn.h"
#include "Token.h"

#include "UmlCom.h"
void UmlAccessVariableValueAction::import_it(FileIn & in, Token & token) {
  QByteArray s = token.valueOf("variable");
  
  if (! s.isEmpty())
    setVariable(s);
    
  if (! token.closed()) {
    QByteArray k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      s = token.what();
      
      if (s == "variable") {
	setVariable(token.xmiIdref());
	if (! token.closed())
	  in.finish("variable");
      }
      else
	import(in, token);
    }
  }
}

void UmlAccessVariableValueAction::setVariable(QByteArray idref) {
  QMap<QByteArray, UmlItem *>::Iterator it = All.find(idref);
  
  if (it == All.end())
    Unresolved::addRef(this, idref);
  else {
    switch ((*it)->kind()) {
    case anAttribute:
    case aRelation:
      set_Variable(*it);
    default:
      break;
    }
  }
}

void UmlAccessVariableValueAction::solve(QByteArray idref) {
  QMap<QByteArray, UmlItem *>::Iterator it = All.find(idref);
  
  if (it == All.end()) {
    if (!FileIn::isBypassedId(idref))
      UmlCom::trace("activity action : unknown variable reference '" + idref + "'<br>");
  }
  else {
    switch ((*it)->kind()) {
    case anAttribute:
    case aRelation:
      set_Variable(*it);
    default:
      break;
    }
  }
}

