#include <qptrdict.h>
#include <qdict.h>
#include <qpopupmenu.h>
#include <qstack.h>

#include "ProfiledStereotypes.h"
#include "BrowserClass.h"
#include "BrowserClassView.h"
#include "BrowserAttribute.h"
#include "BrowserPackage.h"
#include "BrowserRelation.h"
#include "BrowserView.h"
#include "RelationData.h"
#include "AttributeData.h"
#include "Tool.h"
#include "ToolCom.h"
#include "DialogUtil.h"

struct ProfiledStereotype {
  ProfiledStereotype() : cl(0), properties_set(FALSE) {}
				
  BrowserClass * cl;
  QStringList propertiesFullName; // profile:stereotype:property
  QValueList<BrowserAttribute *> properties;
  bool properties_set;
  
  void clear_properties();
  void set_properties();
  void renamed(BrowserAttribute * at);
  void deleted(BrowserAttribute * at);
  bool enumerated(QString, QStringList & l) const;
};

// stereotypable element kind form -> code
static QDict<UmlCode> UnPretty2Code;

// pretty <-> encoded element kind
static QDict<QString> Pretty;
static QDict<QString> UnPretty;

// all unpretty element kinds
static QStringList Targets;

// list of stereotypes applicable to an element kind
// all relation are managed through UmlRelations
static QStringList Defaults[BrowserNodeSup];

// key = an element (BrowserNode), give the stereotype
// if its stereotype is modeled
static QPtrDict<ProfiledStereotype> ProfiledStereotyped(257);

// all the stereotype, key = profile:stereotype
static QDict<ProfiledStereotype> All;

// to continue to apply check recursively
struct CheckCell {
  BrowserNode * bn;
  bool next; // else children
  
  CheckCell(BrowserNode * b, bool n) : bn(b), next(n) {}
};

static QStack<CheckCell> CheckStack;

void ProfiledStereotype::clear_properties() {
  properties.clear(); 
  propertiesFullName.clear();
  properties_set = FALSE;
}

void ProfiledStereotype::set_properties() {
  if (properties_set)
    return;
  
  properties_set = TRUE;
  
  BrowserNode * child = (BrowserNode *) cl->firstChild();
  QString prefix = 
    ((BrowserNode *) cl->parent()->parent())->get_name() +
      QString(":") + cl->get_name() + QString(":");

  while (child != 0) {
    switch (child->get_type()) {
    case UmlAttribute:
      if (! child->deletedp()) {
	properties.append((BrowserAttribute *) child);
	propertiesFullName.append(prefix + child->get_name());
      }
      break;
    case UmlGeneralisation:
    case UmlRealize:
      if (! child->deletedp()) {
	BrowserClass * super =
	  ((RelationData *) child->get_data())->get_end_class();
	ProfiledStereotype * super_st =
	  All[((BrowserNode *) super->parent()->parent())->get_name() +
	      QString(":") + super->get_name()];
	
	if ((super_st != 0) && (super_st->cl == super)) {
	  super_st->set_properties();
	  properties += super_st->properties;
	  propertiesFullName += super_st->propertiesFullName;
	}
      }
      break;
    default:
      break;
    }
    
    child = (BrowserNode *) child->nextSibling();
  }
}

// an attribute is renamed, st is pack:class
void ProfiledStereotype::renamed(BrowserAttribute * at) {
  int rank = properties.findIndex(at);
  
  if (rank == -1)
    return;
  
  QString prop_oldname = propertiesFullName[rank];
  QString prop_newname = 
    prop_oldname.left(prop_oldname.findRev(':')) + at->get_name();
  const char * oldprop = (const char *) prop_oldname;
  const char * newprop = (const char *) prop_newname;
  
  propertiesFullName[rank] = prop_newname;

  QPtrDictIterator<ProfiledStereotype> it(ProfiledStereotyped);

  while (it.current() != 0) {
    // update properties
    // even if it.current() != this because of inheritances
    BrowserNode * bn = (BrowserNode *) it.currentKey();
    unsigned nprop = bn->get_n_keys();
    
    for (unsigned index = 0; index != nprop; index += 1) {
      if (!strcmp(bn->get_key(index), oldprop)) {
	bn->set_key(index, newprop);
	break;
      }
    }
  
    ++it;
  }
}

// the attribute is deleted
void ProfiledStereotype::deleted(BrowserAttribute * at) {
  int rank = properties.findIndex(at);

  if (rank == -1)
    return;

  QString prop_name = propertiesFullName[rank];
  const char * prop = (const char *) prop_name;
  
  propertiesFullName.remove(propertiesFullName.at(rank));
  properties.remove(properties.at(rank));

  QPtrDictIterator<ProfiledStereotype> it(ProfiledStereotyped);

  while (it.current() != 0) {
    // update properties
    // even if it.current() != this because of inheritances
    BrowserNode * bn = (BrowserNode *) it.currentKey();
    unsigned nprop = bn->get_n_keys();
      
    for (unsigned index = 0; index != nprop; index += 1) {
      if (!strcmp(bn->get_key(index), prop)) {
    	bn->remove_key_value(index);
	bn->modified();
	break;
      }
      ++it;
    }
  }
}

// k is a property part of the stereotype,
// if its type return TRUE and set 'l' to the enum's items
bool ProfiledStereotype::enumerated(QString k, QStringList & l) const {
  int index = propertiesFullName.findIndex(k);
  
  if (index == -1)
    // non consistent
    return FALSE;
  
  BrowserAttribute * at = properties[index];
  const AType & t = ((AttributeData *) at->get_data())->get_type();
  
  if ((t.type == 0) || 
      strcmp(t.type->get_data()->get_stereotype(), "enum"))
    return FALSE;
  
  l.clear();
  
  BrowserNode * bn;
  
  for (bn = (BrowserNode *) t.type->firstChild();
       bn != 0;
       bn = (BrowserNode *) bn->nextSibling())
    if ((bn->get_type() == UmlAttribute) && !bn->deletedp())
      l.append(bn->get_name());
  
  return !l.isEmpty();
}

//
  
void ProfiledStereotypes::init()
{
  CheckStack.setAutoDelete(TRUE);
  CheckStack.clear();
  CheckStack.setAutoDelete(FALSE);
  
  int index;
  
  for (index = 0; index != BrowserNodeSup; index += 1)
    Defaults[index].clear();
  
  ProfiledStereotyped.clear();
  
  All.setAutoDelete(TRUE);
  All.clear();
  All.setAutoDelete(FALSE);
  
  if (UnPretty2Code.isEmpty()) {
    UmlCode * pcode;
  
#define ADD(c, unp, p) \
    pcode = new UmlCode; \
    *pcode = c; \
    UnPretty2Code.insert(unp, pcode); \
    Targets.append(unp); \
    Pretty.insert(unp, new QString(p)); \
    UnPretty.insert(p, new QString(unp));
		     
    ADD(UmlRelations, "Relation", "Relation");
    ADD(UmlAttribute, "Attribute", "Attribute");
    ADD(UmlOperation, "Operation", "Operation");
    ADD(UmlExtraMember, "ExtraMember", "Extra Member");
    ADD(UmlClass, "Class", "Class");
    ADD(UmlUseCase, "UseCase", "Use Case");
    ADD(UmlComponent, "Component", "Component");
    ADD(UmlDeploymentNode, "DeploymentNode", "Deployment Node");
    ADD(UmlArtifact, "Artifact", "Artifact");
    ADD(UmlClassDiagram, "ClassDiagram", "Class Diagram");
    ADD(UmlUseCaseDiagram, "UseCaseDiagram", "UseCase Diagram");
    ADD(UmlSeqDiagram, "SequenceDiagram", "Sequence Diagram");
    ADD(UmlColDiagram, "CommunicationDiagram", "Communication Diagram");
    ADD(UmlComponentDiagram, "ComponentDiagram", "Component Diagram");
    ADD(UmlDeploymentDiagram, "DeploymentDiagram", "Deployment Diagram");
    ADD(UmlObjectDiagram, "ObjectDiagram", "Object Diagram");
    ADD(UmlActivityDiagram, "ActivityDiagram", "Activity Diagram");
    ADD(UmlClassView, "ClassView", "Class View");
    ADD(UmlUseCaseView, "UseCaseView", "Use Case View");
    ADD(UmlComponentView, "ComponentView", "Component View");
    ADD(UmlDeploymentView, "DeploymentView", "Deployment View");
    ADD(UmlPackage, "Package", "Package");
    ADD(UmlState, "State", "State");
    ADD(UmlTransition, "Transition", "Transition");
    ADD(UmlRegion, "Region", "Region");
    ADD(UmlStateDiagram, "StateDiagram", "State Diagram");
    ADD(UmlStateAction, "StateAction", "State Action");
    ADD(InitialPS, "InitialPS", "Initial Pseudo State");
    ADD(EntryPointPS, "EntryPointPS", "EntryPoint Pseudo State");
    ADD(FinalPS, "FinalPS", "Final State");
    ADD(TerminatePS, "TerminatePS", "Terminate Pseudo State");
    ADD(ExitPointPS, "ExitPointPS", "Exit Point Pseudo State");
    ADD(DeepHistoryPS, "DeepHistoryPS", "Deep History Pseudo State");
    ADD(ShallowHistoryPS, "ShallowHistoryPS", "Shallow History Pseudo State");
    ADD(JunctionPS, "JunctionPS", "Junction Pseudo State");
    ADD(ChoicePS, "ChoicePS", "Choice Pseudo State");
    ADD(ForkPS, "ForkPS", "Fork Pseudo State");
    ADD(JoinPS, "JoinPS", "Join Pseudo State");
    ADD(UmlActivity, "Activity", "Activity");
    ADD(UmlFlow, "Flow", "Flow");
    ADD(UmlParameter, "Parameter", "Parameter");
    ADD(UmlParameterSet, "ParameterSet", "Parameter Set");
    //ADD(UmlPartition, "Partition", "Partition");
    ADD(UmlExpansionRegion, "ExpansionRegion", "Expansion Region");
    ADD(UmlInterruptibleActivityRegion, "InterruptibleActivityRegion", "Interruptible Activity Region");
    ADD(UmlActivityAction, "ActivityAction", "Activity Action");
    ADD(UmlActivityObject, "ActivityObject", "Activity Object");
    ADD(UmlExpansionNode, "ExpansionNode", "Expansion Node");
    ADD(UmlActivityPin, "ActivityPin", "Activity Pin");
    ADD(InitialAN, "InitialAN", "Initial Activity Node");
    ADD(FlowFinalAN, "FlowFinalAN", "Flow Final Activity Node");
    ADD(ActivityFinalAN, "ActivityFinalAN", "Activity Final Activity Node");
    ADD(DecisionAN, "DecisionAN", "Decision Activity Node");
    ADD(MergeAN, "MergeAN", "Merge Activity Node");
    ADD(ForkAN, "ForkAN", "Fork Activity Node");
    ADD(JoinAN, "JoinAN", "Join Activity Node");
    ADD(UmlClassInstance, "ClassInstance", "Class Instance");
    
    Targets.sort();
  
#undef ADD
  }
}

bool ProfiledStereotypes::hasStereotype()
{
  return !All.isEmpty();
}

const QStringList & ProfiledStereotypes::defaults(UmlCode c)
{
  return Defaults[c];
}

const QStringList & ProfiledStereotypes::availableTargets()
{
  return Targets;
}

QString ProfiledStereotypes::pretty(QString s)
{
  QString * p = Pretty[s];
  
  return (p == 0) ? QString("") : *p;
}

QString ProfiledStereotypes::unpretty(QString s)
{
  QString * p = UnPretty[s];
  
  return (p == 0) ? QString("") : *p;
}

static void setDefault(QString st, QString sl)
{
  if (! sl.isEmpty()) {
    QChar sep(' ');
    QStringList ls = QStringList::split(sep, sl);
    QStringList::Iterator it;
    
    for (it = ls.begin(); it != ls.end(); ++it) {
      UmlCode * pc = UnPretty2Code[*it];
      
      if (pc != 0)
	Defaults[*pc].append(st);
    }
  }
}

// the class being a stereotype or its package being a package was renamed
void renamed(BrowserClass * cl, QString oldname, QString oldpkname)
{
  QString oldst = oldpkname + ":" + oldname;
  QString newst = ((BrowserNode *) cl->parent()->parent())->get_name()
    + QString(":") + cl->get_name();
  unsigned index;
  
  for (index = 0; index != BrowserNodeSup; index += 1)
    Defaults[index].remove(oldst);

  setDefault(newst, cl->get_value("stereotypeApplyOn"));
  
  ProfiledStereotype * pst = All[oldst];
  
  All.remove(oldst);
  All.insert(newst, pst);
  
  oldst += ":";
  
  QPtrDictIterator<ProfiledStereotype> it(ProfiledStereotyped);
  const char * oldsst = (const char *) oldst;
  int oldlen = oldst.length();
  QCString newcst = (const char *) newst;
  
  newcst += ":";
  
  while (it.current()) {
    BrowserNode * bn = (BrowserNode *) it.currentKey();
    
    // update stereotype
    if (it.current() == pst)
      bn->get_data()->set_stereotype(newst);
    
    // update properties
    // even if it.current() != pst because of inheritances
    unsigned nprop = bn->get_n_keys();
    
    for (index = 0; index != nprop; index += 1) {
      const char * k = bn->get_key(index);
      
      if (!strncmp(k, oldsst, oldlen)) {
	QCString newk = newcst + (k + oldlen);
	
	bn->set_key(index, (const char *) newk);
      }
    }
    
    bn->modified();
    
    ++it;
  }
}

// indicate if a profile can be added
bool ProfiledStereotypes::canAddPackage(BrowserPackage * pk, QString name)
{
  QString pfix = name + QString(":");

  BrowserNode * bn;
  
  for (bn = (BrowserNode *) pk->firstChild();
       bn != 0;
       bn = (BrowserNode *) bn->nextSibling())
    if ((bn->get_type() == UmlClass) && 
	!bn->deletedp() &&
	!strcmp(bn->get_data()->get_stereotype(), "stereotype") &&
	(All[pfix + QString(bn->get_name())] != 0))
      return false;
  
  return TRUE;
}

// pk is and was a profile, it is renamed
void ProfiledStereotypes::renamed(BrowserPackage * pk, QString oldname)
{
  BrowserNode * v;
  
  for (v = (BrowserNode *) pk->firstChild();
       v != 0;
       v = (BrowserNode *) v->nextSibling()) {
    if ((v->get_type() == UmlClassView) && !v->deletedp()) {
      BrowserNode * bn;
      
      for (bn = (BrowserNode *) v->firstChild();
	   bn != 0;
	   bn = (BrowserNode *) bn->nextSibling())
	if ((bn->get_type() == UmlClass) && 
	    !bn->deletedp() &&
	    !strcmp(bn->get_data()->get_stereotype(), "stereotype"))
	  ::renamed((BrowserClass *) bn, bn->get_name(), oldname);
    }
  }
}

static bool deleted(QString pfix, BrowserClass * cl, bool propag)
{
  pfix += cl->get_name();

  ProfiledStereotype * pst = All.find(pfix);

  if (pst != 0) {
    QPtrDictIterator<ProfiledStereotype> it(ProfiledStereotyped);

    while (it.current()) {
      if (it.current() == pst) {
	if (propag) {
	  ((BrowserNode *) it.currentKey())->get_data()->set_stereotype("");
	  ((BrowserNode *) it.currentKey())->modified();
	}
        ProfiledStereotyped.remove(it.currentKey());
      }
      else
        ++it;
    }
  
    All.remove(pfix);

    for (unsigned index = 0; index != BrowserNodeSup; index += 1)
      Defaults[index].remove(pfix);

    delete pst;
    return TRUE;
  }

  return FALSE;
}

// the package is deleted or it is not a profile now
// warning : must be called before any change on the package
void ProfiledStereotypes::deleted(BrowserPackage * pk, bool propag)
{
  QString pfix = pk->get_name() + QString(":");
  
  BrowserNode * v;
  bool propag_usefull = FALSE;
  
  for (v = (BrowserNode *) pk->firstChild();
       v != 0;
       v = (BrowserNode *) v->nextSibling()) {
    if ((v->get_type() == UmlClassView) && !v->deletedp()) {
      BrowserNode * bn;
      
      for (bn = (BrowserNode *) v->firstChild();
	   bn != 0;
	   bn = (BrowserNode *) bn->nextSibling())
	if ((bn->get_type() == UmlClass) && 
	    !bn->deletedp() &&
	    !strcmp(bn->get_data()->get_stereotype(), "stereotype"))
	  propag_usefull |= ::deleted(pfix, (BrowserClass *) bn, propag);
    }  
  }

  if (propag && propag_usefull)
    recompute(FALSE);
}

// indicate if a stereotype can be added
const char * ProfiledStereotypes::canAddStereotype(BrowserClass * cl, QString name)
{
  BrowserNode * pf = (BrowserNode *) cl->parent()->parent();
  
  if ((pf->get_type() != UmlPackage) ||
      (strcmp(pf->get_data()->get_stereotype(), "profile") != 0))
    return "isn't in a profile";
  else {
    ProfiledStereotype * st = 
      All[pf->get_name() + QString(":") + name];
    
    return ((st != 0) && (st->cl != cl))
      ? "is already a stereotype"
      : (const char *) 0;
  }
}

// indicate if a stereotype can be added
const char * ProfiledStereotypes::canAddStereotype(BrowserClassView * v, QString name)
{
  BrowserNode * pf = (BrowserNode *) v->parent();
  
  if (strcmp(pf->get_data()->get_stereotype(), "profile") != 0)
    return "isn't in a profile";
  else {
    ProfiledStereotype * st = 
      All[pf->get_name() + QString(":") + name];
    
    return (st != 0)
      ? "is already a stereotype"
      : (const char *) 0;
  }
}

// new stereotype
void ProfiledStereotypes::added(BrowserClass * cl)
{
  BrowserNode * pf = (BrowserNode *) cl->parent()->parent();
  
  if ((pf->get_type() == UmlPackage) &&
      (strcmp(pf->get_data()->get_stereotype(), "profile") == 0)) {
    QString s = pf->get_name() + QString(":") + cl->get_name();
    
    if (All[s] != 0) {
      // only possible during a load / import
      msg_critical("Unconsistency",
		   "stereotype " + s + " defined several times,\n"
		   "redondant definition is not managed as a stereotype");
      cl->get_data()->set_stereotype("redondantProfiledStereotype");
    }
    else {
      ProfiledStereotype * st = new ProfiledStereotype;
      
      st->cl = cl;
      All.insert(s, st);
      setDefault(s, cl->get_value("stereotypeApplyOn"));
      st->clear_properties();
      st->set_properties();
    }
  }
}

// cl is and was a profiled stereotype, it was edited
void ProfiledStereotypes::changed(BrowserClass * cl, QString oldname)
{
  QString pkname = ((BrowserNode *) cl->parent()->parent())->get_name();
  
  if (oldname != cl->get_name())
    ::renamed(cl, oldname, pkname);
  else {
    QString pfix = pkname + QString(":");
    QString oldst = pfix + oldname;
    QString newst = pfix + cl->get_name();
    unsigned index;
    
    for (index = 0; index != BrowserNodeSup; index += 1)
      Defaults[index].remove(oldst);
    
    setDefault(newst, cl->get_value("stereotypeApplyOn"));
  }
}

// the class is deleted or it is not a stereotype now
// warning : must be called before any change on the class
void ProfiledStereotypes::deleted(BrowserClass * cl)
{
  if (::deleted(((BrowserNode *) cl->parent()->parent())->get_name() + QCString(":"),
    		cl, TRUE))
    recompute(FALSE);
}

// attribute added on stereotype, add property on elements
void ProfiledStereotypes::added(BrowserAttribute *)
{
  recompute(TRUE);
}

// attribute part of stereotype is modified
void ProfiledStereotypes::changed(BrowserAttribute * at, QString oldname)
{
  if (strcmp(oldname, at->get_name())) {
    QString sts = ((BrowserNode *) at->parent()->parent()->parent())->get_name() +
      QString(":") + ((BrowserNode *) at->parent())->get_name();
    ProfiledStereotype * st = All[sts];

    if ((st != 0) && (st->cl == (BrowserClass *) at->parent()))
      st->renamed(at);
  }
}

// the attribute part of stereotype is deleted
void ProfiledStereotypes::deleted(BrowserAttribute * at)
{
  QString sts = ((BrowserNode *) at->parent()->parent()->parent())->get_name() +
    QString(":") + ((BrowserNode *) at->parent())->get_name();
  ProfiledStereotype * st = All[sts];

  if ((st != 0) && (st->cl == (BrowserClass *) at->parent()))
    st->deleted(at);
}

// return the class suporting the given profiled stereotype
BrowserClass * ProfiledStereotypes::isModeled(QString s, bool case_sensitive)
{
  ProfiledStereotype * st = All[s];
  
  if (st != 0)
    return st->cl;
  else if (case_sensitive)
    return 0;
  
  QDictIterator<ProfiledStereotype> it(All);
  
  s = s.lower();
  
  while (it.current() != 0) {
    if (s == it.currentKey().lower())
      return it.current()->cl;
    ++it;
  }
  
  return 0;
}

void ProfiledStereotypes::post_load()
{
  recompute(FALSE);
}

void ProfiledStereotypes::recompute(BrowserNode * bn)
{
  QString s_st = bn->get_data()->get_stereotype();
  bool modified = FALSE;
  
  if (!s_st.isEmpty()) {
    ProfiledStereotype * st = All[s_st];
    
    if (st != 0) {
      ProfiledStereotyped.replace(bn, st);
      
      // search for extra properties
      unsigned nprop = bn->get_n_keys();
      unsigned findprop = 0;
      unsigned removed = 0;
      unsigned index = 0;
      
      while (index != nprop) {
	const char * k = bn->get_key(index);
	int nseps = 0;
	
	for (const char * p = k; *p; p += 1)
	  if (*p == ':')
	    nseps += 1;
	
	if (nseps == 2) {
	  if (st->propertiesFullName.findIndex(k) == -1) {
	    // extra property, remove it
	    bn->remove_key_value(index);
	    nprop -= 1;
	    removed += 1;
	    modified = TRUE;
	  }
	  else {
	    findprop += 1;
	    index += 1;
	  }
	}
	else
	  index += 1;
      }
      
      unsigned ndesired = st->properties.count();
      unsigned missing = ndesired - findprop;
      
      if (missing != 0) {
	// some properties are missing, add them
	bn->resize_n_keys(nprop + missing, removed < missing);
	
	unsigned insertindex = nprop;
	QStringList::Iterator its = 
	  st->propertiesFullName.begin();
	QValueList<BrowserAttribute *>::Iterator ita = 
	  st->properties.begin();
	
	for (;;) {
	  QString s = *its;
	  const char * k = s;
	  
	  for (index = 0; index != nprop; index += 1)
	    if (strcmp(k, bn->get_key(index)) == 0)
	      break;
	  
	  if (index == nprop) {
	    // missing
	    modified = TRUE;
	    bn->set_key(insertindex, k);
	    bn->set_value(insertindex,
			  ((AttributeData *) (*ita)->get_data())->get_init_value());
	    if (--missing == 0)
	      break;
	    insertindex += 1;
	  }
	  ++its;
	  ++ita;
	}
      }
    }
    else
      ProfiledStereotyped.remove(bn);
  }
  else {
    ProfiledStereotyped.remove(bn);
    
    // search for extra properties
    unsigned nprop = bn->get_n_keys();
    unsigned index = 0;
    
    while (index != nprop) {
      const char * k = bn->get_key(index);
      int nseps = 0;
      
      for (const char * p = k; *p; p += 1)
	if (*p == ':')
	  nseps += 1;
      
      if (nseps == 2) {
	// extra property, remove it
	bn->remove_key_value(index);
	modified = TRUE;
	nprop -= 1;
      }
      else
	index += 1;
    }
  }
  
  if (modified)
    bn->modified();
}

static void recompute_recur(BrowserNode * bn)
{
  ProfiledStereotypes::recompute(bn);
  
  for (bn = (BrowserNode *) bn->firstChild();
       bn != 0; 
       bn = (BrowserNode *) bn->nextSibling())
    if (! bn->deletedp())
      recompute_recur(bn);
}

static void recompute_st_list()
{
  All.setAutoDelete(TRUE);
  All.clear();
  All.setAutoDelete(FALSE);
  
  int index;
  
  for (index = 0; index != BrowserNodeSup; index += 1)
    Defaults[index].clear();
  
  BrowserNodeList l;
  
  BrowserClass::instances(l, "stereotype", TRUE);

  while (! l.isEmpty()) {
    BrowserClass * cl = (BrowserClass *) l.take(0);
    BrowserNode * pf = (BrowserNode *) cl->parent()->parent();
    
    if ((pf->get_type() == UmlPackage) &&
	(strcmp(pf->get_data()->get_stereotype(), "profile") == 0)) {
      QString s = pf->get_name() + QString(":") + cl->get_name();
      
      if (All[s] != 0) {
	msg_critical("Unconsistency",
		     "stereotype " + s + " defined several times,\n"
		     "redondant definition is not managed as a stereotype");
	cl->get_data()->set_stereotype("redondantProfiledStereotype");
      }
      else {
	ProfiledStereotype * st = new ProfiledStereotype;
	
	st->cl = cl;
	All.insert(s, st);
	setDefault(s, cl->get_value("stereotypeApplyOn"));
      }
    }
  }
}  

// if warm the stereotyped element list is not recomputed
void ProfiledStereotypes::recompute(bool warm) 
{
  // compute stereotype attributes
  QDictIterator<ProfiledStereotype> it(All);
  ProfiledStereotype * st;
  
  if (! warm)
    recompute_st_list();
  else {
    while ((st = it.current()) != 0) {
      st->clear_properties();
      ++it;
    }
  }
  
  it.toFirst();
  
  while ((st = it.current()) != 0) {
    st->set_properties();
    ++it;
  }
  
  if (warm) {
    QPtrDictIterator<ProfiledStereotype> it(ProfiledStereotyped);

    while (it.current()) {
      recompute((BrowserNode *) it.currentKey());
      ++it;
    }
  }
  else {
    // search for stereotyped elements
    ProfiledStereotyped.clear();
    recompute_recur(BrowserView::get_project());
  }
}

// 'bn' is modified, newst is true if bn's stereotype was changed
void ProfiledStereotypes::modified(BrowserNode * bn, bool newst)
{
  QString sts = bn->get_data()->get_stereotype();
  ProfiledStereotype * st = All[sts];
  
  if (st != 0) {
    if (newst) {
      ProfiledStereotyped.replace(bn, st);	// add it if needed
      recompute(bn);
    }
    
    const char * s =
      st->cl->get_value((newst) ? "stereotypeSet" : "stereotypeCheck");
    
    if ((s != 0) && ((s = Tool::command(s)) != 0))
      ToolCom::run(s, bn, FALSE);
  }
  else if (newst)
    ProfiledStereotyped.remove(bn);

  // call check on parent
  if ((bn = (BrowserNode *) bn->parent()) != 0)
    callCheck(bn, FALSE);
}

// for plug-out, apply the stereotype
void ProfiledStereotypes::applyStereotype(BrowserNode * bn)
{
  QString sts = bn->get_data()->get_stereotype();
  ProfiledStereotype * st = All[sts];
  
  if (st != 0) {
    ProfiledStereotyped.replace(bn, st);	// add it if needed
    recompute(bn);
  }
  else
    ProfiledStereotyped.remove(bn);
}

// continue to apply check
static void callCheckCont()
{
  do {
    CheckCell * c = CheckStack.pop();
    BrowserNode * bn;
    
    for (bn = (c->next) ? (BrowserNode *) c->bn->nextSibling()
			: (BrowserNode *) c->bn->firstChild();
	 bn != 0;
	 bn = (BrowserNode *) bn->nextSibling()) {
      if (!bn->deletedp()) {
	if (bn->nextSibling() != 0)
	  CheckStack.push(new CheckCell(bn, TRUE));
	ProfiledStereotypes::callCheck(bn, TRUE);
	delete c;
	return;
      }
    }
    delete c;
  } while (! CheckStack.isEmpty());
}

// call the check plug-out if exist, return true if plug-out applied
void ProfiledStereotypes::callCheck(BrowserNode * bn, bool rec)
{
  QString sts = bn->get_data()->get_stereotype();
  ProfiledStereotype * st = All[sts];
  bool applied = FALSE;
  
  if (st != 0) {
    const char * s = st->cl->get_value("stereotypeCheck");
    
    if ((s != 0) && ((s = Tool::command(s)) != 0)) {
      ToolCom::run(s, bn, FALSE, (rec) ? &callCheckCont : 0);
      applied = TRUE;
    }
  }
  
  if (rec) {
    CheckStack.push(new CheckCell(bn, FALSE));
    if (! applied)
      callCheckCont();
  }
}

// return true if the element or a sub element has a stereotype having
// a check plug-out
static bool haveCheck(BrowserNode * bn)
{
  QString sts = bn->get_data()->get_stereotype();
  ProfiledStereotype * st = All[sts];
  
  if (st != 0) {
    const char * s = st->cl->get_value("stereotypeCheck");
    
    if ((s != 0) && (Tool::command(s) != 0))
      return TRUE;
  }
  
  for (bn = (BrowserNode *) bn->firstChild();
       bn != 0;
       bn = (BrowserNode *) bn->nextSibling())
    if (!bn->deletedp() && haveCheck(bn))
      return TRUE;
  
  return FALSE;
}

// add menu entries if relevant
void ProfiledStereotypes::menu(QPopupMenu & m, BrowserNode * bn, int bias)
{
  QString sts = bn->get_data()->get_stereotype();
  ProfiledStereotype * st = All[sts];
  bool separatorinserted = FALSE;
  
  if (st != 0) {
    const char * s = st->cl->get_value("stereotypeCheck");
    
    if ((s != 0) && (Tool::command(s) != 0)) {
      separatorinserted = TRUE;
      m.insertSeparator();
      m.setWhatsThis(m.insertItem("Check", bias),
		   "to call the <em>check plug-out</em> associated to the stereotype");
    }
  }

  bool have = !All.isEmpty();
  
  if (! have) {
    BrowserNodeList l;
    
    BrowserClass::instances(l, "stereotype", TRUE);
    have = !l.isEmpty();
  }
  
  if (have) {
    if (! separatorinserted)
      m.insertSeparator();

    m.setWhatsThis(m.insertItem("Force stereotype consistency", bias+2),
		   "to recompute all concerning the profiles");
    if (!ProfiledStereotyped.isEmpty() && haveCheck(bn))
      m.setWhatsThis(m.insertItem("Check recursively", bias+1),
		     "to call the <em>check plug-out</em> associated to the stereotypes,"
		     "doing down recursively");
  }
}

// do action depending on menu choice
void ProfiledStereotypes::choiceManagement(BrowserNode * bn, int choice)
{
  switch (choice) {
  case 0:
    callCheck(bn, FALSE);
    break;
  case 1:
    {
      callCheck(bn, TRUE);

    }
    break;
  case 2:
    recompute(FALSE);
    break;
  default:
    break;
  }
}

// k contains 2 ':', if i is a property part of a stereotype and
// its type is an enum, return TRUE and set 'l' to the enum's items

bool ProfiledStereotypes::enumerated(QString k, QStringList & l)
{
  QString st = k.left(k.findRev(':'));
  ProfiledStereotype * pst = All[st];

  return (pst == 0) ? FALSE : pst->enumerated(k, l);
}
