/********************************************************************

  ROMEBuilder.cpp, M. Schneebeli PSI

  $Id: ROMEBuilder.cpp 2614 2012-09-04 13:36:50Z sawada $

********************************************************************/
#include <stdlib.h>
#include <map>
#include <RConfig.h>
#if defined( R__VISUAL_CPLUSPLUS )
#pragma warning( push )
#pragma warning( disable : 4800 )
#   if !defined( __CINT__ )
#      include <Windows4Root.h>
#   endif
#endif // R__VISUAL_CPLUSPLUS
#include <TSystem.h>
#if defined( R__VISUAL_CPLUSPLUS )
#pragma warning( pop )
#endif // R__VISUAL_CPLUSPLUS

#include "ROMEConfigParameter.h"
#include <Riostream.h>
#include "ROMEBuilder.h"

using namespace std;

ROMEXML* configXSD = 0;

//______________________________________________________________________________
ROMEBuilder::ROMEBuilder()
:romeVersion("")
,romeDescription("")
,romeStable(kTRUE)
,makeFlag("")
,histoParameters(0)
,histoParameterTypes(0)
,histoParameterWidgetTypes(0)
,graphParameters(0)
,graphParameterTypes(0)
,graphParameterWidgetTypes(0)
,outDir("")
,xmlFile("")
,makeOutput(kFALSE)
,noLink(kFALSE)
,midas(kFALSE)
,xz(kFALSE)
,bzip2(kFALSE)
,orca(kFALSE)
,sql(kFALSE)
,mysql(kFALSE)
,pgsql(kFALSE)
,sqlite(kFALSE)
,sqlite3(kFALSE)
,noVP(kFALSE)
,librome(kLIBNone)
#if defined( R__MACOSX )
,dynamicLink(kFALSE)
#else
,dynamicLink(kTRUE)
#endif
,pch(kFALSE)
,minRebuild(kFALSE)
,quietMake(kFALSE)
,flags()
,affiliations()
,affiliationList()
,xml(0)
,shortCut("")
,experimentName("")
,readExperiment(kFALSE)
,readGlobalSteeringParameters(kFALSE)
,hasStructuredBank(kFALSE)
,maxNumberOfTasks(0)
,maxNumberOfHistos(0)
,maxNumberOfGraphs(0)
,maxNumberOfFolders(0)
,maxNumberOfValues(0)
,maxNumberOfTrees(0)
,maxNumberOfBranches(0)
,maxNumberOfDAQ(0)
,maxNumberOfDB(0)
,maxNumberOfEvents(0)
,maxNumberOfRootTrees(0)
,maxNumberOfMFDictHeaders(0)
,maxNumberOfMFDictIncDirs(0)
,maxNumberOfMFWinLibs(0)
,maxNumberOfMFUnixLibs(0)
,maxNumberOfMFIncDirs(0)
,maxNumberOfMFPreDefs(0)
,maxNumberOfMFSources(0)
,maxNumberOfTabs(0)
,maxNumberOfNetFolders(0)
,maxNumberOfSteering(0)
,maxNumberOfSteeringField(0)
,maxNumberOfAffiliations(0)
,maxNumberOfAuthors(0)
,parent(0)
,recursiveDepth(0)
,recursiveSteerDepth(0)
,actualSteerIndex(0)
,fNumberOfInterpreterCodes(0)
,recursiveTabDepth(0)
,recursiveMenuDepth(0)
,daqNameArray(new ROMEStrArray(1))
,daqTypeArray(new ROMEStrArray(1))
,daqDirArray(new ROMEStrArray(1))
,numOfFolder(0)
,numOfValue(0)
,numOfFolderInclude(0)
,numOfFolderAffiliations(0)
,folderAffiliation(0)
,folderUsed(0)
,folderName(0)
,folderDescription(0)
,folderShortDescription(0)
,folderParentName(0)
,folderInheritName(0)
,folderTitle(0)
,folderArray(0)
,numOfFolderAuthors(0)
,folderAuthor(0)
,folderAuthorInstitute(0)
,folderAuthorCollaboration(0)
,folderAuthorEmail(0)
,folderVersion(0)
,folderInclude(0)
,folderLocalFlag(0)
,folderDataBase(0)
,folderIgnoreTObjectStreamer(0)
,folderUserCode(0)
,folderSupport(0)
,folderNoReset(0)
,folderNoResetModified(0)
,folderHasClassField(0)
,hasFolderUserCode(kFALSE)
,hasFolderGenerated(kFALSE)
,hasSupportFolderGenerated(kFALSE)
,hasTaskGenerated(kFALSE)
,valueName(0)
,valueType(0)
,valueInit(0)
,valueComment(0)
,valueDimension(0)
,valueArray(0)
,valueArraySpecifier(0)
,valueDBName(0)
,valueDBPath(0)
,valueDBIf(0)
,valueNoBoundChech(0)
,valueIsTObject(0)
,numOfNetFolder(0)
,netFolderName(0)
,netFolderTitle(0)
,netFolderHost(0)
,netFolderPort(0)
,netFolderRoot(0)
,numOfTask(0)
,taskName(0)
,numOfTaskAffiliations(0)
,taskAffiliation(0)
,taskUsed(0)
,taskEventID(0)
,taskDescription(0)
,taskShortDescription(0)
,taskUsage(0)
,taskStatus(0)
,taskToDo(0)
,taskKnownProblems(0)
,numOfTaskAuthors(0)
,taskAuthor(0)
,taskAuthorInstitute(0)
,taskAuthorCollaboration(0)
,taskAuthorEmail(0)
,taskVersion(0)
,taskDependence(0)
,numOfTaskInclude(0)
,taskInclude(0)
,taskLocalFlag(0)
,numOfTaskAccessedFolder(0)
,taskAccessedFolder(0)
,numOfTaskConnectedFrom(0)
,taskConnectedFrom(0)
,numOfHistos(0)
,histoComment(0)
,histoName(0)
,histoTitle(0)
,histoFolderName(0)
,histoFolderTitle(0)
,histoType(0)
,histoArraySize(0)
,histoArrayStartIndex(0)
,histoXLabel(0)
,histoYLabel(0)
,histoZLabel(0)
,histoXNbins(0)
,histoXmin(0)
,histoXmax(0)
,histoYNbins(0)
,histoYmin(0)
,histoYmax(0)
,histoZNbins(0)
,histoZmin(0)
,histoZmax(0)
,histoOption(0)
,numOfHistoSingleObjectTabs(0)
,histoSingleObjectTabName(0)
,histoSingleObjectTabIndex(0)
,histoSingleObjectTabArrayIndex(0)
,histoSingleObjectTabDrawSamePad(0)
,numOfGraphs(0)
,graphName(0)
,graphTitle(0)
,graphFolderName(0)
,graphFolderTitle(0)
,graphType(0)
,graphArraySize(0)
,graphArrayStartIndex(0)
,graphXLabel(0)
,graphYLabel(0)
,graphZLabel(0)
,graphXmin(0)
,graphXmax(0)
,graphYmin(0)
,graphYmax(0)
,graphZmin(0)
,graphZmax(0)
,numOfGraphSingleObjectTabs(0)
,graphSingleObjectTabName(0)
,graphSingleObjectTabIndex(0)
,graphSingleObjectTabArrayIndex(0)
,graphSingleObjectTabDrawSamePad(0)
,numOfTaskHierarchy(0)
,taskHierarchyName(0)
,taskHierarchyParentIndex(0)
,taskHierarchyClassIndex(0)
,taskHierarchyMultiplicity(0)
,taskHierarchyLevel(0)
,taskHierarchyObjectIndex(0)
,taskHierarchySuffix(0)
,numOfTaskHierarchyConnectedFrom(0)
,taskHierarchyConnectedFrom(0)
,numOfTaskHierarchyWOAffiliation(0)
,taskHierarchyNameWOAffiliation(0)
,taskHierarchySuffixWOAffiliation(0)
,numOfSteering(0)
,numOfSteerFields(0)
,numOfSteerChildren(0)
,steerName(0)
,steerArraySize(0)
,steerParent(0)
,steerChildren(0)
,numOfSteerAffiliations(0)
,steerAffiliation(0)
,steerUsed(0)
,steerFieldName(0)
,steerFieldType(0)
,steerFieldFormat(0)
,steerFieldArraySize(0)
,steerFieldInit(0)
,steerFieldComment(0)
,steerFieldShortDescription(0)
,steerFieldCLOption(0)
,steerFieldCLDescription(0)
,numOfSteerFieldAffiliations(0)
,steerFieldAffiliation(0)
,steerFieldUsed(0)
,steerFieldHotLink(0)
,haveSteerFieldHotLinks(kFALSE)
,numOfGSPInclude(0)
,gspInclude(0)
,gspLocalFlag(0)
,usedCLO("")
,numOfTab(0)
,tabName(0)
,tabSuffix(0)
,tabTitle(0)
,tabObjectDisplay(0)
,numOfTabAffiliations(0)
,tabAffiliation(0)
,tabUsed(0)
,tabUsedIndex(0)
,tabDescription(0)
,tabShortDescription(0)
,tabUsage(0)
,tabStatus(0)
,tabToDo(0)
,tabKnownProblems(0)
,numOfTabAuthors(0)
,tabAuthor(0)
,tabAuthorInstitute(0)
,tabAuthorCollaboration(0)
,tabAuthorEmail(0)
,tabVersion(0)
,tabDependence(0)
,tabHeredity(0)
,tabHeredityIndex(0)
,tabParentIndex(0)
,tabNumOfChildren(0)
,numOfMenu(0)
,numOfMenuItem(0)
,menuTitle(0)
,menuDepth(0)
,menuItemChildMenuIndex(0)
,menuItemEnumName(0)
,menuItemTitle(0)
,numOfTabSingleObjects(0)
,tabSingleObjectName(0)
,tabSingleObjectIndex(0)
,tabSingleObjectArrayIndexStart(0)
,tabSingleObjectArrayIndexEnd(0)
,tabSingleObjectTaskHierarchyIndex(0)
,tabSingleObjectTaskIndex(0)
,tabSingleObjectObjectIndex(0)
,tabSingleObjectType(0)
,tabSingleObjectDrawSamePad(0)
,tabSingleObjectIndexMax(0)
,numOfTabObjectDisplays(0)
,tabObjectDisplayName(0)
,tabObjectDisplayTitle(0)
,numOfTabObjectDisplayObjects(0)
,tabObjectDisplayObject(0)
,tabObjectDisplayType(0)
,tabObjectDisplayTaskHierarchyIndex(0)
,tabObjectDisplayTaskIndex(0)
,tabObjectDisplayObjectIndex(0)
,tabObjectDisplayTaskHierarchyNumber(0)
,tabObjectDisplaySupportedObjects()
,tabObjectDisplayObjectTypeIndex(0)
,numOfTabObjectDisplayObjectTypes(0)
,tabObjectDisplayObjectType(0)
,hasDependenceCheck(kFALSE)
,numOfTree(0)
,numOfBranch(0)
,numOfRunHeader(0)
,treeName(0)
,treeTitle(0)
,treeFileName(0)
,treeDescription(0)
,branchName(0)
,branchFolder(0)
,branchBufferSize(0)
,branchSplitLevel(0)
,branchSplitLevelSpecified(0)
,runHeaderName(0)
,runHeaderFolder(0)
,runHeaderFolderIndex(0)
,numOfThreadFunctions(0)
,numOfThreadFunctionArguments(0)
,threadFunctionName(0)
,threadFunctionArgument(0)
,numOfDAQ(0)
,daqName(0)
,numOfDAQAffiliations(0)
,daqAffiliation(0)
,daqUsed(0)
,numOfDB(0)
,dbName(0)
,dbDescription(0)
,numOfEvent(0)
,numOfBank(0)
,numOfStructFields(0)
,eventName(0)
,eventID(0)
,eventTriggerMask(0)
,eventSamplingRate(0)
,maxEventID(2) /* at least 2, for backward compatibility (assuming trigger and scaler) */
,bankName(0)
,bankAlias(0)
,bankType(0)
,bankArraySize(0)
,bankArrayStart(0)
,bankArrayDigit(0)
,structFieldName(0)
,structFieldType(0)
,structFieldSize(0)
,bankFieldArraySize(0)
,bankHasHeader(kFALSE)
,bankHeaderFolder("")
,bankHeaderEventID("")
,bankHeaderTriggerMask("")
,bankHeaderSerialNumber("")
,bankHeaderTimeStamp("")
,numOfRootTree(0)
,numOfRootBranch(0)
,numOfRootBranchField(0)
,rootTreeName(0)
,rootBranchName(0)
,rootBranchType(0)
,rootBranchArraySize(0)
,rootBranchClassName(0)
,rootBranchClassVersion(0)
,rootBranchFieldName(0)
,rootBranchFieldType(0)
,rootBranchFieldArraySize(0)
,numOfMFDictHeaders(0)
,mfDictHeaderName(0)
,mfDictHeaderLinkDefSuffix(0)
,numOfMFDictHeaderAffiliations(0)
,mfDictHeaderAffiliation(0)
,mfDictHeaderUsed(0)
,numOfIncludeDirectories(0)
,numOfMFDictIncDirs(0)
,mfDictIncDir(0)
,numOfMFWinLibs(0)
,mfWinLibName(0)
,numOfMFWinLibFlags(0)
,mfWinLibFlag(0)
,numOfMFUnixLibs(0)
,mfUnixLibName(0)
,numOfMFUnixLibFlags(0)
,mfUnixLibFlag(0)
,numOfMFIncDirs(0)
,mfIncDir(0)
,numOfMFPreDefs(0)
,mfPreDefName(0)
,numOfMFSources(0)
,mfSourceFileName(0)
,mfSourceFilePath(0)
,mfSourceFileObjPath(0)
,mfHeaderFileName(0)
,mfHeaderFilePath(0)
,numOfMFSourceFlags(0)
,mfSourceFileFlag(0)
,numOfMFSourceAffiliations(0)
,mfSourceFileAffiliation(0)
,mfSourceFileUsed(0)
,objDirList()
,numOfMainAuthors(0)
,mainAuthor(0)
,mainInstitute(0)
,mainCollaboration(0)
,mainEmail(0)
,mainProgName("")
,mainDefinitionVersion("1")
,mainDescription("")
,styleSheet("")
,baseURL("htmldoc/")
,dictionaryType(0)
,dictionaryNames(0)
,dictionaryOutputs(0)
,dictionaryDependencies(0)
,dictionaryCommands(0)
,dictionaryHeaders(0)
,dictionarySources(0)
,dictionaryLinkDefSuffix(0)
,includeDirectories(0)
,romeHeaders(0)
,romeDictHeaders(0)
,romeSources(0)
,romeLinkDefSuffix(0)
,argusHeaders(0)
,argusSources(0)
,argusLinkDefSuffix(0)
,generatedHeaders(0)
,generatedDictHeaders(0)
,generatedLinkDefSuffix(0)
,generatedFolderDictHeaders(0)
,generatedFolderLinkDefSuffix(0)
,generatedSupportFolderDictHeaders(0)
,generatedSupportFolderLinkDefSuffix(0)
,generatedTaskDictHeaders(0)
,generatedTaskLinkDefSuffix(0)
,generatedTabDictHeaders(0)
,generatedTabLinkDefSuffix(0)
,generatedSources(0)
,folderHeaders(0)
,folderSources(0)
,folderLinkDefSuffix(0)
,taskHeaders(0)
,taskSources(0)
,taskLinkDefSuffix(0)
,tabHeaders(0)
,tabSources(0)
,tabLinkDefSuffix(0)
,daqHeaders(0)
,daqSources(0)
,daqLinkDefSuffix(0)
,databaseHeaders(0)
,databaseSources(0)
,databaseLinkDefSuffix(0)
,additionalDictHeaders(0)
,additionalDictLinkDefSuffix(0)
,rootLibraries(0)
,mysqlLibraries(0)
,daqLibraries(0)
,daqFlags(0)
,precompiledHeaders(0)
,precompiledIncludeHeaders(0)
,mainParGroup(0)
,maxConfigParameterHierarchyLevel(0)
{
}

//______________________________________________________________________________
ROMEBuilder::~ROMEBuilder()
{
   // Free memory

   delete configXSD;

   SafeDelete(histoParameters);
   SafeDelete(histoParameterTypes);
   SafeDelete(histoParameterWidgetTypes);
   SafeDelete(graphParameters);
   SafeDelete(graphParameterTypes);
   SafeDelete(graphParameterWidgetTypes);

   FreeArray(parent);

   SafeDelete(daqNameArray);
   SafeDelete(daqTypeArray);
   SafeDelete(daqDirArray);

   FreeArray(numOfValue);
   FreeArray(numOfFolderInclude);

   FreeArray(numOfFolderAffiliations);
   FreeArray(folderAffiliation);
   FreeArray(folderUsed);
   FreeArray(folderName);
   FreeArray(folderDescription);
   FreeArray(folderShortDescription);
   FreeArray(folderParentName);
   FreeArray(folderInheritName);
   FreeArray(folderTitle);
   FreeArray(folderArray);
   FreeArray(numOfFolderAuthors);
   FreeArray(folderAuthor);
   FreeArray(folderAuthorInstitute);
   FreeArray(folderAuthorCollaboration);
   FreeArray(folderAuthorEmail);
   FreeArray(folderVersion);
   FreeArray(folderInclude);
   FreeArray(folderLocalFlag);
   FreeArray(folderDataBase);
   FreeArray(folderIgnoreTObjectStreamer);
   FreeArray(folderUserCode);
   FreeArray(folderSupport);
   FreeArray(folderNoReset);
   FreeArray(folderNoResetModified);
   FreeArray(folderHasClassField);

   FreeArray(valueName);
   FreeArray(valueType);
   FreeArray(valueInit);
   FreeArray(valueComment);
   FreeArray(valueDimension);
   FreeArray(valueArray);
   FreeArray(valueArraySpecifier);
   FreeArray(valueDBName);
   FreeArray(valueDBPath);
   FreeArray(valueDBIf);
   FreeArray(valueNoBoundChech);
   FreeArray(valueIsTObject);

   // net folder
   FreeArray(netFolderName);
   FreeArray(netFolderTitle);
   FreeArray(netFolderHost);
   FreeArray(netFolderPort);
   FreeArray(netFolderRoot);

   // task
   FreeArray(numOfTaskInclude);
   FreeArray(taskName);
   FreeArray(numOfTaskAffiliations);
   FreeArray(taskAffiliation);
   FreeArray(taskUsed);
   FreeArray(taskEventID);
   FreeArray(taskDescription);
   FreeArray(taskShortDescription);
   FreeArray(taskUsage);
   FreeArray(taskStatus);
   FreeArray(taskToDo);
   FreeArray(taskKnownProblems);
   FreeArray(numOfTaskAuthors);
   FreeArray(taskAuthor);
   FreeArray(taskAuthorInstitute);
   FreeArray(taskAuthorCollaboration);
   FreeArray(taskAuthorEmail);
   FreeArray(taskVersion);
   FreeArray(taskDependence);
   FreeArray(taskInclude);
   FreeArray(taskLocalFlag);
   FreeArray(numOfTaskAccessedFolder);
   FreeArray(taskAccessedFolder);
   FreeArray(numOfTaskConnectedFrom);
   FreeArray(taskConnectedFrom);
   FreeArray(numOfHistos);
   FreeArray(histoComment);
   FreeArray(histoName);
   FreeArray(histoTitle);
   FreeArray(histoFolderName);
   FreeArray(histoFolderTitle);
   FreeArray(histoType);
   FreeArray(histoArraySize);
   FreeArray(histoArrayStartIndex);
   FreeArray(histoXLabel);
   FreeArray(histoYLabel);
   FreeArray(histoZLabel);
   FreeArray(histoXNbins);
   FreeArray(histoXmin);
   FreeArray(histoXmax);
   FreeArray(histoYNbins);
   FreeArray(histoYmin);
   FreeArray(histoYmax);
   FreeArray(histoZNbins);
   FreeArray(histoZmin);
   FreeArray(histoZmax);
   FreeArray(histoOption);
   FreeArray(numOfHistoSingleObjectTabs);
   FreeArray(histoSingleObjectTabName);
   FreeArray(histoSingleObjectTabIndex);
   FreeArray(histoSingleObjectTabArrayIndex);
   FreeArray(histoSingleObjectTabDrawSamePad);
   FreeArray(numOfGraphs);
   FreeArray(graphName);
   FreeArray(graphTitle);
   FreeArray(graphFolderName);
   FreeArray(graphFolderTitle);
   FreeArray(graphType);
   FreeArray(graphArraySize);
   FreeArray(graphArrayStartIndex);
   FreeArray(graphXLabel);
   FreeArray(graphYLabel);
   FreeArray(graphZLabel);
   FreeArray(graphXmin);
   FreeArray(graphXmax);
   FreeArray(graphYmin);
   FreeArray(graphYmax);
   FreeArray(graphZmin);
   FreeArray(graphZmax);
   FreeArray(numOfGraphSingleObjectTabs);
   FreeArray(graphSingleObjectTabName);
   FreeArray(graphSingleObjectTabIndex);
   FreeArray(graphSingleObjectTabArrayIndex);
   FreeArray(graphSingleObjectTabDrawSamePad);

   // task hierarchy
   FreeArray(taskHierarchyName);
   FreeArray(taskHierarchyParentIndex);
   FreeArray(taskHierarchyClassIndex);
   FreeArray(taskHierarchyMultiplicity);
   FreeArray(taskHierarchyLevel);
   FreeArray(taskHierarchyObjectIndex);
   FreeArray(taskHierarchySuffix);
   FreeArray(numOfTaskHierarchyConnectedFrom);
   FreeArray(taskHierarchyConnectedFrom);

   // task hierarchy without affiliation
   FreeArray(taskHierarchyNameWOAffiliation);
   FreeArray(taskHierarchySuffixWOAffiliation);

   // steering
   FreeArray(numOfSteering);
   FreeArray(numOfSteerFields);
   FreeArray(numOfSteerChildren);
   FreeArray(steerName);
   FreeArray(steerArraySize);
   FreeArray(steerParent);
   FreeArray(steerChildren);
   FreeArray(numOfSteerAffiliations);
   FreeArray(steerAffiliation);
   FreeArray(steerUsed);
   FreeArray(steerFieldName);
   FreeArray(steerFieldType);
   FreeArray(steerFieldFormat);
   FreeArray(steerFieldArraySize);
   FreeArray(steerFieldInit);
   FreeArray(steerFieldComment);
   FreeArray(steerFieldShortDescription);
   FreeArray(steerFieldCLOption);
   FreeArray(steerFieldCLDescription);
   FreeArray(numOfSteerFieldAffiliations);
   FreeArray(steerFieldAffiliation);
   FreeArray(steerFieldUsed);
   FreeArray(steerFieldHotLink);
   FreeArray(gspInclude);
   FreeArray(gspLocalFlag);

   // tab
   FreeArray(tabName);
   FreeArray(tabSuffix);
   FreeArray(tabTitle);
   FreeArray(tabObjectDisplay);
   FreeArray(numOfTabAffiliations);
   FreeArray(tabAffiliation);
   FreeArray(tabUsed);
   FreeArray(tabUsedIndex);
   FreeArray(tabDescription);
   FreeArray(tabShortDescription);
   FreeArray(tabUsage);
   FreeArray(tabStatus);
   FreeArray(tabToDo);
   FreeArray(tabKnownProblems);
   FreeArray(numOfTabAuthors);
   FreeArray(tabAuthor);
   FreeArray(tabAuthorInstitute);
   FreeArray(tabAuthorCollaboration);
   FreeArray(tabAuthorEmail);
   FreeArray(tabVersion);
   FreeArray(tabDependence);
   FreeArray(tabHeredity);
   FreeArray(tabHeredityIndex);
   FreeArray(tabParentIndex);
   FreeArray(tabNumOfChildren);
   FreeArray(numOfMenu);
   FreeArray(numOfMenuItem);
   FreeArray(menuTitle);
   FreeArray(menuDepth);
   FreeArray(menuItemChildMenuIndex);
   FreeArray(menuItemEnumName);
   FreeArray(menuItemTitle);
   FreeArray(numOfTabSingleObjects);
   FreeArray(tabSingleObjectName);
   FreeArray(tabSingleObjectIndex);
   FreeArray(tabSingleObjectArrayIndexStart);
   FreeArray(tabSingleObjectArrayIndexEnd);
   FreeArray(tabSingleObjectTaskHierarchyIndex);
   FreeArray(tabSingleObjectTaskIndex);
   FreeArray(tabSingleObjectObjectIndex);
   FreeArray(tabSingleObjectType);
   FreeArray(tabSingleObjectDrawSamePad);
   FreeArray(tabSingleObjectIndexMax);
   FreeArray(numOfTabObjectDisplays);
   FreeArray(tabObjectDisplayName);
   FreeArray(tabObjectDisplayTitle);
   FreeArray(numOfTabObjectDisplayObjects);
   FreeArray(tabObjectDisplayObject);
   FreeArray(tabObjectDisplayType);
   FreeArray(tabObjectDisplayTaskHierarchyIndex);
   FreeArray(tabObjectDisplayTaskIndex);
   FreeArray(tabObjectDisplayObjectIndex);
   FreeArray(tabObjectDisplayTaskHierarchyNumber);
   FreeArray(tabObjectDisplayObjectTypeIndex);
   FreeArray(numOfTabObjectDisplayObjectTypes);
   FreeArray(tabObjectDisplayObjectType);

   // tree
   FreeArray(numOfBranch);
   FreeArray(numOfRunHeader);
   FreeArray(treeName);
   FreeArray(treeTitle);
   FreeArray(treeFileName);
   FreeArray(treeDescription);
   FreeArray(branchName);
   FreeArray(branchFolder);
   FreeArray(branchBufferSize);
   FreeArray(branchSplitLevel);
   FreeArray(branchSplitLevelSpecified);
   FreeArray(runHeaderName);
   FreeArray(runHeaderFolder);
   FreeArray(runHeaderFolderIndex);

   // thread functions
   FreeArray(numOfThreadFunctions);
   FreeArray(numOfThreadFunctionArguments);
   FreeArray(threadFunctionName);
   FreeArray(threadFunctionArgument);

   // daq
   FreeArray(daqName);
   FreeArray(numOfDAQAffiliations);
   FreeArray(daqAffiliation);
   FreeArray(daqUsed);

   // database
   FreeArray(dbName);
   FreeArray(dbDescription);

   // midas
   FreeArray(numOfBank);
   FreeArray(numOfStructFields);
   FreeArray(eventName);
   FreeArray(eventID);
   FreeArray(eventTriggerMask);
   FreeArray(eventSamplingRate);
   FreeArray(bankName);
   FreeArray(bankAlias);
   FreeArray(bankType);
   FreeArray(bankArraySize);
   FreeArray(bankArrayStart);
   FreeArray(bankArrayDigit);
   FreeArray(structFieldName);
   FreeArray(structFieldType);
   FreeArray(structFieldSize);
   FreeArray(bankFieldArraySize);

   // root DAQ
   FreeArray(numOfRootBranch);
   FreeArray(numOfRootBranchField);
   FreeArray(rootTreeName);
   FreeArray(rootBranchName);
   FreeArray(rootBranchType);
   FreeArray(rootBranchArraySize);
   FreeArray(rootBranchClassName);
   FreeArray(rootBranchClassVersion);
   FreeArray(rootBranchFieldName);
   FreeArray(rootBranchFieldType);
   FreeArray(rootBranchFieldArraySize);

   // user makefile
   FreeArray(mfDictHeaderName);
   FreeArray(mfDictHeaderLinkDefSuffix);
   FreeArray(numOfMFDictHeaderAffiliations);
   FreeArray(mfDictHeaderAffiliation);
   FreeArray(mfDictHeaderUsed);
   FreeArray(mfDictIncDir);
   FreeArray(mfWinLibName);
   FreeArray(numOfMFWinLibFlags);
   FreeArray(mfWinLibFlag);
   FreeArray(mfUnixLibName);
   FreeArray(numOfMFUnixLibFlags);
   FreeArray(mfUnixLibFlag);
   FreeArray(mfIncDir);
   FreeArray(mfPreDefName);
   FreeArray(mfSourceFileName);
   FreeArray(mfSourceFilePath);
   FreeArray(mfSourceFileObjPath);
   FreeArray(mfHeaderFileName);
   FreeArray(mfHeaderFilePath);
   FreeArray(numOfMFSourceFlags);
   FreeArray(mfSourceFileFlag);
   FreeArray(numOfMFSourceAffiliations);
   FreeArray(mfSourceFileAffiliation);
   FreeArray(mfSourceFileUsed);
   FreeArray(mainAuthor);
   FreeArray(mainInstitute);
   FreeArray(mainCollaboration);
   FreeArray(mainEmail);

   SafeDelete(dictionaryNames);
   SafeDelete(dictionaryOutputs);
   SafeDelete(dictionaryDependencies);
   SafeDelete(dictionaryCommands);
   SafeDelete(dictionaryHeaders);
   SafeDelete(dictionarySources);
   SafeDelete(dictionaryLinkDefSuffix);
   SafeDelete(includeDirectories);
   SafeDelete(romeHeaders);
   SafeDelete(romeDictHeaders);
   SafeDelete(romeSources);
   SafeDelete(romeLinkDefSuffix);
   SafeDelete(argusHeaders);
   SafeDelete(argusSources);
   SafeDelete(argusLinkDefSuffix);
   SafeDelete(generatedHeaders);
   SafeDelete(generatedDictHeaders);
   SafeDelete(generatedLinkDefSuffix);
   SafeDelete(generatedFolderDictHeaders);
   SafeDelete(generatedFolderLinkDefSuffix);
   SafeDelete(generatedSupportFolderDictHeaders);
   SafeDelete(generatedSupportFolderLinkDefSuffix);
   SafeDelete(generatedTaskDictHeaders);
   SafeDelete(generatedTaskLinkDefSuffix);
   SafeDelete(generatedTabDictHeaders);
   SafeDelete(generatedTabLinkDefSuffix);
   SafeDelete(generatedSources);
   SafeDelete(folderHeaders);
   SafeDelete(folderSources);
   SafeDelete(folderLinkDefSuffix);
   SafeDelete(taskHeaders);
   SafeDelete(taskSources);
   SafeDelete(taskLinkDefSuffix);
   SafeDelete(tabHeaders);
   SafeDelete(tabSources);
   SafeDelete(tabLinkDefSuffix);
   SafeDelete(daqHeaders);
   SafeDelete(daqSources);
   SafeDelete(daqLinkDefSuffix);
   SafeDelete(databaseHeaders);
   SafeDelete(databaseSources);
   SafeDelete(databaseLinkDefSuffix);
   SafeDelete(additionalDictHeaders);
   SafeDelete(additionalDictLinkDefSuffix);
   SafeDelete(rootLibraries);
   SafeDelete(mysqlLibraries);
   SafeDelete(daqLibraries);
   SafeDelete(daqFlags);
   SafeDelete(precompiledHeaders);
   SafeDelete(precompiledIncludeHeaders);

   SafeDelete(mainParGroup);
}

//______________________________________________________________________________
Bool_t ROMEBuilder::StartBuilder()
{
   int i,j,k,l;
   bool found;
   ROMEString str;
   ROMEString oldFile;
   ROMEString newFile;

   tabObjectDisplaySupportedObjects.AddLast("ROMETGraph");
   tabObjectDisplaySupportedObjects.AddLast("ROMETGraphErrors");
   tabObjectDisplaySupportedObjects.AddLast("ROMETCutG");
//   tabObjectDisplaySupportedObjects.AddLast("TGraph2D");
   tabObjectDisplaySupportedObjects.AddLast("TH1C");
   tabObjectDisplaySupportedObjects.AddLast("TH1S");
   tabObjectDisplaySupportedObjects.AddLast("TH1I");
   tabObjectDisplaySupportedObjects.AddLast("TH1F");
   tabObjectDisplaySupportedObjects.AddLast("TH1D");
   tabObjectDisplaySupportedObjects.AddLast("TH2C");
   tabObjectDisplaySupportedObjects.AddLast("TH2S");
   tabObjectDisplaySupportedObjects.AddLast("TH2I");
   tabObjectDisplaySupportedObjects.AddLast("TH2F");
   tabObjectDisplaySupportedObjects.AddLast("TH2D");
   tabObjectDisplaySupportedObjects.AddLast("TH3C");
   tabObjectDisplaySupportedObjects.AddLast("TH3S");
   tabObjectDisplaySupportedObjects.AddLast("TH3I");
   tabObjectDisplaySupportedObjects.AddLast("TH3F");
   tabObjectDisplaySupportedObjects.AddLast("TH3D");
   tabObjectDisplaySupportedObjects.AddLast("TProfile");

   configXSD = new ROMEXML();
   ROMEString xsdName = "$(ROMESYS)/romeConfig.xsd";
   gSystem->ExpandPathName(xsdName);
   configXSD->OpenFileForPath(xsdName.Data());

   if (!AllocateMemorySpace())
      return false;
   if (!ReadXMLDefinitionFile())
      return false;

   int tabNumber   = numOfTab;
   int histoNumber = 0;
   int graphNumber = 0;
   int is,ie,ind;

   // Assign Single Object Tabs
   for (i = 0; i < numOfTaskHierarchy; i++) {
      if (!taskUsed[taskHierarchyClassIndex[i]])
         continue;
      for (j = 0; j < numOfHistos[taskHierarchyClassIndex[i]]; j++) {
         for (k = 0; k < numOfHistoSingleObjectTabs[taskHierarchyClassIndex[i]][j]; k++) {
            found = false;
            for (l=0;l<numOfTab;l++) {
               if (!tabUsed[l])
                  continue;
               if (histoSingleObjectTabName[taskHierarchyClassIndex[i]][j][k] == tabName[l]) {
                  found = true;
                  tabNumber = l;
                  histoNumber = numOfTabSingleObjects[l];
                  break;
               }
            }
            if (!found) {
               if (numOfTab < 0)
                  numOfTab = 0;
               tabName[numOfTab] = histoSingleObjectTabName[taskHierarchyClassIndex[i]][j][k];
               tabTitle[numOfTab] = histoSingleObjectTabName[taskHierarchyClassIndex[i]][j][k];
               tabObjectDisplay[numOfTab] = false;
               numOfTabAuthors[numOfTab] = numOfMainAuthors;
               for (l = 0; l < numOfTabAuthors[numOfTab]; l++) {
                  tabAuthorInstitute[numOfTab][l] = mainInstitute[l];
                  tabAuthorCollaboration[numOfTab][l] = mainCollaboration[l];
                  tabAuthorEmail[numOfTab][l] = mainEmail[l];
               }
               tabVersion[numOfTab] = "1";
               tabDescription[numOfTab] = "";
               tabShortDescription[numOfTab] = "";
               tabUsage[numOfTab] = "";
               tabStatus[numOfTab] = "";
               tabToDo[numOfTab] = "";
               tabKnownProblems[numOfTab] = "";
               numOfSteering[numOfTab+numOfTask+1] = -1;
               numOfSteerChildren[numOfTab+numOfTask+1][0] = 0;
               numOfSteerFields[numOfTab+numOfTask+1][0] = 0;
               numOfMenu[numOfTab] = -1;
               tabNumOfChildren[numOfTab] = 0;
               numOfThreadFunctions[numOfTab] = 0;
               tabParentIndex[numOfTab] = -1;
               tabNumber = numOfTab;
               histoNumber = 0;
               numOfTabSingleObjects[numOfTab] = 0;
               tabUsed[numOfTab] = true;
               numOfTab++;
            }
            is = histoSingleObjectTabArrayIndex[taskHierarchyClassIndex[i]][j][k].ToInteger(); ie = is;
            if ((ind=histoSingleObjectTabArrayIndex[taskHierarchyClassIndex[i]][j][k].Index("-")) != -1) {
               str = histoSingleObjectTabArrayIndex[taskHierarchyClassIndex[i]][j][k](0,ind);
               is = str.ToInteger();
               str = histoSingleObjectTabArrayIndex[taskHierarchyClassIndex[i]][j][k](ind+1,
                                                                                      histoSingleObjectTabArrayIndex[taskHierarchyClassIndex[i]][j][k].Length()-ind-1);
               ie = str.ToInteger();
            }
            tabSingleObjectName[tabNumber][histoNumber] = histoName[taskHierarchyClassIndex[i]][j];
            tabSingleObjectIndex[tabNumber][histoNumber] =
                  histoSingleObjectTabIndex[taskHierarchyClassIndex[i]][j][k].ToInteger();
            tabSingleObjectArrayIndexStart[tabNumber][histoNumber] = is;
            tabSingleObjectArrayIndexEnd[tabNumber][histoNumber] = ie;
            tabSingleObjectTaskHierarchyIndex[tabNumber][histoNumber] = i;
            tabSingleObjectTaskIndex[tabNumber][histoNumber] = taskHierarchyClassIndex[i];
            tabSingleObjectObjectIndex[tabNumber][histoNumber] = j;
            tabSingleObjectType[tabNumber][histoNumber] = "Histogram";
            tabSingleObjectDrawSamePad[tabNumber][histoNumber] =
               histoSingleObjectTabDrawSamePad[taskHierarchyClassIndex[i]][j][k].ToBool();
            numOfTabSingleObjects[tabNumber]++;
         }
      }
      for (j = 0; j < numOfGraphs[taskHierarchyClassIndex[i]]; j++) {
         for (k = 0; k < numOfGraphSingleObjectTabs[taskHierarchyClassIndex[i]][j]; k++) {
            found = false;
            for (l=0;l<numOfTab;l++) {
               if (!tabUsed[l])
                  continue;
               if (graphSingleObjectTabName[taskHierarchyClassIndex[i]][j][k] == tabName[l]) {
                  found = true;
                  tabNumber = l;
                  graphNumber = numOfTabSingleObjects[l];
                  break;
               }
            }
            if (!found) {
               if (numOfTab < 0)
                  numOfTab = 0;
               tabName[numOfTab] = graphSingleObjectTabName[taskHierarchyClassIndex[i]][j][k];
               tabTitle[numOfTab] = graphSingleObjectTabName[taskHierarchyClassIndex[i]][j][k];
               tabObjectDisplay[numOfTab] = false;
               numOfTabAuthors[numOfTab] = numOfMainAuthors;
               for (l = 0; l < numOfTabAuthors[numOfTab]; l++) {
                  tabAuthorInstitute[numOfTab][l] = mainInstitute[l];
                  tabAuthorCollaboration[numOfTab][l] = mainCollaboration[l];
                  tabAuthorEmail[numOfTab][l] = mainEmail[l];
               }
               tabVersion[numOfTab] = "1";
               tabDescription[numOfTab] = "";
               tabShortDescription[numOfTab] = "";
               tabUsage[numOfTab] = "";
               tabStatus[numOfTab] = "";
               tabToDo[numOfTab] = "";
               tabKnownProblems[numOfTab] = "";
               numOfSteering[numOfTab+numOfTask+1] = -1;
               numOfSteerChildren[numOfTab+numOfTask+1][0] = 0;
               numOfSteerFields[numOfTab+numOfTask+1][0] = 0;
               numOfMenu[numOfTab] = -1;
               tabNumOfChildren[numOfTab] = 0;
               numOfThreadFunctions[numOfTab] = 0;
               tabParentIndex[numOfTab] = -1;
               tabNumber = numOfTab;
               graphNumber = 0;
               numOfTabSingleObjects[numOfTab] = 0;
               tabUsed[numOfTab] = true;
               numOfTab++;
            }
            is = graphSingleObjectTabArrayIndex[taskHierarchyClassIndex[i]][j][k].ToInteger(); ie = is;
            if ((ind=graphSingleObjectTabArrayIndex[taskHierarchyClassIndex[i]][j][k].Index("-")) != -1) {
               str = graphSingleObjectTabArrayIndex[taskHierarchyClassIndex[i]][j][k](0,ind);
               is = str.ToInteger();
               str = graphSingleObjectTabArrayIndex[taskHierarchyClassIndex[i]][j][k](ind+1,
                                                                                      graphSingleObjectTabArrayIndex[taskHierarchyClassIndex[i]][j][k].Length()-ind-1);
               ie = str.ToInteger();
            }
            tabSingleObjectName[tabNumber][graphNumber] = graphName[taskHierarchyClassIndex[i]][j];
            tabSingleObjectIndex[tabNumber][graphNumber]
                  = graphSingleObjectTabIndex[taskHierarchyClassIndex[i]][j][k].ToInteger();
            tabSingleObjectArrayIndexStart[tabNumber][graphNumber] = is;
            tabSingleObjectArrayIndexEnd[tabNumber][graphNumber] = ie;
            tabSingleObjectTaskHierarchyIndex[tabNumber][graphNumber] = i;
            tabSingleObjectTaskIndex[tabNumber][graphNumber] = taskHierarchyClassIndex[i];
            tabSingleObjectObjectIndex[tabNumber][graphNumber] = j;
            tabSingleObjectType[tabNumber][graphNumber] = "Graph";
            tabSingleObjectDrawSamePad[tabNumber][graphNumber]
               = graphSingleObjectTabDrawSamePad[taskHierarchyClassIndex[i]][j][k].ToBool();
            numOfTabSingleObjects[tabNumber]++;
         }
      }
   }

   Int_t iUsedTab = -1;
   for (i = 0; i < numOfTab; i++) {
      if (!tabUsed[i]) {
         continue;
      }
      iUsedTab++;
      tabUsedIndex[i] = iUsedTab;
      tabSingleObjectIndexMax[i] = 0;
      for (j = 0; j < numOfTabSingleObjects[i]; j++) {
         tabSingleObjectIndexMax[i] = TMath::Max(tabSingleObjectIndex[i][j] + 1 +
                                                 tabSingleObjectArrayIndexEnd[i][j] -
                                                 tabSingleObjectArrayIndexStart[i][j],tabSingleObjectIndexMax[i]);
      }
   }
   // fill task hierarchy index and suffix
   Int_t taskIndex = 0;
   Int_t suffixNumber = 0;
   Int_t multiplicity = 0;
   for (i = 0; i < numOfTaskHierarchy; i++) {
      if (!taskUsed[taskHierarchyClassIndex[i]])
         continue;
      taskHierarchyObjectIndex[i] = taskIndex++;
      suffixNumber = 0;
      multiplicity = 0;
      for (j = 0; j < numOfTaskHierarchy; j++) {
         if (j != i && taskHierarchyName[i] == taskHierarchyName[j]) {
            multiplicity++;
            if (j<i)
               suffixNumber++;
         }
      }
      if (multiplicity > 0) {
         if (multiplicity <= 9)
            taskHierarchySuffix[i].SetFormatted("_%01d",suffixNumber);
         if (multiplicity>9)
            taskHierarchySuffix[i].SetFormatted("_%02d",suffixNumber);
         if (multiplicity>99)
            taskHierarchySuffix[i].SetFormatted("_%03d",suffixNumber);
      } else {
         taskHierarchySuffix[i] = "";
      }
   }

   // fill task hierarchy index and suffix without affiliation
   for (i = 0; i < numOfTaskHierarchyWOAffiliation; i++) {
      suffixNumber = 0;
      multiplicity = 0;
      for (j = 0; j < numOfTaskHierarchyWOAffiliation; j++) {
         if (j != i && taskHierarchyNameWOAffiliation[i] == taskHierarchyNameWOAffiliation[j]) {
            multiplicity++;
            if (j<i)
               suffixNumber++;
         }
      }
      if (multiplicity > 0) {
         if (multiplicity <= 9)
            taskHierarchySuffixWOAffiliation[i].SetFormatted("_%01d",suffixNumber);
         if (multiplicity>9)
            taskHierarchySuffixWOAffiliation[i].SetFormatted("_%02d",suffixNumber);
         if (multiplicity>99)
            taskHierarchySuffixWOAffiliation[i].SetFormatted("_%03d",suffixNumber);
      } else {
         taskHierarchySuffixWOAffiliation[i] = "";
      }
   }

   // fill tab suffix
   for (i = 0; i < numOfTab; i++) {
      if (!tabUsed[i])
         continue;
      suffixNumber = 0;
      multiplicity = 0;
      for (j = 0; j < numOfTab; j++) {
         if (j != i && tabName[i] == tabName[j]) {
            multiplicity++;
            if (j<i)
               suffixNumber++;
         }
      }
      if (multiplicity > 0) {
         if (multiplicity <= 9)
            tabSuffix[i].SetFormatted("_%01d",suffixNumber);
         if (multiplicity > 9)
            tabSuffix[i].SetFormatted("_%02d",suffixNumber);
         if (multiplicity > 99)
            tabSuffix[i].SetFormatted("_%03d",suffixNumber);
      } else {
         tabSuffix[i] = "";
      }
   }

   if (!readGlobalSteeringParameters) {
      if (numOfTaskHierarchy == -1)
         numOfTaskHierarchy++;
      steerName[numOfTask][0] = "GlobalSteering";
      steerParent[numOfTask][0] = -1;
      numOfSteering[numOfTask] = 0;
      numOfSteerFields[numOfTask][0] = 0;
      numOfSteerChildren[numOfTask][0] = 0;
   }

   // Add DAQs to daqArray
   if (numOfEvent > 0 || midas) {
      daqNameArray->AddLast("Midas");
      daqTypeArray->AddLast(shortCut.Data());
      daqDirArray->AddLast("generated/");
   }
   if (numOfTree > 0) {
      daqNameArray->AddLast("Rome");
      daqTypeArray->AddLast(shortCut.Data());
      daqDirArray->AddLast("generated/");
   }
   if (numOfRootTree > 0) {
      daqNameArray->AddLast("Root");
      daqTypeArray->AddLast(shortCut.Data());
      daqDirArray->AddLast("generated/");
   }
   daqNameArray->AddLast("DataBase");
   daqTypeArray->AddLast("ROME");
   daqDirArray->AddLast("");
   if (this->orca) {
      daqNameArray->AddLast("Orca");
      daqTypeArray->AddLast("ROME");
      daqDirArray->AddLast("");
   }
   for (i = 0; i < numOfDAQ; i++) {
      if (!daqUsed[i])
         continue;
      str.SetFormatted("%s",daqName[i].Data());
      daqNameArray->AddLast(str.Data());
      daqTypeArray->AddLast(shortCut.Data());
      daqDirArray->AddLast("daqs/");
   }

   // make directories
   ROMEString path;
   path.SetFormatted("%sobj",outDir.Data());
   gSystem->MakeDirectory(path.Data());
   path.SetFormatted("%sdict",outDir.Data());
   gSystem->MakeDirectory(path.Data());
   path.SetFormatted("%ssrc", outDir.Data());
   gSystem->MakeDirectory(path.Data());
   path.SetFormatted("%sinclude", outDir.Data());
   gSystem->MakeDirectory(path.Data());
   path.SetFormatted("%ssrc/generated", outDir.Data());
   gSystem->MakeDirectory(path.Data());
   path.SetFormatted("%sinclude/generated", outDir.Data());
   gSystem->MakeDirectory(path.Data());
   if (numOfTask > 0) {
      path.SetFormatted("%ssrc/tasks", outDir.Data());
      gSystem->MakeDirectory(path.Data());
      path.SetFormatted("%sinclude/tasks", outDir.Data());
      gSystem->MakeDirectory(path.Data());
   }
   if (numOfTab > 0) {
      path.SetFormatted("%ssrc/tabs", outDir.Data());
      gSystem->MakeDirectory(path.Data());
      path.SetFormatted("%sinclude/tabs", outDir.Data());
      gSystem->MakeDirectory(path.Data());
   }
   if (numOfFolder > 0) {
      path.SetFormatted("%ssrc/folders", outDir.Data());
      gSystem->MakeDirectory(path.Data());
      path.SetFormatted("%sinclude/folders", outDir.Data());
      gSystem->MakeDirectory(path.Data());
   }
   if (numOfDAQ > 0) {
      path.SetFormatted("%ssrc/daqs", outDir.Data());
      gSystem->MakeDirectory(path.Data());
      path.SetFormatted("%sinclude/daqs", outDir.Data());
      gSystem->MakeDirectory(path.Data());
   }
   if (numOfDB > 0) {
      path.SetFormatted("%ssrc/databases", outDir.Data());
      gSystem->MakeDirectory(path.Data());
      path.SetFormatted("%sinclude/databases", outDir.Data());
      gSystem->MakeDirectory(path.Data());
   }
   path.SetFormatted("%sres", outDir.Data());
   gSystem->MakeDirectory(path.Data());
   path.SetFormatted("%sres/xmltoform", outDir.Data());
   gSystem->MakeDirectory(path.Data());
   path.SetFormatted("%sres/documents", outDir.Data());
   gSystem->MakeDirectory(path.Data());

   char* romesys = getenv("ROMESYS");
   oldFile.SetFormatted("%s/res/xmltoform/PadConfigDia.xml", romesys);
   newFile.SetFormatted("%sres/xmltoform/PadConfigDia.xml", outDir.Data());
   CopyFile(oldFile.Data(),newFile.Data());

   // set TString increments
#if ROOT_VERSION_CODE < ROOT_VERSION(5,29,2)
   TString::MaxWaste(kTStringResizeIncrement - 1);
   TString::ResizeIncrement(kTStringResizeIncrement);
#endif

   // write classes
   if (!AddConfigParameters()) return false;
   if (!CheckConfigParameters(mainParGroup)) return false;
   if (!AddConfigParametersFolder()) return false;
   if (makeOutput)
      cout<<"\n\nAnalyzer:"<<endl;
   if (!WriteAnalyzerCpp()) return false;
   if (!WriteAnalyzer2Cpp()) return false;
   if (!WriteAnalyzer3Cpp()) return false;
   if (!WriteAnalyzer4Cpp()) return false;
   if (!WriteAnalyzerH()) return false;
   if (makeOutput)
      cout<<"\n\nWindow:"<<endl;
   if (!WriteWindowCpp()) return false;
   if (!WriteWindow2Cpp()) return false;
   if (!WriteWindowH()) return false;
   if (makeOutput)
      cout<<"\n\nFolders:"<<endl;
   if (!WriteFolderH()) return false;
   if (!WriteFolderCpp()) return false;
   if (!WriteAllFoldersH()) return false;
   if (makeOutput)
      cout<<"\n\nTasks:"<<endl;
   if (!WriteTaskCpp()) return false;
   if (!WriteBaseTaskCpp()) return false;
   if (!WriteTaskH()) return false;
   if (!WriteBaseTaskH()) return false;
   if (makeOutput)
      cout<<"\n\nTabs:"<<endl;
   if (!WriteTabCpp()) return false;
   if (!WriteBaseTabCpp()) return false;
   if (!WriteTabH()) return false;
   if (!WriteBaseTabH()) return false;
   if (makeOutput)
      cout<<"\n\nUser DAQ Systems:"<<endl;
   if (!WriteDAQCpp()) return false;
   if (!WriteDAQH()) return false;
   if (makeOutput)
      cout<<"\n\nUser Database Interfaces:"<<endl;
   if (!WriteDBCpp()) return false;
   if (!WriteDBH()) return false;
   if (makeOutput)
      cout<<"\n\nDatabase Interface:"<<endl;
   if (!WriteDBAccessCpp()) return false;
   if (!WriteDBAccessH()) return false;
   if (makeOutput)
      cout<<"\n\nFramework:"<<endl;
   if (readGlobalSteeringParameters)
      if (!WriteSteering(numOfTask)) return false;
   if (!WriteConfigToFormCpp()) return false;
   if (!WriteConfigToFormH()) return false;
   if (!WriteConfigCpp()) return false;
   if (!WriteConfig2Cpp()) return false;
   if (!WriteConfig3Cpp()) return false;
   if (!WriteConfig4Cpp()) return false;
   if (!WriteConfigH()) return false;
   if (numOfEvent > 0 || midas) {
      if (!WriteMidasDAQCpp()) return false;
      if (!WriteMidasDAQH()) return false;
   }
   if (numOfTree > 0) {
      if (!WriteRomeDAQCpp()) return false;
      if (!WriteRomeDAQH()) return false;
   }
   if (numOfRootTree > 0) {
      if (!WriteRootDAQCpp()) return false;
      if (!WriteRootDAQH()) return false;
      if (!WriteRootDAQClassesH()) return false;
   }
   if (!WriteEventLoopCpp()) return false;
   if (!WriteEventLoopH()) return false;
   if (!WriteNetFolderServerCpp()) return false;
   if (!WriteNetFolderServerH()) return false;
   if (!WriteMain()) return false;
   if (!WriteReadTreesC()) return false;
   if (!WriteDistillTreesC()) return false;
   if (!WriteDOT()) return false;
   if (!WriteVersionH()) return false;

   gSystem->ChangeDirectory(outDir.Data());

// Linking
   WriteMakefile();
#if defined( R__UNIX )
   Int_t n;
   if (noLink) {
      ROMEString tempStr;
#if defined( R__UNIX )
      if (quietMake) {
         tempStr.SetFormatted("make -s");
      } else {
         tempStr.SetFormatted("make");
      }
#endif
#if defined( R__VISUAL_CPLUSPLUS )
      tempStr.SetFormatted("nmake -f Makefile.win");
#endif
      n = dictionaryHeaders->GetEntriesFast();
      if (n > 0) {
         for (i = 0; i < (n - 1) / maxNumberOfClassesInDictionary + 1; i++) {
            tempStr.AppendFormatted(" dict/%sDict%d.cpp",shortCut.Data(), i);
         }
      }
      tempStr.AppendFormatted(" dict/%sUserDict.cpp",shortCut.Data());
      gSystem->Exec(tempStr.Data());
   }
#endif
// Documentation
   if (makeOutput) cout<<"\nWrite HTML Documentation."<<endl;
   WriteHTMLDoku();
   return true;
}

//______________________________________________________________________________
Bool_t ROMEBuilder::ReadCommandLineParameters(int argc, const char *argv[])
{
   makeOutput = false;
   noLink = false;
   midas = false;
   xz = false;
   bzip2 = false;
   orca = false;
   sql = false;
   mysql = false;
   pgsql = false;
   sqlite = false;
   sqlite3 = false;
   noVP = false;
   pch = false;
   minRebuild = false;
   quietMake = false;
   makeFlag = "";
#if defined (HAVE_LIBROMESTATIC)
   librome = kLIBStatic;
#elif defined (HAVE_LIBROMEDYNAMIC)
   librome = kLIBDynamic;
#else
   librome = kLIBNone;
#endif

#if defined( R__MACOSX )
   dynamicLink = kFALSE;
#else
   dynamicLink = kTRUE;
#endif

   char workDir[kMAXPATHLEN];
   strcpy(workDir,gSystem->WorkingDirectory());

   outDir = workDir;
   outDir.Append("/");

   if (argc == 1) {
      Usage();
      return false;
   }
   for (int i = 1; i < argc; i++) {
      // -- only for testing (start) --
      if (!strcmp(argv[i],"-meg")) {
         makeOutput = false;
         midas = true;
         noLink = true;
         minRebuild = true;
         sql = false;
         mysql = true;
         outDir = "C:/meg/meganalyzer/";
         xmlFile = "C:/meg/meganalyzer/MEGAnalyzer.xml";
         affiliations.AddAtAndExpand("meg",0);
      } else if (!strcmp(argv[i],"-meg_test")) {
         makeOutput = false;
         midas = true;
         noLink = true;
         minRebuild = true;
         sql = false;
         mysql = true;
         outDir = "C:/temp/meg_test/meganalyzer/";
         xmlFile = "C:/temp/meg_test/meganalyzer/MEGAnalyzer.xml";
         affiliations.AddAtAndExpand("meg",0);
      } else if (!strcmp(argv[i],"-drs")) {
         makeOutput = false;
         midas = false;
         noLink = true;
         minRebuild = true;
         sql = false;
         mysql = false;
         outDir = "C:/meg/meganalyzer/";
         xmlFile = "C:/meg/meganalyzer/MEGAnalyzer.xml";
         flags.AddAtAndExpand("HAVE_DRS",0);
         flags.AddAtAndExpand("HAVE_USB",1);
//         flags.AddAtAndExpand("HAVE_MSCB",2);
//         flags.AddAtAndExpand("HAVE_AFG3251",3);
//         flags.AddAtAndExpand("HAVE_VME",4);
         affiliations.AddAtAndExpand("drs",0);
      } else if (!strcmp(argv[i],"-drstest")) {
         makeOutput = false;
         midas = false;
         noLink = true;
         minRebuild = true;
         sql = false;
         mysql = false;
         outDir = "C:/meg/meganalyzer/";
         xmlFile = "C:/meg/meganalyzer/MEGAnalyzer.xml";
         affiliations.AddAtAndExpand("drs",0);
         flags.AddAtAndExpand("HAVE_DRS",0);
      } else if (!strcmp(argv[i],"-dch")) {
         makeOutput = false;
         midas = true;
         noLink = true;
         minRebuild = true;
         sql = false;
         mysql = true;
         outDir = "C:/meg/meganalyzer/";
         xmlFile = "C:/meg/meganalyzer/MEGAnalyzer.xml";
         affiliations.AddAtAndExpand("dch",0);
         affiliations.AddAtAndExpand("dchcabling",1);
         affiliations.AddAtAndExpand("dchtic",2);
      } else if (!strcmp(argv[i],"-dchtic")) {
         makeOutput = false;
         midas = true;
         noLink = true;
         minRebuild = true;
         sql = false;
         mysql = true;
         outDir = "C:/meg/meganalyzer/";
         xmlFile = "C:/meg/meganalyzer/MEGAnalyzer.xml";
         affiliations.AddAtAndExpand("dch",0);
         affiliations.AddAtAndExpand("dchtic",1);
         affiliations.AddAtAndExpand("ticp",2);
         affiliations.AddAtAndExpand("ticz",3);
         affiliations.AddAtAndExpand("dchcabling",4);
      } else if (!strcmp(argv[i],"-dchhv")) {
         makeOutput = false;
         midas = true;
         noLink = true;
         minRebuild = true;
         sql = false;
         mysql = true;
         outDir = "C:/meg/meganalyzer/";
         xmlFile = "C:/meg/meganalyzer/MEGAnalyzer.xml";
         affiliations.AddAtAndExpand("dch",0);
         affiliations.AddAtAndExpand("dchcabling",1);
         affiliations.AddAtAndExpand("xechv",2);
      } else if (!strcmp(argv[i],"-drsdch")) {
         makeOutput = false;
         midas = true;
         noLink = true;
         minRebuild = true;
         sql = false;
         sqlite3 = false;
         mysql = true;
         outDir = "C:/meg/meganalyzer/";
         xmlFile = "C:/meg/meganalyzer/MEGAnalyzer.xml";
         affiliations.AddAtAndExpand("dch",0);
         affiliations.AddAtAndExpand("drs",1);
         affiliations.AddAtAndExpand("dchcabling",2);
         flags.AddAtAndExpand("HAVE_DRS",0);
         flags.AddAtAndExpand("HAVE_VME",1);
         flags.AddAtAndExpand("HAVE_AFG3251",2);
//         flags.AddAtAndExpand("HAVE_MSCB",3);
//         flags.AddAtAndExpand("HAVE_USB",4);
      } else if (!strcmp(argv[i],"-stepbystep")) {
         noLink = true;
         minRebuild = true;
         outDir = "C:/rome/examples/stepbystep/";
         xmlFile = "C:/rome/examples/stepbystep/stepbystep.xml";
      } else if (!strcmp(argv[i],"-readwrite")) {
         noLink = true;
         minRebuild = true;
         outDir = "C:/rome/examples/tree_write_read/";
         xmlFile = "C:/rome/examples/tree_write_read/tree_write_read.xml";
      } else if (!strcmp(argv[i],"-readwrite2")) {
         noLink = true;
         minRebuild = true;
         outDir = "C:/rome/examples/tree_write_read2/";
         xmlFile = "C:/rome/examples/tree_write_read2/tree_write_read2.xml";
      } else if (!strcmp(argv[i],"-multi")) {
         noLink = true;
         minRebuild = true;
         outDir = "C:/rome/examples/multirun/";
         xmlFile = "C:/rome/examples/multirun/multirun.xml";
      } else if (!strcmp(argv[i],"-midasdaq")) {
         noLink = true;
         minRebuild = true;
         midas = true;
         outDir = "C:/rome/examples/midas/";
         xmlFile = "C:/rome/examples/midas/midas.xml";
      } else if (!strcmp(argv[i],"-taskhierarchy")) {
         noLink = true;
         minRebuild = true;
         outDir = "C:/rome/examples/taskhierarchy/";
         xmlFile = "C:/rome/examples/taskhierarchy/taskhierarchy.xml";
      } else if (!strcmp(argv[i],"-taskhierarchyandhistos")) {
         noLink = true;
         minRebuild = true;
         outDir = "C:/rome/examples/taskhierarchyandhistos/";
         xmlFile = "C:/rome/examples/taskhierarchyandhistos/taskhierarchyandhistos.xml";
      } else if (!strcmp(argv[i],"-displayobjects")) {
         noLink = true;
         minRebuild = true;
         outDir = "C:/rome/examples/displayobjects/";
         xmlFile = "C:/rome/examples/displayobjects/displayObjects.xml";
      } else if (!strcmp(argv[i],"-netfolder")) {
         noLink = true;
         minRebuild = true;
         outDir = "C:/rome/examples/netfolder/";
         xmlFile = "C:/rome/examples/netfolder/netfolder.xml";
      } else if (!strcmp(argv[i],"-database")) {
         noLink = true;
         minRebuild = true;
         outDir = "C:/rome/examples/dbexample/";
         xmlFile = "C:/rome/examples/dbexample/RDBframework.xml";
      } else if (!strcmp(argv[i],"-histogui")) {
         noLink = true;
         minRebuild = true;
         outDir = "C:/rome/examples/histogui/";
         xmlFile = "C:/rome/examples/histogui/histogui.xml";
      } else if (!strcmp(argv[i],"-rootDAQ")) {
         noLink = true;
         minRebuild = true;
         outDir = "C:/rome/examples/rootDAQ/";
         xmlFile = "C:/rome/examples/rootDAQ/rootDAQ.xml";
      } else if (!strcmp(argv[i],"-romeargus")) {
         noLink = true;
         minRebuild = true;
         outDir = "C:/rome/examples/rome_argus/";
         xmlFile = "C:/rome/examples/rome_argus/rome_argus.xml";
      } else if (!strcmp(argv[i],"-cutg")) {
         noLink = true;
         minRebuild = true;
         outDir = "C:/rome/examples/cutg/";
         xmlFile = "C:/rome/examples/cutg/cutg.xml";
      } else if (!strcmp(argv[i],"-basefolder")) {
         noLink = true;
         minRebuild = true;
         outDir = "C:/rome/examples/basefolder/";
         xmlFile = "C:/rome/examples/basefolder/basefolder.xml";
      } else if (!strcmp(argv[i],"-lp")) {
         makeOutput = true;
         midas = true;
         noLink = true;
         minRebuild = true;
         outDir = "C:/lpframework/";
         xmlFile = "C:/lpframework/lpframework.xml";
      } else if (!strcmp(argv[i],"-laura")) {
         noLink = true;
         minRebuild = true;
         outDir = "C:/Data/Testprojects/laura/";
         xmlFile = "C:/Data/Testprojects/laura/rootDAQClean.xml";
      } else if (!strcmp(argv[i],"-jan")) {
         noLink = true;
         minRebuild = true;
         outDir = "C:/Data/Testprojects/jan/folderProb/";
         xmlFile = "C:/Data/Testprojects/jan/folderProb/danceobj.xml";
      } else if (!strcmp(argv[i],"-gsptest")) {
         noLink = true;
         minRebuild = true;
         outDir = "C:/rome/examples/GSPTest/";
         xmlFile = "C:/rome/examples/GSPTest/gsptest.xml";
         //-- only for testing (end) --
      } else if (!strcmp(argv[i],"-v")) {
         makeOutput = true;
      } else if (!strcmp(argv[i],"-nvp")) {
         noVP = true;
      } else if (!strcmp(argv[i],"-nl")) {
         noLink = true;
      } else if (!strcmp(argv[i],"-dl")) {
#if 1
#if defined(USE_PIC_UPPER) || defined(USE_PIC_LOWER)
#   if defined( R__UNIX )
         dynamicLink = true;
#   else
         cout<<"-dl option is available on Un*x."<<endl;
#   endif
#else
         cout<<"PIC option is necessary to use dynamic link library."<<endl;
#endif
#else
         cout<<"====================================="<<endl;
         cout<<"-dl option is obsolete."<<endl;
         cout<<"Dynamic-link is on by default."<<endl;
         cout<<"It can be disabled by -st option."<<endl;
         cout<<"====================================="<<endl;
#endif
      } else if (!strcmp(argv[i],"-st")) {
         dynamicLink = false;
      } else if (!strcmp(argv[i],"-nosql")) {
         cout<<"-nosql is obsolete. SQL support is off by default."<<endl;
      } else if (!strcmp(argv[i],"-mysql")) {
         mysql = true;
      } else if (!strcmp(argv[i],"-pgsql")) {
         pgsql = true;
      } else if (!strcmp(argv[i],"-sqlite")) {
         sqlite = true;
      } else if (!strcmp(argv[i],"-sqlite3")) {
         sqlite3 = true;
      } else if (!strcmp(argv[i],"-xz")) {
         xz = true;
      } else if (!strcmp(argv[i],"-bzip2")) {
         bzip2 = true;
      } else if (!strcmp(argv[i],"-midas")) {
         midas = true;
         ROMEString midasFile;
         midasFile = getenv("MIDASSYS");
         midasFile.Append("/include/midas.h");
         if (gSystem->AccessPathName(midasFile.Data(),kFileExists)) {
            cout<<"Midas library not found. Have you set the MIDASSYS environment variable ?"<<endl;
            return false;
         }
      } else if (!strcmp(argv[i],"-orca")) {
         orca = true;
      } else if (!strcmp(argv[i],"-minrb")) {
#if defined( R__VISUAL_CPLUSPLUS )
         minRebuild = true;
#   else
         cout<<"Minimal rebuild is only available on windows."<<endl;
#   endif
      } else if (!strcmp(argv[i],"-qm")) {
#if defined( R__VISUAL_CPLUSPLUS )
         cout<<"Quiet make is only available on Unix."<<endl;
#else
         quietMake = true;
#endif
      } else if (!strcmp(argv[i],"-pch")) {
#if defined( __GNUC__ ) && (( __GNUC__ >= 4 ) || (( __GNUC__ == 3 ) && ( __GNUC_MINOR__ >= 4 )))
         pch = true;
#else
         cout<<"Precompiled header is available with GCC 3.4 or later."<<endl;
#endif
      } else if (!strcmp(argv[i],"-nopch")) {
         pch = false;
      } else if (!strcmp(argv[i],"-makeflag")&&i<argc-1) {
         i++;
         makeFlag = argv[i];
         if (makeFlag[0] == '\"' && makeFlag[makeFlag.Length() - 1] == '\"') {
            makeFlag.Remove(makeFlag.Length() - 1, 1);
            makeFlag.Remove(0, 1);
         }
      } else if (!strcmp(argv[i],"-f")&&i<argc-1) {
         i++;
         while (argv[i][0] != '-') {
            flags.AddLast(argv[i]);
            i++;
            if (i>argc-1)
               break;
         }
         i--;
      } else if (!strcmp(argv[i],"-i")&&i<argc-1) {
         xmlFile = argv[i+1];
         i++;
      } else if (!strcmp(argv[i],"-o")&&i<argc-1) {
         outDir = argv[i+1];
         if (outDir[outDir.Length()-1] != '/' && outDir[outDir.Length()-1] != '\\')
            outDir.Append("/");
         i++;
      } else if (!strcmp(argv[i],"-a")&&i<argc-1) {
         i++;
         while (argv[i][0] != '-') {
            affiliations.AddLast(argv[i]);
            i++;
            if (i>argc-1)
               break;
         }
         i--;
      } else if (argv[i][0] == '-') {
         Usage();
         return false;
      } else {
         xmlFile = argv[i];
      }
   }

   sql = (mysql || pgsql || sqlite || sqlite3 );

   return true;
}

//______________________________________________________________________________
Bool_t ROMEBuilder::AddConfigParametersFolder()
{
   // add configuration folder
   ROMEString tmp;
   int i;
   int iDm;
   ROMEString csStr; // check sum string;

   // initialisation
   folderName[numOfFolder].SetFormatted("%sConfigParameters",mainProgName.Data());
   numOfFolderAffiliations[numOfFolder] = 0;
   folderUsed[numOfFolder] = true;
   folderTitle[numOfFolder] = "ConfigParameters added by romebuilder";
   folderVersion[numOfFolder] = mainDefinitionVersion;
   folderArray[numOfFolder] = "1";
   folderDataBase[numOfFolder] = false;
   folderIgnoreTObjectStreamer[numOfFolder] = true;
   folderUserCode[numOfFolder] = false;
   folderDescription[numOfFolder] = "folder for configuration parameters";
   folderShortDescription[numOfFolder] = "";
   numOfFolderAuthors[numOfFolder] = numOfMainAuthors;
   for (i = 0; i < numOfFolderAuthors[numOfFolder]; i++) {
      folderAuthor[numOfFolder][i] = mainAuthor[i];
      folderAuthorInstitute[numOfFolder][i] = mainInstitute[i];
      folderAuthorCollaboration[numOfFolder][i] = mainCollaboration[i];
      folderAuthorEmail[numOfFolder][i] = mainEmail[i];
   }
   numOfFolderInclude[numOfFolder] = 0;
   numOfValue[numOfFolder] = 0;
   folderNoReset[numOfFolder] = true;
   folderNoResetModified[numOfFolder] = false;
   folderHasClassField[numOfFolder] = false;
   folderParentName[numOfFolder] = "GetMainFolder()";
   folderInheritName[numOfFolder] = "";
   folderSupport[numOfFolder] = false;

   // Config file name
   if (numOfValue[numOfFolder] >= maxNumberOfValues) {
      cout<<"Maximal number of fields in folder '"<<folderName[numOfFolder].Data()<<"' reached : "
          <<maxNumberOfValues<<" !"<<endl;
      cout<<"Terminating program."<<endl;
      return false;
   }
   valueName[numOfFolder][numOfValue[numOfFolder]] = "ConfigFileName";
   valueType[numOfFolder][numOfValue[numOfFolder]] = "TString";
   valueComment[numOfFolder][numOfValue[numOfFolder]] = "File name of configuration file";
   valueDimension[numOfFolder][numOfValue[numOfFolder]] = 0;
   valueNoBoundChech[numOfFolder][numOfValue[numOfFolder]] = false;
   valueIsTObject[numOfFolder][numOfValue[numOfFolder]] = false;
   valueInit[numOfFolder][numOfValue[numOfFolder]] = "\"\"";
   for (iDm = 0; iDm < maxNumberOfValueDimension; iDm++)
      valueArray[numOfFolder][numOfValue[numOfFolder]][iDm] = "1";
   valueArraySpecifier[numOfFolder][numOfValue[numOfFolder]] = "";
   valueDBName[numOfFolder][numOfValue[numOfFolder]] = "";
   valueDBPath[numOfFolder][numOfValue[numOfFolder]] = "";
   valueDBIf[numOfFolder][numOfValue[numOfFolder]] = "";
   csStr += valueName[numOfFolder][numOfValue[numOfFolder]];
   csStr += valueType[numOfFolder][numOfValue[numOfFolder]];
   csStr += valueDimension[numOfFolder][numOfValue[numOfFolder]];
   // count fields
   numOfValue[numOfFolder]++;

   // Config file content
   if (numOfValue[numOfFolder] >= maxNumberOfValues) {
      cout<<"Maximal number of fields in folder '"<<folderName[numOfFolder].Data()<<"' reached : "
          <<maxNumberOfValues<<" !"<<endl;
      cout<<"Terminating program."<<endl;
      return false;
   }
   valueName[numOfFolder][numOfValue[numOfFolder]] = "ConfigString";
   valueType[numOfFolder][numOfValue[numOfFolder]] = "TString";
   valueComment[numOfFolder][numOfValue[numOfFolder]] = "Content of configuration file";
   valueDimension[numOfFolder][numOfValue[numOfFolder]] = 0;
   valueNoBoundChech[numOfFolder][numOfValue[numOfFolder]] = false;
   valueIsTObject[numOfFolder][numOfValue[numOfFolder]] = false;
   valueInit[numOfFolder][numOfValue[numOfFolder]] = "\"\"";
   for (iDm = 0; iDm < maxNumberOfValueDimension; iDm++)
      valueArray[numOfFolder][numOfValue[numOfFolder]][iDm] = "1";
   valueArraySpecifier[numOfFolder][numOfValue[numOfFolder]] = "";
   valueDBName[numOfFolder][numOfValue[numOfFolder]] = "";
   valueDBPath[numOfFolder][numOfValue[numOfFolder]] = "";
   valueDBIf[numOfFolder][numOfValue[numOfFolder]] = "";
   csStr += valueName[numOfFolder][numOfValue[numOfFolder]];
   csStr += valueType[numOfFolder][numOfValue[numOfFolder]];
   csStr += valueDimension[numOfFolder][numOfValue[numOfFolder]];
   // count fields
   numOfValue[numOfFolder]++;

   if (hasDependenceCheck || mainDefinitionVersion != "1") {
      // Task active flag
      for (i = 0; i < numOfTaskHierarchyWOAffiliation; i++) {
         if (numOfValue[numOfFolder] >= maxNumberOfValues) {
            cout<<"Maximal number of fields in folder '"<<folderName[numOfFolder].Data()<<"' reached : "
                <<maxNumberOfValues<<" !"<<endl;
            cout<<"Terminating program."<<endl;
            return false;
         }
         tmp.SetFormatted("%s%sTaskActive",taskHierarchyNameWOAffiliation[i].Data(),taskHierarchySuffixWOAffiliation[i].Data());
         valueName[numOfFolder][numOfValue[numOfFolder]] = tmp;
         valueType[numOfFolder][numOfValue[numOfFolder]] = "Bool_t";
         valueComment[numOfFolder][numOfValue[numOfFolder]].SetFormatted("Active flag of %s task",
                                                                         taskHierarchyNameWOAffiliation[i].Data());
         valueDimension[numOfFolder][numOfValue[numOfFolder]] = 0;
         valueNoBoundChech[numOfFolder][numOfValue[numOfFolder]] = false;
         valueIsTObject[numOfFolder][numOfValue[numOfFolder]] = false;
         valueInit[numOfFolder][numOfValue[numOfFolder]] = "kFALSE";
         for (iDm = 0; iDm < maxNumberOfValueDimension; iDm++)
            valueArray[numOfFolder][numOfValue[numOfFolder]][iDm] = "1";
         valueArraySpecifier[numOfFolder][numOfValue[numOfFolder]] = "";
         valueDBName[numOfFolder][numOfValue[numOfFolder]] = "";
         valueDBPath[numOfFolder][numOfValue[numOfFolder]] = "";
         valueDBIf[numOfFolder][numOfValue[numOfFolder]] = "";
         csStr += valueName[numOfFolder][numOfValue[numOfFolder]];
         csStr += valueType[numOfFolder][numOfValue[numOfFolder]];
         csStr += valueDimension[numOfFolder][numOfValue[numOfFolder]];
         // count fields
         numOfValue[numOfFolder]++;
      }
   }

   // count folders
   numOfFolder++;
   if (numOfFolder > maxNumberOfFolders) {
      cout<<"Maximal number of folders reached : "<<maxNumberOfFolders<<" !"<<endl;
      cout<<"Terminating program."<<endl;
      return false;
   }

   return true;
}

//______________________________________________________________________________
Bool_t ROMEBuilder::CheckFileAndPath()
{
   if (gSystem->AccessPathName(xmlFile.Data(),kFileExists)) {
      if ( xmlFile == "") {
         cout<<"No inputfile specified."<<endl;
      } else {
         cout<<"Inputfile '"<<xmlFile.Data()<<"' not found."<<endl;
      }
      return false;
   }
   ROMEString path;
   path = outDir;
   path.Remove(path.Length()-1);
   if (gSystem->AccessPathName(path.Data(),kFileExists)) {
      cout<<"Outputpath '"<<outDir.Data()<<"' not found."<<endl;
      return false;
   }
   return true;
}

//______________________________________________________________________________
void ROMEBuilder::Usage()
{
   cout<<"  -i        Inputfile"<<endl;
   cout<<"  -o        Outputfile path"<<endl;
   cout<<"  -v        Verbose Mode (no Argument)"<<endl;
   cout<<"  -nl       No Linking (no Argument)"<<endl;
#if defined( R__MACOSX )
   cout<<"  -dl       Makes ROME library and link to generated executable binary (no Argument)"<<endl;
   cout<<"  -st       Link all compiled objects to the executable binary instead of linking ROME library (no Argument)"<<endl;
   cout<<"            This option is on by default"<<endl;
#else
   cout<<"  -dl       Makes ROME library and link to generated executable binary (no Argument)"<<endl;
   cout<<"            This option is on by default"<<endl;
   cout<<"  -st       Link all compiled objects to the executable binary instead of linking ROME library (no Argument)"<<endl;
#endif
   cout<<"  -pch      Use precompiled header (no Argument)"<<endl;
   cout<<"  -nopch    Not use precompiled header (no Argument)"<<endl;
#if defined( R__VISUAL_CPLUSPLUS )
   cout<<"  -minrb    Enables minimal rebuild, only on windows (no Argument)"<<endl;
#endif
#if defined( R__UNIX )
   cout<<"  -qm       Quiet make. Not print compile commands in make procedure."<<endl;
#endif
   cout<<"  -midas    Generated program can be connected to a midas online system (no Argument)"<<endl;
   cout<<"  -xz       Generated program can read compressed midas files by xz"<<endl;
   cout<<"  -bzip2    Generated program can read compressed midas files by bzip2"<<endl;
   cout<<"  -orca     Generated program can be connected to a orca DAQ system (no Argument)"<<endl;
   cout<<"  -mysql    Generated program can be connected to a MySQL server (no Argument)"<<endl;
   cout<<"  -pgsql    Generated program can be connected to a PostgreSQL server (no Argument)"<<endl;
   cout<<"  -sqlite   Generated program can be connected to a SQLite database (no Argument)"<<endl;
   cout<<"  -sqlite3  Generated program can be connected to a SQLite3 database (no Argument)"<<endl;
   cout<<"  -f        Compile flags added to the Makefile"<<endl;
   cout<<"  -makeflag Options for make command executed in romebuilder"<<endl;
}

//______________________________________________________________________________
void ROMEBuilder::AnalyzeFileName(const char* file,ROMEString& pathOfFile,ROMEString& nameOfFile,
                                  ROMEString& extensionOfFile)
{
   Ssiz_t ind;
   ROMEString str = file;
   ind = TMath::Max(str.Last('/'),str.Last('\\'));
   pathOfFile = str(0,ind+1);
   str = str(ind+1,str.Length()-ind-1);
   ind = str.Last('.');
   if (ind == -1) {
      nameOfFile = str;
      extensionOfFile = "";
   } else {
      nameOfFile = str(0,ind);
      extensionOfFile = str(ind+1,str.Length()-ind-1);
   }
}

//______________________________________________________________________________
Bool_t ROMEBuilder::WriteFile(const char* filename,const char* body,Int_t nspace, Bool_t backup)
{
   // return true when backup file is created
   bool backupCreated = false;
   fstream * fileStream;
   ROMEString fileBuffer;
   fileBuffer.ReadFile(filename, kTRUE);

   if (fileBuffer != body) {
      if (backup)
         backupCreated = BackUpFile(filename);
      if (!(fileStream = new fstream(filename,ios::out | ios::trunc))) {
         if (makeOutput) cout<<"\n\nError : Failed to open '"<<filename<<"' !!!"<<endl;
         return backupCreated;
      }
      if (makeOutput) cout<<setw(nspace)<< ""<<filename<<endl;
      *fileStream<<body;
      fileStream->close();
      delete fileStream;
   }
   return backupCreated;
}

//______________________________________________________________________________
void ROMEBuilder::setValue(ROMEString* buf,const char *destination,const char *source,const char *type,Int_t version)
{
   buf->Resize(0);
   ROMEString typeStr = type;
   typeStr.StripSpaces();
   // returns code which transformes a source variable of any type into a destination variable of type character
   if (
         typeStr == "char" ||
         typeStr == "unsigned char" ||
         typeStr == "Char_t" ||
         typeStr == "UChar_t" ||

         typeStr == "short" ||
         typeStr == "unsigned short" ||
         typeStr == "Short_t" ||
         typeStr == "UShort_t" ||

         typeStr == "int" ||
         typeStr == "unsigned int" ||
         typeStr == "Int_t" ||
         typeStr == "UInt_t" ||

         typeStr == "long" ||
         typeStr == "unsigned long" ||
         typeStr == "Long_t" ||
         typeStr == "ULong_t" ||

         typeStr == "Style_t" ||
         typeStr == "Marker_t" ||
         typeStr == "Color_t" ||
         typeStr == "Font_t" ||
         typeStr == "Version_t") {
      if (version == 0) {
         buf->AppendFormatted("%s = strtol(%s,&cstop,10)",destination,source);
      } else {
         buf->AppendFormatted("strtol(%s,&cstop,10)",source);
      }
   } else if (
         typeStr == "long long" ||
         typeStr == "unsigned long long" ||
         typeStr == "Long64_t" ||
         typeStr == "ULong64_t") {
      if (version == 0) {
#if defined( R__UNIX )
         buf->AppendFormatted("%s = strtoll(%s,&cstop,10)",destination,source);
#else
         buf->AppendFormatted("%s = _strtoi64(%s,&cstop,10)",destination,source);
#endif
      } else {
#if defined( R__UNIX )
         buf->AppendFormatted("strtol(%s,&cstop,10)",source);
#else
         buf->AppendFormatted("_strtoi64(%s,&cstop,10)",source);
#endif
      }
   } else if (
         typeStr == "bool" ||
         typeStr == "Bool_t") {
      if (version == 0) {
         buf->AppendFormatted("%s = gAnalyzer->strtobool(%s)",destination,source);
      } else {
         buf->AppendFormatted("gAnalyzer->strtobool(%s)",source);
      }
   } else if (
         typeStr == "char*" ||
         typeStr == "unsigned char*" ||
         typeStr == "Char_t*" ||
         typeStr == "UChar_t*" ||

         typeStr == "Option_t" ||
         typeStr == "Text_t") {
      if (version == 0) {
         buf->AppendFormatted("strcpy(%s,%s)",destination,source);
      } else {
         buf->AppendFormatted("%s",source);
      }
   } else if (
         typeStr == "float" ||
         typeStr == "Float_t" ||
         typeStr == "Float16_t" ||

         typeStr == "double" ||
         typeStr == "Double_t" ||
         typeStr == "Double32_t" ||

         typeStr == "Stat_t" ||
         typeStr == "Axis_t") {
      if (version == 0) {
         buf->AppendFormatted("%s = strtod(%s,&cstop)",destination,source);
      } else {
         buf->AppendFormatted("strtod(%s,&cstop)",source);
      }
   } else {
      if (version == 0) {
         buf->AppendFormatted("%s = %s",destination,source);
      } else {
         buf->AppendFormatted("%s",source);
      }
   }
}

//______________________________________________________________________________
bool ROMEBuilder::toMidasODBType(ROMEString& type,ROMEString& midasODBType)
{
   // converts c++ types to midas odb compatible types
   ROMEString typeStr = type;
   typeStr.StripSpaces();
   if (typeStr == "char" ||
       typeStr == "Char_t") {
      midasODBType = "CHAR";
   } else if (typeStr == "unsigned char" ||
              typeStr == "UChar_t") {
      midasODBType = "BYTE";
   } else if (typeStr == "short" ||
              typeStr == "Short_t") {
      midasODBType = "SHORT";
   } else if (typeStr == "unsigned short" ||
              typeStr == "UShort_t") {
      midasODBType = "WORD";
   } else if (typeStr == "int" ||
              typeStr == "Int_t") {
      midasODBType = "INT";
   } else if (typeStr == "unsigned int" ||
              typeStr == "UInt_t") {
      midasODBType = "DWORD";
   } else if (typeStr == "bool" ||
              typeStr == "Bool_t") {
      midasODBType = "BOOL";
   } else if (typeStr == "float" ||
              typeStr == "Float_t") {
      midasODBType = "FLOAT";
   } else if (typeStr == "double" ||
              typeStr == "Double_t") {
      midasODBType = "DOUBLE";
   } else {
      cout<<type<<" no conversion to a midas odb type available"<<endl;
      return false;
   }
   return true;
}

//______________________________________________________________________________
Bool_t ROMEBuilder::isNumber(const char* str)
{
   ROMEString typeStr = str;
   typeStr.StripSpaces();
   if (typeStr != "float"          && typeStr != "Float_t"            && typeStr != "Float16_t" &&
       typeStr != "double"         && typeStr != "Double_t"           && typeStr != "Double32_t" &&
       typeStr != "char"           && typeStr != "Char_t"             &&
       typeStr != "unsigned char"  && typeStr != "UChar_t"            &&
       typeStr != "short"          && typeStr != "Short_t"            &&
       typeStr != "unsigned short" && typeStr != "UShort_t"           &&
       typeStr != "int"            && typeStr != "Int_t"              &&
       typeStr != "unsigned int"   && typeStr != "UInt_t"             &&
       typeStr != "long"           && typeStr != "Long_t"             &&
       typeStr != "unsigned long"  && typeStr != "ULong_t"            &&
       typeStr != "Long64_t"       && typeStr != "ULong64_t"          &&
       typeStr != "long long"      && typeStr != "unsigned long long" &&
       typeStr != "WORD"  &&
       typeStr != "INT"   &&
       typeStr != "DWORD")
      return false;
   return true;
}

//______________________________________________________________________________
Bool_t ROMEBuilder::isFloatingType(const char *type)
{
   ROMEString typeStr = type;
   typeStr.StripSpaces();

   if (
         typeStr == "float" ||
         typeStr == "Float_t" ||
         typeStr == "Float16_t" ||

         typeStr == "double" ||
         typeStr == "Double_t" ||
         typeStr == "Double32_t" ||

         typeStr == "Stat_t" ||
         typeStr == "Axis_t") {
      return true;
   }
   return false;
}

//______________________________________________________________________________
Bool_t ROMEBuilder::isBoolType(const char *type)
{
   ROMEString typeStr = type;
   typeStr.StripSpaces();
   if (typeStr == "bool" || typeStr == "Bool_t") {
      return true;
   }
   return false;
}

//______________________________________________________________________________
Bool_t ROMEBuilder::isIntType(const char *type)
{
// check if the type is valid for array length specifier in TTree.
// unsigned, char, short, Long64_t are not valid.
   ROMEString typeStr = type;
   typeStr.StripSpaces();
   return (typeStr == "int" || typeStr == "Int_t");
}

//______________________________________________________________________________
ROMEString& ROMEBuilder::convertType(const char *value,const char *oldType,const char *newType,
                                     ROMEString& stringBuffer)
{
   int type = 0;
   ROMEString newTypeStr = newType;
   ROMEString oldTypeStr = oldType;
   newTypeStr.StripSpaces();
   oldTypeStr.StripSpaces();
   if (newTypeStr == "int" || newTypeStr == "Int_t") {
      type = 1;
   } else if (newTypeStr == "double" || newTypeStr == "Double_t") {
      type = 2;
   } else if (newTypeStr == "ROMEString&") {
      type = 3;
   } else {
      return stringBuffer;
   }
   if (type == 1 || type == 2) {
      if (isNumber(oldType) || isBoolType(oldType)) {
         if (type == 1) {
            return stringBuffer.SetFormatted("static_cast<int>(%s)",value);
         } else if (type == 2) {
            return stringBuffer.SetFormatted("static_cast<double>(%s)",value);
         }
      } else {
         if (type == 1) {
            stringBuffer.SetFormatted("strtol(%s,&cstop,10)",value);
         } else if (type == 2) {
            stringBuffer.SetFormatted("strtod(%s,&cstop)",value);
         }
         if (oldTypeStr == "std::string") {
            stringBuffer.Insert(strlen(value)+7,".c_str()");
         } else if (oldTypeStr == "ROMEString" || oldTypeStr == "TString") {
            stringBuffer.Insert(strlen(value)+7,".Data()");
         }
         return stringBuffer;
      }
   } else if (type == 3) {
      if (isBoolType(oldType)) {
         return stringBuffer;
      }
      ROMEString tmp = stringBuffer;
      if (oldTypeStr == "std::string") {
         return stringBuffer.SetFormatted("%s = %s.c_str()",tmp.Data(),value);
      } else if (oldTypeStr == "ROMEString" || oldTypeStr == "TString" ||
                 oldTypeStr == "char*" || oldTypeStr == "unsigned char*" ||
                 oldTypeStr == "Char_t*" || oldTypeStr == "UChar_t*") {
         return stringBuffer.SetFormatted("%s = %s",tmp.Data(),value);
      } else if (isFloatingType(oldType)) {
         return stringBuffer.SetFormatted("%s.SetFormatted(\"%%.16g\",%s)",tmp.Data(),value);
      } else {
         return stringBuffer.SetFormatted("%s.SetFormatted(\"%" R_LLD "\",static_cast<Long64_t>(%s))",tmp.Data(),value);
      }
   }
   return stringBuffer;
}

static map<string, int> fgIsFolderCache;
//______________________________________________________________________________
Bool_t ROMEBuilder::isFolder(const char *type)
{
   if (fgIsFolderCache[type] == 1) {
      return kTRUE;
   } else if (fgIsFolderCache[type] == -1) {
      return kFALSE;
   }

   int j;
   ROMEString str;
   Int_t n = -1;
   bool ret = kFALSE;
   for (j = 0; j < numOfFolder && n < 0; j++) {
      if (folderName[j] == type)
         n = j;
      str.SetFormatted("%s*",folderName[j].Data());
      if (str== type)
         n = j;
      str.SetFormatted("%s%s",shortCut.Data(),folderName[j].Data());
      if (str == type)
         n = j;
      str.SetFormatted("%s%s*",shortCut.Data(),folderName[j].Data());
      if (str == type)
         n = j;
   }
   ret = (n >= 0  && folderUsed[n] && FolderToBeGenerated(n));
   fgIsFolderCache[type] = ret ? 1 : -1;

   return ret;
}

//______________________________________________________________________________
Bool_t ROMEBuilder::isTArrayType(const char *type)
{
   int j;
   ROMEString typeStr = type;
   typeStr.StripSpaces();
   ROMEString str;
   const char arrayTypes[][8]
         = {"TArrayC"
            ,"TArrayI"
            ,"TArrayD"
            ,"TArrayL"
            ,"TArrayF"
            ,"TArrayS"
         };

   for (j = 0; j < 6; j++) {
      if (typeStr == arrayTypes[j])
         return true;
      str.SetFormatted("%s*",arrayTypes[j]);
      if (typeStr == str.Data())
         return true;
   }
   return false;
}

//______________________________________________________________________________
const char* ROMEBuilder::TArray2StandardType(const char *type,ROMEString &standardType)
{
   int j;
   ROMEString str;
   ROMEString typeStr = type;
   typeStr.StripSpaces();

   const char arrayTypes[][2][9] =
         {{"TArrayC","Char_t  "},
          {"TArrayI","Int_t   "},
          {"TArrayD","Double_t"},
          {"TArrayL","Long_t  "},
          {"TArrayF","Float_t "},
          {"TArrayS","Short_t "}};

   for (j = 0; j < 6; j++) {
      if (typeStr == arrayTypes[j][0]) {
         standardType = arrayTypes[j][1];
         return standardType.Data();
      }
      str.SetFormatted("%s*", arrayTypes[j][1]);
      if (typeStr == str.Data()) {
         standardType = arrayTypes[j][1];
         return standardType.Data();
      }
   }
   return type;
}


//______________________________________________________________________________
template <class T>
void ROMEBuilder::FreeArray(T* p0)
{
   // memory must be allocated by using AllocateArray.
   if (p0) {
      delete [] p0;
   }
}

//______________________________________________________________________________
template <class T>
void ROMEBuilder::FreeArray(T** p0)
{
   // memory must be allocated by using AllocateArray.
   if (!p0) {
      return;
   }
   Int_t i = 0;
   while (1) {
      if (!p0[i]) {
         break;
      }
      FreeArray(p0[i]);
      i++;
   }
   delete [] p0;
}

//______________________________________________________________________________
template <class T>
void ROMEBuilder::FreeArray(T*** p0)
{
   // memory must be allocated by using AllocateArray.
   if (!p0) {
      return;
   }
   Int_t i = 0;
   while (1) {
      if (!p0[i]) {
         break;
      }
      FreeArray(p0[i]);
      i++;
   }
   delete [] p0;
}

//______________________________________________________________________________
template <class T>
void ROMEBuilder::FreeArray(T**** p0)
{
   // memory must be allocated by using AllocateArray.
   if (!p0) {
      return;
   }
   Int_t i = 0;
   while (1) {
      if (!p0[i]) {
         break;
      }
      FreeArray(p0[i]);
      i++;
   }
   delete [] p0;
}

//______________________________________________________________________________
template <class T>
void ROMEBuilder::FreeArray(T***** p0)
{
   // memory must be allocated by using AllocateArray.
   if (!p0) {
      return;
   }
   Int_t i = 0;
   while (1) {
      if (!p0[i]) {
         break;
      }
      FreeArray(p0[i]);
      i++;
   }
   delete [] p0;
}

//______________________________________________________________________________
template <class T>
void ROMEBuilder::FreeArray(T****** p0)
{
   // memory must be allocated by using AllocateArray.
   if (!p0) {
      return;
   }
   Int_t i = 0;
   while (1) {
      if (!p0[i]) {
         break;
      }
      FreeArray(p0[i]);
      i++;
   }
   delete [] p0;
}

//______________________________________________________________________________
template <class T>
void* ROMEBuilder::AllocateArray(T* p0, Int_t x1, Int_t x2, Int_t x3, Int_t x4, Int_t x5, Int_t x6)
{
   Int_t n = 0;

   T *p1;
   T **p2;
   T ***p3;
   T ****p4;
   T *****p5;
   T ******p6;
   Int_t i;

   while(1) {
      if(x1 >= 0) n++; else break;
      if(x2 >= 0) n++; else break;
      if(x3 >= 0) n++; else break;
      if(x4 >= 0) n++; else break;
      if(x5 >= 0) n++; else break;
      if(x6 >= 0) n++; else break;
      break;
   }

   bool reset = (typeid(T) == typeid(char)) ||
         (typeid(T) == typeid(UChar_t)) ||
         (typeid(T) == typeid(Short_t)) ||
         (typeid(T) == typeid(UShort_t)) ||
         (typeid(T) == typeid(Int_t)) ||
         (typeid(T) == typeid(UInt_t)) ||
         (typeid(T) == typeid(Long64_t)) ||
         (typeid(T) == typeid(ULong64_t)) ||
         (typeid(T) == typeid(Float_t)) ||
         (typeid(T) == typeid(Double_t));

   switch(n) {
   case 1:
      p1 = new T[x1];
      if (reset) memset(static_cast<void*>(p1), 0, sizeof(T) * x1);
      return p1;

   case 2:
      p2 = new T*[x1 + 1];
      for (i = 0; i < x1; i++)
         p2[i] = static_cast<T*>(AllocateArray(p0, x2));
      p2[x1] = 0;
      return p2;

   case 3:
      p3 = new T**[x1 + 1];
      for (i = 0; i < x1; i++)
         p3[i] = static_cast<T**>(AllocateArray(p0, x2, x3));
      p3[x1] = 0;
      return p3;

   case 4:
      p4 = new T***[x1 + 1];
      for (i = 0; i < x1; i++)
         p4[i] = static_cast<T***>(AllocateArray(p0, x2, x3, x4));
      p4[x1] = 0;
      return p4;

   case 5:
      p5 = new T****[x1 + 1];
      for (i = 0; i < x1; i++)
         p5[i] = static_cast<T****>(AllocateArray(p0, x2, x3, x4, x5));
      p5[x1] = 0;
      return p5;

   case 6:
      p6 = new T*****[x1 + 1];
      for (i = 0; i < x1; i++)
         p6[i] = static_cast<T*****>(AllocateArray(p0, x2, x3, x4, x5, x6));
      p6[x1] = 0;
      return p6;
   }

   return 0;
}

//______________________________________________________________________________
void* ROMEBuilder::AllocateInt(Int_t x1, Int_t x2, Int_t x3, Int_t x4, Int_t x5, Int_t x6)
{
   return AllocateArray(static_cast<Int_t*>(0), x1, x2, x3, x4, x5, x6);
}

//______________________________________________________________________________
void* ROMEBuilder::AllocateBool(Int_t x1, Int_t x2, Int_t x3, Int_t x4, Int_t x5, Int_t x6)
{
   return AllocateArray(static_cast<Bool_t*>(0), x1, x2, x3, x4, x5, x6);
}

//______________________________________________________________________________
void* ROMEBuilder::AllocateROMEString(Int_t x1, Int_t x2, Int_t x3, Int_t x4, Int_t x5, Int_t x6)
{
   return AllocateArray(static_cast<ROMEString*>(0), x1, x2, x3, x4, x5, x6);
}

//______________________________________________________________________________
Bool_t ROMEBuilder::CopyFile(const char* oldFileName,const char* newFileName)
{
   fstream *fileStreamO;
   ROMEString fileBuffer;
   if (!fileBuffer.ReadFile(oldFileName, kTRUE)) {
      return false;
   }
   if ((fileStreamO = new fstream(newFileName,ios::out | ios::trunc))) {
      *fileStreamO<<fileBuffer.Data();
      fileStreamO->close();
      delete fileStreamO;
   } else {
      return false;
   }
   return true;
}
