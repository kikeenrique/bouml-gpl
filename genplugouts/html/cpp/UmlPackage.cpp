
#include "UmlPackage.h"

#include "UmlClassDiagram.h"

QCString UmlPackage::sKind() {
  return "package";
}

void UmlPackage::html(QCString pfix, unsigned int rank, unsigned int level) {
  define();
  
  chapter("Package", pfix, rank, "package", level);

  QCString s = cppNamespace();
  bool ul = FALSE;
  
  if (!s.isEmpty()) {
    fw.write("<p></p><ul>\n");
    ul = TRUE;
    fw.write("<li>C++ namespace : ");
    writeq(s);
    fw.write("</li>\n");
  }

  s = javaPackage();
  
  if (!s.isEmpty()) {
    if (! ul)
      fw.write("<p></p><ul>");
    ul = TRUE;
    fw.write("<li>Java package : ");
    writeq(s);
    fw.write("</li>\n");
  }
    
  if (ul)
    fw.write("</ul>\n");
    
  write_dependencies();
    
  s = description();
  
  if (!s.isEmpty()) {
    fw.write("<p>");
    writeq(s);
    fw.write("<br /></p>");
  }
  
  UmlDiagram * d = associatedDiagram();
  
  if (d != 0) {
    fw.write("<p>Diagram : ");
    d->write();
    fw.write("</p>\n");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

void UmlPackage::memo_ref() {
  packages.addElement(this);
  UmlItem::memo_ref();
}

void UmlPackage::ref_index()
{
  if (!packages.isEmpty())
    fw.write("<a href=\"packages.html\" target = \"projectFrame\"><b> -Packages- </b></a>");
}

void UmlPackage::generate_index()
{
  UmlItem::generate_index(packages, "Packages", "packages");
}

bool UmlPackage::chapterp() {
  return TRUE;
}

Vector UmlPackage::packages;

