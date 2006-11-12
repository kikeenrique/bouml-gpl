
#include "UmlExtraClassMember.h"

QCString UmlExtraClassMember::sKind() {
  return "extra class member";
}

void UmlExtraClassMember::html(QCString, unsigned int, unsigned int) {
  define();

  fw.write("<table><tr><td><div class=\"element\">Extra Class Member <b>");
  writeq(name());
  fw.write("</b></div></td></tr></table>");

  fw.write("<p>Definition :</p><ul>");
  
  QCString s = cppDecl();

  if (!s.isEmpty()) {
    fw.write("<li>C++ : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = javaDecl();

  if (!s.isEmpty()) {
    fw.write("<li>Java : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  fw.write("</ul>");
 
  s = description();
  
   if (!s.isEmpty()) {
     fw.write("<p>");
     writeq(description());
     fw.write("<br /></p>");
   }

  unload(FALSE, FALSE);
}

