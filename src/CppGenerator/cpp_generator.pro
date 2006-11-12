TEMPLATE      = app
CONFIG	      += qt warn_on release
SOURCES	      = UmlClassItem.cpp CppRefType.cpp UmlAttribute.cpp \
		UmlClass.cpp UmlClassDiagram.cpp UmlClassMember.cpp \
		UmlExtraClassMember.cpp UmlArtifact.cpp \
		UmlClassView.cpp UmlCollaborationDiagram.cpp UmlComponent.cpp \
		UmlComponentDiagram.cpp UmlComponentView.cpp \
		UmlDeploymentDiagram.cpp UmlDeploymentView.cpp UmlDiagram.cpp \
		UmlItem.cpp UmlNode.cpp UmlOperation.cpp UmlPackage.cpp \
		UmlRelation.cpp UmlNcRelation.cpp UmlSequenceDiagram.cpp UmlUseCase.cpp \
		UmlUseCaseDiagram.cpp UmlUseCaseView.cpp main.cpp util.cpp \
		UmlChoicePseudoState.cpp UmlDeepHistoryPseudoState.cpp \
		UmlEntryPointPseudoState.cpp UmlExitPointPseudoState.cpp \
		UmlFinalState.cpp UmlForkPseudoState.cpp \
		UmlInitialPseudoState.cpp UmlJoinPseudoState.cpp \
		UmlJunctionPseudoState.cpp UmlShallowHistoryPseudoState.cpp \
		UmlStateAction.cpp UmlState.cpp \
		UmlStateDiagram.cpp UmlTerminatePseudoState.cpp \
		UmlRegion.cpp \
		../Tools/UmlBaseClassItem.cpp ../Tools/CppSettings.cpp \
		../Tools/IdlSettings.cpp ../Tools/JavaSettings.cpp \
		../Tools/UmlTypeSpec.cpp ../Tools/UmlBaseArtifact.cpp \
		../Tools/UmlBaseAttribute.cpp ../Tools/UmlBaseClass.cpp \
		../Tools/UmlBaseClassDiagram.cpp ../Tools/UmlBaseClassMember.cpp \
		../Tools/UmlBaseExtraClassMember.cpp ../Tools/UmlBaseClassView.cpp \
		../Tools/UmlBaseCollaborationDiagram.cpp ../Tools/UmlBaseComponent.cpp \
		../Tools/UmlBaseComponentDiagram.cpp ../Tools/UmlBaseComponentView.cpp \
		../Tools/UmlBaseDeploymentDiagram.cpp ../Tools/UmlBaseDeploymentView.cpp \
		../Tools/UmlBaseDiagram.cpp ../Tools/UmlBaseItem.cpp \
		../Tools/UmlBaseNode.cpp ../Tools/UmlBaseOperation.cpp \
		../Tools/UmlBasePackage.cpp ../Tools/UmlBaseRelation.cpp \
		../Tools/UmlBaseNcRelation.cpp \
		../Tools/UmlBaseSequenceDiagram.cpp ../Tools/UmlBaseUseCase.cpp \
		../Tools/UmlBaseUseCaseDiagram.cpp ../Tools/UmlBaseUseCaseView.cpp \
		../Tools/UmlCom.cpp ../Tools/UmlSettings.cpp \
		../Tools/UmlBaseChoicePseudoState.cpp ../Tools/UmlBaseDeepHistoryPseudoState.cpp \
		../Tools/UmlBaseEntryPointPseudoState.cpp ../Tools/UmlBaseExitPointPseudoState.cpp \
		../Tools/UmlBaseFinalState.cpp ../Tools/UmlBaseForkPseudoState.cpp \
		../Tools/UmlBaseInitialPseudoState.cpp ../Tools/UmlBaseJoinPseudoState.cpp \
		../Tools/UmlBaseJunctionPseudoState.cpp ../Tools/UmlBaseShallowHistoryPseudoState.cpp \
		../Tools/UmlBaseStateAction.cpp ../Tools/UmlBaseState.cpp \
		../Tools/UmlBaseStateDiagram.cpp ../Tools/UmlBaseTerminatePseudoState.cpp \
		../Tools/UmlBaseRegion.cpp ../Tools/StateBehavior.cpp ../Tools/UmlBaseTransition.cpp \
		../Tools/TransitionBehavior.cpp 
TARGET	      = cpp_generator
DEFINES	      = WITHCPP
INCLUDEPATH   = ../Tools ../CppGenerator


