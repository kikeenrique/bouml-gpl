
#include "UmlItem.h"
#include "FileIn.h"
#include "Token.h"

#include "UmlCom.h"
#include "UmlClass.h"
#include "UmlNcRelation.h"

void Unresolved::addGeneralization(UmlItem * e, QCString & id)
{
  Generalizations.append(Unresolved(e, id));
}

void Unresolved::addRef(UmlItem * e, QCString & id)
{
  Refs.append(Unresolved(e, id));
}

QValueList<Unresolved> Unresolved::Generalizations;

QValueList<Unresolved> Unresolved::Refs;

void Unresolved::solveThem()
{
  QValueList<Unresolved>::Iterator it;
      
  for (it = Refs.begin(); it != Refs.end(); ++it)
    (*it).element->solve((*it).idref);
  Refs.clear();
  
  for (it = Generalizations.begin(); it != Generalizations.end(); ++it)
    (*it).element->solveGeneralizationDependencyRealization(0, (*it).idref, "");
  Generalizations.clear();
}

void UnresolvedWithContext::add(UmlItem * e, QCString id, int c)
{
  All.append(UnresolvedWithContext(e, id, c));
}

void UnresolvedWithContext::solveThem()
{
  QValueList<UnresolvedWithContext>::Iterator it;
      
  for (it = All.begin(); it != All.end(); ++it)
    (*it).element->solve((*it).context, (*it).idref);
    
  All.clear();
}

QValueList<UnresolvedWithContext> UnresolvedWithContext::All;

void UnresolvedRelation::add(int ctx, QCString idFrom, QCString idTo, QCString label)
{
  All.append(UnresolvedRelation(ctx, idFrom, idTo, label));
}

UnresolvedRelation::UnresolvedRelation() {
}

QValueList<UnresolvedRelation> UnresolvedRelation::All;

void UnresolvedRelation::solveThem()
{
  QValueList<UnresolvedRelation>::Iterator it;
      
  for (it = All.begin(); it != All.end(); ++it) {
    QMap<QCString, UmlItem *>::Iterator from = UmlItem::All.find((*it).from);
    
    if (from != UmlItem::All.end())
      (*from)->solveGeneralizationDependencyRealization((*it).context, (*it).to, (*it).name);
    else
      UmlCom::trace("relation : unknown source reference '" + (*it).from + "'<br>");
  }
  All.clear();
}

 UmlItem::~UmlItem() {
}

void UmlItem::import() {
  UmlCom::trace("Error : must be applied on a package<br>");
}

void UmlItem::addItem(QCString id, FileIn & in) {
  QMap<QCString, UmlItem *>::ConstIterator iter = All.find(id);
  
  if (iter != All.end())
    in.error("xmi:id '" + id + "' used twice");
    
  All.insert(id, this);
  _xmi_id = id;
}

void UmlItem::import(FileIn & in, Token & tk) {
  QCString key = tk.what() + " " + tk.xmiType();
    
  PFunc pf = Functions[key];
  
  if (pf != 0)
    pf(in, tk, this);
  else {
    if (tk.xmiType().isEmpty())
      in.warning("bypass &lt;" + tk.what() + "...&gt;");
    else
      in.warning("bypass &lt;" + tk.what() + 
		 " xmi:type=\"" + tk.xmiType() + "\"...&gt;");
    
    if (! tk.closed())
      in.finish(tk.what());
  }
}

UmlItem * UmlItem::container(anItemKind kind, const Token & token, FileIn & in) {
  return parent()->container(kind, token, in);
}

void UmlItem::solve(QCString) {
}

void UmlItem::solve(int, QCString) {
}

void UmlItem::generalizeDependRealize(UmlItem * target, FileIn & in, int context, QCString label) {
  static const struct {
    aRelationKind rk;
    const char * err;
  } r[] = {
    { aGeneralisation, "cannot create generalization from '" },
    { aDependency, "cannot create dependency from '" },
    { aRealization, "cannot create realization from '" }
  };
  UmlItem * rel = UmlNcRelation::create(r[context].rk, this, target);
  
  if (rel == 0)
    in.warning(r[context].err + name() + "' to '" + target->name() + "'");
  else if (!label.isEmpty())
    rel->set_Name(label);
}

void UmlItem::solveGeneralizationDependencyRealization(int context, QCString idref, QCString label) {
  QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
  
  if (it == All.end())
    UmlCom::trace("relation : unknown target reference '" + idref + "'<br>");
  else {
    static const struct {
      aRelationKind rk;
      const char * err;
    } r[] = {
      { aGeneralisation, "cannot create generalization from '" },
      { aDependency, "cannot create dependency from '" },
      { aRealization, "cannot create realization from '" }
    };
    UmlItem * target = *it;
    UmlItem * rel = UmlNcRelation::create(r[context].rk, this, target);
    
    if (rel == 0)
      UmlCom::trace(r[context].err + name() + "' to '" + target->name() + "'");
    else if (! label.isEmpty())
      rel->set_Name(label);
  }
}

bool UmlItem::setType(QCString idref, UmlTypeSpec & type) {
  if (idref.isEmpty())
    return FALSE;
  else if (getType(idref, type))
    return TRUE;
  else {
    Unresolved::addRef(this, idref);
    return FALSE;
  }
}

bool UmlItem::setType(QCString idref, int context, UmlTypeSpec & type) {
  if (idref.isEmpty())
    return FALSE;
  else if (getType(idref, type))
    return TRUE;
  else {
    UnresolvedWithContext::add(this, idref, context);
    return FALSE;
  }
}

bool UmlItem::setType(Token & token, UmlTypeSpec & type) {
  QCString idref = token.xmiIdref();
  
  if (idref.isEmpty())
    return UmlClass::isPrimitiveType(token, type);
  else if (getType(idref, type))
    return TRUE;
  else {
    Unresolved::addRef(this, idref);
    return FALSE;
  }
}

bool UmlItem::setType(Token & token, int context, UmlTypeSpec & type) {
  QCString idref = token.xmiIdref();
  
  if (idref.isEmpty())
    return UmlClass::isPrimitiveType(token, type);
  else if (getType(idref, type))
    return TRUE;
  else {
    UnresolvedWithContext::add(this, idref, context);
    return FALSE;
  }
}

bool UmlItem::getType(QCString idref, UmlTypeSpec & type)
{
  QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
  
  type.type = 0;
  type.explicit_type = "";
  
  if (it != All.end()) {
    if ((*it)->kind() == aClass) {
      type.type = (UmlClass *) *it;
      return TRUE;
    }
    else
      return FALSE;
  }
  else {
    QMap<QCString, UmlTypeSpec>::Iterator itp = PrimitiveTypes.find(idref);
    
    if (itp != PrimitiveTypes.end()) {
      type = *itp;
      return TRUE;
    }
    else
      return FALSE;
  }
}

void UmlItem::declareFct(QCString what, QCString type, PFunc fct)
{
  Functions[what + " " + type] = fct;
}

void UmlItem::init()
{
  declareFct("xmi:documentation", "", &importDocumentation);

  declareFct("ownedcomment", "uml:Comment", &importComment);
  
  declareFct("xmi:extension", "", &importExtension);

  declareFct("incoming", "", &incoming);
  declareFct("outgoing", "", &outgoing);
  
  declareFct("ownedbehavior", "uml:OpaqueBehavior", &importOpaqueDef);
  declareFct("ownedmember", "uml:OpaqueExpression", &importOpaqueDef);
  declareFct("packagedelement", "uml:OpaqueExpression", &importOpaqueDef);

  declareFct("generalization", "uml:Generalization", &importGeneralization);
  
  declareFct("ownedelement", "uml:Dependency", &importDependency);
  declareFct("ownedmember", "uml:Dependency", &importDependency);
  declareFct("packagedelement", "uml:Dependency", &importDependency);
  
  declareFct("ownedelement", "uml:Realization", &importRealization);
  declareFct("ownedmember", "uml:Realization", &importRealization);
  declareFct("packagedelement", "uml:Realization", &importRealization);
  declareFct("interfacerealization", "uml:InterfaceRealization", &importRealization);
}

void UmlItem::importDocumentation(FileIn & in, Token & token, UmlItem *)
{
  QCString who = token.valueOf("exporter");

  if (who.isNull())
    who = token.valueOf("xmi:exporter");
    
    if (! who.isNull()) {
      UmlCom::trace("xmi file produced by <b>" + who + "</b><br><br>");
      FromBouml = (who == "Bouml");
    }

  if (! token.closed())
    in.finish(token.what());
}

void UmlItem::importComment(FileIn & in, Token & token, UmlItem * where)
{
  QCString doc = token.valueOf("body");

  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      if (token.what() == "body") {
	if (! doc.isEmpty())
	  doc += "\n";
	doc += in.body("body");
      }
      else if (! token.closed())
	in.finish(token.what());
    }
  }

  if (! doc.isEmpty())
    where->set_Description(doc);
}

void UmlItem::importExtension(FileIn & in, Token & token, UmlItem * where)
{
  if (! token.closed()) {
    QCString s = token.valueOf("extender");
    
    if (s.isNull())
      s = token.valueOf("xmi:extender");
    
    if (s == "Bouml") {
      QCString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr)) {
	s = token.what();
	
	if (s == "taggedvalue")
	  // is closed
	  where->set_PropertyValue(token.valueOf("tag"), token.valueOf("value"));
	else if (s == "stereotype")
	  where->set_Stereotype(token.valueOf("name"));

	if (! token.closed())
	  in.finish(token.what());
      }
    }
    else if (s == "Visual Paradigm for UML") {
      QCString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr)) {
	s = token.what();
	
	if (s == "appliedstereotype") {
	  s = token.valueOf("xmi:value");
	  
	  if (s.right(3) == "_id") {
	    s = s.mid(s.find("_") + 1);
	    s = s.left(s.length() - 3).lower();
	  }
	  where->set_Stereotype(s);
	}

	if (! token.closed())
	  in.finish(token.what());
      }
    }
    else
      in.finish(token.what());
  }
}

void UmlItem::importOpaqueDef(FileIn & in, Token & token, UmlItem *)
{
  QCString id = token.xmiId();
  QMap<QCString, QCString>::ConstIterator iter = OpaqueDefs.find(id);
  
  if (iter != OpaqueDefs.end())
    in.error("xmi:id '" + id + "' used twice");
    
  QCString s = token.valueOf("body");
  
  if (!s.isNull()) {
    OpaqueDefs.insert(id, s);
    
    if (! token.closed())
      in.finish(token.what());
  }
  else if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      s = token.what();
      
      if (s == "body")
	OpaqueDefs.insert(id, in.body("body"));
      else if (! token.closed())
	in.finish(s);
    }
  }
}

void UmlItem::importGeneralization(FileIn & in, Token & token, UmlItem * where)
{
  QCString id = token.valueOf("general");
  
  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      QCString s = token.what();
      
      if (s == "general")
	id = token.xmiIdref();
      
      if (! token.closed())
	in.finish(s);
    }
  }

  if (id.isEmpty())
    in.warning("'general' is missing");
  else {
    QMap<QCString, UmlItem *>::ConstIterator iter = All.find(id);
  
    if (iter != All.end())
      where->generalizeDependRealize(*iter, in, 0, "");
    else
      Unresolved::addGeneralization(where, id);
  }
}

void UmlItem::importDependency(FileIn & in, Token & token, UmlItem * where)
{
  QCString client = token.valueOf("client");
  QCString supplier = token.valueOf("supplier");
  QCString label = token.valueOf("name");
  
  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      QCString s = token.what();
      
      if (s == "client")
	client = token.xmiIdref();
      else if (s == "supplier")
	supplier = token.xmiIdref();
      
      if (! token.closed())
	in.finish(s);
    }
  }

  if (client.isEmpty())
    in.warning("'client' is missing");
  else {
    if (supplier.isEmpty())
      // Borland Together 2006 for Eclipse
      supplier = where->id();

    QMap<QCString, UmlItem *>::ConstIterator from = All.find(client);
    QMap<QCString, UmlItem *>::ConstIterator to = All.find(supplier);
  
    if ((from != All.end()) && (to != All.end()))
      (*from)->generalizeDependRealize(*to, in, 1, label);
    else
      UnresolvedRelation::add(1, client, supplier, label);
  }
}

void UmlItem::importRealization(FileIn & in, Token & token, UmlItem * where)
{
  QCString client = token.valueOf("client");
  QCString supplier = token.valueOf("supplier");
  QCString label = token.valueOf("name");
  
  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      QCString s = token.what();
      
      if (s == "client")
	client = token.xmiIdref();
      else if (s == "supplier")
	supplier = token.xmiIdref();
      
      if (! token.closed())
	in.finish(s);
    }
  }

  if (client.isEmpty())
    in.warning("'client' is missing");
  else {
    if (supplier.isEmpty())
      // Borland Together 2006 for Eclipse
      supplier = where->id();

    QMap<QCString, UmlItem *>::ConstIterator from = All.find(client);
    QMap<QCString, UmlItem *>::ConstIterator to = All.find(supplier);
  
    if ((from != All.end()) && (to != All.end()))
      (*from)->generalizeDependRealize(*to, in, 2, label);
    else
      UnresolvedRelation::add(2, client, supplier, label);
  }
}

void UmlItem::incoming(FileIn & in, Token & token, UmlItem * where)
{
  Incomings.insert(token.xmiIdref(), where);
  
  if (! token.closed())
    in.finish(token.what());
}

void UmlItem::outgoing(FileIn & in, Token & token, UmlItem * where)
{
  while (where->kind() == aRegion)
    // up to the state
    where = where->parent();

  Outgoings.insert(token.xmiIdref(), where);
  
  if (! token.closed())
    in.finish(token.what());
}

QCString UmlItem::legalName(QCString s)
{
  int index = 0;
  int index2;
  
  while ((index2 = s.find(' ', index)) != -1) {
    s.replace(index2, 1, "_");
    index = index2 + 1;
  }

  return s;
}

QMap<QCString, QCString> UmlItem::OpaqueDefs;

bool UmlItem::FromBouml;

QMap<QCString, UmlItem *> UmlItem::All;

QMap<QCString, PFunc> UmlItem::Functions;

QMap<QCString, UmlTypeSpec> UmlItem::PrimitiveTypes;

QMap<QCString,UmlItem*> UmlItem::Incomings;

QMap<QCString, UmlItem*> UmlItem::Outgoings;

