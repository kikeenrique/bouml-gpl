#ifndef UMLPIXMAP_H
#define UMLPIXMAP_H

#include <qpixmap.h>

extern void init_pixmaps();

// toolbar buttons

extern QPixmap * noteButton;
extern QPixmap * textButton;
extern QPixmap * actorButton;
extern QPixmap * classButton;
extern QPixmap * anchorButton;
extern QPixmap * selectButton;
extern QPixmap * usecaseButton;
extern QPixmap * packageButton;
extern QPixmap * fragmentButton;
extern QPixmap * subjectButton;
extern QPixmap * continuationButton;
extern QPixmap * componentButton;
extern QPixmap * artifactButton;
extern QPixmap * deploymentNodeButton;
extern QPixmap * hubButton;
extern QPixmap * generalisationButton;
extern QPixmap * associationButton;
extern QPixmap * directionalAssociationButton;
extern QPixmap * aggregationButton;
extern QPixmap * aggregationByValueButton;
extern QPixmap * directionalAggregationButton;
extern QPixmap * directionalAggregationByValueButton;
extern QPixmap * dependencyButton;
extern QPixmap * realizeButton;
extern QPixmap * syncmsgButton;
extern QPixmap * syncselfmsgButton;
extern QPixmap * asyncselfmsgButton;
extern QPixmap * returnmsgButton;
extern QPixmap * selflinkButton;
extern QPixmap * fitscaleButton;
extern QPixmap * optwindowsizeButton;
extern QPixmap * editButton;
extern QPixmap * providedButton;
extern QPixmap * requiredButton;
extern QPixmap * stateButton;
extern QPixmap * entrypointButton;
extern QPixmap * exitpointButton;
extern QPixmap * initialButton;
extern QPixmap * finalButton;
extern QPixmap * terminateButton;
extern QPixmap * deephistoryButton;
extern QPixmap * shallowhistoryButton;
extern QPixmap * junctionButton;
extern QPixmap * choiceButton;
extern QPixmap * forkButton;
extern QPixmap * joinButton;
extern QPixmap * regionButton;
extern QPixmap * actionButton;
extern QPixmap * signalinButton;
extern QPixmap * signaloutButton;
extern QPixmap * activityButton;
extern QPixmap * interruptibleactivityregionButton;
extern QPixmap * activityactionButton;
extern QPixmap * decisionButton;
extern QPixmap * mergeButton;
extern QPixmap * pinButton;
extern QPixmap * expansionregionButton;
extern QPixmap * expansionnodeButton;

// browser icons

extern QPixmap * ActorIcon;
extern QPixmap * DeletedActorIcon;
extern QPixmap * ClassIcon;
extern QPixmap * DeletedClassIcon;
extern QPixmap * TemplateIcon;
extern QPixmap * DeletedTemplateIcon;
extern QPixmap * PublicEmbeddedClassIcon;
extern QPixmap * ProtectedEmbeddedClassIcon;
extern QPixmap * PrivateEmbeddedClassIcon;
extern QPixmap * PackageEmbeddedClassIcon;
extern QPixmap * PublicEmbeddedTemplateIcon;
extern QPixmap * ProtectedEmbeddedTemplateIcon;
extern QPixmap * PrivateEmbeddedTemplateIcon;
extern QPixmap * PackageEmbeddedTemplateIcon;
extern QPixmap * UseCaseIcon;
extern QPixmap * DeletedUseCaseIcon;
extern QPixmap * ComponentIcon;
extern QPixmap * DeletedComponentIcon;
extern QPixmap * ArtifactIcon;
extern QPixmap * DeletedArtifactIcon;
extern QPixmap * DeploymentNodeIcon;
extern QPixmap * DeletedDeploymentNodeIcon;
extern QPixmap * PackageIcon;
extern QPixmap * DeletedPackageIcon;
extern QPixmap * SeqDiagramIcon;
extern QPixmap * DeletedSeqDiagramIcon;
extern QPixmap * ColDiagramIcon;
extern QPixmap * DeletedColDiagramIcon;
extern QPixmap * ObjectDiagramIcon;
extern QPixmap * DeletedObjectDiagramIcon;
extern QPixmap * ActDiagramIcon;
extern QPixmap * DeletedActDiagramIcon;
extern QPixmap * ClassViewIcon;
extern QPixmap * ComponentDiagramIcon;
extern QPixmap * DeletedComponentDiagramIcon;
extern QPixmap * DeploymentDiagramIcon;
extern QPixmap * DeletedDeploymentDiagramIcon;
extern QPixmap * DeletedClassViewIcon;
extern QPixmap * ClassDiagramIcon;
extern QPixmap * DeletedClassDiagramIcon;
extern QPixmap * UseCaseViewIcon;
extern QPixmap * DeletedUseCaseViewIcon;
extern QPixmap * UseCaseDiagramIcon;
extern QPixmap * DeletedUseCaseDiagramIcon;
extern QPixmap * ComponentViewIcon;
extern QPixmap * DeletedComponentViewIcon;
extern QPixmap * DeploymentViewIcon;
extern QPixmap * DeletedDeploymentViewIcon;
extern QPixmap * PublicAttributeIcon;
extern QPixmap * PrivateAttributeIcon;
extern QPixmap * PackageAttributeIcon;
extern QPixmap * ProtectedAttributeIcon;
extern QPixmap * DeletedAttributeIcon;
extern QPixmap * PublicOperationIcon;
extern QPixmap * PrivateOperationIcon;
extern QPixmap * ProtectedOperationIcon;
extern QPixmap * PackageOperationIcon;
extern QPixmap * DeletedOperationIcon;
extern QPixmap * PublicRelationIcon;
extern QPixmap * PrivateRelationIcon;
extern QPixmap * ProtectedRelationIcon;
extern QPixmap * PackageRelationIcon;
extern QPixmap * DeletedRelationIcon;
extern QPixmap * SimpleRelationIcon;
extern QPixmap * ExtraMemberIcon;
extern QPixmap * ExtraMemberMarkedIcon;
extern QPixmap * DeletedExtraMemberIcon;
extern QPixmap * StateIcon;
extern QPixmap * DeletedStateIcon;
extern QPixmap * RegionIcon;
extern QPixmap * DeletedRegionIcon;
extern QPixmap * ActionIcon;
extern QPixmap * ActionMarkedIcon;
extern QPixmap * DeletedActionIcon;
extern QPixmap * SignalInIcon;
extern QPixmap * SignalInMarkedIcon;
extern QPixmap * DeletedSignalInIcon;
extern QPixmap * SignalOutIcon;
extern QPixmap * SignalOutMarkedIcon;
extern QPixmap * DeletedSignalOutIcon;
extern QPixmap * StateDiagramIcon;
extern QPixmap * DeletedStateDiagramIcon;
extern QPixmap * EntryPointIcon;
extern QPixmap * DeletedEntryPointIcon;
extern QPixmap * ExitPointIcon;
extern QPixmap * DeletedExitPointIcon;
extern QPixmap * InitialIcon;
extern QPixmap * InitialMarkedIcon;
extern QPixmap * DeletedInitialIcon;
extern QPixmap * FinalIcon;
extern QPixmap * FinalMarkedIcon;
extern QPixmap * DeletedFinalIcon;
extern QPixmap * TerminateIcon;
extern QPixmap * TerminateMarkedIcon;
extern QPixmap * DeletedTerminateIcon;
extern QPixmap * DeepHistoryIcon;
extern QPixmap * DeepHistoryMarkedIcon;
extern QPixmap * DeletedDeepHistoryIcon;
extern QPixmap * ShallowHistoryIcon;
extern QPixmap * ShallowHistoryMarkedIcon;
extern QPixmap * DeletedShallowHistoryIcon;
extern QPixmap * JunctionIcon;
extern QPixmap * JunctionMarkedIcon;
extern QPixmap * DeletedJunctionIcon;
extern QPixmap * ChoiceIcon;
extern QPixmap * ChoiceMarkedIcon;
extern QPixmap * DeletedChoiceIcon;
extern QPixmap * ForkIcon;
extern QPixmap * ForkMarkedIcon;
extern QPixmap * DeletedForkIcon;
extern QPixmap * JoinIcon;
extern QPixmap * JoinMarkedIcon;
extern QPixmap * DeletedJoinIcon;
extern QPixmap * ActivityIcon;
extern QPixmap * DeletedActivityIcon;
extern QPixmap * ActivityActionIcon;
extern QPixmap * ActivityActionMarkedIcon;
extern QPixmap * DeletedActivityActionIcon;
extern QPixmap * ActivityDiagramIcon;
extern QPixmap * DeletedActivityDiagramIcon;
extern QPixmap * ParameterIcon;
extern QPixmap * DeletedParameterIcon;
extern QPixmap * ParameterSetIcon;
extern QPixmap * ParameterSetMarkedIcon;
extern QPixmap * DeletedParameterSetIcon;
extern QPixmap * DecisionIcon;
extern QPixmap * DecisionMarkedIcon;
extern QPixmap * DeletedDecisionIcon;
extern QPixmap * MergeIcon;
extern QPixmap * MergeMarkedIcon;
extern QPixmap * DeletedMergeIcon;
extern QPixmap * PinIcon;
extern QPixmap * PinMarkedIcon;
extern QPixmap * DeletedPinIcon;
extern QPixmap * ActivitySendSignalIcon;
extern QPixmap * ActivitySendSignalMarkedIcon;
extern QPixmap * DeletedActivitySendSignalIcon;
extern QPixmap * ActivityAcceptTimeIcon;
extern QPixmap * ActivityAcceptTimeMarkedIcon;
extern QPixmap * DeletedActivityAcceptTimeIcon;
extern QPixmap * ActivityAcceptEventIcon;
extern QPixmap * ActivityAcceptEventMarkedIcon;
extern QPixmap * DeletedActivityAcceptEventIcon;
extern QPixmap * InterruptibleActivityRegionIcon;
extern QPixmap * InterruptibleActivityRegionMarkedIcon;
extern QPixmap * DeletedInterruptibleActivityRegionIcon;
extern QPixmap * ExpansionRegionIcon;
extern QPixmap * ExpansionRegionMarkedIcon;
extern QPixmap * DeletedExpansionRegionIcon;
extern QPixmap * ExpansionNodeIcon;
extern QPixmap * ExpansionNodeMarkedIcon;
extern QPixmap * DeletedExpansionNodeIcon;

// color pixmaps

extern QPixmap * WhitePixmap;
extern QPixmap * LightYellowPixmap;
extern QPixmap * YellowPixmap;
extern QPixmap * MediumYellowPixmap;
extern QPixmap * DarkYellowPixmap;
extern QPixmap * LightBluePixmap;
extern QPixmap * BluePixmap;
extern QPixmap * MediumBluePixmap;
extern QPixmap * DarkBluePixmap;
extern QPixmap * LightGreenPixmap;
extern QPixmap * GreenPixmap;
extern QPixmap * MediumGreenPixmap;
extern QPixmap * DarkGreenPixmap;
extern QPixmap * LightRedPixmap;
extern QPixmap * MidRedPixmap;
extern QPixmap * RedPixmap;
extern QPixmap * LightGrayPixmap;
extern QPixmap * GrayPixmap;

extern QPixmap * VeryLightOrangePixmap;
extern QPixmap * LightOrangePixmap;
extern QPixmap * OrangePixmap;
extern QPixmap * DarkOrangePixmap;
extern QPixmap * LightMagentaPixmap;
extern QPixmap * MagentaPixmap;
extern QPixmap * MidMagentaPixmap;
extern QPixmap * DarkMagentaPixmap;

// dialog pixmap

extern QPixmap * leftPixmap;
extern QPixmap * rightPixmap;
extern QPixmap * upPixmap;
extern QPixmap * downPixmap;

// diagram pixmap

extern QPixmap * providedPixmap;
extern QPixmap * entryPointPixmap;
extern QPixmap * exitPointPixmap;
extern QPixmap * initialPixmap;
extern QPixmap * finalPixmap;
extern QPixmap * terminatePixmap;
extern QPixmap * deepHistoryPixmap;
extern QPixmap * shallowHistoryPixmap;
extern QPixmap * junctionPixmap;
extern QPixmap * choicePixmap;
extern QPixmap * choiceShadowPixmap;
extern QPixmap * joinForkPixmap;
extern QPixmap * joinForkHorizPixmap;
extern QPixmap * entryPointBigPixmap;
extern QPixmap * exitPointBigPixmap;
extern QPixmap * initialBigPixmap;
extern QPixmap * finalBigPixmap;
extern QPixmap * terminateBigPixmap;
extern QPixmap * deepHistoryBigPixmap;
extern QPixmap * shallowHistoryBigPixmap;
extern QPixmap * junctionBigPixmap;
extern QPixmap * choiceBigPixmap;
extern QPixmap * choiceShadowBigPixmap;
extern QPixmap * joinForkBigPixmap;
extern QPixmap * joinForkHorizBigPixmap;

#endif
