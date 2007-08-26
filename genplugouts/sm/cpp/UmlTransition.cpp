
#include "UmlTransition.h"
#include "UmlClass.h"

#include <qlist.h>

#include "UmlCom.h"
#include "UmlState.h"
#include "UmlOperation.h"

QCString UmlTransition::triggerName() {
  // get & check trigger's name
  QCString s = cppTrigger();
  
  if (s.isEmpty()) {
    switch (parent()->kind()) {
    case anInitialPseudoState:
#warning to check
    case anEntryPointPseudoState:
      s = "create";
      break;
    case aState:
    case anExitPointPseudoState:
      s = "_completion";
      break;
    default:
#warning to check
      UmlCom::trace("Error : transition must have a trigger<br>");
      throw 0;
    }
  }
  else if (parent()->kind() == anInitialPseudoState) {
    if (s != "create") {
      UmlCom::trace("Error : the transition from an 'initial' sub state may only have the trigger 'create'<br>");
      throw 0;
    }
  }
  else if (s == "create") {
    UmlCom::trace("Error : only the transition from an 'initial' sub state may have the trigger 'create'<br>");
    throw 0;
  }
  else if (s[0] == '_') {
    UmlCom::trace("Error : sorry, trigger's name can't start by a '_'<br>");
    throw 0;
  }

  return s;
}

void UmlTransition::init(UmlClass *, QCString, UmlState * state) {
  if (triggerName() == "_completion")
    state->setHasCompletion();
}

void UmlTransition::generate(UmlClass * machine, UmlClass * anystate, UmlState * state) {
  if (_already_managed)
    return;
    
  QCString s = triggerName();
  
  // group transitions having the same trigger
  const QVector<UmlItem> ch = parent()->children();
  unsigned index = ch.findRef(this);
  QList<UmlTransition> trs;
  
  trs.append(this);
  
  while (++index != ch.count()) {
    if ((ch[index]->kind() == aTransition) &&
	(((UmlTransition *) ch[index])->triggerName() == s)) {
      trs.append((UmlTransition *) ch[index]);
      ((UmlTransition *) ch[index])->_already_managed = TRUE;
    }
  }

  // made the trigger

  UmlOperation * trg = state->assocClass()->trigger(s, machine, anystate);
  QCString body;
  
  if (s == "create") {
    // manage entry
    if (!state->cppEntryBehavior().isEmpty())
      body = "  _doentry(stm);\n";
  }

  if (!state->cppDoActivity().isEmpty())
    // state do activity before each event except create
    body += "  _do(stm);\n";
    
  bool completion = (s == "_completion");

  if (!completion && state->isLeaf() && state->hasCompletion())
    // manage completion
    body += "  if (_completion(stm)) return;\n";
    
  UmlTransition::generate(trs, machine, anystate, state,
			  body, "  ", completion);
  
  trg->set_CppBody(body);
}

void UmlTransition::generate(UmlClass * machine, UmlClass * anystate, UmlState * state, QCString & body, QCString indent) {
  if (!cppTrigger().isEmpty()) {
    UmlCom::trace("Error : transition from a pseudo state can't have trigger<br>");
    throw 0;
  }

  QList<UmlTransition> l;
  
  l.append(this);
  generate(l, machine, anystate, state, body, indent, FALSE);
}

void UmlTransition::generate(QList<UmlTransition> trs, UmlClass * machine, UmlClass * anystate, UmlState * state, QCString & body, QCString indent, bool completion)
{
  UmlTransition * tr;
  bool guard = FALSE;
  
  for (tr = trs.first(); tr != 0; tr = trs.next()) {
    body += indent;
    
    if (!tr->cppGuard().isEmpty()) {
      // manage guard
      body += ((tr == trs.getFirst()) ? "if (" : "else if (")
	+ tr->cppGuard() + ") {\n";
      guard = TRUE;
    }
    else if (tr != trs.getFirst())
      body += ((tr == trs.getLast()) ? "else {\n" : "else if (1) {\n");
    else
      body += ((tr == trs.getLast()) ? "{\n" : "if (1) {\n");

    // the target state
    UmlItem * tg = tr->target();
    
    while (tg->kind() != aState)
      tg = tg->parent();
    
    // the parent common to the current and the target state
    UmlState * common = state;
    bool self_external = (state == tg) && tr->isExternal();
    
    if (self_external) {
      // execute exit behavior
      if (!state->cppExitBehavior().isEmpty())
	body += indent + "  _doexit(stm);\n";
    }
    else {
      bool fromExit = 
	// the exit behavior is made entering in the exit point
	(tr->parent()->kind() == anExitPointPseudoState);
      
      // compute common parent and manage exit behavior
      if (tr->target()->kind() != aTerminatePseudoState) {
	while (! ((UmlState *) tg)->inside(common)) {
	  if (!fromExit && !common->cppExitBehavior().isEmpty())
	    body += indent + "  stm" + common->path() + "._doexit(stm);\n";
	  fromExit = FALSE;
	  
	  switch (common->parent()->kind()) {
	  case aState:
	    common = (UmlState *) common->parent();
	    break;
	  case aRegion:
	    common = (UmlState *) common->parent()->parent();
	    break;
	  default:
	    UmlCom::trace("Error : transition from '" + state->name()
			  + "' goes outside the state machine");
		    throw 0;
	  }
	}
      }
    }
    
    // manage transition activity
    if (!tr->cppActivity().isEmpty())
      body += "\
#ifdef VERBOSE_STATE_MACHINE\n\
  puts(\"DEBUG : execute activity of transition " + tr->name() + "\");\n\
#endif\n" + tr->cppActivity();
  
    // manage entry behavior
    if (self_external) {
      if (!state->cppEntryBehavior().isEmpty())
	body += indent + "  _doentry(stm);\n";
    }
    else if (tr->target()->kind() != aTerminatePseudoState) {
      if (tg != common) {
	QCString enter;
	UmlState * tg_parent;
      
	// the enter behavior of the target state will be managed
	// generating a call to create
	for (tg_parent = (UmlState *) tg->parent();
	     tg_parent != common;
	     tg_parent = (UmlState *) tg_parent->parent())
	  if (!tg_parent->cppEntryBehavior().isEmpty())
	    enter.insert(0,
			 indent + "  stm" + tg_parent->path() + "._doentry(stm);\n");
	    
	if (!enter.isEmpty())
	  body += enter;
      }

      // set the current state if needed
      if (tg != state)
	body += indent + "  stm._set_currentState(stm"
	  + ((UmlState *) tg)->path() + ");\n\
#ifdef VERBOSE_STATE_MACHINE\n\
  puts(\"DEBUG : current state is now " + ((UmlState *) tg)->prettyPath() + "\");\n\
#endif\n";
    }

    // do the transition
    if (tr->target()->kind() == aState) {
      if (tg != state)
	// enter state
	body += indent + "  stm"
	  + ((UmlState *) tg)->path() + ".create(stm);\n";
    }
    else
      tr->target()->generate(machine, anystate, ((UmlState *) tg), body, indent + "  ");
    
    if (completion)
      body += indent + "  return (bool) 1;\n";
    
    body += indent + "}\n";
  }
 
  if (completion && guard)
    body += indent + "return (bool) 0;\n";
}

