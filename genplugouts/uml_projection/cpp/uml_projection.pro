TEMPLATE	= app
TARGET		= uml_proj
CONFIG		+= release warn_on qt
DEFINES		= WITHCPP WITHJAVA WITHIDL WITHPHP
HEADERS		= ./UmlBaseTransition.h \
		  ./UmlBaseFragmentCompartment.h \
		  ./UmlParameter.h \
		  ./UmlUseCaseDiagramDefinition.h \
		  ./UmlBaseEntryPointPseudoState.h \
		  ./UmlBaseNode.h \
		  ./IdlSettingsCmd.h \
		  ./UmlBaseClassItem.h \
		  ./UmlTypeSpec.h \
		  ./UmlActivity.h \
		  ./UmlBaseClassMember.h \
		  ./UmlArtifact.h \
		  ./UmlUseCaseView.h \
		  ./UmlBaseAccessVariableValueAction.h \
		  ./UmlBaseObjectDiagram.h \
		  ./UmlBaseActivityRegion.h \
		  ./aDirection.h \
		  ./aVisibility.h \
		  ./UmlBaseOperation.h \
		  ./UmlObjectDiagram.h \
		  ./UmlActivityControlNode.h \
		  ./UmlBaseUseCaseAssociation.h \
		  ./UmlClass.h \
		  ./UmlUseCaseAssociation.h \
		  ./UmlBaseOnSignalAction.h \
		  ./UmlUseCaseReference.h \
		  ./UmlBaseDeploymentDiagram.h \
		  ./UmlOperation.h \
		  ./PackageGlobalCmd.h \
		  ./UmlDeepHistoryPseudoState.h \
		  ./UmlBaseActualParameter.h \
		  ./UmlBaseUseCaseDiagramDefinition.h \
		  ./UmlActivityActionClasses.h \
		  ./UmlBaseFlow.h \
		  ./UmlBaseSequenceDiagram.h \
		  ./UmlBaseActivityControlNodeClasses.h \
		  ./UmlBaseDeploymentView.h \
		  ./UmlSettingsCmd.h \
		  ./PhpSettings.h \
		  ./UmlCollaborationMessage.h \
		  ./aLanguage.h \
		  ./UmlBaseActivityAction.h \
		  ./UmlActivityNode.h \
		  ./UmlExpansionRegion.h \
		  ./UmlBasePinParameter.h \
		  ./UmlCom.h \
		  ./UmlActivityItem.h \
		  ./UmlDeploymentDiagram.h \
		  ./UmlFlow.h \
		  ./UmlStereotype.h \
		  ./UmlBaseJoinPseudoState.h \
		  ./anItemKind.h \
		  ./UmlActivityAction.h \
		  ./SlotRelation.h \
		  ./CppSettingsCmd.h \
		  ./UmlExtraClassMember.h \
		  ./UmlItem.h \
		  ./UmlBaseClassView.h \
		  ./UmlNode.h \
		  ./FlowBehavior.h \
		  ./UmlStateDiagram.h \
		  ./UmlBaseChoicePseudoState.h \
		  ./UmlBaseAttribute.h \
		  ./UmlState.h \
		  ./UmlNcRelation.h \
		  ./UmlBaseSubject.h \
		  ./UmlStateItem.h \
		  ./UmlBaseUseCaseDiagram.h \
		  ./aMessageKind.h \
		  ./UmlBaseExitPointPseudoState.h \
		  ./UmlBaseExtraClassMember.h \
		  ./UmlTransition.h \
		  ./UmlClassInstance.h \
		  ./UmlSequenceDiagram.h \
		  ./UmlShallowHistoryPseudoState.h \
		  ./CppSettings.h \
		  ./UmlBaseSequenceDiagramDefinition.h \
		  ./UmlBaseState.h \
		  ./UmlBaseParameter.h \
		  ./UmlBaseExpansionRegion.h \
		  ./UmlComponentView.h \
		  ./UmlBaseActivityControlNode.h \
		  ./UmlBaseActivity.h \
		  ./UmlBaseActivityDiagram.h \
		  ./UmlBaseFinalState.h \
		  ./UmlBaseTerminatePseudoState.h \
		  ./UmlBaseDeployment.h \
		  ./UmlBasePackage.h \
		  ./JavaSettings.h \
		  ./UmlFinalState.h \
		  ./anOrdering.h \
		  ./UmlBaseClassInstance.h \
		  ./aParameterEffectKind.h \
		  ./UmlView.h \
		  ./UmlBaseClass.h \
		  ./UmlSettings.h \
		  ./UmlExitPointPseudoState.h \
		  ./UmlBaseFormalParameter.h \
		  ./UmlRelation.h \
		  ./UmlClassItem.h \
		  ./UmlBaseRegion.h \
		  ./UmlBaseFragment.h \
		  ./UmlJunctionPseudoState.h \
		  ./TransitionBehavior.h \
		  ./UmlBaseActivityNode.h \
		  ./UmlParameterSet.h \
		  ./UmlTerminatePseudoState.h \
		  ./JavaSettingsCmd.h \
		  ./UmlClassInstanceReference.h \
		  ./UmlBaseClassInstanceReference.h \
		  ./UmlBaseCollaborationDiagram.h \
		  ./UmlCollaborationDiagramDefinition.h \
		  ./UmlBaseView.h \
		  ./UmlInterruptibleActivityRegion.h \
		  ./UmlActivityDiagram.h \
		  ./CmdFamily.h \
		  ./UmlAttribute.h \
		  ./UmlBaseStateDiagram.h \
		  ./UmlBaseItem.h \
		  ./UmlActivityObject.h \
		  ./UmlBaseComponent.h \
		  ./UmlBaseClassDiagram.h \
		  ./UmlBaseTypeSpec.h \
		  ./UmlBaseStateAction.h \
		  ./UmlPinParameter.h \
		  ./UmlSubject.h \
		  ./UmlActivityPin.h \
		  ./UmlAccessVariableValueAction.h \
		  ./UmlActivityRegion.h \
		  ./UmlBuiltin.h \
		  ./UmlFormalParameter.h \
		  ./OnInstanceCmd.h \
		  ./UmlFragment.h \
		  ./UmlDeploymentView.h \
		  ./UmlRegion.h \
		  ./UmlCollaborationDiagram.h \
		  ./UmlOnSignalAction.h \
		  ./UmlBaseSequenceMessage.h \
		  ./UmlBaseComponentDiagram.h \
		  ./UmlActivityParameter.h \
		  ./UmlPackage.h \
		  ./UmlFragmentCompartment.h \
		  ./UmlUseCaseDiagram.h \
		  ./UmlClassMember.h \
		  ./UmlPseudoState.h \
		  ./UmlInitialPseudoState.h \
		  ./UmlBaseActivityActionClasses.h \
		  ./UmlBaseActivityPin.h \
		  ./UmlBaseActivityParameter.h \
		  ./UmlBaseActivityObject.h \
		  ./SlotAttribute.h \
		  ./UmlBaseCollaborationDiagramDefinition.h \
		  ./UmlBaseRelation.h \
		  ./UmlExpansionNode.h \
		  ./UmlSequenceMessage.h \
		  ./UmlJoinPseudoState.h \
		  ./UmlBaseUseCaseView.h \
		  ./UmlSequenceDiagramDefinition.h \
		  ./UmlActualParameter.h \
		  ./UmlBaseCollaborationMessage.h \
		  ./UmlBaseNcRelation.h \
		  ./aRelationKind.h \
		  ./UmlEntryPointPseudoState.h \
		  ./UmlBaseComponentView.h \
		  ./UmlBaseParameterSet.h \
		  ./UmlForkPseudoState.h \
		  ./UmlComponentDiagram.h \
		  ./UmlDiagram.h \
		  ./IdlSettings.h \
		  ./UmlMessage.h \
		  ./UmlBaseShallowHistoryPseudoState.h \
		  ./UmlBaseForkPseudoState.h \
		  ./UmlBaseArtifact.h \
		  ./Dialog.h \
		  ./UmlBaseUseCaseReference.h \
		  ./MiscGlobalCmd.h \
		  ./StateBehavior.h \
		  ./UmlUseCase.h \
		  ./UmlBaseUseCase.h \
		  ./UmlComponent.h \
		  ./UmlBaseMessage.h \
		  ./UmlBaseDeepHistoryPseudoState.h \
		  ./UmlBaseJunctionPseudoState.h \
		  ./UmlStateAction.h \
		  ./UmlBaseDiagram.h \
		  ./ClassGlobalCmd.h \
		  ./PhpSettingsCmd.h \
		  ./UmlBaseExpansionNode.h \
		  ./UmlChoicePseudoState.h \
		  ./UmlClassView.h \
		  ./UmlClassDiagram.h \
		  ./anExpansionKind.h \
		  ./UmlActivityControlNodeClasses.h \
		  ./UmlBaseInterruptibleActivityRegion.h \
		  ./UmlBaseInitialPseudoState.h
SOURCES		= ./UmlBaseTransition.cpp \
		  ./UmlBaseFragmentCompartment.cpp \
		  ./UmlParameter.cpp \
		  ./UmlUseCaseDiagramDefinition.cpp \
		  ./UmlBaseEntryPointPseudoState.cpp \
		  ./UmlBaseNode.cpp \
		  ./IdlSettingsCmd.cpp \
		  ./UmlBaseClassItem.cpp \
		  ./UmlTypeSpec.cpp \
		  ./UmlActivity.cpp \
		  ./UmlBaseClassMember.cpp \
		  ./UmlArtifact.cpp \
		  ./UmlUseCaseView.cpp \
		  ./UmlBaseAccessVariableValueAction.cpp \
		  ./UmlBaseObjectDiagram.cpp \
		  ./UmlBaseActivityRegion.cpp \
		  ./aDirection.cpp \
		  ./aVisibility.cpp \
		  ./UmlBaseOperation.cpp \
		  ./UmlObjectDiagram.cpp \
		  ./UmlActivityControlNode.cpp \
		  ./UmlBaseUseCaseAssociation.cpp \
		  ./UmlClass.cpp \
		  ./UmlUseCaseAssociation.cpp \
		  ./UmlBaseOnSignalAction.cpp \
		  ./UmlUseCaseReference.cpp \
		  ./UmlBaseDeploymentDiagram.cpp \
		  ./UmlOperation.cpp \
		  ./PackageGlobalCmd.cpp \
		  ./UmlDeepHistoryPseudoState.cpp \
		  ./UmlBaseActualParameter.cpp \
		  ./UmlBaseUseCaseDiagramDefinition.cpp \
		  ./UmlActivityActionClasses.cpp \
		  ./UmlBaseFlow.cpp \
		  ./UmlBaseSequenceDiagram.cpp \
		  ./UmlBaseActivityControlNodeClasses.cpp \
		  ./UmlBaseDeploymentView.cpp \
		  ./UmlSettingsCmd.cpp \
		  ./PhpSettings.cpp \
		  ./UmlCollaborationMessage.cpp \
		  ./aLanguage.cpp \
		  ./UmlBaseActivityAction.cpp \
		  ./UmlActivityNode.cpp \
		  ./UmlExpansionRegion.cpp \
		  ./UmlBasePinParameter.cpp \
		  ./UmlCom.cpp \
		  ./UmlActivityItem.cpp \
		  ./UmlDeploymentDiagram.cpp \
		  ./UmlFlow.cpp \
		  ./UmlStereotype.cpp \
		  ./UmlBaseJoinPseudoState.cpp \
		  ./anItemKind.cpp \
		  ./UmlActivityAction.cpp \
		  ./SlotRelation.cpp \
		  ./CppSettingsCmd.cpp \
		  ./UmlExtraClassMember.cpp \
		  ./UmlItem.cpp \
		  ./UmlBaseClassView.cpp \
		  ./UmlNode.cpp \
		  ./FlowBehavior.cpp \
		  ./UmlStateDiagram.cpp \
		  ./UmlBaseChoicePseudoState.cpp \
		  ./UmlBaseAttribute.cpp \
		  ./UmlState.cpp \
		  ./UmlNcRelation.cpp \
		  ./UmlBaseSubject.cpp \
		  ./UmlStateItem.cpp \
		  ./UmlBaseUseCaseDiagram.cpp \
		  ./aMessageKind.cpp \
		  ./UmlBaseExitPointPseudoState.cpp \
		  ./UmlBaseExtraClassMember.cpp \
		  ./UmlTransition.cpp \
		  ./UmlClassInstance.cpp \
		  ./UmlSequenceDiagram.cpp \
		  ./UmlShallowHistoryPseudoState.cpp \
		  ./CppSettings.cpp \
		  ./UmlBaseSequenceDiagramDefinition.cpp \
		  ./UmlBaseState.cpp \
		  ./UmlBaseParameter.cpp \
		  ./UmlBaseExpansionRegion.cpp \
		  ./UmlComponentView.cpp \
		  ./UmlBaseActivityControlNode.cpp \
		  ./UmlBaseActivity.cpp \
		  ./UmlBaseActivityDiagram.cpp \
		  ./UmlBaseFinalState.cpp \
		  ./UmlBaseTerminatePseudoState.cpp \
		  ./UmlBaseDeployment.cpp \
		  ./UmlBasePackage.cpp \
		  ./JavaSettings.cpp \
		  ./UmlFinalState.cpp \
		  ./anOrdering.cpp \
		  ./UmlBaseClassInstance.cpp \
		  ./aParameterEffectKind.cpp \
		  ./UmlView.cpp \
		  ./UmlBaseClass.cpp \
		  ./UmlSettings.cpp \
		  ./UmlExitPointPseudoState.cpp \
		  ./UmlBaseFormalParameter.cpp \
		  ./UmlRelation.cpp \
		  ./UmlClassItem.cpp \
		  ./UmlBaseRegion.cpp \
		  ./UmlBaseFragment.cpp \
		  ./UmlJunctionPseudoState.cpp \
		  ./TransitionBehavior.cpp \
		  ./UmlBaseActivityNode.cpp \
		  ./UmlParameterSet.cpp \
		  ./UmlTerminatePseudoState.cpp \
		  ./JavaSettingsCmd.cpp \
		  ./UmlClassInstanceReference.cpp \
		  ./UmlBaseClassInstanceReference.cpp \
		  ./UmlBaseCollaborationDiagram.cpp \
		  ./UmlCollaborationDiagramDefinition.cpp \
		  ./UmlBaseView.cpp \
		  ./UmlInterruptibleActivityRegion.cpp \
		  ./UmlActivityDiagram.cpp \
		  ./CmdFamily.cpp \
		  ./UmlAttribute.cpp \
		  ./UmlBaseStateDiagram.cpp \
		  ./UmlBaseItem.cpp \
		  ./UmlActivityObject.cpp \
		  ./UmlBaseComponent.cpp \
		  ./UmlBaseClassDiagram.cpp \
		  ./Main.cpp \
		  ./UmlBaseTypeSpec.cpp \
		  ./UmlBaseStateAction.cpp \
		  ./UmlPinParameter.cpp \
		  ./UmlSubject.cpp \
		  ./UmlActivityPin.cpp \
		  ./UmlAccessVariableValueAction.cpp \
		  ./UmlActivityRegion.cpp \
		  ./UmlBuiltin.cpp \
		  ./UmlFormalParameter.cpp \
		  ./OnInstanceCmd.cpp \
		  ./UmlFragment.cpp \
		  ./UmlDeploymentView.cpp \
		  ./UmlRegion.cpp \
		  ./UmlCollaborationDiagram.cpp \
		  ./UmlOnSignalAction.cpp \
		  ./UmlBaseSequenceMessage.cpp \
		  ./UmlBaseComponentDiagram.cpp \
		  ./UmlActivityParameter.cpp \
		  ./UmlPackage.cpp \
		  ./UmlFragmentCompartment.cpp \
		  ./UmlUseCaseDiagram.cpp \
		  ./UmlClassMember.cpp \
		  ./UmlPseudoState.cpp \
		  ./UmlInitialPseudoState.cpp \
		  ./UmlBaseActivityActionClasses.cpp \
		  ./UmlBaseActivityPin.cpp \
		  ./UmlBaseActivityParameter.cpp \
		  ./UmlBaseActivityObject.cpp \
		  ./SlotAttribute.cpp \
		  ./UmlBaseCollaborationDiagramDefinition.cpp \
		  ./UmlBaseRelation.cpp \
		  ./UmlExpansionNode.cpp \
		  ./UmlSequenceMessage.cpp \
		  ./UmlJoinPseudoState.cpp \
		  ./UmlBaseUseCaseView.cpp \
		  ./UmlSequenceDiagramDefinition.cpp \
		  ./UmlActualParameter.cpp \
		  ./UmlBaseCollaborationMessage.cpp \
		  ./UmlBaseNcRelation.cpp \
		  ./aRelationKind.cpp \
		  ./UmlEntryPointPseudoState.cpp \
		  ./UmlBaseComponentView.cpp \
		  ./UmlBaseParameterSet.cpp \
		  ./UmlForkPseudoState.cpp \
		  ./UmlComponentDiagram.cpp \
		  ./UmlDiagram.cpp \
		  ./IdlSettings.cpp \
		  ./UmlMessage.cpp \
		  ./UmlBaseShallowHistoryPseudoState.cpp \
		  ./UmlBaseForkPseudoState.cpp \
		  ./UmlBaseArtifact.cpp \
		  ./Dialog.cpp \
		  ./UmlBaseUseCaseReference.cpp \
		  ./MiscGlobalCmd.cpp \
		  ./StateBehavior.cpp \
		  ./UmlUseCase.cpp \
		  ./UmlBaseUseCase.cpp \
		  ./UmlComponent.cpp \
		  ./UmlBaseMessage.cpp \
		  ./UmlBaseDeepHistoryPseudoState.cpp \
		  ./UmlBaseJunctionPseudoState.cpp \
		  ./UmlStateAction.cpp \
		  ./UmlBaseDiagram.cpp \
		  ./ClassGlobalCmd.cpp \
		  ./PhpSettingsCmd.cpp \
		  ./UmlBaseExpansionNode.cpp \
		  ./UmlChoicePseudoState.cpp \
		  ./UmlClassView.cpp \
		  ./UmlClassDiagram.cpp \
		  ./anExpansionKind.cpp \
		  ./UmlActivityControlNodeClasses.cpp \
		  ./UmlBaseInterruptibleActivityRegion.cpp \
		  ./UmlBaseInitialPseudoState.cpp

