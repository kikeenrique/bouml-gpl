
#include "UmlActivityPin.h"
#include "UmlParameterSet.h"

QCString UmlParameterSet::sKind() {
  return "parameter set";
}

void UmlParameterSet::html(QCString, unsigned int, unsigned int) {
  fw.write("<table><tr><td><div class=\"element\">Parameter set  <b>");
  writeq(name());
  fw.write("</div></td></tr></table>");
  
  fw.write("<p>Contains :");
  
  const QVector<UmlActivityPin> & p = pins();
  unsigned n = p.size();
  
  for (unsigned i = 0; i != n; i += 1) {
    fw.write(" ");
    p[i]->write();
  }
  
  fw.write("</p>");

  QCString s = description();

  if (!s.isEmpty()) {
    fw.write("<p>");
    writeq(s);
    fw.write("<br /></p>");
  }

  unload(FALSE, FALSE);

}

