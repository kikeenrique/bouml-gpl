TEMPLATE	= app
TARGET		= stmgen
CONFIG		+= release warn_on qt
DEFINES		= WITHCPP WITHJAVA WITHIDL
HEADERS		= ./UmlBuiltin.h \
		  ./UmlExpansionRegion.h \
		  ./UmlBaseComponent.h \
		  ./UmlBaseRelation.h \
		  ./UmlJoinPseudoState.h \
		  ./UmlStateAction.h \
		  ./OnInstanceCmd.h \
		  ./UmlAccessVariableValueAction.h \
		  ./UmlActivityAction.h \
		  ./UmlBaseActivityAction.h \
		  ./UmlBaseParameterSet.h \
		  ./UmlObjectDiagram.h \
		  ./UmlStateItem.h \
		  ./UmlBaseClassDiagram.h \
		  ./UmlBaseDeployment.h \
		  ./UmlBaseClassItem.h \
		  ./UmlBaseNcRelation.h \
		  ./UmlItem.h \
		  ./CmdFamily.h \
		  ./UmlOperation.h \
		  ./UmlBaseExitPointPseudoState.h \
		  ./UmlBaseTerminatePseudoState.h \
		  ./UmlExitPointPseudoState.h \
		  ./UmlActivityItem.h \
		  ./UmlBaseActivityControlNodeClasses.h \
		  ./aLanguage.h \
		  ./UmlClassView.h \
		  ./UmlBaseUseCaseDiagram.h \
		  ./UmlClassDiagram.h \
		  ./UmlDeepHistoryPseudoState.h \
		  ./UmlBaseJunctionPseudoState.h \
		  ./UmlFinalState.h \
		  ./UmlBaseActivityObject.h \
		  ./UmlBaseFlow.h \
		  ./UmlFlow.h \
		  ./UmlPinParameter.h \
		  ./UmlBaseActualParameter.h \
		  ./UmlBaseActivityControlNode.h \
		  ./UmlParameter.h \
		  ./aRelationKind.h \
		  ./aDirection.h \
		  ./UmlCollaborationDiagram.h \
		  ./UmlBaseOperation.h \
		  ./UmlBaseEntryPointPseudoState.h \
		  ./UmlBaseUseCaseView.h \
		  ./UmlTerminatePseudoState.h \
		  ./UmlBaseRegion.h \
		  ./UmlActivityActionClasses.h \
		  ./UmlActivityRegion.h \
		  ./UmlBaseActivityDiagram.h \
		  ./UmlBaseExpansionRegion.h \
		  ./UmlCom.h \
		  ./UmlPackage.h \
		  ./UmlBaseArtifact.h \
		  ./UmlSettings.h \
		  ./UmlRegion.h \
		  ./UmlBaseAttribute.h \
		  ./UmlBaseJoinPseudoState.h \
		  ./UmlBaseAccessVariableValueAction.h \
		  ./UmlBaseInterruptibleActivityRegion.h \
		  ./UmlExpansionNode.h \
		  ./UmlBaseStateAction.h \
		  ./UmlAttribute.h \
		  ./UmlRelation.h \
		  ./UmlBaseChoicePseudoState.h \
		  ./CppSettings.h \
		  ./UmlBaseFormalParameter.h \
		  ./UmlBaseActivityNode.h \
		  ./aParameterEffectKind.h \
		  ./UmlBaseItem.h \
		  ./UmlBaseForkPseudoState.h \
		  ./UmlBaseDeploymentDiagram.h \
		  ./UmlActivityNode.h \
		  ./StateBehavior.h \
		  ./UmlBaseTransition.h \
		  ./UmlSequenceDiagram.h \
		  ./UmlForkPseudoState.h \
		  ./anItemKind.h \
		  ./UmlNcRelation.h \
		  ./UmlBaseComponentDiagram.h \
		  ./UmlBaseUseCase.h \
		  ./UmlBaseActivityPin.h \
		  ./UmlBaseActivityRegion.h \
		  ./anExpansionKind.h \
		  ./UmlState.h \
		  ./JavaSettings.h \
		  ./JavaSettingsCmd.h \
		  ./UmlBaseCollaborationDiagram.h \
		  ./UmlExtraClassMember.h \
		  ./UmlClass.h \
		  ./UmlUseCase.h \
		  ./UmlTypeSpec.h \
		  ./UmlBaseDeploymentView.h \
		  ./UmlComponentDiagram.h \
		  ./UmlArtifact.h \
		  ./UmlBaseSequenceDiagram.h \
		  ./UmlFormalParameter.h \
		  ./ClassGlobalCmd.h \
		  ./PackageGlobalCmd.h \
		  ./UmlJunctionPseudoState.h \
		  ./UmlBaseFinalState.h \
		  ./UmlBaseShallowHistoryPseudoState.h \
		  ./UmlInitialPseudoState.h \
		  ./UmlStereotype.h \
		  ./UmlPseudoState.h \
		  ./UmlNode.h \
		  ./UmlDiagram.h \
		  ./UmlActivity.h \
		  ./UmlActivityPin.h \
		  ./UmlBaseActivityActionClasses.h \
		  ./UmlInterruptibleActivityRegion.h \
		  ./UmlOnSignalAction.h \
		  ./UmlParameterSet.h \
		  ./anOrdering.h \
		  ./IdlSettingsCmd.h \
		  ./UmlBaseState.h \
		  ./UmlComponentView.h \
		  ./TransitionBehavior.h \
		  ./FlowBehavior.h \
		  ./UmlActivityControlNode.h \
		  ./UmlActivityObject.h \
		  ./UmlEntryPointPseudoState.h \
		  ./UmlClassMember.h \
		  ./UmlActualParameter.h \
		  ./UmlShallowHistoryPseudoState.h \
		  ./UmlSettingsCmd.h \
		  ./UmlBaseExpansionNode.h \
		  ./UmlBaseObjectDiagram.h \
		  ./UmlBaseOnSignalAction.h \
		  ./UmlBasePinParameter.h \
		  ./UmlStateDiagram.h \
		  ./UmlClassItem.h \
		  ./UmlBasePackage.h \
		  ./UmlBaseInitialPseudoState.h \
		  ./MiscGlobalCmd.h \
		  ./UmlActivityControlNodeClasses.h \
		  ./UmlActivityDiagram.h \
		  ./UmlActivityParameter.h \
		  ./UmlChoicePseudoState.h \
		  ./UmlBaseExtraClassMember.h \
		  ./UmlComponent.h \
		  ./aVisibility.h \
		  ./UmlDeploymentDiagram.h \
		  ./UmlBaseClass.h \
		  ./UmlBaseStateDiagram.h \
		  ./UmlDeploymentView.h \
		  ./UmlBaseActivity.h \
		  ./UmlBaseActivityParameter.h \
		  ./UmlBaseComponentView.h \
		  ./UmlUseCaseView.h \
		  ./IdlSettings.h \
		  ./UmlUseCaseDiagram.h \
		  ./CppSettingsCmd.h \
		  ./UmlBaseDeepHistoryPseudoState.h \
		  ./UmlTransition.h \
		  ./UmlBaseNode.h \
		  ./UmlBaseClassView.h \
		  ./UmlBaseDiagram.h \
		  ./UmlBaseClassMember.h
SOURCES		= ./UmlBuiltin.cpp \
		  ./UmlExpansionRegion.cpp \
		  ./UmlBaseComponent.cpp \
		  ./UmlBaseRelation.cpp \
		  ./UmlJoinPseudoState.cpp \
		  ./UmlStateAction.cpp \
		  ./OnInstanceCmd.cpp \
		  ./UmlAccessVariableValueAction.cpp \
		  ./UmlActivityAction.cpp \
		  ./UmlBaseActivityAction.cpp \
		  ./UmlBaseParameterSet.cpp \
		  ./UmlObjectDiagram.cpp \
		  ./UmlStateItem.cpp \
		  ./UmlBaseClassDiagram.cpp \
		  ./UmlBaseDeployment.cpp \
		  ./UmlBaseClassItem.cpp \
		  ./UmlBaseNcRelation.cpp \
		  ./UmlItem.cpp \
		  ./CmdFamily.cpp \
		  ./UmlOperation.cpp \
		  ./UmlBaseExitPointPseudoState.cpp \
		  ./UmlBaseTerminatePseudoState.cpp \
		  ./UmlExitPointPseudoState.cpp \
		  ./UmlActivityItem.cpp \
		  ./UmlBaseActivityControlNodeClasses.cpp \
		  ./aLanguage.cpp \
		  ./UmlClassView.cpp \
		  ./UmlBaseUseCaseDiagram.cpp \
		  ./UmlClassDiagram.cpp \
		  ./UmlDeepHistoryPseudoState.cpp \
		  ./UmlBaseJunctionPseudoState.cpp \
		  ./UmlFinalState.cpp \
		  ./UmlBaseActivityObject.cpp \
		  ./UmlBaseFlow.cpp \
		  ./UmlFlow.cpp \
		  ./UmlPinParameter.cpp \
		  ./UmlBaseActualParameter.cpp \
		  ./UmlBaseActivityControlNode.cpp \
		  ./UmlParameter.cpp \
		  ./aRelationKind.cpp \
		  ./aDirection.cpp \
		  ./UmlCollaborationDiagram.cpp \
		  ./UmlBaseOperation.cpp \
		  ./UmlBaseEntryPointPseudoState.cpp \
		  ./UmlBaseUseCaseView.cpp \
		  ./UmlTerminatePseudoState.cpp \
		  ./UmlBaseRegion.cpp \
		  ./UmlActivityActionClasses.cpp \
		  ./UmlActivityRegion.cpp \
		  ./UmlBaseActivityDiagram.cpp \
		  ./UmlBaseExpansionRegion.cpp \
		  ./UmlCom.cpp \
		  ./UmlPackage.cpp \
		  ./UmlBaseArtifact.cpp \
		  ./UmlSettings.cpp \
		  ./UmlRegion.cpp \
		  ./UmlBaseAttribute.cpp \
		  ./UmlBaseJoinPseudoState.cpp \
		  ./UmlBaseAccessVariableValueAction.cpp \
		  ./UmlBaseInterruptibleActivityRegion.cpp \
		  ./UmlExpansionNode.cpp \
		  ./UmlBaseStateAction.cpp \
		  ./UmlAttribute.cpp \
		  ./UmlRelation.cpp \
		  ./UmlBaseChoicePseudoState.cpp \
		  ./CppSettings.cpp \
		  ./UmlBaseFormalParameter.cpp \
		  ./UmlBaseActivityNode.cpp \
		  ./aParameterEffectKind.cpp \
		  ./UmlBaseItem.cpp \
		  ./UmlBaseForkPseudoState.cpp \
		  ./UmlBaseDeploymentDiagram.cpp \
		  ./UmlActivityNode.cpp \
		  ./StateBehavior.cpp \
		  ./UmlBaseTransition.cpp \
		  ./UmlSequenceDiagram.cpp \
		  ./UmlForkPseudoState.cpp \
		  ./anItemKind.cpp \
		  ./UmlNcRelation.cpp \
		  ./UmlBaseComponentDiagram.cpp \
		  ./UmlBaseUseCase.cpp \
		  ./UmlBaseActivityPin.cpp \
		  ./UmlBaseActivityRegion.cpp \
		  ./anExpansionKind.cpp \
		  ./UmlState.cpp \
		  ./JavaSettings.cpp \
		  ./JavaSettingsCmd.cpp \
		  ./UmlBaseCollaborationDiagram.cpp \
		  ./UmlExtraClassMember.cpp \
		  ./UmlClass.cpp \
		  ./UmlUseCase.cpp \
		  ./UmlTypeSpec.cpp \
		  ./UmlBaseDeploymentView.cpp \
		  ./UmlComponentDiagram.cpp \
		  ./UmlArtifact.cpp \
		  ./UmlBaseSequenceDiagram.cpp \
		  ./UmlFormalParameter.cpp \
		  ./ClassGlobalCmd.cpp \
		  ./PackageGlobalCmd.cpp \
		  ./UmlJunctionPseudoState.cpp \
		  ./UmlBaseFinalState.cpp \
		  ./UmlBaseShallowHistoryPseudoState.cpp \
		  ./UmlInitialPseudoState.cpp \
		  ./UmlStereotype.cpp \
		  ./UmlPseudoState.cpp \
		  ./UmlNode.cpp \
		  ./UmlDiagram.cpp \
		  ./UmlActivity.cpp \
		  ./UmlActivityPin.cpp \
		  ./UmlBaseActivityActionClasses.cpp \
		  ./UmlInterruptibleActivityRegion.cpp \
		  ./UmlOnSignalAction.cpp \
		  ./UmlParameterSet.cpp \
		  ./anOrdering.cpp \
		  ./IdlSettingsCmd.cpp \
		  ./UmlBaseState.cpp \
		  ./UmlComponentView.cpp \
		  ./TransitionBehavior.cpp \
		  ./FlowBehavior.cpp \
		  ./UmlActivityControlNode.cpp \
		  ./UmlActivityObject.cpp \
		  ./UmlEntryPointPseudoState.cpp \
		  ./UmlClassMember.cpp \
		  ./UmlActualParameter.cpp \
		  ./UmlShallowHistoryPseudoState.cpp \
		  ./UmlSettingsCmd.cpp \
		  ./UmlBaseExpansionNode.cpp \
		  ./UmlBaseObjectDiagram.cpp \
		  ./UmlBaseOnSignalAction.cpp \
		  ./UmlBasePinParameter.cpp \
		  ./UmlStateDiagram.cpp \
		  ./UmlClassItem.cpp \
		  ./UmlBasePackage.cpp \
		  ./UmlBaseInitialPseudoState.cpp \
		  ./MiscGlobalCmd.cpp \
		  ./UmlActivityControlNodeClasses.cpp \
		  ./UmlActivityDiagram.cpp \
		  ./UmlActivityParameter.cpp \
		  ./UmlChoicePseudoState.cpp \
		  ./UmlBaseExtraClassMember.cpp \
		  ./UmlComponent.cpp \
		  ./aVisibility.cpp \
		  ./UmlDeploymentDiagram.cpp \
		  ./UmlBaseClass.cpp \
		  ./Main.cpp \
		  ./UmlBaseStateDiagram.cpp \
		  ./UmlDeploymentView.cpp \
		  ./UmlBaseActivity.cpp \
		  ./UmlBaseActivityParameter.cpp \
		  ./UmlBaseComponentView.cpp \
		  ./UmlUseCaseView.cpp \
		  ./IdlSettings.cpp \
		  ./UmlUseCaseDiagram.cpp \
		  ./CppSettingsCmd.cpp \
		  ./UmlBaseDeepHistoryPseudoState.cpp \
		  ./UmlTransition.cpp \
		  ./UmlBaseNode.cpp \
		  ./UmlBaseClassView.cpp \
		  ./UmlBaseDiagram.cpp \
		  ./UmlBaseClassMember.cpp
