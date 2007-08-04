TEMPLATE	= app
TARGET		= ixmi2
CONFIG		+= release warn_on qt
DEFINES		= WITHCPP WITHJAVA WITHIDL
HEADERS		= ./UmlBaseClassInstance.h \
		  ./UmlBaseUseCase.h \
		  ./UmlActivityObject.h \
		  ./UmlActivityRegion.h \
		  ./UmlBaseNode.h \
		  ./UmlActivityActionClasses.h \
		  ./UmlBaseComponent.h \
		  ./IdlSettings.h \
		  ./UmlBaseStateAction.h \
		  ./aRelationKind.h \
		  ./Association.h \
		  ./UmlClassMember.h \
		  ./UmlBaseSequenceDiagram.h \
		  ./UmlBaseUseCaseView.h \
		  ./UmlPackage.h \
		  ./UmlBaseNcRelation.h \
		  ./UmlBaseClassDiagram.h \
		  ./CppSettingsCmd.h \
		  ./UmlBaseActivityDiagram.h \
		  ./UmlBaseDeployment.h \
		  ./UmlBaseShallowHistoryPseudoState.h \
		  ./UmlExtraClassMember.h \
		  ./UmlBaseTransition.h \
		  ./UmlBaseEntryPointPseudoState.h \
		  ./UmlDeploymentDiagram.h \
		  ./UmlBaseOnSignalAction.h \
		  ./UmlBaseInterruptibleActivityRegion.h \
		  ./UmlFormalParameter.h \
		  ./UmlActivityNode.h \
		  ./aDirection.h \
		  ./UmlCom.h \
		  ./UmlActivityControlNodeClasses.h \
		  ./Sgnl.h \
		  ./UmlBaseClassView.h \
		  ./OnInstanceCmd.h \
		  ./UmlStateItem.h \
		  ./UmlEntryPointPseudoState.h \
		  ./UmlBaseArtifact.h \
		  ./UmlInterruptibleActivityRegion.h \
		  ./UmlBaseDeepHistoryPseudoState.h \
		  ./UmlBaseExtraClassMember.h \
		  ./UmlNode.h \
		  ./CppSettings.h \
		  ./anItemKind.h \
		  ./UmlBaseActivityNode.h \
		  ./UmlBaseForkPseudoState.h \
		  ./anExpansionKind.h \
		  ./UmlBaseComponentView.h \
		  ./UmlBaseFlow.h \
		  ./UmlStereotype.h \
		  ./UmlUseCase.h \
		  ./UmlTransition.h \
		  ./UmlPseudoState.h \
		  ./JavaSettingsCmd.h \
		  ./MiscGlobalCmd.h \
		  ./UmlStateAction.h \
		  ./UmlChoicePseudoState.h \
		  ./UmlCollaborationDiagram.h \
		  ./aLanguage.h \
		  ./UmlBaseActivityRegion.h \
		  ./UmlStateDiagram.h \
		  ./UmlParameterSet.h \
		  ./UmlBaseJoinPseudoState.h \
		  ./UmlActivityAction.h \
		  ./UmlActivityPin.h \
		  ./FlowBehavior.h \
		  ./CmdFamily.h \
		  ./UmlBasePackage.h \
		  ./UmlSequenceDiagram.h \
		  ./Token.h \
		  ./UmlBaseCollaborationDiagram.h \
		  ./ClassGlobalCmd.h \
		  ./UmlExpansionNode.h \
		  ./UmlBaseAccessVariableValueAction.h \
		  ./UmlClassDiagram.h \
		  ./UmlUseCaseView.h \
		  ./UmlBaseActivity.h \
		  ./UmlItem.h \
		  ./UmlDeepHistoryPseudoState.h \
		  ./FileIn.h \
		  ./JavaSettings.h \
		  ./UmlBaseParameterSet.h \
		  ./UmlExitPointPseudoState.h \
		  ./UmlBaseActivityAction.h \
		  ./UmlClass.h \
		  ./UmlBaseItem.h \
		  ./UmlActivityControlNode.h \
		  ./SlotAttribute.h \
		  ./UmlView.h \
		  ./UmlOnSignalAction.h \
		  ./UmlBaseActivityObject.h \
		  ./UmlJunctionPseudoState.h \
		  ./UmlBuiltin.h \
		  ./UmlBaseExpansionRegion.h \
		  ./UmlExpansionRegion.h \
		  ./UmlBaseUseCaseDiagram.h \
		  ./UmlBaseActivityParameter.h \
		  ./UmlBaseRelation.h \
		  ./UmlClassInstance.h \
		  ./UmlDiagram.h \
		  ./UmlRelation.h \
		  ./UmlBaseFinalState.h \
		  ./TransitionBehavior.h \
		  ./UmlBaseAttribute.h \
		  ./UmlFinalState.h \
		  ./UmlBaseJunctionPseudoState.h \
		  ./UmlBaseParameter.h \
		  ./UmlAccessVariableValueAction.h \
		  ./Trigger.h \
		  ./aParameterEffectKind.h \
		  ./UmlNcRelation.h \
		  ./UmlBaseExitPointPseudoState.h \
		  ./UmlBaseTerminatePseudoState.h \
		  ./UmlTypeSpec.h \
		  ./UmlBaseInitialPseudoState.h \
		  ./UmlClassItem.h \
		  ./UmlBaseFormalParameter.h \
		  ./UmlBaseObjectDiagram.h \
		  ./UmlAttribute.h \
		  ./UmlBaseDiagram.h \
		  ./UmlSettings.h \
		  ./UmlState.h \
		  ./UmlBaseTypeSpec.h \
		  ./UmlBaseView.h \
		  ./UmlActivityItem.h \
		  ./IdlSettingsCmd.h \
		  ./PackageGlobalCmd.h \
		  ./UmlTerminatePseudoState.h \
		  ./UmlComponentDiagram.h \
		  ./UmlBaseState.h \
		  ./UmlArtifact.h \
		  ./UmlObjectDiagram.h \
		  ./UmlForkPseudoState.h \
		  ./UmlBaseRegion.h \
		  ./UmlOperation.h \
		  ./UmlJoinPseudoState.h \
		  ./UmlActualParameter.h \
		  ./UmlActivityParameter.h \
		  ./anOrdering.h \
		  ./UmlBaseClass.h \
		  ./UmlDeploymentView.h \
		  ./UmlBaseDeploymentDiagram.h \
		  ./UmlBaseExpansionNode.h \
		  ./UmlComponentView.h \
		  ./UmlClassView.h \
		  ./UmlActivity.h \
		  ./UmlSettingsCmd.h \
		  ./UmlBaseActivityControlNode.h \
		  ./UmlBaseChoicePseudoState.h \
		  ./UmlBaseStateDiagram.h \
		  ./UmlInitialPseudoState.h \
		  ./UmlBaseActivityActionClasses.h \
		  ./UmlParameter.h \
		  ./UmlBaseOperation.h \
		  ./UmlBaseActivityControlNodeClasses.h \
		  ./UmlRegion.h \
		  ./UmlBaseActivityPin.h \
		  ./UmlActivityDiagram.h \
		  ./UmlBaseClassMember.h \
		  ./SlotRelation.h \
		  ./aVisibility.h \
		  ./UmlComponent.h \
		  ./UmlBaseActualParameter.h \
		  ./UmlPinParameter.h \
		  ./UmlFlow.h \
		  ./UmlUseCaseDiagram.h \
		  ./UmlBasePinParameter.h \
		  ./UmlBaseComponentDiagram.h \
		  ./UmlBaseDeploymentView.h \
		  ./UmlBaseClassItem.h \
		  ./StateBehavior.h \
		  ./UmlShallowHistoryPseudoState.h
SOURCES		= ./UmlBaseClassInstance.cpp \
		  ./UmlBaseUseCase.cpp \
		  ./UmlActivityObject.cpp \
		  ./UmlActivityRegion.cpp \
		  ./UmlBaseNode.cpp \
		  ./UmlActivityActionClasses.cpp \
		  ./UmlBaseComponent.cpp \
		  ./IdlSettings.cpp \
		  ./UmlBaseStateAction.cpp \
		  ./aRelationKind.cpp \
		  ./Association.cpp \
		  ./UmlClassMember.cpp \
		  ./UmlBaseSequenceDiagram.cpp \
		  ./UmlBaseUseCaseView.cpp \
		  ./UmlPackage.cpp \
		  ./UmlBaseNcRelation.cpp \
		  ./UmlBaseClassDiagram.cpp \
		  ./CppSettingsCmd.cpp \
		  ./UmlBaseActivityDiagram.cpp \
		  ./UmlBaseDeployment.cpp \
		  ./UmlBaseShallowHistoryPseudoState.cpp \
		  ./UmlExtraClassMember.cpp \
		  ./UmlBaseTransition.cpp \
		  ./UmlBaseEntryPointPseudoState.cpp \
		  ./UmlDeploymentDiagram.cpp \
		  ./UmlBaseOnSignalAction.cpp \
		  ./UmlBaseInterruptibleActivityRegion.cpp \
		  ./UmlFormalParameter.cpp \
		  ./UmlActivityNode.cpp \
		  ./aDirection.cpp \
		  ./UmlCom.cpp \
		  ./UmlActivityControlNodeClasses.cpp \
		  ./Sgnl.cpp \
		  ./UmlBaseClassView.cpp \
		  ./OnInstanceCmd.cpp \
		  ./UmlStateItem.cpp \
		  ./UmlEntryPointPseudoState.cpp \
		  ./UmlBaseArtifact.cpp \
		  ./UmlInterruptibleActivityRegion.cpp \
		  ./UmlBaseDeepHistoryPseudoState.cpp \
		  ./UmlBaseExtraClassMember.cpp \
		  ./UmlNode.cpp \
		  ./CppSettings.cpp \
		  ./anItemKind.cpp \
		  ./UmlBaseActivityNode.cpp \
		  ./UmlBaseForkPseudoState.cpp \
		  ./anExpansionKind.cpp \
		  ./UmlBaseComponentView.cpp \
		  ./UmlBaseFlow.cpp \
		  ./UmlStereotype.cpp \
		  ./UmlUseCase.cpp \
		  ./UmlTransition.cpp \
		  ./UmlPseudoState.cpp \
		  ./JavaSettingsCmd.cpp \
		  ./MiscGlobalCmd.cpp \
		  ./UmlStateAction.cpp \
		  ./UmlChoicePseudoState.cpp \
		  ./UmlCollaborationDiagram.cpp \
		  ./aLanguage.cpp \
		  ./UmlBaseActivityRegion.cpp \
		  ./UmlStateDiagram.cpp \
		  ./UmlParameterSet.cpp \
		  ./UmlBaseJoinPseudoState.cpp \
		  ./UmlActivityAction.cpp \
		  ./UmlActivityPin.cpp \
		  ./FlowBehavior.cpp \
		  ./CmdFamily.cpp \
		  ./UmlBasePackage.cpp \
		  ./UmlSequenceDiagram.cpp \
		  ./Token.cpp \
		  ./UmlBaseCollaborationDiagram.cpp \
		  ./ClassGlobalCmd.cpp \
		  ./UmlExpansionNode.cpp \
		  ./UmlBaseAccessVariableValueAction.cpp \
		  ./UmlClassDiagram.cpp \
		  ./UmlUseCaseView.cpp \
		  ./UmlBaseActivity.cpp \
		  ./UmlItem.cpp \
		  ./UmlDeepHistoryPseudoState.cpp \
		  ./FileIn.cpp \
		  ./JavaSettings.cpp \
		  ./UmlBaseParameterSet.cpp \
		  ./UmlExitPointPseudoState.cpp \
		  ./UmlBaseActivityAction.cpp \
		  ./UmlClass.cpp \
		  ./UmlBaseItem.cpp \
		  ./UmlActivityControlNode.cpp \
		  ./SlotAttribute.cpp \
		  ./UmlView.cpp \
		  ./UmlOnSignalAction.cpp \
		  ./UmlBaseActivityObject.cpp \
		  ./UmlJunctionPseudoState.cpp \
		  ./UmlBuiltin.cpp \
		  ./UmlBaseExpansionRegion.cpp \
		  ./UmlExpansionRegion.cpp \
		  ./UmlBaseUseCaseDiagram.cpp \
		  ./UmlBaseActivityParameter.cpp \
		  ./UmlBaseRelation.cpp \
		  ./UmlClassInstance.cpp \
		  ./UmlDiagram.cpp \
		  ./UmlRelation.cpp \
		  ./UmlBaseFinalState.cpp \
		  ./TransitionBehavior.cpp \
		  ./UmlBaseAttribute.cpp \
		  ./UmlFinalState.cpp \
		  ./UmlBaseJunctionPseudoState.cpp \
		  ./UmlBaseParameter.cpp \
		  ./UmlAccessVariableValueAction.cpp \
		  ./Main.cpp \
		  ./Trigger.cpp \
		  ./aParameterEffectKind.cpp \
		  ./UmlNcRelation.cpp \
		  ./UmlBaseExitPointPseudoState.cpp \
		  ./UmlBaseTerminatePseudoState.cpp \
		  ./UmlTypeSpec.cpp \
		  ./UmlBaseInitialPseudoState.cpp \
		  ./UmlClassItem.cpp \
		  ./UmlBaseFormalParameter.cpp \
		  ./UmlBaseObjectDiagram.cpp \
		  ./UmlAttribute.cpp \
		  ./UmlBaseDiagram.cpp \
		  ./UmlSettings.cpp \
		  ./UmlState.cpp \
		  ./UmlBaseTypeSpec.cpp \
		  ./UmlBaseView.cpp \
		  ./UmlActivityItem.cpp \
		  ./IdlSettingsCmd.cpp \
		  ./PackageGlobalCmd.cpp \
		  ./UmlTerminatePseudoState.cpp \
		  ./UmlComponentDiagram.cpp \
		  ./UmlBaseState.cpp \
		  ./UmlArtifact.cpp \
		  ./UmlObjectDiagram.cpp \
		  ./UmlForkPseudoState.cpp \
		  ./UmlBaseRegion.cpp \
		  ./UmlOperation.cpp \
		  ./UmlJoinPseudoState.cpp \
		  ./UmlActualParameter.cpp \
		  ./UmlActivityParameter.cpp \
		  ./anOrdering.cpp \
		  ./UmlBaseClass.cpp \
		  ./UmlDeploymentView.cpp \
		  ./UmlBaseDeploymentDiagram.cpp \
		  ./UmlBaseExpansionNode.cpp \
		  ./UmlComponentView.cpp \
		  ./UmlClassView.cpp \
		  ./UmlActivity.cpp \
		  ./UmlSettingsCmd.cpp \
		  ./UmlBaseActivityControlNode.cpp \
		  ./UmlBaseChoicePseudoState.cpp \
		  ./UmlBaseStateDiagram.cpp \
		  ./UmlInitialPseudoState.cpp \
		  ./UmlBaseActivityActionClasses.cpp \
		  ./UmlParameter.cpp \
		  ./UmlBaseOperation.cpp \
		  ./UmlBaseActivityControlNodeClasses.cpp \
		  ./UmlRegion.cpp \
		  ./UmlBaseActivityPin.cpp \
		  ./UmlActivityDiagram.cpp \
		  ./UmlBaseClassMember.cpp \
		  ./SlotRelation.cpp \
		  ./aVisibility.cpp \
		  ./UmlComponent.cpp \
		  ./UmlBaseActualParameter.cpp \
		  ./UmlPinParameter.cpp \
		  ./UmlFlow.cpp \
		  ./UmlUseCaseDiagram.cpp \
		  ./UmlBasePinParameter.cpp \
		  ./UmlBaseComponentDiagram.cpp \
		  ./UmlBaseDeploymentView.cpp \
		  ./UmlBaseClassItem.cpp \
		  ./StateBehavior.cpp \
		  ./UmlShallowHistoryPseudoState.cpp

