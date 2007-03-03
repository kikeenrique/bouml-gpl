
#include "UmlState.h"
#include "UmlItem.h"
#include "Token.h"
#include "FileIn.h"

#include "UmlRegion.h"
#include "UmlPseudoState.h"
#include "UmlFinalState.h"
#include "UmlTransition.h"
UmlItem * UmlState::container(anItemKind kind, const Token & token, FileIn & in) {
  switch (kind) {
  case aState:
  case aRegion:
  case anInitialPseudoState:
  case anEntryPointPseudoState:
  case aFinalState:
  case aTerminatePseudoState:
  case anExitPointPseudoState:
  case aDeepHistoryPseudoState:
  case aShallowHistoryPseudoState:
  case aJunctionPseudoState:
  case aChoicePseudoState:
  case aForkPseudoState:
  case aJoinPseudoState:
    return this;
  default:
    return parent()->container(kind, token, in);
  }

}

void UmlState::init()
{
  declareFct("ownedmember", "uml:StateMachine", &importIt);
  declareFct("packagedelement", "uml:StateMachine", &importIt);
  declareFct("subvertex", "uml:State", &importIt);
  
  UmlRegion::init();
  UmlPseudoState::init();
  UmlFinalState::init();
  UmlTransition::init();
}

void UmlState::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aState, token, in);
    
  if (where != 0) {
    bool machine = ((token.xmiType() == "uml:StateMachine") ||
		    (token.valueOf("issubmachinestate") == "true"));
    QCString s = token.valueOf("name");
    
    if (s.isEmpty()) {
      static unsigned n = 0;
      
      s.sprintf((machine) ? "anonymous_state_machine_%u" 
			  : "anonymous_state_%u",
		++n);
    }
    
    UmlState * st = create(where, s);
    
    if (st == 0)
      in.error((machine) ? "cannot create state machine '"
			 : "cannot create state '"
	       + s + "' in '" + where->name() + "'");
    
    st->addItem(token.xmiId(), in);
    
    if (! token.closed()) {
      QCString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr))
	st->UmlItem::import(in, token);
    }
    
    if (machine)
      st->set_Stereotype("machine");

    st->unload(TRUE, FALSE);
  }
}

int UmlState::NumberOf;

