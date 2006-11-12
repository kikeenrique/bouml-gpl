TEMPLATE	= app
TARGET		= global_change
CONFIG		+= release warn_on qt
DEFINES		= WITHCPP WITHJAVA WITHIDL
HEADERS		= ./UmlBaseDiagram.h \
		  ./UmlBaseActivityActionClasses.h \
		  ./UmlBaseActivityControlNode.h \
		  ./UmlBaseActivity.h \
		  ./FlowBehavior.h \
		  ./UmlBaseActivityPin.h \
		  ./anItemKind.h \
		  ./UmlBaseActivityControlNodeClasses.h \
		  ./MiscGlobalCmd.h \
		  ./JavaSettingsCmd.h \
		  ./UmlBaseStateAction.h \
		  ./UmlActivityNode.h \
		  ./UmlDeepHistoryPseudoState.h \
		  ./UmlBaseFinalState.h \
		  ./UmlExtraClassMember.h \
		  ./UmlBaseUseCaseDiagram.h \
		  ./UmlJunctionPseudoState.h \
		  ./UmlBasePinParameter.h \
		  ./CmdFamily.h \
		  ./UmlBaseItem.h \
		  ./UmlBaseCollaborationDiagram.h \
		  ./UmlChoicePseudoState.h \
		  ./UmlBaseComponent.h \
		  ./UmlClassItem.h \
		  ./UmlBaseDeploymentDiagram.h \
		  ./UmlActivityActionClasses.h \
		  ./UmlBuiltin.h \
		  ./UmlBaseNcRelation.h \
		  ./UmlDeploymentView.h \
		  ./ClassGlobalCmd.h \
		  ./UmlDeploymentDiagram.h \
		  ./IdlSettings.h \
		  ./UmlActivityDiagram.h \
		  ./UmlBaseNode.h \
		  ./UmlBaseExitPointPseudoState.h \
		  ./UmlRegion.h \
		  ./UmlComponent.h \
		  ./UmlNcRelation.h \
		  ./UmlParameter.h \
		  ./aRelationKind.h \
		  ./UmlEntryPointPseudoState.h \
		  ./UmlBaseEntryPointPseudoState.h \
		  ./UmlFlow.h \
		  ./UmlBaseJoinPseudoState.h \
		  ./UmlItem.h \
		  ./UmlFormalParameter.h \
		  ./UmlBaseState.h \
		  ./UmlBaseClassDiagram.h \
		  ./CppSettings.h \
		  ./UmlBaseObjectDiagram.h \
		  ./UmlBaseOnSignalAction.h \
		  ./UmlObjectDiagram.h \
		  ./UmlAttribute.h \
		  ./UmlBaseTransition.h \
		  ./UmlBaseExtraClassMember.h \
		  ./UmlBaseExpansionRegion.h \
		  ./UmlStereotype.h \
		  ./UmlClass.h \
		  ./UmlComponentDiagram.h \
		  ./UmlActivityControlNodeClasses.h \
		  ./UmlFinalState.h \
		  ./UmlBaseClassItem.h \
		  ./UmlBaseStateDiagram.h \
		  ./UmlActivityPin.h \
		  ./UmlBaseRegion.h \
		  ./UmlBaseActualParameter.h \
		  ./UmlBaseActivityNode.h \
		  ./UmlBaseOperation.h \
		  ./UmlStateItem.h \
		  ./UmlActualParameter.h \
		  ./UmlBaseShallowHistoryPseudoState.h \
		  ./UmlBaseClassView.h \
		  ./UmlNode.h \
		  ./UmlBaseComponentDiagram.h \
		  ./PackageGlobalCmd.h \
		  ./IdlSettingsCmd.h \
		  ./UmlStateDiagram.h \
		  ./UmlCom.h \
		  ./UmlForkPseudoState.h \
		  ./StateBehavior.h \
		  ./UmlClassDiagram.h \
		  ./UmlBaseDeployment.h \
		  ./UmlBaseInterruptibleActivityRegion.h \
		  ./UmlBaseTerminatePseudoState.h \
		  ./TransitionBehavior.h \
		  ./UmlBaseClassMember.h \
		  ./UmlRelation.h \
		  ./UmlComponentView.h \
		  ./UmlBaseComponentView.h \
		  ./Context.h \
		  ./UmlSettingsCmd.h \
		  ./UmlActivity.h \
		  ./UmlAccessVariableValueAction.h \
		  ./UmlBaseDeepHistoryPseudoState.h \
		  ./UmlUseCaseDiagram.h \
		  ./UmlBaseActivityDiagram.h \
		  ./UmlBaseAccessVariableValueAction.h \
		  ./UmlStateAction.h \
		  ./UmlCollaborationDiagram.h \
		  ./UmlBaseDeploymentView.h \
		  ./UmlPseudoState.h \
		  ./UmlActivityObject.h \
		  ./UmlBaseParameterSet.h \
		  ./aVisibility.h \
		  ./UmlBaseRelation.h \
		  ./UmlBaseActivityAction.h \
		  ./UmlBaseAttribute.h \
		  ./UmlActivityRegion.h \
		  ./UmlUseCaseView.h \
		  ./UmlBaseInitialPseudoState.h \
		  ./JavaSettings.h \
		  ./UmlExpansionNode.h \
		  ./UmlPinParameter.h \
		  ./UmlBaseActivityObject.h \
		  ./UmlBaseActivityRegion.h \
		  ./UmlBaseActivityParameter.h \
		  ./UmlBaseArtifact.h \
		  ./UmlExitPointPseudoState.h \
		  ./CppSettingsCmd.h \
		  ./UmlClassMember.h \
		  ./UmlBaseForkPseudoState.h \
		  ./UmlOperation.h \
		  ./UmlTransition.h \
		  ./UmlBaseChoicePseudoState.h \
		  ./UmlBaseJunctionPseudoState.h \
		  ./UmlJoinPseudoState.h \
		  ./UmlActivityItem.h \
		  ./UmlArtifact.h \
		  ./UmlShallowHistoryPseudoState.h \
		  ./UmlTerminatePseudoState.h \
		  ./UmlInitialPseudoState.h \
		  ./UmlActivityAction.h \
		  ./UmlOnSignalAction.h \
		  ./aParameterEffectKind.h \
		  ./OnInstanceCmd.h \
		  ./UmlBaseUseCase.h \
		  ./Dialog.h \
		  ./UmlSettings.h \
		  ./UmlActivityParameter.h \
		  ./UmlBasePackage.h \
		  ./UmlBaseFormalParameter.h \
		  ./aDirection.h \
		  ./UmlActivityControlNode.h \
		  ./UmlDiagram.h \
		  ./UmlClassView.h \
		  ./UmlState.h \
		  ./UmlParameterSet.h \
		  ./anOrdering.h \
		  ./UmlPackage.h \
		  ./anExpansionKind.h \
		  ./UmlBaseFlow.h \
		  ./UmlTypeSpec.h \
		  ./UmlBaseExpansionNode.h \
		  ./UmlBaseUseCaseView.h \
		  ./UmlBaseSequenceDiagram.h \
		  ./UmlInterruptibleActivityRegion.h \
		  ./UmlExpansionRegion.h \
		  ./UmlBaseClass.h \
		  ./UmlSequenceDiagram.h \
		  ./UmlUseCase.h
SOURCES		= ./UmlBaseDiagram.cpp \
		  ./UmlBaseActivityActionClasses.cpp \
		  ./UmlBaseActivityControlNode.cpp \
		  ./UmlBaseActivity.cpp \
		  ./FlowBehavior.cpp \
		  ./UmlBaseActivityPin.cpp \
		  ./anItemKind.cpp \
		  ./UmlBaseActivityControlNodeClasses.cpp \
		  ./MiscGlobalCmd.cpp \
		  ./JavaSettingsCmd.cpp \
		  ./UmlBaseStateAction.cpp \
		  ./UmlActivityNode.cpp \
		  ./UmlDeepHistoryPseudoState.cpp \
		  ./UmlBaseFinalState.cpp \
		  ./UmlExtraClassMember.cpp \
		  ./UmlBaseUseCaseDiagram.cpp \
		  ./UmlJunctionPseudoState.cpp \
		  ./UmlBasePinParameter.cpp \
		  ./CmdFamily.cpp \
		  ./UmlBaseItem.cpp \
		  ./UmlBaseCollaborationDiagram.cpp \
		  ./UmlChoicePseudoState.cpp \
		  ./UmlBaseComponent.cpp \
		  ./UmlClassItem.cpp \
		  ./UmlBaseDeploymentDiagram.cpp \
		  ./UmlActivityActionClasses.cpp \
		  ./UmlBuiltin.cpp \
		  ./UmlBaseNcRelation.cpp \
		  ./UmlDeploymentView.cpp \
		  ./ClassGlobalCmd.cpp \
		  ./UmlDeploymentDiagram.cpp \
		  ./IdlSettings.cpp \
		  ./UmlActivityDiagram.cpp \
		  ./UmlBaseNode.cpp \
		  ./UmlBaseExitPointPseudoState.cpp \
		  ./UmlRegion.cpp \
		  ./UmlComponent.cpp \
		  ./UmlNcRelation.cpp \
		  ./UmlParameter.cpp \
		  ./aRelationKind.cpp \
		  ./UmlEntryPointPseudoState.cpp \
		  ./UmlBaseEntryPointPseudoState.cpp \
		  ./UmlFlow.cpp \
		  ./UmlBaseJoinPseudoState.cpp \
		  ./UmlItem.cpp \
		  ./UmlFormalParameter.cpp \
		  ./UmlBaseState.cpp \
		  ./UmlBaseClassDiagram.cpp \
		  ./CppSettings.cpp \
		  ./UmlBaseObjectDiagram.cpp \
		  ./UmlBaseOnSignalAction.cpp \
		  ./UmlObjectDiagram.cpp \
		  ./UmlAttribute.cpp \
		  ./UmlBaseTransition.cpp \
		  ./UmlBaseExtraClassMember.cpp \
		  ./UmlBaseExpansionRegion.cpp \
		  ./UmlStereotype.cpp \
		  ./UmlClass.cpp \
		  ./UmlComponentDiagram.cpp \
		  ./UmlActivityControlNodeClasses.cpp \
		  ./UmlFinalState.cpp \
		  ./UmlBaseClassItem.cpp \
		  ./UmlBaseStateDiagram.cpp \
		  ./UmlActivityPin.cpp \
		  ./UmlBaseRegion.cpp \
		  ./UmlBaseActualParameter.cpp \
		  ./UmlBaseActivityNode.cpp \
		  ./UmlBaseOperation.cpp \
		  ./UmlStateItem.cpp \
		  ./UmlActualParameter.cpp \
		  ./UmlBaseShallowHistoryPseudoState.cpp \
		  ./Main.cpp \
		  ./UmlBaseClassView.cpp \
		  ./UmlNode.cpp \
		  ./UmlBaseComponentDiagram.cpp \
		  ./PackageGlobalCmd.cpp \
		  ./IdlSettingsCmd.cpp \
		  ./UmlStateDiagram.cpp \
		  ./UmlCom.cpp \
		  ./UmlForkPseudoState.cpp \
		  ./StateBehavior.cpp \
		  ./UmlClassDiagram.cpp \
		  ./UmlBaseDeployment.cpp \
		  ./UmlBaseInterruptibleActivityRegion.cpp \
		  ./UmlBaseTerminatePseudoState.cpp \
		  ./TransitionBehavior.cpp \
		  ./UmlBaseClassMember.cpp \
		  ./UmlRelation.cpp \
		  ./UmlComponentView.cpp \
		  ./UmlBaseComponentView.cpp \
		  ./Context.cpp \
		  ./UmlSettingsCmd.cpp \
		  ./UmlActivity.cpp \
		  ./UmlAccessVariableValueAction.cpp \
		  ./UmlBaseDeepHistoryPseudoState.cpp \
		  ./UmlUseCaseDiagram.cpp \
		  ./UmlBaseActivityDiagram.cpp \
		  ./UmlBaseAccessVariableValueAction.cpp \
		  ./UmlStateAction.cpp \
		  ./UmlCollaborationDiagram.cpp \
		  ./UmlBaseDeploymentView.cpp \
		  ./UmlPseudoState.cpp \
		  ./UmlActivityObject.cpp \
		  ./UmlBaseParameterSet.cpp \
		  ./aVisibility.cpp \
		  ./UmlBaseRelation.cpp \
		  ./UmlBaseActivityAction.cpp \
		  ./UmlBaseAttribute.cpp \
		  ./UmlActivityRegion.cpp \
		  ./UmlUseCaseView.cpp \
		  ./UmlBaseInitialPseudoState.cpp \
		  ./JavaSettings.cpp \
		  ./UmlExpansionNode.cpp \
		  ./UmlPinParameter.cpp \
		  ./UmlBaseActivityObject.cpp \
		  ./UmlBaseActivityRegion.cpp \
		  ./UmlBaseActivityParameter.cpp \
		  ./UmlBaseArtifact.cpp \
		  ./UmlExitPointPseudoState.cpp \
		  ./CppSettingsCmd.cpp \
		  ./UmlClassMember.cpp \
		  ./UmlBaseForkPseudoState.cpp \
		  ./UmlOperation.cpp \
		  ./UmlTransition.cpp \
		  ./UmlBaseChoicePseudoState.cpp \
		  ./UmlBaseJunctionPseudoState.cpp \
		  ./UmlJoinPseudoState.cpp \
		  ./UmlActivityItem.cpp \
		  ./UmlArtifact.cpp \
		  ./UmlShallowHistoryPseudoState.cpp \
		  ./UmlTerminatePseudoState.cpp \
		  ./UmlInitialPseudoState.cpp \
		  ./UmlActivityAction.cpp \
		  ./UmlOnSignalAction.cpp \
		  ./aParameterEffectKind.cpp \
		  ./OnInstanceCmd.cpp \
		  ./UmlBaseUseCase.cpp \
		  ./Dialog.cpp \
		  ./UmlSettings.cpp \
		  ./UmlActivityParameter.cpp \
		  ./UmlBasePackage.cpp \
		  ./UmlBaseFormalParameter.cpp \
		  ./aDirection.cpp \
		  ./UmlActivityControlNode.cpp \
		  ./UmlDiagram.cpp \
		  ./UmlClassView.cpp \
		  ./UmlState.cpp \
		  ./UmlParameterSet.cpp \
		  ./anOrdering.cpp \
		  ./UmlPackage.cpp \
		  ./anExpansionKind.cpp \
		  ./UmlBaseFlow.cpp \
		  ./UmlTypeSpec.cpp \
		  ./UmlBaseExpansionNode.cpp \
		  ./UmlBaseUseCaseView.cpp \
		  ./UmlBaseSequenceDiagram.cpp \
		  ./UmlInterruptibleActivityRegion.cpp \
		  ./UmlExpansionRegion.cpp \
		  ./UmlBaseClass.cpp \
		  ./UmlSequenceDiagram.cpp \
		  ./UmlUseCase.cpp
