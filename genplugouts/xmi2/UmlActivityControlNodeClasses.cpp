
#include "UmlActivityControlNodeClasses.h"

const char * UmlInitialActivityNode::sKind() const {
  return "InitialNode";
}

const char * UmlFlowFinalActivityNode::sKind() const {
  return "FlowFinalNode";
}

const char * UmlActivityFinalActivityNode::sKind() const {
  return "FinalNode";
}

const char * UmlDecisionActivityNode::sKind() const {
  return "DecisionNode";
}

const char * UmlMergeActivityNode::sKind() const {
  return "MergeNode";
}

const char * UmlForkActivityNode::sKind() const {
  return "ForkNode";
}

const char * UmlJoinActivityNode::sKind() const {
  return "JoinNode";
}

