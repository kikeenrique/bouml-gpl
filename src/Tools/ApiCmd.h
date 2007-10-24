// *************************************************************************
//
// Copyleft 2004-2007 Bruno PAGES  .
//
// This file is part of the BOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : bouml@free.fr
// home   : http://bouml.free.fr
//
// *************************************************************************

#ifndef APICMD_H
#define APICMD_H

enum CmdFamily {
  onInstanceCmd, classGlobalCmd, packageGlobalCmd, miscGlobalCmd,
  umlSettingsCmd, cppSettingsCmd, javaSettingsCmd, idlSettingsCmd,
  phpSettingsCmd
};

enum MiscGlobalCmd {
  byeCmd,
  traceCmd,
  messageCmd,
  toolRunningCmd,
  targetCmd,
  allMarkedCmd,
  loadCmd,
  showTraceCmd,
  traceAutoRaiseCmd,
  
  fromIdCmd = 124,
  setRootPermissionCmd = 125,
  setUserIdCmd = 126,
  userIdCmd = 127
};

enum OnInstanceCmd {
  // get commands
  
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
  
  // api 31
  attributesCmd,
  relationsCmd,
  
  // api 34
  getPhpDefCmd,
  phpBodyCmd,
  
  // set commands
  
  firstSetCmd = 50,	// just a mark
    
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
  
  // api 16
  moveAfterCmd,
  
  // api 19
  setJavaAnnotationCmd,
  
  // api 22
  setRelationAssocClassCmd,
  
  // api 25
  setReadOnlyCmd,
  setSingleExecutionCmd,
  setUnmarshallCmd,
  setTimeEventCmd,
  setFlagCmd,
  setUniqueCmd,
  setStreamCmd,
  setInStateCmd,
  setOrderingCmd,

  // api 26
  setCppFrozenCmd,
  setJavaFrozenCmd,
  setIdlFrozenCmd,
  
  // api 30
  setConstraintCmd,
  deleteCmd,
  
  // api 31
  setAttributeCmd,
  addRelationCmd,
  removeRelationCmd,
  
  // api 34
  setPhpDeclCmd,
  setIsPhpExternalCmd,
  setIsPhpFinalCmd,
  setPhpSrcCmd,
  setPhpFinalCmd,
  setPhpBodyCmd,
  setPhpNameSpecCmd,
  setPhpDirCmd,
  setPhpFrozenCmd,
  
  // api 35
  setIsForceBodyGenCmd,
    
  setUserCmd = 252, // not in plug-outs managed through bouml
  moveInCmd = 253,  // not in plug-outs managed through bouml

  getIdCmd = 254, // not in old plug-outs managed through bouml
    
  // api 20
  old_deleteCmd = 255 // not in plug-outs managed through bouml
};

enum ClassGlobalCmd {
  findClassCmd
};

enum PackageGlobalCmd {
  findNamespaceCmd,
  findPackageCmd,
  findModuleCmd,
  getProjectCmd,
  isProjectModifiedCmd,
  saveProjectCmd
};

enum UmlSettingsCmd {
  getUmlSettingsCmd,
  
  firstSetUmlSettingsCmd = 50,	// just a mark
  // api 16
  setDefaultArtifactDescriptionCmd = firstSetUmlSettingsCmd,
  setDefaultClassDescriptionCmd,
  setDefaultOperationDescriptionCmd,
  setDefaultAttributeDescriptionCmd,
  setDefaultRelationDescriptionCmd,
  // api 28
  setUmlDefaultGetNameCmd,
  setUmlDefaultSetNameCmd
};

enum CppSettingsCmd {
  getCppSettingsCmd,
  getCppUseDefaultsCmd,
  
  setCppUseDefaultsCmd,
  setCppTypeCmd,
  setCppRelationAttributeStereotypeCmd,
  setCppClassStereotypeCmd,
  setCppIncludeCmd,
  setCppHeaderContentCmd,
  setCppRootDirCmd,
  setCppSourceContentCmd,
  setCppHeaderExtensionCmd,
  setCppSourceExtensionCmd,
  setCppIncludeWithPathCmd,
  setCppEnumInCmd,
  setCppEnumOutCmd,
  setCppEnumInOutCmd,
  setCppInCmd,
  setCppOutCmd,
  setCppInOutCmd,
  setCppClassDeclCmd,
  setCppExternalClassDeclCmd,
  setCppStructDeclCmd,
  setCppUnionDeclCmd,
  setCppEnumDeclCmd,
  setCppTypedefDeclCmd,
  setCppAttributeDeclCmd,
  setCppEnumItemDeclCmd,
  setCppRelationDeclCmd,
  setCppOperationDeclCmd,
  setCppOperationDefCmd,
  setCppOperationForceThrowCmd,
  setCppGetVisibilityCmd,
  setCppGetNameCmd,
  setCppIsGetInlineCmd,
  setCppIsGetConstCmd,
  setCppIsGetValueConstCmd,
  setCppSetVisibilityCmd,
  setCppSetNameCmd,
  setCppIsSetInlineCmd,
  setCppIsSetParamConstCmd,
  // api 16
  setCppEnumReturnCmd,
  setCppReturnCmd,
  // api 26
  setCppIsSetParamRefCmd,
  // api 27
  setCppRelativePathCmd,
  setCppForceNamespaceGenCmd,
  // api 29
  setCppRootRelativePathCmd,
  // api 30
  setCppJavadocStyleCmd
};

enum JavaSettingsCmd {
  getJavaSettingsCmd,
  getJavaUseDefaultsCmd,
  
  setJavaUseDefaultsCmd,
  setJavaTypeCmd,
  setJavaRelationAttributeStereotypeCmd,
  setJavaClassStereotypeCmd,
  setJavaImportCmd,
  setJavaRootdirCmd,
  setJavaSourceContentCmd,
  setJavaSourceExtensionCmd,
  setJavaClassDeclCmd,
  setJavaExternalClassDeclCmd,
  setJavaEnumPatternDeclCmd,
  setJavaInterfaceDeclCmd,
  setJavaAttributeDeclCmd,
  setJavaEnumPatternItemDeclCmd,
  setJavaEnumPatternItemCaseCmd,
  setJavaRelationDeclCmd,
  setJavaOperationDefCmd,
  setJavaGetvisibilityCmd,
  setJavaGetNameCmd,
  setJavaSetNameCmd,
  setJavaIsGetFinalCmd,
  setJavaIsSetFinalCmd,
  setJavaSetVisibilityCmd,
  setJavaIsSetParamFinalCmd,
  
  setJavaEnumDeclCmd,
  setJavaEnumItemDeclCmd,
  
  // api 30
  setJavaJavadocStyleCmd
};

// api 34
enum PhpSettingsCmd {
  getPhpSettingsCmd,
  getPhpUseDefaultsCmd,
  
  setPhpUseDefaultsCmd,
  setPhpClassStereotypeCmd,
  setPhpRootdirCmd,
  setPhpSourceContentCmd,
  setPhpSourceExtensionCmd,
  setPhpClassDeclCmd,
  setPhpEnumDeclCmd,
  setPhpExternalClassDeclCmd,
  setPhpInterfaceDeclCmd,
  setPhpAttributeDeclCmd,
  setPhpEnumItemDeclCmd,
  setPhpRelationDeclCmd,
  setPhpOperationDefCmd,
  setPhpGetNameCmd,
  setPhpSetNameCmd,
  setPhpIsGetFinalCmd,
  setPhpIsSetFinalCmd,
  setPhpIsSetParamFinalCmd,
};

enum IdlSettingsCmd {
  getIdlSettingsCmd,
  getIdlUseDefaultsCmd,
  
  setIdlUseDefaultsCmd,
  setIdlTypeCmd,
  setIdlRelationAttributeStereotypeCmd,
  setIdlClassStereotypeCmd,
  setIdlIncludeCmd,
  setIdlRootDirCmd,
  setIdlSourceContentCmd,
  setIdlSourceExtensionCmd,
  setIdlInterfaceDeclCmd,
  setIdlValuetypeDeclCmd,
  setIdlStructDeclCmd,
  setIdlTypedefDeclCmd,
  setIdlExceptionDeclCmd,
  setIdlUnionDeclCmd,
  setIdlEnumDeclCmd,
  setIdlExternalClassDeclCmd,
  setIdlAttributeDeclCmd,
  setIdlValuetypeAttributeDeclCmd,
  setIdlUnionItemDeclCmd,
  setIdlEnumItemDeclCmd,
  setIdlConstDeclCmd,
  setIdlRelationDeclCmd,
  setIdlValuetypeRelationDeclCmd,
  setIdlUnionRelationDeclCmd,
  setIdlOperationDeclCmd,
  setIdlGetNameCmd,
  setIdlSetNameCmd,
  setIdlIsSetOneWayCmd
};

#endif
