
#include "UmlClass.h"
#include "UmlItem.h"
#include "Token.h"
#include "FileIn.h"
#include "UmlTypeSpec.h"

#include "UmlAttribute.h"
#include "UmlRelation.h"
#include "UmlOperation.h"
#include "UmlNcRelation.h"
#include "Association.h"
#include "UmlCom.h"
#include "Binding.h"
#include "ClassInstance.h"
#include "CppSettings.h"

#include "PackageGlobalCmd.h"

UmlItem * UmlClass::container(anItemKind kind, Token & token, FileIn & in) {
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
  
  declareFct("ownedmember", "uml:AssociationClass", &importIt);
  declareFct("packagedelement", "uml:AssociationClass", &importIt);
  declareFct("nestedclassifier", "uml:AssociationClass", &importIt);
  
  declareFct("ownedmember", "uml:Interface", &importIt);
  declareFct("packagedelement", "uml:Interface", &importIt);
  declareFct("nestedclassifier", "uml:Interface", &importIt);
  
  declareFct("ownedmember", "uml:Enumeration", &importIt);
  declareFct("packagedelement", "uml:Enumeration", &importIt);
  declareFct("nestedclassifier", "uml:Enumeration", &importIt);
  
  declareFct("ownedmember", "uml:Actor", &importIt);
  declareFct("packagedelement", "uml:Actor", &importIt);
  declareFct("nestedclassifier", "uml:Actor", &importIt);
  
  declareFct("ownedmember", "uml:Stereotype", &importIt);
  declareFct("packagedelement", "uml:Stereotype", &importIt);

  //
  
  declareFct("ownedmember", "uml:PrimitiveType", &importPrimitiveType);
  declareFct("packagedelement", "uml:PrimitiveType", &importPrimitiveType);
  declareFct("ownedmember", "uml:DataType", &importPrimitiveType);
  declareFct("packagedelement", "uml:DataType", &importPrimitiveType);
  
  //
  
  UmlAttribute::init();
  UmlRelation::init();
  UmlOperation::init();
  ClassInstance::init();
}

void UmlClass::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aClass, token, in);	// can't be null
    
  QCString s = token.valueOf("name");
  
  if (s.isEmpty()) {
    static unsigned n = 0;
    
    s.sprintf("anonymous_%u", ++n);
  }
  else
    s = legalName(s);
    
  UmlClass * cl = create(where, s);
  Association * assocclass = 0;
  
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
  else if (token.xmiType() == "uml:Stereotype") {
    cl->set_Stereotype("stereotype");
    NumberOf -= 1;
    NumberOfStereotype += 1;
  }
  else if (token.xmiType() == "uml:AssociationClass") {
    assocclass = &Association::get(token.xmiId(), token.valueOf("name"));
    assocclass->set_class_association();
  }
    
  cl->setVisibility(token.valueOf("visibility"));
  
  if (token.valueOf("isabstract") == "true")
    cl->set_isAbstract(TRUE);

  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    QCString assocclass_ref1;
    QCString assocclass_ref2;
  
    while (in.read(), !token.close(kstr)) {
      s = token.what();
      
      if ((s == "ownedtemplatesignature") &&
	  ((token.xmiType() == "uml:TemplateSignature") ||
	   (token.xmiType() == "uml:RedefinableTemplateSignature")))
	cl->readFormal(in, token);
      else if ((s == "templatebinding") &&
	       (token.xmiType() == "uml:TemplateBinding")) {
	Binding::import(in, token, cl);
      }
      else if ((assocclass != 0) && (s == "memberend")) {
	if (assocclass_ref1.isEmpty())
	  assocclass_ref1 = token.xmiIdref();
	else
	  assocclass_ref2 = token.xmiIdref();
	if (! token.closed())
	  in.finish(s);
      }
      else if ((assocclass != 0) && 
	       (s == "ownedend") && 
	       (token.xmiType() == "uml:Property"))
	assocclass->import(in, token);
      else
	cl->UmlItem::import(in, token);
    }
  }
  
  cl->unload(TRUE, FALSE);
}

void UmlClass::readFormal(FileIn & in, Token & token) {
  if (! token.closed()) {
    signatures[token.xmiId()] = this;
    
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
	QCString idparam = token.xmiId();
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
	  formalsId.append(idparam);
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
    { aRealization, "cannot create realization from '" },
    { aDependency, "cannot create usage from '" }
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
  
  if (it != All.end()) {
    static const struct {
      aRelationKind rk;
      const char * err;
    } r[] = {
      { aGeneralisation, "cannot create generalization from '" },
      { aDependency, "cannot create dependency from '" },
      { aRealization, "cannot create realization from '" },
      { aDependency, "cannot create usage from '" }
    };
    UmlItem * target = *it;
    UmlItem * rel;
       
    if (target->kind() == aClass)
      rel = UmlRelation::create(r[context].rk, this, (UmlClass *) target);
    else
      rel = UmlNcRelation::create(r[context].rk, this, target);
    
    if (rel == 0)
      UmlCom::trace(r[context].err + name() + "' to '" + target->name() + "'<br>");
    else {
      if (!label.isEmpty())
	rel->set_Name(label);
      if (context == 3)
	rel->set_Stereotype("use");
    }
  }
  else if (!FileIn::isBypassedId(idref))
    UmlCom::trace("relation : unknown target reference '" + idref + "'<br>");
}

UmlClass * UmlClass::signature(QCString id)
{
  QMap<QCString, UmlClass *>::Iterator iter = signatures.find(id);
  
  return (iter == signatures.end()) ? 0 : *iter;
}

int UmlClass::formalRank(QCString id) {
  int r = formalsId.findIndex(id);
  
  if ((r == -1) && !FileIn::isBypassedId(r))
    UmlCom::trace("unknown template formal reference '" + id + "'<br>");

  return r;
}

bool UmlClass::bind(UmlClass * tmpl) {
  const QVector<UmlItem> ch = children();
  unsigned int n = ch.size();
  int i;
  
  for (i = 0; i != (int) n; i += 1) {
    if ((ch[i]->kind() == aRelation) && 
	(((UmlRelation *) ch[i])->roleType() == tmpl)) {
      switch (((UmlRelation *) ch[i])->relationKind()) {
      case aRealization:
	((UmlRelation *) ch[i])->set_Stereotype("bind");
	// no break
      case aGeneralisation:
	return TRUE;
      default:
	break;
      }
    }
  }

  // add realization
  UmlRelation * r =
    UmlRelation::create(aRealization, this, tmpl);
    
  if (r == 0) {
    UmlCom::trace("class reference '" + id() +
		  "' can't realize class reference '" + tmpl->id() + "'<br>");

    return FALSE;
  }

  r->set_Stereotype("bind");
  return TRUE;
}

bool UmlClass::isAppliedStereotype(Token & tk, QCString & prof_st, QCString & base_v)
{
  static QDict<QCString> stereotypes;
  static QDict<QCString> bases;
  
  QString s = tk.what();
  QCString * st = stereotypes[s];
  
  if (st != 0) {
    prof_st = *st;
    base_v = *bases[s];
    return TRUE;
  }
      
  if (tk.xmiType().isEmpty() && (getFct(tk) == 0))  {
    int index = s.find(':');
    
    if ((index != -1) &&
	((index != 3) || ((s.left(3) != "uml") && (s.left(3) != "xmi")))) {
#ifndef WIN32
#warning must use UmlPackage::findStereotype(s, FALSE)
#endif
      UmlCom::send_cmd(packageGlobalCmd, saveProjectCmd + 3, (void *) 0, (const char *) s);
      UmlClass * cl = (UmlClass *) UmlBaseItem::read_();
      
      if (cl != 0) {
	QCString ext;
	
	cl->propertyValue("stereotypeExtension", ext);
	
	if (ext.isEmpty())
	  ext = "base_element";
	else
	  ext = "base_" + ext.mid(ext.findRev('#') + 1).lower();
	
	base_v = ext;
	prof_st = cl->parent()->parent()->name() + ":" + cl->name();
	stereotypes.insert(s, new QCString(prof_st));
	bases.insert(s, new QCString(base_v));
	return TRUE;
      }
    }
  }

  return FALSE;
}

bool UmlClass::isPrimitiveType(Token & token, UmlTypeSpec & ts)
{
  if (token.xmiType() != "uml:PrimitiveType")
    return FALSE;
    
  QCString href = token.valueOf("href");
  int index;
  
  if (href.isEmpty() || ((index = href.find('#')) == -1))
    return FALSE;
    
  ts.explicit_type = href.mid(index + 1);
  
  if ((CppSettings::type(ts.explicit_type) == ts.explicit_type) &&
      CppSettings::umlType(ts.explicit_type).isEmpty()) {
    // not defined
    href = ts.explicit_type.lower();
    
    if (href == "integer")
      ts.explicit_type = "int";
    else if (href == "boolean")
      ts.explicit_type = "bool";
    else if (href == "string")
      ts.explicit_type = "string";
    else if (href == "unlimitednatural")
      ts.explicit_type = "long";
  }

  return TRUE;
}

int UmlClass::NumberOf;

int UmlClass::NumberOfStereotype;

//associate the class owning the template signature with the signature id
QMap<QCString, UmlClass *> UmlClass::signatures;

