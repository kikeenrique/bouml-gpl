#ifndef _ONINSTANCECMD_H
#define _ONINSTANCECMD_H


// Internal enum
enum OnInstanceCmd {
  parentCmd,
  childrenCmd,
  getDefCmd,
  getUmlDefCmd,
  getCppDefCmd,
  getJavaDefCmd,
  getIdlDefCmd,
  isWritableCmd,
  supportFileCmd,
  isOpenCmd,
  setOpenCmd,
  assocComponentCmd,
  assocArtifactCmd,
  saveInCmd,
  formalsCmd,
  actualsCmd,
  cppBodyCmd,
  javaBodyCmd,
  applyCmd,
  referencedByCmd,
  sideCmd,
  attributesCmd,
  relationsCmd,
  getPhpDefCmd,
  phpBodyCmd,
  getPythonDefCmd,
  pythonBodyCmd,
  firstSetCmd= 50,// just a mark
  createCmd,
  setCoupleValueCmd,
  setDescriptionCmd,
  setNameCmd,
  setStereotypeCmd,
  addAssocClassCmd,
  removeAssocClassCmd,
  setAssocClassesCmd,
  setAssocDiagramCmd,
  setAssocViewCmd,
  addGetOperCmd,
  addSetOperCmd,
  setIsClassMemberCmd,
  setVisibilityCmd,
  setCppDeclCmd,
  setCppDefCmd,
  setJavaDeclCmd,
  setIdlDeclCmd,
  setCppVisibilityCmd,
  setIsReadOnlyCmd,
  setDefaultValueCmd,
  setTypeCmd,
  setIsVolatileCmd,
  setIsCppMutableCmd,
  setIsJavaTransientCmd,
  setIdlCaseCmd,
  setIsCppInlineCmd,
  setIsAbstractCmd,
  setBaseTypeCmd,
  setIsCppExternalCmd,
  setIsJavaExternalCmd,
  setIsIdlExternalCmd,
  setIsJavaPublicCmd,
  setIsJavaFinalCmd,
  setIsIdlLocalCmd,
  setIsIdlCustomCmd,
  setSwitchTypeCmd,
  removeFormalCmd,
  addFormalCmd,
  replaceFormalCmd,
  replaceActualCmd,
  setCppHCmd,
  setCppSrcCmd,
  setJavaSrcCmd,
  setIdlSrcCmd,
  addAssocArtifactCmd,
  removeAssocArtifactCmd,
  removeAllAssocArtifactsCmd,
  setReturnTypeCmd,
  setIsCppConstCmd,
  setIsCppFriendCmd,
  setIsCppVirtualCmd,
  setIsIdlTruncatableCmd,
  setCppBodyCmd,
  setCppNameSpecCmd,
  setJavaFinalCmd,
  setJavaSynchronizedCmd,
  setJavaBodyCmd,
  setJavaNameSpecCmd,
  setIsIdlOnewayCmd,
  setIdlNameSpecCmd,
  addParameterCmd,
  removeParameterCmd,
  replaceParameterCmd,
  addExceptionCmd,
  removeExceptionCmd,
  replaceExceptionCmd,
  setCppSrcDirCmd,
  setCppHDirCmd,
  setCppNamespaceCmd,
  setJavaDirCmd,
  setJavaPackageCmd,
  setIdlDirCmd,
  setIdlModuleCmd,
  setRoleNameCmd,
  setMultiplicityCmd,
  setCppVirtualInheritanceCmd,
  setMarkedCmd,
  setUmlEntryBehaviorCmd,
  setUmlExitBehaviorCmd,
  setUmlActivityCmd,
  setCppEntryBehaviorCmd,
  setCppExitBehaviorCmd,
  setCppActivityCmd,
  setJavaEntryBehaviorCmd,
  setJavaExitBehaviorCmd,
  setJavaActivityCmd,
  setUmlTriggerCmd,
  setUmlGuardCmd,
  setCppTriggerCmd,
  setCppGuardCmd,
  setJavaGuardCmd,
  setJavaTriggerCmd,
  setDefCmd,
  moveAfterCmd,
  setJavaAnnotationCmd,
  setRelationAssocClassCmd,
  setReadOnlyCmd,
  setSingleExecutionCmd,
  setUnmarshallCmd,
  setTimeEventCmd,
  setFlagCmd,
  setUniqueCmd,
  setStreamCmd,
  setInStateCmd,
  setOrderingCmd,
  setCppFrozenCmd,
  setJavaFrozenCmd,
  setIdlFrozenCmd,
  setConstraintCmd,
  deleteCmd,
  setAttributeCmd,
  addRelationCmd,
  removeRelationCmd,
  setPhpDeclCmd,
  setIsPhpExternalCmd,
  setIsPhpFinalCmd,
  setPhpSrcCmd,
  setPhpFinalCmd,
  setPhpBodyCmd,
  setPhpNameSpecCmd,
  setPhpDirCmd,
  setPhpFrozenCmd,
  setIsForceBodyGenCmd,
  setCppContextualBodyIndentCmd,
  setJavaContextualBodyIndentCmd,
  setPhpContextualBodyIndentCmd,
  setPythonDeclCmd,
  setIsPythonExternalCmd,
  setPythonSrcCmd,
  setPythonBodyCmd,
  setPythonNameSpecCmd,
  setPythonDirCmd,
  setPythonFrozenCmd,
  setIsPython2_2Cmd,
  setPythonDecoratorsCmd,
  setPythonPackageCmd,
  setPythonContextualBodyIndentCmd,
  applyStereotypeCmd,
  setDerivedCmd,
  importProjectCmd,
  setActiveCmd,
  setPhpNamespaceCmd

};
#endif
