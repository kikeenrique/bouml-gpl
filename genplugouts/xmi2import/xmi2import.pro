TEMPLATE	= app
TARGET		= ixmi2
CONFIG		+= release warn_on qt
DEFINES		= WITHCPP WITHJAVA WITHIDL
HEADERS		= ./UmlExtraClassMember.h \
		  ./UmlTypeSpec.h \
		  ./UmlClassMember.h \
		  ./ClassGlobalCmd.h \
		  ./PackageGlobalCmd.h \
		  ./UmlBaseFlow.h \
		  ./UmlBaseStateAction.h \
		  ./aVisibility.h \
		  ./UmlBaseExitPointPseudoState.h \
		  ./UmlBaseObjectDiagram.h \
		  ./Sgnl.h \
		  ./UmlDeploymentView.h \
		  ./UmlActivityRegion.h \
		  ./UmlStateAction.h \
		  ./UmlStateItem.h \
		  ./IdlSettingsCmd.h \
		  ./UmlState.h \
		  ./UmlBaseComponent.h \
		  ./UmlBaseInitialPseudoState.h \
		  ./UmlBaseParameterSet.h \
		  ./IdlSettings.h \
		  ./UmlPinParameter.h \
		  ./UmlActivityControlNode.h \
		  ./UmlActivityObject.h \
		  ./UmlSettingsCmd.h \
		  ./UmlDeploymentDiagram.h \
		  ./UmlBaseJoinPseudoState.h \
		  ./UmlBaseFinalState.h \
		  ./UmlBaseSequenceDiagram.h \
		  ./UmlBaseRelation.h \
		  ./UmlBaseFormalParameter.h \
		  ./UmlTerminatePseudoState.h \
		  ./MiscGlobalCmd.h \
		  ./UmlClass.h \
		  ./UmlBaseComponentView.h \
		  ./UmlActivityAction.h \
		  ./UmlBasePinParameter.h \
		  ./UmlBaseState.h \
		  ./UmlBaseOperation.h \
		  ./CppSettings.h \
		  ./FileIn.h \
		  ./UmlBaseTransition.h \
		  ./UmlBaseClassMember.h \
		  ./UmlBaseExpansionNode.h \
		  ./UmlBaseActivityParameter.h \
		  ./UmlAccessVariableValueAction.h \
		  ./UmlPackage.h \
		  ./UmlSequenceDiagram.h \
		  ./UmlBaseNcRelation.h \
		  ./UmlBaseEntryPointPseudoState.h \
		  ./UmlBaseUseCaseView.h \
		  ./UmlBaseForkPseudoState.h \
		  ./UmlBaseUseCase.h \
		  ./UmlStereotype.h \
		  ./UmlBaseActivity.h \
		  ./UmlBaseArtifact.h \
		  ./UmlFormalParameter.h \
		  ./UmlExitPointPseudoState.h \
		  ./UmlBaseClassItem.h \
		  ./CppSettingsCmd.h \
		  ./UmlStateDiagram.h \
		  ./UmlBaseDeploymentDiagram.h \
		  ./aParameterEffectKind.h \
		  ./UmlBaseActivityObject.h \
		  ./UmlBaseActivityControlNode.h \
		  ./UmlBaseActivityActionClasses.h \
		  ./UmlBaseAccessVariableValueAction.h \
		  ./UmlBaseItem.h \
		  ./UmlSettings.h \
		  ./UmlDiagram.h \
		  ./OnInstanceCmd.h \
		  ./UmlInterruptibleActivityRegion.h \
		  ./UmlBaseActivityPin.h \
		  ./UmlBaseDeepHistoryPseudoState.h \
		  ./UmlBaseChoicePseudoState.h \
		  ./UmlBaseTerminatePseudoState.h \
		  ./UmlBaseClass.h \
		  ./UmlBaseAttribute.h \
		  ./Token.h \
		  ./UmlDeepHistoryPseudoState.h \
		  ./StateBehavior.h \
		  ./CmdFamily.h \
		  ./UmlActivityActionClasses.h \
		  ./UmlEntryPointPseudoState.h \
		  ./aDirection.h \
		  ./UmlBaseRegion.h \
		  ./UmlBaseActivityDiagram.h \
		  ./UmlBaseNode.h \
		  ./UmlNcRelation.h \
		  ./UmlBaseClassView.h \
		  ./UmlBaseClassDiagram.h \
		  ./UmlCom.h \
		  ./UmlBaseActualParameter.h \
		  ./UmlJoinPseudoState.h \
		  ./UmlActivityNode.h \
		  ./UmlItem.h \
		  ./UmlChoicePseudoState.h \
		  ./UmlExpansionRegion.h \
		  ./UmlInitialPseudoState.h \
		  ./UmlObjectDiagram.h \
		  ./UmlAttribute.h \
		  ./UmlRegion.h \
		  ./UmlBaseCollaborationDiagram.h \
		  ./UmlBaseUseCaseDiagram.h \
		  ./UmlBaseActivityRegion.h \
		  ./UmlNode.h \
		  ./UmlOnSignalAction.h \
		  ./UmlTransition.h \
		  ./UmlBaseDiagram.h \
		  ./Association.h \
		  ./UmlClassItem.h \
		  ./UmlUseCase.h \
		  ./UmlBasePackage.h \
		  ./UmlBaseStateDiagram.h \
		  ./UmlActivityDiagram.h \
		  ./UmlPseudoState.h \
		  ./UmlActualParameter.h \
		  ./UmlJunctionPseudoState.h \
		  ./Trigger.h \
		  ./UmlBaseComponentDiagram.h \
		  ./UmlActivityParameter.h \
		  ./UmlBuiltin.h \
		  ./UmlBaseShallowHistoryPseudoState.h \
		  ./UmlBaseJunctionPseudoState.h \
		  ./UmlBaseExtraClassMember.h \
		  ./UmlBaseActivityControlNodeClasses.h \
		  ./UmlExpansionNode.h \
		  ./UmlUseCaseDiagram.h \
		  ./UmlBaseDeployment.h \
		  ./UmlParameterSet.h \
		  ./UmlActivity.h \
		  ./anOrdering.h \
		  ./UmlBaseOnSignalAction.h \
		  ./anExpansionKind.h \
		  ./UmlClassView.h \
		  ./UmlOperation.h \
		  ./UmlShallowHistoryPseudoState.h \
		  ./UmlFlow.h \
		  ./UmlFinalState.h \
		  ./UmlBaseDeploymentView.h \
		  ./UmlComponent.h \
		  ./UmlParameter.h \
		  ./UmlBaseActivityAction.h \
		  ./UmlBaseActivityNode.h \
		  ./JavaSettings.h \
		  ./UmlArtifact.h \
		  ./UmlCollaborationDiagram.h \
		  ./UmlActivityControlNodeClasses.h \
		  ./UmlClassDiagram.h \
		  ./anItemKind.h \
		  ./UmlBaseExpansionRegion.h \
		  ./FlowBehavior.h \
		  ./UmlBaseInterruptibleActivityRegion.h \
		  ./UmlRelation.h \
		  ./UmlForkPseudoState.h \
		  ./UmlUseCaseView.h \
		  ./UmlComponentView.h \
		  ./JavaSettingsCmd.h \
		  ./UmlActivityPin.h \
		  ./aRelationKind.h \
		  ./TransitionBehavior.h \
		  ./UmlComponentDiagram.h \
		  ./UmlActivityItem.h
SOURCES		= ./UmlExtraClassMember.cpp \
		  ./UmlTypeSpec.cpp \
		  ./UmlClassMember.cpp \
		  ./ClassGlobalCmd.cpp \
		  ./PackageGlobalCmd.cpp \
		  ./UmlBaseFlow.cpp \
		  ./UmlBaseStateAction.cpp \
		  ./aVisibility.cpp \
		  ./UmlBaseExitPointPseudoState.cpp \
		  ./UmlBaseObjectDiagram.cpp \
		  ./Sgnl.cpp \
		  ./UmlDeploymentView.cpp \
		  ./UmlActivityRegion.cpp \
		  ./UmlStateAction.cpp \
		  ./UmlStateItem.cpp \
		  ./IdlSettingsCmd.cpp \
		  ./UmlState.cpp \
		  ./UmlBaseComponent.cpp \
		  ./UmlBaseInitialPseudoState.cpp \
		  ./UmlBaseParameterSet.cpp \
		  ./IdlSettings.cpp \
		  ./UmlPinParameter.cpp \
		  ./UmlActivityControlNode.cpp \
		  ./UmlActivityObject.cpp \
		  ./UmlSettingsCmd.cpp \
		  ./UmlDeploymentDiagram.cpp \
		  ./UmlBaseJoinPseudoState.cpp \
		  ./UmlBaseFinalState.cpp \
		  ./UmlBaseSequenceDiagram.cpp \
		  ./UmlBaseRelation.cpp \
		  ./UmlBaseFormalParameter.cpp \
		  ./UmlTerminatePseudoState.cpp \
		  ./MiscGlobalCmd.cpp \
		  ./UmlClass.cpp \
		  ./UmlBaseComponentView.cpp \
		  ./UmlActivityAction.cpp \
		  ./UmlBasePinParameter.cpp \
		  ./UmlBaseState.cpp \
		  ./UmlBaseOperation.cpp \
		  ./CppSettings.cpp \
		  ./FileIn.cpp \
		  ./UmlBaseTransition.cpp \
		  ./UmlBaseClassMember.cpp \
		  ./UmlBaseExpansionNode.cpp \
		  ./UmlBaseActivityParameter.cpp \
		  ./UmlAccessVariableValueAction.cpp \
		  ./UmlPackage.cpp \
		  ./UmlSequenceDiagram.cpp \
		  ./UmlBaseNcRelation.cpp \
		  ./UmlBaseEntryPointPseudoState.cpp \
		  ./UmlBaseUseCaseView.cpp \
		  ./UmlBaseForkPseudoState.cpp \
		  ./UmlBaseUseCase.cpp \
		  ./UmlStereotype.cpp \
		  ./UmlBaseActivity.cpp \
		  ./UmlBaseArtifact.cpp \
		  ./UmlFormalParameter.cpp \
		  ./UmlExitPointPseudoState.cpp \
		  ./UmlBaseClassItem.cpp \
		  ./CppSettingsCmd.cpp \
		  ./UmlStateDiagram.cpp \
		  ./UmlBaseDeploymentDiagram.cpp \
		  ./aParameterEffectKind.cpp \
		  ./UmlBaseActivityObject.cpp \
		  ./UmlBaseActivityControlNode.cpp \
		  ./UmlBaseActivityActionClasses.cpp \
		  ./UmlBaseAccessVariableValueAction.cpp \
		  ./UmlBaseItem.cpp \
		  ./UmlSettings.cpp \
		  ./UmlDiagram.cpp \
		  ./OnInstanceCmd.cpp \
		  ./UmlInterruptibleActivityRegion.cpp \
		  ./UmlBaseActivityPin.cpp \
		  ./UmlBaseDeepHistoryPseudoState.cpp \
		  ./UmlBaseChoicePseudoState.cpp \
		  ./UmlBaseTerminatePseudoState.cpp \
		  ./UmlBaseClass.cpp \
		  ./UmlBaseAttribute.cpp \
		  ./Token.cpp \
		  ./UmlDeepHistoryPseudoState.cpp \
		  ./StateBehavior.cpp \
		  ./CmdFamily.cpp \
		  ./UmlActivityActionClasses.cpp \
		  ./UmlEntryPointPseudoState.cpp \
		  ./aDirection.cpp \
		  ./UmlBaseRegion.cpp \
		  ./UmlBaseActivityDiagram.cpp \
		  ./UmlBaseNode.cpp \
		  ./UmlNcRelation.cpp \
		  ./UmlBaseClassView.cpp \
		  ./UmlBaseClassDiagram.cpp \
		  ./UmlCom.cpp \
		  ./UmlBaseActualParameter.cpp \
		  ./UmlJoinPseudoState.cpp \
		  ./UmlActivityNode.cpp \
		  ./UmlItem.cpp \
		  ./UmlChoicePseudoState.cpp \
		  ./UmlExpansionRegion.cpp \
		  ./UmlInitialPseudoState.cpp \
		  ./UmlObjectDiagram.cpp \
		  ./UmlAttribute.cpp \
		  ./UmlRegion.cpp \
		  ./UmlBaseCollaborationDiagram.cpp \
		  ./UmlBaseUseCaseDiagram.cpp \
		  ./UmlBaseActivityRegion.cpp \
		  ./UmlNode.cpp \
		  ./UmlOnSignalAction.cpp \
		  ./UmlTransition.cpp \
		  ./UmlBaseDiagram.cpp \
		  ./Association.cpp \
		  ./UmlClassItem.cpp \
		  ./UmlUseCase.cpp \
		  ./UmlBasePackage.cpp \
		  ./UmlBaseStateDiagram.cpp \
		  ./UmlActivityDiagram.cpp \
		  ./UmlPseudoState.cpp \
		  ./UmlActualParameter.cpp \
		  ./UmlJunctionPseudoState.cpp \
		  ./Trigger.cpp \
		  ./UmlBaseComponentDiagram.cpp \
		  ./UmlActivityParameter.cpp \
		  ./UmlBuiltin.cpp \
		  ./UmlBaseShallowHistoryPseudoState.cpp \
		  ./UmlBaseJunctionPseudoState.cpp \
		  ./UmlBaseExtraClassMember.cpp \
		  ./UmlBaseActivityControlNodeClasses.cpp \
		  ./UmlExpansionNode.cpp \
		  ./Main.cpp \
		  ./UmlUseCaseDiagram.cpp \
		  ./UmlBaseDeployment.cpp \
		  ./UmlParameterSet.cpp \
		  ./UmlActivity.cpp \
		  ./anOrdering.cpp \
		  ./UmlBaseOnSignalAction.cpp \
		  ./anExpansionKind.cpp \
		  ./UmlClassView.cpp \
		  ./UmlOperation.cpp \
		  ./UmlShallowHistoryPseudoState.cpp \
		  ./UmlFlow.cpp \
		  ./UmlFinalState.cpp \
		  ./UmlBaseDeploymentView.cpp \
		  ./UmlComponent.cpp \
		  ./UmlParameter.cpp \
		  ./UmlBaseActivityAction.cpp \
		  ./UmlBaseActivityNode.cpp \
		  ./JavaSettings.cpp \
		  ./UmlArtifact.cpp \
		  ./UmlCollaborationDiagram.cpp \
		  ./UmlActivityControlNodeClasses.cpp \
		  ./UmlClassDiagram.cpp \
		  ./anItemKind.cpp \
		  ./UmlBaseExpansionRegion.cpp \
		  ./FlowBehavior.cpp \
		  ./UmlBaseInterruptibleActivityRegion.cpp \
		  ./UmlRelation.cpp \
		  ./UmlForkPseudoState.cpp \
		  ./UmlUseCaseView.cpp \
		  ./UmlComponentView.cpp \
		  ./JavaSettingsCmd.cpp \
		  ./UmlActivityPin.cpp \
		  ./aRelationKind.cpp \
		  ./TransitionBehavior.cpp \
		  ./UmlComponentDiagram.cpp \
		  ./UmlActivityItem.cpp
