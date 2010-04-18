
#include "UmlActivityDiagram.h"
#include "UmlOperation.h"
#include "UmlActivity.h"

QCString UmlActivity::sKind() {
  return "activity";
}

void UmlActivity::html(QCString pfix, unsigned int rank, unsigned int level) {
  define();

  chapter("Activity", pfix, rank, "activity", level);
  
  QCString s = description();
  
  if (!s.isEmpty()) {
    fw.write("<p>");
    writeq(description());
    fw.write("<br /></p>");
  }
  
  if (isActive())
    fw.write("<p>Active activity</p>\n");

  UmlOperation * beh = specification();
  
  if (beh != 0) {
    fw.write("<p>Implements ");
    beh->write();
    fw.write("</p>");
  }

  if (isReadOnly()) {
    if (isSingleExecution())
      fw.write("<p>Read only, single execution</p>");
    else
      fw.write("<p>Read only</p>");
  }
  else if (isSingleExecution())
    fw.write("<p>Single execution</p>");

  fw.write("<p>Pre Condition :</p><ul>");
  
  s = preCondition();

  if (!s.isEmpty()) {
    fw.write("<li>OCL : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = cppPreCondition();

  if (!s.isEmpty()) {
    fw.write("<li>C++ : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = javaPreCondition();

  if (!s.isEmpty()) {
    fw.write("<li>Java : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  fw.write("</ul>");

  fw.write("<p>Post Condition :</p><ul>");
  
  s = postCondition();

  if (!s.isEmpty()) {
    fw.write("<li>OCL : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = cppPostCondition();

  if (!s.isEmpty()) {
    fw.write("<li>C++ : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = javaPostCondition();

  if (!s.isEmpty()) {
    fw.write("<li>Java : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  fw.write("</ul>");

  if (! constraint().isEmpty()) {
    fw.write("<p> Constraint :<ul>");
    writeq(constraint());
    fw.write("</ul>");
  }
 
  UmlActivityDiagram * d = associatedDiagram();
  
  if (d != 0) {
    fw.write("<p>Diagram : ");
    d->write();
    fw.write("</p>");
  }
  
  write_properties();

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

void UmlActivity::memo_ref() {
  activities.addElement(this);
  UmlItem::memo_ref();
}

void UmlActivity::ref_index()
{
  if (!activities.isEmpty())
    fw.write("<a href=\"activities.html\" target = \"projectFrame\"><b> -Activities- </b></a>");
}

void UmlActivity::generate_index()
{
  UmlItem::generate_index(activities, "Activities", "activities");
}

bool UmlActivity::chapterp() {
  return TRUE;
}

Vector UmlActivity::activities;

