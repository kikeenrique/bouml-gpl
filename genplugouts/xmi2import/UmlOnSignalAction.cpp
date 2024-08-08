
#include "UmlOnSignalAction.h"
#include "FileIn.h"
#include "Token.h"

#include "UmlCom.h"
#include "Sgnl.h"
void UmlOnSignalAction::solve(QByteArray idref) {
  QByteArray sig = Signal::get(idref);
  
  if (!sig.isNull())
    set_Signal(sig);
  else if (!FileIn::isBypassedId(idref))
    UmlCom::trace("activity action : unknown signal reference '" + idref + "'<br>");

}

void UmlOnSignalAction::import_it(FileIn & in, Token & token) {
  QByteArray s = token.valueOf("signal");
  
  if (!s.isEmpty()) {
    QByteArray sig = Signal::get(s);
    
    if (!sig.isNull())
      set_Signal(sig);
    else
      Unresolved::addRef(this, s);
  }

  if (! token.closed()) {
    QByteArray k = token.what();
    const char * ks = k;
    
    while (in.read(), !token.close(ks)) {
      if (token.what() == "signal") {
	QByteArray sig_name;
	QByteArray sig_ref;
	
	Signal::add(in, token, sig_name, sig_ref);
	
	if (!sig_name.isNull())
	  set_Signal(sig_name);
	else
	  Unresolved::addRef(this, sig_ref);
      }
      else
	import(in, token);
    }
  }
}

