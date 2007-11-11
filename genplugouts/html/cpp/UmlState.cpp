
#include "UmlStateDiagram.h"
#include "UmlState.h"

QCString UmlState::sKind() {
  return (parent()->kind() == aClassView)
    ? "state machine" : "state";
}

void UmlState::html(QCString pfix, unsigned int rank, unsigned int level) {
  define();

  chapter((parent()->kind() == aClassView)
	  ? "StateMachine" : "State",
	  pfix, rank, "state", level);

  QCString s = description();
  
  if (!s.isEmpty()) {
    fw.write("<p>");
    writeq(description());
    fw.write("<br /></p>");
  }

  fw.write("<p>Entry Behavior :</p><ul>");
  
  s = entryBehavior();

  if (!s.isEmpty()) {
    fw.write("<li>OCL : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = cppEntryBehavior();

  if (!s.isEmpty()) {
    fw.write("<li>C++ : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = javaEntryBehavior();

  if (!s.isEmpty()) {
    fw.write("<li>Java : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  fw.write("</ul>");

  fw.write("<p>Exit Behavior :</p><ul>");
  
  s = exitBehavior();

  if (!s.isEmpty()) {
    fw.write("<li>OCL : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = cppExitBehavior();

  if (!s.isEmpty()) {
    fw.write("<li>C++ : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = javaExitBehavior();

  if (!s.isEmpty()) {
    fw.write("<li>Java : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  fw.write("</ul>");

  fw.write("<p>Do activity :</p><ul>");
  
  s = doActivity();

  if (!s.isEmpty()) {
    fw.write("<li>OCL : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = cppDoActivity();

  if (!s.isEmpty()) {
    fw.write("<li>C++ : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  s = javaDoActivity();

  if (!s.isEmpty()) {
    fw.write("<li>Java : <pre>\n");
    writeq(s);
    fw.write("</pre></li>");
  }

  fw.write("</ul>");
 
  UmlStateDiagram * d = associatedDiagram();
  
  if (d != 0) {
    fw.write("<p>Diagram : ");
    d->write();
    fw.write("</p>");
  }

  write_properties();

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

void UmlState::memo_ref() {
  states.addElement(this);
  UmlItem::memo_ref();
}

void UmlState::ref_index()
{
  if (!states.isEmpty())
    fw.write("<a href=\"states.html\" target = \"projectFrame\"><b> -States- </b></a>");
}

void UmlState::generate_index()
{
  UmlItem::generate_index(states, "States", "states");
}

bool UmlState::chapterp() {
  return TRUE;
}

Vector UmlState::states;

