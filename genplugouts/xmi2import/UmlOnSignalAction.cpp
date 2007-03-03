
#include "UmlOnSignalAction.h"
#include "FileIn.h"
#include "Token.h"

#include "UmlCom.h"
#include "Sgnl.h"
void UmlOnSignalAction::import(FileIn & in, Token & tk) {
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

void UmlOnSignalAction::solve(QCString idref) {
  QCString sig = Signal::get(idref);
  
  if (sig.isNull())
    UmlCom::trace("activity action : unknown signal reference '" + idref + "'<br>");
  else
    set_Signal(sig);
}

void UmlOnSignalAction::import_it(FileIn & in, Token & token) {
  QCString s = token.valueOf("signal");
  
  if (!s.isEmpty()) {
    QCString sig = Signal::get(s);
    
    if (!sig.isNull())
      set_Signal(sig);
    else
      Unresolved::addRef(this, s);
  }

  if (! token.closed()) {
    QCString k = token.what();
    const char * ks = k;
    
    while (in.read(), !token.close(ks)) {
      if (token.what() == "signal") {
	QCString sig_name;
	QCString sig_ref;
	
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

