// *************************************************************************
//
// Copyleft 2004-2007 Bruno PAGES  .
//
// This file is part of the BOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : bouml@free.fr
// home   : http://bouml.free.fr
//
// *************************************************************************

#include <qmessagebox.h>

#include "UmlPackage.h"
#include "UmlClassView.h"
#include "UmlDeploymentView.h"
#include "UmlClass.h"
#include "UmlArtifact.h"
#include "UmlRelation.h"
#include "UmlOperation.h"
#include "UmlAttribute.h"
#include "UmlCom.h"
#include "UmlSettings.h"
#include "CppSettings.h"
#include "JavaSettings.h"

#define defGetField(base, where, oper, field, type, if_def, end_if, descr) \
  op = base->add_op(#oper, PublicVisibility, type); \
  op->set_Description(" return the " descr); \
  op->set_cpp("const ${type} &", "", \
	      "  read_if_needed_();\n" \
	      "  return "#where"."#field";\n", \
	      FALSE, if_def, end_if); \
  op->set_java("${type}", "", \
	       "  read_if_needed_();\n" \
	       "  return "#where"."#field";\n", FALSE)
	      
#define defSetField(base, where, oper, field, type, cmd, if_def, end_if, descr) \
  op = base->add_op(#oper, PublicVisibility, "bool", TRUE); \
  op->set_Description(" set the " descr "\n" \
		      "\n" \
		      " On error return FALSE in C++, produce a RuntimeException in Java"); \
  op->add_param(0, InputDirection, "v", type); \
  op->set_cpp("${type}", "const ${t0} ${p0}", \
	      "  return set_it_("#where"."#field", v, "#cmd");", \
	      FALSE, if_def, end_if); \
  op->set_java("void", "${t0} ${p0}", \
	       "\
  UmlCom.send_cmd(identifier_(), OnInstanceCmd."#cmd", v);\n\
  UmlCom.check();\n\
\n\
  "#where"."#field" = v;\n", \
	      FALSE)

#define defGet(base, where, oper, type, if_def, end_if, descr) \
  op = base->add_op(#oper, PublicVisibility, type); \
  op->set_Description(" return the " descr); \
  op->set_cpp("const ${type} &", "", \
	      "  read_if_needed_();\n" \
	      "  return "#where";\n", \
	      FALSE, if_def, end_if); \
  op->set_java("${type}", "", \
	       "  read_if_needed_();\n" \
	       "  return "#where";\n", FALSE)
	      
#define defSet(base, where, oper, type, cmd, if_def, end_if, descr) \
  op = base->add_op(#oper, PublicVisibility, "bool", TRUE); \
  op->set_Description(" set the " descr "\n" \
		      "\n" \
		      " On error return FALSE in C++, produce a RuntimeException in Java"); \
  op->add_param(0, InputDirection, "v", type); \
  op->set_cpp("${type}", "const ${t0} ${p0}", \
	      "  return set_it_("#where", v, "#cmd");", \
	      FALSE, if_def, end_if); \
  op->set_java("void", "${t0} ${p0}", \
	       "\
  UmlCom.send_cmd(identifier_(), OnInstanceCmd."#cmd", v);\n\
  UmlCom.check();\n\
\n\
  "#where" = v;\n", \
	      FALSE)

#define defGetPtr(base, where, oper, type, if_def, end_if, descr) \
  op = base->add_op(#oper, PublicVisibility, type); \
  op->set_Description(" return the " descr); \
  op->set_cpp("${type} *", "", \
	      "  read_if_needed_();\n" \
	      "  return "#where";\n", \
	      FALSE, if_def, end_if); \
  op->set_java("${type}", "", \
	       "  read_if_needed_();\n" \
	       "  return "#where";\n", FALSE)
	      
#define defSetPtr(base, where, oper, type, cmd, if_def, end_if, descr) \
  op = base->add_op(#oper, PublicVisibility, "bool", TRUE); \
  op->set_Description(" set the " descr "\n" \
		      "\n" \
		      " On error return FALSE in C++, produce a RuntimeException in Java"); \
  op->add_param(0, InputDirection, "v", type); \
  op->set_cpp("${type}", "${t0} * ${p0}", \
	       "\
  UmlCom::send_cmd(_identifier, "#cmd", ((UmlBaseItem *) v)->_identifier); \
  if (UmlCom::read_bool()) { \
    "#where" = v; \
    return TRUE; \
  } \
  else \
    return FALSE;", \
	      FALSE, if_def, end_if); \
  op->set_java("void", "${t0} ${p0}", \
	       "\
  UmlCom.send_cmd(identifier_(), OnInstanceCmd."#cmd", v.identifier_());\n\
  UmlCom.check();\n\
\n\
  "#where" = v;\n", \
	      FALSE)

#define defSetRefType(base, where, oper, type, cmd, if_def, end_if, descr) \
  op = base->add_op(#oper, PublicVisibility, "bool", TRUE); \
  op->set_Description(" set the " descr "\n" \
		      "\n" \
		      " On error return FALSE in C++, produce a RuntimeException in Java"); \
  op->add_param(0, InputDirection, "v", type); \
  op->set_cpp("${type}", "const ${t0} & ${p0}", \
	      "  return set_it_("#where", v, "#cmd");", \
	      FALSE, if_def, end_if); \
  op->set_java("void", "${t0} ${p0}", \
	       "\
  UmlCom.send_cmd(identifier_(), OnInstanceCmd."#cmd", v);\n\
  UmlCom.check();\n\
\n\
  "#where" = v;\n", \
	      FALSE)

#define defGetBool(base, where, oper, if_def, end_if, descr) \
  op = base->add_op(#oper, PublicVisibility, "bool"); \
  op->set_Description(" return the " descr); \
  op->set_cpp("${type}", "", \
	      "  read_if_needed_();\n" \
	      "  return "#where";\n", \
	      FALSE, if_def, end_if); \
  op->set_java("${type}", "", \
	       "  read_if_needed_();\n" \
	       "  return "#where";\n", FALSE)
	      
#define defSetBool(base, where, oper, cmd, if_def, end_if, descr) \
  op = base->add_op(#oper, PublicVisibility, "bool", TRUE); \
  op->set_Description(" set the " descr "\n" \
		      "\n" \
		      " On error return FALSE in C++, produce a RuntimeException in Java"); \
  op->add_param(0, InputDirection, "v", "bool"); \
  op->set_cpp("${type}", "${t0} ${p0}", \
	      "  return set_it_("#where", v, "#cmd");", \
	      FALSE, if_def, end_if); \
  op->set_java("void", "${t0} ${p0}", \
	       "\
  UmlCom.send_cmd(identifier_(), OnInstanceCmd."#cmd", (v) ? (byte) 1 : (byte) 0);\n\
  UmlCom.check();\n\
\n\
  "#where" = v;\n", \
	      FALSE)

#define defGetEnum(base, where, oper, type, if_def, end_if, descr) \
  op = base->add_op(#oper, PublicVisibility, type); \
  op->set_Description(" return the " descr); \
  op->set_cpp("${type}", "", \
	      "  read_if_needed_();\n" \
	      "  return "#where";\n", \
	      FALSE, if_def, end_if); \
  op->set_java("${type}", "", \
	       "  read_if_needed_();\n" \
	       "  return "#where";\n", FALSE)
	      
#define defSetEnum(base, where, oper, type, cmd, if_def, end_if, descr) \
  op = base->add_op(#oper, PublicVisibility, "bool", TRUE); \
  op->set_Description(" set the " descr "\n" \
		      "\n" \
		      " On error return FALSE in C++, produce a RuntimeException in Java"); \
  op->add_param(0, InputDirection, "v", type); \
  op->set_cpp("${type}", "${t0} ${p0}", \
	       "\
  UmlCom::send_cmd(_identifier, "#cmd", (char) v); \
  if (UmlCom::read_bool()) { \
    "#where" = v; \
    return TRUE; \
  } \
  else \
    return FALSE;", \
	      FALSE, if_def, end_if); \
  op->set_java("void", "${t0} ${p0}", \
	       "\
  UmlCom.send_cmd(identifier_(), OnInstanceCmd."#cmd", (byte) v.value());\n\
  UmlCom.check();\n\
\n\
  "#where" = v;\n", \
	      FALSE)

#define defSetBoolBitField(base, where, oper, cmd, if_def, end_if, descr) \
  op = base->add_op(#oper, PublicVisibility, "bool", TRUE); \
  op->set_Description(" set the " descr "\n" \
		      "\n" \
		      " On error return FALSE in C++, produce a RuntimeException in Java"); \
  op->add_param(0, InputDirection, "v", "bool"); \
  op->set_cpp("${type}", "${t0} ${p0}", \
	      "  bool vv;\n" \
	      "\n" \
	      "  if (set_it_(vv, v, "#cmd")) {\n" \
	      "    "#where" = v;\n" \
	      "    return TRUE;\n" \
	      "  }\n" \
	      "  else\n" \
	      "    return FALSE;\n", \
	      FALSE, if_def, end_if); \
  op->set_java("void", "${t0} ${p0}", \
	       "\
  UmlCom.send_cmd(identifier_(), OnInstanceCmd."#cmd", (v) ? 1 : 0);\n\
  UmlCom.check();\n\
\n\
  "#where" = v;\n", \
	      FALSE)

//
// add all concerning states
//

void include_umlcom(UmlClass * cl)
{
  UmlArtifact * art = cl->associatedArtifact();
  QCString s = art->cppSource();
  
  s.insert(s.find("${includes}"), "#include \"UmlCom.h\"\n");
  art->set_CppSource(s);
}

void include_umlcom(UmlArtifact * art)
{
  QCString s = art->cppSource();
  
  s.insert(s.find("${includes}"), "#include \"UmlCom.h\"\n");
  art->set_CppSource(s);
}

UmlClass * add_state_behavior(UmlClassView * base_class_view,
			      UmlDeploymentView * base_depl_view)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlClass * state_behavior = 
    UmlClass::made(base_class_view, base_depl_view, "StateBehavior");
  
  include_umlcom(state_behavior);
  
  state_behavior->add_attribute("on_entry", PublicVisibility, "string", 0, 0);
  state_behavior->add_attribute("on_exit", PublicVisibility, "string", 0, 0);
  state_behavior->add_attribute("do_activity", PublicVisibility, "string", 0, 0);
  
  UmlOperation * op;
  
  op = state_behavior->add_op("read", PublicVisibility, "void");
  op->set_cpp("${type}", "", "\
  on_entry = UmlCom::read_string();\n\
  on_exit = UmlCom::read_string();\n\
  do_activity = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "", "\
  on_entry = UmlCom.read_string();\n\
  on_exit = UmlCom.read_string();\n\
  do_activity = UmlCom.read_string();\n",
	       FALSE);
  
  op = state_behavior->add_op("unload", PublicVisibility, "void");
  op->set_cpp("${type}", "", "\
  on_entry = 0;\n\
  on_exit = 0;\n\
  do_activity = 0;\n",
	      FALSE, 0, 0);
  op->set_JavaDecl("");
  
  UmlCom::set_user_id(uid);
  
  return state_behavior;
}

UmlClass * add_state_diagram(UmlClassView * base_class_view, UmlClassView * user_class_view,
			     UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
			     UmlClass * base_item, UmlClass * user_stateitem, UmlClass * user_state)
{
  UmlClass * user_diagram = UmlClass::get("UmlDiagram", 0);
  UmlClass * base_statediagram;
  UmlClass * user_statediagram;
  
  user_diagram->made(base_class_view, user_class_view,
		     base_depl_view, user_depl_view,
		     base_item, "StateDiagram",
		     base_statediagram, user_statediagram,
		     user_stateitem);
  base_statediagram->add_default_base_op(user_diagram, user_statediagram, user_state,
					 "state diagram", "aStateDiagram");
  user_statediagram->add_constr(base_statediagram, PublicVisibility);
  
  user_statediagram->set_Description(" This class manages 'class diagrams', notes that the class 'UmlDiagram'\n"
				     " is a mother class of all the diagrams, allowing to generalize their\n"
				     " management\n"
				     "\n"
				     " You can modify it as you want (except the constructor)");
  return user_statediagram;
}

void add_assoc_diag_ops(UmlClass * base, UmlClass * diag)
{
  UmlOperation * op;
  
  op = base->add_op("associatedDiagram", PublicVisibility, diag);
  
  op->set_cpp("${type} *", "",
	      "  read_if_needed_();\n\n"
	      "  return _assoc_diagram;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	      "  read_if_needed_();\n\n"
	      "  return _assoc_diagram;\n",
	      FALSE);
  op->set_Description(" returns the optional associated diagram");
  
  //
  
  op = base->add_op("set_AssociatedDiagram", PublicVisibility, "bool", TRUE);
  op->add_param(0, InputDirection, "d", diag);
  
  op->set_cpp("${type}", "${t0} * ${p0}",
	      "  UmlCom::send_cmd(_identifier, setAssocDiagramCmd, ((UmlBaseItem *) d)->_identifier);\n"
	      "  if (UmlCom::read_bool()) {\n"
	      "    _assoc_diagram = d;\n"
	      "    return TRUE;\n"
	      "  }\n"
	      "  else\n"
	      "    return FALSE;\n",
	      FALSE, 0, 0);
  op->set_java("void", "${t0} ${p0}",
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.setAssocDiagramCmd, d.identifier_());\n"
	       "  UmlCom.check();\n"
	       "  _assoc_diagram = d;\n",
	      FALSE);
  op->set_Description(" sets the associated diagram, arg may be null to unset it\n\n"
		      " On error return FALSE in C++, produce a RuntimeException in Java");
}

UmlClass * add_state(UmlClassView * base_class_view, UmlClassView * user_class_view,
		     UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		     UmlClass * base_item, UmlClass * user_item, UmlClass * user_stateitem)
{
  UmlClass * base_state;
  UmlClass * user_state;
  UmlOperation * op;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "State", base_state, user_state,
		  user_stateitem);
  base_state->add_default_base_op(user_item, user_state,
				  UmlClass::get("UmlClassView", 0),
				  "state", "aState");
  user_state->add_constr(base_state, PublicVisibility);
  
  UmlClass * user_statediagram = 
    add_state_diagram(base_class_view, user_class_view,
		      base_depl_view, user_depl_view,
		      base_item, user_stateitem, user_state);
  
    
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
    
  include_umlcom(base_state);
  
  defGetField(base_state, _uml, entryBehavior, on_entry, "string", 0, 0,
	      "entry behavior in OCL");
  defSetField(base_state, _uml, set_EntryBehavior, on_entry, "str", setUmlEntryBehaviorCmd, 0, 0,
	      "entry behavior in OCL");
  defGetField(base_state, _uml, exitBehavior, on_exit, "string", 0, 0,
	      "exit behavior in OCL");
  defSetField(base_state, _uml, set_ExitBehavior, on_exit, "str", setUmlExitBehaviorCmd, 0, 0,
	      "exit behavior in OCL");
  defGetField(base_state, _uml, doActivity, do_activity, "string", 0, 0, 
	      "activity in OCL");
  defSetField(base_state, _uml, set_DoActivity, do_activity, "str", setUmlActivityCmd, 0, 0,
	      "activity in OCL");

  defGetField(base_state, _cpp, cppEntryBehavior, on_entry, "string", "WITHCPP", 0,
	      "entry behavior in C++");
  defSetField(base_state, _cpp, set_CppEntryBehavior, on_entry, "str", setCppEntryBehaviorCmd, 0, 0,
	      "entry behavior in C++");
  defGetField(base_state, _cpp, cppExitBehavior, on_exit, "string", 0, 0,
	      "exit behavior in C++");
  defSetField(base_state, _cpp, set_CppExitBehavior, on_exit, "str", setCppExitBehaviorCmd, 0, 0,
	      "exit behavior in C++");
  defGetField(base_state, _cpp, cppDoActivity, do_activity, "string", 0, 0,
	      "activity in C++");
  defSetField(base_state, _cpp, set_CppDoActivity, do_activity, "str", setCppActivityCmd, 0, "endif",
	      "activity in C++");

  defGetField(base_state, _java, javaEntryBehavior, on_entry, "string", "WITHJAVA", 0,
	      "entry behavior in Java");
  defSetField(base_state, _java, set_JavaEntryBehavior, on_entry, "str", setJavaEntryBehaviorCmd, 0, 0,
	      "entry behavior in Java");
  defGetField(base_state, _java, javaExitBehavior, on_exit, "string", 0, 0,
	      "exit behavior in Java");
  defSetField(base_state, _java, set_JavaExitBehavior, on_exit, "str", setJavaExitBehaviorCmd, 0, 0,
	      "exit behavior in Java");
  defGetField(base_state, _java, javaDoActivity, do_activity, "string", 0, 0,
	      "activity in Java");
  defSetField(base_state, _java, set_JavaDoActivity, do_activity, "str", setJavaActivityCmd, 0, "endif",
	      "activity in Java");

  add_assoc_diag_ops(base_state, user_statediagram);
  include_umlcom(base_state);
  
  op = base_state->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _uml.unload();\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp.unload();\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java.unload();\n"
	      "#endif\n"
	      "  UmlBaseItem::unload(rec, del);\n",
	      FALSE, 0, 0);
  QCString s = op->cppDecl();
  
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _uml = null;\n"
	       "  _cpp = null;\n"
	       "  _java = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE. \n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
  
  UmlClass * state_behavior =
    add_state_behavior(base_class_view, base_depl_view);
  
  base_state->add_relation(aDirectionalAggregationByValue,
			   "_uml", PrivateVisibility,
			   state_behavior, 0, 0);
  base_state->add_relation(aDirectionalAggregationByValue,
			   "_cpp", PrivateVisibility,
			   state_behavior, "WITHCPP", "endif");
  base_state->add_relation(aDirectionalAggregationByValue,
			   "_java", PrivateVisibility,
			   state_behavior, "WITHJAVA", "endif");
  base_state->add_relation(aDirectionalAssociation,
			   "_assoc_diagram", PrivateVisibility,
			   user_statediagram, 0, 0);
  
  op = base_state->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _assoc_diagram = (UmlStateDiagram *) UmlBaseItem::read_();\n"
	      "  UmlBaseItem::read_uml_();\n"
	      "  _uml.read();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  _assoc_diagram = (UmlStateDiagram) UmlBaseItem.read_();\n"
	       "  super.read_uml_();\n"
	       "  _uml = new StateBehavior();\n"
	       "  _uml.read();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_state->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _cpp.read();\n", FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "", "  _cpp = new StateBehavior();\n  _cpp.read();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_state->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _java.read();\n", FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "", "  _java = new StateBehavior();\n  _java.read();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  return user_state;
}

UmlClass * add_transition_behavior(UmlClassView * base_class_view,
				   UmlDeploymentView * base_depl_view)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlClass * transition_behavior = 
    UmlClass::made(base_class_view, base_depl_view, "TransitionBehavior");
  
  include_umlcom(transition_behavior);
  
  transition_behavior->add_attribute("trigger", PublicVisibility, "string", 0, 0);
  transition_behavior->add_attribute("guard", PublicVisibility, "string", 0, 0);
  transition_behavior->add_attribute("activity", PublicVisibility, "string", 0, 0);
  
  UmlOperation * op;
  
  op = transition_behavior->add_op("read", PublicVisibility, "void");
  op->set_cpp("${type}", "", "\
  trigger = UmlCom::read_string();\n\
  guard = UmlCom::read_string();\n\
  activity = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "", "\
  trigger = UmlCom.read_string();\n\
  guard = UmlCom.read_string();\n\
  activity = UmlCom.read_string();\n",
	       FALSE);
  
  op = transition_behavior->add_op("unload", PublicVisibility, "void");
  op->set_cpp("${type}", "", "\
  trigger = 0;\n\
  guard = 0;\n\
  activity = 0;\n",
	      FALSE, 0, 0);
  op->set_JavaDecl("");
  
  UmlCom::set_user_id(uid);
  
  return transition_behavior;
}

void add_transition(UmlClassView * base_class_view, UmlClassView * user_class_view,
		    UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		    UmlClass * base_item, UmlClass * user_item, UmlClass * user_stateitem)
{
  UmlClass * base_transition;
  UmlClass * user_transition;
  UmlOperation * op;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "Transition", base_transition,
		  user_transition, user_stateitem);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  include_umlcom(base_transition);
  
  base_transition->add_kind("aTransition");
  
  op = base_transition->add_op("create", PublicVisibility, user_transition, TRUE);
  op->set_isClassMember(TRUE);
  op->add_param(0, InputOutputDirection, "start", user_item);
  op->add_param(1, InputOutputDirection, "end", user_item);
  op->set_cpp("${type} *", 
	      "${t0} * ${p0}, ${t1} * ${p1}",
	      "  UmlCom::send_cmd(start->_identifier, createCmd, aTransition,\n"
	      "\t\t   end->_identifier);\n"
	      "  UmlTransition * result = (UmlTransition *) UmlBaseItem::read_();\n"
	      "\n"
	      "  if (result != 0)\n"
	      "    start->reread_children_if_needed_();\n"
	      "  return result;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  UmlCom.send_cmd(start.identifier_(), OnInstanceCmd.createCmd, anItemKind.aTransition,\n"
	       "\t\t   end.identifier_());\n"
	       "  UmlTransition result = (UmlTransition) UmlBaseItem.read_();\n"
	       "  \n"
	       "  if (result != null)\n"
	       "    start.reread_children_if_needed_();\n"
	       "  else\n"
	       "    throw new RuntimeException(\"Cannot create the Transition\");\n"
	       "  return result;\n",
	      FALSE);
  op->set_Description(" Returns a new Transition from 'start' to 'end'\n\
\n\
 In case it cannot be created ('parent' cannot contain it etc ...) return 0\n\
  in C++ and produce a RuntimeException in Java\n");
  
  op = base_transition->add_op("target", PublicVisibility, user_item);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" returns the 'end' object (the 'start' object is the parent of the transition) no set !");
  op->set_cpp("${type} *", "",
	      "  read_if_needed_();\n"
	      "  return _target;\n",
	      FALSE, 0, 0); \
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "  return _target;\n", FALSE);
  
  defGetField(base_transition, _uml, trigger, trigger, "string", 0, 0,
	      "trigger in OCL");
  defSetField(base_transition, _uml, set_Trigger, trigger, "str", setUmlTriggerCmd, 0, 0,
	      "trigger in OCL");
  defGetField(base_transition, _uml, guard, guard, "string", 0, 0,
	      "guard in OCL");
  defSetField(base_transition, _uml, set_Guard, guard, "str", setUmlGuardCmd, 0, 0,
	      "guard in OCL");
  defGetField(base_transition, _uml, activity, activity, "string", 0, 0, 
	      "activity in OCL");
  defSetField(base_transition, _uml, set_Activity, activity, "str", setUmlActivityCmd, 0, 0,
	      "activity in OCL");

  defGetField(base_transition, _cpp, cppTrigger, trigger, "string", "WITHCPP", 0,
	      "trigger in C++");
  defSetField(base_transition, _cpp, set_CppTrigger, trigger, "str", setCppTriggerCmd, 0, 0,
         "trigger in C++");
  defGetField(base_transition, _cpp, cppGuard, guard, "string", 0, 0,
	      "guard in C++");
  defSetField(base_transition, _cpp, set_CppGuard, guard, "str", setCppGuardCmd, 0, 0,
	      "guard in C++");
  defGetField(base_transition, _cpp, cppActivity, activity, "string", 0, 0,
	      "activity in C++");
  defSetField(base_transition, _cpp, set_CppActivity, activity, "str", setCppActivityCmd, 0, "endif",
	      "activity in C++");

  defGetField(base_transition, _java, javaTrigger, trigger, "string", "WITHJAVA", 0,
	      "trigger in Java");
  defSetField(base_transition, _java, set_JavaTrigger, trigger, "str", setJavaTriggerCmd, 0, 0,
	      "trigger in Java");
  defGetField(base_transition, _java, javaGuard, guard, "string", 0, 0,
	      "guard in Java");
  defSetField(base_transition, _java, set_JavaGuard, guard, "str", setJavaGuardCmd, 0, 0,
	      "guard in Java");
  defGetField(base_transition, _java, javaActivity, activity, "string", 0, 0,
	      "activity in Java");
  defSetField(base_transition, _java, set_JavaActivity, activity, "str", setJavaActivityCmd, 0, "endif",
	      "activity in Java");
  
  op = base_transition->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _uml.unload();\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp.unload();\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java.unload();\n"
	      "#endif\n"
	      "  UmlBaseItem::unload(rec, del);\n",
	      FALSE, 0, 0);

  QCString s = op->cppDecl();
  
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _uml = null;\n"
	       "  _cpp = null;\n"
	       "  _java = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE. \n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
  
  base_transition->add_relation(aDirectionalAssociation,
				"_target", PrivateVisibility,
				user_item, 0, 0);
			  
  UmlClass * transition_behavior =
    add_transition_behavior(base_class_view, base_depl_view);
  
  base_transition->add_relation(aDirectionalAggregationByValue,
				"_uml", PrivateVisibility,
				transition_behavior, 0, 0);
  base_transition->add_relation(aDirectionalAggregationByValue,
				"_cpp", PrivateVisibility,
				transition_behavior, "WITHCPP", "endif");
  base_transition->add_relation(aDirectionalAggregationByValue,
				"_java", PrivateVisibility,
				transition_behavior, "WITHJAVA", "endif");
  
  op = base_transition->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseItem::read_uml_();\n"
	      "  _target = UmlBaseItem::read_();\n"
	      "  _uml.read();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _target = UmlBaseItem.read_();\n"
	       "  _uml = new TransitionBehavior();\n"
	       "  _uml.read();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_transition->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _cpp.read();\n", FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "", "  _cpp = new TransitionBehavior();\n  _cpp.read();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_transition->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _java.read();\n", FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "", "  _java = new TransitionBehavior();\n  _java.read();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  base_transition->add_constr(user_item, ProtectedVisibility);
  
  UmlCom::set_user_id(uid);
  
  op = user_transition->add_op("UmlTransition", PublicVisibility,
			       (UmlClass *) 0, FALSE);
  op->add_param(0, InputDirection, "id", "item_id");
  op->add_param(1, InputDirection, "n", "string");
  op->set_cpp(": UmlBaseTransition(id, n)",
	      "${t0} ${p0}, const ${t1} & ${p1}",
	      "", TRUE, 0, 0);
  op->set_java("", "${t0} ${p0}, ${t1} ${p1}",
	       "  super(id, n);\n", TRUE);
}

void add_region(UmlClassView * base_class_view, UmlClassView * user_class_view,
		UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		UmlClass * base_item, UmlClass * user_item,
		UmlClass * user_stateitem, UmlClass * user_state)
{
  UmlClass * base_region;
  UmlClass * user_region;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "Region", base_region, user_region, user_stateitem);
  base_region->add_default_base_op(user_item, user_region, user_state, "region", "aRegion");
  user_region->add_constr(base_region, PublicVisibility);
}

void add_stateaction(UmlClassView * base_class_view, UmlClassView * user_class_view,
		     UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		     UmlClass * base_item, UmlClass * user_item,
		     UmlClass * user_stateitem, UmlClass * user_state)
{
  UmlClass * base_stateaction;
  UmlClass * user_stateaction;
  UmlOperation * op;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "StateAction", base_stateaction,
		  user_stateaction, user_stateitem);

  user_stateaction->add_constr(base_stateaction, PublicVisibility, TRUE);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  include_umlcom(base_stateaction);
  
  base_stateaction->add_default_base_op(user_item, user_stateaction, user_state,
					"state action", "aStateAction", TRUE);
    
  defGet(base_stateaction, _uml, expression, "string", 0, 0,
         "expression in OCL");
  defSet(base_stateaction, _uml, set_Expression, "str", setDefCmd, 0, 0,
         "expression in OCL");
  defGet(base_stateaction, _cpp, cppExpression, "string", "WITHCPP", 0,
         "expression in C++");
  defSet(base_stateaction, _cpp, set_CppExpression, "str", setCppDeclCmd, 0, "endif",
         "expression in C++");
  defGet(base_stateaction, _java, javaExpression, "string", "WITHJAVA", 0,
         "expression in Java");
  defSet(base_stateaction, _java, set_JavaExpression, "str", setJavaDeclCmd, 0, "endif",
         "expression in Java");

  op = base_stateaction->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _uml = 0;\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp = 0;\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java = 0;\n"
	      "#endif\n"
	      "  UmlBaseItem::unload(rec, del);\n",
	      FALSE, 0, 0);
  
  QCString s = op->cppDecl();
  
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _uml = null;\n"
	       "  _cpp = null;\n"
	       "  _java = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE. \n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
  
  base_stateaction->add_attribute("_uml", PrivateVisibility, "string",
				  0, 0);
  base_stateaction->add_attribute("_cpp", PrivateVisibility, "string",
				  "WITHCPP", "endif");
  base_stateaction->add_attribute("_java", PrivateVisibility, "string",
				  "WITHJAVA", "endif");
  
  op = base_stateaction->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseItem::read_uml_();\n"
	      "  _uml = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _uml = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_stateaction->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _cpp = UmlCom::read_string();\n", FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "", "  _cpp  = UmlCom.read_string();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_stateaction->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _java = UmlCom::read_string();\n", FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "", "  _java = UmlCom.read_string();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
}

  
void add_pseudostates(UmlClassView * base_class_view, UmlClassView * user_class_view,
		      UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		      UmlClass * base_item, UmlClass * user_item, UmlClass * user_stateitem)
{
  UmlClass * user_pseudostate =
    UmlClass::made(user_class_view, user_depl_view, "UmlPseudoState");
  
  user_pseudostate->set_isAbstract(TRUE);
  user_pseudostate->add_constr(user_item, PublicVisibility);
    
  UmlRelation * rel;
  
  if ((rel = UmlBaseRelation::create(aGeneralisation, user_pseudostate, user_stateitem)) == 0) {
    QCString msg = user_pseudostate->name() + " can't inherit " + user_stateitem->name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  else {
    rel->set_CppDecl("${type}");
    rel->set_JavaDecl("${type}");
  }
  
  if ((rel = UmlBaseRelation::create(aGeneralisation, user_pseudostate, user_item)) == 0) {
    QCString msg = user_pseudostate->name() + " can't inherit " + user_item->name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  else {
    rel->set_CppDecl("${type}");
    rel->set_JavaDecl("${type}");
  }
    
  const struct {
    const char * name;
    const char * what;
    const char * kind;
  } ps[] = {
    { "InitialPseudoState", "initial pseudo state", "anInitialPseudoState" },
    { "EntryPointPseudoState", "entry point pseudo state", "anEntryPointPseudoState" },
    { "FinalState", "final state", "aFinalState" },
    { "TerminatePseudoState", "terminate pseudo state", "aTerminatePseudoState" },
    { "ExitPointPseudoState", "exit point pseudo state", "anExitPointPseudoState" },
    { "DeepHistoryPseudoState", "deep history pseudo state", "aDeepHistoryPseudoState" },
    { "ShallowHistoryPseudoState", "shallow history pseudo state", "aShallowHistoryPseudoState" },
    { "JunctionPseudoState", "junction pseudo state", "aJunctionPseudoState" },
    { "ChoicePseudoState", "choice pseudo state", "aChoicePseudoState" },
    { "ForkPseudoState", "fork pseudo state", "aForkPseudoState" },
    { "JoinPseudoState", "join pseudo state", "aJoinPseudoState" }
  };
#define N_PS (sizeof(ps)/sizeof(ps[0]))
  UmlClass * base_ps[N_PS];
  UmlClass * user_ps[N_PS];
  int i;
  
  for (i = 0; i != N_PS; i += 1) {
    user_pseudostate->made(base_class_view, user_class_view,
			   base_depl_view, user_depl_view,
			   base_item, ps[i].name, base_ps[i], user_ps[i]);
    
    bool unnamed = 
      (strcmp(ps[i].kind, "anEntryPointPseudoState") != 0) &&
      (strcmp(ps[i].kind, "anExitPointPseudoState") != 0);
    
    base_ps[i]->add_default_base_op(user_pseudostate, user_ps[i], user_item,
				    ps[i].what, ps[i].kind, unnamed);
    user_ps[i]->add_constr(base_ps[i], PublicVisibility, unnamed);
  }
}

void add_state_item_kind()
{
  // already root
  static const char * const kinds[] = {
    "aState", 
    "aTransition", 
    "aRegion", 
    "aStateDiagram", 
    "aStateAction", 
    "anInitialPseudoState", 
    "anEntryPointPseudoState", 
    "aFinalState", 
    "aTerminatePseudoState", 
    "anExitPointPseudoState", 
    "aDeepHistoryPseudoState", 
    "aShallowHistoryPseudoState", 
    "aJunctionPseudoState", 
    "aChoicePseudoState", 
    "aForkPseudoState", 
    "aJoinPseudoState", 
  };
  UmlClass * itkind = UmlClass::get("anItemKind", 0);
  
  for (int i = 0; i != sizeof(kinds)/sizeof(kinds[0]); i += 1)
    itkind->add_enum_item(kinds[i]);
}

void add_state_on_instance_cmd()
{
  // already root
  static const char * const cmds[] = {
  "setMarkedCmd",
  
  "setUmlEntryBehaviorCmd",
  "setUmlExitBehaviorCmd",
  "setUmlActivityCmd",
  "setCppEntryBehaviorCmd",
  "setCppExitBehaviorCmd",
  "setCppActivityCmd",
  "setJavaEntryBehaviorCmd",
  "setJavaExitBehaviorCmd",
  "setJavaActivityCmd",
  "setUmlTriggerCmd",
  "setUmlGuardCmd",
  "setCppTriggerCmd",
  "setCppGuardCmd",
  "setJavaGuardCmd",
  "setJavaTriggerCmd",
  "setDefCmd"
  };
  UmlClass * itcmd = UmlClass::get("OnInstanceCmd", 0);
  QCString cpp = CppSettings::enumItemDecl();
  QCString java = JavaSettings::enumPatternItemDecl();
  QCString m = "add enum item OnInstanceCmd::";
  
  for (int i = 0; i != sizeof(cmds)/sizeof(cmds[0]); i += 1) {
    UmlAttribute * at;
    
    if ((at = UmlBaseAttribute::create(itcmd, cmds[i])) == 0) {
      // setMarkedCmd may alreadu exist
      if (i != 0) {
	QCString msg = "cannot add enum item '" + QCString(cmds[i]) +
	  "' in 'OnInstanceCmd'<br>\n";
	
	UmlCom::trace(msg);
	throw 0;
      }
    }
    else {
      UmlCom::trace(m + cmds[i] + "<br>\n");
      at->set_CppDecl(cpp);
      at->set_JavaDecl(java);
    }
  }
}

void baseitem_read_state(UmlClass * base_item) {
  // update read_()'s body
  UmlOperation * op = base_item->get_operation("read_");
  
  if (op != 0) {
    QCString body;
    
    body = op->cppBody();
    body.insert(body.findRev("default:"),
		"case aState:\n\
      return new UmlState(id, name);\n\
    case aTransition:\n\
      return new UmlTransition(id, name);\n\
    case aRegion:\n\
      return new UmlRegion(id, name);\n\
    case aStateDiagram:\n\
      return new UmlStateDiagram(id, name);\n\
    case aStateAction:\n\
      return new UmlStateAction(id);\n\
    case anInitialPseudoState:\n\
      return new UmlInitialPseudoState(id);\n\
    case anEntryPointPseudoState:\n\
      return new UmlEntryPointPseudoState(id, name);\n\
    case aFinalState:\n\
      return new UmlFinalState(id);\n\
    case aTerminatePseudoState:\n\
      return new UmlTerminatePseudoState(id);\n\
    case anExitPointPseudoState:\n\
      return new UmlExitPointPseudoState(id, name);\n\
    case aDeepHistoryPseudoState:\n\
      return new UmlDeepHistoryPseudoState(id);\n\
    case aShallowHistoryPseudoState:\n\
      return new UmlShallowHistoryPseudoState(id);\n\
    case aJunctionPseudoState:\n\
      return new UmlJunctionPseudoState(id);\n\
    case aChoicePseudoState:\n\
      return new UmlChoicePseudoState(id);\n\
    case aForkPseudoState:\n\
      return new UmlForkPseudoState(id);\n\
    case aJoinPseudoState:\n\
      return new UmlJoinPseudoState(id);\n\
    ");
    op->set_CppBody(body);
      
    body = op->javaBody();
    body.insert(body.findRev("default:"),
		"case anItemKind._aState:\n\
      return new UmlState(id, name);\n\
    case anItemKind._aTransition:\n\
      return new UmlTransition(id, name);\n\
    case anItemKind._aRegion:\n\
      return new UmlRegion(id, name);\n\
    case anItemKind._aStateDiagram:\n\
      return new UmlStateDiagram(id, name);\n\
    case anItemKind._aStateAction:\n\
      return new UmlStateAction(id);\n\
    case anItemKind._anInitialPseudoState:\n\
      return new UmlInitialPseudoState(id);\n\
    case anItemKind._anEntryPointPseudoState:\n\
      return new UmlEntryPointPseudoState(id, name);\n\
    case anItemKind._aFinalState:\n\
      return new UmlFinalState(id);\n\
    case anItemKind._aTerminatePseudoState:\n\
      return new UmlTerminatePseudoState(id);\n\
    case anItemKind._anExitPointPseudoState:\n\
      return new UmlExitPointPseudoState(id, name);\n\
    case anItemKind._aDeepHistoryPseudoState:\n\
      return new UmlDeepHistoryPseudoState(id);\n\
    case anItemKind._aShallowHistoryPseudoState:\n\
      return new UmlShallowHistoryPseudoState(id);\n\
    case anItemKind._aJunctionPseudoState:\n\
      return new UmlJunctionPseudoState(id);\n\
    case anItemKind._aChoicePseudoState:\n\
      return new UmlChoicePseudoState(id);\n\
    case anItemKind._aForkPseudoState:\n\
      return new UmlForkPseudoState(id);\n\
    case anItemKind._aJoinPseudoState:\n\
      return new UmlJoinPseudoState(id);\n\
    ");
    op->set_JavaBody(body);
  }
  
  // update artifact
  UmlArtifact * art = base_item->associatedArtifact();
  QCString s;
  
  s = art->cppSource();
  s.insert(s.find("#include \"MiscGlobalCmd.h\""),
	   "#include \"UmlState.h\"\n"
	   "#include \"UmlTransition.h\"\n"
	   "#include \"UmlRegion.h\"\n"
	   "#include \"UmlStateDiagram.h\"\n"
	   "#include \"UmlStateAction.h\"\n"
	   "#include \"UmlInitialPseudoState.h\"\n"
	   "#include \"UmlEntryPointPseudoState.h\"\n"
	   "#include \"UmlFinalState.h\"\n"
	   "#include \"UmlTerminatePseudoState.h\"\n"
	   "#include \"UmlExitPointPseudoState.h\"\n"
	   "#include \"UmlDeepHistoryPseudoState.h\"\n"
	   "#include \"UmlShallowHistoryPseudoState.h\"\n"
	   "#include \"UmlJunctionPseudoState.h\"\n"
	   "#include \"UmlChoicePseudoState.h\"\n"
	   "#include \"UmlForkPseudoState.h\"\n"
	   "#include \"UmlJoinPseudoState.h\"\n");
  art->set_CppSource(s);
}

void update_uml_com()
{
  // already root
  UmlClass * uml_com = UmlClass::get("UmlCom", 0);
  UmlOperation * op =
    uml_com->add_op("send_cmd", PublicVisibility, "void", FALSE);
  
  op->add_param(0, InputDirection, "id", "item_id");
  op->add_param(1, InputDirection, "cmd", UmlClass::get("OnInstanceCmd", 0));
  op->add_param(2, InputOutputDirection, "arg", UmlClass::get("anItemKind", 0));
  op->add_param(3, InputOutputDirection, "id2", "item_id");

  op->set_Description("internal, do NOT use it\n");
  op->set_isClassMember(TRUE);
  
  op->set_cpp("${type}", 
	      "const ${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}, const ${t3} ${p3}",
	      "\
#ifdef TRACE\n\
  cout << \"UmlCom::send_cmd(id, \" << cmd << \", \" << arg << \", \" << id2 << \")\\n\";\n\
#endif\n\
  \n\
  write_char(onInstanceCmd);\n\
  write_id(id);\n\
  write_char(cmd);\n\
  write_char(arg);\n\
  write_id(id2);\n\
  flush();\n",
	      FALSE, 0, 0);

  op->set_java("${type}",
	       "${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}, ${t3} ${p3}",
	       "\
  //cout << \"UmlCom.send_cmd(id, \" << cmd << \", \" << arg << \", \" << id2 << \")\\n\";\n\
  \n\
  write_char((byte) CmdFamily._onInstanceCmd);\n\
  write_id(id);\n\
  write_char((byte) cmd.value());\n\
  write_char((byte) arg.value());\n\
  write_id(id2);\n\
  flush();\n",
	       FALSE);
}

void upgrade_states(UmlClass * base_item,
		    UmlClass * user_item)
{
  UmlClassView * base_class_view = (UmlClassView *) base_item->parent();
  UmlClassView * user_class_view = (UmlClassView *) user_item->parent();
  UmlDeploymentView * base_depl_view = (UmlDeploymentView *)
    base_item->associatedArtifact()->parent();
  UmlDeploymentView * user_depl_view = (UmlDeploymentView *)
    user_item->associatedArtifact()->parent();
  
  UmlCom::trace("State<br>\n");
  
  UmlClass * user_stateitem = 
    UmlClass::made(user_class_view, user_depl_view, "UmlStateItem", TRUE);
  UmlClass * user_state =
    add_state(base_class_view, user_class_view,
	      base_depl_view, user_depl_view,
	      base_item, user_item, user_stateitem);
  
  UmlCom::trace("Transition<br>\n");
  
  add_transition(base_class_view, user_class_view,
		 base_depl_view, user_depl_view,
		 base_item, user_item, user_stateitem);
  
  UmlCom::trace("Region<br>\n");
  
  add_region(base_class_view, user_class_view,
	     base_depl_view, user_depl_view,
	     base_item, user_item, user_stateitem, user_state);
  
  UmlCom::trace("Action<br>\n");
  
  add_stateaction(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, user_item, user_stateitem, user_state);

  UmlCom::trace("Pseudo States<br>\n");
  
  add_pseudostates(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, user_item, user_stateitem);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("update anItemKind<br>\n");
  
  add_state_item_kind();
  
  UmlCom::trace("update cmd list<br>\n");
  
  add_state_on_instance_cmd();
  
  UmlCom::trace("update item read<br>\n");
  
  baseitem_read_state(base_item);
  
  UmlCom::trace("update com<br>\n");
  
  update_uml_com();
  
  UmlCom::set_user_id(uid);
}

void base_state_include_umlcom()
{
  UmlArtifact * art = UmlClass::get("UmlBaseState", 0)->associatedArtifact();
  QCString s = art->cppSource();
  
  if (s.find("#include \"UmlCom.h\"\n") == -1) {
    s.insert(s.find("${includes}"), "#include \"UmlCom.h\"\n");
    art->set_CppSource(s);
  }
}

//
// define set_Name on UmlBaseClass to rename 
// constructors & destructor

bool class_set_name_defined(UmlClass * base_class)
{
  return (base_class->get_operation("set_Name") != 0);
}

void upgrade_rename_class(UmlClass * base_item, 
			  UmlClass * base_class)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlOperation * set_item_name
    = base_item->get_operation("set_Name");
      
  set_item_name->set_isCppVirtual(TRUE);
  
  UmlOperation * set_class_name = 
    base_class->add_op("set_Name", PublicVisibility, "bool", TRUE);
  
  set_class_name->set_isCppVirtual(TRUE);
  set_class_name->add_param(0, InputDirection, "s", "string");
  set_class_name->set_Description(set_item_name->description());
  
  set_class_name->set_CppDecl(set_item_name->cppDecl());
  set_class_name->set_CppDef(set_item_name->cppDef());
  set_class_name->set_CppBody("\
  if (!UmlBaseItem::set_Name(s))\n\
    return FALSE;\n\
\n\
  const QVector<UmlItem> ch = children();\n\
  QCString destr = \"~\" + name();\n\
\n\
  for (unsigned i = 0; i != ch.size(); i += 1) {\n\
    if (ch[i]->kind() == anOperation) {\n\
      if (ch[i]->name() == name())\n\
	ch[i]->set_Name(s);\n\
      else if (ch[i]->name() == destr)\n\
	ch[i]->set_Name(\"~\" + s);\n\
    }\n\
  }\n\
\n\
  return TRUE;\n");

  set_class_name->set_JavaDecl(set_item_name->javaDecl());
  set_class_name->set_JavaBody("\
  super.set_Name(s);\n\
\n\
  UmlItem[] ch = children();\n\
  String destr = \"~\" + name();\n\
\n\
  for (int i = 0; i != ch.length; i += 1) {\n\
    if (ch[i].kind() == anItemKind.anOperation) {\n\
      if (ch[i].name() == name())\n\
	ch[i].set_Name(s);\n\
      else if (ch[i].name() == destr)\n\
	ch[i].set_Name(\"~\" + s);\n\
    }\n\
  }\n");
  
  UmlCom::set_user_id(uid);
}


//
// add new settings linked to the default description and default C++ return forms
// add UmlBaseItem::moveAfter
//

bool item_move_after_defined(UmlClass * base_item)
{
  return (base_item->get_operation("moveAfter") != 0);
}

void add_move_after(UmlClass * base_item, UmlClass * uml_item)
{
  UmlOperation * move_after = 
    base_item->add_op("moveAfter", PublicVisibility, "bool", TRUE);
  
  move_after->add_param(0, InputDirection, "x", uml_item);
  move_after->set_Description(" If the parameter is null, move the current item to be\n"
			      " the first child of its parent. Else the current item and\n"
			      " the parameter must have the same parent, the current item\n"
			      " is moved to be just after the parameter.\n"
			      " On error return FALSE in C++, produce a RuntimeException in Java");
  move_after->set_cpp("${type}", "const ${t0} * ${p0}",
		      "  UmlCom::send_cmd(_identifier, moveAfterCmd, (x != 0) ? ((UmlBaseItem *) x)->_identifier : 0);\n"
		      "  if (UmlCom::read_bool()) {\n"
		      "    ((UmlBaseItem *) parent())->reread_children_if_needed_();\n"
		      "    return TRUE;\n"
		      "  }\n"
		      "  else\n"
		      "    return FALSE;\n",
		      FALSE, 0, 0);
  move_after->set_java("void", "${t0} ${p0}", \
		       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.moveAfterCmd, \n"
		       "                  (x != null) ? x.identifier_() : 0);\n"
		       "  UmlCom.check();\n"
		       "  parent().reread_children_if_needed_();\n",
		       FALSE);
  
  // move it (!) after properties
  
  move_after->moveAfter(base_item->get_operation("properties"));
}

void upgrade_OnInstanceCmd_UmlSettingsCmd_CppSettingsCmd()
{
  // upgrade OnInstanceCmd
  
  UmlClass * cl = UmlClass::get("OnInstanceCmd", 0);
  
  cl->add_enum_item("moveAfterCmd");

  // update UmlSettingsCmd enum
  
  cl = UmlClass::get("UmlSettingsCmd", 0);
  
  cl->add_enum_item("setDefaultArtifactDescriptionCmd")->set_DefaultValue("= 50");
  cl->add_enum_item("setDefaultClassDescriptionCmd");
  cl->add_enum_item("setDefaultOperationDescriptionCmd");
  cl->add_enum_item("setDefaultAttributeDescriptionCmd");
  cl->add_enum_item("setDefaultRelationDescriptionCmd");
  
  // update CppSettingsCmd enum
  
  cl = UmlClass::get("CppSettingsCmd", 0);
  
  cl->add_enum_item("setCppEnumReturnCmd");
  cl->add_enum_item("setCppReturnCmd");
 }

void upgrade_UmlSettings()
{
    
  UmlClass * cl = UmlClass::get("UmlSettings", 0);
  
  cl->set_isAbstract(FALSE);
  cl->set_Description(" This class manages settings not linked with a language, configured through\n"
		      " the 'Generation settings' dialog.\n"
		      "\n"
		      " This class may be defined as a 'singleton', but I prefer to use static \n"
		      " members allowing to just write 'UmlSettings::member' rather than\n"
		      " 'UmlSettings::instance()->member' or other long sentence like this.\n");
  
  static const struct {
    const char * att;
    const char * after;
    const char * get;
    const char * set;
    const char * cmd;
  } s[] = {
    { "_artifact_default_description",
      "_defined",
      "artifactDescription",
      "set_ArtifactDescription",
      "setDefaultArtifactDescriptionCmd" },
    { "_class_default_description",
      "_artifact_default_description", 
      "classDescription",
      "set_ClassDescription",
      "setDefaultClassDescriptionCmd" },
    { "_operation_default_description",
      "_class_default_description",
      "operationDescription",
      "set_OperationDescription",
      "setDefaultOperationDescriptionCmd" },
    { "_attribute_default_description",
      "_operation_default_description",
      "attributeDescription",
      "set_AttributeDescription",
      "setDefaultAttributeDescriptionCmd" },
    { "_relation_default_description",
      "_attribute_default_description",
      "relationDescription",
      "set_RelationDescription",
      "setDefaultRelationDescriptionCmd" }
  };
  unsigned i;

  for (i = 0; i != sizeof(s)/sizeof(s[0]); i += 1) {
    // add attribute
    const QVector<UmlItem> ch = cl->children();
    UmlAttribute * at =
      cl->add_attribute(s[i].att, ProtectedVisibility, "string", 0, 0);
    at->set_isClassMember(TRUE);
    
    for (unsigned j = 0; j != ch.size(); j += 1) {
      if (ch[j]->name() == s[i].after) {
	at->moveAfter(ch[j]);
	break;
      }
    }
    
  }
  
  for (i = sizeof(s)/sizeof(s[0]) - 1; i != (unsigned) -1; i -= 1) {
    // add set
    UmlOperation * op = cl->add_op(s[i].set, PublicVisibility, "bool");
   
    op->moveAfter(0);
    op->set_isClassMember(TRUE);
    op->set_Description(" set the default description\n\
\n\
 On error : return FALSE in C++, produce a RuntimeException in Java");
    op->add_param(0, InputDirection, "v", "string"); 
    op->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(umlSettingsCmd, " + QCString(s[i].cmd) + ", v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    " + QCString(s[i].att) + " = v;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
    op->set_java("void", "${t0} ${p0}",
		 "  UmlCom.send_cmd(CmdFamily.umlSettingsCmd, UmlSettingsCmd._"
		 + QCString(s[i].cmd) + ", v);\n"
		 "  UmlCom.check();\n"
		 "  " + QCString(s[i].att) + " = v;\n",
		 FALSE);
    
    // add get
    op = 
      cl->add_op(s[i].get, PublicVisibility, "string");
   
    op->moveAfter(0);
    op->set_isClassMember(TRUE);
    op->set_Description(" return the default description");
    op->set_cpp("${type}", "",
		"  read_if_needed_();\n"
		"\n"
		"  return " + QCString(s[i].att) + ";\n",
		FALSE, 0, 0);
    op->set_java("${type}", "",
		 "  read_if_needed_();\n"
		"\n"
		"  return " + QCString(s[i].att) + ";\n",
		 FALSE);
  }
  
  // upgrade read_
  
  UmlOperation * op = cl->get_operation("read_");

  if (op != 0) {
    QCString body;
    
    body = op->cppBody();
    body.append("\n\
  _artifact_default_description = UmlCom::read_string();\n\
  _class_default_description = UmlCom::read_string();\n\
  _operation_default_description = UmlCom::read_string();\n\
  _attribute_default_description = UmlCom::read_string();\n\
  _relation_default_description = UmlCom::read_string();\n");
    op->set_CppBody(body);
      
    body = op->javaBody();    
    body.append("\n\
  _artifact_default_description = UmlCom.read_string();\n\
  _class_default_description = UmlCom.read_string();\n\
  _operation_default_description = UmlCom.read_string();\n\
  _attribute_default_description = UmlCom.read_string();\n\
  _relation_default_description = UmlCom.read_string();\n");
    op->set_JavaBody(body);  
  }
}

void upgrade_CppSettings()
{    
  static const struct {
    const char * att;
    const char * after_att;
    const char * get;
    const char * set;
    const char * after_oper;
    const char * cmd;
  } s[] = {
    { "_return", "_inout",
      "Return", "set_Return", "set_Inout", "setCppReturnCmd" },
    { "_enum_return", "_enum_inout",
      "enumReturn", "set_EnumReturn", "set_EnumInout", "setCppEnumReturnCmd" }
  };
  UmlClass * cl = UmlClass::get("CppSettings", 0);
  unsigned i;
  unsigned j;
  
  for (i = 0; i != sizeof(s)/sizeof(s[0]); i += 1) {
    UmlAttribute * at =
      cl->add_attribute(s[i].att, PrivateVisibility, "string", 0, 0);
    at->set_isClassMember(TRUE);
    
    const QVector<UmlItem> ch = cl->children();
  
    for (j = 0; j != ch.size(); j += 1) {
      if (ch[j]->name() == s[i].after_att) {
	at->moveAfter(ch[j]);
	break;
      }
    }
    
    // add set
    UmlOperation * op = cl->add_op(s[i].set, PublicVisibility, "bool");
   
    op->set_isClassMember(TRUE);
    op->set_Description(" set the default operation value type form\n\
\n\
 On error : return FALSE in C++, produce a RuntimeException in Java");
    op->add_param(0, InputDirection, "v", "string"); 
    op->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(cppSettingsCmd, " + QCString(s[i].cmd) + ", v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    " + QCString(s[i].att) + " = v;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
    op->set_java("void", "${t0} ${p0}",
		 "  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._"
		 + QCString(s[i].cmd) + ", v);\n"
		 "  UmlCom.check();\n"
		 "  " + QCString(s[i].att) + " = v;\n",
		 FALSE);
    
    // add get
    UmlOperation * op2 = cl->add_op(s[i].get, PublicVisibility, "string");
   
    op2->set_isClassMember(TRUE);
    op2->set_Description(" return the default operation value type form");
    op2->set_cpp("${type}", "",
		 "  read_if_needed_();\n"
		 "\n"
		 "  return " + QCString(s[i].att) + ";\n",
		 FALSE, 0, 0);
    op2->set_java("${type}", "",
		  "  read_if_needed_();\n"
		  "\n"
		  "  return " + QCString(s[i].att) + ";\n",
		  FALSE);
    
    for (j = 0; j != ch.size(); j += 1) {
      if (ch[j]->name() == s[i].after_oper) {
	op->moveAfter(ch[j]);
	op2->moveAfter(ch[j]);
	break;
      }
    }
  }
  
  // upgrade read_
  
  UmlOperation * op = cl->get_operation("read_");
  
  if (op != 0) {
    QCString body;
    int index;
    
    body = op->cppBody();
    index = body.find("}");
    body.insert(index, "  UmlSettings::_builtins[index].cpp_return = UmlCom::read_string();\n  ");
    index = body.find("_enum_in", index);
    body.insert(index, "_return = UmlCom::read_string();\n  ");
    index = body.find("_class_decl", index);
    body.insert(index, "_enum_return = UmlCom::read_string();\n  ");
    op->set_CppBody(body);
    
    body = op->javaBody(); 
    index = body.find("}");
    body.insert(index, "  UmlSettings._builtins[index].cpp_return = UmlCom.read_string();\n  ");
    index = body.find("_enum_in", index);
    body.insert(index, "_return = UmlCom.read_string();\n  ");
    index = body.find("_class_decl", index);
    body.insert(index, "_enum_return = UmlCom.read_string();\n  ");
    op->set_JavaBody(body);
  }
}

    
void upgrade_UmlBuiltin()
{
  UmlClass * cl = UmlClass::get("UmlBuiltin", 0);
  UmlAttribute * at =
    cl->add_attribute("cpp_return", PublicVisibility, "string", 0, 0);
  
  at->move_after(anAttribute, "cpp_inout");
}

void upgrade_new_settings__move_after_item(UmlClass * base_item, UmlClass * uml_item)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  add_move_after(base_item, uml_item);
  upgrade_OnInstanceCmd_UmlSettingsCmd_CppSettingsCmd();
  upgrade_UmlSettings();
  upgrade_CppSettings();
  upgrade_UmlBuiltin();
  
  UmlCom::set_user_id(uid);
}

//
// update UmlBaseClass::associatedComponent
//

bool one_component_per_class(UmlClass * uml_base_class)
{
  return (uml_base_class->get_operation("associatedComponent") != 0);
}

void several_components_per_class(UmlClass * uml_base_class)
{
  UmlOperation * op = 
    uml_base_class->get_operation("associatedComponent");
  
  op->set_Name("associatedComponents");
      
  op->set_Description(" returns the components realizing or providing the class.\n"
		      " To set them refer to the UmlBaseComponent's operation"
		      " setAssociatedClasses()");
      
  QCString s;
  int index;
  
  s = op->cppDecl();
  index = s.find("${type} *");
  if (index != -1)
    op->set_CppDecl(s.replace(index, 9, "const QVector<${type}>"));
  
  s = op->cppDef();
  index = s.find("${type} *");
  if (index != -1)
    op->set_CppDef(s.replace(index, 9, "const QVector<${type}>"));
  
  op->set_CppBody("  UmlCom::send_cmd(_identifier, assocComponentCmd);\n"
		  "\n"
		  "  QVector<UmlComponent> result;\n"
		  "  unsigned n = UmlCom::read_unsigned();\n"
		  "\n"
		  "  result.resize(n);\n"
		  "\n"
		  "  for (unsigned index = 0; index != n; index += 1)\n"
		  "    result.insert(index, (UmlComponent *) UmlBaseItem::read_());\n"
		  "\n"
		  "  return result;\n");
  
  s = op->javaDecl();
  index = s.find("${type}");
  if (index != -1)
    op->set_JavaDecl(s.replace(index, 7, "${type}[]"));
  
  op->set_JavaBody("  UmlCom.send_cmd(identifier_(), OnInstanceCmd.assocComponentCmd);\n"
		   "\n"
		   "  int n = UmlCom.read_unsigned();\n"
		   "  UmlComponent[] result = new UmlComponent[n];\n"
		   "\n"
		   "  for (int index = 0; index != n; index += 1)\n"
		   "    result[index] = (UmlComponent) UmlBaseItem.read_();\n"
		   "\n"
		   "  return result;\n");
  
  UmlCom::trace("update operation UmlBaseClass::associatedComponent<br>\n");
}

//
// Java JDK 5 upgrade
//

bool java_jdk1_4(UmlClass * javasettings)
{
  return (javasettings->get_operation("enumPatternDecl") == 0);
}

void upgrade_jdk5(UmlClass * javasettings)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
    
  //
  // modify old enum petterns, add new enums
  //
  
  UmlPackage::getProject()->rename_jdk5();
  
  QVector<UmlItem> ch = javasettings->children();
  UmlOperation * set_EnumPatternDecl = 0;
  UmlOperation * set_EnumPatternItemCase = 0;
  UmlAttribute * _enum_pattern_decl = 0;
  UmlAttribute * _enum_pattern_item_case = 0;
  unsigned i;
  
  // rename JavaSettings operations and attributes
  // updates read_'s body
  
  for (i = 0; i != ch.size(); i += 1) {
    switch (ch[i]->kind()) {
    case anOperation:
      {
	QCString s = ch[i]->name();
	
	if (s == "enumDecl") {
	  UmlCom::trace("rename JavaSettings::enumDecl to enumPatternDecl<br>\n");
	  ch[i]->set_Name("enumPatternDecl");
	}
	else if (s == "set_EnumDecl") {
	  UmlCom::trace("rename JavaSettings::set_EnumDecl to set_EnumPatternDecl<br>\n");
	  ch[i]->set_Name("set_EnumPatternDecl");
	  set_EnumPatternDecl = (UmlOperation *) ch[i];
	}
	else if (s == "enumItemDecl") {
	  UmlCom::trace("rename JavaSettings::enumItemDecl to enumPatternItemDecl<br>\n");
	  ch[i]->set_Name("enumPatternItemDecl");
	}
	else if (s == "set_EnumItemDecl") {
	  UmlCom::trace("rename JavaSettings::set_EnumItemDecl to set_EnumPatternItemDecl<br>\n");
	  ch[i]->set_Name("set_EnumPatternItemDecl");
	}
	else if (s == "enumItemCase") {
	  UmlCom::trace("rename JavaSettings::enumItemCase to enumPatternItemCase<br>\n");
	  ch[i]->set_Name("enumPatternItemCase");
	}
	else if (s == "set_EnumItemCase") {
	  UmlCom::trace("rename JavaSettings::set_EnumItemCase to set_EnumPatternItemCase<br>\n");
	  ch[i]->set_Name("set_EnumPatternItemCase");
	  set_EnumPatternItemCase = (UmlOperation *) ch[i];
	}
	else if (s == "read_") {
	  UmlCom::trace("update JavaSettings::read_<br>\n");
	  ((UmlOperation *) ch[i])->
	    set_CppBody("  _root = UmlCom::read_string();\n"
			"  \n"
			"  unsigned n;\n"
			"  unsigned index;\n"
			"  \n"
			"  n = UmlCom::read_unsigned();\n"
			"  \n"
			"  for (index = 0; index != n; index += 1) {\n"
			"    UmlSettings::_builtins[index].java = UmlCom::read_string();\n"
			"  }\n"
			"  \n"
			"  n = UmlCom::read_unsigned();\n"
			"  \n"
			"  for (index = 0; index != n; index += 1)\n"
			"    UmlSettings::_relation_stereotypes[index].java = UmlCom::read_string();\n"
			"  \n"
			"  n = UmlCom::read_unsigned();\n"
			"  \n"
			"  for (index = 0; index != n; index += 1)\n"
			"    UmlSettings::_class_stereotypes[index].java = UmlCom::read_string();\n"
			"  \n"
			"  n = UmlCom::read_unsigned();\n"
			"  _map_imports.clear();\n"
			"  if (n > _map_imports.size())\n"
			"    _map_imports.resize(n);\n"
			"  \n"
			"  for (index = 0; index != n; index += 1) {\n"
			"    QCString t = UmlCom::read_string();\n"
			"    QCString i = UmlCom::read_string();\n"
			"    \n"
			"    _map_imports.insert(t, new QCString(i));\n"
			"  }\n"
			"    \n"
			"  _src_content = UmlCom::read_string();\n"
			"  _ext = UmlCom::read_string();\n"
			"\n"
			"  _class_decl = UmlCom::read_string();\n"
			"  _external_class_decl = UmlCom::read_string();\n"
			"  _enum_decl = UmlCom::read_string();\n"
			"  _enum_pattern_decl = UmlCom::read_string();\n"
			"  _interface_decl = UmlCom::read_string();\n"
			"  _attr_decl = UmlCom::read_string();\n"
			"  _enum_item_decl = UmlCom::read_string();\n"
			"  _enum_pattern_item_decl = UmlCom::read_string();\n"
			"  _enum_pattern_item_case = UmlCom::read_string();\n"
			"  for (index = 0; index != 3; index += 1)\n"
			"    _rel_decl[index] = UmlCom::read_string();\n"
			"  _oper_def = UmlCom::read_string();\n"
			"  _get_visibility = (aVisibility) UmlCom::read_char();\n"
			"  _get_name = UmlCom::read_string();\n"
			"  _is_get_final = UmlCom::read_bool();\n"
			"  _set_visibility = (aVisibility) UmlCom::read_char();\n"
			"  _set_name = UmlCom::read_string();\n"
			"  _is_set_final = UmlCom::read_bool();\n"
			"  _is_set_param_final = UmlCom::read_bool();\n"
			);
	  
	  ((UmlOperation *) ch[i])->
	    set_JavaBody("  _root = UmlCom.read_string();\n"
			 "  \n"
			 "  int n;\n"
			 "  int index;\n"
			 "  \n"
			 "  n = UmlCom.read_unsigned();\n"
			 "  \n"
			 "  for (index = 0; index != n; index += 1) {\n"
			 "    UmlSettings._builtins[index].java = UmlCom.read_string();\n"
			 "  }\n"
			 "  \n"
			 "  n = UmlCom.read_unsigned();\n"
			 "  \n"
			 "  for (index = 0; index != n; index += 1)\n"
			 "    UmlSettings._relation_stereotypes[index].java = UmlCom.read_string();\n"
			 "  \n"
			 "  n = UmlCom.read_unsigned();\n"
			 "  \n"
			 "  for (index = 0; index != n; index += 1)\n"
			 "    UmlSettings._class_stereotypes[index].java = UmlCom.read_string();\n"
			 "  \n"
			 "  n = UmlCom.read_unsigned();\n"
			 "  _map_imports = new Hashtable((n == 0) ? 1 : n);\n"
			 "  \n"
			 "  for (index = 0; index != n; index += 1) {\n"
			 "    String t = UmlCom.read_string();\n"
			 "    String i = UmlCom.read_string();\n"
			 "    \n"
			 "    _map_imports.put(t, i);\n"
			 "  }\n"
			 "    \n"
			 "  _src_content = UmlCom.read_string();\n"
			 "  _ext = UmlCom.read_string();\n"
			 "\n"
			 "  _class_decl = UmlCom.read_string();\n"
			 "  _external_class_decl = UmlCom.read_string();\n"
			 "  _enum_decl = UmlCom.read_string();\n"
			 "  _enum_pattern_decl = UmlCom.read_string();\n"
			 "  _interface_decl = UmlCom.read_string();\n"
			 "  _attr_decl = UmlCom.read_string();\n"
			 "  _enum_item_decl = UmlCom.read_string();\n"
			 "  _enum_pattern_item_decl = UmlCom.read_string();\n"
			 "  _enum_pattern_item_case = UmlCom.read_string();\n"
			 "  _rel_decl = new String[3];\n"
			 "  for (index = 0; index != 3; index += 1)\n"
			 "    _rel_decl[index] = UmlCom.read_string();\n"
			 "  _oper_def = UmlCom.read_string();\n"
			 "  _get_visibility = aVisibility.fromInt(UmlCom.read_char());\n"
			 "  _get_name = UmlCom.read_string();\n"
			 "  _is_get_final = UmlCom.read_bool();\n"
			 "  _set_visibility = aVisibility.fromInt(UmlCom.read_char());\n"
			 "  _set_name = UmlCom.read_string();\n"
			 "  _is_set_final = UmlCom.read_bool();\n"
			 "  _is_set_param_final = UmlCom.read_bool();\n"
			 );
	}
      }
      break;
    case anAttribute:
      {
	QCString s = ch[i]->name();
	
	if (s == "_enum_decl") {
	  UmlCom::trace("rename JavaSettings::_enum_decl to _enum_pattern_decl<br>\n");
	  ch[i]->set_Name("_enum_pattern_decl");
	  _enum_pattern_decl = (UmlAttribute *) ch[i];
	}
	else if (s == "_enum_item_decl") {
	  UmlCom::trace("rename JavaSettings::_enum_item_decl to _enum_pattern_item_decl<br>\n");
	  ch[i]->set_Name("_enum_pattern_item_decl");
	}
	else if (s == "_enum_item_case") {
	  UmlCom::trace("rename JavaSettings::_enum_item_case to _enum_pattern_item_case<br>\n");
	  ch[i]->set_Name("_enum_pattern_item_case");
	  _enum_pattern_item_case = (UmlAttribute *) ch[i];
	}
      }
      break;
    default:
      break;
    }
  }
  
  // add new JavaSettings operations
  
  UmlOperation * op1;
  
  op1 = javasettings->add_op("enumDecl", PublicVisibility, "string");
  op1->set_Description(" returns the default definition of an enumeration");
  op1->set_cpp("${type}", "", "\
  read_if_needed_();\n\
  \n\
  return _enum_decl;\n",
	      FALSE, 0, 0);
  op1->set_java("${type}", "", "\
  read_if_needed_();\n\
  \n\
  return _enum_decl;\n",
	       FALSE);
  op1->set_isClassMember(TRUE);
  op1->moveAfter(set_EnumPatternDecl);

  UmlOperation * op2;
  
  op2 = javasettings->add_op("set_EnumDecl", PublicVisibility, "bool", TRUE);
  op2->add_param(0, InputDirection, "v", "string");
  op2->set_Description(" set the default definition of an enumeration\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->set_cpp("${type}", "${t0} ${p0}", "\
  UmlCom::send_cmd(javaSettingsCmd, setJavaEnumDeclCmd, v);\n\
  if (UmlCom::read_bool()) {\n\
    _enum_decl = v;\n\
    return TRUE;\n\
  }\n\
  else\n\
    return FALSE;\n",
	      FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}", "\
  UmlCom.send_cmd(CmdFamily.javaSettingsCmd, JavaSettingsCmd._setJavaEnumDeclCmd, v);\n\
  UmlCom.check();\n\
  \n\
  _enum_decl = v;\n",
	       FALSE);
  op2->set_isClassMember(TRUE);
  op2->moveAfter(op1);

  op1 = javasettings->add_op("enumItemDecl", PublicVisibility, "string", FALSE);
  op1->set_Description(" returns the default definition of an enumeration item");
  op1->set_cpp("${type}", "", "\
  read_if_needed_();\n\
  \n\
  return _enum_item_decl;\n",
	      FALSE, 0, 0);
  op1->set_java("${type}", "", "\
  read_if_needed_();\n\
  \n\
  return _enum_item_decl;\n",
	       FALSE);
  op1->set_isClassMember(TRUE);
  op1->moveAfter(set_EnumPatternItemCase);

  op2 = javasettings->add_op("set_EnumItemDecl", PublicVisibility, "bool", TRUE);
  op2->set_Description(" set the default definition of an enumeration item\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", "string");
  op2->set_cpp("${type}", "${t0} ${p0}", "\
  UmlCom::send_cmd(javaSettingsCmd, setJavaEnumItemDeclCmd, v);\n\
  if (UmlCom::read_bool()) {\n\
    _enum_item_decl = v;\n\
    return TRUE;\n\
  }\n\
  else\n\
    return FALSE;\n",
	      FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}", "\
  UmlCom.send_cmd(CmdFamily.javaSettingsCmd, JavaSettingsCmd._setJavaEnumItemDeclCmd, v);\n\
  UmlCom.check();\n\
    \n\
  _enum_item_decl = v;\n",
	       FALSE);
  op2->set_isClassMember(TRUE);
  op2->moveAfter(op1);
  
  // add new JavaSettings attributes
  
  UmlAttribute * att;
  
  att = javasettings->add_attribute("_enum_decl", PrivateVisibility, "string", 0, 0);
  att->set_isClassMember(TRUE);
  att->moveAfter(_enum_pattern_decl);
  
  att = javasettings->add_attribute("_enum_item_decl", PrivateVisibility, "string", 0, 0);
  att->set_isClassMember(TRUE);
  att->moveAfter(_enum_pattern_item_case);
  
  // rename JavaSettingsCmd items
  
  UmlClass * javacmd = UmlClass::get("JavaSettingsCmd", 0);
  
  ch = javacmd->children();
  
  for (i = 0; i != ch.size(); i += 1) {
    if (ch[i]->kind() == anAttribute) {
      if (ch[i]->name() == "setJavaEnumDeclCmd") {
	UmlCom::trace("rename JavaSettingsCmd::setJavaEnumDeclCmd to setJavaEnumPatternDeclCmd<br>\n");
	ch[i]->set_Name("setJavaEnumPatternDeclCmd");
      }
      else if (ch[i]->name() == "setJavaEnumItemDeclCmd") {
	UmlCom::trace("rename JavaSettingsCmd::setJavaEnumItemDeclCmd to setJavaEnumPatternItemDeclCmd<br>\n");
	ch[i]->set_Name("setJavaEnumPatternItemDeclCmd");
      }
      else if (ch[i]->name() == "setJavaEnumItemCaseCmd") {
	UmlCom::trace("rename JavaSettingsCmd::setJavaEnumItemCaseCmd to setJavaEnumPatternItemCaseCmd<br>\n");
	ch[i]->set_Name("setJavaEnumPatternItemCaseCmd");
      }
    }
  }
  
  // new JavaSettingsCmd items
  
  javacmd->add_enum_item("setJavaEnumDeclCmd");
  javacmd->add_enum_item("setJavaEnumItemDeclCmd");

  //
  // adds generics management
  //
  
  // add UmlBaseFormalParameter::_extends
  
  UmlClass * baseformalparam =
    UmlClass::get("UmlBaseFormalParameter", 0);
  
  ch = baseformalparam->children();
  
  for (i = 0; i != ch.size(); i += 1) {
    if (ch[i]->kind() == aRelation) {
      if (((UmlRelation *) ch[i])->roleName() == "_default_value") {
	UmlRelation * default_value = (UmlRelation *) ch[i];
	
	UmlCom::trace("add relation UmlBaseFormalParameter::_extends<br>\n");
	UmlRelation * extend = 
	  UmlRelation::create(aDirectionalAggregationByValue,
			      baseformalparam,
			      default_value->roleType());
	
	extend->set_CppDecl(default_value->cppDecl());
	extend->set_JavaDecl(default_value->javaDecl());
	extend->set_Visibility(ProtectedVisibility);
	extend->set_RoleName("_extends");
        extend->moveAfter(default_value);
	break;
      }
    }
  }
  
  // add UmlBaseFormalParameter::extend()
  
  UmlClass * typespec = UmlClass::get("UmlTypeSpec", 0);
  
  op1 = baseformalparam->add_op("extend", PublicVisibility,
				typespec, FALSE);
  op1->set_CppDecl("  ${comment}${static}${virtual}const ${type} & ${name}${(}${)}${const}${volatile}${abstract} { return _extends; };");
  op1->set_CppDef("");
  op1->set_JavaDecl("  ${comment}${visibility}${final}${static}${abstract}${synchronized}${type} ${name}${(}${)}${throws}${staticnl}{ return _extends; }");
  op1->set_Description("  return the optional extend (Java)");
  op1->moveAfter(baseformalparam->get_operation("defaultValue"));
  
  // update UmlBaseFormalParameter::read_()
  
  UmlCom::trace("update UmlBaseFormalParameter::read_()<br>\n");
  op1 = baseformalparam->get_operation("read_");
  op1->set_CppBody("  _name = UmlCom::read_string();\n"
		   "  _type = UmlCom::read_string();\n"
		   "  _default_value.type = (UmlClass *) UmlBaseItem::read_();\n"
		   "  if (_default_value.type == 0)\n"
		   "    _default_value.explicit_type = UmlCom::read_string();\n"
		   "  _extends.type = (UmlClass *) UmlBaseItem::read_();\n"
		   "  if (_extends.type == 0)\n"
		   "    _extends.explicit_type = UmlCom::read_string();\n");
  op1->set_JavaBody("  _name = UmlCom.read_string();\n"
		    "  _type = UmlCom.read_string();\n"
		    "  _default_value = new UmlTypeSpec();\n"
		    "  _default_value.type = (UmlClass) UmlBaseItem.read_();\n"
		    "  if (_default_value.type == null)\n"
		    "    _default_value.explicit_type = UmlCom.read_string();\n"
		    "  _extends = new UmlTypeSpec();\n"
		    "  _extends.type = (UmlClass) UmlBaseItem.read_();\n"
		    "  if (_extends.type == null)\n"
		    "    _extends.explicit_type = UmlCom.read_string();\n");
  
  // update an UmlCom::send_cmd

  ch = UmlClass::get("UmlCom", 0)->children();
  
  UmlCom::trace("update UmlCom::send_cmd(...)<br>\n");
  
  for (i = 0; i != ch.size(); i += 1) {
    if ((ch[i]->kind() == anOperation) &&
	(ch[i]->name() == "send_cmd")) {
      op1 = (UmlOperation *) ch[i];
      
      QValueList<UmlParameter> params = op1->params();
      
      if (params.count() == 6) {
	UmlParameter p = params.last();
	
	p.name = "arg5";
	op1->addParameter(6, p);
	op1->set_CppDecl("    ${comment}${friend}${static}${virtual}${type} ${name}${(}const ${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}, const ${t3} ${p3}, const ${t4} ${p4}, const ${t5} & ${p5}, const ${t6} & ${p6}${)}${const}${volatile}${abstract};");
	op1->set_CppDef("${inline}${type} ${class}::${name}${(}const ${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}, const ${t3} ${p3}, const ${t4} ${p4}, const ${t5} & ${p5}, const ${t6} & ${p6}${)}${const}${volatile}${staticnl}{\n${body}}");
	op1->set_JavaDef("  ${comment}${visibility}${final}${static}${abstract}${synchronized}${type} ${name}${(}${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}, ${t3} ${p3}, ${t4} ${p4}, ${t5} ${p5}, ${t6} ${p6}${)}${throws}${staticnl}{\n${body}}");
	op1->set_CppBody("#ifdef DEBUGBOUML\n"
			 "  cout << \"UmlCom::send_cmd(id, \" << cmd << \", \" << arg1 << \\\", \\\"\" << arg2 << \"\\\", \\\"\" << arg3 << \"\\\", \" << \", UmlTypeSpec, UmlTypeSpec)\\n\";\n"
			 "#endif\n"
			 "  \n"
			 "  write_char(onInstanceCmd);\n"
			 "  write_id(id);\n"
			 "  write_char(cmd);\n"
			 "  write_unsigned(arg1);\n"
			 "  write_string(arg2);\n"
			 "  write_string(arg3);\n"
			 "  if (arg4.type) {\n"
			 "    write_id(arg4.type->_identifier);\n"
			 "    write_string(\"\");\n"
			 "  }\n"
			 "  else {\n"
			 "    write_id(0);\n"
			 "    write_string(arg4.explicit_type);\n"
			 "  }\n"
			 "  if (arg5.type) {\n"
			 "    write_id(arg5.type->_identifier);\n"
			 "    write_string(\"\");\n"
			 "  }\n"
			 "  else {\n"
			 "    write_id(0);\n"
			 "    write_string(arg5.explicit_type);\n"
			 "  }\n"
			 "  flush();\n");
	op1->set_JavaBody("  write_char((byte) CmdFamily._onInstanceCmd);\n"
			  "  write_id(id);\n"
			  "  write_char((byte) cmd.value());\n"
			  "  write_unsigned(arg1);\n"
			  "  write_string(arg2);\n"
			  "  write_string(arg3);\n"
			  "  if (arg4.type != null) {\n"
			  "    write_id(arg4.type.identifier_());\n"
			  "    write_string(null);\n"
			  "  }\n"
			  "  else {\n"
			  "    write_id(0);\n"
			  "    write_string(arg4.explicit_type);\n"
			  "  }\n"
			  "  if (arg5.type != null) {\n"
			  "    write_id(arg5.type.identifier_());\n"
			  "    write_string(null);\n"
			  "  }\n"
			  "  else {\n"
			  "    write_id(0);\n"
			  "    write_string(arg5.explicit_type);\n"
			  "  }\n"
			  "  flush();\n");
	break;
      }
    }
  }
  
  
  // upgrade addFormal() and replaceFormal on UmlBaseClass
  
  UmlClass * baseclass = UmlClass::get("UmlBaseClass", 0);
  
  UmlCom::trace("update UmlBaseClass::addFormal()<br>\n");
  op1 = baseclass->get_operation("addFormal");
  op1->set_CppBody("  UmlCom::send_cmd(_identifier, addFormalCmd, rank, formal._name, \n"
		   "		   formal._type, formal._default_value, formal._extends);\n"
		   "  return UmlCom::read_bool();\n");
  op1->set_JavaBody("  UmlCom.send_cmd(identifier_(), OnInstanceCmd.addFormalCmd, rank, formal._name, \n"
		    "		   formal._type, formal._default_value, formal._extends);\n"
		    "  UmlCom.check();\n");
  
  UmlCom::trace("update UmlBaseClass::replaceFormal()<br>\n");
  op1 = baseclass->get_operation("replaceFormal");
  op1->set_CppBody("  UmlCom::send_cmd(_identifier, replaceFormalCmd, rank, formal._name, \n"
		   "		   formal._type, formal._default_value, formal._extends);\n"
		   "  return UmlCom::read_bool();\n");
  op1->set_JavaBody("  UmlCom.send_cmd(identifier_(), OnInstanceCmd.replaceFormalCmd, rank, formal._name, \n"
		    "		   formal._type, formal._default_value, formal._extends);\n"
		    "  UmlCom.check();\n");
  
  //
  // adds annotation management
  //

  // add OnInstanceCmd::setJavaAnnotationCmd
      
  UmlClass::get("OnInstanceCmd", 0)->add_enum_item("setJavaAnnotationCmd");

  // add operations javaAnnotations and set_JavaAnnotations
  // on UmlBaseClassMember 
  
  UmlClass * baseclassmember = UmlClass::get("UmlBaseClassMember", 0);
  
  op1 = baseclassmember->add_op("javaAnnotations", PublicVisibility, "string");
  op1->set_Description(" returns the Java annotations");
  op1->set_cpp("${type}", "", "\
  read_if_needed_();\n\
  \n\
  return _java_annotation;\n",
	      FALSE, "WITHJAVA", 0);
  op1->set_java("${type}", "", "\
  read_if_needed_();\n\
  \n\
  return _java_annotation;\n",
	       FALSE);
  op1->moveAfter(baseclassmember->get_operation("set_CppVisibility"));

  op2 = baseclassmember->add_op("set_JavaAnnotations", PublicVisibility, "bool", TRUE);
  op2->add_param(0, InputDirection, "v", "str");
  op2->set_Description(" set the Java annotations\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->set_cpp("${type}", "const ${t0} ${p0}",
	       "  return set_it_(_java_annotation, v, setJavaAnnotationCmd);\n",
	       FALSE, 0, "#endif");
  op2->set_java("void", "${t0} ${p0}", "\
  UmlCom.send_cmd(identifier_(), OnInstanceCmd.setJavaAnnotationCmd, v);\n\
  UmlCom.check();\n\
  \n\
  _java_annotation = v;\n",
	       FALSE);
  op2->moveAfter(op1);
  
  // add _java_annotations on UmlBaseClassMember 
  
  ch = baseclassmember->children();
  att = baseclassmember->add_attribute("_java_annotation", PrivateVisibility,
				       "string", "WITHJAVA", "#endif");
  att->move_after(aRelation, "_cpp_visibility");
  
  // add read_java_ on UmlBaseClassMember
  
  op1 = baseclassmember->add_op("read_java_", ProtectedVisibility, "void");
  op1->set_isCppVirtual(TRUE);
  op1->set_cpp("${type}", "", "\
  UmlBaseClassItem::read_java_();\n\
  _java_annotation = UmlCom::read_string();\n",
	      FALSE, "WITHJAVA", "#endif");
  op1->set_java("${type}", "", "\
  super.read_java_();\n\
  _java_annotation = UmlCom.read_string();\n",
	       FALSE);
  op1->moveAfter(baseclassmember->get_operation("read_cpp_"));
  
  // update read_java_ on UmlBaseClass UmlBaseAttribute UmlBaseRelation
  // and UmlBaseOperation to call UmlBaseClassMember::read_java_
  
  UmlCom::trace("update UmlBaseClass::read_java_()<br>\n");
  baseclass->get_operation("read_java_")
    ->set_CppBody("  UmlBaseClassMember::read_java_();\n"
		  "  _java_public = UmlCom::read_bool();\n"
		  "  _java_final = UmlCom::read_bool();\n"
		  "  _java_external = UmlCom::read_bool();\n");
  
  UmlCom::trace("update UmlBaseAttribute::read_java_()<br>\n");
  UmlClass::get("UmlBaseAttribute", 0)->get_operation("read_java_")
    ->set_CppBody("  UmlBaseClassMember::read_java_();\n"
		  "  _java_transient = UmlCom::read_bool();\n");
  
  UmlCom::trace("update UmlBaseRelation::read_java_()<br>\n");
  UmlClass::get("UmlBaseRelation", 0)->get_operation("read_java_")
    ->set_CppBody("  UmlBaseClassMember::read_java_();\n"
		  "  _java_transient = UmlCom::read_bool();\n");
  
  UmlCom::trace("update UmlBaseOperation::read_java_()<br>\n");
  UmlClass::get("UmlBaseOperation", 0)->get_operation("read_java_")
    ->set_CppBody("  UmlBaseClassMember::read_java_();\n"
		  "  _java_final = UmlCom::read_bool();\n"
		  "  _java_synchronized = UmlCom::read_bool();\n"
		  "  _java_name_spec = UmlCom::read_string();\n");
  
  // done
  
  UmlCom::set_user_id(uid);
}
 
//
// fixe Package associated diagram management
//

void fixe_package_diagram()
{
  UmlClass * basepackage = UmlClass::get("UmlBasePackage", 0);
  UmlOperation * op;
  UmlTypeSpec t;
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  // fixe UmlBasePackage::associatedDiagram()
  
  op = basepackage->get_operation("associatedDiagram");
  t.type = UmlClass::get("UmlDiagram", 0);
  op->set_ReturnType(t);
  UmlCom::trace("operation UmlBasePackage::associatedDiagram corrected<br>\n");
  
  UmlParameter param;
  
  // fixe UmlBasePackage::set_AssociatedDiagram()
  
  param.dir = InputOutputDirection;
  param.type.type = t.type;
  param.name = "d";
  op = basepackage->get_operation("set_AssociatedDiagram");
  op->replaceParameter(0, param);
  UmlCom::trace("operation UmlBasePackage::set_AssociatedDiagram corrected<br>\n");
  
  // fixe UmlBasePackage::read_uml_()
  
  op = basepackage->get_operation("read_uml_");
  op->set_CppBody("  _assoc_diagram = (UmlDiagram *) UmlBaseItem::read_();\n"
		  "  UmlBaseItem::read_uml_();\n");
  op->set_JavaBody("  _assoc_diagram = (UmlDiagram) UmlBaseItem.read_();\n"
		   "  super.read_uml_();\n");
  UmlCom::trace("operation UmlBasePackage::read_ corrected<br>\n");
  
  // replace _assoc_diagram
  
  const QVector<UmlItem> ch = basepackage->children();
  unsigned i;
  
  for (i = 0; i != ch.size(); i += 1) {
    if (ch[i]->kind() == aRelation) {
      if (((UmlRelation *) ch[i])->roleName() == "_assoc_diagram") {
	UmlCom::trace("replace UmlBasePackage::_assoc_diagram<br>\n");
	UmlRelation * old = (UmlRelation *) ch[i];
	UmlRelation * rel = UmlRelation::create(aDirectionalAssociation,
						basepackage,
						UmlClass::get("UmlDiagram", 0));
	
	rel->set_CppDecl(old->cppDecl());
	rel->set_JavaDecl(old->javaDecl());
	rel->set_Visibility(old->visibility());
        rel->moveAfter(old);
	old->delete_it();
	rel->set_RoleName("_assoc_diagram");
	break;
      }
    }
  }
    
  UmlCom::set_user_id(uid);
}

//
// fixe Java definition of UmlBaseActualParameter.read_
//

void fixe_umlbaseactualparameter_read()
{
  UmlClass::get("UmlBaseActualParameter", 0)->get_operation("read_")
    ->set_JavaBody("  _superClass = (UmlClass) UmlBaseItem.read_();	// cannot be 0\n"
		   "  _rank = UmlCom.read_unsigned();\n"
		   "  _value = new UmlTypeSpec();\n"
		   "  _value.type = (UmlClass) UmlBaseItem.read_();\n"
		   "  if (_value.type == null)\n"
		   "    _value.explicit_type = UmlCom.read_string();\n");
  UmlCom::trace("operation UmlBaseActualParameter.read_ corrected<br>\n");
}

//
// add class association management
//

void add_class_association(UmlClass * baserelation)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  // add OnInstanceCmd::setRelationAssocClassCmd
      
  UmlClass::get("OnInstanceCmd", 0)->add_enum_item("setRelationAssocClassCmd");
  
  // add relation _association
  
  UmlClass * typespec = UmlClass::get("UmlTypeSpec", 0);
  UmlRelation * rel =
    baserelation->add_relation(aDirectionalAggregationByValue, 
			       "_association", PrivateVisibility,
				typespec, 0, 0);
  
  rel->move_after(anAttribute, "_multiplicity");
  
  // add association on UmlBaseRelation
  
  UmlOperation * op1 =
    baserelation->add_op("association", PublicVisibility, typespec);
  
  op1->set_cpp("${type}", "", "\
  read_if_needed_();\n\
  \n\
  return _association;\n",
	       FALSE, 0, 0);
  op1->set_java("${type}", "", "\
  read_if_needed_();\n\
  \n\
  return _association;\n",
		FALSE);
  op1->set_Description(" return the associated class/type, may be an empty spec");
  op1->moveAfter(baserelation->get_operation("roleType"));
 
  // add set_Association on UmlBaseRelation
  
  UmlOperation * op2 =
    baserelation->add_op("set_Association", PublicVisibility, "bool", TRUE);
  
  op2->add_param(0, InputDirection, "t", typespec);
  op2->set_Description(" set the associated class/type, may be an empty spec\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->set_cpp("${type}", "const ${t0} & ${p0}", "\
  if (set_it_(_association, t, setRelationAssocClassCmd)) {\n\
    UmlBaseRelation * other = (UmlBaseRelation *) UmlBaseItem::read_();\n\
    \n\
    if (other != 0)\n\
      other->_association = t;\n\
    return TRUE;\n\
  }\n\
  else\n\
    return FALSE;\n",
	      FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}", "\
  UmlCom.send_cmd(identifier_(), OnInstanceCmd.setRelationAssocClassCmd, t);\n\
  UmlCom.check();\n\
  \n\
  _association = t;\n\
  UmlBaseRelation other = (UmlBaseRelation) UmlBaseItem.read_();\n\
  \n\
  if (other != null)\n\
    other._association = t;\n",
	       FALSE);
  op2->moveAfter(op1);
  
  // upgrade read_uml_
  
  op1 = baserelation->get_operation("read_uml_");
  
  op1->set_CppBody("  UmlBaseClassMember::read_uml_();\n"
		   "  _rel_kind = (aRelationKind) UmlCom::read_char();\n"
		   "  _role_type = (UmlClass *) UmlBaseItem::read_();\n"
		   "  _association.type = (UmlClass *) UmlBaseItem::read_();\n"
		   "  if (_association.type == 0)\n"
		   "    _association.explicit_type = UmlCom::read_string();\n"
		   "  _role_name = UmlCom::read_string();\n"
		   "  _multiplicity = UmlCom::read_string();\n"
		   "  _default_value = UmlCom::read_string();\n"
		   "  _read_only = UmlCom::read_bool();\n"
		   "  _get_oper = (UmlOperation *) UmlBaseItem::read_();\n"
		   "  _set_oper = (UmlOperation *) UmlBaseItem::read_();\n");
  op1->set_JavaBody("  super.read_uml_();\n"
		    "  _rel_kind = aRelationKind.fromInt(UmlCom.read_char());\n"
		    "  _role_type = (UmlClass) UmlBaseItem.read_();\n"
		    "  _association = new UmlTypeSpec();\n"
		    "  _association.type = (UmlClass) UmlBaseItem.read_();\n"
		    "  if (_association.type == null)\n"
		    "    _association.explicit_type = UmlCom.read_string();\n"
		    "  _role_name = UmlCom.read_string();\n"
		    "  _multiplicity = UmlCom.read_string();\n"
		    "  _default_value = UmlCom.read_string();\n"
		    "  _read_only = UmlCom.read_bool();\n"
		    "  _get_oper = (UmlOperation) UmlBaseItem.read_();\n"
		    "  _set_oper = (UmlOperation) UmlBaseItem.read_();\n");
  UmlCom::trace("operation UmlBaseRelation::read_uml_ upgraded<br>\n");
    
  UmlCom::set_user_id(uid);
}

//
// add visibility 'package' management
//

void add_package_visibility()
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  // add aVisibility::setRelationAssocClassCmd
  UmlAttribute * at =
    UmlClass::get("aVisibility", 0)->add_enum_item("PackageVisibility");
  
  at->move_after(anAttribute, "PrivateVisibility");
    
  UmlCom::set_user_id(uid);
}

//
// add UmlBaseRelation::side()
//

void add_side(UmlClass * baserelation)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);  
  UmlOperation * op =
    baserelation->add_op("side", PublicVisibility,
			 UmlClass::get("UmlRelation", 0));
  op->add_param(0, InputDirection, "first", "bool");
  op->set_cpp("${type} *", "${t0} ${p0}", "\
  UmlCom::send_cmd(_identifier, sideCmd, (char) first);\n\
  \n\
  return (UmlRelation *) UmlBaseItem::read_();\n",
	       FALSE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}", "\
  UmlCom.send_cmd(identifier_(), OnInstanceCmd.sideCmd, \n\
		  (first) ? (byte) 1 : (byte) 0);\n\
  \n\
  return (UmlRelation) UmlBaseItem.read_();\n",
		FALSE);
  op->set_Description(" if 'first' is true returns the relation associated to the\n"
		      " first role, else the relation associated to the second\n"
		      " role or 0/null if the relation is uni directional");
  op->move_after(anOperation, "relationKind");
  
  // upgrade OnInstanceCmd
  
  UmlClass * cl = UmlClass::get("OnInstanceCmd", 0);
  
  cl->add_enum_item("sideCmd")->move_after(anAttribute, "referencedByCmd");
    
  UmlCom::set_user_id(uid);
}

//
// add ObjectBaseDiagram & ObjectDiagram classes
//

void add_object_activity_diagram_item_kind()
{
  // already root
  UmlClass * itkind = UmlClass::get("anItemKind", 0);
  UmlAttribute * anObjectDiagram = itkind->add_enum_item("anObjectDiagram");
  UmlAttribute * anActivityDiagram = itkind->add_enum_item("anActivityDiagram");
  
  const QVector<UmlItem> ch = itkind->children();
    
  for (unsigned i = 0; i != ch.size(); i += 1) {
    if (ch[i]->name() == "aDeploymentDiagram") {
      anObjectDiagram->moveAfter(ch[i]);
      anActivityDiagram->moveAfter(anObjectDiagram);
      break;
    }
  }
}

void baseitem_read_objectdiagram(UmlClass * base_item) {
  // update read_()'s body
  UmlOperation * op = base_item->get_operation("read_");
  
  if (op != 0) {
    QCString body;
    
    body = op->cppBody();
    body.insert(body.findRev("default:"),
		"case anObjectDiagram:\n\
      return new UmlObjectDiagram(id, name);\n\
    case anActivityDiagram:\n\
      //return new UmlActivityDiagram(id, name);\n\
    ");
    op->set_CppBody(body);
      
    body = op->javaBody();
    body.insert(body.findRev("default:"),
		"case anItemKind._anObjectDiagram:\n\
      return new UmlObjectDiagram(id, name);\n\
    case anItemKind._anActivityDiagram:\n\
      //return new UmlActivityDiagram(id, name);\n\
    ");
    op->set_JavaBody(body);
  }
  
  // update BaseUmlItem artifact
  UmlArtifact * art = base_item->associatedArtifact();
  QCString s;
  
  s = art->cppSource();
  s.insert(s.find("#include \"UmlDeploymentDiagram.h\""),
	   "#include \"UmlObjectDiagram.h\"\n");
  art->set_CppSource(s);
}
				 
void add_objectdiagram(UmlClass * base_item, UmlClass * user_item)
{
  UmlClassView * base_class_view = (UmlClassView *) base_item->parent();
  UmlClassView * user_class_view = (UmlClassView *) user_item->parent();
  UmlDeploymentView * base_depl_view = (UmlDeploymentView *)
    base_item->associatedArtifact()->parent();
  UmlDeploymentView * user_depl_view = (UmlDeploymentView *)
    user_item->associatedArtifact()->parent();
  UmlClass * user_diagram = UmlClass::get("UmlDiagram", 0);
  UmlClass * base_objectdiagram;
  UmlClass * user_objectdiagram;
  
  UmlCom::trace("Object diagram<br>\n");
  
  user_diagram->made(base_class_view, user_class_view,
		     base_depl_view, user_depl_view,
		     base_item, "ObjectDiagram",
		     base_objectdiagram, user_objectdiagram,
		     0);
  base_objectdiagram->add_default_base_op(user_diagram, user_objectdiagram, user_item,
					  "object diagram", "anObjectDiagram");
  user_objectdiagram->add_constr(base_objectdiagram, PublicVisibility);

  user_objectdiagram->set_Description(" This class manages 'object diagrams', notes that the class 'UmlDiagram'\n"
				      " is a mother class of all the diagrams, allowing to generalize their\n"
				      " management\n"
				      "\n"
				      " You can modify it as you want (except the constructor)");
  UmlClass * cl;
  
  if ((cl = UmlClass::get("UmlBaseClassDiagram", 0)) != 0)
    base_objectdiagram->moveAfter(cl);
  
  if ((cl = UmlClass::get("UmlClassDiagram", 0)) != 0)
    user_objectdiagram->moveAfter(cl);
  
  //
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("update anItemKind<br>\n");
  
  add_object_activity_diagram_item_kind();
  
  UmlCom::trace("update item read<br>\n");
  
  baseitem_read_objectdiagram(base_item);
  
  UmlCom::set_user_id(uid);
}

//
// add all concerning activities
//

UmlClass * add_activity_diagram(UmlClassView * base_class_view, UmlClassView * user_class_view,
				UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
				UmlClass * base_item, UmlClass * user_activity,
				UmlClass * user_activityitem)
{
  UmlClass * user_diagram = UmlClass::get("UmlDiagram", 0);
  UmlClass * base_activitydiagram;
  UmlClass * user_activitydiagram;
    
  user_diagram->made(base_class_view, user_class_view,
		     base_depl_view, user_depl_view,
		     base_item, "ActivityDiagram",
		     base_activitydiagram, user_activitydiagram,
		     user_activityitem);
  base_activitydiagram->add_default_base_op(user_diagram, user_activitydiagram, user_activity,
					    "activity diagram", "anActivityDiagram");
  user_activitydiagram->add_constr(base_activitydiagram, PublicVisibility);

  user_activitydiagram->set_Description(" This class manages 'activity diagrams', notes that the class 'UmlDiagram'\n"
					" is a mother class of all the diagrams, allowing to generalize their\n"
					" management\n"
					"\n"
					" You can modify it as you want (except the constructor)");
  UmlClass * cl;
  
  if ((cl = UmlClass::get("UmlBaseStateDiagram", 0)) != 0)
    base_activitydiagram->moveAfter(cl);
  
  if ((cl = UmlClass::get("UmlStateDiagram", 0)) != 0)
    user_activitydiagram->moveAfter(cl);

  return user_activitydiagram;
}

void add_pre_post_conditions(UmlClass * base_class)
{
  // uid is already 0
  UmlOperation * op;

  defGet(base_class, _pre_condition, preCondition, "string", 0, 0,
	 "pre condition");
  defSet(base_class, _pre_condition, set_PreCondition, "str", setUmlEntryBehaviorCmd, 0, 0,
	 "pre condition");
  defGet(base_class, _post_condition, postCondition, "string", 0, 0,
	 "post condition");
  defSet(base_class, _post_condition, set_PostCondition, "str", setUmlExitBehaviorCmd, 0, 0,
	 "post condition");

  defGet(base_class, _cpp_pre_condition, cppPreCondition, "string", "WITHCPP", 0,
	 "pre condition in C++");
  defSet(base_class, _cpp_pre_condition, set_CppPreCondition, "str", setCppEntryBehaviorCmd, 0, 0,
	 "pre condition in C++");
  defGet(base_class, _cpp_post_condition, cppPostCondition, "string", 0, 0,
	 "post condition in C++");
  defSet(base_class, _cpp_post_condition, set_CppPostCondition, "str", setCppExitBehaviorCmd, 0, "#endif",
	 "post condition in C++");

  defGet(base_class, _java_pre_condition, javaPreCondition, "string", "WITHJAVA", 0,
	 "pre condition in Java");
  defSet(base_class, _java_pre_condition, set_JavaPreCondition, "str", setJavaEntryBehaviorCmd, 0, 0,
	 "pre condition in Java");
  defGet(base_class, _java_post_condition, javaPostCondition, "string", 0, 0,
	 "post condition in Java");
  defSet(base_class, _java_post_condition, set_JavaPostCondition, "str", setJavaExitBehaviorCmd, 0, "#endif",
	 "post condition in Java");
}

void add_activity(UmlClassView * base_class_view, UmlClassView * user_class_view,
		  UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		  UmlClass * base_item, UmlClass * user_item, UmlClass * user_activityitem)
{
  UmlClass * base_activity;
  UmlClass * user_activity;
  UmlOperation * op;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "Activity", base_activity, user_activity,
		  0);
  base_activity->add_default_base_op(user_item, user_activity,
				     UmlClass::get("UmlClassView", 0),
				     "activity", "anActivity");
  user_activity->add_constr(base_activity, PublicVisibility);  
    
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
    
  add_pre_post_conditions(base_activity);
  include_umlcom(base_activity);

  defGetBool(base_activity, _read_only, isReadOnly, 0, 0,
	     "isReadOnly attribute, if TRUE the activity must not made any changes to variables ouside the activity or to objects.");
  defSetBool(base_activity, _read_only, set_isReadOnly, setReadOnlyCmd, 0, 0,
	     "isReadOnly attribute");
  defGetBool(base_activity, _single_execution, isSingleExecution, 0, 0,
	     "isSingleExecution attribute, if TRUE all invocations of the activity are handle by the same execution");
  defSetBool(base_activity, _single_execution, set_isSingleExecution, setSingleExecutionCmd, 0, 0,
	     "isSingleExecution attribute");

  UmlCom::set_user_id(uid);
    
  UmlClass * user_activitydiagram = 
    add_activity_diagram(base_class_view, user_class_view,
			 base_depl_view, user_depl_view,
			 base_item, user_item, user_activityitem);
  
  UmlCom::set_user_id(0);
    
  add_assoc_diag_ops(base_activity, user_activitydiagram);

  op = base_activity->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _pre_condition = 0;\n"
	      "  _post_condition = 0;\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp_pre_condition = 0;\n"
	      "  _cpp_post_condition = 0;\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java_pre_condition = 0;\n"
	      "  _java_post_condition = 0;\n"
	      "#endif\n"
	      "  UmlBaseItem::unload(rec, del);\n",
	      FALSE, 0, 0);
  QCString s = op->cppDecl();
  
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _pre_condition = null;\n"
	       "  _post_condition = null;\n"
	       "  _cpp_pre_condition = null;\n"
	       "  _cpp_post_condition = null;\n"
	       "  _java_pre_condition = null;\n"
	       "  _java_post_condition = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE. \n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
    
  base_activity->add_attribute("_pre_condition", PrivateVisibility, "string",
			       0, 0);
  base_activity->add_attribute("_post_condition", PrivateVisibility, "string",
			       0, 0);
  base_activity->add_attribute("_cpp_pre_condition", PrivateVisibility, "string",
			       "WITHCPP", 0);
  base_activity->add_attribute("_cpp_post_condition", PrivateVisibility, "string",
			       0, "endif");
  base_activity->add_attribute("_java_pre_condition", PrivateVisibility, "string",
				  "WITHJAVA", 0);
  base_activity->add_attribute("_java_post_condition", PrivateVisibility, "string",
			       0, "endif");
  base_activity->add_attribute("_read_only", PrivateVisibility, "bool",
			       0, 0);
  base_activity->add_attribute("_single_execution", PrivateVisibility, "bool",
			       0, 0);
  base_activity->add_relation(aDirectionalAssociation,
			      "_assoc_diagram", PrivateVisibility,
			      user_activitydiagram, 0, 0);  
  
  op = base_activity->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _assoc_diagram = (UmlActivityDiagram *) UmlBaseItem::read_();\n"
	      "  UmlBaseItem::read_uml_();\n"
	      "  _pre_condition = UmlCom::read_string();\n"
	      "  _post_condition = UmlCom::read_string();\n"
	      "  _read_only = UmlCom::read_bool();\n"
	      "  _single_execution = UmlCom::read_bool();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  _assoc_diagram = (UmlActivityDiagram) UmlBaseItem.read_();\n"
	       "  super.read_uml_();\n"
	       "  _pre_condition = UmlCom.read_string();\n"
	       "  _post_condition = UmlCom.read_string();\n"
	       "  _read_only = UmlCom.read_bool();\n"
	       "  _single_execution = UmlCom.read_bool();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_activity->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _cpp_pre_condition = UmlCom::read_string();\n"
	      "  _cpp_post_condition = UmlCom::read_string();\n",
	      FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "",
	       "  _cpp_pre_condition = UmlCom.read_string();\n"
	       "  _cpp_post_condition = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_activity->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	       "  _java_pre_condition = UmlCom::read_string();\n"
	       "  _java_post_condition = UmlCom::read_string();\n",
	      FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "",
	       "  _java_pre_condition = UmlCom.read_string();\n"
	       "  _java_post_condition = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
}

UmlClass * add_activitynode(UmlClassView * base_class_view, UmlClassView * user_class_view,
			    UmlDeploymentView * base_depl_view,
			    UmlDeploymentView * user_depl_view,
			    UmlClass * base_item, UmlClass * user_item,
			    UmlClass * user_activityitem)
{
  UmlClass * base_activitynode;
  UmlClass * user_activitynode;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "ActivityNode", base_activitynode, user_activitynode,
		  user_activityitem);

  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
    
  base_activitynode->add_constr(user_item, ProtectedVisibility); 
  
  UmlCom::set_user_id(uid);
  
  user_activitynode->add_constr(base_activitynode, ProtectedVisibility);
  user_activitynode->set_isAbstract(TRUE);

  return user_activitynode;
}

UmlClass * add_flow_behavior(UmlClassView * base_class_view,
			     UmlDeploymentView * base_depl_view)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlClass * flow_behavior = 
    UmlClass::made(base_class_view, base_depl_view, "FlowBehavior");
  
  include_umlcom(flow_behavior);
  
  flow_behavior->add_attribute("weight", PublicVisibility, "string", 0, 0);
  flow_behavior->add_attribute("guard", PublicVisibility, "string", 0, 0);
  flow_behavior->add_attribute("selection", PublicVisibility, "string", 0, 0);
  flow_behavior->add_attribute("transformation", PublicVisibility, "string", 0, 0);
  
  UmlOperation * op;
  
  op = flow_behavior->add_op("read", PublicVisibility, "void");
  op->set_cpp("${type}", "", "\
  weight = UmlCom::read_string();\n\
  guard = UmlCom::read_string();\n\
  selection = UmlCom::read_string();\n\
  transformation = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "", "\
  weight = UmlCom.read_string();\n\
  guard = UmlCom.read_string();\n\
  selection = UmlCom.read_string();\n\
  transformation = UmlCom.read_string();\n",
	       FALSE);
  
  op = flow_behavior->add_op("unload", PublicVisibility, "void");
  op->set_cpp("${type}", "", "\
  weight = 0;\n\
  guard = 0;\n\
  selection = 0;\n\
  transformation = 0;\n",
	      FALSE, 0, 0);
  op->set_JavaDecl("");
  
  UmlCom::set_user_id(uid);
  
  return flow_behavior;
}

void add_flow(UmlClassView * base_class_view, UmlClassView * user_class_view,
	      UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
	      UmlClass * base_item, UmlClass * user_item,
	      UmlClass * user_activityitem, UmlClass * user_activitynode)
{
  UmlClass * base_flow;
  UmlClass * user_flow;
  UmlOperation * op;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "Flow", base_flow,
		  user_flow, user_activityitem);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  include_umlcom(base_flow);
  
  base_flow->add_kind("aFlow");
  
  op = base_flow->add_op("create", PublicVisibility, user_flow, TRUE);
  op->set_isClassMember(TRUE);
  op->add_param(0, InputOutputDirection, "start", user_activitynode);
  op->add_param(1, InputOutputDirection, "end", user_activitynode);
  op->set_cpp("${type} *", 
	      "${t0} * ${p0}, ${t1} * ${p1}",
	      "  UmlCom::send_cmd(start->_identifier, createCmd, aFlow,\n"
	      "\t\t   end->_identifier);\n"
	      "  UmlFlow * result = (UmlFlow *) UmlBaseItem::read_();\n"
	      "\n"
	      "  if (result != 0)\n"
	      "    start->reread_children_if_needed_();\n"
	      "  return result;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  UmlCom.send_cmd(start.identifier_(), OnInstanceCmd.createCmd, anItemKind.aFlow,\n"
	       "\t\t   end.identifier_());\n"
	       "  UmlFlow result = (UmlFlow) UmlBaseItem.read_();\n"
	       "  \n"
	       "  if (result != null)\n"
	       "    start.reread_children_if_needed_();\n"
	       "  else\n"
	       "    throw new RuntimeException(\"Cannot create the flow\");\n"
	       "  return result;\n",
	      FALSE);
  op->set_Description(" Returns a new flow from 'start' to 'end'\n\
\n\
 In case it cannot be created ('parent' cannot contain it etc ...) return 0\n\
  in C++ and produce a RuntimeException in Java\n");
  
  op = base_flow->add_op("target", PublicVisibility, user_activitynode);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" returns the 'end' object (the 'start' object is the parent of the flow) no set !");
  op->set_cpp("${type} *", "",
	      "  read_if_needed_();\n"
	      "  return _target;\n",
	      FALSE, 0, 0); \
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "  return _target;\n", FALSE);
  
  defGetField(base_flow, _uml, weight, weight, "string", 0, 0,
	      "weight in OCL");
  defSetField(base_flow, _uml, set_Weight, weight, "str", setUmlActivityCmd, 0, 0,
	      "weight in OCL");
  defGetField(base_flow, _uml, guard, guard, "string", 0, 0,
	      "guard in OCL");
  defSetField(base_flow, _uml, set_Guard, guard, "str", setUmlGuardCmd, 0, 0,
	      "guard in OCL");
  defGetField(base_flow, _uml, selection, selection, "string", 0, 0, 
	      "selection in OCL");
  defSetField(base_flow, _uml, set_Selection, selection, "str", setUmlEntryBehaviorCmd, 0, 0,
	      "selection in OCL");
  defGetField(base_flow, _uml, transformation, transformation, "string", 0, 0, 
	      "transformation in OCL");
  defSetField(base_flow, _uml, set_Transformation, transformation, "str", setUmlExitBehaviorCmd, 0, 0,
	      "transformation in OCL");

  defGetField(base_flow, _cpp, cppWeight, weight, "string", "WITHCPP", 0,
	      "weight in C++");
  defSetField(base_flow, _cpp, set_CppWeight, weight, "str", setCppActivityCmd, 0, 0,
	      "weight in C++");
  defGetField(base_flow, _cpp, cppGuard, guard, "string", 0, 0,
	      "guard in C++");
  defSetField(base_flow, _cpp, set_CppGuard, guard, "str", setCppGuardCmd, 0, 0,
	      "guard in C++");
  defGetField(base_flow, _cpp, cppSelection, selection, "string", 0, 0,
	      "selection in C++");
  defSetField(base_flow, _cpp, set_CppSelection, selection, "str", setCppEntryBehaviorCmd, 0, 0,
	      "selection in C++");
  defGetField(base_flow, _cpp, cppTransformation, transformation, "string", 0, 0,
	      "transformation in C++");
  defSetField(base_flow, _cpp, set_CppTransformation, transformation, "str", setCppExitBehaviorCmd, 0, "endif",
	      "transformation in C++");

  defGetField(base_flow, _java, javaWeight, weight, "string", "WITHJAVA", 0,
	      "weight in Java");
  defSetField(base_flow, _java, set_JavaWeight, weight, "str", setJavaActivityCmd, 0, 0,
	      "weight in Java");
  defGetField(base_flow, _java, javaGuard, guard, "string", 0, 0,
	      "guard in Java");
  defSetField(base_flow, _java, set_JavaGuard, guard, "str", setJavaGuardCmd, 0, 0,
	      "guard in Java");
  defGetField(base_flow, _java, javaSelection, selection, "string", 0, 0,
	      "selection in Java");
  defSetField(base_flow, _java, set_JavaSelection, selection, "str", setJavaEntryBehaviorCmd, 0, 0,
	      "selection in Java");
  defGetField(base_flow, _java, javaTransformation, transformation, "string", 0, 0,
	      "transformation in Java");
  defSetField(base_flow, _java, set_JavaTransformation, transformation, "str", setJavaExitBehaviorCmd, 0, "endif",
	      "transformation in Java");
  
  op = base_flow->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _uml.unload();\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp.unload();\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java.unload();\n"
	      "#endif\n"
	      "  UmlBaseItem::unload(rec, del);\n",
	      FALSE, 0, 0);

  QCString s = op->cppDecl();
  
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _uml = null;\n"
	       "  _cpp = null;\n"
	       "  _java = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE. \n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
  
  base_flow->add_relation(aDirectionalAssociation, "_target", PrivateVisibility,
			  user_activitynode, 0, 0);
			  
  UmlClass * flow_behavior =
    add_flow_behavior(base_class_view, base_depl_view);
  
  base_flow->add_relation(aDirectionalAggregationByValue, "_uml", PrivateVisibility,
			  flow_behavior, 0, 0);
  base_flow->add_relation(aDirectionalAggregationByValue, "_cpp", PrivateVisibility,
			  flow_behavior, "WITHCPP", "endif");
  base_flow->add_relation(aDirectionalAggregationByValue, "_java", PrivateVisibility,
			  flow_behavior, "WITHJAVA", "endif");
  
  op = base_flow->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseItem::read_uml_();\n"
	      "  _target = (UmlActivityNode *) UmlBaseItem::read_();\n"
	      "  _uml.read();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _target = (UmlActivityNode) UmlBaseItem.read_();\n"
	       "  _uml = new FlowBehavior();\n"
	       "  _uml.read();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_flow->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _cpp.read();\n", FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "", "  _cpp = new FlowBehavior();\n  _cpp.read();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_flow->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _java.read();\n", FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "", "  _java = new FlowBehavior();\n  _java.read();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  base_flow->add_constr(user_item, ProtectedVisibility);
  
  UmlCom::set_user_id(uid);
  
  op = user_flow->add_op("UmlFlow", PublicVisibility, (UmlClass *) 0, FALSE);
  op->add_param(0, InputDirection, "id", "item_id");
  op->add_param(1, InputDirection, "n", "string");
  op->set_cpp(": UmlBaseFlow(id, n)",
	      "${t0} ${p0}, const ${t1} & ${p1}",
	      "", TRUE, 0, 0);
  op->set_java("", "${t0} ${p0}, ${t1} ${p1}",
	       "  super(id, n);\n", TRUE);
}

void add_return_direction()
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlClass * dir = UmlClass::get("aDirection", 0);
  
  dir->add_enum_item("ReturnDirection");
  
  UmlCom::set_user_id(uid);
}

void add_activityregion(UmlClassView * base_class_view, UmlClassView * user_class_view,
			UmlDeploymentView * base_depl_view,
			UmlDeploymentView * user_depl_view,
			UmlClass * base_item, UmlClass * user_item,
			UmlClass * user_activityitem, UmlClass * user_activitydiagram)
{
  // region
  
  UmlClass * base_activityregion;
  UmlClass * user_activityregion;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "ActivityRegion",
		  base_activityregion, user_activityregion,
		  user_activityitem);

  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  base_activityregion->add_constr(user_item, ProtectedVisibility);
  include_umlcom(base_activityregion);
  
  UmlCom::set_user_id(uid);
  
  user_activityregion->add_constr(base_activityregion, ProtectedVisibility);
  user_activityregion->set_isAbstract(TRUE);
  
  UmlCom::set_user_id(0);
  
  UmlOperation * op;
  
  add_assoc_diag_ops(base_activityregion, user_activitydiagram);
  
  base_activityregion->add_relation(aDirectionalAssociation,
				    "_assoc_diagram", PrivateVisibility,
				    user_activitydiagram, 0, 0); 
  
  op = base_activityregion->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _assoc_diagram = (UmlActivityDiagram *) UmlBaseItem::read_();\n"
	      "  UmlBaseItem::read_uml_();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  _assoc_diagram = (UmlActivityDiagram) UmlBaseItem.read_();\n"
	       "  super.read_uml_();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // expansion region
  
  UmlClass * base_expansionregion;
  UmlClass * user_expansionregion;
  
  user_activityregion->made(base_class_view, user_class_view,
			    base_depl_view, user_depl_view,
			    base_item, "ExpansionRegion",
			    base_expansionregion, user_expansionregion,
			    0);
  
  base_expansionregion->add_default_base_op(user_activityregion, user_expansionregion,
					    user_item, "expansion region", "anExpansionRegion");
  user_expansionregion->add_constr(base_expansionregion, PublicVisibility);
  
  UmlCom::set_user_id(0);
  
  include_umlcom(base_expansionregion);
  
  defGetBool(base_expansionregion, _must_isolate, isMustIsolate, 0, 0,
	     "isMustIsolate attribute, if TRUE the actions in the region execute in isolation from actions outside the region.");
  defSetBool(base_expansionregion, _must_isolate, set_isMustIsolate, setFlagCmd, 0, 0,
	     "isMustIsolate attribute");
  
  UmlClass * expansionkind = 
    UmlClass::made(base_class_view, base_depl_view, "anExpansionKind");

  expansionkind->set_Stereotype("enum_pattern");
  expansionkind->set_CppDecl(CppSettings::enumDecl());
  expansionkind->set_JavaDecl(JavaSettings::enumPatternDecl());
  expansionkind->add_enum_item("parallelExecution");
  expansionkind->add_enum_item("iterativeExecution");
  expansionkind->add_enum_item("streamExecution");
  
  defGetEnum(base_expansionregion, _mode, mode, expansionkind, 0, 0,
		   "mode attribute, the way in which the execution interact.");
  defSetEnum(base_expansionregion, _mode, set_Mode, expansionkind, setDefCmd, 0, 0,
		   "mode attribute");
  
  base_expansionregion->add_attribute("_must_isolate", PrivateVisibility, "bool", 0, 0);  
  base_expansionregion->add_relation(aDirectionalAggregationByValue,
				     "_mode", PrivateVisibility,
				     expansionkind, 0, 0);
  
  op = base_expansionregion->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityRegion::read_uml_();\n"
	      "  _must_isolate = UmlCom::read_bool();\n"
	      "  _mode = (anExpansionKind) UmlCom::read_char();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	      "  _must_isolate = UmlCom.read_bool();\n"
	      "  _mode = anExpansionKind.fromInt(UmlCom.read_char());\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // interruptible region
  
  UmlClass * base_interruptibleregion;
  UmlClass * user_interruptibleregion;
  
  user_activityregion->made(base_class_view, user_class_view,
			    base_depl_view, user_depl_view,
			    base_item, "InterruptibleActivityRegion",
			    base_interruptibleregion, user_interruptibleregion,
			    0);
  
  base_interruptibleregion->add_default_base_op(user_activityregion, user_interruptibleregion,
						user_item, "interruptible activity region", "anInterruptibleActivityRegion");
  user_interruptibleregion->add_constr(base_interruptibleregion, PublicVisibility);
}

void add_activityaction(UmlClassView * base_class_view, UmlClassView * user_class_view,
			UmlDeploymentView * base_depl_view,
			UmlDeploymentView * user_depl_view,
			UmlClass * base_item, UmlClass * user_item, 
			UmlClass * user_activitynode)
{
  UmlClass * user_diagram = UmlClass::get("UmlDiagram", 0);
  UmlClass * base_activityaction;
  UmlClass * user_activityaction;
  
  user_activitynode->made(base_class_view, user_class_view,
			  base_depl_view, user_depl_view,
			  base_item, "ActivityAction",
			  base_activityaction, user_activityaction,
			  0);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  base_activityaction->add_constr(user_activitynode, ProtectedVisibility);
  include_umlcom(base_activityaction);
  
  UmlCom::set_user_id(uid);
  
  user_activityaction->add_constr(base_activityaction, ProtectedVisibility);
  user_activityaction->set_isAbstract(TRUE);
  
  UmlCom::set_user_id(0);
  
  add_pre_post_conditions(base_activityaction);
  
  add_assoc_diag_ops(base_activityaction, user_diagram);

  UmlOperation * op;
  
  op = base_activityaction->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _pre_condition = 0;\n"
	      "  _post_condition = 0;\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp_pre_condition = 0;\n"
	      "  _cpp_post_condition = 0;\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java_pre_condition = 0;\n"
	      "  _java_post_condition = 0;\n"
	      "#endif\n"
	      "  UmlBaseItem::unload(rec, del);\n",
	      FALSE, 0, 0);
  QCString s = op->cppDecl();
  
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _pre_condition = null;\n"
	       "  _post_condition = null;\n"
	       "  _cpp_pre_condition = null;\n"
	       "  _cpp_post_condition = null;\n"
	       "  _java_pre_condition = null;\n"
	       "  _java_post_condition = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE. \n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
    
  base_activityaction->add_attribute("_pre_condition", PrivateVisibility, "string",
				     0, 0);
  base_activityaction->add_attribute("_post_condition", PrivateVisibility, "string",
				     0, 0);
  base_activityaction->add_attribute("_cpp_pre_condition", PrivateVisibility, "string",
				     "WITHCPP", 0);
  base_activityaction->add_attribute("_cpp_post_condition", PrivateVisibility, "string",
				     0, "endif");
  base_activityaction->add_attribute("_java_pre_condition", PrivateVisibility, "string",
				     "WITHJAVA", 0);
  base_activityaction->add_attribute("_java_post_condition", PrivateVisibility, "string",
				     0, "endif");
  base_activityaction->add_relation(aDirectionalAssociation,
				    "_assoc_diagram", PrivateVisibility,
				    user_diagram, 0, 0);  
  
  op = base_activityaction->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _assoc_diagram = (UmlDiagram *) UmlBaseItem::read_();\n"
	      "  UmlBaseItem::read_uml_();\n"
	      "  _pre_condition = UmlCom::read_string();\n"
	      "  _post_condition = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  _assoc_diagram = (UmlDiagram) UmlBaseItem.read_();\n"
	       "  super.read_uml_();\n"
	       "  _pre_condition = UmlCom.read_string();\n"
	       "  _post_condition = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_activityaction->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _cpp_pre_condition = UmlCom::read_string();\n"
	      "  _cpp_post_condition = UmlCom::read_string();\n",
	      FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "",
	       "  _cpp_pre_condition = UmlCom.read_string();\n"
	       "  _cpp_post_condition = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_activityaction->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	       "  _java_pre_condition = UmlCom::read_string();\n"
	       "  _java_post_condition = UmlCom::read_string();\n",
	      FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "",
	       "  _java_pre_condition = UmlCom.read_string();\n"
	       "  _java_post_condition = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // the artifacts containing all
  
  UmlCom::set_user_id(0);
  
  UmlArtifact * base_art = UmlArtifact::made(base_depl_view, "UmlBaseActivityActionClasses");
  
  UmlCom::set_user_id(uid);
  
  UmlArtifact * user_art = UmlArtifact::made(user_depl_view, "UmlActivityActionClasses");
  
  include_umlcom(base_art);
  
  // send object
  
  UmlClass * base_sendobject;
  UmlClass * user_sendobject;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "SendObjectAction",
			    base_sendobject, user_sendobject,
			    0);
  
  base_sendobject->add_default_base_op(user_activityaction, user_sendobject,
				       user_item,
				       "send object action", "aSendObjectAction");
  user_sendobject->add_constr(base_sendobject, PublicVisibility);

  // unmarshall action
  
  UmlClass * base_unmarshall;
  UmlClass * user_unmarshall;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "UnmarshallAction",
			    base_unmarshall, user_unmarshall,
			    0);
  
  base_unmarshall->add_default_base_op(user_activityaction, user_unmarshall,
				       user_item,
				       "unmarshall action", "anUnmarshallAction");
  user_unmarshall->add_constr(base_unmarshall, PublicVisibility);

  // on signal action
  
  UmlClass * base_onsignal;
  UmlClass * user_onsignal;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_depl_view, user_depl_view,
			    base_item, "OnSignalAction",
			    base_onsignal, user_onsignal,
			    0);
  
  UmlCom::set_user_id(0);
  
  base_onsignal->add_constr(user_activityaction, ProtectedVisibility);
  include_umlcom(base_onsignal);
  
  UmlCom::set_user_id(uid);
  
  user_onsignal->add_constr(base_onsignal, ProtectedVisibility);
  user_onsignal->set_isAbstract(TRUE);

  UmlCom::set_user_id(0);
  
  defGet(base_onsignal, _signal, signal, "string", 0, 0,
	 "signal");
  defSet(base_onsignal, _signal, set_Signal, "str", setUmlActivityCmd, 0, 0,
	 "signal");
  defGet(base_onsignal, _cpp_signal, cppSignal, "string", "WITHCPP", 0,
	 "signal in C++");
  defSet(base_onsignal, _cpp_signal, set_CppSignal, "str", setCppActivityCmd, 0, "#endif",
	 "signal in C++");
  defGet(base_onsignal, _java_signal, javaSignal, "string", "WITHJAVA", 0,
	 "signal in Java");
  defSet(base_onsignal, _java_signal, set_JavaSignal, "str", setJavaActivityCmd, 0, "#endif",
	 "signal in Java");

  op = base_onsignal->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _signal = 0;\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp_signal = 0;\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java_signal = 0;\n"
	      "#endif\n"
	      "  UmlBaseActivityAction::unload(rec, del);\n",
	      FALSE, 0, 0);
  s = op->cppDecl();
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _signal = null;\n"
	       "  _cpp_signal = null;\n"
	       "  _java_signal = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE. \n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
    
  base_onsignal->add_attribute("_signal", PrivateVisibility, "string",
				 0, 0);
  base_onsignal->add_attribute("_cpp_signal", PrivateVisibility, "string",
				 "WITHCPP", "endif");
  base_onsignal->add_attribute("_java_signal", PrivateVisibility, "string",
				 "WITHJAVA", "endif");

  op = base_onsignal->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _signal = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _signal = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_onsignal->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_cpp_();\n"
	      "  _cpp_signal = UmlCom::read_string();\n",
	      FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "",
	       "  super.read_cpp_();\n"
	       "  _cpp_signal = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_onsignal->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_java_();\n"
	      "  _java_signal = UmlCom::read_string();\n",
	      FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "",
	       "  super.read_java_();\n"
	       "  _java_signal = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // send signal action
  
  UmlClass * base_sendsignal;
  UmlClass * user_sendsignal;

  
  user_onsignal->made(base_class_view, user_class_view,
		      base_art, user_art,
		      base_item, "SendSignalAction",
		      base_sendsignal, user_sendsignal,
		      0);
  
  base_sendsignal->add_default_base_op(user_onsignal, user_sendsignal,
				       user_item,
				       "send signal action", "aSendSignalAction");
  user_sendsignal->add_constr(base_sendsignal, PublicVisibility);
  
  // broadcast signal action
  
  UmlClass * base_broadcastsignal;
  UmlClass * user_broadcastsignal;

  
  user_onsignal->made(base_class_view, user_class_view,
		      base_art, user_art,
		      base_item, "BroadcastSignalAction",
		      base_broadcastsignal, user_broadcastsignal,
		      0);
  
  base_broadcastsignal->add_default_base_op(user_onsignal, user_broadcastsignal,
					    user_item,
					    "broadcast signal action", "aBroadcastSignalAction");
  user_broadcastsignal->add_constr(base_broadcastsignal, PublicVisibility);
  
  // value specification action
  
  UmlClass * base_valuespecification;
  UmlClass * user_valuespecification;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "ValueSpecificationAction",
			    base_valuespecification, user_valuespecification,
			    0);
  
  base_valuespecification->add_default_base_op(user_activityaction, user_valuespecification,
					       user_item,
					       "value specification action", "aValueSpecificationAction");
  user_valuespecification->add_constr(base_valuespecification, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGet(base_valuespecification, _value, value, "string", 0, 0,
	 "value");
  defSet(base_valuespecification, _value, set_Value, "str", setUmlActivityCmd, 0, 0,
	 "value");
  defGet(base_valuespecification, _cpp_value, cppValue, "string", "WITHCPP", 0,
	 "value in C++");
  defSet(base_valuespecification, _cpp_value, set_CppValue, "str", setCppActivityCmd, 0, "#endif",
	 "value in C++");
  defGet(base_valuespecification, _java_value, javaValue, "string", "WITHJAVA", 0,
	 "value in Java");
  defSet(base_valuespecification, _java_value, set_JavaValue, "str", setJavaActivityCmd, 0, "#endif",
	 "value in Java");

  op = base_valuespecification->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _value = 0;\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp_value = 0;\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java_value = 0;\n"
	      "#endif\n"
	      "  UmlBaseActivityAction::unload(rec, del);\n",
	      FALSE, 0, 0);
  s = op->cppDecl();
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _value = null;\n"
	       "  _cpp_value = null;\n"
	       "  _java_value = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE. \n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
    
  base_valuespecification->add_attribute("_value", PrivateVisibility, "string",
					 0, 0);
  base_valuespecification->add_attribute("_cpp_value", PrivateVisibility, "string",
					 "WITHCPP", "endif");
  base_valuespecification->add_attribute("_java_value", PrivateVisibility, "string",
					 "WITHJAVA", "endif");

  op = base_valuespecification->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _value = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _value = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_valuespecification->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_cpp_();\n"
	      "  _cpp_value = UmlCom::read_string();\n",
	      FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "",
	       "  super.read_cpp_();\n"
	       "  _cpp_value = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_valuespecification->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_java_();\n"
	      "  _java_value = UmlCom::read_string();\n",
	      FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "",
	       "  super.read_java_();\n"
	       "  _java_value = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // opaque action
  
  UmlClass * base_opaque;
  UmlClass * user_opaque;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "OpaqueAction",
			    base_opaque, user_opaque,
			    0);
  
  base_opaque->add_default_base_op(user_activityaction, user_opaque,
				   user_item,
				   "opaque action", "anOpaqueAction");
  user_opaque->add_constr(base_opaque, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGet(base_opaque, _behavior, behavior, "string", 0, 0,
	 "behavior");
  defSet(base_opaque, _behavior, set_Behavior, "str", setUmlActivityCmd, 0, 0,
	 "behavior");
  defGet(base_opaque, _cpp_behavior, cppBehavior, "string", "WITHCPP", 0,
	 "behavior in C++");
  defSet(base_opaque, _cpp_behavior, set_CppBehavior, "str", setCppActivityCmd, 0, "#endif",
	 "behavior in C++");
  defGet(base_opaque, _java_behavior, javaBehavior, "string", "WITHJAVA", 0,
	 "behavior in Java");
  defSet(base_opaque, _java_behavior, set_JavaBehavior, "str", setJavaActivityCmd, 0, "#endif",
	 "behavior in Java");

  op = base_opaque->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _behavior = 0;\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp_behavior = 0;\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java_behavior = 0;\n"
	      "#endif\n"
	      "  UmlBaseActivityAction::unload(rec, del);\n",
	      FALSE, 0, 0);
  s = op->cppDecl();
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _behavior = null;\n"
	       "  _cpp_behavior = null;\n"
	       "  _java_behavior = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE. \n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
    
  base_opaque->add_attribute("_behavior", PrivateVisibility, "string",
			     0, 0);
  base_opaque->add_attribute("_cpp_behavior", PrivateVisibility, "string",
			     "WITHCPP", "endif");
  base_opaque->add_attribute("_java_behavior", PrivateVisibility, "string",
			     "WITHJAVA", "endif");

  op = base_opaque->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _behavior = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _behavior = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_opaque->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_cpp_();\n"
	      "  _cpp_behavior = UmlCom::read_string();\n",
	      FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "",
	       "  super.read_cpp_();\n"
	       "  _cpp_behavior = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_opaque->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_java_();\n"
	      "  _java_behavior = UmlCom::read_string();\n",
	      FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "",
	       "  super.read_java_();\n"
	       "  _java_behavior = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // accept event action
  
  UmlClass * base_acceptevent;
  UmlClass * user_acceptevent;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "AcceptEventAction",
			    base_acceptevent, user_acceptevent,
			    0);
  
  base_acceptevent->add_default_base_op(user_activityaction, user_acceptevent,
					user_item,
					"accept event action", "anAcceptEventAction");
  user_acceptevent->add_constr(base_acceptevent, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGetBool(base_acceptevent, _unmarshall, isUnmarshall, 0, 0,
	     "isUnmarshall attribute, if TRUE there are multiple output pins for attributes or the event.");
  defSetBool(base_acceptevent, _unmarshall, set_isUnmarshall, setUnmarshallCmd, 0, 0,
	     "isUnmarshall attribute");
  
  defGetBool(base_acceptevent, _timeevent, isTimeEvent, 0, 0,
	     "isTimeEvent attribute, if TRUE the event is a time event");
  defSetBool(base_acceptevent, _timeevent, set_isTimeEvent, setTimeEventCmd, 0, 0,
	     "isTimeEvent attribute");
  
  defGet(base_acceptevent, _trigger, trigger, "string", 0, 0,
	 "trigger");
  defSet(base_acceptevent, _trigger, set_Trigger, "str", setUmlTriggerCmd, 0, 0,
	 "trigger");
  defGet(base_acceptevent, _cpp_trigger, cppTrigger, "string", "WITHCPP", 0,
	 "trigger in C++");
  defSet(base_acceptevent, _cpp_trigger, set_CppTrigger, "str", setCppTriggerCmd, 0, "#endif",
	 "trigger in C++");
  defGet(base_acceptevent, _java_trigger, javaTrigger, "string", "WITHJAVA", 0,
	 "trigger in Java");
  defSet(base_acceptevent, _java_trigger, set_JavaTrigger, "str", setJavaTriggerCmd, 0, "#endif",
	 "trigger in Java");

  op = base_acceptevent->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _trigger = 0;\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp_trigger = 0;\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java_trigger = 0;\n"
	      "#endif\n"
	      "  UmlBaseActivityAction::unload(rec, del);\n",
	      FALSE, 0, 0);
  s = op->cppDecl();
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _trigger = null;\n"
	       "  _cpp_trigger = null;\n"
	       "  _java_trigger = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE. \n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
    
  base_acceptevent->add_attribute("_unmarshall", PrivateVisibility, "bool",
				  0, 0);
  base_acceptevent->add_attribute("_timeevent", PrivateVisibility, "bool",
				  0, 0);
  
  base_acceptevent->add_attribute("_trigger", PrivateVisibility, "string",
				  0, 0);
  base_acceptevent->add_attribute("_cpp_trigger", PrivateVisibility, "string",
				  "WITHCPP", "endif");
  base_acceptevent->add_attribute("_java_trigger", PrivateVisibility, "string",
				  "WITHJAVA", "endif");

  op = base_acceptevent->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _unmarshall = UmlCom::read_bool();\n"
	      "  _timeevent = UmlCom::read_bool();\n"
	      "  _trigger = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _unmarshall = UmlCom.read_bool();\n"
	       "  _timeevent = UmlCom.read_bool();\n"
	       "  _trigger = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_acceptevent->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_cpp_();\n"
	      "  _cpp_trigger = UmlCom::read_string();\n",
	      FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "",
	       "  super.read_cpp_();\n"
	       "  _cpp_trigger = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_acceptevent->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_java_();\n"
	      "  _java_trigger = UmlCom::read_string();\n",
	      FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "",
	       "  super.read_java_();\n"
	       "  _java_trigger = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // call operation action
  
  UmlClass * base_calloperation;
  UmlClass * user_calloperation;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "CallOperationAction",
			    base_calloperation, user_calloperation,
			    0);
  
  base_calloperation->add_default_base_op(user_activityaction, user_calloperation,
					  user_item,
					  "call operation action", "aCallOperationAction");
  user_calloperation->add_constr(base_calloperation, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGetBool(base_calloperation, _synchronous, isSynchronous, 0, 0,
	     "isSynchronous attribute, if TRUE the caller waits for the completion of the invoked behavior");
  defSetBool(base_calloperation, _synchronous, set_isSynchronous, setFlagCmd, 0, 0,
	     "isSynchronous attribute");
  
  UmlClass * user_operation = UmlClass::get("UmlOperation", 0);
  
  defGetPtr(base_calloperation, _operation, operation, user_operation, 0, 0,
	    "operation");
  defSetPtr(base_calloperation, _operation, set_Operation, user_operation, setDefCmd, 0, 0,
	    "operation");

  base_calloperation->add_attribute("_synchronous", PrivateVisibility, "bool",
				    0, 0);

  base_calloperation->add_relation(aDirectionalAssociation,
				   "_operation", PrivateVisibility,
				   user_operation, 0, 0);  
  
  op = base_calloperation->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _synchronous = UmlCom::read_bool();\n"
	      "  _operation = (UmlOperation *) UmlBaseItem::read_();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _synchronous = UmlCom.read_bool();\n"
	       "  _operation = (UmlOperation) UmlBaseItem.read_();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // call behavior action
  
  UmlClass * base_callbehavior;
  UmlClass * user_callbehavior;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "CallBehaviorAction",
			    base_callbehavior, user_callbehavior,
			    0);
  
  base_callbehavior->add_default_base_op(user_activityaction, user_callbehavior,
					 user_item,
					 "call behavior action", "aCallBehaviorAction");
  user_callbehavior->add_constr(base_callbehavior, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGetBool(base_callbehavior, _synchronous, isSynchronous, 0, 0,
	     "isSynchronous attribute, if TRUE the caller waits for the completion of the invoked behavior");
  defSetBool(base_callbehavior, _synchronous, set_isSynchronous, setFlagCmd, 0, 0,
	     "isSynchronous attribute");
  
  defGetPtr(base_callbehavior, _behavior, behavior, user_item, 0, 0,
	    "behavior, may be an activity or a state machine");
  defSetPtr(base_callbehavior, _behavior, set_Behavior, user_item, setDefCmd, 0, 0,
	    "behavior");

  base_callbehavior->add_attribute("_synchronous", PrivateVisibility, "bool",
				   0, 0);

  base_callbehavior->add_relation(aDirectionalAssociation,
				  "_behavior", PrivateVisibility,
				  user_item, 0, 0);  
  
  op = base_callbehavior->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _synchronous = UmlCom::read_bool();\n"
	      "  _behavior = UmlBaseItem::read_();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _synchronous = UmlCom.read_bool();\n"
	       "  _behavior = UmlBaseItem.read_();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // access variable value action
  
  UmlClass * base_accessvariablevalue;
  UmlClass * user_accessvariablevalue;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_depl_view, user_depl_view,
			    base_item, "AccessVariableValueAction",
			    base_accessvariablevalue, user_accessvariablevalue,
			    0);
  

  UmlCom::set_user_id(0);
  
  base_accessvariablevalue->add_constr(user_activityaction, ProtectedVisibility);
  include_umlcom(base_accessvariablevalue);

  UmlCom::set_user_id(uid);
  
  user_accessvariablevalue->add_constr(base_accessvariablevalue, ProtectedVisibility);
  user_accessvariablevalue->set_isAbstract(TRUE);

  UmlCom::set_user_id(0);
  
  defGetPtr(base_accessvariablevalue, _variable, variable, user_item, 0, 0,
	    "variable, may be an attribute or a relation");
  defSetPtr(base_accessvariablevalue, _variable, set_Variable, user_item, setDefCmd, 0, 0,
	    "variable");

  base_accessvariablevalue->add_relation(aDirectionalAssociation,
					 "_variable", PrivateVisibility,
					 user_item, 0, 0);  
  
  op = base_accessvariablevalue->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _variable = UmlBaseItem::read_();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _variable = UmlBaseItem.read_();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // clear variable value action
  
  UmlClass * base_clearvariablevalue;
  UmlClass * user_clearvariablevalue;
  
  user_accessvariablevalue->made(base_class_view, user_class_view,
				 base_art, user_art,
				 base_item, "ClearVariableValueAction",
				 base_clearvariablevalue, user_clearvariablevalue,
				 0);
  
  base_clearvariablevalue->add_default_base_op(user_accessvariablevalue, user_clearvariablevalue,
					       user_item,
					       "clear variable value action", "aClearVariableValueAction");
  user_clearvariablevalue->add_constr(base_clearvariablevalue, PublicVisibility);
  
  // read variable value action
  
  UmlClass * base_readvariablevalue;
  UmlClass * user_readvariablevalue;
  
  user_accessvariablevalue->made(base_class_view, user_class_view,
				 base_art, user_art,
				 base_item, "ReadVariableValueAction",
				 base_readvariablevalue, user_readvariablevalue,
				 0);
  
  base_readvariablevalue->add_default_base_op(user_accessvariablevalue, user_readvariablevalue,
					      user_item,
					      "read variable value action", "aReadVariableValueAction");
  user_readvariablevalue->add_constr(base_readvariablevalue, PublicVisibility);
  
  // write variable value action
  
  UmlClass * base_writevariablevalue;
  UmlClass * user_writevariablevalue;
  
  user_accessvariablevalue->made(base_class_view, user_class_view,
				 base_art, user_art,
				 base_item, "WriteVariableValueAction",
				 base_writevariablevalue, user_writevariablevalue,
				 0);
  
  base_writevariablevalue->add_default_base_op(user_accessvariablevalue, user_writevariablevalue,
					       user_item,
					       "write variable value action", "aWriteVariableValueAction");
  user_writevariablevalue->add_constr(base_writevariablevalue, PublicVisibility);
  
  // add variable value action
  
  UmlClass * base_addvariablevalue;
  UmlClass * user_addvariablevalue;
  
  user_accessvariablevalue->made(base_class_view, user_class_view,
				 base_art, user_art,
				 base_item, "AddVariableValueAction",
				 base_addvariablevalue, user_addvariablevalue,
				 0);
  
  base_addvariablevalue->add_default_base_op(user_accessvariablevalue, user_addvariablevalue,
					     user_item,
					     "add variable value action", "anAddVariableValueAction");
  user_addvariablevalue->add_constr(base_addvariablevalue, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGetBool(base_addvariablevalue, _replace_all, isReplaceAll, 0, 0,
	     "isReplaceAll attribute, if TRUE existing values of the variable must be removed before adding the new value");
  defSetBool(base_addvariablevalue, _replace_all, set_isReplaceAll, setFlagCmd, 0, 0,
	     "isReplaceAll attribute");
  
  base_addvariablevalue->add_attribute("_replace_all", PrivateVisibility, "bool",
				  0, 0);

  op = base_addvariablevalue->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _replace_all = UmlCom::read_bool();\n"
	      "  UmlBaseAccessVariableValueAction::read_uml_();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	      "  _replace_all = UmlCom.read_bool();\n"
	       "  super.read_uml_();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // remove variable value action
  
  UmlClass * base_removevariablevalue;
  UmlClass * user_removevariablevalue;
  
  user_accessvariablevalue->made(base_class_view, user_class_view,
				 base_art, user_art,
				 base_item, "RemoveVariableValueAction",
				 base_removevariablevalue, user_removevariablevalue,
				 0);
  
  base_removevariablevalue->add_default_base_op(user_accessvariablevalue, user_removevariablevalue,
						user_item,
						"remove variable value action", "aRemoveVariableValueAction");
  user_removevariablevalue->add_constr(base_removevariablevalue, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGetBool(base_removevariablevalue, _remove_duplicates, isRemoveDuplicates, 0, 0,
	     "isRemoveDuplicates attribute, if TRUE remove duplicates of the value if non-unique");
  defSetBool(base_removevariablevalue, _remove_duplicates, set_isRemoveDuplicates, setFlagCmd, 0, 0,
	     "isRemoveDuplicates attribute");
  
  base_removevariablevalue->add_attribute("_remove_duplicates", PrivateVisibility, "bool",
					  0, 0);

  op = base_removevariablevalue->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _remove_duplicates = UmlCom::read_bool();\n"
	      "  UmlBaseAccessVariableValueAction::read_uml_();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	      "  _remove_duplicates = UmlCom.read_bool();\n"
	       "  super.read_uml_();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
}

void add_activitycontrolnode(UmlClassView * base_class_view, UmlClassView * user_class_view,
			     UmlDeploymentView * base_depl_view,
			     UmlDeploymentView * user_depl_view,
			     UmlClass * base_item, UmlClass * user_item, 
			     UmlClass * user_activitynode)
{
  UmlClass * base_activitycontrolnode;
  UmlClass * user_activitycontrolnode;
  
  user_activitynode->made(base_class_view, user_class_view,
			  base_depl_view, user_depl_view,
			  base_item, "ActivityControlNode",
			  base_activitycontrolnode, user_activitycontrolnode,
			  0);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  base_activitycontrolnode->add_constr(user_activitynode, ProtectedVisibility);
  
  UmlCom::set_user_id(uid);
  
  user_activitycontrolnode->add_constr(base_activitycontrolnode, ProtectedVisibility);
  user_activitycontrolnode->set_isAbstract(TRUE);
  
  // the artifacts containing all
  UmlCom::set_user_id(0);
  
  UmlArtifact * base_art = UmlArtifact::made(base_depl_view, "UmlBaseActivityControlNodeClasses");
  
  UmlCom::set_user_id(uid);
  
  UmlArtifact * user_art = UmlArtifact::made(user_depl_view, "UmlActivityControlNodeClasses");
  
  include_umlcom(base_art);
  
#define DefControlNode(n, k, s)	\
  UmlClass * base_##n;	\
  UmlClass * user_##n;	\
  	\
  user_activitycontrolnode->made(base_class_view, user_class_view,	\
				 base_art, user_art,	\
				 base_item, #n,	\
				 base_##n, user_##n,	\
				 0);	\
  	\
  base_##n->add_default_base_op(user_activitycontrolnode, user_##n,	\
  			       user_item,	\
			       s " activity control node", #k);	\
  user_##n->add_constr(base_##n, PublicVisibility)

  DefControlNode(InitialActivityNode, anInitialActivityNode, "initial");
  DefControlNode(FlowFinalActivityNode, aFlowFinalActivityNode, "flow final");
  DefControlNode(ActivityFinalActivityNode, anActivityFinalActivityNode, "activity final");
  DefControlNode(DecisionActivityNode, aDecisionActivityNode, "decision");
  DefControlNode(MergeActivityNode, aMergeActivityNode, "merge");
  DefControlNode(ForkActivityNode, aForkActivityNode, "fork");
  DefControlNode(JoinActivityNode, aJoinActivityNode, "join");
}
  
UmlClass * add_activityobject(UmlClassView * base_class_view, UmlClassView * user_class_view,
			      UmlDeploymentView * base_depl_view,
			      UmlDeploymentView * user_depl_view,
			      UmlClass * base_item, UmlClass * user_item,
			      UmlClass * user_activitynode,
			      UmlClass * user_activitydiagram)
{
  UmlClass * base_activityobject;
  UmlClass * user_activityobject;
  
  user_activitynode->made(base_class_view, user_class_view,
			  base_depl_view, user_depl_view,
			  base_item, "ActivityObject",
			  base_activityobject, user_activityobject,
			  0);
  base_activityobject->add_default_base_op(user_activitynode, user_activityobject,
					   user_item, "activity object", "anActivityObject");
  user_activityobject->add_constr(base_activityobject, PublicVisibility);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  include_umlcom(base_activityobject);
  
  UmlClass * ordering = 
    UmlClass::made(base_class_view, base_depl_view, "anOrdering");

  ordering->set_Stereotype("enum_pattern");
  ordering->set_CppDecl(CppSettings::enumDecl());
  ordering->set_JavaDecl(JavaSettings::enumPatternDecl());
  ordering->add_enum_item("unordered");
  ordering->add_enum_item("ordered");
  ordering->add_enum_item("lifo");
  ordering->add_enum_item("fifo");
  
  UmlClass * typespec = UmlClass::get("UmlTypeSpec", 0);
  UmlOperation * op;
  
  defGet(base_activityobject, _type, type, typespec, 0, 0,
         "type");
  defSetRefType(base_activityobject, _type, set_Type, typespec, setTypeCmd, 0, 0,
		"type");
  
  defGet(base_activityobject, _multiplicity, multiplicity, "string", 0, 0,
	 "multiplicity");
  defSet(base_activityobject, _multiplicity, set_Multiplicity, "str", setMultiplicityCmd, 0, 0,
	 "multiplicity");

  defGet(base_activityobject, _in_state, inState, "string", 0, 0,
	 "inState attribute");
  defSet(base_activityobject, _in_state, set_InState, "str", setInStateCmd, 0, 0,
	 "inState attribute");

  defGet(base_activityobject, _selection, selection, "string", 0, 0,
	 "selection in OCL");
  defSet(base_activityobject, _selection, set_Selection, "str", setUmlActivityCmd, 0, 0,
	 "selection in OCL");
  defGet(base_activityobject, _cpp_selection, cppSelection, "string", "WITHCPP", 0,
	 "selection in C++");
  defSet(base_activityobject, _cpp_selection, set_CppSelection, "str", setCppActivityCmd, 0, "endif",
	 "selection in C++");
  defGet(base_activityobject, _java_selection, javaSelection, "string", "WITHJAVA", 0,
	 "selection in Java");
  defSet(base_activityobject, _java_selection, set_JavaSelection, "str", setJavaActivityCmd, 0, "endif",
	 "selection in Java");

  defGetEnum(base_activityobject, _ordering, ordering, ordering, 0, 0,
		   "ordering");
  defSetEnum(base_activityobject, _ordering, set_Ordering, ordering,
	     setOrderingCmd, 0, 0, "ordering");
  
  defGetBool(base_activityobject, _is_control, isControlType, 0, 0,
	     "isControlType attribute, tells whether the type of the object node is to be treated as control");
  defSetBoolBitField(base_activityobject, _is_control, set_IsControlType,
		     setFlagCmd, 0, 0, "isControlType attribute");
  
  add_assoc_diag_ops(base_activityobject, user_activitydiagram);

  op = base_activityobject->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _type.explicit_type = 0;\n"
	      "  _multiplicity = 0;\n"
	      "  _in_state = 0;\n"
	      "  _selection = 0;\n"
	      "#ifdef WINCPP\n"
	      "  _cpp_selection = 0;\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java_selection = 0;\n"
	      "#endif\n"
	      "  UmlBaseActivityNode::unload(rec, del);\n",
	      FALSE, 0, 0);

  QCString s = op->cppDecl();
  
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _type = null;\n"
	       "  _multiplicity = null;\n"
	       "  _in_state = null;\n"
	       "  _selection = null;\n"
	       "  _cpp_selection = null;\n"
	       "  _java_selection = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE. \n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
  
  base_activityobject->add_relation(aDirectionalAssociation,
				    "_assoc_diagram", PrivateVisibility,
				    user_activitydiagram, 0, 0);
  
  base_activityobject->add_relation(aDirectionalAggregationByValue,
				     "_type", PrivateVisibility,
				     typespec, 0, 0);
  base_activityobject->add_attribute("_multiplicity", PrivateVisibility, "string", 0, 0);  
  base_activityobject->add_attribute("_in_state", PrivateVisibility, "string", 0, 0);  
  base_activityobject->add_attribute("_selection", PrivateVisibility, "string", 0, 0);  
  base_activityobject->add_attribute("_cpp_selection", PrivateVisibility, "string", "WITHCPP", "endif");  
  base_activityobject->add_attribute("_java_selection", PrivateVisibility, "string", "WITHJAVA", "endif");  
  
  UmlRelation * rel = 
    base_activityobject->add_relation(aDirectionalAggregationByValue,
				     "_ordering", PrivateVisibility,
				     ordering, 0, 0);
  s = rel->cppDecl();
  int index = s.find("${type} ${name}");
  
  if (index != -1) {
    s.insert(index + 15, " : 8");
    rel->set_CppDecl(s);
  }
  
  UmlAttribute * att =
    base_activityobject->add_attribute("_is_control", PrivateVisibility, "bool", 0, 0);  
  
  s = att->cppDecl();
  index = s.find("${type} ${name}");
  if (index != -1) {
    s.insert(index + 15, " : 1");
    att->set_CppDecl(s);
  }
  
  op = base_activityobject->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _assoc_diagram = (kind() == anActivityObject) ? (UmlActivityDiagram *) UmlBaseItem::read_() : (UmlActivityDiagram *) 0;\n"
	      "  UmlBaseActivityNode::read_uml_();\n"
	      "  _type.type = (UmlClass *) UmlBaseItem::read_();\n"
	      "  if (_type.type == 0)\n"
	      "    _type.explicit_type = UmlCom::read_string();\n"
	      "  _multiplicity = UmlCom::read_string();\n"
	      "  _in_state = UmlCom::read_string();\n"
	      "  _selection = UmlCom::read_string();\n"
	      "  _ordering = (anOrdering) UmlCom::read_char();\n"
	      "  _is_control = UmlCom::read_bool();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  _assoc_diagram = (kind() == anItemKind.anActivityObject) ? (UmlActivityDiagram) UmlBaseItem.read_() : null;\n"
	       "  super.read_uml_();\n"
	       "  _type = new UmlTypeSpec();\n"
	       "  _type.type = (UmlClass) UmlBaseItem.read_();\n"
	       "  if (_type.type == null)\n"
	       "    _type.explicit_type = UmlCom.read_string();\n"
	       "  _multiplicity = UmlCom.read_string();\n"
	       "  _in_state = UmlCom.read_string();\n"
	       "  _selection = UmlCom.read_string();\n"
	       "  _ordering = anOrdering.fromInt(UmlCom.read_char());\n"
	       "  _is_control = UmlCom.read_bool();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);

  op = base_activityobject->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _cpp_selection = UmlCom::read_string();\n", FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "", "  _cpp_selection  = UmlCom.read_string();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_activityobject->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _java_selection = UmlCom::read_string();\n", FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "", "  _java_selection = UmlCom.read_string();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  return user_activityobject;
}

void add_expansionnode(UmlClassView * base_class_view, UmlClassView * user_class_view,
		       UmlDeploymentView * base_depl_view,
		       UmlDeploymentView * user_depl_view,
		       UmlClass * base_item, UmlClass * user_activityobject)
{
  UmlClass * base_expansionnode;
  UmlClass * user_expansionnode;
  
  user_activityobject->made(base_class_view, user_class_view,
			    base_depl_view, user_depl_view,
			    base_item, "ExpansionNode",
			    base_expansionnode, user_expansionnode,
			    0);
  base_expansionnode->add_default_base_op(user_activityobject, user_expansionnode,
					  UmlClass::get("UmlExpansionRegion", 0), 
					  "expansion node", "anExpansionNode");
  user_expansionnode->add_constr(base_expansionnode, PublicVisibility);
}

void add_pinparam(UmlClassView * base_class_view, UmlClassView * user_class_view,
		  UmlDeploymentView * base_depl_view,
		  UmlDeploymentView * user_depl_view,
		  UmlClass * base_item, UmlClass * user_item, 
		  UmlClass * user_activityobject)
{
  // UmlPinParameter
  
  UmlClass * base_pinparam;
  UmlClass * user_pinparam;
  
  user_activityobject->made(base_class_view, user_class_view,
			    base_depl_view, user_depl_view,
			    base_item, "PinParameter",
			    base_pinparam, user_pinparam,
			    0);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  base_pinparam->add_constr(user_activityobject, ProtectedVisibility);
  include_umlcom(base_pinparam);
  
  UmlCom::set_user_id(uid);
  
  user_pinparam->add_constr(base_pinparam, ProtectedVisibility);
  user_pinparam->set_isAbstract(TRUE);
  
  UmlCom::set_user_id(0);
  
  UmlClass * effect = 
    UmlClass::made(base_class_view, base_depl_view, "aParameterEffectKind");

  effect->set_Stereotype("enum_pattern");
  effect->set_CppDecl(CppSettings::enumDecl());
  effect->set_JavaDecl(JavaSettings::enumPatternDecl());
  effect->add_enum_item("noEffect");
  effect->add_enum_item("createEffect");
  effect->add_enum_item("readEffect");
  effect->add_enum_item("updateEffect");
  effect->add_enum_item("deleteEffect");
  
  UmlOperation * op;
  
  defGetBool(base_pinparam, _unique, isUnique, 0, 0,
	     "isUnique attribute");
  defSetBoolBitField(base_pinparam, _unique, set_IsUnique, 
		     setUniqueCmd, 0, 0, "isUnique attribute");

  defGetBool(base_pinparam, _exception, isException, 0, 0,
	     "isException attribute, exclusive with isStream");
  defSetBoolBitField(base_pinparam, _exception, set_IsException,
		     replaceExceptionCmd, 0, 0,
		     "isException attribute, exclusive with isStream");

  UmlClass * dir = UmlClass::get("aDirection", 0);
  
  defGetEnum(base_pinparam, _dir, direction, dir, 0, 0, "direction");
  defSetEnum(base_pinparam, _dir, set_Direction, dir, setIdlDirCmd, 0, 0, "direction");

  defGetEnum(base_pinparam, _effect, effect, effect, 0, 0,
		   "effect");
  defSetEnum(base_pinparam, _effect, set_Effect, effect, replaceParameterCmd, 0, 0, "effect");

  UmlAttribute * att;
  QCString s;
  int index;
  
  att = base_pinparam->add_attribute("_unique", PrivateVisibility, "bool", 0, 0);  
  s = att->cppDecl();
  index = s.find("${type} ${name}");
  if (index != -1) {
    s.insert(index + 15, " : 1");
    att->set_CppDecl(s);
  }
  
  att = base_pinparam->add_attribute("_exception", PrivateVisibility, "bool", 0, 0);  
  s = att->cppDecl();
  index = s.find("${type} ${name}");
  if (index != -1) {
    s.insert(index + 15, " : 1");
    att->set_CppDecl(s);
  }
  
  att = base_pinparam->add_attribute("_stream", PrivateVisibility, "bool", 0, 0);  
  s = att->cppDecl();
  index = s.find("${type} ${name}");
  if (index != -1) {
    s.insert(index + 15, " : 1");
    att->set_CppDecl(s);
  }
  
  att = base_pinparam->add_attribute("_dummy", PrivateVisibility, "int", 0, 0);  
  att->set_JavaDecl("");
  s = att->cppDecl();
  index = s.find("${type} ${name}");
  if (index != -1) {
    s.insert(index + 15, " : 4");
    att->set_CppDecl(s);
  }
  
  UmlRelation * rel;
  
  rel = base_pinparam->add_relation(aDirectionalAggregationByValue,
				    "_dir", PrivateVisibility, dir, 0, 0);
  s = rel->cppDecl();
  index = s.find("${type} ${name}");
  if (index != -1) {
    s.insert(index + 15, " : 8");
    rel->set_CppDecl(s);
  }
  
  rel = base_pinparam->add_relation(aDirectionalAggregationByValue,
				    "_effect", PrivateVisibility, effect, 0, 0);
  s = rel->cppDecl();
  index = s.find("${type} ${name}");
  if (index != -1) {
    s.insert(index + 15, " : 8");
    rel->set_CppDecl(s);
  }
  
  op = base_pinparam->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityObject::read_uml_();\n"
	      "  _unique = UmlCom::read_bool();\n"
	      "  _exception = UmlCom::read_bool();\n"
	      "  _stream = UmlCom::read_bool();\n"
	      "  _dir = (aDirection) UmlCom::read_char();\n"
	      "  _effect = (aParameterEffectKind) UmlCom::read_char();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _unique = UmlCom.read_bool();\n"
	       "  _exception = UmlCom.read_bool();\n"
	       "  _stream = UmlCom.read_bool();\n"
	       "  _dir = aDirection.fromInt(UmlCom.read_char());\n"
	       "  _effect = aParameterEffectKind.fromInt(UmlCom.read_char());\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // pin
  
  UmlClass * base_activitypin;
  UmlClass * user_activitypin;
  UmlClass * user_activityaction = UmlClass::get("UmlActivityAction", 0);
  
  user_pinparam->made(base_class_view, user_class_view,
		      base_depl_view, user_depl_view,
		      base_item, "ActivityPin",
		      base_activitypin, user_activitypin,
		      0);
  
  base_activitypin->add_default_base_op(user_pinparam, user_activitypin,
					user_activityaction,
					"activity action pin", "anActivityPin");
  user_activitypin->add_constr(base_activitypin, PublicVisibility);
  
  // activity parameter
  
  UmlClass * base_activityparameter;
  UmlClass * user_activityparameter;
  
  user_pinparam->made(base_class_view, user_class_view,
		      base_depl_view, user_depl_view,
		      base_item, "ActivityParameter",
		      base_activityparameter, user_activityparameter,
		      0);
  
  base_activityparameter->add_default_base_op(user_pinparam, user_activityparameter,
					UmlClass::get("UmlActivity", 0),
					"activity parameter", "anActivityParameter");
  user_activityparameter->add_constr(base_activityparameter, PublicVisibility);
  
  UmlCom::set_user_id(0);
  
  include_umlcom(base_activityparameter);
  
  defGet(base_activityparameter, _default_value, defaultValue, "string", 0, 0,
	 "default value");
  defSet(base_activityparameter, _default_value, set_DefaultValue, "str",
	 setDefaultValueCmd, 0, 0, "default value");
  
  op = base_activityparameter->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _default_value = 0;\n"
	      "  UmlBaseActivityObject::unload(rec, del);\n",
	      FALSE, 0, 0);

  s = op->cppDecl();
  
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _default_value = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE. \n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
  
  base_activityparameter->add_attribute("_default_value", PrivateVisibility, "string", 0, 0);  
  
  op = base_activityparameter->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBasePinParameter::read_uml_();\n"
	      "  _default_value = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _default_value = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // parameter set

  UmlClass * base_parameterset;
  UmlClass * user_parameterset;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "ParameterSet",
		  base_parameterset, user_parameterset,
		  0);
  
  base_parameterset->add_default_base_op(user_item, user_activityparameter,
					 user_activityaction,
					 "parameter set", "aParameterSet");
  user_parameterset->add_constr(base_parameterset, PublicVisibility);
  
  UmlCom::set_user_id(0);
  
  include_umlcom(base_parameterset);
  
  op = base_parameterset->add_op("pins", PublicVisibility, user_activitypin);
  op->set_Description(" return the pins part of the parameter set");
  op->set_cpp("const QVector<${type}> &", "",
	      "  read_if_needed_();\n"
	      "  return _pins;\n",
	      FALSE, 0, 0);
  op->set_java("${type}[]", "",
	       "  read_if_needed_();\n"
	       "  return (UmlActivityPin[]) _pins.clone();\n", FALSE);

  op = base_parameterset->add_op("set_Pins", PublicVisibility, "bool");
  op->set_Description(" set the pins");
  op->add_param(0, InputDirection, "v", user_activitypin);
  op->set_cpp("${type}", "const QVector<${t0}> & ${p0}",
	      "  UmlCom::send_cmd(_identifier, replaceParameterCmd, (const QVector<UmlItem> &) v);\n"
	      "  if (UmlCom::read_bool()) {\n"
	      "    if (_defined) _pins = v;\n"
	      "    return TRUE;\n"
	      "  }\n"
	      "  else\n"
	      "    return FALSE;\n",
	      FALSE, 0, 0);
  op->set_java("void", "${t0}[] ${p0}",
	      "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.replaceParameterCmd, v);\n"
	      "  UmlCom.check();\n"
	      "  _pins = (UmlActivityPin[]) v.clone();\n",
	      FALSE);
  
  op = base_parameterset->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _pins.resize(0);\n"
	      "  UmlBaseItem::unload(rec, del);\n",
	      FALSE, 0, 0);
  s = op->cppDecl();
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _pins = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE. \n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
  
  rel = base_parameterset->add_relation(aDirectionalAssociation,
					"_pins", PrivateVisibility,
					user_activitypin, 0, 0);  
  rel->set_Stereotype("vector");
  rel->set_Multiplicity("*");
  rel->set_CppDecl(CppSettings::relationDecl(TRUE, "*"));

  s = rel->javaDecl();
  index = s.find("${type} ${name}");
  if (index != -1) {
    s.insert(index + 7,"[]");
    rel->set_JavaDecl(s);
  }

  op = base_parameterset->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseItem::read_uml_();\n"
	      "  UmlCom::read_item_list((QVector<UmlItem> &) _pins);\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _pins = (UmlActivityPin[]) UmlCom.read_item_list();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  // update an UmlCom::send_cmd

  const QVector<UmlItem> ch = UmlClass::get("UmlCom", 0)->children();
  UmlClass * cl = UmlClass::get("UmlClass", 0);
  
  UmlCom::trace("update UmlCom::send_cmd(...)<br>\n");
  
  for (index = 0; index != (int) ch.size(); index += 1) {
    if ((ch[index]->kind() == anOperation) &&
	(ch[index]->name() == "send_cmd")) {
      op = (UmlOperation *) ch[index];
      
      QValueList<UmlParameter> params = op->params();
      
      if (params.count() == 3) {
	UmlParameter p = params.last();
	
	if ((p.name == "l") && (p.type.type == cl)) {
	  p.type.type = user_item;
	  op->replaceParameter(2, p);
	  
	  s = op->cppBody();
	  if ((index = s.find("UmlClass")) != -1) {
	    s.replace(index+3, 5, "Item");
	    op->set_CppBody(s);
	  }
	  
	  s = op->javaBody();
	  if ((index = s.find("UmlClass")) != -1) {
	    s.replace(index+3, 5, "Item");
	    op->set_JavaBody(s);
	  }
	  break;
	}
      }
    }
  }
  
  // update UmlBaseArtifact::set_AssociatedClasses
  
  UmlCom::trace("update UmlBaseArtifact::set_AssociatedClasses(...)<br>\n");
  op = UmlClass::get("UmlBaseArtifact", 0)->get_operation("set_AssociatedClasses");
  s = op->cppBody();
  if ((index = s.find("l);")) != -1) {
    s.insert(index, "(const QVector<UmlItem> &) ");
    op->set_CppBody(s);
  }
  
  UmlCom::set_user_id(uid);
}

void add_activities_item_kind()
{
  // warning : anActivityDiagram already exist
  // already root
  static const char * const kinds[] = {
    "anActivity", 
    "aFlow", 
    "anActivityParameter", 
    "aParameterSet", 
    "aPartition", 
    "anExpansionRegion", 
    "anInterruptibleActivityRegion", 
    "anOpaqueAction",
    "anAcceptEventAction",
    "aReadVariableValueAction",
    "aClearVariableValueAction",
    "aWriteVariableValueAction",
    "anAddVariableValueAction",
    "aRemoveVariableValueAction",
    "aCallBehaviorAction",
    "aCallOperationAction",
    "aSendObjectAction",
    "aSendSignalAction",
    "aBroadcastSignalAction",
    "anUnmarshallAction",
    "aValueSpecificationAction",
    "anActivityObject", 
    "anExpansionNode", 
    "anActivityPin", 
    "anInitialActivityNode", 
    "aFlowFinalActivityNode", 
    "anActivityFinalActivityNode", 
    "aDecisionActivityNode", 
    "aMergeActivityNode", 
    "aForkActivityNode", 
    "aJoinActivityNode", 
  };
  UmlClass * itkind = UmlClass::get("anItemKind", 0);
  
  for (int i = 0; i != sizeof(kinds)/sizeof(kinds[0]); i += 1)
    itkind->add_enum_item(kinds[i]);
}

void add_activity_on_instance_cmd()
{
  // already root
  UmlClass * itcmd = UmlClass::get("OnInstanceCmd", 0);
  
  itcmd->add_enum_item("setReadOnlyCmd");
  itcmd->add_enum_item("setSingleExecutionCmd");
  itcmd->add_enum_item("setUnmarshallCmd");
  itcmd->add_enum_item("setTimeEventCmd");
  itcmd->add_enum_item("setFlagCmd");
  itcmd->add_enum_item("setUniqueCmd");
  itcmd->add_enum_item("setStreamCmd");
  itcmd->add_enum_item("setInStateCmd");
  itcmd->add_enum_item("setOrderingCmd");
}    

void baseitem_read_activities(UmlClass * base_item)
{
  // update read_()'s body
  UmlOperation * op = base_item->get_operation("read_");
  
  if (op != 0) {
    QCString body;
    
    body = op->cppBody();
    body.remove(body.findRev("//return new UmlActivityDiagram(id, name);"), 2);
    body.insert(body.findRev("default:"),
		"    case anActivity:\n"
		"      return new UmlActivity(id, name);\n"
		"    case aFlow:\n"
		"      return new UmlFlow(id, name);\n"
		"    case anActivityParameter:\n"
		"      return new UmlActivityParameter(id, name);\n"
		"    case aParameterSet:\n"
		"      return new UmlParameterSet(id, name);\n"
		"    case anExpansionRegion:\n"
		"      return new UmlExpansionRegion(id, name);\n"
		"    case anInterruptibleActivityRegion:\n"
		"      return new UmlInterruptibleActivityRegion(id, name);\n"
		"    case anOpaqueAction:\n"
		"      return new UmlOpaqueAction(id, name);\n"
		"    case anAcceptEventAction:\n"
		"      return new UmlAcceptEventAction(id, name);\n"
		"    case aReadVariableValueAction:\n"
		"      return new UmlReadVariableValueAction(id, name);\n"
		"    case aClearVariableValueAction:\n"
		"      return new UmlClearVariableValueAction(id, name);\n"
		"    case aWriteVariableValueAction:\n"
		"      return new UmlWriteVariableValueAction(id, name);\n"
		"    case anAddVariableValueAction:\n"
		"      return new UmlAddVariableValueAction(id, name);\n"
		"    case aRemoveVariableValueAction:\n"
		"      return new UmlRemoveVariableValueAction(id, name);\n"
		"    case aCallBehaviorAction:\n"
		"      return new UmlCallBehaviorAction(id, name);\n"
		"    case aCallOperationAction:\n"
		"      return new UmlCallOperationAction(id, name);\n"
		"    case aSendObjectAction:\n"
		"      return new UmlSendObjectAction(id, name);\n"
		"    case aSendSignalAction:\n"
		"      return new UmlSendSignalAction(id, name);\n"
		"    case aBroadcastSignalAction:\n"
		"      return new UmlBroadcastSignalAction(id, name);\n"
		"    case anUnmarshallAction:\n"
		"      return new UmlUnmarshallAction(id, name);\n"
		"    case aValueSpecificationAction:\n"
		"      return new UmlValueSpecificationAction(id, name);\n"
		"    case anActivityObject:\n"
		"      return new UmlActivityObject(id, name);\n"
		"    case anExpansionNode:\n"
		"      return new UmlExpansionNode(id, name);\n"
		"    case anActivityPin:\n"
		"      return new UmlActivityPin(id, name);\n"
		"    case anInitialActivityNode:\n"
		"      return new UmlInitialActivityNode(id, name);\n"
		"    case aFlowFinalActivityNode:\n"
		"      return new UmlFlowFinalActivityNode(id, name);\n"
		"    case anActivityFinalActivityNode:\n"
		"      return new UmlActivityFinalActivityNode(id, name);\n"
		"    case aDecisionActivityNode:\n"
		"      return new UmlDecisionActivityNode(id, name);\n"
		"    case aMergeActivityNode:\n"
		"      return new UmlMergeActivityNode(id, name);\n"
		"    case aForkActivityNode:\n"
		"      return new UmlForkActivityNode(id, name);\n"
		"    case aJoinActivityNode:\n"
		"      return new UmlJoinActivityNode(id, name);\n"
		"    case aPartition:\n"
		"      //return new UmlPartition(id, name);\n"
		"    ");
    op->set_CppBody(body);
      
    body = op->javaBody();
    body.remove(body.findRev("//return new UmlActivityDiagram(id, name);"), 2);
    body.insert(body.findRev("default:"),
		"    case anItemKind._anActivity:\n"
		"      return new UmlActivity(id, name);\n"
		"    case anItemKind._aFlow:\n"
		"      return new UmlFlow(id, name);\n"
		"    case anItemKind._anActivityParameter:\n"
		"      return new UmlActivityParameter(id, name);\n"
		"    case anItemKind._aParameterSet:\n"
		"      return new UmlParameterSet(id, name);\n"
		"    case anItemKind._anExpansionRegion:\n"
		"      return new UmlExpansionRegion(id, name);\n"
		"    case anItemKind._anInterruptibleActivityRegion:\n"
		"      return new UmlInterruptibleActivityRegion(id, name);\n"
		"    case anItemKind._anOpaqueAction:\n"
		"      return new UmlOpaqueAction(id, name);\n"
		"    case anItemKind._anAcceptEventAction:\n"
		"      return new UmlAcceptEventAction(id, name);\n"
		"    case anItemKind._aReadVariableValueAction:\n"
		"      return new UmlReadVariableValueAction(id, name);\n"
		"    case anItemKind._aClearVariableValueAction:\n"
		"      return new UmlClearVariableValueAction(id, name);\n"
		"    case anItemKind._aWriteVariableValueAction:\n"
		"      return new UmlWriteVariableValueAction(id, name);\n"
		"    case anItemKind._anAddVariableValueAction:\n"
		"      return new UmlAddVariableValueAction(id, name);\n"
		"    case anItemKind._aRemoveVariableValueAction:\n"
		"      return new UmlRemoveVariableValueAction(id, name);\n"
		"    case anItemKind._aCallBehaviorAction:\n"
		"      return new UmlCallBehaviorAction(id, name);\n"
		"    case anItemKind._aCallOperationAction:\n"
		"      return new UmlCallOperationAction(id, name);\n"
		"    case anItemKind._aSendObjectAction:\n"
		"      return new UmlSendObjectAction(id, name);\n"
		"    case anItemKind._aSendSignalAction:\n"
		"      return new UmlSendSignalAction(id, name);\n"
		"    case anItemKind._aBroadcastSignalAction:\n"
		"      return new UmlBroadcastSignalAction(id, name);\n"
		"    case anItemKind._anUnmarshallAction:\n"
		"      return new UmlUnmarshallAction(id, name);\n"
		"    case anItemKind._aValueSpecificationAction:\n"
		"      return new UmlValueSpecificationAction(id, name);\n"
		"    case anItemKind._anActivityObject:\n"
		"      return new UmlActivityObject(id, name);\n"
		"    case anItemKind._anExpansionNode:\n"
		"      return new UmlExpansionNode(id, name);\n"
		"    case anItemKind._anActivityPin:\n"
		"      return new UmlActivityPin(id, name);\n"
		"    case anItemKind._anInitialActivityNode:\n"
		"      return new UmlInitialActivityNode(id, name);\n"
		"    case anItemKind._aFlowFinalActivityNode:\n"
		"      return new UmlFlowFinalActivityNode(id, name);\n"
		"    case anItemKind._anActivityFinalActivityNode:\n"
		"      return new UmlActivityFinalActivityNode(id, name);\n"
		"    case anItemKind._aDecisionActivityNode:\n"
		"      return new UmlDecisionActivityNode(id, name);\n"
		"    case anItemKind._aMergeActivityNode:\n"
		"      return new UmlMergeActivityNode(id, name);\n"
		"    case anItemKind._aForkActivityNode:\n"
		"      return new UmlForkActivityNode(id, name);\n"
		"    case anItemKind._aJoinActivityNode:\n"
		"      return new UmlJoinActivityNode(id, name);\n"
		"    case anItemKind._aPartition:\n"
		"      //return new UmlPartition(id, name);\n"
		"    ");
    op->set_JavaBody(body);
  }
  
  // update UmlBaseItem artifact
  UmlArtifact * art = base_item->associatedArtifact();
  QCString s;
  
  s = art->cppSource();
  s.insert(s.find("#include \"MiscGlobalCmd.h\""),
	   "#include \"UmlActivityDiagram.h\"\n"
	   "#include \"UmlActivity.h\"\n"
	   "#include \"UmlFlow.h\"\n"
	   "#include \"UmlActivityParameter.h\"\n"
	   "#include \"UmlParameterSet.h\"\n"
	   "#include \"UmlExpansionRegion.h\"\n"
	   "#include \"UmlInterruptibleActivityRegion.h\"\n"
	   "#include \"UmlActivityActionClasses.h\"\n"
	   "#include \"UmlActivityObject.h\"\n"
	   "#include \"UmlExpansionNode.h\"\n"
	   "#include \"UmlActivityPin.h\"\n"
	   "#include \"UmlActivityControlNodeClasses.h\"\n"
	   );
  art->set_CppSource(s);
}

void add_activity(UmlClass * base_item, UmlClass * user_item)
{
  UmlClassView * base_class_view = (UmlClassView *) base_item->parent();
  UmlClassView * user_class_view = (UmlClassView *) user_item->parent();
  UmlDeploymentView * base_depl_view = (UmlDeploymentView *)
    base_item->associatedArtifact()->parent();
  UmlDeploymentView * user_depl_view = (UmlDeploymentView *)
    user_item->associatedArtifact()->parent();

  UmlCom::trace("Activity<br>\n");
  
  UmlClass * user_activityitem = 
    UmlClass::made(user_class_view, user_depl_view, "UmlActivityItem", TRUE);

  add_activity(base_class_view, user_class_view, base_depl_view, user_depl_view,
	       base_item, user_item, user_activityitem);

  UmlClass * user_activitynode = 
    add_activitynode(base_class_view, user_class_view, base_depl_view, user_depl_view,
		     base_item, user_item, user_activityitem);

  add_flow(base_class_view, user_class_view, base_depl_view, user_depl_view,
	   base_item, user_item, user_activityitem, user_activitynode);
  
  UmlClass * user_diagram = UmlClass::get("UmlActivityDiagram", 0);
  
  add_activityregion(base_class_view, user_class_view, base_depl_view, user_depl_view,
		     base_item, user_item, user_activityitem, user_diagram);

  add_activityaction(base_class_view, user_class_view, base_depl_view, user_depl_view,
		     base_item, user_item, user_activitynode);
  
  add_activitycontrolnode(base_class_view, user_class_view, base_depl_view, user_depl_view,
			  base_item, user_item, user_activitynode);
  
  UmlClass * user_activityobject =
    add_activityobject(base_class_view, user_class_view, base_depl_view, user_depl_view,
		     base_item, user_item, user_activitynode, user_diagram);
  
  add_expansionnode(base_class_view, user_class_view, base_depl_view, user_depl_view,
		    base_item, user_activityobject);
  
  add_pinparam(base_class_view, user_class_view, base_depl_view, user_depl_view,
	       base_item, user_item, user_activityobject);
  
  //

  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("update anItemKind<br>\n");
  
  add_activities_item_kind();
  
  UmlCom::trace("update cmd list<br>\n");
  
  add_activity_on_instance_cmd();
  
  UmlCom::trace("update item read<br>\n");
  
  baseitem_read_activities(base_item);
  
  UmlCom::trace("update aDirection<br>\n");
  
  add_return_direction();
  
  UmlCom::set_user_id(uid);
}

//
// add void UmlCom::showTrace() and
// void UmlCom::traceAutoRaise(bool y)
//
// fixe write_bool definition in java
//

void add_new_trace_operations(UmlClass * uml_com)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("update UmlCom<br>\n");
  
  UmlOperation * op1 =
    uml_com->add_op("showTrace", PublicVisibility, "void", FALSE);
  
  op1->set_Description("to show the trace window\n");
  op1->set_isClassMember(TRUE);
  
  op1->set_cpp("${type}", "",
	      "  send_cmd(miscGlobalCmd, showTraceCmd);\n",
	      FALSE, 0, 0);

  op1->set_java("${type}", "",
	       "  send_cmd(CmdFamily.miscGlobalCmd, MiscGlobalCmd._showTraceCmd);\n",
	       FALSE);
  op1->moveAfter(uml_com->get_operation("trace"));
  
  //
  
  UmlOperation * op2 =
    uml_com->add_op("traceAutoRaise", PublicVisibility, "void", FALSE);
  
  op2->add_param(0, InputDirection, "y", "bool");

  op2->set_Description("to automatically raise the trace window\n"
		      "each time trace() is called\n");
  op2->set_isClassMember(TRUE);
  
  op2->set_cpp("${type}", "${t0} ${p0}",
	      "  send_cmd(miscGlobalCmd, traceAutoRaiseCmd, (y == 0) ? 0 : 1);\n",
	      FALSE, 0, 0);

  op2->set_java("${type}", "${t0} ${p0}",
	       "  send_cmd(CmdFamily.miscGlobalCmd, MiscGlobalCmd._traceAutoRaiseCmd, (y) ? (byte) 1 : (byte) 0);\n",
	       FALSE);
  op2->moveAfter(op1);
  
  //
  
  UmlOperation * op3 = uml_com->get_operation("write_bool");
    
  if (op3 != 0)
    op3->set_JavaBody("  check_size_out(1);\n"
		      "\n"
		      "  buffer_out[p_buffer_out++] = (b) ? (byte) 1 : (byte) 0;\n");
  
  // add new global cmd

  UmlClass * cl = UmlClass::get("MiscGlobalCmd", 0);
  UmlAttribute * at1 = cl->add_enum_item("showTraceCmd");
  UmlAttribute * at2;
  
  if ((at2 = uml_com->get_attribute("loadCmd")) != 0)
    at1->moveAfter(at2);
  
  cl->add_enum_item("traceAutoRaiseCmd")->moveAfter(at1);
  
  // update UmlCom description
  uml_com->set_Description(
			   " This class manages the communications\n"
			   "\n"
			   " This class may be defined as a 'singleton', but I prefer to use static \n"
			   " members allowing to just write 'UmlCom::member' rather than\n"
			   " 'UmlCom::instance()->member' or other long sentence like this.\n"
			   "\n"
			   " The operation you can use yourself are :\n"
			   "\n"
			   " - connect()\n"
			   "\n"
			   " - targetItem()\n"
			   "\n"
			   " - trace()\n"
			   "\n"
			   " - showTrace()\n"
			   "\n"
			   " - traceAutoRaise()\n"
			   "\n"
			   " - message()\n"
			   "\n"
			   " - bye()\n"
			   "\n"
			   " - close()\n"
			   " \n"
			   " you must NOT call the others\n"
			   );
  //
  
  UmlCom::set_user_id(uid);
}
     
//
//
//

void fixe_activity(UmlClass * base_pinparam)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("update UmlBasePinParameter<br>\n");
  
  UmlOperation * op;
  
  defGetBool(base_pinparam, _stream, isStream, 0, 0,
	     "isStream attribute");
  op->moveAfter(base_pinparam->get_operation("set_IsException"));
  
  UmlOperation * op1 = op;
  
  
  defSetBoolBitField(base_pinparam, _stream, set_IsStream, 
		     setStreamCmd, 0, 0, "isStream attribute");
  op->moveAfter(op1);
 
  // dummy must have 5 bits rather than 4
  UmlAttribute * dummy = base_pinparam->get_attribute("_dummy");
  QCString cppdecl = dummy->cppDecl();
  int index = cppdecl.find(": 4");
  
  if (index != -1) {
    cppdecl[index + 2] = '5';
    dummy->set_CppDecl(cppdecl);
  }
  
  //
  
  UmlCom::trace("fixe UmlBaseAddVariableValueAction::read_uml_()<br>\n");
  
  op = UmlClass::get("UmlBaseAddVariableValueAction", 0)->get_operation("read_uml_");
  
  op->set_CppBody("  UmlBaseAccessVariableValueAction::read_uml_();\n"
		  "  _replace_all = UmlCom::read_bool();\n");
  
  op->set_JavaBody("  super.read_uml_();\n"
		   "  _replace_all = UmlCom.read_bool();\n");
  
  //
  
  UmlCom::trace("fixe UmlBaseRemoveVariableValueAction::read_uml_()<br>\n");
  
  op = UmlClass::get("UmlBaseRemoveVariableValueAction", 0)->get_operation("read_uml_");
  
  op->set_CppBody("  UmlBaseAccessVariableValueAction::read_uml_();\n"
		  "  _remove_duplicates = UmlCom::read_bool();\n");
  
  op->set_JavaBody("  super.read_uml_();\n"
		   "  _remove_duplicates = UmlCom.read_bool();\n");
  
  //
  
  UmlCom::set_user_id(uid);
  
  //
  
  UmlRelation * rel = 
    UmlBaseRelation::create(aGeneralisation, 
			    UmlClass::get("UmlParameterSet", 0),
			    UmlClass::get("UmlActivityItem", 0));
  
  if (rel == 0) {
    QCString msg = "UmlParameterSet can't inherit UmlActivityItem<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  else {
    rel->set_CppDecl("${type}");
    rel->set_JavaDecl("${type}");
  }
}

//
//
//

void add_cpp_set_param_ref(UmlClass * cppsetting)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("update CppSettings<br>\n");

  UmlAttribute * att = 
    cppsetting->add_attribute("_is_set_param_ref", PrivateVisibility, "bool", 0, 0);

  att->set_isClassMember(TRUE);
  att->moveAfter(cppsetting->get_attribute("_is_set_param_const"));

  //

  UmlOperation * op = cppsetting->get_operation("read_");

  op->set_CppBody(op->cppBody() + "  _is_set_param_ref = UmlCom::read_bool();\n");
  op->set_JavaBody(op->javaBody() + "  _is_set_param_ref = UmlCom.read_bool();\n");
  
  //

  UmlOperation * op2 = cppsetting->get_operation("set_IsSetParamConst");

  op = cppsetting->add_op("set_IsSetParamRef", PublicVisibility, "bool", TRUE);
  op->set_isClassMember(TRUE);
  
  op->add_param(0, InputDirection, "v", "bool");
  op->set_Description(" set if the parameter of a 'set' operation generated through the\n"
		      " attribute and relation 'add set operation' menu is a reference by default\n"
		      "\n"
		      " On error : return FALSE in C++, produce a RuntimeException in Java");
  
  op->set_CppDecl(op2->cppDecl());
  op->set_CppDef(op2->cppDef());
  op->set_CppBody("\
  UmlCom::send_cmd(cppSettingsCmd, setCppIsSetParamRefCmd, v);\n\
  if (UmlCom::read_bool()) {\n\
    _is_set_param_ref = v;\n\
    return TRUE;\n\
  }\n\
  else\n\
    return FALSE;\n");

  op->set_JavaDecl(op2->javaDecl());
  op->set_JavaBody("\
  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._setCppIsSetParamRefCmd,\n\
		  (v) ? (byte) 1 : (byte) 0);\n\
  UmlCom.check();\n\
  \n\
  _is_set_param_ref = v;\n");
  
  op->moveAfter(op2);

  //

  UmlClass::get("CppSettingsCmd", 0)->add_enum_item("setCppIsSetParamRefCmd");

  //

  UmlCom::set_user_id(uid);
}

void add_getter_setter_on_instance_cmd()
{
  // already root
  UmlClass * itcmd = UmlClass::get("OnInstanceCmd", 0);
  
  itcmd->add_enum_item("setCppFrozenCmd");
  itcmd->add_enum_item("setJavaFrozenCmd");
  itcmd->add_enum_item("setIdlFrozenCmd");
}    

void upgrade_setter_getter()
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("update UmlBaseOperation setter and getter<br>\n");

  //

  add_getter_setter_on_instance_cmd();

  //

  UmlClass * baseoper = UmlClass::get("UmlBaseOperation", 0);
  UmlAttribute * att;
  UmlAttribute * att2;
  QCString s;
  int index;
  
  att = baseoper->add_attribute("_cpp_get_set_frozen", PrivateVisibility, "bool", "WITHCPP", "endif");  
  s = att->cppDecl();
  index = s.find("${type} ${name}");
  if (index != -1) {
    s.insert(index + 15, " : 1");
    att->set_CppDecl(s);
  }
  att->moveAfter(baseoper->get_attribute("_idl_oneway"));

  att2 = baseoper->add_attribute("_java_get_set_frozen", PrivateVisibility, "bool", "WITHJAVA", "endif");  
  att2->set_CppDecl(s);
  att2->moveAfter(att);

  att = baseoper->add_attribute("_idl_get_set_frozen", PrivateVisibility, "bool", "WITHIDL", "endif");  
  att->set_CppDecl(s);
  att->moveAfter(att2);

  //

  UmlOperation * op;
  UmlOperation * pos;
  
  pos = baseoper->get_operation("set_CppNameSpec");
  defGetBool(baseoper, _cpp_get_set_frozen, cppGetSetFrozen, "WITHCPP", 0,
	     "if the C++ definition is frozen, only for getter/setter operation");
  op->moveAfter(pos);
  pos = op;
  defSetBoolBitField(baseoper, _cpp_get_set_frozen, set_CppGetSetFrozen, setCppFrozenCmd, 0, "endif",
		     "if the C++ definition is frozen, only for getter/setter operation");
  op->moveAfter(pos);

  pos = baseoper->get_operation("set_JavaNameSpec");
  defGetBool(baseoper, _java_get_set_frozen, javaGetSetFrozen, "WITHJAVA", 0,
	     "if the Java definition is frozen, only for getter/setter operation");
  op->moveAfter(pos);
  pos = op;
  defSetBoolBitField(baseoper, _java_get_set_frozen, set_JavaGetSetFrozen, setJavaFrozenCmd, 0, "endif",
		     "if the Java definition is frozen, only for getter/setter operation");
  op->moveAfter(pos);

  pos = baseoper->get_operation("set_IdlNameSpec");
  defGetBool(baseoper, _idl_get_set_frozen, idlGetSetFrozen, "WITHIDL", 0,
	     "if the IDL definition is frozen, only for getter/setter operation");
  op->moveAfter(pos);
  pos = op;
  defSetBoolBitField(baseoper, _idl_get_set_frozen, set_IdlGetSetFrozen, setIdlFrozenCmd, 0, "endif",
		     "if the IDL definition is frozen, only for getter/setter operation");
  op->moveAfter(pos);

  //

  op = baseoper->get_operation("read_cpp_");
  op->set_CppBody(op->cppBody() + "  _cpp_get_set_frozen = UmlCom::read_bool();\n");
  op->set_JavaBody(op->javaBody() + "  _cpp_get_set_frozen = UmlCom.read_bool();\n");

  op = baseoper->get_operation("read_java_");
  op->set_CppBody(op->cppBody() + "  _java_get_set_frozen = UmlCom::read_bool();\n");
  op->set_JavaBody(op->javaBody() + "  _java_get_set_frozen = UmlCom.read_bool();\n");

  op = baseoper->get_operation("read_idl_");
  op->set_CppBody(op->cppBody() + "  _idl_get_set_frozen = UmlCom::read_bool();\n");
  op->set_JavaBody(op->javaBody() + "  _idl_get_set_frozen = UmlCom.read_bool();\n");

  //

  UmlCom::set_user_id(uid);
}

//
//
//

void add_cpp_relative_path_force_namespace(UmlClass * cppsetting)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("update CppSettings<br>\n");

  //

  UmlOperation * op = cppsetting->get_operation("read_");

  op->set_CppBody(op->cppBody() + 
		  "  _is_relative_path = UmlCom::read_bool();\n"
		  "  _is_force_namespace_gen = UmlCom::read_bool();\n");
  op->set_JavaBody(op->javaBody() + 
		   "  _is_relative_path = UmlCom.read_bool();\n"
		   "  _is_force_namespace_gen = UmlCom.read_bool();\n");
  
  //
  // add missing operation isSetParamRef()
  // 
  
  op = cppsetting->add_op("isSetParamRef", PublicVisibility, "bool");
   
  op->set_isClassMember(TRUE);
  op->set_Description(" return if the parameter of a 'set' operation generated through the\n"
		      " attribute and relation 'add set operation' menu is a reference by default");
  op->set_cpp("${type}", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _is_set_param_ref;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _is_set_param_ref;\n",
	       FALSE);
  op->moveAfter(cppsetting->get_operation("set_IsSetParamConst"));
  
  //
  // relative path
  //
  
  UmlAttribute * att1 = 
    cppsetting->add_attribute("_is_relative_path", PrivateVisibility, "bool", 0, 0);

  att1->set_isClassMember(TRUE);
  att1->moveAfter(cppsetting->get_attribute("_incl_with_path"));

  // get
  
  op = cppsetting->add_op("isRelativePath", PublicVisibility, "bool");
   
  op->set_isClassMember(TRUE);
  op->set_Description(" return if a relative path must be used when the path\n"
		      " must be generated in the produced #includes");
  op->set_cpp("${type}", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _is_relative_path;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _is_relative_path;\n",
	       FALSE);
  op->moveAfter(cppsetting->get_operation("set_IncludeWithPath"));

  // set
  
  UmlOperation * op2 = cppsetting->add_op("set_IsRelativePath", PublicVisibility, "bool");
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" set if a relative path must be used when the path\n"
		       " must be generated in the produced #includes\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", "bool"); 
  op2->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(cppSettingsCmd, setCppRelativePathCmd, v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _is_relative_path = v;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._setCppRelativePathCmd,\n"
		"		   (v) ? (byte) 1 : (byte) 0);\n"
		"  UmlCom.check();\n"
		"  _is_relative_path = v;\n",
		FALSE);
  op2->moveAfter(op);

  //
  // force namespace prefix generation
  //
  
  UmlAttribute * att2 = 
    cppsetting->add_attribute("_is_force_namespace_gen", PrivateVisibility, "bool", 0, 0);

  att2->set_isClassMember(TRUE);
  att2->moveAfter(att1);

  // get
  
  op = cppsetting->add_op("isForceNamespacePrefixGeneration", PublicVisibility, "bool");
   
  op->set_isClassMember(TRUE);
  op->set_Description(" return if the namespace prefix must be\n"
		      " always generated before class's names");
  op->set_cpp("${type}", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _is_force_namespace_gen;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _is_force_namespace_gen;\n",
	       FALSE);
  op->moveAfter(op2);

  // set
  
  op2 = cppsetting->add_op("set_IsForceNamespacePrefixGeneration", PublicVisibility, "bool");
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" set if the namespace prefix must be always generated before class's names\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", "bool"); 
  op2->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(cppSettingsCmd, setCppForceNamespaceGenCmd, v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _is_force_namespace_gen = v;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._setCppForceNamespaceGenCmd,\n"
		"		   (v) ? (byte) 1 : (byte) 0);\n"
		"  UmlCom.check();\n"
		"  _is_force_namespace_gen = v;\n",
		FALSE);
  op2->moveAfter(op);
  
  //

  UmlClass * cppsettingcmd = UmlClass::get("CppSettingsCmd", 0);
  
  cppsettingcmd->add_enum_item("setCppRelativePathCmd");
  cppsettingcmd->add_enum_item("setCppForceNamespaceGenCmd");

  //

  UmlCom::set_user_id(uid);
}

//
//
//

void add_getter_setter_rules(UmlClass * umlsetting)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("update UmlSettings<br>\n");
  
  //
  
  UmlClassView * base_class_view = (UmlClassView *) umlsetting->parent();
  UmlDeploymentView * base_depl_view = (UmlDeploymentView *)
    umlsetting->associatedArtifact()->parent();
  UmlClass * lang = 
    UmlClass::made(base_class_view, base_depl_view, "aLanguage");

  lang->set_Stereotype("enum_pattern");
  lang->set_CppDecl(CppSettings::enumDecl());
  lang->set_JavaDecl(JavaSettings::enumPatternDecl());
  lang->add_enum_item("umlLanguage");
  lang->add_enum_item("cppLanguage");
  lang->add_enum_item("javaLanguage");
  lang->add_enum_item("idlLanguage");
  
  //
  
  UmlRelation * rel1 =
    umlsetting->add_relation(aDirectionalAggregationByValue, "_uml_get_name",
			     PrivateVisibility, lang, 0, 0);
  rel1->set_isClassMember(TRUE);
  rel1->moveAfter(umlsetting->get_attribute("_defined"));
  
  UmlRelation * rel2 =
    umlsetting->add_relation(aDirectionalAggregationByValue, "_uml_set_name",
			     PrivateVisibility,	lang, 0, 0); 
  rel2->set_isClassMember(TRUE);
  rel2->moveAfter(rel1);
  
  //

  UmlOperation * op = umlsetting->get_operation("read_");

  op->set_CppBody(op->cppBody() + 
		  "  _uml_get_name = (aLanguage) UmlCom::read_char();\n"
		  "  _uml_set_name = (aLanguage) UmlCom::read_char();\n");
  op->set_JavaBody(op->javaBody() + 
		   "  _uml_get_name = aLanguage.fromInt(UmlCom.read_char());\n"
		   "  _uml_set_name = aLanguage.fromInt(UmlCom.read_char());\n");
  
  // get getter name
  
  op = umlsetting->add_op("umlGetName", PublicVisibility, lang);
   
  op->set_isClassMember(TRUE);
  op->set_Description(" return the language from which the getter's name rule must be followed at Uml level");
  op->set_cpp("${type}", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _uml_get_name;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _uml_get_name;\n",
	       FALSE);
  op->moveAfter(umlsetting->get_operation("set_RelationDescription"));

  // set getter name
  
  UmlOperation * op2;
  
  op2 = umlsetting->add_op("set_UmlGetName", PublicVisibility, "bool");
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" set the language from which the getter's name rule must be followed at Uml level\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", lang); 
  op2->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(umlSettingsCmd, setUmlDefaultGetNameCmd, (char) v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _uml_get_name = v;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.umlSettingsCmd, UmlSettingsCmd._setUmlDefaultGetNameCmd,\n"
		"                  (byte) v.value());\n"
		"  UmlCom.check();\n"
		"  _uml_get_name = v;\n",
		FALSE);
  op2->moveAfter(op);
  
  // get setter name
  
  op = umlsetting->add_op("umlSetName", PublicVisibility, lang);
   
  op->set_isClassMember(TRUE);
  op->set_Description(" return the language from which the setter's name rule must be followed at Uml level");
  op->set_cpp("${type}", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _uml_set_name;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _uml_set_name;\n",
	       FALSE);
  op->moveAfter(op2);

  // set setter name
  
  op2 = umlsetting->add_op("set_UmlSetName", PublicVisibility, "bool");
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" set the language from which the setter's name rule must be followed at Uml level\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", lang); 
  op2->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(umlSettingsCmd, setUmlDefaultSetNameCmd, (char) v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _uml_set_name = v;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.umlSettingsCmd, UmlSettingsCmd._setUmlDefaultSetNameCmd,\n"
		"                  (byte) v.value());\n"
		"  UmlCom.check();\n"
		"  _uml_set_name = v;\n",
		FALSE);
  op2->moveAfter(op);
  
  // update UmlSettingsCmd enum
  
  UmlClass * cl = UmlClass::get("UmlSettingsCmd", 0);
  
  cl->add_enum_item("setUmlDefaultGetNameCmd");
  cl->add_enum_item("setUmlDefaultSetNameCmd");

  //

  UmlCom::set_user_id(uid);
}


//
//
//

void add_extension_points()
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("update use case<br>\n");
  
  //
  
  UmlClass * base_usecase = UmlClass::get("UmlBaseUseCase", 0);
  UmlOperation * op;
  
  op = base_usecase->get_operation("read_uml_");
  op->set_CppBody(op->cppBody() + 
		  "  _extension_points = UmlCom::read_string();\n");
  op->set_JavaBody(op->javaBody() + 
		   "  _extension_points = UmlCom.read_string();\n");

  //
  
  defGet(base_usecase, _extension_points, extensionPoints, "string", 0, 0,
         "extension points");
  op->moveAfter(base_usecase->get_operation("set_AssociatedDiagram"));
  
  UmlOperation * op2 = op;
  
  defSet(base_usecase, _extension_points, set_ExtensionPoints,
	 "str", replaceExceptionCmd, 0, 0,
	 "extension points");
  op->moveAfter(op2);
  
  //

  base_usecase->add_attribute("_extension_points", PrivateVisibility, "string", 0, 0)
    ->moveAfter(op);

  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

void remove_java_public(UmlClass * uml_base_class)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("new class visibility<br>\n");
  
  //

  uml_base_class->get_attribute("_java_public")->delete_it();
  
  UmlOperation * op;
  
  op = uml_base_class->get_operation("isJavaPublic");
  op->set_CppBody("  return visibility() == PublicVisibility;\n");
  op->set_JavaBody("  return visibility() == aVisibility.PublicVisibility;\n");
  
  op = uml_base_class->get_operation("set_isJavaPublic");
  op->set_CppBody("  return set_Visibility((y) ? PublicVisibility : PackageVisibility);\n");
  op->set_JavaBody("  set_Visibility((y) ? aVisibility.PublicVisibility : aVisibility.PackageVisibility);\n");
  
  QCString s;
  
  op = uml_base_class->get_operation("read_java_");
  
  s = op->cppBody();
  s.remove(s.find("_java_public = UmlCom::read_bool();"), 34);
  op->set_CppBody(s);
  
  s = op->javaBody();
  s.remove(s.find("_java_public = UmlCom.read_bool();"), 34);
  op->set_JavaBody(s);
  
  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

void add_cpp_root_relative_path(UmlClass * cppsetting)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("update UmlSettings<br>\n");
  
  //
  // add root relative
  //
  
  UmlAttribute * att1 = 
    cppsetting->add_attribute("_is_root_relative_path", PrivateVisibility, "bool", 0, 0);

  att1->set_isClassMember(TRUE);
  att1->moveAfter(cppsetting->get_attribute("_is_relative_path"));

  // get
  
  UmlOperation * op = cppsetting->add_op("isRootRelativePath", PublicVisibility, "bool");
   
  op->set_isClassMember(TRUE);
  op->set_Description(" return if a path relative to the project root must be used\n"
		      " when the path must be generated in the produced #includes");
  op->set_cpp("${type}", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _is_root_relative_path;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _is_root_relative_path;\n",
	       FALSE);
  op->moveAfter(cppsetting->get_operation("set_IsRelativePath"));

  // set
  
  UmlOperation * op2 = cppsetting->add_op("set_IsRootRelativePath", PublicVisibility, "bool");
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" set if a relative to the project root path must be used\n"
		       " when the path must be generated in the produced #includes\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", "bool"); 
  op2->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(cppSettingsCmd, setCppRootRelativePathCmd, v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _is_root_relative_path = v;\n"
		"    if (v) _is_relative_path = FALSE;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._setCppRootRelativePathCmd,\n"
		"		   (v) ? (byte) 1 : (byte) 0);\n"
		"  UmlCom.check();\n"
		"  _is_root_relative_path = v;\n"
		"  if (v) _is_relative_path = false;\n",
		FALSE);
  op2->moveAfter(op);  
  
  // upgrade set_IsRelativePath
  op2 = cppsetting->get_operation("set_IsRelativePath");
  
  op2->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(cppSettingsCmd, setCppRelativePathCmd, v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _is_relative_path = v;\n"
		"    if (v) _is_root_relative_path = FALSE;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._setCppRelativePathCmd,\n"
		"		   (v) ? (byte) 1 : (byte) 0);\n"
		"  UmlCom.check();\n"
		"  _is_relative_path = v;\n"
		"  if (v) _is_root_relative_path = false;\n",
		FALSE);
  
  //
  
  UmlClass * cppsettingcmd = UmlClass::get("CppSettingsCmd", 0);
  
  cppsettingcmd->add_enum_item("setCppRootRelativePathCmd");

  //
  
  op = cppsetting->get_operation("read_");

  op->set_CppBody(op->cppBody() + 
		  "  _is_root_relative_path = UmlCom::read_bool();\n");
  op->set_JavaBody(op->javaBody() + 
		   "  _is_root_relative_path = UmlCom.read_bool();\n");
  
  //
  
  UmlCom::set_user_id(uid);
}

///
// add generate javadoc comment settings
//

void add_cpp_generate_javadoc_comment(UmlClass * cppsetting)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("update CppSettings<br>\n");
  
  //
  // add _is_generate_javadoc_comment
  //
  
  UmlAttribute * att1 = 
    cppsetting->add_attribute("_is_generate_javadoc_comment", PrivateVisibility, "bool", 0, 0);

  att1->set_isClassMember(TRUE);
  att1->moveAfter(cppsetting->get_attribute("_is_force_namespace_gen"));

  // get
  
  UmlOperation * op = cppsetting->add_op("isGenerateJavadocStyleComment", PublicVisibility, "bool");
   
  op->set_isClassMember(TRUE);
  op->set_Description(" return if ${comment} generate Javadoc style comment");
  op->set_cpp("${type}", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _is_generate_javadoc_comment;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _is_generate_javadoc_comment;\n",
	       FALSE);
  op->moveAfter(cppsetting->get_operation("set_IsForceNamespacePrefixGeneration"));

  // set
  
  UmlOperation * op2 = cppsetting->add_op("set_IsGenerateJavadocStyleComment", PublicVisibility, "bool");
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" set if ${comment} generate Javadoc style comment\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", "bool"); 
  op2->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(cppSettingsCmd, setCppJavadocStyleCmd, v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _is_generate_javadoc_comment = v;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._setCppJavadocStyleCmd,\n"
		"		   (v) ? (byte) 1 : (byte) 0);\n"
		"  UmlCom.check();\n"
		"  _is_generate_javadoc_comment = v;\n",
		FALSE);
  op2->moveAfter(op);  
  
  //
  
  UmlClass * cppsettingcmd = UmlClass::get("CppSettingsCmd", 0);
  
  cppsettingcmd->add_enum_item("setCppJavadocStyleCmd");

  //
  
  op = cppsetting->get_operation("read_");

  op->set_CppBody(op->cppBody() + 
		  "  _is_generate_javadoc_comment = UmlCom::read_bool();\n");
  op->set_JavaBody(op->javaBody() + 
		   "  _is_generate_javadoc_comment = UmlCom.read_bool();\n");
  
  //
  
  UmlCom::set_user_id(uid);
}

void add_java_generate_javadoc_comment(UmlClass * javasetting)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("update JavaSettings<br>\n");
  
  //
  // add _is_generate_javadoc_comment
  //
  
  UmlAttribute * att1 = 
    javasetting->add_attribute("_is_generate_javadoc_comment", PrivateVisibility, "bool", 0, 0);

  att1->set_isClassMember(TRUE);
  att1->moveAfter(javasetting->get_attribute("_ext"));

  // get
  
  UmlOperation * op = javasetting->add_op("isGenerateJavadocStyleComment", PublicVisibility, "bool");
   
  op->set_isClassMember(TRUE);
  op->set_Description(" return if ${comment} generate Javadoc style comment");
  op->set_cpp("${type}", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _is_generate_javadoc_comment;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _is_generate_javadoc_comment;\n",
	       FALSE);
  op->moveAfter(javasetting->get_operation("set_SourceExtension"));

  // set
  
  UmlOperation * op2 = javasetting->add_op("set_IsGenerateJavadocStyleComment", PublicVisibility, "bool");
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" set if ${comment} generate Javadoc style comment\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", "bool"); 
  op2->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(javaSettingsCmd, setJavaJavadocStyleCmd, v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _is_generate_javadoc_comment = v;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.javaSettingsCmd, JavaSettingsCmd._setJavaJavadocStyleCmd,\n"
		"		   (v) ? (byte) 1 : (byte) 0);\n"
		"  UmlCom.check();\n"
		"  _is_generate_javadoc_comment = v;\n",
		FALSE);
  op2->moveAfter(op);  
  
  //
  
  UmlClass * javasettingcmd = UmlClass::get("JavaSettingsCmd", 0);
  
  javasettingcmd->add_enum_item("setJavaJavadocStyleCmd");

  //
  
  op = javasetting->get_operation("read_");

  op->set_CppBody(op->cppBody() + 
		  "  _is_generate_javadoc_comment = UmlCom::read_bool();\n");
  op->set_JavaBody(op->javaBody() + 
		   "  _is_generate_javadoc_comment = UmlCom.read_bool();\n");
  
  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

void add_constraint(UmlClass * baseclassmember)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("Upgrade class members<br>\n");
  
  //
  
  baseclassmember->add_attribute("_constraint", PrivateVisibility, "string",
				 0, 0)
    ->moveAfter(baseclassmember->get_attribute("_java_annotation"));
  
  UmlOperation * op;
  UmlOperation * op1;
  
  defGet(baseclassmember, _constraint, constraint, "string", 0, 0,
         "constraint");
  op->moveAfter(baseclassmember->get_operation("set_JavaAnnotations"));
  op1 = op;
  defSet(baseclassmember, _constraint, set_Constraint, "str", setConstraintCmd, 0, 0,
         "constraint");
  op->moveAfter(op1);
  
  op = baseclassmember->get_operation("read_uml_");
  op->set_CppBody(op->cppBody() + "  _constraint = UmlCom::read_string();\n");
  op->set_JavaBody(op->javaBody() + "  _constraint = UmlCom.read_string();\n");
  
  //
  
  UmlClass::get("OnInstanceCmd", 0)->add_enum_item("setConstraintCmd");
  
  //
  
  UmlCom::set_user_id(uid);
}

//
// add UmlBaseItem::deleteIt
//

void add_deleteit(UmlClass * uml_base_item)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("Upgrade UmlBaseItem<br>\n");
 
  //
  
  UmlOperation * op = uml_base_item->add_op("deleteIt", PublicVisibility, "bool", TRUE);
  
  op->set_Description(" remove the element from the model, use it carefully because\n"
		      " after that the element can't be used anymore by the plug-out\n"
		      "\n"
		      " On error : return FALSE in C++, produce a RuntimeException in Java");
  op->set_CppBody("  UmlCom::send_cmd(_identifier, deleteCmd);\n"
		  "  if (UmlCom::read_bool() == 0) return FALSE;\n"
		  "  parent()->unload(TRUE);\n"
		  "  return TRUE;\n");
  op->set_java("void", "",
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.deleteCmd);\n"
	       "  UmlCom.check();\n"
	       "  parent().unload(true, false);\n"
	       , FALSE);
  op->moveAfter(uml_base_item->get_operation("unload"));

  //
  
  UmlClass::get("OnInstanceCmd", 0)->add_enum_item("deleteCmd");
  
  //
  
  UmlCom::set_user_id(uid);
}

//
// add get_id()
//
void add_get_id(UmlClass * uml_base_item)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("Upgrade UmlBaseItem<br>\n");
 
  //
  
  UmlOperation * op = uml_base_item->add_op("getIdentifier", PublicVisibility, "int");
  
  op->set_Description(" return a constant identifier, it is unique within a given\n"
		      " kind of element (two classes can't have the same identifiers, but\n"
		      " a class and a component may have the same identifier) \n"
		      " except for the diagrams (a class diagram can't have the identifier\n"
		      " of a component diagram)");
  op->set_CppBody("  read_if_needed_();\n"
		  "\n"
		  "  return _modeler_id;\n");
  op->set_JavaBody("  read_if_needed_();\n"
		   "\n"
		   "  return _modeler_id;\n");
  op->moveAfter(uml_base_item->get_operation("referencedBy"));

  //
  
  UmlAttribute * att =
    uml_base_item->add_attribute("_modeler_id", ProtectedVisibility, "int", 0, 0);

  att->moveAfter(uml_base_item->get_attribute("_identifier"));
  
  //
  
  op = uml_base_item->get_operation("read_uml_");
  
  if (op != 0) {
    QCString body;
    
    body = op->cppBody();
    body += "  _modeler_id = (int) UmlCom::read_unsigned();\n";
    op->set_CppBody(body);
      
    body = op->javaBody();
    body += "  _modeler_id = UmlCom.read_unsigned();\n";
    op->set_JavaBody(body);
  }
  
  //
  
  UmlCom::set_user_id(uid);
}

// 
// add class instance
//

void add_class_instance(UmlClassView * base_class_view, UmlClassView * user_class_view,
			UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
			UmlClass * base_item, UmlClass * user_item)
{
  UmlClass * base_class_instance;
  UmlClass * user_class_instance;
  
  user_item->made(base_class_view, user_class_view, base_depl_view, user_depl_view,
		  base_item, "ClassInstance", base_class_instance, user_class_instance,
		  0);
  
  user_class_instance->add_constr(base_class_instance, PublicVisibility);  
    
  //
  // SlotAttribute
  //
  
  UmlClass * user_class = UmlClass::get("UmlClass", 0);
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlClass * slotattr = UmlClass::create(base_class_view, "SlotAttribute");
  UmlClass * attr = UmlClass::get("UmlAttribute", 0);
  UmlArtifact * art;
  UmlOperation * op;
  
  slotattr->set_Stereotype("struct");
  slotattr->set_CppDecl(CppSettings::structDecl());
  slotattr->add_relation(aDirectionalAssociation, "attribute",
			 PublicVisibility, attr, 0, 0);
  slotattr->add_attribute("value", PublicVisibility, "string", 0, 0);
  op = slotattr->add_op("SlotAttribute", PublicVisibility, "", FALSE);
  op->set_Description(" because of QValueList use");
  op->set_cpp(": attribute(0)", "", "", TRUE, 0, 0);
  op->set_JavaDef("");
  op = slotattr->add_op("SlotAttribute", PublicVisibility, "", FALSE);
  op->add_param(0, InputOutputDirection, "a", attr);
  op->add_param(1, InputOutputDirection, "v", "string");
  op->set_cpp(": attribute(a), value(v)", "${t0} * ${p0}, ${t1} ${p1}",
	      "", TRUE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  attribute = a;\n"
	       "  value = v;\n",
	       FALSE);
  
  art = UmlArtifact::made(base_depl_view, "SlotAttribute");
  art->set_CppSource("");
  art->addAssociatedClass(slotattr);
  
  //
  // SlotRelation
  //
  
  UmlClass * slotrel = UmlClass::create(base_class_view, "SlotRelation");
  UmlClass * rel = UmlClass::get("UmlRelation", 0);
  
  slotrel->set_Stereotype("struct");
  slotrel->set_CppDecl(CppSettings::structDecl());
  slotrel->add_relation(aDirectionalAssociation, "relation",
			PublicVisibility, rel, 0, 0); 
  slotrel->add_relation(aDirectionalAssociation,
			"value", PublicVisibility, user_class_instance, 0, 0); 
  op = slotrel->add_op("SlotRelation", PublicVisibility, "", FALSE);
  op->set_Description(" because of QValueList use");
  op->set_cpp(": relation(0), value(0)",  "", "", TRUE, 0, 0);
  op->set_JavaDef("");
  op = slotrel->add_op("SlotRelation", PublicVisibility, "", FALSE);
  op->add_param(0, InputOutputDirection, "r", rel);
  op->add_param(1, InputOutputDirection, "v", user_class_instance);
  op->set_cpp(": relation(r), value(v)",  "${t0} * ${p0}, ${t1} * ${p1}",
	      "", TRUE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  relation = r;\n"
	       "  value = v;\n",
	       FALSE);
  
  art = UmlArtifact::made(base_depl_view, "SlotRelation");
  art->set_CppSource("");
  art->addAssociatedClass(slotrel);
  
  //
  // ClassInstance members
  //
  
  include_umlcom(base_class_instance);
  
  base_class_instance->add_kind("aClassInstance");
  
  // create()
  
  op = base_class_instance->add_op("create", PublicVisibility, user_class_instance, TRUE);
  op->set_isClassMember(TRUE);
  op->add_param(0, InputOutputDirection, "parent", user_item);
  op->add_param(1, InputOutputDirection, "name", "str");
  op->add_param(2, InputOutputDirection, "type", user_class);
  op->set_cpp("${type} *", 
	      "${t0} * ${p0}, const ${t1} ${p1}, ${t2} * ${p2}",
	      "  UmlCom::send_cmd(parent->_identifier, createCmd, aClassInstance,\n"
	      "\t\t   type->_identifier);\n"
	      "  UmlClassInstance * result = (UmlClassInstance *) UmlBaseItem::read_();\n"
	      "\n"
	      "  if (result != 0) {\n"
	      "    parent->reread_children_if_needed_();\n"
	      "    if (name != 0) result->set_Name(name);\n"
	      "  }\n"
	      "  return result;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}",
	       "  UmlCom.send_cmd(parent.identifier_(), OnInstanceCmd.createCmd, anItemKind.aClassInstance,\n"
	       "\t\t   type.identifier_());\n"
	       "  UmlClassInstance result = (UmlClassInstance) UmlBaseItem.read_();\n"
	       "  \n"
	       "  if (result != null) {\n"
	       "    parent.reread_children_if_needed_();\n"
	       "    if (name != null) result.set_Name(name);\n"
	      "  }\n"
	       "  else\n"
	       "    throw new RuntimeException(\"Cannot create the class instance\");\n"
	       "  return result;\n",
	      FALSE);
  op->set_Description(" Returns a new class instance\n\
\n\
 In case it cannot be created ('parent' cannot contain it etc ...) return 0\n\
  in C++ and produce a RuntimeException in Java\n");
  
  // type(), set_Type()
    
  defGetPtr(base_class_instance, _type, type, user_class, 0, 0, "type");
  defSetPtr(base_class_instance, _type, set_Type, user_class, setTypeCmd, 0, 0, "type");
    
  // attributesValue()
  
  op = base_class_instance->add_op("attributesValue", PublicVisibility, slotattr);
  op->add_param(0, OutputDirection, "result", slotattr);
  op->set_cpp("void", "QValueList<${t0}> & ${p0}",
	      "  UmlCom::send_cmd(_identifier, attributesCmd, (char) 0);\n"
	      "\n"
	      "  unsigned n = UmlCom::read_unsigned();\n"
	      "\n"
	      "  result.clear();\n"
	      "  while (n--) {\n"
	      "    UmlAttribute * at = (UmlAttribute *) UmlBaseItem::read_();\n"
	      "\n"
	      "    result.append(SlotAttribute(at, UmlCom::read_string()));\n"
	      "  }\n",
	      FALSE, 0, 0);
  op->set_java("${type}[]", "",
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.attributesCmd, (byte) 0);\n"
	      "\n"
	       "  int n = UmlCom.read_unsigned();\n"
	       "  SlotAttribute[] v = new SlotAttribute[n];\n"
	       "\n"
	       "  for (int index = 0; index != n; index += 1) {\n"
	       "    UmlAttribute at = (UmlAttribute) UmlBaseItem.read_();\n"
	       "\n"
	       "    v[index] = new SlotAttribute(at, UmlCom.read_string());\n"
	       "  }\n"
	       "  return v;\n",
	      FALSE);
  op->set_Description(" Returns the attributes having a value");
  
  // relationsValue()
  
  op = base_class_instance->add_op("relationsValue", PublicVisibility, slotrel);
  op->add_param(0, OutputDirection, "result", slotrel);
  op->set_cpp("void", "QValueList<${t0}> & ${p0}",
	      "  UmlCom::send_cmd(_identifier, relationsCmd, (void *) 0);\n"
	      "\n"
	      "  unsigned n = UmlCom::read_unsigned();\n"
	      "\n"
	      "  result.clear();\n"
	      "  while (n--) {\n"
	      "    UmlRelation * r = (UmlRelation *) UmlBaseItem::read_();\n"
	      "\n"
	      "    result.append(SlotRelation(r, (UmlClassInstance *) UmlBaseItem::read_()));\n"
	      "  }\n",
	      FALSE, 0, 0);
  op->set_java("${type}[]", "",
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.relationsCmd, (long) 0);\n"
	      "\n"
	       "  int n = UmlCom.read_unsigned();\n"
	       "  SlotRelation[] v = new SlotRelation[n];\n"
	       "\n"
	       "  for (int index = 0; index != n; index += 1) {\n"
	       "    UmlRelation rel = (UmlRelation) UmlBaseItem.read_();\n"
	       "\n"
	       "    v[index] = new SlotRelation(rel, (UmlClassInstance) UmlBaseItem.read_());\n"
	       "  }\n"
	       "  return v;\n",
	      FALSE);
  op->set_Description(" Returns the attributes having a value");
  
  // availableAttributes()
  
  op = base_class_instance->add_op("availableAttributes", PublicVisibility, attr);
  op->add_param(0, OutputDirection, "result", attr);
  op->set_cpp("void", "QVector<${t0}> & ${p0}",
	      "  UmlCom::send_cmd(_identifier, attributesCmd, (char) 1);\n"
	      "  UmlCom::read_item_list((QVector<UmlItem> &) result);\n",
	      FALSE, 0, 0);
  op->set_java("${type}[]", "",
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.attributesCmd, (byte) 1);\n"
	      "\n"
	       "  int n = UmlCom.read_unsigned();\n"
	       "  UmlAttribute[] v = new UmlAttribute[n];\n"
	       "\n"
	       "  for (int index = 0; index != n; index += 1)\n"
	       "    v[index] = (UmlAttribute) UmlBaseItem.read_();\n"
	       "  return v;\n",
	      FALSE);
  op->set_Description(" Returns all the attributes of the class instance,\n"
		      " including the inherited");
  
  // availableRelations()
  
  op = base_class_instance->add_op("availableRelations", PublicVisibility, rel);
  op->add_param(0, OutputDirection, "other", user_class_instance);
  op->add_param(1, OutputDirection, "result", rel);
  op->set_cpp("void", 
	      "${t0} * ${p0}, QVector<${t1}> & ${p1}",
	      "  UmlCom::send_cmd(_identifier, relationsCmd, other->_identifier);\n"
	      "  UmlCom::read_item_list((QVector<UmlItem> &) result);\n",
	      FALSE, 0, 0);
  op->set_java("${type}[]", "${t0} ${p0}",
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.relationsCmd, other.identifier_());\n"
	      "\n"
	       "  int n = UmlCom.read_unsigned();\n"
	       "  UmlRelation[] v = new UmlRelation[n];\n"
	       "\n"
	       "  for (int index = 0; index != n; index += 1)\n"
	       "    v[index] = (UmlRelation) UmlBaseItem.read_();\n"
	       "  return v;\n",
	      FALSE);
  op->set_Description(" Returns all the possible relations from the current instance"
		      " to 'other', including the inherited");
  
  // set_AttributeValue()
  
  op = base_class_instance->add_op("set_AttributeValue", PublicVisibility, "bool", TRUE);
  op->add_param(0, InputDirection, "attribute", attr);
  op->add_param(1, InputDirection, "value", "str");
  op->set_cpp("bool", "${t0} * ${p0}, const ${t1} ${p1}",
	      "  UmlCom::send_cmd(_identifier, setAttributeCmd, attribute->_identifier, value);\n"
	      "  return UmlCom::read_bool();\n",
	      FALSE, 0, 0);
  op->set_java("void", "${t0} ${p0}, ${t1} ${p1}",
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.setAttributeCmd, attribute.identifier_(), value);\n"
	       "  UmlCom.check();\n",
	      TRUE);
  op->set_Description(" Remove the slot if the value is an empty string.\n"
		      " Else set the value for the given attribute, replacing it\n"
		      " if the slot already exist."
		      "\n"
		      " On error : return FALSE in C++, produce a RuntimeException in Java");
  
  // add_Relation()
  
  op = base_class_instance->add_op("add_Relation", PublicVisibility, "bool", TRUE);
  op->add_param(0, InputDirection, "relation", attr);
  op->add_param(1, InputDirection, "other", user_class_instance);
  op->set_cpp("bool", "${t0} * ${p0}, ${t1} * ${p1}",
	      "  QVector<UmlItem> v(2);\n"
	      "\n"
	      "  v.insert(0, relation);\n"
	      "  v.insert(1, other);\n"
	      "  UmlCom::send_cmd(_identifier, addRelationCmd, v);\n"
	      "  return UmlCom::read_bool();\n",
	      FALSE, 0, 0);
  op->set_java("void", "${t0} ${p0}, ${t1} ${p1}",
	       "  UmlItem[] v = new UmlItem[2];\n"
	       "\n"
	       "  v[0] = relation;\n"
	       "  v[1] = other;\n"
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.addRelationCmd, v);\n"
	       "  UmlCom.check();\n",
	      TRUE);
  op->set_Description(" Add the slot (does nothing if it already exist)\n"
		      "\n"
		      " On error : return FALSE in C++, produce a RuntimeException in Java");
  
  // remove_Relation()
  
  op = base_class_instance->add_op("remove_Relation", PublicVisibility, "bool", TRUE);
  op->add_param(0, InputDirection, "relation", attr);
  op->add_param(1, InputDirection, "other", user_class_instance);
  op->set_cpp("bool", "${t0} * ${p0}, ${t1} * ${p1}",
	      "  QVector<UmlItem> v(2);\n"
	      "\n"
	      "  v.insert(0, relation);\n"
	      "  v.insert(1, other);\n"
	      "  UmlCom::send_cmd(_identifier, removeRelationCmd, v);\n"
	      "  return UmlCom::read_bool();\n",
	      FALSE, 0, 0);
  op->set_java("void", "${t0} ${p0}, ${t1} ${p1}",
	       "  UmlItem[] v = new UmlItem[2];\n"
	       "\n"
	       "  v[0] = relation;\n"
	       "  v[1] = other;\n"
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.removeRelationCmd, v);\n"
	       "  UmlCom.check();\n",
	      TRUE);
  op->set_Description(" Remove the slot (does nothing if it doesn't exist)\n"
		      "\n"
		      " On error : return FALSE in C++, produce a RuntimeException in Java");
  
  // assoc diagram
  
  UmlClass * user_objectdiagram = UmlClass::get("UmlObjectDiagram", 0);
  
  add_assoc_diag_ops(base_class_instance, user_objectdiagram);

  // attributes and relations
  
  base_class_instance->add_relation(aDirectionalAssociation,
				    "_type", PrivateVisibility,
				    user_class, 0, 0);  
  
  base_class_instance->add_relation(aDirectionalAssociation,
				    "_assoc_diagram", PrivateVisibility,
				    user_objectdiagram, 0, 0);  
  
  // read_uml
  
  op = base_class_instance->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _assoc_diagram = (UmlObjectDiagram *) UmlBaseItem::read_();\n"
	      "  UmlBaseItem::read_uml_();\n"
	      "  _type = (UmlClass *) UmlBaseItem::read_();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  _assoc_diagram = (UmlObjectDiagram) UmlBaseItem.read_();\n"
	       "  super.read_uml_();\n"
	       "  _type = (UmlClass) UmlBaseItem.read_();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  // constructor
  
  base_class_instance->add_constr(user_item, ProtectedVisibility);

  //
  
  QCString s;
  
  art = base_class_instance->associatedArtifact();
  s = art->cppHeader();
  s.insert(s.find("${includes}"),
	   "#include \"SlotAttribute.h\"\n"
	   "#include \"SlotRelation.h\"\n");
  art->set_CppHeader(s);
  UmlCom::set_user_id(uid);
}

void add_class_instance_on_instance_cmd()
{
  // already root
  UmlClass * itcmd = UmlClass::get("OnInstanceCmd", 0);
  
  itcmd->add_enum_item("setAttributeCmd");
  itcmd->add_enum_item("addRelationCmd");
  itcmd->add_enum_item("removeRelationCmd");
  
  UmlAttribute * at = itcmd->add_enum_item("attributesCmd");
  
  at->moveAfter(itcmd->get_attribute("sideCmd"));
  itcmd->add_enum_item("relationsCmd")->moveAfter(at);
}

void baseitem_read_class_instance(UmlClass * base_item)
{
  // update read_()'s body
  UmlOperation * op = base_item->get_operation("read_");
  
  if (op != 0) {
    QCString body;
    
    body = op->cppBody();
    body.insert(body.findRev("default:"),
		"case aClassInstance:\n\
      return new UmlClassInstance(id, name);\n\
    ");
    op->set_CppBody(body);
      
    body = op->javaBody();
    body.insert(body.findRev("default:"),
		"case anItemKind._aClassInstance:\n\
      return new UmlClassInstance(id, name);\n\
    ");
    op->set_JavaBody(body);
  }
  
  // update BaseUmlItem artifact
  UmlArtifact * art = base_item->associatedArtifact();
  QCString s;
  
  s = art->cppSource();
  s.insert(s.find("#include \"UmlUseCase.h\""),
	   "#include \"UmlClassInstance.h\"\n");
  art->set_CppSource(s);
}

void add_class_instance(UmlClass * base_item, UmlClass * user_item)
{
  UmlClassView * base_class_view = (UmlClassView *) base_item->parent();
  UmlClassView * user_class_view = (UmlClassView *) user_item->parent();
  UmlDeploymentView * base_depl_view = (UmlDeploymentView *)
    base_item->associatedArtifact()->parent();
  UmlDeploymentView * user_depl_view = (UmlDeploymentView *)
    user_item->associatedArtifact()->parent();

  UmlCom::trace("Class instance<br>\n");
  
  add_class_instance(base_class_view, user_class_view, base_depl_view, user_depl_view,
		     base_item, user_item);

  //

  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("update anItemKind<br>\n");
  
  UmlClass * itkind = UmlClass::get("anItemKind", 0);
  
  itkind->add_enum_item("aClassInstance");
  
  UmlCom::trace("update cmd list<br>\n");
  
  add_class_instance_on_instance_cmd();
  
  UmlCom::trace("update item read<br>\n");
  
  baseitem_read_class_instance(base_item);
  
  UmlCom::set_user_id(uid);
}

//
// fixe UmlBaseParameterSet::read_uml_() in java
//

void fixe_parameterset_read_uml()
{
  UmlOperation * op =
    UmlClass::get("UmlBaseParameterSet", 0)->get_operation("read_uml_");
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  op->set_JavaBody("  super.read_uml_();\n"
		   "\n"
		   "  int n = UmlCom.read_unsigned();\n"
		   "\n"
		   "  _pins = new UmlActivityPin[n];\n"
		   "\n"
		   "  for (int index = 0; index != n; index += 1)\n"
		   "    _pins[index] = (UmlActivityPin) UmlBaseItem.read_();\n");
  
  UmlCom::set_user_id(uid);
}

//
// add UmlBaseParameter, move UmlParameter members into it
// add add inheritance
//

void add_umlbaseparameter(UmlClass * uml_base_item, UmlClass * uml_item)
{
  UmlClassView * base_class_view = (UmlClassView *) uml_base_item->parent();
  UmlClassView * user_class_view = (UmlClassView *) uml_item->parent();
  UmlDeploymentView * base_depl_view = (UmlDeploymentView *)
    uml_base_item->associatedArtifact()->parent();
  UmlDeploymentView * user_depl_view = (UmlDeploymentView *)
    uml_item->associatedArtifact()->parent();
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlClass * uml_parameter = UmlClass::get("UmlParameter", 0);
  UmlClass * uml_base_parameter = 
    UmlClass::made(base_class_view, base_depl_view, "UmlBaseParameter");

  uml_base_parameter->set_Stereotype("struct");
  uml_base_parameter->set_Description(uml_parameter->description());
  uml_base_parameter->set_CppDecl(uml_parameter->cppDecl());
  uml_base_parameter->set_JavaDecl(uml_parameter->javaDecl());

  UmlClass * oper;

  oper = UmlClass::get("UmlBaseOperation", 0);
  uml_base_parameter->moveAfter(oper);
  uml_base_parameter->associatedArtifact()->moveAfter(oper->associatedArtifact());

  uml_parameter->moveIn(user_class_view);
  oper = UmlClass::get("UmlOperation", 0);
  uml_parameter->moveAfter(oper);

  UmlArtifact * art = uml_parameter->associatedArtifact();
  
  art->moveIn(user_depl_view);
  art->moveAfter(oper->associatedArtifact());
  art->setUser(uid);

  //

  uml_parameter->get_attribute("default_value")->moveIn(uml_base_parameter);
  uml_parameter->get_attribute("name")->moveIn(uml_base_parameter);

  UmlRelation * rel;
  UmlRelation * old;

  old = uml_parameter->get_relation(aDirectionalAggregationByValue, "type");
  rel = UmlBaseRelation::create(aDirectionalAggregationByValue,
				uml_base_parameter, 
				UmlClass::get("UmlTypeSpec", 0));
  rel->set_CppDecl(old->cppDecl());
  rel->set_JavaDecl(old->javaDecl());
  rel->set_RoleName("type");
  rel->set_Visibility(PublicVisibility);
  old->deleteIt();

  old = uml_parameter->get_relation(aDirectionalAggregationByValue, "dir");
  rel = UmlBaseRelation::create(aDirectionalAggregationByValue,
				uml_base_parameter, 
				UmlClass::get("aDirection", 0));
  rel->set_CppDecl(old->cppDecl());
  rel->set_JavaDecl(old->javaDecl());
  rel->set_RoleName("dir");
  rel->set_Visibility(PublicVisibility);
  old->deleteIt();

  UmlOperation * op;
  QCString cpp_body;
  QCString java_body;
  
  op = uml_parameter->get_operation("clone_it");
  cpp_body = op->cppBody();
  java_body = op->javaBody();
  op->moveIn(uml_base_parameter);
  op->set_CppBody(cpp_body);
  op->set_JavaBody(java_body);

  op = uml_parameter->get_operation("UmlParameter");
  cpp_body = op->cppBody();
  java_body = op->javaBody();  
  op->moveIn(uml_base_parameter);
  op->set_CppBody(cpp_body);
  op->set_JavaBody(java_body);
  op->set_Name("UmlBaseParameter");

  // must not be done before else .body not find
  uml_parameter->setUser(uid);
  
  //

  UmlCom::set_user_id(uid);

  rel = UmlBaseRelation::create(aGeneralisation, uml_parameter, uml_base_parameter);

  if (rel == 0) {
    QCString msg = "UmlParameter can't inherit UmlBaseParameter<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  else {
    rel->set_CppDecl("${type}");
    rel->set_JavaDecl("${type}");
  }
}

//
// add UmlBaseTypeSpec, move UmlTypeSpec members into it
// add add inheritance
//

void add_umlbasetypespec(UmlClass * uml_base_item, UmlClass * uml_item)
{
  UmlClassView * base_class_view = (UmlClassView *) uml_base_item->parent();
  UmlClassView * user_class_view = (UmlClassView *) uml_item->parent();
  UmlDeploymentView * base_depl_view = (UmlDeploymentView *)
    uml_base_item->associatedArtifact()->parent();
  UmlDeploymentView * user_depl_view = (UmlDeploymentView *)
    uml_item->associatedArtifact()->parent();
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlClass * uml_typespec = UmlClass::get("UmlTypeSpec", 0);
  UmlClass * uml_base_typespec = 
    UmlClass::made(base_class_view, base_depl_view, "UmlBaseTypeSpec");

  uml_base_typespec->set_Description(uml_typespec->description());
  uml_base_typespec->set_CppDecl(uml_typespec->cppDecl());
  uml_base_typespec->set_JavaDecl(uml_typespec->javaDecl());

  uml_base_typespec->moveAfter(uml_typespec);
  uml_base_typespec->associatedArtifact()->moveAfter(uml_typespec->associatedArtifact());

  uml_typespec->moveIn(user_class_view);

  UmlArtifact * art = uml_typespec->associatedArtifact();
  
  art->moveIn(user_depl_view);
  art->setUser(uid);

  //

  uml_typespec->get_attribute("explicit_type")->moveIn(uml_base_typespec);

  UmlRelation * rel;
  UmlRelation * old;

  old = uml_typespec->get_relation(aDirectionalAssociation, "type");
  rel = UmlBaseRelation::create(aDirectionalAssociation,
				uml_base_typespec, 
				UmlClass::get("UmlClass", 0));
  rel->set_CppDecl(old->cppDecl());
  rel->set_JavaDecl(old->javaDecl());
  rel->set_RoleName("type");
  rel->set_Visibility(PublicVisibility);
  old->deleteIt();

  UmlOperation * op;
  QCString cpp_body;
  QCString java_body;
  
  op = uml_typespec->get_operation("clone_it");
  cpp_body = op->cppBody();
  java_body = op->javaBody();  
  op->moveIn(uml_base_typespec);
  op->set_CppBody(cpp_body);
  op->set_JavaBody(java_body);
  
  op = uml_typespec->get_operation("toString");
  cpp_body = op->cppBody();
  java_body = op->javaBody();  
  op->moveIn(uml_base_typespec);
  op->set_CppBody(cpp_body);
  op->set_JavaBody(java_body);

  op = uml_typespec->get_operation("UmlTypeSpec");
  cpp_body = op->cppBody();
  java_body = op->javaBody();  
  op->moveIn(uml_base_typespec);
  op->set_CppBody(cpp_body);
  op->set_JavaBody(java_body);
  op->set_Name("UmlBaseTypeSpec");

  // must not be done before else .body not find
  uml_typespec->setUser(uid);
  
  //

  UmlCom::set_user_id(uid);

  rel = UmlBaseRelation::create(aGeneralisation, uml_typespec, uml_base_typespec);

  if (rel == 0) {
    QCString msg = "UmlTypeSpec can't inherit UmlBaseTypeSpec<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  else {
    rel->set_CppDecl("${type}");
    rel->set_JavaDecl("${type}");
  }
}

//
// add UmlBaseView and UmlView classes between base view
// classes and UmlItem
//

void add_umlbaseview_umlview(UmlClass * base_item, UmlClass * user_item)
{
  UmlClassView * base_class_view = (UmlClassView *) base_item->parent();
  UmlClassView * user_class_view = (UmlClassView *) user_item->parent();
  UmlDeploymentView * base_depl_view = (UmlDeploymentView *)
    base_item->associatedArtifact()->parent();
  UmlDeploymentView * user_depl_view = (UmlDeploymentView *)
    user_item->associatedArtifact()->parent();
  UmlClass * base_view;
  UmlClass * user_view;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "View", base_view, user_view,
		  0);

  user_view->add_constr(base_view, ProtectedVisibility);
  user_view->set_isAbstract(TRUE);

  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
    
  base_view->add_constr(user_item, ProtectedVisibility); 
  
  const char * views[] = {
    "UmlBaseClassView",
    "UmlBaseComponentView",
    "UmlBaseDeploymentView",
    "UmlBaseUseCaseView"
  };
  
  for (unsigned i = 0; i != sizeof(views)/sizeof(views[0]); i += 1) {
    UmlClass * bview = UmlClass::get(views[i], 0);
    UmlRelation * rel =
      UmlBaseRelation::create(aGeneralisation, bview, user_view);
    
    if (rel == 0) {
      QCString msg = QCString(views[i]) + " can't inherit UmlView<br>\n";
      
      UmlCom::trace(msg);
      throw 0;
    }
    rel->set_CppDecl("${type}");
    rel->set_JavaDecl("${type}");
    bview->get_relation(aGeneralisation, 0)->deleteIt(); // old gene
    rel->moveAfter(0);	// after old gene del
    
    UmlOperation * op = bview->get_operation(views[i]);
    QCString s = op->cppDecl();
    
    op->set_CppDecl(s.replace(s.find(": UmlItem"), 9, ": UmlView"));
  }
  
  //

  UmlCom::set_user_id(uid);
}

//
// add multiplicity on attributes
//

void add_multiplicity(UmlClass * settings, QCString attr,
			     QCString get, QCString set,
			     QCString who, QCString Who, QCString what)
{
  
  UmlOperation * op;
  QCString s;
  
  // upgrade get
  
  op = settings->get_operation(get);
  op->add_param(0, InputDirection, "multiplicity", "str");
  s = op->cppDecl();
  op->set_CppDecl(s.insert(s.find("${)}"), "const ${t0} ${p0}"));
  s = op->cppDef();
  op->set_CppDef(s.insert(s.find("${)}"), "const ${t0} ${p0}"));
  op->set_CppBody("  read_if_needed_();\n"
		  "\n"
		  "  return " + attr + "[UmlSettings::multiplicity_column(multiplicity)];\n");
  s = op->javaDecl();
  op->set_JavaDef(s.insert(s.find("${)}"), "${t0} ${p0}"));
  op->set_JavaBody("  read_if_needed_();\n"
		   "\n"
		   "  return " + attr + "[UmlSettings.multiplicity_column(multiplicity)];\n");
  
  op->set_Description(op->description() + " depending on the multiplicity");
  
  // upgrade set
  
  op = settings->get_operation(set);
  op->add_param(0, InputDirection, "multiplicity", "str");
  s = op->cppDecl();
  op->set_CppDecl(s.replace(s.find("${t0}"), 11,
			    "const ${t0} ${p0}, ${t1} ${p1}"));
  s = op->cppDef();
  op->set_CppDef(s.replace(s.find("${t0}"), 11,
			   "const ${t0} ${p0}, ${t1} ${p1}"));
  op->set_CppBody("  UmlCom::send_cmd(" + who + "SettingsCmd, set" + Who + what + "DeclCmd, multiplicity, v);\n"
		  "  if (UmlCom::read_bool()) {\n"
		  "    " + attr + "[UmlSettings::multiplicity_column(multiplicity)] = v;\n"
		  "    return TRUE;\n"
		  "  }\n"
		  "  else\n"
		  "    return FALSE;\n");

  s = op->javaDecl();
  op->set_JavaDef(s.insert(s.find("${)}"), ", ${t1} ${p1}"));
  op->set_JavaBody("  UmlCom.send_cmd(CmdFamily." + who + "SettingsCmd, " + Who + "SettingsCmd._set" + Who + what + "DeclCmd, multiplicity, v);\n"
		   "  UmlCom.check();\n"
		   "\n"
		   "  " + attr + "[UmlSettings.multiplicity_column(multiplicity)] = v;\n");
  
  UmlAttribute * att = settings->get_attribute(attr);
  
  s = att->cppDecl();
  att->set_CppDecl(s.insert(s.find("${name}")+7, "[3/*multiplicity*/]"));
  s = att->javaDecl();
  att->set_JavaDecl(s.insert(s.find("${name}")+7, "[/*multiplicity*/]"));

  
  // upgrade read_
  
  op = settings->get_operation("read_");
  s = op->cppBody();
  op->set_CppBody(s.replace(s.find(attr + " ="), attr.length() + 2,
			    "for (index = 0; index != 3; index += 1)\n"
			    "    " + attr + "[index] ="));
  s = op->javaBody();
  op->set_JavaBody(s.replace(s.find(attr + " ="), attr.length() + 2,
			     "for (index = 0; index != 3; index += 1)\n"
			     "    " + attr + "[index] ="));
}

void add_attribute_multiplicity(UmlClass * settings, QCString who, QCString Who)
{  
  UmlCom::trace("upgrade " + Who + "Settings<br>");
  
  settings->get_operation("relationStereotype")->set_Name("relationAttributeStereotype");
  settings->get_operation("set_RelationStereotype")->set_Name("set_RelationAttributeStereotype");
  settings->get_operation("relationUmlStereotype")->set_Name("relationAttributeUmlStereotype");

  add_multiplicity(settings, "_attr_decl", "attributeDecl", "set_AttributeDecl",
		   who, Who, "Attribute");
}

void rename_in(QCString & s, QCString from, QCString to)
{
  int index = 0;
  
  while ((index = s.find(from, index)) != -1) {
    s.replace(index, from.length(), to);
    index += to.length() - from.length();
  }
}

QCString rename_in(QCString s)
{
  rename_in(s, "elationStereotype", "elationAttributeStereotype");
  rename_in(s, "relationUmlStereotype", "relationAttributeUmlStereotype");
  rename_in(s, "_relation_stereotypes", "_relation_attribute_stereotypes");
  rename_in(s, "_rel_stereotype", "_rel_attr_stereotype");
  
  return s;
}

void rename_in(UmlOperation * op)
{
  op->set_CppBody(rename_in(op->cppBody()));
  op->set_JavaBody(rename_in(op->javaBody()));
  op->set_Description(rename_in(op->description()));
}

void add_attribute_multiplicity(UmlClass * umlsettings, UmlClass * cppsettings, 
				UmlClass * javasettings, UmlClass * idlsettings)
{
  UmlCom::trace("Add multiplicity on attribute<br>\n");
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  // copy multiplicity() and set_Multiplicity() from UmlBaseRelation
  
  UmlClass * attribute = UmlClass::get("UmlBaseAttribute", 0);
  UmlClass * relation = UmlClass::get("UmlBaseRelation", 0);
  UmlOperation * op1, * op2;
  
  op1 = relation->get_operation("multiplicity");
  op2 = attribute->add_op("multiplicity", PublicVisibility,
			  op1->returnType().explicit_type, FALSE);
  op2->set_CppDecl(op1->cppDecl());
  op2->set_CppDef(op1->cppDef());
  op2->set_CppBody(op1->cppBody());
  op2->set_JavaDef(op1->javaDef());
  op2->set_JavaBody(op1->javaBody());
  op2->set_Description(op1->description());
  op2->moveAfter(attribute->get_operation("set_Type"));
  
  op1 = relation->get_operation("set_Multiplicity");
  op2 = attribute->add_op("set_Multiplicity", PublicVisibility,
			  op1->returnType().explicit_type, TRUE);
  op2->addParameter(0, op1->params()[0]);
  op2->addParameter(1, op1->params()[1]);
  op2->set_CppDecl(op1->cppDecl());
  op2->set_CppDef(op1->cppDef());
  op2->set_CppBody(op1->cppBody());
  op2->set_JavaDef(op1->javaDef());
  op2->set_JavaBody(op1->javaBody());
  op2->set_Description(op1->description());
  op2->moveAfter(attribute->get_operation("multiplicity"));
  
  // copy _multiplicity from UmlBaseRelation
  
  UmlAttribute * att1, * att2;
  
  att1 = relation->get_attribute("_multiplicity");
  att2 = attribute->add_attribute("_multiplicity", PublicVisibility,
				  att1->type().explicit_type, 0, 0);
  att2->set_CppDecl(att1->cppDecl());
  att2->set_JavaDecl(att1->javaDecl());
  att2->moveAfter(attribute->get_attribute("_read_only"));
  
  // modify read_uml_()
  
  QCString s;
  
  op1 = attribute->get_operation("read_uml_");
  s = op1->cppBody();
  op1->set_CppBody(s.insert(s.find("_default_value ="),
			    "_multiplicity = UmlCom::read_string();\n"
			    "  "));
  s = op1->javaBody();
  op1->set_JavaBody(s.insert(s.find("_default_value ="),
			     "_multiplicity = UmlCom.read_string();\n"
			     "  "));
  
  // modify unload

  op1 = attribute->get_operation("unload");
  s = op1->cppBody();
  op1->set_CppBody(s + "  _multiplicity = 0;\n");
  s = op1->javaBody();
  op1->set_JavaBody(s + "  _multiplicity = null;\n");
  
  // upgrade *Settings
  
  add_attribute_multiplicity(cppsettings, "cpp", "Cpp");
  add_attribute_multiplicity(javasettings, "java", "Java");
  add_attribute_multiplicity(idlsettings, "idl", "Idl");
  
  add_multiplicity(idlsettings, "_valuetype_attr_decl", "valuetypeAttributeDecl",
		   "set_ValuetypeAttributeDecl", "idl", "Idl", "ValuetypeAttribute");
  add_multiplicity(idlsettings, "_union_item_decl", "unionItemDecl",
		   "set_UnionItemDecl", "idl", "Idl", "UnionItem");
  add_multiplicity(idlsettings, "_const_decl", "constDecl",
		   "set_ConstDecl", "idl", "Idl", "Const");

  // rename cmds
  
  UmlCom::trace("rename settings cmds<br>\n");
  
  UmlClass::get("CppSettingsCmd", 0)->get_attribute("setCppRelationStereotypeCmd")
    ->set_Name("setCppRelationAttributeStereotypeCmd");
  UmlClass::get("JavaSettingsCmd", 0)->get_attribute("setJavaRelationStereotypeCmd")
    ->set_Name("setJavaRelationAttributeStereotypeCmd");
  UmlClass::get("IdlSettingsCmd", 0)->get_attribute("setIdlRelationStereotypeCmd")
    ->set_Name("setIdlRelationAttributeStereotypeCmd");

  //
  
  UmlCom::trace("upgrade UmlSettings<br>\n");
  
  umlsettings->get_attribute("_map_relation_stereotypes")
    ->set_Name("_map_relation_attribute_stereotypes");
  umlsettings->get_relation(aDirectionalAssociation, "_relation_stereotypes")
    ->set_RoleName("_relation_attribute_stereotypes");

  umlsettings->get_operation("uml_rel_stereotype")->set_Name("uml_rel_attr_stereotype");
  umlsettings->get_operation("add_rel_stereotype")->set_Name("add_rel_attr_stereotype");    

  //
  
  UmlCom::trace("upgrade body to take into account renamings<br>\n");
  
  rename_in(cppsettings->get_operation("set_RelationAttributeStereotype"));
  rename_in(cppsettings->get_operation("relationAttributeStereotype"));
  rename_in(cppsettings->get_operation("relationAttributeUmlStereotype"));
  rename_in(cppsettings->get_operation("read_"));
  
  rename_in(javasettings->get_operation("set_RelationAttributeStereotype"));
  rename_in(javasettings->get_operation("relationAttributeStereotype"));
  rename_in(javasettings->get_operation("relationAttributeUmlStereotype"));
  rename_in(javasettings->get_operation("read_"));
  
  rename_in(idlsettings->get_operation("set_RelationAttributeStereotype"));  
  rename_in(idlsettings->get_operation("relationAttributeStereotype"));
  rename_in(idlsettings->get_operation("relationAttributeUmlStereotype"));
  rename_in(idlsettings->get_operation("read_"));
  
  rename_in(umlsettings->get_operation("read_"));
  rename_in(umlsettings->get_operation("uml_rel_attr_stereotype"));
  rename_in(umlsettings->get_operation("add_rel_attr_stereotype"));
  
  //

  UmlCom::set_user_id(uid);
}

//

void fixe_set_name(UmlClass * baseitem)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlOperation * op = baseitem->get_operation("set_Name");
  
  op->set_CppBody("  UmlCom::send_cmd(_identifier, setNameCmd, s);\n"
		  "  if (UmlCom::read_bool()) {\n"
		  "    _name = s;\n"
		  "    return TRUE;\n"
		  "  }\n"
		  "  else\n"
		  "    return FALSE;\n");
  
  op->set_JavaBody("    UmlCom.send_cmd(identifier_(), OnInstanceCmd.setNameCmd, s);\n"
		   "    UmlCom.check();\n"
		   "    _name = s;\n");
  
  //

  UmlCom::set_user_id(uid);
  
}

//

void add_external(UmlClass * transition)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlAttribute * att = 
    transition->add_attribute("_is_external", PublicVisibility, "bool", 0, 0);
  
  att->moveAfter(transition->get_relation(aDirectionalAssociation, "_target"));
  
  UmlOperation * op;
  UmlOperation * get;
  
  defGetBool(transition, _is_external, isExternal, 0, 0, "");
  op->set_Description(" return if the transition is internal or external,\n"
		      " only self transitions can't be external");
  op->moveAfter(transition->get_operation("target"));
  get = op;
  
  defSetBool(transition, _is_external, set_IsExternal, setIsCppExternalCmd, 0, 0,
	     "");
  op->set_Description(" set if the transition is internal or external,\n"
		      " only a self transition may be set internal"
		      "\n"
		      " On error return FALSE in C++, produce a RuntimeException in Java");
  op->moveAfter(get);
    
  QCString body;
    
  op = transition->get_operation("read_uml_");
  body = op->cppBody();
  body += "  _is_external = !UmlCom::read_bool();\n";
  op->set_CppBody(body);
      
  body = op->javaBody();
  body += "  _is_external = !UmlCom.read_bool();\n";
  op->set_JavaBody(body);
  
  //

  UmlCom::set_user_id(uid);
}

//
//
//

bool ask_for_upgrade()
{
  if (QMessageBox::warning(0, "Upgrade",
			   "VERY IMPORTANT : in case you valid the upgrade and\n"
			   "the message indicating that all is done never appears,\n"
			   "you MUST close the project WITHOUT saving it\n\n\n"
			   "Upgrade the plug-out ?",
			   "Yes", "No", QString::null, 1, 1)
	  != 0)
    return FALSE;
  
  if (! UmlCom::set_root_permission(TRUE)) {
    QMessageBox::critical(0, "Upgrade", 
			  "Can't be done until all the dialog are closed");
    return FALSE;
  }
   
  return TRUE;
}

//

void update_api_version(const char * v)
{
  UmlClass * com = UmlClass::get("UmlCom", 0);
  const QVector<UmlItem> ch = com->children();
  
  for (unsigned i = 0; i != ch.size(); i += 1){
    if ((ch[i]->kind() == anOperation) &&
	(ch[i]->name() == "connect")) {
      unsigned uid = UmlCom::user_id();
      UmlOperation * op = (UmlOperation *) ch[i];
      QCString body;
      int index1;
      int index2;
      
      UmlCom::set_user_id(0);
  
      body = op->cppBody();
      index1 = body.find("write_unsigned(") + 15;
      index2 = body.find(")", index1);
      body.replace(index1, index2-index1, v);
      op->set_CppBody(body);
      
      body = op->javaBody();
      index1 = body.find("write_unsigned(") + 15;
      index2 = body.find(")", index1);
      body.replace(index1, index2-index1, v);
      op->set_JavaBody(body);
      
      UmlCom::set_user_id(uid);
      
      return;
    }
  }
}

//
// Entry point
//

bool UmlPackage::upgrade() {
  UmlClass * uml_base_item = UmlClass::get("UmlBaseItem", 0);
  UmlClass * uml_item = UmlClass::get("UmlItem", 0);
  
  if ((uml_base_item == 0) || (uml_item == 0))
    UmlCom::trace("<tt>        </tt><font face=helvetica color=red>Not a plug-out</font><br>\n");
  else {
    bool work = FALSE;
    
    if (UmlClass::get("UmlBaseState", 0) == 0) {
      if (! ask_for_upgrade())
	return FALSE;
      
      set_childrenVisible(FALSE);
      upgrade_states(uml_base_item, uml_item);
      work = TRUE;
    }
    
    UmlClass * uml_base_class = UmlClass::get("UmlBaseClass", 0);
    
    if (! class_set_name_defined(uml_base_class)) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      upgrade_rename_class(uml_base_item, uml_base_class);
      work = TRUE;
    }
    
    if (! item_move_after_defined(uml_base_item)) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      upgrade_new_settings__move_after_item(uml_base_item, uml_item);
      work = TRUE;
    }
    
    if (one_component_per_class(uml_base_class)) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      several_components_per_class(uml_base_class);
      work = TRUE;
    }
    
    UmlClass * javasettings = UmlClass::get("JavaSettings", 0);
    
    if (java_jdk1_4(javasettings)) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      upgrade_jdk5(javasettings);
      fixe_package_diagram();
      fixe_umlbaseactualparameter_read();
      work = TRUE;
    }
    
    UmlClass * baserelation = UmlClass::get("UmlBaseRelation", 0);
    
    if (baserelation->get_operation("association") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_class_association(baserelation);
      add_package_visibility();
      work = TRUE;
    }
    
    if (baserelation->get_operation("side") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_side(baserelation);
      work = TRUE;
    }
    
    if (UmlClass::get("UmlBaseObjectDiagram", 0) == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_objectdiagram(uml_base_item, uml_item);
      work = TRUE;
    }
    
    if (UmlClass::get("UmlBaseActivity", 0) == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_class_instance(uml_base_item, uml_item);
      base_state_include_umlcom();
      work = TRUE;
    }
    
    UmlClass * uml_com = UmlClass::get("UmlCom", 0);
    
    if (uml_com->get_operation("showTrace") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_new_trace_operations(uml_com);
      work = TRUE;
    }
    
    UmlClass * basepinparam = UmlClass::get("UmlBasePinParameter", 0);
    
    if (basepinparam->get_operation("isStream") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      fixe_activity(basepinparam);
      work = TRUE;
    }

    UmlClass * cppsetting = UmlClass::get("CppSettings", 0);

    if (cppsetting->get_attribute("_is_set_param_ref") == 0)  {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_cpp_set_param_ref(cppsetting);
      upgrade_setter_getter();

      work = TRUE;
    }

    if (cppsetting->get_attribute("_is_relative_path") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_cpp_relative_path_force_namespace(cppsetting);

      work = TRUE;
    }

    UmlClass * umlsetting = UmlClass::get("UmlSettings", 0);

    if (umlsetting->get_operation("umlGetName") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_getter_setter_rules(umlsetting);
      add_extension_points();

      work = TRUE;
    }
    
    if (uml_base_class->get_attribute("_java_public") != 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      remove_java_public(uml_base_class);
      add_cpp_root_relative_path(cppsetting);

      work = TRUE;
    }
    
    if (cppsetting->get_attribute("_is_generate_javadoc_comment") == 0)  {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_cpp_generate_javadoc_comment(cppsetting);
      add_java_generate_javadoc_comment(UmlClass::get("JavaSettings", 0));

      work = TRUE;
    }    
    
    UmlClass * baseclassmember = UmlClass::get("UmlBaseClassMember", 0);
    
    if (baseclassmember->get_attribute("_constraint") == 0)  {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_constraint(baseclassmember);

      work = TRUE;
    }    
    
    if (uml_base_item->get_operation("deleteIt") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_deleteit(uml_base_item);

      work = TRUE;
    }   
    
    if (uml_base_item->get_attribute("_modeler_id") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_get_id(uml_base_item);
      add_class_instance(uml_base_item, uml_item);
      fixe_parameterset_read_uml();
      
      work = TRUE;
    }

    if (UmlClass::get("UmlBaseParameter", 0) == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_umlbaseparameter(uml_base_item, uml_item);
      add_umlbasetypespec(uml_base_item, uml_item);
      add_umlbaseview_umlview(uml_base_item, uml_item);
      
      work = TRUE;
    }
    
    if (cppsetting->get_operation("relationAttributeStereotype") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      fixe_set_name(uml_base_item);
      add_attribute_multiplicity(umlsetting, cppsetting, javasettings,
				 UmlClass::get("IdlSettings", 0));

      work = TRUE;
    }
    
    UmlClass * transition = UmlClass::get("UmlBaseTransition", 0);
    
    if (transition->get_operation("isExternal") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_external(transition);

      work = TRUE;
    }
    
    if (work) {
      UmlCom::trace("update api version<br>\n");
      update_api_version("33");
      UmlCom::message("ask for save-as");
      QMessageBox::information(0, "Upgrade", 
			       "Upgrade done\n\n"
			       "a 'save as' will be called, thus the project will be closed");  
      UmlCom::set_root_permission(FALSE);
      return TRUE;
    }
    else
      UmlCom::trace("<tt>        </tt><font face=helvetica>Plug-out already up to date</font><br>\n");
  }
  
  return FALSE;
}
