
#include "UmlDiagram.h"

#include "UmlCom.h"
void UmlDiagram::html(QCString, unsigned int, unsigned int) {
  UmlCom::message(name());

  define();

  QCString s;
  
  s.setNum(++fig_rank);
  s = "fig" + s + ((svg) ? ".svg" : ".png");
  saveIn(directory + s);
  
  if (svg) {
    fw.write("<p align=\"center\"><embed src=\"");
    fw.write(s);
    fw.write('"');
    
    s = directory + s;
    
    FILE * fp = fopen((const char *) s, "r");
    
    if (fp != 0) {
      char line[256];
      
      fgets(line, sizeof(line) - 1, fp);	// <?xml version="1.0" ...
      fgets(line, sizeof(line) - 1, fp);	// <!-- Created with Bouml ...
      fgets(line, sizeof(line) - 1, fp);	// <!DOCTYPE svg PUBLIC ...
      fgets(line, sizeof(line) - 1, fp);	// <svg width="495" height="560" version=...
      fclose(fp);
      
      char * p = strstr(line, " version=");
      
      if (p != 0) {
	*p = 0;
	fw.write(line+4);
      }
    }
    
    fw.write("/></p>\n");
  }
  else {
    fw.write("<p align=\"center\"><img src=\"");
    fw.write(s);
    fw.write("\" alt=\"\" /></p>\n");
  }
  
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

