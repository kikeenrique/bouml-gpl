
#include "UmlActivityControlNodeClasses.h"

QByteArray UmlInitialActivityNode::sKind() {
  return "initial activity node";
}

QByteArray UmlFlowFinalActivityNode::sKind() {
  return "flow final";
}

QByteArray UmlActivityFinalActivityNode::sKind() {
  return "activity final";
}

QByteArray UmlDecisionActivityNode::sKind() {
  return "decision activity node";
}

QByteArray UmlMergeActivityNode::sKind() {
  return "merge activity node";
}

QByteArray UmlForkActivityNode::sKind() {
  return "fork activity node";
}

QByteArray UmlJoinActivityNode::sKind() {
  return "join activity node";
}

