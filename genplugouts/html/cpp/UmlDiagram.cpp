
#include "UmlDiagram.h"

#include "UmlCom.h"
void UmlDiagram::html(QCString, unsigned int, unsigned int) {
  UmlCom::message(name());

  define();

  QCString s;
  
  s.setNum(++fig_rank);
  s = "fig" + s + ".png"; 
  saveIn(directory + s);
  
  fw.write("<p align=\"center\"><img src=\"");
  fw.write(s);
  fw.write("\" alt=\"\" /></p>\n");
  
  fw.write(" <p align=\"center\"><b>");
  writeq(name());
  fw.write("</b></p><p><br /></p><p><br /></p>\n");

  s = description();
  
  if (!s.isEmpty()) {
    fw.write("<p>");
    writeq(description());
    fw.write("<br /></p>");
  }

  unload(FALSE, FALSE);
}

int UmlDiagram::fig_rank;

