
#include "UmlPseudoState.h"
#include "FileIn.h"
#include "Token.h"

#include "UmlFinalState.h"
#include "UmlChoicePseudoState.h"
#include "UmlDeepHistoryPseudoState.h"
#include "UmlEntryPointPseudoState.h"
#include "UmlExitPointPseudoState.h"
#include "UmlForkPseudoState.h"
#include "UmlInitialPseudoState.h"
#include "UmlJoinPseudoState.h"
#include "UmlJunctionPseudoState.h"
#include "UmlShallowHistoryPseudoState.h"
#include "UmlTerminatePseudoState.h"
void UmlPseudoState::init()
{
  declareFct("connectionpoint", "uml:Pseudostate", &importIt);
  declareFct("subvertex", "uml:Pseudostate", &importIt);

  declareFct("connectionpoint", "", &importIt);// Borland Together 2006 for Eclipse
}

void UmlPseudoState::importIt(FileIn & in, Token & token, UmlItem * where)
{
  QCString k = token.valueOf("kind");
  
  if (k == "final") {
    // Visual Paradigm 6.1
    UmlFinalState::importIt(in, token, where);
    return;
  }
  
  // search a container for any pseudo state
  where = where->container(anInitialPseudoState, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    
    if (s.isEmpty()) {
      static unsigned n = 0;
      
      s.sprintf("anonymous_pseudo_state_%u", ++n);
    }
    
    UmlPseudoState * ps;
    
    if (k == "initial")
      ps = UmlInitialPseudoState::create(where);
    else if (k == "entryPoint")
      ps = UmlEntryPointPseudoState::create(where, s);
    else if (k == "terminate")
      ps = UmlTerminatePseudoState::create(where);
    else if (k == "exitPoint")
      ps = UmlExitPointPseudoState::create(where, s);
    else if (k == "deepHistory")
      ps = UmlDeepHistoryPseudoState::create(where);
    else if (k == "shallowHistory")
      ps = UmlShallowHistoryPseudoState::create(where);
    else if (k == "junction")
      ps = UmlJunctionPseudoState::create(where);
    else if (k == "choice")
      ps = UmlChoicePseudoState::create(where);
    else if (k == "fork")
      ps = UmlForkPseudoState::create(where);
    else if (k == "join")
      ps = UmlJoinPseudoState::create(where);
    else {
      in.warning("unknown pseudo state kind '" + k + "'");
      if (! token.closed())
	in.finish(token.what());
      return;
    }
    
    if (ps == 0)
      in.error("cannot create pseudo state of kind '" + k +
	       "' in '" + where->name() + "'");

    ps->addItem(token.xmiId(), in);
    
    if (! token.closed()) {
      QCString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr))
	ps->UmlItem::import(in, token);
    }
  }
}

