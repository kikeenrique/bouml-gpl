
#include "UmlClass.h"
#include "UmlItem.h"
#include "Token.h"
#include "FileIn.h"

#include "UmlAttribute.h"
#include "UmlRelation.h"
#include "UmlOperation.h"
#include "UmlNcRelation.h"
#include "UmlCom.h"
UmlItem * UmlClass::container(anItemKind kind, const Token & token, FileIn & in) {
  switch (kind) {
  case aClass:
  case anAttribute:
  case aRelation:
  case anOperation:
    return this;
  default:
    return parent()->container(kind, token, in );
  }

}

void UmlClass::init()
{
  declareFct("ownedmember", "uml:Class", &importIt);
  declareFct("packagedelement", "uml:Class", &importIt);
  declareFct("nestedclassifier", "uml:Class", &importIt);
  
  declareFct("ownedmember", "uml:Interface", &importIt);
  declareFct("packagedelement", "uml:Interface", &importIt);
  declareFct("nestedclassifier", "uml:Interface", &importIt);
  
  declareFct("ownedmember", "uml:Enumeration", &importIt);
  declareFct("packagedelement", "uml:Enumeration", &importIt);
  declareFct("nestedclassifier", "uml:Enumeration", &importIt);
  
  declareFct("ownedmember", "uml:Actor", &importIt);
  declareFct("packagedelement", "uml:Actor", &importIt);
  declareFct("nestedclassifier", "uml:Actor", &importIt);

  //
  
  declareFct("ownedmember", "uml:PrimitiveType", &importPrimitiveType);
  declareFct("packagedelement", "uml:PrimitiveType", &importPrimitiveType);
  declareFct("ownedmember", "uml:DataType", &importPrimitiveType);
  declareFct("packagedelement", "uml:DataType", &importPrimitiveType);
  
  //
  
  UmlAttribute::init();
  UmlRelation::init();
  UmlOperation::init();
}

void UmlClass::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aClass, token, in);	// can't be null
    
  QCString s = token.valueOf("name");
  
  if (s.isEmpty()) {
    static unsigned n = 0;
    
    s.sprintf("anonymous_%u", ++n);
  }
    
  UmlClass * cl = create(where, s);
  
  if (cl == 0)
    in.error("cannot create classe '" + s +
	     "' in '" + where->name() + "'");
  
  cl->addItem(token.xmiId(), in);
  
  if (token.xmiType() == "uml:Actor")
    cl->set_Stereotype("actor");
  else if (token.xmiType() == "uml:Interface")
    cl->set_Stereotype("interface");
  else if (token.xmiType() == "uml:Enumeration")
    cl->set_Stereotype("enum");
    
  cl->setVisibility(token.valueOf("visibility"));
  
  if (token.valueOf("isabstract") == "true")
    cl->set_isAbstract(TRUE);

  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      s = token.what();
      
      if ((s == "ownedtemplatesignature") &&
	  (token.xmiType() == "uml:TemplateSignature"))
	cl->readFormal(in, token);
      else if (FromBouml &&
	       (s == "templatebinding") &&
	       (token.xmiType() == "uml:TemplateBinding")) {
	// don't know how to generate then read this form
	if (! token.closed())
	  in.finish(token.what());
      }
      else
	cl->UmlItem::import(in, token);
    }
  }

  cl->unload(TRUE, FALSE);
}

void UmlClass::readFormal(FileIn & in, Token & token) {
  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    unsigned int rank = 0;
    
    while (in.read(), !token.close(kstr)) {
      QCString s = token.what();
      
      if (s == "parameter") {
	// useless
	if (! token.closed())
	  in.finish(token.what());
      }
      else if ((s == "ownedparameter") &&
	       (token.xmiType() == "uml:ClassifierTemplateParameter")) {
	QCString pname = token.valueOf("name");	// at least for VP
	QCString value;
	
	if (! token.closed()) {
	  while (in.read(), !token.close("ownedparameter")) {
	    s = token.what();
	    
	    if ((s == "ownedparameteredelement") ||
		(s == "ownedelement")) {
	      s = token.valueOf("name");
	      if (! s.isEmpty())
		pname = s;
	    }
	    else if (s == "defaultvalue")
	      value = token.valueOf("value");
	    
	    if (! token.closed())
	      in.finish(token.what());
	  }
	}
	
	if (! pname.isEmpty()) {
	  UmlFormalParameter f(pname, value);
	  
	  addFormal(rank++, f);
	}
      }
      else if (! token.closed())
	in.finish(token.what());
    }
  }
}

void UmlClass::importPrimitiveType(FileIn & in, Token & token, UmlItem *)
{
  QCString id = token.xmiId();
  UmlTypeSpec t;
  
  t.explicit_type = token.valueOf("name");

  if (FromBouml) {
    if (! token.closed()) {
      bool dummy;
      
      in.read(); 	// <xmi:Extension extender="Bouml">
      in.readWord(FALSE, dummy);	// <
      in.readWord(FALSE, dummy);	// basedOn
      t.type = dynamic_cast<UmlClass *>(All[in.readWord(FALSE, dummy)]);
      if (t.type != 0)
        // forget modifiers
        t.explicit_type = "";
      in.readWord(FALSE, dummy);	// /
      in.readWord(FALSE, dummy);	// >
      in.read(); 	// </xmi:Extension>
      in.read();	// end of token
    }
  }
  else if (! token.closed())
    in.finish(token.what());

  if (!id.isEmpty())
    PrimitiveTypes[id] = t;
}

void UmlClass::generalizeDependRealize(UmlItem * target, FileIn & in, int context, QCString label) {
  static const struct {
    aRelationKind rk;
    const char * err;
  } r[] = {
    { aGeneralisation, "cannot create generalization from '" },
    { aDependency, "cannot create dependency from '" },
    { aRealization, "cannot create realization from '" }
  };
  UmlItem * rel;
  
  if (target->kind() == aClass)
    rel = UmlRelation::create(r[context].rk, this, (UmlClass *) target);
  else
    rel = UmlNcRelation::create(r[context].rk, this, target);
    
  if (rel == 0)
    in.warning(r[context].err + name() + "' to '" + target->name() + "'");
  else if (! label.isEmpty())
    rel->set_Name(label);
}

void UmlClass::solveGeneralizationDependencyRealization(int context, QCString idref, QCString label) {
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
    UmlItem * rel;
       
    if (target->kind() == aClass)
      rel = UmlRelation::create(r[context].rk, this, (UmlClass *) target);
    else
      rel = UmlNcRelation::create(r[context].rk, this, target);
    
    if (rel == 0)
      UmlCom::trace(r[context].err + name() + "' to '" + target->name() + "'");
    else if (!label.isEmpty())
      rel->set_Name(label);
  }
}

int UmlClass::NumberOf;

