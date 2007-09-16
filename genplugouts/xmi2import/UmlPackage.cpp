
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
      
      UmlCom::trace("<br><font face=helvetica>solve references<br>");
      
      UnresolvedWithContext::solveThem();
      Unresolved::solveThem();
      UnresolvedRelation::solveThem();
      Association::solveThem();	// must be done after UnresolvedRelation::solveThem();
      UmlFlow::solveThem();
      UmlTransition::solveThem();
      Binding::solveThem(); // must be done when all classes and realization or generalization are done
      ClassInstance::solveThem(); // must be done when all classes are done
      
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
		    QCString().setNum(UmlNode::numberOf()) + QCString(" deployment nodes </font><br>"));
      
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

    while ((void) in.read(), !tk.close("xmi:xmi"))
      UmlCom::targetItem()->import(in, tk);
  }
  else if (tk.what() == "uml:model") {
    // Borland Together 2006 for Eclipse
    // Visual Paradigm for UML 6.1
    QCString ver = tk.valueOf("xmi:version");

    if (ver.isEmpty())
      UmlCom::trace("warning : unknown xmi version<br><br>");
    else if (ver != "2.1")
      UmlCom::trace("warning : imported file is not xmi 2.1<br><br>");

    UmlCom::targetItem()->import(in, tk);
  }
  else
    in.error("uml:model or xmi:xmi expected, nothing imported");
}

UmlItem * UmlPackage::container(anItemKind kind, const Token & token, FileIn & in) {
  switch (kind) {
  case aClass:
  case aClassInstance:
  case aState:
  case anActivity:
    if (_classview == 0)
      _classview = UmlClassView::create(this, "Class View");
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
    in.warning("bypass &lt;" + token.what() + 
	       " xmi:type=\"" + token.xmiType() + "\"...&gt;");
    
    if (! token.closed())
      in.finish(token.what());
    
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
  
  pack->addItem(token.xmiId(), in);

  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr))
      pack->UmlItem::import(in, token);
  }

  pack->unload(TRUE, FALSE);
}

void UmlPackage::init()
{
  declareFct("uml:model", "uml:Model", &importIt);
  declareFct("packagedelement", "uml:Package", &importIt);
  declareFct("ownedmember", "uml:Package", &importIt);
  
  declareFct("uml:package", "", &importIt);
  
  declareFct("uml:model", "", &importIt);

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

int UmlPackage::NumberOf;

