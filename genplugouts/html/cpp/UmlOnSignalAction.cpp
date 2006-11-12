
#include "UmlOnSignalAction.h"

void UmlOnSignalAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  fw.write("<p>Signal :</p><ul>");
  
  QCString s = signal();

  if (!s.isEmpty()) {
    fw.write("<li>OCL : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = cppSignal();

  if (!s.isEmpty()) {
    fw.write("<li>C++ : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = javaSignal();

  if (!s.isEmpty()) {
    fw.write("<li>Java : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  fw.write("</ul>");

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

