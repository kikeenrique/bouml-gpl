TEMPLATE	= app
TARGET		= gxmi2
CONFIG		+= release warn_on qt
DEFINES		= WITHCPP WITHJAVA WITHIDL
HEADERS		= ./SlotRelation.h \
		  ./UmlExpansionRegion.h \
		  ./UmlBaseUseCaseDiagram.h \
		  ./aDirection.h \
		  ./UmlBaseRelation.h \
		  ./UmlComponentView.h \
		  ./UmlBaseExtraClassMember.h \
		  ./UmlBaseClassInstance.h \
		  ./aLanguage.h \
		  ./UmlDeploymentView.h \
		  ./CppSettings.h \
		  ./UmlNode.h \
		  ./UmlBaseChoicePseudoState.h \
		  ./UmlStereotype.h \
		  ./UmlBaseFinalState.h \
		  ./TransitionBehavior.h \
		  ./UmlNcRelation.h \
		  ./UmlInitialPseudoState.h \
		  ./UmlBaseComponentView.h \
		  ./OnInstanceCmd.h \
		  ./UmlActivityObject.h \
		  ./UmlBaseInitialPseudoState.h \
		  ./UmlBaseExitPointPseudoState.h \
		  ./UmlBaseTerminatePseudoState.h \
		  ./UmlShallowHistoryPseudoState.h \
		  ./UmlBaseOnSignalAction.h \
		  ./UmlDiagram.h \
		  ./UmlBaseCollaborationDiagram.h \
		  ./UmlBaseState.h \
		  ./UmlActualParameter.h \
		  ./UmlBasePinParameter.h \
		  ./UmlJoinPseudoState.h \
		  ./UmlBaseActualParameter.h \
		  ./UmlUseCaseView.h \
		  ./UmlBaseExpansionNode.h \
		  ./UmlBaseItem.h \
		  ./anExpansionKind.h \
		  ./UmlBaseActivityRegion.h \
		  ./UmlBaseFlow.h \
		  ./UmlBaseRegion.h \
		  ./UmlDeploymentDiagram.h \
		  ./UmlJunctionPseudoState.h \
		  ./JavaSettingsCmd.h \
		  ./MiscGlobalCmd.h \
		  ./UmlEntryPointPseudoState.h \
		  ./UmlBaseInterruptibleActivityRegion.h \
		  ./UmlBaseStateDiagram.h \
		  ./aVisibility.h \
		  ./UmlArtifact.h \
		  ./CmdFamily.h \
		  ./UmlInterruptibleActivityRegion.h \
		  ./UmlExtraClassMember.h \
		  ./UmlUseCaseDiagram.h \
		  ./StateBehavior.h \
		  ./UmlBaseActivity.h \
		  ./UmlBaseForkPseudoState.h \
		  ./ClassGlobalCmd.h \
		  ./UmlPinParameter.h \
		  ./UmlRegion.h \
		  ./UmlBaseActivityPin.h \
		  ./aRelationKind.h \
		  ./UmlBaseActivityDiagram.h \
		  ./UmlBaseStateAction.h \
		  ./UmlBaseUseCase.h \
		  ./UmlStateItem.h \
		  ./UmlBaseDiagram.h \
		  ./UmlActivityParameter.h \
		  ./UmlBasePackage.h \
		  ./UmlActivity.h \
		  ./UmlActivityActionClasses.h \
		  ./SmallPushButton.h \
		  ./UmlDeepHistoryPseudoState.h \
		  ./UmlBaseNcRelation.h \
		  ./UmlItem.h \
		  ./UmlActivityRegion.h \
		  ./UmlAccessVariableValueAction.h \
		  ./UmlActivityDiagram.h \
		  ./JavaSettings.h \
		  ./FlowBehavior.h \
		  ./UmlBaseUseCaseView.h \
		  ./UmlBaseClass.h \
		  ./UmlBaseDeploymentDiagram.h \
		  ./UmlActivityNode.h \
		  ./UmlObjectDiagram.h \
		  ./UmlActivityPin.h \
		  ./UmlTerminatePseudoState.h \
		  ./UmlClass.h \
		  ./UmlBaseActivityNode.h \
		  ./UmlBaseEntryPointPseudoState.h \
		  ./UmlBaseTransition.h \
		  ./UmlActivityControlNode.h \
		  ./UmlBaseClassItem.h \
		  ./UmlParameterSet.h \
		  ./UmlSequenceDiagram.h \
		  ./FileOut.h \
		  ./UmlBaseClassMember.h \
		  ./UmlActivityItem.h \
		  ./UmlOperation.h \
		  ./UmlFlow.h \
		  ./UmlBaseParameterSet.h \
		  ./UmlBaseActivityControlNodeClasses.h \
		  ./anItemKind.h \
		  ./UmlBaseOperation.h \
		  ./SlotAttribute.h \
		  ./UmlBaseActivityObject.h \
		  ./IdlSettingsCmd.h \
		  ./PackageGlobalCmd.h \
		  ./UmlExitPointPseudoState.h \
		  ./UmlStateAction.h \
		  ./UmlBaseSequenceDiagram.h \
		  ./UmlBaseAttribute.h \
		  ./UmlAttribute.h \
		  ./UmlFinalState.h \
		  ./UmlBaseDeploymentView.h \
		  ./UmlBaseShallowHistoryPseudoState.h \
		  ./UmlBaseDeepHistoryPseudoState.h \
		  ./UmlBaseComponentDiagram.h \
		  ./UmlBaseActivityAction.h \
		  ./UmlBaseActivityParameter.h \
		  ./UmlRelation.h \
		  ./UmlClassInstance.h \
		  ./UmlBaseNode.h \
		  ./UmlBaseComponent.h \
		  ./UmlClassItem.h \
		  ./UmlTransition.h \
		  ./UmlChoicePseudoState.h \
		  ./UmlPseudoState.h \
		  ./UmlSettings.h \
		  ./UmlUseCase.h \
		  ./UmlBaseExpansionRegion.h \
		  ./UmlSettingsCmd.h \
		  ./aParameterEffectKind.h \
		  ./UmlForkPseudoState.h \
		  ./UmlBaseClassDiagram.h \
		  ./UmlComponentDiagram.h \
		  ./UmlActivityAction.h \
		  ./Language.h \
		  ./UmlBuiltin.h \
		  ./UmlBaseJunctionPseudoState.h \
		  ./UmlStateDiagram.h \
		  ./UmlComponent.h \
		  ./UmlPackage.h \
		  ./UmlFormalParameter.h \
		  ./UmlBaseDeployment.h \
		  ./UmlBaseAccessVariableValueAction.h \
		  ./UmlOnSignalAction.h \
		  ./UmlActivityControlNodeClasses.h \
		  ./UmlBaseFormalParameter.h \
		  ./UmlCom.h \
		  ./UmlClassMember.h \
		  ./UmlBaseJoinPseudoState.h \
		  ./UmlBaseObjectDiagram.h \
		  ./anOrdering.h \
		  ./UmlBaseClassView.h \
		  ./UmlCollaborationDiagram.h \
		  ./FlowContainer.h \
		  ./UmlExpansionNode.h \
		  ./UmlClassView.h \
		  ./IdlSettings.h \
		  ./UmlBaseArtifact.h \
		  ./UmlBaseActivityControlNode.h \
		  ./UmlBaseActivityActionClasses.h \
		  ./UmlParameter.h \
		  ./Dialog.h \
		  ./UmlState.h \
		  ./UmlClassDiagram.h \
		  ./UmlTypeSpec.h \
		  ./CppSettingsCmd.h
SOURCES		= ./SlotRelation.cpp \
		  ./UmlExpansionRegion.cpp \
		  ./UmlBaseUseCaseDiagram.cpp \
		  ./aDirection.cpp \
		  ./UmlBaseRelation.cpp \
		  ./UmlComponentView.cpp \
		  ./UmlBaseExtraClassMember.cpp \
		  ./UmlBaseClassInstance.cpp \
		  ./aLanguage.cpp \
		  ./UmlDeploymentView.cpp \
		  ./CppSettings.cpp \
		  ./UmlNode.cpp \
		  ./UmlBaseChoicePseudoState.cpp \
		  ./UmlStereotype.cpp \
		  ./UmlBaseFinalState.cpp \
		  ./TransitionBehavior.cpp \
		  ./UmlNcRelation.cpp \
		  ./UmlInitialPseudoState.cpp \
		  ./UmlBaseComponentView.cpp \
		  ./OnInstanceCmd.cpp \
		  ./UmlActivityObject.cpp \
		  ./UmlBaseInitialPseudoState.cpp \
		  ./UmlBaseExitPointPseudoState.cpp \
		  ./UmlBaseTerminatePseudoState.cpp \
		  ./UmlShallowHistoryPseudoState.cpp \
		  ./UmlBaseOnSignalAction.cpp \
		  ./UmlDiagram.cpp \
		  ./UmlBaseCollaborationDiagram.cpp \
		  ./UmlBaseState.cpp \
		  ./UmlActualParameter.cpp \
		  ./UmlBasePinParameter.cpp \
		  ./UmlJoinPseudoState.cpp \
		  ./UmlBaseActualParameter.cpp \
		  ./UmlUseCaseView.cpp \
		  ./UmlBaseExpansionNode.cpp \
		  ./UmlBaseItem.cpp \
		  ./anExpansionKind.cpp \
		  ./UmlBaseActivityRegion.cpp \
		  ./UmlBaseFlow.cpp \
		  ./UmlBaseRegion.cpp \
		  ./UmlDeploymentDiagram.cpp \
		  ./UmlJunctionPseudoState.cpp \
		  ./JavaSettingsCmd.cpp \
		  ./MiscGlobalCmd.cpp \
		  ./UmlEntryPointPseudoState.cpp \
		  ./UmlBaseInterruptibleActivityRegion.cpp \
		  ./UmlBaseStateDiagram.cpp \
		  ./aVisibility.cpp \
		  ./UmlArtifact.cpp \
		  ./CmdFamily.cpp \
		  ./UmlInterruptibleActivityRegion.cpp \
		  ./UmlExtraClassMember.cpp \
		  ./UmlUseCaseDiagram.cpp \
		  ./StateBehavior.cpp \
		  ./UmlBaseActivity.cpp \
		  ./UmlBaseForkPseudoState.cpp \
		  ./ClassGlobalCmd.cpp \
		  ./UmlPinParameter.cpp \
		  ./UmlRegion.cpp \
		  ./UmlBaseActivityPin.cpp \
		  ./aRelationKind.cpp \
		  ./UmlBaseActivityDiagram.cpp \
		  ./UmlBaseStateAction.cpp \
		  ./UmlBaseUseCase.cpp \
		  ./UmlStateItem.cpp \
		  ./UmlBaseDiagram.cpp \
		  ./UmlActivityParameter.cpp \
		  ./UmlBasePackage.cpp \
		  ./UmlActivity.cpp \
		  ./UmlActivityActionClasses.cpp \
		  ./SmallPushButton.cpp \
		  ./UmlDeepHistoryPseudoState.cpp \
		  ./UmlBaseNcRelation.cpp \
		  ./UmlItem.cpp \
		  ./UmlActivityRegion.cpp \
		  ./UmlAccessVariableValueAction.cpp \
		  ./Main.cpp \
		  ./UmlActivityDiagram.cpp \
		  ./JavaSettings.cpp \
		  ./FlowBehavior.cpp \
		  ./UmlBaseUseCaseView.cpp \
		  ./UmlBaseClass.cpp \
		  ./UmlBaseDeploymentDiagram.cpp \
		  ./UmlActivityNode.cpp \
		  ./UmlObjectDiagram.cpp \
		  ./UmlActivityPin.cpp \
		  ./UmlTerminatePseudoState.cpp \
		  ./UmlClass.cpp \
		  ./UmlBaseActivityNode.cpp \
		  ./UmlBaseEntryPointPseudoState.cpp \
		  ./UmlBaseTransition.cpp \
		  ./UmlActivityControlNode.cpp \
		  ./UmlBaseClassItem.cpp \
		  ./UmlParameterSet.cpp \
		  ./UmlSequenceDiagram.cpp \
		  ./FileOut.cpp \
		  ./UmlBaseClassMember.cpp \
		  ./UmlActivityItem.cpp \
		  ./UmlOperation.cpp \
		  ./UmlFlow.cpp \
		  ./UmlBaseParameterSet.cpp \
		  ./UmlBaseActivityControlNodeClasses.cpp \
		  ./anItemKind.cpp \
		  ./UmlBaseOperation.cpp \
		  ./SlotAttribute.cpp \
		  ./UmlBaseActivityObject.cpp \
		  ./IdlSettingsCmd.cpp \
		  ./PackageGlobalCmd.cpp \
		  ./UmlExitPointPseudoState.cpp \
		  ./UmlStateAction.cpp \
		  ./UmlBaseSequenceDiagram.cpp \
		  ./UmlBaseAttribute.cpp \
		  ./UmlAttribute.cpp \
		  ./UmlFinalState.cpp \
		  ./UmlBaseDeploymentView.cpp \
		  ./UmlBaseShallowHistoryPseudoState.cpp \
		  ./UmlBaseDeepHistoryPseudoState.cpp \
		  ./UmlBaseComponentDiagram.cpp \
		  ./UmlBaseActivityAction.cpp \
		  ./UmlBaseActivityParameter.cpp \
		  ./UmlRelation.cpp \
		  ./UmlClassInstance.cpp \
		  ./UmlBaseNode.cpp \
		  ./UmlBaseComponent.cpp \
		  ./UmlClassItem.cpp \
		  ./UmlTransition.cpp \
		  ./UmlChoicePseudoState.cpp \
		  ./UmlPseudoState.cpp \
		  ./UmlSettings.cpp \
		  ./UmlUseCase.cpp \
		  ./UmlBaseExpansionRegion.cpp \
		  ./UmlSettingsCmd.cpp \
		  ./aParameterEffectKind.cpp \
		  ./UmlForkPseudoState.cpp \
		  ./UmlBaseClassDiagram.cpp \
		  ./UmlComponentDiagram.cpp \
		  ./UmlActivityAction.cpp \
		  ./Language.cpp \
		  ./UmlBuiltin.cpp \
		  ./UmlBaseJunctionPseudoState.cpp \
		  ./UmlStateDiagram.cpp \
		  ./UmlComponent.cpp \
		  ./UmlPackage.cpp \
		  ./UmlFormalParameter.cpp \
		  ./UmlBaseDeployment.cpp \
		  ./UmlBaseAccessVariableValueAction.cpp \
		  ./UmlOnSignalAction.cpp \
		  ./UmlActivityControlNodeClasses.cpp \
		  ./UmlBaseFormalParameter.cpp \
		  ./UmlCom.cpp \
		  ./UmlClassMember.cpp \
		  ./UmlBaseJoinPseudoState.cpp \
		  ./UmlBaseObjectDiagram.cpp \
		  ./anOrdering.cpp \
		  ./UmlBaseClassView.cpp \
		  ./UmlCollaborationDiagram.cpp \
		  ./FlowContainer.cpp \
		  ./UmlExpansionNode.cpp \
		  ./UmlClassView.cpp \
		  ./IdlSettings.cpp \
		  ./UmlBaseArtifact.cpp \
		  ./UmlBaseActivityControlNode.cpp \
		  ./UmlBaseActivityActionClasses.cpp \
		  ./UmlParameter.cpp \
		  ./Dialog.cpp \
		  ./UmlState.cpp \
		  ./UmlClassDiagram.cpp \
		  ./UmlTypeSpec.cpp \
		  ./CppSettingsCmd.cpp
