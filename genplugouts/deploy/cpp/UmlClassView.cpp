
#include "UmlClassView.h"

#include "UmlDeploymentView.h"
#include "UmlCom.h"
void UmlClassView::deploy() {
  UmlDeploymentView * depl = associatedDeploymentView();
  
  if (depl == 0) {
    // create the associated deployment view
    UmlCom::trace("Create <i>deployment view</i> " + name());
    
    if ((depl = UmlDeploymentView::create((UmlPackage*) parent(), name())) == 0) {
      UmlCom::trace("cannot create <i>deployment view</i> named " + name());
      return;
    }
    else
      set_AssociatedDeploymentView(depl);
  }
  
  // look at non deployed classes
  
  const QVector<UmlItem> ch = children();
    
  for (unsigned i = 0; i != ch.size(); i += 1)
    ch[i]->deploy(depl);
  
}

