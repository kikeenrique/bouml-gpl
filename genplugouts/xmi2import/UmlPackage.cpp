
#include "UmlPackage.h"
#include "UmlItem.h"
#include "UmlClassView.h"
#include "UmlComponentView.h"
#include "UmlDeploymentView.h"
#include "UmlUseCaseView.h"

#include <stdio.h>
#include <qfiledialog.h> 

#include "UmlCom.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlUseCase.h"
#include "UmlClass.h"
#include "UmlState.h"
#include "UmlActivity.h"
#include "UmlComponent.h"
#include "UmlArtifact.h"
#include "UmlNode.h"
#include "UmlFlow.h"
#include "UmlTransition.h"
#include "Binding.h"
#include "ClassInstance.h"
#include "Manifestation.h"

#include "PackageGlobalCmd.h"

void UmlPackage::import() {
  QString path = QFileDialog::getOpenFileName(QString::null, QString::null, 0, 0, "open xmi/xml file");
  
  if (! path.isEmpty()) {
    // note : QTextStream(FILE *) bugged under windows
    FILE * fp = fopen(path, "rb");
    
    if (fp == 0)
      UmlCom::trace("cannot open " + path);
    else {
      FileIn in(path, fp);
      
      getProject()->set_childrenVisible(FALSE);
      
      init();
      importHeader(in);
      
      UmlCom::trace(QCString("<br><font face=helvetica>xmi import done<br><br>") +
		    QCString("<font face=helvetica> ") +
		    QCString().setNum(numberOf() - 1) + QCString(" packages </font><br>") +
		    QCString("<font face=helvetica> ") +
		    QCString().setNum(UmlUseCase::numberOf()) + QCString(" use cases </font><br>") +
		    QCString("<font face=helvetica> ") +
		    QCString().setNum(UmlClass::numberOf()) + QCString(" classes </font><br>") +
		    QCString("<font face=helvetica> ") +
		    QCString().setNum(UmlActivity::numberOf()) + QCString(" activities </font><br>") +
		    QCString("<font face=helvetica> ") +
		    QCString().setNum(UmlState::numberOf()) + " states </font><br>" +
		    QCString("<font face=helvetica> ") +
		    QCString().setNum(UmlComponent::numberOf()) + QCString(" components </font><br>") +
		    QCString("<font face=helvetica> ") +
		    QCString().setNum(UmlArtifact::numberOf()) + QCString(" artifacts </font><br>") +
		    QCString("<font face=helvetica> ") +
		    QCString().setNum(UmlNode::numberOf()) + QCString(" deployment nodes </font><br>") +
		    QCString("<font face=helvetica> ") +
		    QCString().setNum(UmlPackage::numberOfProfile()) + QCString(" profiles </font><br>") +
		    QCString("<font face=helvetica> ") +
		    QCString().setNum(UmlClass::numberOfStereotype()) + QCString(" stereotypes </font><br>"));
      
      getProject()->set_childrenVisible(TRUE);
    }
  }
}

void UmlPackage::importHeader(FileIn & in) {
  Token & tk = in.read();
  
  if (tk.what() != "xml")
    in.error("xml version expected");
    
  if (tk.valueOf("version") != "1.0") {
    UmlCom::trace("Only import xml 1.0");
    return;
  }

  in.setEncoding(tk.valueOf("encoding"));

  (void) in.read();	// update tk
  
  if (tk.what() == "xmi:xmi") {
    QCString ver = tk.valueOf("xmi:version");

    if (ver.isEmpty())
      UmlCom::trace("warning : unknown xmi version<br><br>");
    else if (ver != "2.1")
      UmlCom::trace("warning : imported file is not xmi 2.1<br><br>");

    FromEclipse = 
      // for instance RSA Eclipse xmi export
      !tk.valueOf("xmlns:ecore").isEmpty();
    if (FromEclipse)
      UmlCom::trace("xmi file produced under <b>Eclipse</b><br><br>");

    // read all before stereotype use
    QCString prof_st;
    QCString base_v;
    
    while ((void) in.read(), !tk.close("xmi:xmi")) {
      if (UmlClass::isAppliedStereotype(tk, prof_st, base_v))
	break;
      
      UmlCom::targetItem()->import(in, tk);
    }
    
    solveRefs();
    
    // read stereotype use
    while (!tk.close("xmi:xmi")) {
      applyStereotype(in, tk);
      (void) in.read();
    }
    
    if (NumberOfProfile != 0)
#ifndef WIN32
#warning must be replaced by updateProfiles()
#endif
      UmlCom::send_cmd(packageGlobalCmd, saveProjectCmd + 2);
  }
  else if ((tk.what() == "uml:model") || (tk.what() == "uml:profile")) {
    // Borland Together 2006 for Eclipse
    // Visual Paradigm for UML 6.1
    // RSA Eclipse (profile)
    QCString ver = tk.valueOf("xmi:version");

    if (ver.isEmpty())
      UmlCom::trace("warning : unknown xmi version<br><br>");
    else if (ver != "2.1")
      UmlCom::trace("warning : imported file is not xmi 2.1<br><br>");

    FromEclipse = 
      // .uml
      !tk.valueOf("xmlns:ecore").isEmpty();
    if (FromEclipse)
      UmlCom::trace("xmi file produced under <b>Eclipse</b><br><br>");
    
    UmlCom::targetItem()->import(in, tk);
    
    solveRefs();
    
    if (NumberOfProfile != 0)
#ifndef WIN32
#warning must be replaced by updateProfiles()
#endif
      UmlCom::send_cmd(packageGlobalCmd, saveProjectCmd + 2);
  }
  else
    in.error("uml:model or xmi:xmi expected, nothing imported");
}

UmlItem * UmlPackage::container(anItemKind kind, Token & token, FileIn & in) {
  switch (kind) {
  case aClass:
  case aClassInstance:
  case aState:
  case anActivity:
    if (_classview == 0)
      _classview = UmlClassView::create(this,
					(stereotype() == "profile")
					? "Stereotype View" : "Class View");
    return _classview;
  case anUseCase:
    if (_usecaseview == 0)
      _usecaseview = UmlUseCaseView::create(this, "Use Case View");
    return _usecaseview;
  case aComponent:
    if (_compview == 0)
      _compview = UmlComponentView::create(this, "Component View");
    return _compview;
  case aNode:
  case anArtifact:
    if (_deplview == 0)
      _deplview = UmlDeploymentView::create(this, "Deployment View");
    return _deplview;
  default:
    in.bypass(token);
    return 0;
  }
}

void UmlPackage::importIt(FileIn & in, Token & token, UmlItem * where)
{
  while (where->kind() != aPackage)
    where = where->parent();
    
  QCString s = token.valueOf("name");
  
  if (s.isEmpty()) {
    static unsigned n = 0;
    
    s.sprintf("anonymous %u", ++n);
  }
    
  UmlPackage * pack = create((UmlPackage *) where, s);
  
  if (pack == 0)
    in.error("cannot create package '" + s + "' in '" + where->name() + "'");
  
  bool profile = 
    (token.what() == "uml:profile") || (token.xmiType() == "uml:Profile");
    
  if (profile) {
    pack->set_Stereotype("profile");
    NumberOf -= 1;
    NumberOfProfile += 1;
    
    if (!(s = token.valueOf("metamodelreference")).isEmpty())
      pack->set_PropertyValue("metamodelReference", s);
    
    if (!(s = token.valueOf("metaclassreference")).isEmpty())
      pack->set_PropertyValue("metaclassReference", s);
  }
    
  pack->addItem(token.xmiId(), in);

  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    if (profile) {
      while (in.read(), !token.close(kstr)) {
	if ((token.what() == "packagedelement") &&
	    (token.xmiType() == "uml:Extension")) {
	  if (! token.closed())
	    in.finish(token.what());
	}
	else if (token.what() == "packageimport")
	  pack->packageImport(in, token);
	else
	  pack->UmlItem::import(in, token);
      }
#ifndef WIN32
#warning must be replaced by updateProfiles()
#endif
      UmlCom::send_cmd(packageGlobalCmd, saveProjectCmd + 2);
    }
    else
      while (in.read(), !token.close(kstr))
	pack->UmlItem::import(in, token);
  }

  pack->unload(TRUE, FALSE);
}

void UmlPackage::appliedProfile(FileIn & in, Token & token, UmlItem *)
{
  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      if (token.what() == "appliedprofile") {
	QCString s = token.valueOf("href");
	
	if (! s.isEmpty()) {
	  int index = s.find('#');
	  
#ifndef WIN32
#warning lire((index == -1) ? s : s.left(index));
#endif
	}
      }
      
      if (! token.closed())
	in.finish(token.what());
    }
  }

}

void UmlPackage::init()
{
  declareFct("uml:model", "uml:Model", &importIt);
  declareFct("packagedelement", "uml:Package", &importIt);
  declareFct("ownedmember", "uml:Package", &importIt);
  declareFct("packagedelement", "uml:Profile", &importIt);
  declareFct("ownedmember", "uml:Profile", &importIt);
  
  declareFct("uml:package", "", &importIt);
  
  declareFct("uml:model", "", &importIt);
  
  declareFct("uml:profile", "", &importIt);
  declareFct("profileapplication", "", &appliedProfile);
  declareFct("profileapplication", "uml:ProfileApplication", &appliedProfile);

  //
  
  UmlItem::init();
  UmlUseCase::init();
  UmlClass::init();
  UmlState::init();
  UmlActivity::init();
  UmlComponent::init();
  UmlNode::init();
  UmlArtifact::init();
}

void UmlPackage::packageImport(FileIn & in, Token & tk) {
  if (! tk.closed()) {
    QCString id = tk.xmiId();
    QCString k = tk.what();
    const char * kstr = k;
    
    while (in.read(), !tk.close(kstr)) {
      if (tk.what() == "importedpackage") {
	if (tk.xmiType() == "uml:Model") {
	  QCString v;
	  
	  if (propertyValue("metamodelReference", v) && (v == id)) {
	    QCString href = tk.valueOf("href");
	    int index = href.find('#');
	    
	    set_PropertyValue("metamodelReference",
			      (index == -1)
			      ? href : href.left(index));
	  }
	}
	else if (tk.xmiType() == "uml:Profile") {
	  in.warning("Profile import not yet implemented");
#ifndef WIN32
#warning import profile to be done
#endif
	}
      }
      else if (tk.what() == "importedelement") {
	QCString v;
	  
	if (propertyValue("metaclassReference", v) && (v == id)) {
	  QCString href = tk.valueOf("href");
	  int index = href.find('#');
	    
	  set_PropertyValue("metaclassReference",
			    (index == -1)
			    ? href : href.left(index));
	}
      }
      
      if (! tk.closed())
	in.finish(tk.what());
    }
  }
}

void UmlPackage::solveRefs() {
  UmlCom::trace("<br><font face=helvetica>solve references<br>");
      
  UnresolvedWithContext::solveThem();
  Unresolved::solveThem();
  UnresolvedRelation::solveThem();
  Association::solveThem();	// must be done after UnresolvedRelation::solveThem();
  UmlFlow::solveThem();
  UmlTransition::solveThem();
  Binding::solveThem(); // must be done when all classes and realization or generalization are done
  ClassInstance::solveThem(); // must be done when all classes are done
  Manifestation::solveThem(); // must be done when all artifact, class, component and node exist

}

void UmlPackage::applyStereotype(FileIn & in, Token & token) {
  QCString prof_st;
  QCString base_v;
  QCString s;
  
  if (UmlClass::isAppliedStereotype(token, prof_st, base_v)) {
    if (!token.valueOf(base_v, s))
      in.warning("value of '" + base_v + "' is missing<br>");
    else {
      UmlItem * elt = All[s];
      
      if (elt == 0) {
	if (!FileIn::isBypassedId(s))
	  in.warning("unknown reference '" + s + "'<br>");
      }
      else {
	elt->set_Stereotype(prof_st);
	elt->UmlItem::applyStereotype();	// set properties
	
	QDict<QCString> props = elt->properties();
	QDictIterator<QCString> it(props);
	
	while (it.current()) {
	  QCString k = it.currentKey().latin1();
	  
	  if (token.valueOf(k.mid(k.findRev(':') + 1).lower(), s))
	    elt->set_PropertyValue(k, s);
	  
	  ++it;
	}
      }
    }
    
    if (! token.closed())
      in.finish(token.what());
  }
  else
    in.bypass(token);

}

int UmlPackage::NumberOf;

int UmlPackage::NumberOfProfile;

