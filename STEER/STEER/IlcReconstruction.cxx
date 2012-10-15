/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id: IlcReconstruction.cxx 58383 2012-08-30 13:47:02Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// class for running the reconstruction                                      //
//                                                                           //
// Clusters and tracks are created for all detectors and all events by       //
// typing:                                                                   //
//                                                                           //
//   IlcReconstruction rec;                                                  //
//   rec.Run();                                                              //
//                                                                           //
// The Run method returns kTRUE in case of successful execution.             //
//                                                                           //
// If the input to the reconstruction are not simulated digits but raw data, //
// this can be specified by an argument of the Run method or by the method   //
//                                                                           //
//   rec.SetInput("...");                                                    //
//                                                                           //
// The input formats and the corresponding argument are:                     //
// - DDL raw data files: directory name, ends with "/"                       //
// - raw data root file: root file name, extension ".root"                   //
// - raw data DATE file: DATE file name, any other non-empty string          //
// - MC root files     : empty string, default                               //
//                                                                           //
// By default all events are reconstructed. The reconstruction can be        //
// limited to a range of events by giving the index of the first and the     //
// last event as an argument to the Run method or by calling                 //
//                                                                           //
//   rec.SetEventRange(..., ...);                                            //
//                                                                           //
// The index -1 (default) can be used for the last event to indicate no      //
// upper limit of the event range.                                           //
//                                                                           //
// In case of raw-data reconstruction the user can modify the default        //
// number of events per digits/clusters/tracks file. In case the option      //
// is not used the number is set 1. In case the user provides 0, than        //
// the number of events is equal to the number of events inside the          //
// raw-data file (i.e. one digits/clusters/tracks file):                     //
//                                                                           //
//   rec.SetNumberOfEventsPerFile(...);                                      //
//                                                                           //
//                                                                           //
// The name of the gilc file can be changed from the default               //
// "gilc.root" by passing it as argument to the IlcReconstruction          //
// constructor or by                                                         //
//                                                                           //
//   rec.SetGIlcFile("...");                                               //
//                                                                           //
// The local reconstruction can be switched on or off for individual         //
// detectors by                                                              //
//                                                                           //
//   rec.SetRunLocalReconstruction("...");                                   //
//                                                                           //
// The argument is a (case sensitive) string with the names of the           //
// detectors separated by a space. The special string "ALL" selects all      //
// available detectors. This is the default.                                 //
//                                                                           //
// The reconstruction of the primary vertex position can be switched off by  //
//                                                                           //
//   rec.SetRunVertexFinder(kFALSE);                                         //
//                                                                           //
// The tracking and the creation of ESD tracks can be switched on for        //
// selected detectors by                                                     //
//                                                                           //
//   rec.SetRunTracking("...");                                              //
//                                                                           //
// Uniform/nonuniform field tracking switches (default: uniform field)       //
//                                                                           //
//   rec.SetUniformFieldTracking(); ( rec.SetUniformFieldTracking(kFALSE); ) //
//                                                                           //
// The filling of additional ESD information can be steered by               //
//                                                                           //
//   rec.SetFillESD("...");                                                  //
//                                                                           //
// Again, for both methods the string specifies the list of detectors.       //
// The default is "ALL".                                                     //
//                                                                           //
// The call of the shortcut method                                           //
//                                                                           //
//   rec.SetRunReconstruction("...");                                        //
//                                                                           //
// is equivalent to calling SetRunLocalReconstruction, SetRunTracking and    //
// SetFillESD with the same detector selecting string as argument.           //
//                                                                           //
// The reconstruction requires digits or raw data as input. For the creation //
// of digits and raw data have a look at the class IlcSimulation.            //
//                                                                           //
// The input data of a detector can be replaced by the corresponding HLT     //
// data by calling (usual detector string)                                   //
// SetUseHLTData("...");                                                     //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TArrayD.h>
#include <TArrayF.h>
#include <TArrayS.h>
#include <TChain.h>
#include <TFile.h>
#include <TGeoGlobalMagField.h>
#include <TGeoManager.h>
#include <TList.h>
#include <TLorentzVector.h>
#include <TMap.h>
#include <TObjArray.h>
#include <TPRegexp.h>
#include <TParameter.h>
#include <TPluginManager.h>
#include <TProof.h>
#include <TProofOutputFile.h>
#include <TROOT.h>
#include <TSystem.h>
#include <THashTable.h>
#include <TGrid.h>
#include <TMessage.h>
#include <TUrl.h>
#include <TRandom.h>
#include <THashList.h>

#include "IlcAlignObj.h"
// #include "IlcAnalysisManager.h"
// #include "IlcAnalysisDataContainer.h"
#include "IlcCDBEntry.h"
#include "IlcCDBManager.h"
#include "IlcCDBStorage.h"
#include "IlcCTPRawStream.h"
#include "IlcCascadeVertexer.h"
#include "IlcCentralTrigger.h"
#include "IlcCodeTimer.h"
#include "IlcDAQ.h"
#include "IlcDetectorRecoParam.h"
#include "IlcESDCaloCells.h"
#include "IlcESDCaloCluster.h"
#include "IlcESDEvent.h"
#include "IlcESDMuonTrack.h"
#include "IlcESDPmdTrack.h"
#include "IlcESDTagCreator.h"
#include "IlcESDVertex.h"
#include "IlcESDcascade.h"
#include "IlcESDfriend.h"
#include "IlcESDkink.h"
#include "IlcESDpid.h"
#include "IlcESDtrack.h"
#include "IlcESDtrack.h"
#include "IlcEventInfo.h"
#include "IlcGRPObject.h"
#include "IlcGRPRecoParam.h"
#include "IlcGenEventHeader.h"
#include "IlcGeomManager.h"
#include "IlcGlobalQADataMaker.h" 
#include "IlcHeader.h"
#include "IlcLog.h"
#include "IlcMagF.h"
#include "IlcMultiplicity.h"
#include "IlcPID.h"
#include "IlcPlaneEff.h"
#include "IlcQAv1.h"
#include "IlcQADataMakerRec.h" 
#include "IlcQAManager.h"
#include "IlcRawVEvent.h"
#include "IlcRawEventHeaderBase.h"
#include "IlcRawHLTManager.h"
#include "IlcRawReaderDate.h"
#include "IlcRawReaderFile.h"
#include "IlcRawReaderRoot.h"
#include "IlcRecoInputHandler.h"
#include "IlcReconstruction.h"
#include "IlcReconstructor.h"
#include "IlcRun.h"
#include "IlcRunInfo.h"
#include "IlcRunLoader.h"
#include "IlcSysInfo.h" // memory snapshots
#include "IlcTrackPointArray.h"
#include "IlcTracker.h"
#include "IlcTriggerClass.h"
#include "IlcTriggerCluster.h"
#include "IlcTriggerIR.h"
#include "IlcTriggerConfiguration.h"
#include "IlcV0vertexer.h"
#include "IlcVertexer.h"
#include "IlcTrackleter.h"
#include "IlcVertexerTracks.h"
#include "IlcTriggerRunScalers.h"
#include "IlcCTPTimeParams.h" 
#include "IlcESDHLTDecision.h"
#include "IlcTriggerInput.h"
#include "IlcLHCData.h"
#include "ARVersion.h"
#include <RVersion.h>
#include <unistd.h>
#include <sys/resource.h>
ClassImp(IlcReconstruction)

//_____________________________________________________________________________
const char* IlcReconstruction::fgkStopEvFName = "_stopEvent_";
const char* IlcReconstruction::fgkDetectorName[IlcReconstruction::kNDetectors] = {"TARGET", "DCH"};

//_____________________________________________________________________________
IlcReconstruction::IlcReconstruction(const char* gIlcFilename) :
  TSelector(),
  fRunVertexFinder(kFALSE),
  fRunVertexFinderTracks(kFALSE),
  fRunHLTTracking(kFALSE),
  fRunMuonTracking(kFALSE),
  fRunV0Finder(kFALSE),
  fRunCascadeFinder(kFALSE),
  fRunMultFinder(kFALSE),
  fStopOnError(kTRUE),
  fWriteAlignmentData(kFALSE),
  fWriteESDfriend(kTRUE),
  fFillTriggerESD(kFALSE),

  fCleanESD(kTRUE),
  fV0DCAmax(3.),
  fV0CsPmin(0.),
  fDmax(50.),
  fZmax(50.),

  fRunLocalReconstruction("ALL"),
  fRunTracking("ALL"),
  fFillESD("ALL"),
  fDeleteRecPoints(""),
  fDeleteDigits(""),
  fLoadCDB(""),
  fUseTrackingErrorsForAlignment(""),
  fGIlcFileName(gIlcFilename),
  fRawInput(""),
  fESDOutput(""),
  fProofOutputFileName(""),
  fProofOutputLocation(""),
  fProofOutputDataset(kFALSE),
  fProofOutputArchive(""),
  fEquipIdMap(""),
  fFirstEvent(0),
  fLastEvent(-1),
  fNumberOfEventsPerFile((UInt_t)-1),
  fFractionFriends(1.),
  fOptions(),
  fLoadAlignFromCDB(kTRUE),
  fLoadAlignData("ALL"),
  fUseHLTData(),
  fRunInfo(NULL),
  fEventInfo(),
  fRunScalers(NULL),
  fCTPTimeParams(NULL),  
  fCTPTimeAlign(NULL),  

  fRunLoader(NULL),
  fRawReader(NULL),
  fParentRawReader(NULL),

  fRecoParam(),

  fSPDTrackleter(NULL),

  fDiamondProfileSPD(NULL),
  fDiamondProfile(NULL),
  fDiamondProfileTPC(NULL),
  fListOfCosmicTriggers(NULL),
  
  fGRPData(NULL),

  fIlcgnObjArray(NULL),
  fCDBUri(),
  fQARefUri(),
  fSpecCDBUri(), 
  fInitCDBCalled(kFALSE),
  fCDBSnapshotMode(kFALSE),
  fSetRunNumberFromDataCalled(kFALSE),
  fQADetectors("ALL"), 
  fQATasks("ALL"), 
  fRunQA(kFALSE),  
  fRunGlobalQA(kFALSE),
  fSameQACycle(kFALSE),
  fInitQACalled(kFALSE), 
  fWriteQAExpertData(kTRUE), 
  fRunPlaneEff(kTRUE),

  fesd(NULL),
  fhltesd(NULL),
  fesdf(NULL),
  ffile(NULL),
  ffileF(NULL),
  ftree(NULL),
  ftreeF(NULL),
  fhlttree(NULL),
  ftVertexer(NULL),
  fIsNewRunLoader(kFALSE),
  fRunIlcEVE(kFALSE),
  fChain(NULL),
  fNall(0),
  fNspecie(0),
  fSspecie(0),
  fNhighPt(0),
  fShighPt(0),
  fUpgradeModule(""),
  fAnalysisMacro(),
// // // //   fAnalysis(0),
  fRecoHandler(0),
  fDeclTriggerClasses(""),
  fStopped(kFALSE),
  fMaxRSS(0),
  fMaxVMEM(0)
{
// create reconstruction object with default parameters
  gGeoManager = NULL;
  
  for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
    fReconstructor[iDet] = NULL;
    fUpgradeMask[iDet]=kFALSE;
    fLoader[iDet] = NULL;
    fTracker[iDet] = NULL;
  }
  for (Int_t iDet = 0; iDet < IlcQAv1::kNDET; iDet++) {
    fQACycles[iDet] = 999999 ;
    fQAWriteExpert[iDet] = kFALSE ; 
  }
  fBeamInt[0][0]=fBeamInt[0][1]=fBeamInt[1][0]=fBeamInt[1][1] = -1;

  IlcPID pid;
}

//_____________________________________________________________________________
IlcReconstruction::IlcReconstruction(const IlcReconstruction& rec) :
  TSelector(),
  fRunVertexFinder(rec.fRunVertexFinder),
  fRunVertexFinderTracks(rec.fRunVertexFinderTracks),
  fRunHLTTracking(rec.fRunHLTTracking),
  fRunMuonTracking(rec.fRunMuonTracking),
  fRunV0Finder(rec.fRunV0Finder),
  fRunCascadeFinder(rec.fRunCascadeFinder),
  fRunMultFinder(rec.fRunMultFinder),
  fStopOnError(rec.fStopOnError),
  fWriteAlignmentData(rec.fWriteAlignmentData),
  fWriteESDfriend(rec.fWriteESDfriend),
  fFillTriggerESD(rec.fFillTriggerESD),

  fCleanESD(rec.fCleanESD),
  fV0DCAmax(rec.fV0DCAmax),
  fV0CsPmin(rec.fV0CsPmin),
  fDmax(rec.fDmax),
  fZmax(rec.fZmax),

  fRunLocalReconstruction(rec.fRunLocalReconstruction),
  fRunTracking(rec.fRunTracking),
  fFillESD(rec.fFillESD),
  fDeleteRecPoints(""),
  fDeleteDigits(""),
  fLoadCDB(rec.fLoadCDB),
  fUseTrackingErrorsForAlignment(rec.fUseTrackingErrorsForAlignment),
  fGIlcFileName(rec.fGIlcFileName),
  fRawInput(rec.fRawInput),
  fESDOutput(rec.fESDOutput),
  fProofOutputFileName(rec.fProofOutputFileName),
  fProofOutputLocation(rec.fProofOutputLocation),
  fProofOutputDataset(rec.fProofOutputDataset),
  fProofOutputArchive(rec.fProofOutputArchive),
  fEquipIdMap(rec.fEquipIdMap),
  fFirstEvent(rec.fFirstEvent),
  fLastEvent(rec.fLastEvent),
  fNumberOfEventsPerFile(rec.fNumberOfEventsPerFile),
  fFractionFriends(rec.fFractionFriends),
  fOptions(),
  fLoadAlignFromCDB(rec.fLoadAlignFromCDB),
  fLoadAlignData(rec.fLoadAlignData),
  fUseHLTData(rec.fUseHLTData),
  fRunInfo(NULL),
  fEventInfo(),
  fRunScalers(NULL),
  fCTPTimeParams(NULL),
  fCTPTimeAlign(NULL),

  fRunLoader(NULL),
  fRawReader(NULL),
  fParentRawReader(NULL),

  fRecoParam(rec.fRecoParam),

  fSPDTrackleter(NULL),

  fDiamondProfileSPD(rec.fDiamondProfileSPD),
  fDiamondProfile(rec.fDiamondProfile),
  fDiamondProfileTPC(rec.fDiamondProfileTPC),
  fListOfCosmicTriggers(NULL),
  
  fGRPData(NULL),

  fIlcgnObjArray(rec.fIlcgnObjArray),
  fCDBUri(rec.fCDBUri),
  fQARefUri(rec.fQARefUri),
  fSpecCDBUri(), 
  fInitCDBCalled(rec.fInitCDBCalled),
  fCDBSnapshotMode(rec.fCDBSnapshotMode),
  fSetRunNumberFromDataCalled(rec.fSetRunNumberFromDataCalled),
  fQADetectors(rec.fQADetectors), 
  fQATasks(rec.fQATasks), 
  fRunQA(rec.fRunQA),  
  fRunGlobalQA(rec.fRunGlobalQA),
  fSameQACycle(rec.fSameQACycle),
  fInitQACalled(rec.fInitQACalled),
  fWriteQAExpertData(rec.fWriteQAExpertData), 
  fRunPlaneEff(rec.fRunPlaneEff),

  fesd(NULL),
  fhltesd(NULL),
  fesdf(NULL),
  ffile(NULL),
  ffileF(NULL),
  ftree(NULL),
  ftreeF(NULL),
  fhlttree(NULL),
  ftVertexer(NULL),
  fIsNewRunLoader(rec.fIsNewRunLoader),
  fRunIlcEVE(kFALSE),
  fChain(NULL),
  fNall(0),
  fNspecie(0),
  fSspecie(0),
  fNhighPt(0),
  fShighPt(0),
  fUpgradeModule(""),
  fAnalysisMacro(rec.fAnalysisMacro),
// // // //   fAnalysis(0),
  fRecoHandler(0),
  fDeclTriggerClasses(rec.fDeclTriggerClasses),
  fStopped(kFALSE),
  fMaxRSS(0),
  fMaxVMEM(0)
{
// copy constructor

  for (Int_t i = 0; i < rec.fOptions.GetEntriesFast(); i++) {
    if (rec.fOptions[i]) fOptions.Add(rec.fOptions[i]->Clone());
  }
  for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
    fReconstructor[iDet] = NULL;
    fUpgradeMask[iDet] = kFALSE;
    fLoader[iDet] = NULL;
    fTracker[iDet] = NULL;
  }  
  
  for (Int_t iDet = 0; iDet < IlcQAv1::kNDET; iDet++) {
    fQACycles[iDet] = rec.fQACycles[iDet];
    fQAWriteExpert[iDet] = rec.fQAWriteExpert[iDet] ; 
  }

  for (Int_t i = 0; i < rec.fSpecCDBUri.GetEntriesFast(); i++) {
    if (rec.fSpecCDBUri[i]) fSpecCDBUri.Add(rec.fSpecCDBUri[i]->Clone());
  }

  for (int i=2;i--;) for (int j=2;j--;) fBeamInt[i][j] = rec.fBeamInt[i][j];

}

//_____________________________________________________________________________
IlcReconstruction& IlcReconstruction::operator = (const IlcReconstruction& rec)
{
// assignment operator
// Used in PROOF mode
// Be very careful while modifing it!
// Simple rules to follow:
// for persistent data members - use their assignment operators
// for non-persistent ones - do nothing or take the default values from constructor
// TSelector members should not be touched
  if(&rec == this) return *this;

  fRunVertexFinder       = rec.fRunVertexFinder;
  fRunVertexFinderTracks = rec.fRunVertexFinderTracks;
  fRunHLTTracking        = rec.fRunHLTTracking;
  fRunMuonTracking       = rec.fRunMuonTracking;
  fRunV0Finder           = rec.fRunV0Finder;
  fRunCascadeFinder      = rec.fRunCascadeFinder;
  fRunMultFinder         = rec.fRunMultFinder;
  fStopOnError           = rec.fStopOnError;
  fWriteAlignmentData    = rec.fWriteAlignmentData;
  fWriteESDfriend        = rec.fWriteESDfriend;
  fFillTriggerESD        = rec.fFillTriggerESD;

  fCleanESD  = rec.fCleanESD;
  fV0DCAmax  = rec.fV0DCAmax;
  fV0CsPmin  = rec.fV0CsPmin;
  fDmax      = rec.fDmax;
  fZmax      = rec.fZmax;

  fRunLocalReconstruction        = rec.fRunLocalReconstruction;
  fRunTracking                   = rec.fRunTracking;
  fFillESD                       = rec.fFillESD;
  fDeleteRecPoints               = rec.fDeleteRecPoints;
  fDeleteDigits                  = rec.fDeleteDigits;
  fLoadCDB                       = rec.fLoadCDB;
  fUseTrackingErrorsForAlignment = rec.fUseTrackingErrorsForAlignment;
  fGIlcFileName                = rec.fGIlcFileName;
  fRawInput                      = rec.fRawInput;
  fESDOutput                     = rec.fESDOutput;
  fProofOutputFileName           = rec.fProofOutputFileName;
  fProofOutputLocation           = rec.fProofOutputLocation;
  fProofOutputDataset            = rec.fProofOutputDataset;
  fProofOutputArchive            = rec.fProofOutputArchive;
  fEquipIdMap                    = rec.fEquipIdMap;
  fFirstEvent                    = rec.fFirstEvent;
  fLastEvent                     = rec.fLastEvent;
  fNumberOfEventsPerFile         = rec.fNumberOfEventsPerFile;
  fFractionFriends               = rec.fFractionFriends;

  for (Int_t i = 0; i < rec.fOptions.GetEntriesFast(); i++) {
    if (rec.fOptions[i]) fOptions.Add(rec.fOptions[i]->Clone());
  }

  fLoadAlignFromCDB              = rec.fLoadAlignFromCDB;
  fLoadAlignData                 = rec.fLoadAlignData;
  fUseHLTData                    = rec.fUseHLTData;

  delete fRunInfo; fRunInfo = NULL;
  if (rec.fRunInfo) fRunInfo = new IlcRunInfo(*rec.fRunInfo);

  fEventInfo                     = rec.fEventInfo;

  delete fRunScalers; fRunScalers = NULL;
  if (rec.fRunScalers) fRunScalers = new IlcTriggerRunScalers(*rec.fRunScalers); 

  delete fCTPTimeParams; fCTPTimeParams = NULL;
  if (rec.fCTPTimeParams) fCTPTimeParams = new IlcCTPTimeParams(*rec.fCTPTimeParams);
  delete fCTPTimeAlign; fCTPTimeAlign = NULL;
  if (rec.fCTPTimeAlign) fCTPTimeAlign = new IlcCTPTimeParams(*rec.fCTPTimeAlign);

  fRunLoader       = NULL;
  fRawReader       = NULL;
  fParentRawReader = NULL;

  fRecoParam = rec.fRecoParam;

  for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
    fUpgradeMask[iDet] = kFALSE;
    delete fReconstructor[iDet]; fReconstructor[iDet] = NULL;
    delete fLoader[iDet]; fLoader[iDet] = NULL;
    delete fTracker[iDet]; fTracker[iDet] = NULL;
  }
  
  for (Int_t iDet = 0; iDet < IlcQAv1::kNDET; iDet++) {
    fQACycles[iDet] = rec.fQACycles[iDet];
    fQAWriteExpert[iDet] = rec.fQAWriteExpert[iDet] ;
  } 

  delete fSPDTrackleter; fSPDTrackleter = NULL;
    
  delete fDiamondProfileSPD; fDiamondProfileSPD = NULL;
  if (rec.fDiamondProfileSPD) fDiamondProfileSPD = new IlcESDVertex(*rec.fDiamondProfileSPD);
  delete fDiamondProfile; fDiamondProfile = NULL;
  if (rec.fDiamondProfile) fDiamondProfile = new IlcESDVertex(*rec.fDiamondProfile);
  delete fDiamondProfileTPC; fDiamondProfileTPC = NULL;
  if (rec.fDiamondProfileTPC) fDiamondProfileTPC = new IlcESDVertex(*rec.fDiamondProfileTPC);

  delete fListOfCosmicTriggers; fListOfCosmicTriggers = NULL;
  if (rec.fListOfCosmicTriggers) fListOfCosmicTriggers = (THashTable*)((rec.fListOfCosmicTriggers)->Clone());

  delete fGRPData; fGRPData = NULL;
  //  if (rec.fGRPData) fGRPData = (TMap*)((rec.fGRPData)->Clone());
  if (rec.fGRPData) fGRPData = (IlcGRPObject*)((rec.fGRPData)->Clone());

  delete fIlcgnObjArray; fIlcgnObjArray = NULL;

  fCDBUri        = "";
  fQARefUri      = rec.fQARefUri;
  fSpecCDBUri.Delete();
  fInitCDBCalled               = rec.fInitCDBCalled;
  fCDBSnapshotMode             = rec.fCDBSnapshotMode;
  fSetRunNumberFromDataCalled  = rec.fSetRunNumberFromDataCalled;
  fQADetectors                 = rec.fQADetectors;
  fQATasks                     = rec.fQATasks; 
  fRunQA                       = rec.fRunQA;  
  fRunGlobalQA                 = rec.fRunGlobalQA;
  fSameQACycle                 = rec.fSameQACycle;
  fInitQACalled                = rec.fInitQACalled;
  fWriteQAExpertData           = rec.fWriteQAExpertData;
  fRunPlaneEff                 = rec.fRunPlaneEff;
  for (int i=2;i--;) for (int j=2;j--;) fBeamInt[i][j] = rec.fBeamInt[i][j];
  fesd     = NULL;
  fhltesd  = NULL;
  fesdf    = NULL;
  ffile    = NULL;
  ffileF   = NULL;
  ftree    = NULL;
  ftreeF   = NULL;
  fhlttree = NULL;
  ftVertexer = NULL;
  fIsNewRunLoader = rec.fIsNewRunLoader;
  fRunIlcEVE = kFALSE;
  fChain = NULL;
  fNall = 0;
  fNspecie = 0;
  fSspecie = 0;
  fNhighPt = 0;
  fShighPt = 0;
  fUpgradeModule="";
  fAnalysisMacro = rec.fAnalysisMacro;
// // // //   fAnalysis = 0;
  fRecoHandler = 0;
  fDeclTriggerClasses = rec.fDeclTriggerClasses;

  return *this;
}

//_____________________________________________________________________________
IlcReconstruction::~IlcReconstruction()
{
// clean up

  CleanUp();
  if (fListOfCosmicTriggers) {
    fListOfCosmicTriggers->Delete();
    delete fListOfCosmicTriggers;
  }
  delete fGRPData;
  delete fRunScalers;
  delete fCTPTimeParams;
  delete fCTPTimeAlign;
  fOptions.Delete();
  if (fIlcgnObjArray) {
    fIlcgnObjArray->Delete();
    delete fIlcgnObjArray;
  }
  fSpecCDBUri.Delete();

  IlcCodeTimer::Instance()->Print();
}

//_____________________________________________________________________________
void IlcReconstruction::InitQA()
{
  //Initialize the QA and start of cycle 
  IlcCodeTimerAuto("",0);
  
  if (fInitQACalled) return;
  fInitQACalled = kTRUE;
  
  if (fGRPData) IlcQADataMaker::SetCloningRequest( fGRPData->GetQATrigClasses(), fGRPData->GetQACloningRequest());


  IlcQAManager * qam = IlcQAManager::QAManager(IlcQAv1::kRECMODE) ; 
  if (fWriteQAExpertData)
    qam->SetWriteExpert() ; 
 
  if (qam->IsDefaultStorageSet()) {
    IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    IlcWarning("Default QA reference storage has been already set !");
    IlcWarning(Form("Ignoring the default storage declared in IlcReconstruction: %s",fQARefUri.Data()));
    IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    fQARefUri = qam->GetDefaultStorage()->GetURI();
  } else {
    if (fQARefUri.Length() > 0) {
    	IlcDebug(2,"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    	IlcDebug(2, Form("Default QA reference storage is set to: %s", fQARefUri.Data()));
    	IlcDebug(2, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      } else {
        fQARefUri="local://$ILC_ROOT/QAref";
        IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        IlcWarning("Default QA refeference storage not yet set !!!!");
        IlcWarning(Form("Setting it now to: %s", fQARefUri.Data()));
        IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    		
      }
    qam->SetDefaultStorage(fQARefUri);
  }
  
  if (fRunQA) {
  qam->SetActiveDetectors(fQADetectors) ; 
  for (Int_t det = 0 ; det < IlcQAv1::kNDET ; det++) {
    qam->SetCycleLength(IlcQAv1::DETECTORINDEX_t(det), fQACycles[det]) ;  
    qam->SetWriteExpert(IlcQAv1::DETECTORINDEX_t(det)) ;
  }
  if (!fRawReader && !fInput && IsInTasks(IlcQAv1::kRAWS))
    fQATasks.ReplaceAll(Form("%d",IlcQAv1::kRAWS), "") ;
  qam->SetTasks(fQATasks) ; 
  qam->InitQADataMaker(IlcCDBManager::Instance()->GetRun()) ; 
  }
  if (fRunGlobalQA) {
    Bool_t sameCycle = kFALSE ;
    IlcQADataMaker *qadm = qam->GetQADataMaker(IlcQAv1::kGLOBAL);
    IlcInfo(Form("Initializing the global QA data maker"));
    if (IsInTasks(IlcQAv1::kRECPOINTS)) {
      qadm->StartOfCycle(IlcQAv1::kRECPOINTS, IlcCDBManager::Instance()->GetRun(), sameCycle) ; 
      TObjArray **arr=qadm->Init(IlcQAv1::kRECPOINTS);
      IlcTracker::SetResidualsArray(arr);
      sameCycle = kTRUE ; 
    }
    if (IsInTasks(IlcQAv1::kESDS)) {
      qadm->StartOfCycle(IlcQAv1::kESDS, IlcCDBManager::Instance()->GetRun(), sameCycle) ; 
      qadm->Init(IlcQAv1::kESDS);
    }
  }
    IlcSysInfo::AddStamp("InitQA") ; 
}

//_____________________________________________________________________________
void IlcReconstruction::MergeQA(const char *fileName)
{
  //Initialize the QA and start of cycle 
  IlcCodeTimerAuto("",0) ;
  IlcQAManager::QAManager()->Merge(IlcCDBManager::Instance()->GetRun(),fileName) ; 
  IlcSysInfo::AddStamp("MergeQA") ; 
}
  
//_____________________________________________________________________________
void IlcReconstruction::InitCDB()
{
// activate a default CDB storage
// First check if we have any CDB storage set, because it is used 
// to retrieve the calibration and alignment constants

  IlcWarning("InitCDB method disabled");
  return;

  IlcCodeTimerAuto("",0);

  if (fInitCDBCalled) return;
  fInitCDBCalled = kTRUE;

  IlcCDBManager* man = IlcCDBManager::Instance();
  if (man->IsDefaultStorageSet())
  {
    IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    IlcWarning("Default CDB storage has been already set !");
    IlcWarning(Form("Ignoring the default storage declared in IlcReconstruction: %s",fCDBUri.Data()));
    IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    fCDBUri = man->GetDefaultStorage()->GetURI();
  }
  else {
    if (fCDBUri.Length() > 0) 
    {
    	IlcDebug(2,"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    	IlcDebug(2, Form("Default CDB storage is set to: %s", fCDBUri.Data()));
    	IlcDebug(2, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	man->SetDefaultStorage(fCDBUri);
    } 
    else if (!man->GetRaw()){
	fCDBUri="local://$ILC_ROOT/OCDB";
    	IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    	IlcWarning("Default CDB storage not yet set !!!!");
    	IlcWarning(Form("Setting it now to: %s", fCDBUri.Data()));
    	IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	man->SetDefaultStorage(fCDBUri);
    }
    else {    
    	IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	IlcWarning("Default storage will be set after setting the Run Number!!!");
    	IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");    		
    }
  }

  // Now activate the detector specific CDB storage locations
  for (Int_t i = 0; i < fSpecCDBUri.GetEntriesFast(); i++) {
    TObject* obj = fSpecCDBUri[i];
    if (!obj) continue;
    IlcDebug(2, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    IlcDebug(2, Form("Specific CDB storage for %s is set to: %s",obj->GetName(),obj->GetTitle()));
    IlcDebug(2, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    man->SetSpecificStorage(obj->GetName(), obj->GetTitle());
  }
  IlcSysInfo::AddStamp("InitCDB");
}

//_____________________________________________________________________________
void IlcReconstruction::SetCDBSnapshotMode(const char* snapshotFileName) {
    fCDBSnapshotMode = kTRUE;
    IlcCDBManager::Instance()->SetSnapshotMode(snapshotFileName);
}

//_____________________________________________________________________________
void IlcReconstruction::SetDefaultStorage(const char* uri) {
// Store the desired default CDB storage location
// Activate it later within the Run() method

  fCDBUri = uri;

}

//_____________________________________________________________________________
void IlcReconstruction::SetQARefDefaultStorage(const char* uri) {
  // Store the desired default CDB storage location
  // Activate it later within the Run() method
  
  fQARefUri = uri;
  IlcQAv1::SetQARefStorage(fQARefUri.Data()) ;
  
}
//_____________________________________________________________________________
void IlcReconstruction::SetSpecificStorage(const char* calibType, const char* uri) {
// Store a detector-specific CDB storage location
// Activate it later within the Run() method

  IlcCDBPath aPath(calibType);
  if(!aPath.IsValid()){
	// if calibType is not wildcard but it is a valid detector, add "/*" to make it a valid path
	for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
		if(!strcmp(calibType, fgkDetectorName[iDet])) {
			aPath.SetPath(Form("%s/*", calibType));
			IlcInfo(Form("Path for specific storage set to %s", aPath.GetPath().Data()));
			break;
		}
        }
	if(!aPath.IsValid()){
  		IlcError(Form("Not a valid path or detector: %s", calibType));
  		return;
	}
  }

//  // check that calibType refers to a "valid" detector name
//  Bool_t isDetector = kFALSE;
//  for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
//    TString detName = fgkDetectorName[iDet];
//    if(aPath.GetLevel0() == detName) {
//    	isDetector = kTRUE;
//	break;
//    }
//  }
//
//  if(!isDetector) {
//	IlcError(Form("Not a valid detector: %s", aPath.GetLevel0().Data()));
//	return;
//  }

  TObject* obj = fSpecCDBUri.FindObject(aPath.GetPath().Data());
  if (obj) fSpecCDBUri.Remove(obj);
  fSpecCDBUri.Add(new TNamed(aPath.GetPath().Data(), uri));

}

//_____________________________________________________________________________
Bool_t IlcReconstruction::SetRunNumberFromData()
{
  // The method is called in Run() in order
  // to set a correct run number.
  // In case of raw data reconstruction the
  // run number is taken from the raw data header

  if (fSetRunNumberFromDataCalled) return kTRUE;
  fSetRunNumberFromDataCalled = kTRUE;
  
  IlcCDBManager* man = IlcCDBManager::Instance();
 
  if(fRawReader) {
    if(fRawReader->NextEvent()) {
      if(man->GetRun() > 0) {
  	IlcWarning("Run number is taken from raw-event header! Ignoring settings in IlcCDBManager!");
      } 
      man->SetRun(fRawReader->GetRunNumber());
      GetEventInfo();
      fRawReader->RewindEvents();
    }
    else {
      if(man->GetRun() > 0) {
	IlcWarning("No raw-data events are found ! Using settings in IlcCDBManager !");
      }
      else {
	IlcWarning("Neither raw events nor settings in IlcCDBManager are found !");
	return kFALSE;
      }
    }
  }
  else {
    IlcRunLoader *rl = IlcRunLoader::Open(fGIlcFileName.Data());
    if (!rl) {
      IlcError(Form("No run loader found in file %s", fGIlcFileName.Data()));
      return kFALSE;
    }
    else {
      rl->LoadHeader();
      // read run number from gIlc
      if(rl->GetHeader()) {
	man->SetRun(rl->GetHeader()->GetRun());
	rl->UnloadHeader();
	delete rl;
      }
      else {
	IlcError("Neither run-loader header nor RawReader objects are found !");
	delete rl;
	return kFALSE;
      }
    }
  }

  man->Print();  
  
  return kTRUE;
}

//_____________________________________________________________________________
void IlcReconstruction::SetCDBLock() {
  // Set CDB lock: from now on it is forbidden to reset the run number
  // or the default storage or to activate any further storage!
  
  IlcWarning("SetCDBLock method disabled");
  return;

  IlcCDBManager::Instance()->SetLock(1);
}

//_____________________________________________________________________________
void IlcReconstruction::MatchUpgradeDetector() {
  // Translates detector name in a boolean.
  // The boolean is used in GetReconstructor to load the 
  // upgrade reconstructor instead of the standard one.
   for(Int_t iDet = 0; iDet < kNDetectors; iDet++) {
    if(fUpgradeModule.Contains(fgkDetectorName[iDet])) fUpgradeMask[iDet]=kTRUE;
   }
}
//_____________________________________________________________________________
Bool_t IlcReconstruction::MisalignGeometry(const TString& detectors)
{

  IlcWarning("MisalignGeometry method disabled");
  return kTRUE;

  // Read the alignment objects from CDB.
  // Each detector is supposed to have the
  // alignment objects in DET/Align/Data CDB path.
  // All the detector objects are then collected,
  // sorted by geometry level (starting from ILCM) and
  // then applied to the TGeo geometry.
  // Finally an overlaps check is performed.

  // Load alignment data from CDB and fill fIlcgnObjArray 
  if(fLoadAlignFromCDB){
  	
    TString detStr = detectors;
    TString loadAlObjsListOfDets = "";
    
    for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
      if(!IsSelected(fgkDetectorName[iDet], detStr)) continue;
      if(!strcmp(fgkDetectorName[iDet],"HLT")) continue;
      
      if(IlcGeomManager::GetNalignable(fgkDetectorName[iDet]) != 0)
      {
	loadAlObjsListOfDets += fgkDetectorName[iDet];
	loadAlObjsListOfDets += " ";
      }
    } // end loop over detectors
    
    if(IlcGeomManager::GetNalignable("GRP") != 0)
      loadAlObjsListOfDets.Prepend("GRP "); //add alignment objects for non-sensitive modules
    IlcGeomManager::ApplyAlignObjsFromCDB(loadAlObjsListOfDets.Data());
  }else{
    // Check if the array with alignment objects was
    // provided by the user. If yes, apply the objects
    // to the present TGeo geometry
    if (fIlcgnObjArray) {
      if (gGeoManager && gGeoManager->IsClosed()) {
	if (IlcGeomManager::ApplyAlignObjsToGeom(*fIlcgnObjArray) == kFALSE) {
	  IlcError("The misalignment of one or more volumes failed!"
		   "Compare the list of simulated detectors and the list of detector alignment data!");
	  return kFALSE;
	}
      }
      else {
	IlcError("Can't apply the misalignment! gGeoManager doesn't exist or it is still opened!");
	return kFALSE;
      }
    }
  }
  
  if (fIlcgnObjArray) {
    fIlcgnObjArray->Delete();
    delete fIlcgnObjArray; fIlcgnObjArray=NULL;
  }

  return kTRUE;
}

//_____________________________________________________________________________
void IlcReconstruction::SetGIlcFile(const char* fileName)
{
// set the name of the gilc file

  fGIlcFileName = fileName;
}

//_____________________________________________________________________________
void IlcReconstruction::SetInput(const char* input) 
{
  // In case the input string starts with 'mem://', we run in an online mode
  // and IlcRawReaderDateOnline object is created. In all other cases a raw-data
  // file is assumed. One can give as an input:
  // mem://: - events taken from DAQ monitoring libs online
  //  or
  // mem://<filename> - emulation of the above mode (via DATE monitoring libs)
  if (input) fRawInput = input;
}

//_____________________________________________________________________________
void IlcReconstruction::SetOutput(const char* output) 
{
  // Set the output ESD filename
  // 'output' is a normalt ROOT url
  // The method is used in case of raw-data reco with PROOF
  if (output) fESDOutput = output;
}

//_____________________________________________________________________________
void IlcReconstruction::SetOption(const char* detector, const char* option)
{
// set options for the reconstruction of a detector

  TObject* obj = fOptions.FindObject(detector);
  if (obj) fOptions.Remove(obj);
  fOptions.Add(new TNamed(detector, option));
}

//_____________________________________________________________________________
void IlcReconstruction::SetRecoParam(const char* detector, IlcDetectorRecoParam *par)
{
  // Set custom reconstruction parameters for a given detector
  // Single set of parameters for all the events

  // First check if the reco-params are global
  if(!strcmp(detector, "GRP")) {
    par->SetAsDefault();
    fRecoParam.AddDetRecoParam(kNDetectors,par);
    return;
  }

  for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
    if(!strcmp(detector, fgkDetectorName[iDet])) {
      par->SetAsDefault();
      fRecoParam.AddDetRecoParam(iDet,par);
      break;
    }
  }

}

//_____________________________________________________________________________
Bool_t IlcReconstruction::InitGRP() {
  //------------------------------------
  // Initialization of the GRP entry 
  //------------------------------------

  IlcWarning("InitGRP method disabled");
  return kTRUE;

  IlcCDBEntry* entry = IlcCDBManager::Instance()->Get("GRP/GRP/Data");

  if (entry) {

    TMap* m = dynamic_cast<TMap*>(entry->GetObject());  // old GRP entry

    if (m) {
       IlcInfo("Found a TMap in GRP/GRP/Data, converting it into an IlcGRPObject");
       m->Print();
       fGRPData = new IlcGRPObject();
       fGRPData->ReadValuesFromMap(m);
    }

    else {
       IlcInfo("Found an IlcGRPObject in GRP/GRP/Data, reading it");
       fGRPData = dynamic_cast<IlcGRPObject*>(entry->GetObject());  // new GRP entry
       entry->SetOwner(0);
    }

    //    FIX ME: The unloading of GRP entry is temporarily disabled
    //    because ZDC and VZERO are using it in order to initialize
    //    their reconstructor objects. In the future one has to think
    //    of propagating IlcRunInfo to the reconstructors.
    //    IlcCDBManager::Instance()->UnloadFromCache("GRP/GRP/Data");
  }

  if (!fGRPData) {
     IlcError("No GRP entry found in OCDB!");
     return kFALSE;
  }

  TString lhcState = fGRPData->GetLHCState();
  if (lhcState==IlcGRPObject::GetInvalidString()) {
    IlcError("GRP/GRP/Data entry:  missing value for the LHC state ! Using UNKNOWN");
    lhcState = "UNKNOWN";
  }

  TString beamType = fGRPData->GetBeamType();
  if (beamType==IlcGRPObject::GetInvalidString()) {
    IlcError("GRP/GRP/Data entry:  missing value for the beam type ! Using UNKNOWN");
    beamType = "UNKNOWN";
  }

  Float_t beamEnergy = fGRPData->GetBeamEnergy();
  if (beamEnergy==IlcGRPObject::GetInvalidFloat()) {
    IlcError("GRP/GRP/Data entry:  missing value for the beam energy ! Using 0");
    beamEnergy = 0;
  }

  TString runType = fGRPData->GetRunType();
  if (runType==IlcGRPObject::GetInvalidString()) {
    IlcError("GRP/GRP/Data entry:  missing value for the run type ! Using UNKNOWN");
    runType = "UNKNOWN";
  }

  Int_t activeDetectors = fGRPData->GetDetectorMask();
  if (activeDetectors==IlcGRPObject::GetInvalidUInt()) {
    IlcError("GRP/GRP/Data entry:  missing value for the detector mask ! Using 1074790399");
    activeDetectors = 1074790399;
  }
  IlcDebug(1, Form("activeDetectors = %d", activeDetectors));

  fRunInfo = new IlcRunInfo(lhcState, beamType, beamEnergy, runType, activeDetectors);
  fRunInfo->Dump();


  // Process the list of active detectors
  if (activeDetectors) {
    UInt_t detMask = activeDetectors;
    IlcDebug(1, Form("Detector List = %s", fRunLocalReconstruction.Data()));
    fRunLocalReconstruction = MatchDetectorList(fRunLocalReconstruction,detMask);
    IlcDebug(1, Form("Detector List = %s", fRunLocalReconstruction.Data()));
    fRunTracking = MatchDetectorList(fRunTracking,detMask);
    fFillESD = MatchDetectorList(fFillESD,detMask);
    fQADetectors = MatchDetectorList(fQADetectors,detMask);
    fDeleteRecPoints = MatchDetectorList(fDeleteRecPoints,detMask);
    fDeleteDigits    = MatchDetectorList(fDeleteDigits,detMask);
    fLoadCDB.Form("%s %s %s %s",
		  fRunLocalReconstruction.Data(),
		  fRunTracking.Data(),
		  fFillESD.Data(),
		  fQADetectors.Data());
    fLoadCDB = MatchDetectorList(fLoadCDB,detMask);
    if (!((detMask >> IlcDAQ::DetectorID("ITSSPD")) & 0x1) &&
	!((detMask >> IlcDAQ::DetectorID("ITSSDD")) & 0x1) &&
	!((detMask >> IlcDAQ::DetectorID("ITSSSD")) & 0x1) ) {
      // switch off the vertexer
      IlcInfo("SPD,SDD,SSD is not in the list of active detectors. Vertexer and Trackleter are switched off.");
      fRunVertexFinder = kFALSE;
      fRunMultFinder = kFALSE;
    }
    if (!((detMask >> IlcDAQ::DetectorID("TRG")) & 0x1)) {
      // switch off the reading of CTP raw-data payload
      if (fFillTriggerESD) {
	IlcInfo("CTP is not in the list of active detectors. CTP data reading switched off.");
	fFillTriggerESD = kFALSE;
      }
    }
  }

  IlcInfo("===================================================================================");
  IlcInfo(Form("Running local reconstruction for detectors: %s",fRunLocalReconstruction.Data()));
  IlcInfo(Form("Running tracking for detectors: %s",fRunTracking.Data()));
  IlcInfo(Form("Filling ESD for detectors: %s",fFillESD.Data()));
  IlcInfo(Form("Quality assurance is active for detectors: %s",fQADetectors.Data()));
  IlcInfo(Form("CDB and reconstruction parameters are loaded for detectors: %s",fLoadCDB.Data()));
  IlcInfo("===================================================================================");

  //*** Dealing with the magnetic field map
  if ( TGeoGlobalMagField::Instance()->IsLocked() ) {
    if (TGeoGlobalMagField::Instance()->GetField()->TestBit(IlcMagF::kOverrideGRP)) {
      IlcInfo("ExpertMode!!! GRP information will be ignored !");
      IlcInfo("ExpertMode!!! Running with the externally locked B field !");
    }
    else {
      IlcInfo("Destroying existing B field instance!");
      delete TGeoGlobalMagField::Instance();
    }
  }
  if ( !TGeoGlobalMagField::Instance()->IsLocked() ) {
    // Construct the field map out of the information retrieved from GRP.
    Bool_t ok = kTRUE;
    // L3
    Float_t l3Current = fGRPData->GetL3Current((IlcGRPObject::Stats)0);
    if (l3Current == IlcGRPObject::GetInvalidFloat()) {
      IlcError("GRP/GRP/Data entry:  missing value for the L3 current !");
      ok = kFALSE;
    }

    Char_t l3Polarity = fGRPData->GetL3Polarity();
    if (l3Polarity == IlcGRPObject::GetInvalidChar()) {
      IlcError("GRP/GRP/Data entry:  missing value for the L3 polarity !");
      ok = kFALSE;
    }

    // Dipole
    Float_t diCurrent = fGRPData->GetDipoleCurrent((IlcGRPObject::Stats)0);
    if (diCurrent == IlcGRPObject::GetInvalidFloat()) {
      IlcError("GRP/GRP/Data entry:  missing value for the dipole current !");
      ok = kFALSE;
    }

    Char_t diPolarity = fGRPData->GetDipolePolarity();
    if (diPolarity == IlcGRPObject::GetInvalidChar()) {
      IlcError("GRP/GRP/Data entry:  missing value for the dipole polarity !");
      ok = kFALSE;
    }

    // read special bits for the polarity convention and map type
    Int_t  polConvention = fGRPData->IsPolarityConventionLHC() ? IlcMagF::kConvLHC : IlcMagF::kConvDCS2008;
    Bool_t uniformB = fGRPData->IsUniformBMap();

    if (ok) {
      IlcMagF* fld = IlcMagF::CreateFieldMap(TMath::Abs(l3Current) * (l3Polarity ? -1:1),
					     TMath::Abs(diCurrent) * (diPolarity ? -1:1),
					     polConvention,uniformB,beamEnergy, beamType.Data());
      if (fld) {
	TGeoGlobalMagField::Instance()->SetField( fld );
	TGeoGlobalMagField::Instance()->Lock();
	IlcInfo("Running with the B field constructed out of GRP !");
      }
      else IlcFatal("Failed to create a B field map !");
    }
    else IlcFatal("B field is neither set nor constructed from GRP ! Exitig...");
  }

  //*** Get the diamond profiles from OCDB
  entry = IlcCDBManager::Instance()->Get("GRP/Calib/MeanVertexSPD");
  if (entry) {
    fDiamondProfileSPD = dynamic_cast<IlcESDVertex*> (entry->GetObject());
  } else {
     IlcError("No SPD diamond profile found in OCDB!");
  }

  entry = IlcCDBManager::Instance()->Get("GRP/Calib/MeanVertex");
  if (entry) {
    fDiamondProfile = dynamic_cast<IlcESDVertex*> (entry->GetObject());
  } else {
     IlcError("No diamond profile found in OCDB!");
  }

  entry = IlcCDBManager::Instance()->Get("GRP/Calib/MeanVertexTPC");
  if (entry) {
    fDiamondProfileTPC = dynamic_cast<IlcESDVertex*> (entry->GetObject());
  } else {
     IlcError("No TPC diamond profile found in OCDB!");
  }

  entry = IlcCDBManager::Instance()->Get("GRP/Calib/CosmicTriggers");
  if (entry) {
    fListOfCosmicTriggers = dynamic_cast<THashTable*>(entry->GetObject());
    entry->SetOwner(0);
  }

  if (!fListOfCosmicTriggers) {
    IlcWarning("Can not get list of cosmic triggers from OCDB! Cosmic event specie will be effectively disabled!");
  }

  return kTRUE;
} 

//_____________________________________________________________________________
Bool_t IlcReconstruction::LoadCDB()
{
  // Load CDB entries for all active detectors.
  // By default we load all the entries in <det>/Calib
  // folder.

  IlcCodeTimerAuto("",0);

  IlcCDBManager::Instance()->Get("GRP/CTP/Config");

  IlcCDBManager::Instance()->Get("GRP/Calib/LHCClockPhase");

  TString detStr = fLoadCDB;
  for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
    if (!IsSelected(fgkDetectorName[iDet], detStr)) continue;
    IlcCDBManager::Instance()->GetAll(Form("%s/Calib/*",fgkDetectorName[iDet]));
    IlcCDBManager::Instance()->GetAll(Form("%s/Trigger/*",fgkDetectorName[iDet]));
  }

  // Temporary fix - one has to define the correct policy in order
  // to load the trigger OCDB entries only for the detectors that
  // in the trigger or that are needed in order to put correct
  // information in ESD
  IlcCDBManager::Instance()->GetAll("TRIGGER/*/*");

  return kTRUE;
}
//_____________________________________________________________________________
Bool_t IlcReconstruction::LoadTriggerScalersCDB()
{
  // Load CTP scalers from OCDB.
  // The scalers are checked for consistency.

  IlcCodeTimerAuto("",0);

  IlcCDBEntry* entry = IlcCDBManager::Instance()->Get("GRP/CTP/Scalers");

  if (entry) { 
   
       IlcInfo("Found an IlcTriggerRunScalers in GRP/CTP/Scalers, reading it");
       fRunScalers = dynamic_cast<IlcTriggerRunScalers*> (entry->GetObject());
       entry->SetOwner(0);
       if (fRunScalers && (fRunScalers->CorrectScalersOverflow() == 0)) IlcInfo("32bit Trigger counters corrected for overflow");

  }
  return kTRUE;
}
//_____________________________________________________________________________
Bool_t IlcReconstruction::LoadCTPTimeParamsCDB()
{
  // Load CTP timing information (alignment)
  // from OCDB.

  IlcCDBEntry* entry = IlcCDBManager::Instance()->Get("GRP/CTP/CTPtiming");
  if (!entry) return kFALSE;

  IlcInfo("Found an IlcCTPTimeParams in GRP/CTP/CTPtiming, reading it");
  fCTPTimeParams = dynamic_cast<IlcCTPTimeParams*> (entry->GetObject());
  entry->SetOwner(0);

  IlcCDBEntry* entry2 = IlcCDBManager::Instance()->Get("GRP/CTP/TimeAlign");
  if (!entry2) return kFALSE;

  IlcInfo("Found an IlcCTPTimeParams in GRP/CTP/TimeAlign, reading it");
  fCTPTimeAlign = dynamic_cast<IlcCTPTimeParams*> (entry2->GetObject());
  entry2->SetOwner(0);

  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcReconstruction::ReadIntensityInfoCDB()
{
  // Load LHC DIP data
  IlcCDBEntry* entry    = IlcCDBManager::Instance()->Get("GRP/GRP/LHCData");
  IlcCDBEntry* entryCTP = IlcCDBManager::Instance()->Get("GRP/CTP/Config");
  //
  if (!entry || !entryCTP) {
    IlcError(Form("Failed to extract CDB objects GRP/GRP/LHCData: %p or GRP/CTP/Config: %p",entry,entryCTP));
    return kFALSE;
  }
  // extract BC masks
  enum {kA,kB,kC,kE,kNMasks};
  IlcTriggerConfiguration* conf = (IlcTriggerConfiguration*)entryCTP->GetObject();
  const TObjArray& clArr = conf->GetClasses();
  TObjArray masks(kNMasks);
  TIter next(&clArr);
  IlcTriggerClass* trClass = 0;
  int nFound = 0;
  masks.SetOwner(kFALSE);
  //
  while ( (trClass=(IlcTriggerClass*)next()) ) {
    TString trName = trClass->GetName();
    int ind = trName.Index("-"); // prefix in front of A,B,C,E
    if (ind<1) continue;   // anomaly
    //
    trName = trName.Data() + ind;
    IlcTriggerBCMask* bcMask = trClass->GetBCMask();
    if (!bcMask) continue;
    UInt_t which = 0;
    if      (trName.BeginsWith("-A-"))  which |= 0x1<<kA;
    else if (trName.BeginsWith("-B-"))  which |= 0x1<<kB;
    else if (trName.BeginsWith("-C-"))  which |= 0x1<<kC;
    else if (trName.BeginsWith("-E-"))  which |= 0x1<<kE;
    else if (trName.BeginsWith("-AC-")) which |= (0x1<<kA) | (0x1<<kC);
    else if (trName.BeginsWith("-CA-")) which |= (0x1<<kA) | (0x1<<kC);
    else { IlcWarning(Form("Unknown trigger type %s\n",trClass->GetName())); continue;}
    //
    for (int ip=kNMasks;ip--;) {
      if ( !(which&(0x1<<ip)) || masks[ip] ) continue; // does not match or already done
      masks[ip] = (TObject*)bcMask;
      nFound++;
    }
    if (nFound==kNMasks) break;
  }  
  //  
  IlcInfo("Reading mean bunch intensities from GRP/GRP/LHCData");
  IlcLHCData* dipData = dynamic_cast<IlcLHCData*> (entry->GetObject());
  //
  for (int ib=2;ib--;) {
    double intI,intNI;
    if (dipData && (dipData->GetMeanIntensity(ib,intI,intNI,&masks)>=0)) {
      fBeamInt[ib][0] = intI;
      fBeamInt[ib][1] = intNI;	
      IlcInfo(Form("Mean intensity for beam %d: Interacting:%.2e Non-Interacting:%.2e",ib,intI,intNI));
    }
  }
  return kTRUE;
  //
}


//_____________________________________________________________________________
Bool_t IlcReconstruction::Run(const char* input)
{
  // Run Run Run
  IlcCodeTimerAuto("",0);

  InitRun(input);
  if (GetAbort() != TSelector::kContinue) return kFALSE;

  TChain *chain = NULL;
  if (fRawReader && (chain = fRawReader->GetChain())) {
    Long64_t nEntries = (fLastEvent < 0) ? (TChain::kBigNumber) : (fLastEvent - fFirstEvent + 1);
    // Proof mode
    if (gProof) {
      // Temporary fix for long raw-data runs (until socket timeout handling in PROOF is revised)
      gProof->Exec("gEnv->SetValue(\"Proof.SocketActivityTimeout\",-1)", kTRUE);

      if (gGrid)
	gProof->Exec("TGrid::Connect(\"alien://\")",kTRUE);

      TMessage::EnableSchemaEvolutionForAll(kTRUE);
      gProof->Exec("TMessage::EnableSchemaEvolutionForAll(kTRUE)",kTRUE);

      gProof->AddInput(this);

      if (!ParseOutput()) return kFALSE;

      gProof->SetParameter("PROOF_MaxSlavesPerNode", 9999);
      chain->SetProof();
      chain->Process("IlcReconstruction","",nEntries,fFirstEvent);
    }
    else {
      chain->Process(this,"",nEntries,fFirstEvent);
    }
  }
  else {
    Begin(NULL);
    if (GetAbort() != TSelector::kContinue) return kFALSE;
    SlaveBegin(NULL);
    if (GetAbort() != TSelector::kContinue) return kFALSE;
    //******* The loop over events
    IlcInfo("Starting looping over events");
    Int_t iEvent = 0;
    while ((iEvent < fRunLoader->GetNumberOfEvents()) ||
	   (fRawReader && fRawReader->NextEvent())) {
      //
      // check if process has enough resources 
      if (!HasEnoughResources(iEvent)) break;
      if (!ProcessEvent(iEvent)) {
        Abort("ProcessEvent",TSelector::kAbortFile);
        return kFALSE;
      }
      iEvent++;
    }
    if (!iEvent) IlcWarning("No events passed trigger selection");
    SlaveTerminate();
    if (GetAbort() != TSelector::kContinue) return kFALSE;
// // // //     Terminate();
    if (GetAbort() != TSelector::kContinue) return kFALSE;
  }

  return kTRUE;
}

//_____________________________________________________________________________
void IlcReconstruction::InitRawReader(const char* input)
{
  // Init raw-reader and
  // set the input in case of raw data

  IlcCodeTimerAuto("",0);

  if (input) fRawInput = input;
  fRawReader = IlcRawReader::Create(fRawInput.Data());
  if (!fRawReader) {
    if (fRawInput.IsNull()) {
      IlcInfo("Reconstruction will run over digits");
    }
    else {
      IlcFatal("Can not create raw-data reader ! Exiting..."); 
    }
  }

  if (!fEquipIdMap.IsNull() && fRawReader)
    fRawReader->LoadEquipmentIdsMap(fEquipIdMap);

  if (!fUseHLTData.IsNull()) {
    // create the RawReaderHLT which performs redirection of HLT input data for
    // the specified detectors
    IlcRawReader* pRawReader=IlcRawHLTManager::CreateRawReaderHLT(fRawReader, fUseHLTData.Data());
    if (pRawReader) {
      fParentRawReader=fRawReader;
      fRawReader=pRawReader;
    } else {
      IlcError(Form("can not create Raw Reader for HLT input %s", fUseHLTData.Data()));
    }
  }
  IlcSysInfo::AddStamp("CreateRawReader");
}

//_____________________________________________________________________________
void IlcReconstruction::InitRun(const char* input)
{
  // Initialization of raw-reader,
  // run number, CDB etc.
  IlcCodeTimerAuto("",0);
  IlcSysInfo::AddStamp("Start");

  // Initialize raw-reader if any
  InitRawReader(input);

  // Initialize the CDB storage
  InitCDB();

  // Set run number in CDBManager (if it is not already set by the user)
  if (!SetRunNumberFromData()) {
    Abort("SetRunNumberFromData", TSelector::kAbortProcess);
    return;
  }

  // Set CDB lock: from now on it is forbidden to reset the run number
  // or the default storage or to activate any further storage!
  SetCDBLock();
  
}

//_____________________________________________________________________________
void IlcReconstruction::Begin(TTree *)
{
  // Initialize IlcReconstruction before
  // going into the event loop
  // Should follow the TSelector convention
  // i.e. initialize only the object on the client side
  IlcCodeTimerAuto("",0);

  IlcReconstruction *reco = NULL;
  if (fInput) {
    if ((reco = (IlcReconstruction*)fInput->FindObject("IlcReconstruction"))) {
      *this = *reco;
    }
    IlcSysInfo::AddStamp("ReadInputInBegin");
  }

  // Import ideal TGeo geometry and apply misalignment
  if (!gGeoManager) {
    TString geom(gSystem->DirName(fGIlcFileName));
    geom += "/geometry.root";
    IlcGeomManager::LoadGeometry(geom.Data());
    if (!gGeoManager) {
      Abort("LoadGeometry", TSelector::kAbortProcess);
      return;
    }
    IlcSysInfo::AddStamp("LoadGeom");
    TString detsToCheck=fRunLocalReconstruction;
    if(!IlcGeomManager::CheckSymNamesLUT(detsToCheck.Data())) {
      Abort("CheckSymNamesLUT", TSelector::kAbortProcess);
      return;
    }
    IlcSysInfo::AddStamp("CheckGeom");
  }

  Bool_t toCDBSnapshot=kFALSE;
  TString snapshotFileOut(""); // we could use fSnapshotFileName if we are not interested
  // in reading from and writing to a snapshot file at the same time
  if(TString(getenv("OCDB_SNAPSHOT_CREATE")) == TString("kTRUE")){
      toCDBSnapshot=kTRUE;
      //fFromCDBSnapshot=kFALSE;
      TString snapshotFile(getenv("OCDB_SNAPSHOT_FILENAME"));
      if(!(snapshotFile.IsNull() || snapshotFile.IsWhitespace()))
	  snapshotFileOut = snapshotFile;
      else
	  snapshotFileOut="OCDB.root";
  }

  if (!MisalignGeometry(fLoadAlignData)) {
    Abort("MisalignGeometry", TSelector::kAbortProcess);
    return;
  }
  IlcCDBManager::Instance()->UnloadFromCache("GRP/Geometry/Data");
  if(!toCDBSnapshot) IlcCDBManager::Instance()->UnloadFromCache("*/Align/*");
  IlcSysInfo::AddStamp("MisalignGeom");

  if (!InitGRP()) {
    Abort("InitGRP", TSelector::kAbortProcess);
    return;
  }
  IlcSysInfo::AddStamp("InitGRP");
  if(!toCDBSnapshot) IlcCDBManager::Instance()->UnloadFromCache("GRP/Calib/CosmicTriggers");

// // // //   if(!fCDBSnapshotMode || toCDBSnapshot){
// // // //       if (!LoadCDB()) {
// // // // 	  Abort("LoadCDB", TSelector::kAbortProcess);
// // // // 	  return;
// // // //       }
// // // //       IlcSysInfo::AddStamp("LoadCDB"); 
// // // //   }
// // // // 
// // // //   if (!LoadTriggerScalersCDB()) {
// // // //     Abort("LoadTriggerScalersCDB", TSelector::kAbortProcess);
// // // //     return;
// // // //   }
// // // //   IlcSysInfo::AddStamp("LoadTriggerScalersCDB");
// // // // 
// // // //   if (!LoadCTPTimeParamsCDB()) {
// // // //     Abort("LoadCTPTimeParamsCDB", TSelector::kAbortProcess);
// // // //     return;
// // // //   }
// // // //   IlcSysInfo::AddStamp("LoadCTPTimeParamsCDB");
// // // // 
// // // //   if (!ReadIntensityInfoCDB()) {
// // // //     Abort("ReadIntensityInfoCDB", TSelector::kAbortProcess);
// // // //     return;
// // // //   }
// // // //   IlcSysInfo::AddStamp("ReadIntensityInfoCDB");

  // Read the reconstruction parameters from OCDB
  if (!InitRecoParams()) {
    IlcWarning("Not all detectors have correct RecoParam objects initialized");
  }
  IlcSysInfo::AddStamp("InitRecoParams");

// // // //   if(toCDBSnapshot)
// // // //   {
// // // //       IlcCDBManager::Instance()->DumpToSnapshotFile(snapshotFileOut.Data(),kFALSE);
// // // //       IlcCDBManager::Instance()->UnloadFromCache("*/Align/*");
// // // //       IlcCDBManager::Instance()->UnloadFromCache("GRP/Calib/CosmicTriggers");
// // // //   }

  if (fInput && gProof) {
    if (reco) *reco = *this;

    gGeoManager->SetName("Geometry");
    gProof->AddInputData(gGeoManager,kTRUE);
    gGeoManager = NULL;
    gProof->AddInputData(const_cast<TMap*>(IlcCDBManager::Instance()->GetEntryCache()),kTRUE);
    fInput->Add(new TParameter<Int_t>("RunNumber",IlcCDBManager::Instance()->GetRun()));
    IlcMagF *magFieldMap = (IlcMagF*)TGeoGlobalMagField::Instance()->GetField();
    magFieldMap->SetName("MagneticFieldMap");
    gProof->AddInputData(magFieldMap,kTRUE);
// // // //     if (fAnalysis) {
// // // //       fAnalysis->SetName("Analysis");
// // // //       gProof->AddInputData(fAnalysis,kTRUE);
// // // //     }  
  }

}

//_____________________________________________________________________________
void IlcReconstruction::SlaveBegin(TTree*)
{
  // Initialization related to run-loader,
  // vertexer, trackers, recontructors
  // In proof mode it is executed on the slave
  IlcCodeTimerAuto("",0);

  TProofOutputFile *outProofFile = NULL;
  if (fInput) {
    if (IlcDebugLevel() > 0) fInput->Print();
    if (IlcDebugLevel() > 10) fInput->Dump();
    if (IlcReconstruction *reco = (IlcReconstruction*)fInput->FindObject("IlcReconstruction")) {
      *this = *reco;
    }
    if (TGeoManager *tgeo = (TGeoManager*)fInput->FindObject("Geometry")) {
      gGeoManager = tgeo;
      IlcGeomManager::SetGeometry(tgeo);
    }
    if (TMap *entryCache = (TMap*)fInput->FindObject("CDBEntryCache")) {
      Int_t runNumber = -1;
      if (TProof::GetParameter(fInput,"RunNumber",runNumber) == 0) {
	IlcCDBManager *man = IlcCDBManager::Instance(entryCache,runNumber);
	man->SetCacheFlag(kTRUE);
	man->SetLock(kTRUE);
	man->Print();
      }
    }

    if (IlcMagF *map = (IlcMagF*)fInput->FindObject("MagneticFieldMap")) {
      IlcMagF *newMap = new IlcMagF(*map);
      if (!newMap->LoadParameterization()) {
	Abort("IlcMagF::LoadParameterization", TSelector::kAbortProcess);
	return;
      }
      TGeoGlobalMagField::Instance()->SetField(newMap);
      TGeoGlobalMagField::Instance()->Lock();
    }
// // // //     if (!fAnalysis) {
// // // //        // Attempt to get the analysis manager from the input list
// // // //        fAnalysis = (IlcAnalysisManager*)fInput->FindObject("Analysis");
// // // //        if (fAnalysis) IlcInfo("==== Analysis manager retrieved from input list ====");
// // // //     }   
    if (TNamed *outputFileName = (TNamed*)fInput->FindObject("PROOF_OUTPUTFILE"))
      fProofOutputFileName = outputFileName->GetTitle();
    if (TNamed *outputLocation = (TNamed*)fInput->FindObject("PROOF_OUTPUTFILE_LOCATION"))
      fProofOutputLocation = outputLocation->GetTitle();
    if (fInput->FindObject("PROOF_OUTPUTFILE_DATASET"))
      fProofOutputDataset = kTRUE;
    if (TNamed *archiveList = (TNamed*)fInput->FindObject("PROOF_OUTPUTFILE_ARCHIVE"))
      fProofOutputArchive = archiveList->GetTitle();
    if (!fProofOutputFileName.IsNull() &&
	!fProofOutputLocation.IsNull() &&
	fProofOutputArchive.IsNull()) {
      if (!fProofOutputDataset) {
	outProofFile = new TProofOutputFile(fProofOutputFileName.Data(),"M");
	outProofFile->SetOutputFileName(Form("%s%s",fProofOutputLocation.Data(),fProofOutputFileName.Data()));
      }
      else {
	outProofFile = new TProofOutputFile(fProofOutputFileName.Data(),"DROV",fProofOutputLocation.Data());
      }
      if (IlcDebugLevel() > 0) outProofFile->Dump();
      fOutput->Add(outProofFile);
    }
    IlcSysInfo::AddStamp("ReadInputInSlaveBegin");
  }
  // Check if analysis was requested in the reconstruction event loop
// // // //   if (!fAnalysis) {
// // // //     // Attempt to connect in-memory singleton
// // // //     fAnalysis = IlcAnalysisManager::GetAnalysisManager();
// // // //     if (fAnalysis) IlcInfo(Form("==== Analysis manager <%s> found in memory ====", fAnalysis->GetName()));
// // // //     // Check if an analysis macro was specified
// // // //     if (!fAnalysis && !fAnalysisMacro.IsNull()) {
// // // //       // Run specified analysis macro
// // // //       gROOT->ProcessLine(Form(".x %s",fAnalysisMacro.Data()));
// // // //       fAnalysis = IlcAnalysisManager::GetAnalysisManager();
// // // //       if (!fAnalysis) IlcError(Form("No analysis manager produced by analysis macro %s", fAnalysisMacro.Data()));
// // // //       else IlcInfo(Form("==== Analysis manager <%s> produced by analysis macro <%s> ====", 
// // // //                         fAnalysis->GetName(), fAnalysisMacro.Data()));
// // // //     }
// // // //   }
  
  // get the run loader
  if (!InitRunLoader()) {
    Abort("InitRunLoader", TSelector::kAbortProcess);
    return;
  }


  fRunLoader->CdGAFile();
  IlcMagF* MagField = (IlcMagF*)gDirectory->Get("MagField");
  TGeoGlobalMagField::Instance()->SetField(MagField);


  IlcSysInfo::AddStamp("LoadLoader");
 
  ftVertexer = new IlcVertexerTracks(IlcTracker::GetBz());

  // get trackers
  if (!fRunTracking.IsNull() && !CreateTrackers(fRunTracking)) {
    Abort("CreateTrackers", TSelector::kAbortProcess);
    return;
  }      
  IlcSysInfo::AddStamp("CreateTrackers");

  // create the ESD output file and tree
  if (!outProofFile) {
    ffile = TFile::Open("IlcESDs.root", "RECREATE");
    ffile->SetCompressionLevel(2);
    if (!ffile->IsOpen()) {
      Abort("OpenESDFile", TSelector::kAbortProcess);
      return;
    }
  }
  else {
    IlcInfo(Form("Opening output PROOF file: %s/%s",
		 outProofFile->GetDir(), outProofFile->GetFileName()));
    if (!(ffile = outProofFile->OpenFile("RECREATE"))) {
      Abort(Form("Problems opening output PROOF file: %s/%s",
		 outProofFile->GetDir(), outProofFile->GetFileName()),
	    TSelector::kAbortProcess);
      return;
    }
  }

  ftree = new TTree("esdTree", "Tree with ESD objects");
  fesd = new IlcESDEvent();
  fesd->CreateStdContent();
  // add a so far non-std object to the ESD, this will
  // become part of the std content
  fesd->AddObject(new IlcESDHLTDecision);

  fesd->WriteToTree(ftree);
  if (fWriteESDfriend) {
    ffileF = TFile::Open("IlcESDfriends.root", "RECREATE");
    ftreeF = new TTree("esdFriendTree", "Tree with ESD Friend objects");
    fesdf  = new IlcESDfriend();
    ftreeF->Branch("ESDfriend.","IlcESDfriend", &fesdf);
    fesd->AddObject(fesdf);
    ffile->cd();
  }
  ftree->GetUserInfo()->Add(fesd);

// // // //   fhlttree = new TTree("HLTesdTree", "Tree with HLT ESD objects");
// // // //   fhltesd = new IlcESDEvent();
// // // //   fhltesd->CreateStdContent();
// // // //   // read the ESD template from CDB
// // // //   // HLT is allowed to put non-std content to its ESD, the non-std
// // // //   // objects need to be created before invocation of WriteToTree in
// // // //   // order to create all branches. Initialization is done from an
// // // //   // ESD layout template in CDB
// // // //   IlcCDBManager* man = IlcCDBManager::Instance();
// // // //   IlcCDBEntry* hltESDConfig = man->Get("HLT/Calib/esdLayout");
// // // //   IlcESDEvent* pESDLayout=dynamic_cast<IlcESDEvent*>(hltESDConfig->GetObject());
// // // //   if (pESDLayout) {
// // // //       // init all internal variables from the list of objects
// // // //       pESDLayout->GetStdContent();
// // // // 
// // // //       // copy content and create non-std objects
// // // //       *fhltesd=*pESDLayout;
// // // //       fhltesd->Reset();
// // // //   } else {
// // // //       IlcError(Form("error setting hltEsd layout from \"HLT/Calib/esdLayout\": invalid object type"));
// // // //   }
// // // // 
// // // //   fhltesd->WriteToTree(fhlttree);
// // // //   fhlttree->GetUserInfo()->Add(fhltesd);

  ProcInfo_t procInfo;
  gSystem->GetProcInfo(&procInfo);
  IlcInfo(Form("Current memory usage %ld %ld", procInfo.fMemResident, procInfo.fMemVirtual));
  
  //QA
  //Initialize the QA and start of cycle 
  if (fRunQA || fRunGlobalQA) 
    InitQA() ; 

  //Initialize the Plane Efficiency framework
  if (fRunPlaneEff && !InitPlaneEff()) {
    Abort("InitPlaneEff", TSelector::kAbortProcess);
    return;
  }

  if (strcmp(gProgName,"ilceve") == 0)
    fRunIlcEVE = InitIlcEVE();
  // If we have an analysis manager, connect the IlcRecoInputHandler here  
// // // //   if (fAnalysis) {
// // // //     if (!dynamic_cast<IlcRecoInputHandler*>(fAnalysis->GetInputEventHandler())) {
// // // //        IlcError("Analysis manager used in reconstruction should use IlcRecoInputHandler -  ->Replacing with IlcRecoInputHandler instance.");
// // // //        delete fAnalysis->GetInputEventHandler();
// // // //     }
// // // //     // Set the event and other data pointers
// // // //     fRecoHandler = new IlcRecoInputHandler();
// // // //     fRecoHandler->Init(ftree, "LOCAL");
// // // //     fRecoHandler->SetEvent(fesd);
// // // //     fRecoHandler->SetESDfriend(fesdf);
// // // //     fRecoHandler->SetHLTEvent(fhltesd);
// // // //     fRecoHandler->SetHLTTree(fhlttree);
// // // //     fAnalysis->SetInputEventHandler(fRecoHandler);
// // // //     // Enter external loop mode
// // // //     fAnalysis->SetExternalLoop(kTRUE);
// // // //     // Initialize analysis
// // // //     fAnalysis->StartAnalysis("local", (TTree*)0);
// // // //     // Connect ESD tree with the input container
// // // //     fAnalysis->GetCommonInputContainer()->SetData(ftree);
// // // //   }  
  return;
}

//_____________________________________________________________________________
Bool_t IlcReconstruction::Process(Long64_t entry)
{
  // run the reconstruction over a single entry
  // from the chain with raw data
  IlcCodeTimerAuto("",0);

  TTree *currTree = fChain->GetTree();
  IlcRawVEvent *event = NULL;
  currTree->SetBranchAddress("rawevent",&event);
  currTree->GetEntry(entry);
  fRawReader = new IlcRawReaderRoot(event);
  // check if process has enough resources 
  if (!HasEnoughResources(entry)) return kFALSE;
  fStatus = ProcessEvent(fRunLoader->GetNumberOfEvents());
  delete fRawReader;
  fRawReader = NULL;
  delete event;

  return fStatus;
}

//_____________________________________________________________________________
void IlcReconstruction::Init(TTree *tree)
{
  // Implementation of TSelector::Init()
  // method
  if (tree == 0) {
    IlcError("The input tree is not found!");
    return;
  }
  fChain = tree;
}

//_____________________________________________________________________________
Bool_t IlcReconstruction::ProcessEvent(Int_t iEvent)
{
  // run the reconstruction over a single event
  // The event loop is steered in Run method


  static Long_t oldMres=0;
  static Long_t oldMvir=0;
  static Float_t oldCPU=0;
  static Long_t aveDMres=0;
  static Long_t aveDMvir=0;
  static Float_t aveDCPU=0;

  IlcCodeTimerAuto("",0);

  IlcESDpid pid;

  IlcSysInfo::AddStamp(Form("StartEv_%d",iEvent), 0,0,iEvent);

  if (iEvent >= fRunLoader->GetNumberOfEvents()) {
    fRunLoader->SetEventNumber(iEvent);
    if (fRawReader)
      fRunLoader->GetHeader()->Reset(fRawReader->GetRunNumber(), 
				     iEvent, iEvent);
    fRunLoader->TreeE()->Fill();

    if (fRawReader && fRawReader->UseAutoSaveESD())
      fRunLoader->TreeE()->AutoSave("SaveSelf");
  }

  if ((iEvent < fFirstEvent) || ((fLastEvent >= 0) && (iEvent > fLastEvent))) {
    return kTRUE;
  }


  fRunLoader->GetEvent(iEvent);

  // Fill Event-info object
  GetEventInfo();
  fRecoParam.SetEventSpecie(fRunInfo,fEventInfo,fListOfCosmicTriggers);
  
  ProcInfo_t procInfo;
  if(iEvent==fFirstEvent) {
    gSystem->GetProcInfo(&procInfo);
    oldMres=procInfo.fMemResident;
    oldMvir=procInfo.fMemVirtual;
    oldCPU=procInfo.fCpuUser+procInfo.fCpuSys;
  }
  IlcInfo(Form("================================= Processing event %d of type %-10s ==================================", iEvent,fRecoParam.PrintEventSpecie()));

  IlcSysInfo::AddStamp(Form("StartReco_%d",iEvent), 0,0,iEvent);

  // Set the reco-params
  {
    TString detStr = fLoadCDB;
    for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
      if (!IsSelected(fgkDetectorName[iDet], detStr)) continue;
      IlcReconstructor *reconstructor = GetReconstructor(iDet);
      if (reconstructor && fRecoParam.GetDetRecoParamArray(iDet)) {
        const IlcDetectorRecoParam *par = fRecoParam.GetDetRecoParam(iDet);
        reconstructor->SetRecoParam(par);
	reconstructor->GetPidSettings(&pid);
	reconstructor->SetEventInfo(&fEventInfo);
        if (fRunQA) {
          IlcQAManager::QAManager()->SetEventInfo(&fEventInfo) ;
          IlcQAManager::QAManager()->SetRecoParam(iDet, par) ; 
          if (par) IlcQAManager::QAManager()->SetEventSpecie(IlcRecoParam::Convert(par->GetEventSpecie())) ;
        }
      }
    }
    //
    if (fRunQA || fRunGlobalQA) IlcQADataMaker::SetEventTrigClasses(fEventInfo.GetTriggerClasses()); // RS: select which histo clones are to be filled
    //
    if (fRunQA) {
      const IlcDetectorRecoParam *grppar = fRecoParam.GetDetRecoParam(kNDetectors);
      IlcQAManager::QAManager()->SetRecoParam(IlcQAv1::kGLOBAL, grppar) ; 
      IlcQAManager::QAManager()->SetEventSpecie(IlcRecoParam::Convert(grppar->GetEventSpecie())) ;
    }
  }

    // QA on single raw 
  if (fRunQA && IsInTasks(IlcQAv1::kRAWS)) {
    IlcQAManager::QAManager()->SetEventSpecie(fRecoParam.GetEventSpecie()) ;
    IlcQAManager::QAManager()->RunOneEvent(fRawReader) ;  
    IlcSysInfo::AddStamp(Form("RawQA_%d",iEvent), 0,0,iEvent);
  }
    // local single event reconstruction
    if (!fRunLocalReconstruction.IsNull()) {
      TString detectors=fRunLocalReconstruction;
      // run HLT event reconstruction first
      // ;-( IsSelected changes the string
      if (IsSelected("HLT", detectors) &&
	  !RunLocalEventReconstruction("HLT")) {
	if (fStopOnError) {CleanUp(); return kFALSE;}
      }
      detectors=fRunLocalReconstruction;
      detectors.ReplaceAll("HLT", "");
      if (!RunLocalEventReconstruction(detectors)) {
        if (fStopOnError) {
          CleanUp(); 
          return kFALSE;
        }
      }
    }

  
    // fill Event header information from the RawEventHeader
    if (fRawReader){FillRawEventHeaderESD(fesd);}
// // // //     if (fRawReader){FillRawEventHeaderESD(fhltesd);}

    fesd->SetRunNumber(fRunLoader->GetHeader()->GetRun());
// // // //     fhltesd->SetRunNumber(fRunLoader->GetHeader()->GetRun());
    
// // // // //     ((IlcESDRun*)fesd->GetESDRun())->SetDetectorsInDAQ(fRunInfo->GetDetectorMask());
// // // // //     ((IlcESDRun*)fhltesd->GetESDRun())->SetDetectorsInDAQ(fRunInfo->GetDetectorMask());
// // // // //     ((IlcESDRun*)fesd->GetESDRun())->SetDetectorsInReco(IlcDAQ::DetectorPatternOffline(fFillESD.Data()));
// // // // //     ((IlcESDRun*)fhltesd->GetESDRun())->SetDetectorsInReco(IlcDAQ::DetectorPatternOffline(fFillESD.Data()));

    fesd->SetEventNumberInFile(fRunLoader->GetHeader()->GetEventNrInRun());
// // // //     fhltesd->SetEventNumberInFile(fRunLoader->GetHeader()->GetEventNrInRun());

    fesd->SetEventSpecie(fRecoParam.GetEventSpecie());
// // // //     fhltesd->SetEventSpecie(fRecoParam.GetEventSpecie());
    
    // Set magnetic field from the tracker
    fesd->SetMagneticField(IlcTracker::GetBz());
    IlcESDRun *esdRun;
    esdRun  = (IlcESDRun*)fesd->GetESDRun();
    esdRun->SetMagneticField(IlcTracker::GetBz());
// // // //     fhltesd->SetMagneticField(IlcTracker::GetBz());
    //
// // // //     IlcESDRun *esdRun,*esdRunH;
// // // //     esdRun  = (IlcESDRun*)fesd->GetESDRun();
// // // //     esdRunH = (IlcESDRun*)fhltesd->GetESDRun();
// // // //     esdRun->SetBeamEnergyIsSqrtSHalfGeV();
// // // //     esdRunH->SetBeamEnergyIsSqrtSHalfGeV();
// // // //     //
// // // //     for (int ib=2;ib--;) for (int it=2;it--;) {
// // // // 	esdRun->SetMeanIntensity(ib,it, fBeamInt[ib][it]); 
// // // // 	esdRunH->SetMeanIntensity(ib,it, fBeamInt[ib][it]); 
// // // //       }
    //
// // // //     fesd->SetBeamEnergy(fGRPData->GetBeamEnergy());
// // // //     fesd->SetBeamType(fGRPData->GetBeamType().Data());
// // // //     fesd->SetBeamParticle(fGRPData->GetSingleBeamType(0).Atoi(),0);
// // // //     fesd->SetBeamParticle(fGRPData->GetSingleBeamType(1).Atoi(),1);
// // // //     fhltesd->SetBeamEnergy(fGRPData->GetBeamEnergy());
// // // //     fhltesd->SetBeamType(fGRPData->GetBeamType().Data());
// // // //     fhltesd->SetBeamParticle(fGRPData->GetSingleBeamType(0).Atoi(),0);
// // // //     fhltesd->SetBeamParticle(fGRPData->GetSingleBeamType(1).Atoi(),1);
    //
    IlcMagF* fld = (IlcMagF*)TGeoGlobalMagField::Instance()->GetField();
    if (fld) { // set info needed for field initialization
      fesd->SetCurrentL3(fld->GetCurrentSol());
      fesd->SetCurrentDip(fld->GetCurrentDip());
      fesd->SetUniformBMap(fld->IsUniform());
      fesd->SetBInfoStored();
      //
// // // //       fhltesd->SetCurrentL3(fld->GetCurrentSol());
// // // //       fhltesd->SetCurrentDip(fld->GetCurrentDip());
// // // //       fhltesd->SetUniformBMap(fld->IsUniform());
// // // //       fhltesd->SetBInfoStored();
    }
    //
    // Set most probable pt, for B=0 tracking
    // Get the global reco-params. They are atposition 16 inside the array of detectors in fRecoParam
    const IlcGRPRecoParam *grpRecoParam = dynamic_cast<const IlcGRPRecoParam*>(fRecoParam.GetDetRecoParam(kNDetectors));
    if (grpRecoParam) IlcExternalTrackParam::SetMostProbablePt(grpRecoParam->GetMostProbablePt());
    
    // Fill raw-data error log into the ESD
    if (fRawReader) FillRawDataErrorLog(iEvent,fesd);

    IlcSysInfo::AddStamp(Form("FillHeadErrs_%d",iEvent), 0,0,iEvent);

    // vertex finder
    if (fRunVertexFinder) {
      if (!RunVertexFinder(fesd)) {
	if (fStopOnError) {CleanUp(); return kFALSE;}
      }
      IlcSysInfo::AddStamp(Form("VtxFinder_%d",iEvent), 0,0,iEvent);
    }

    // For Plane Efficiency: run the SPD trackleter
    if (fRunPlaneEff && fSPDTrackleter) {
      if (!RunSPDTrackleting(fesd)) {
        if (fStopOnError) {CleanUp(); return kFALSE;}
      }
      IlcSysInfo::AddStamp(Form("TrackletEff_%d",iEvent), 0,0,iEvent);
    }

// // // //     // Muon tracking
// // // //     if (!fRunTracking.IsNull()) {
// // // //       if (fRunMuonTracking) {
// // // // 	if (!RunMuonTracking(fesd)) {
// // // // 	  if (fStopOnError) {CleanUp(); return kFALSE;}
// // // // 	}
// // // //       }
// // // //       IlcSysInfo::AddStamp(Form("TrackingMUON_%d",iEvent), 0,0,iEvent);      
// // // //     }
// // // //     IlcInfo("TrackingMUON");

    // barrel tracking
    if (!fRunTracking.IsNull()) {
      if (!RunTracking(fesd,pid)) {
	if (fStopOnError) {CleanUp(); return kFALSE;}
      }
    }

    // fill ESD
    if (!fFillESD.IsNull()) {
      TString detectors=fFillESD;
      // run HLT first and on hltesd
      // ;-( IsSelected changes the string
// // // //       if (IsSelected("HLT", detectors) &&
// // // // 	  !FillESD(fhltesd, "HLT")) {
// // // // 	if (fStopOnError) {CleanUp(); return kFALSE;}
// // // //       }
      detectors=fFillESD;
      // Temporary fix to avoid problems with HLT that overwrites the offline ESDs
      if (detectors.Contains("ALL")) {
	detectors="";
	for (Int_t idet=0; idet<kNDetectors; ++idet){
	  detectors += fgkDetectorName[idet];
	  detectors += " ";
	}
      }
      detectors.ReplaceAll("HLT", "");
      if (!FillESD(fesd, detectors)) {
	if (fStopOnError) {CleanUp(); return kFALSE;}
      }
    }
    

    ffile->cd();

    //
    // Propagate track to the beam pipe  (if not already done by ITS)
    //
    const Int_t ntracks = fesd->GetNumberOfTracks();
    const Double_t kRadius  = 2.8; //something less than the beam pipe radius

    TObjArray trkArray;
    UShort_t selectedIdx[ntracks];

    for (Int_t itrack=0; itrack<ntracks; itrack++){
      const Double_t kMaxStep = 1;   //max step over the material
      Bool_t ok;

      IlcESDtrack *track = fesd->GetTrack(itrack);
      if (!track) continue;

      IlcExternalTrackParam *tpcTrack =
           (IlcExternalTrackParam *)track->GetTPCInnerParam();
      ok = kFALSE;
      if (tpcTrack)
	ok = IlcTracker::
	  PropagateTrackToBxByBz(tpcTrack,kRadius,track->GetMass(),kMaxStep,kFALSE);

      if (ok) {
	Int_t n=trkArray.GetEntriesFast();
        selectedIdx[n]=track->GetID();
        trkArray.AddLast(tpcTrack);
      }

      //Tracks refitted by VXD should already be at the SPD vertex
      if (track->IsOn(IlcESDtrack::kVXDrefit)) continue;

      IlcTracker::
         PropagateTrackToBxByBz(track,kRadius,track->GetMass(),kMaxStep,kFALSE);
      Double_t x[3]; track->GetXYZ(x);
      Double_t b[3]; IlcTracker::GetBxByBz(x,b);
      track->RelateToVertexBxByBz(fesd->GetPrimaryVertexSPD(), b, kVeryBig);

    }
    IlcSysInfo::AddStamp(Form("RelToSPDVtx_%d",iEvent), 0,0,iEvent);      
    //
    // Improve the reconstructed primary vertex position using the tracks
    //
    Bool_t runVertexFinderTracks = fRunVertexFinderTracks;
    if(fesd->GetPrimaryVertexSPD()) {
      TString vtitle = fesd->GetPrimaryVertexSPD()->GetTitle();
      if(vtitle.Contains("cosmics")) {
	runVertexFinderTracks=kFALSE;
      }
    }

    if (runVertexFinderTracks) {
       // TPC + ITS primary vertex
       ftVertexer->SetITSMode();
       ftVertexer->SetConstraintOff();
       // get cuts for vertexer from IlcGRPRecoParam
       Bool_t constrSPD=kFALSE;
       if (grpRecoParam) {
	 Int_t nCutsVertexer = grpRecoParam->GetVertexerTracksNCuts();
	 Double_t *cutsVertexer = new Double_t[nCutsVertexer];
	 grpRecoParam->GetVertexerTracksCutsITS(cutsVertexer,nCutsVertexer);
	 ftVertexer->SetCuts(cutsVertexer,nCutsVertexer);
	 delete [] cutsVertexer; cutsVertexer = NULL; 
	 if(grpRecoParam->GetVertexerTracksConstraintITS()) { 
	   if(fDiamondProfile && fDiamondProfile->GetXRes()<kRadius){
	     ftVertexer->SetVtxStart(fDiamondProfile); // apply constraint only if sigmax is smaller than the beam pipe radius 
	   }else{
	     if(fDiamondProfileSPD && fDiamondProfileSPD->GetXRes()<kRadius){
	       ftVertexer->SetVtxStart(fDiamondProfileSPD);
	       constrSPD=kTRUE;
	     }
	   }
	 } 
       }
       IlcESDVertex *pvtx=ftVertexer->FindPrimaryVertex(fesd);
       if (pvtx) {
	 if(constrSPD){
	   TString title=pvtx->GetTitle();
	   title.Append("SPD");
	   pvtx->SetTitle(title);
	 }
          if (pvtx->GetStatus()) {
             fesd->SetPrimaryVertexTracks(pvtx);
             for (Int_t i=0; i<ntracks; i++) {
	         IlcESDtrack *t = fesd->GetTrack(i);
                 Double_t x[3]; t->GetXYZ(x);
                 Double_t b[3]; IlcTracker::GetBxByBz(x,b);
                 t->RelateToVertexBxByBz(pvtx, b, kVeryBig);
             } 
          }
	  delete pvtx; pvtx=NULL;
       }
       IlcSysInfo::AddStamp(Form("VtxTrk_%d",iEvent), 0,0,iEvent);      

       // TPC-only primary vertex
       ftVertexer->SetTPCMode();
       ftVertexer->SetConstraintOff();
       // get cuts for vertexer from IlcGRPRecoParam
       if (grpRecoParam) {
	 Int_t nCutsVertexer = grpRecoParam->GetVertexerTracksNCuts();
	 Double_t *cutsVertexer = new Double_t[nCutsVertexer];
	 grpRecoParam->GetVertexerTracksCutsTPC(cutsVertexer,nCutsVertexer);
	 ftVertexer->SetCuts(cutsVertexer,nCutsVertexer);
	 delete [] cutsVertexer; cutsVertexer = NULL; 
	 if(fDiamondProfileTPC && grpRecoParam->GetVertexerTracksConstraintTPC()) { 
	   if(fDiamondProfileTPC->GetXRes()<kRadius) ftVertexer->SetVtxStart(fDiamondProfileTPC); // apply constraint only if sigmax is smaller than the beam pipe radius 
	 } 
       }
       pvtx=ftVertexer->FindPrimaryVertex(&trkArray,selectedIdx);
       if (pvtx) {
          if (pvtx->GetStatus()) {
             fesd->SetPrimaryVertexTPC(pvtx);
             for (Int_t i=0; i<ntracks; i++) {
	         IlcESDtrack *t = fesd->GetTrack(i);
                 Double_t x[3]; t->GetXYZ(x);
                 Double_t b[3]; IlcTracker::GetBxByBz(x,b);
                 t->RelateToVertexTPCBxByBz(pvtx, b, kVeryBig);
             } 
          }
	  delete pvtx; pvtx=NULL;
       }
       IlcSysInfo::AddStamp(Form("VtxTPC_%d",iEvent), 0,0,iEvent);      

    }
    
    if(fDiamondProfile && fDiamondProfile->GetXRes()<kRadius) fesd->SetDiamond(fDiamondProfile);
    else fesd->SetDiamond(fDiamondProfileSPD);

    if (fRunV0Finder) {
       // V0 finding
       IlcV0vertexer vtxer;
       // get cuts for V0vertexer from IlcGRPRecoParam
       if (grpRecoParam) {
	 Int_t nCutsV0vertexer = grpRecoParam->GetVertexerV0NCuts();
	 Double_t cutsV0vertexer[nCutsV0vertexer];
	 grpRecoParam->GetVertexerV0Cuts(cutsV0vertexer);
	 vtxer.SetCuts(cutsV0vertexer);
       }
       vtxer.Tracks2V0vertices(fesd);
       IlcSysInfo::AddStamp(Form("V0Finder_%d",iEvent), 0,0,iEvent); 

       if (fRunCascadeFinder) {
          // Cascade finding
          IlcCascadeVertexer cvtxer;
	  // get cuts for CascadeVertexer from IlcGRPRecoParam
	  if (grpRecoParam) {
	    Int_t nCutsCascadeVertexer = grpRecoParam->GetVertexerCascadeNCuts();
	    Double_t cutsCascadeVertexer[nCutsCascadeVertexer];
	    grpRecoParam->GetVertexerCascadeCuts(cutsCascadeVertexer);
	    cvtxer.SetCuts(cutsCascadeVertexer);
	  }
          cvtxer.V0sTracks2CascadeVertices(fesd);
	  IlcSysInfo::AddStamp(Form("CascadeFinder_%d",iEvent), 0,0,iEvent); 
       }
    }

    // AdC+FN
// // // //     if (fReconstructor[4])
// // // //       GetReconstructor(4)->FillEventTimeWithTOF(fesd,&pid);

    // combined PID
    pid.MakePID(fesd);

    if (fFillTriggerESD) {
      if (!FillTriggerESD(fesd)) {
	if (fStopOnError) {CleanUp(); return kFALSE;}
      }
    }
    // Always fill scalers
    if (!FillTriggerScalers(fesd)) {
       if (fStopOnError) {CleanUp(); return kFALSE;}
    }

    IlcSysInfo::AddStamp(Form("FillVaria_%d",iEvent), 0,0,iEvent); 

    // write ESD
    UInt_t specie = fesd->GetEventSpecie();
    Bool_t keepAll = (specie==IlcRecoParam::kCosmic || specie==IlcRecoParam::kCalib);
    if (fCleanESD && (!keepAll) ) {
      CleanESD(fesd);
      IlcSysInfo::AddStamp(Form("CleanESD_%d",iEvent), 0,0,iEvent); 
    }
    // 
    // RS run updated trackleter: since we want to mark the clusters used by tracks and also mark the 
    // tracks interpreted as primary, this step should be done in the very end, when full 
    // ESD info is available (particulalry, V0s)
    // vertex finder
    if (fRunMultFinder) {
      if (!RunMultFinder(fesd)) {
	if (fStopOnError) {CleanUp(); return kFALSE;}
      }
      IlcSysInfo::AddStamp(Form("MultFinder_%d",iEvent), 0,0,iEvent); 
    }

  if (fRunQA && IsInTasks(IlcQAv1::kESDS)) {
    IlcQAManager::QAManager()->SetEventSpecie(fRecoParam.GetEventSpecie()) ;
// // // //     IlcQAManager::QAManager()->RunOneEvent(fesd, fhltesd) ; 
    IlcSysInfo::AddStamp(Form("RunQA_%d",iEvent), 0,0,iEvent); 
  }
  if (fRunGlobalQA) {
    IlcQADataMaker *qadm = IlcQAManager::QAManager()->GetQADataMaker(IlcQAv1::kGLOBAL);
    if (qadm)
      qadm->SetEventSpecie(fRecoParam.GetEventSpecie()) ;
    if (qadm && IsInTasks(IlcQAv1::kESDS))
      qadm->Exec(IlcQAv1::kESDS, fesd);
    IlcSysInfo::AddStamp(Form("RunGlobQA_%d",iEvent), 0,0,iEvent);     
  }

// // // //   // copy HLT decision from HLTesd to esd
// // // //   // the most relevant information is stored in a reduced container in the esd,
// // // //   // while the full information can be found in the HLTesd
// // // //   TObject* pHLTSrc=fhltesd->FindListObject(IlcESDHLTDecision::Name());
// // // //   TObject* pHLTTgt=fesd->FindListObject(IlcESDHLTDecision::Name());
// // // //   if (pHLTSrc && pHLTTgt) {
// // // //     pHLTSrc->Copy(*pHLTTgt);
// // // //   }
  //
  // Perform analysis of this event if requested
  // RS: Should be done before WriteESDfriend, since the latter may clean the esdfriend
// // // //   if (fAnalysis) {
// // // //     fRecoHandler->BeginEvent(iEvent);
// // // //     fAnalysis->ExecAnalysis();
// // // //     fRecoHandler->FinishEvent();
// // // //     IlcSysInfo::AddStamp(Form("Analysis_%d",iEvent), 0,0,iEvent);     
// // // //   }  
  //
  if (fWriteESDfriend) {
    fesd->GetESDfriend(fesdf);
    IlcSysInfo::AddStamp(Form("CreateFriend_%d",iEvent), 0,0,iEvent);     
  
  }
  //
  ftree->Fill();
  IlcSysInfo::AddStamp(Form("ESDFill_%d",iEvent), 0,0,iEvent);     
  //
  if (fWriteESDfriend) {
    WriteESDfriend();
    IlcSysInfo::AddStamp(Form("WriteFriend_%d",iEvent), 0,0,iEvent);     
  }
  //
  //
  // Auto-save the ESD tree in case of prompt reco @P2
  if (fRawReader && fRawReader->UseAutoSaveESD()) {
    ftree->AutoSave("SaveSelf");
    if (fWriteESDfriend) ftreeF->AutoSave("SaveSelf");
  }
    // write HLT ESD
// // // //     fhlttree->Fill();

    // call IlcEVE
    if (fRunIlcEVE) RunIlcEVE();
    //
    fesd->Reset();
// // // //     fhltesd->Reset();
    if (fWriteESDfriend) {
      fesdf->~IlcESDfriend();
      new (fesdf) IlcESDfriend(); // Reset...
    }
 
    gSystem->GetProcInfo(&procInfo);
    Long_t dMres=(procInfo.fMemResident-oldMres)/1024;
    Long_t dMvir=(procInfo.fMemVirtual-oldMvir)/1024;
    Float_t dCPU=procInfo.fCpuUser+procInfo.fCpuSys-oldCPU;
    aveDMres+=(dMres-aveDMres)/(iEvent-fFirstEvent+1);
    aveDMvir+=(dMvir-aveDMvir)/(iEvent-fFirstEvent+1);
    aveDCPU+=(dCPU-aveDCPU)/(iEvent-fFirstEvent+1);
    IlcInfo(Form("======================= End Event %d: Res %ld(%3ld <%3ld>) Vir %ld(%3ld <%3ld>) CPU %5.2f <%5.2f> ===================",
		 iEvent, procInfo.fMemResident/1024, dMres, aveDMres, procInfo.fMemVirtual/1024, dMvir, aveDMvir, dCPU, aveDCPU));
    oldMres=procInfo.fMemResident;
    oldMvir=procInfo.fMemVirtual;
    oldCPU=procInfo.fCpuUser+procInfo.fCpuSys;
  
    fEventInfo.Reset();
    for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
      if (fReconstructor[iDet]) {
	fReconstructor[iDet]->SetRecoParam(NULL);
	fReconstructor[iDet]->SetEventInfo(NULL);
      }
      if (fTracker[iDet]) fTracker[iDet]->SetEventInfo(NULL);
    }
	
  if (fRunQA || fRunGlobalQA)
    IlcQAManager::QAManager()->Increment() ; 

  DeleteRecPoints(fDeleteRecPoints);
  DeleteDigits(fDeleteDigits);
  //
  return kTRUE;
}

//_____________________________________________________________________________
void IlcReconstruction::SlaveTerminate()
{
  // Finalize the run on the slave side
  // Called after the exit
  // from the event loop
  IlcCodeTimerAuto("",0);
  // If analysis was done during reconstruction, we need to call SlaveTerminate for it
// // // //   if (fAnalysis) {
// // // //      fAnalysis->PackOutput(fOutput);
// // // //      fAnalysis->SetSkipTerminate(kTRUE);
// // // //      fAnalysis->Terminate();
// // // //   }   

  if (fIsNewRunLoader) { // gilc.root didn't exist
    fRunLoader->WriteHeader("OVERWRITE");
    fRunLoader->WriteTrigger("OVERWRITE");
    fRunLoader->CdGAFile();
    fRunLoader->Write(0, TObject::kOverwrite);
  }

  const TMap *cdbMap = IlcCDBManager::Instance()->GetStorageMap();	 
  const TList *cdbList = IlcCDBManager::Instance()->GetRetrievedIds();	 
	 	 
   TMap *cdbMapCopy = new TMap(cdbMap->GetEntries());	 
   cdbMapCopy->SetOwner(1);	 
   cdbMapCopy->SetName("cdbMap");	 
   TIter iter(cdbMap->GetTable());	 
 	 
   TPair* pair = 0;	 
   while((pair = dynamic_cast<TPair*> (iter.Next()))){	 
         TObjString* keyStr = dynamic_cast<TObjString*> (pair->Key());	 
         TObjString* valStr = dynamic_cast<TObjString*> (pair->Value());
	 if (keyStr && valStr)
	   cdbMapCopy->Add(new TObjString(keyStr->GetName()), new TObjString(valStr->GetName()));	 
   }	 
 	 
   TList *cdbListCopy = new TList();	 
   cdbListCopy->SetOwner(1);	 
   cdbListCopy->SetName("cdbList");	 
 	 
   TIter iter2(cdbList);	 
 	 
	IlcCDBId* id=0;
	while((id = dynamic_cast<IlcCDBId*> (iter2.Next()))){	 
         cdbListCopy->Add(new TObjString(id->ToString().Data()));	 
   }	 
 	 
   ftree->GetUserInfo()->Add(cdbMapCopy);	 
   ftree->GetUserInfo()->Add(cdbListCopy);

   // Add the IlcRoot version that created this file
   TString sVersion("ilcroot ");
   sVersion += ILCROOT_SVN_BRANCH;
   sVersion += ":";
   sVersion += ILCROOT_SVN_REVISION;
   sVersion += "; root ";
   sVersion += ROOT_SVN_BRANCH;
   sVersion += ":";
   sVersion += ROOT_SVN_REVISION;
   sVersion += "; metadata ";
   sVersion += getenv("PRODUCTION_METADATA");
		    

   TNamed * ilcrootVersion = new TNamed("ilcrootVersion",sVersion.Data());
   ftree->GetUserInfo()->Add(ilcrootVersion); // The list becomes owner of ilcrootVersion

  ffile->cd();

  // we want to have only one tree version number
  ftree->Write(ftree->GetName(),TObject::kOverwrite);
// // // //   fhlttree->Write(fhlttree->GetName(),TObject::kOverwrite);

  if (fWriteESDfriend) {
    ffileF->cd();
    ftreeF->Write(ftreeF->GetName(),TObject::kOverwrite);
  }

// Finish with Plane Efficiency evaluation: before of CleanUp !!!
  if (fRunPlaneEff && !FinishPlaneEff()) {
   IlcWarning("Finish PlaneEff evaluation failed");
  }

  for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
    if (fReconstructor[iDet]) fReconstructor[iDet]->Terminate();
  }
  // End of cycle for the in-loop  

  if (fRunQA || fRunGlobalQA) {
    IlcQAManager::QAManager()->EndOfCycle() ;
    if (fInput &&
	!fProofOutputLocation.IsNull() &&
	fProofOutputArchive.IsNull() &&
	!fProofOutputDataset) {
      TString qaOutputFile(Form("%sMerged.%s.Data.root",
				fProofOutputLocation.Data(),
				IlcQAv1::GetQADataFileName()));
      TProofOutputFile *qaProofFile = new TProofOutputFile(Form("Merged.%s.Data.root",
								IlcQAv1::GetQADataFileName()));
      qaProofFile->SetOutputFileName(qaOutputFile.Data());
      if (IlcDebugLevel() > 0) qaProofFile->Dump();
      fOutput->Add(qaProofFile);
      MergeQA(qaProofFile->GetFileName());
    }
    else {
      MergeQA();
    }
  }

  gROOT->cd();
  CleanUp();

  if (fInput) {
    if (!fProofOutputFileName.IsNull() &&
	!fProofOutputLocation.IsNull() &&
	fProofOutputDataset &&
	!fProofOutputArchive.IsNull()) {
      TProofOutputFile *zipProofFile = new TProofOutputFile(fProofOutputFileName.Data(),
							    "DROV",
							    fProofOutputLocation.Data());
      if (IlcDebugLevel() > 0) zipProofFile->Dump();
      fOutput->Add(zipProofFile);
      TString fileList(fProofOutputArchive.Data());
      fileList.ReplaceAll(","," ");
      TString command;
#if ROOT_SVN_REVISION >= 30174
      command.Form("zip -n root %s/%s %s",zipProofFile->GetDir(kTRUE),zipProofFile->GetFileName(),fileList.Data());
#else
      command.Form("zip -n root %s/%s %s",zipProofFile->GetDir(),zipProofFile->GetFileName(),fileList.Data());
#endif
      IlcInfo(Form("Executing: %s",command.Data()));
      gSystem->Exec(command.Data());
    }
  }
}
    
//_____________________________________________________________________________
void IlcReconstruction::Terminate()
{
  // Create tags for the events in the ESD tree (the ESD tree is always present)
  // In case of empty events the tags will contain dummy values
  IlcCodeTimerAuto("",0);

  // Do not call the ESD tag creator in case of PROOF-based reconstruction
  if (!fInput) {
    IlcESDTagCreator *esdtagCreator = new IlcESDTagCreator();
    esdtagCreator->CreateESDTags(fFirstEvent,fLastEvent,fGRPData, IlcQAv1::Instance()->GetQA(), IlcQAv1::Instance()->GetEventSpecies(), IlcQAv1::kNDET, IlcRecoParam::kNSpecies);
    delete esdtagCreator;
  }

  // Cleanup of CDB manager: cache and active storages!
  IlcCDBManager::Instance()->ClearCache();
}

//_____________________________________________________________________________
Bool_t IlcReconstruction::RunLocalEventReconstruction(const TString& detectors)
{
// run the local reconstruction

  static Int_t eventNr=0;
  IlcCodeTimerAuto("",0)

  TString detStr = detectors;
  // execute HLT reconstruction first since other detector reconstruction
  // might depend on HLT data
  // key 'HLT' is removed from detStr by IsSelected
  if (!IsSelected("HLT", detStr)) {
    IlcReconstructor* reconstructor = GetReconstructor(kNDetectors-1);
    if (reconstructor) {
      // there is no IlcLoader for HLT, see
      // https://savannah.cern.ch/bugs/?35473
      IlcInfo("running reconstruction for HLT");
      if (fRawReader) {
	IlcInfo("reconstructor->Reconstruct(fRawReader, NULL)");
        reconstructor->Reconstruct(fRawReader, NULL);
      } 
      else {
	IlcInfo("reconstructor->Reconstruct(dummy, NULL)");
        TTree* dummy=NULL;
        reconstructor->Reconstruct(dummy, NULL);
      }
    }
    IlcSysInfo::AddStamp(Form("LRecHLT_%d",eventNr), -1,1,eventNr);
  }

  IlcInfo(Form("kNDetectors = %d",kNDetectors));

  for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
    if (!IsSelected(fgkDetectorName[iDet], detStr)) continue;
    IlcDebug(1, Form("Detector: %s", fgkDetectorName[iDet]));
    IlcReconstructor* reconstructor = GetReconstructor(iDet);
    if (!reconstructor) continue;
    IlcLoader* loader = fLoader[iDet];
    if (!loader) {
      IlcWarning(Form("No loader is defined for %s!",fgkDetectorName[iDet]));
      continue;
    }
    // conversion of digits
    if (fRawReader && reconstructor->HasDigitConversion()) {
      IlcInfo(Form("converting raw data digits into root objects for %s", 
		   fgkDetectorName[iDet]));
//      IlcCodeTimerAuto(Form("converting raw data digits into root objects for %s", 
//                            fgkDetectorName[iDet]),0);
      loader->LoadDigits("update");
      loader->CleanDigits();
      loader->MakeDigitsContainer();
      TTree* digitsTree = loader->TreeD();
      reconstructor->ConvertDigits(fRawReader, digitsTree);
      loader->WriteDigits("OVERWRITE");
      loader->UnloadDigits();
    }
    // local reconstruction
    IlcInfo(Form("running reconstruction for %s", fgkDetectorName[iDet]));
    //IlcCodeTimerAuto(Form("running reconstruction for %s", fgkDetectorName[iDet]),0);
    IlcDebug(1, "Loading Rec Points");
    loader->LoadRecPoints("update");
    IlcDebug(1, "Cleaning Rec Points");
    loader->CleanRecPoints();
    IlcDebug(1, "Making Rec Points Container");
    loader->MakeRecPointsContainer();
    TTree* clustersTree = loader->TreeR();
    if (fRawReader && !reconstructor->HasDigitConversion()) {
      reconstructor->Reconstruct(fRawReader, clustersTree);
    } 
    else {
      IlcDebug(1, "Loading Digits");
      loader->LoadDigits("read");
      TTree* digitsTree = loader->TreeD();
      IlcDebug(1, Form("Digits Tree = %p",digitsTree));
      if (!digitsTree) {
        IlcError(Form("Can't get the %s digits tree", fgkDetectorName[iDet]));
        if (fStopOnError) 
          return kFALSE;
      } 
      else {
	IlcDebug(1, "Digits -> Clusters");
        reconstructor->Reconstruct(digitsTree, clustersTree);
        if (fRunQA && IsInTasks(IlcQAv1::kDIGITSR)) {
          IlcQAManager::QAManager()->SetEventSpecie(fRecoParam.GetEventSpecie()) ;
          IlcQAManager::QAManager()->RunOneEventInOneDetector(iDet, digitsTree) ; 
        }
      }
      loader->UnloadDigits();
    }
    if (fRunQA && IsInTasks(IlcQAv1::kRECPOINTS)) {
      IlcQAManager::QAManager()->SetEventSpecie(fRecoParam.GetEventSpecie()) ;
      IlcQAManager::QAManager()->RunOneEventInOneDetector(iDet, clustersTree) ; 
    }
    loader->WriteRecPoints("OVERWRITE");
    loader->UnloadRecPoints();
    IlcSysInfo::AddStamp(Form("LRec%s_%d",fgkDetectorName[iDet],eventNr), iDet,1,eventNr);
  }
  if (!IsSelected("CTP", detStr)) IlcDebug(10,"No CTP");
  if ((detStr.CompareTo("ALL") != 0) && !detStr.IsNull()) {
    IlcError(Form("the following detectors were not found: %s",
                  detStr.Data()));
    if (fStopOnError) 
      return kFALSE;
  }
  eventNr++;
  return kTRUE;
}
//_____________________________________________________________________________
Bool_t IlcReconstruction::RunSPDTrackleting(IlcESDEvent*& esd)
{
// run the SPD trackleting (for SPD efficiency purpouses)

  IlcCodeTimerAuto("",0)

  Double_t vtxPos[3] = {0, 0, 0};
  Double_t vtxErr[3] = {0.0, 0.0, 0.0};
/*
  TArrayF m
/
cVertex(3);
  // if(MC)
  if (fRunLoader->GetHeader() && fRunLoader->GetHeader()->GenEventHeader()) {
    fRunLoader->GetHeader()->GenEventHeader()->PrimaryVertex(mcVertex);
    for (Int_t i = 0; i < 3; i++) vtxPos[i] = mcVertex[i];
  }
*/
  const IlcESDVertex *vertex = esd->GetVertex();
  if(!vertex){
    IlcWarning("Vertex not found");
    return kFALSE;
  }
  vertex->GetXYZ(vtxPos);
  vertex->GetSigmaXYZ(vtxErr);
  if (fSPDTrackleter) {
    IlcInfo("running the SPD Trackleter for Plane Efficiency Evaluation");

    // load clusters
    fLoader[0]->LoadRecPoints("read");
    TTree* tree = fLoader[0]->TreeR();
    if (!tree) {
      IlcError("Can't get the ITS cluster tree");
      return kFALSE;
    }
    fSPDTrackleter->LoadClusters(tree);
    fSPDTrackleter->SetVertex(vtxPos, vtxErr);
    // run trackleting
    if (fSPDTrackleter->Clusters2Tracks(esd) != 0) {
      IlcWarning("IlcITSTrackleterSPDEff Clusters2Tracks failed");
     // fLoader[0]->UnloadRecPoints();
      return kFALSE;
    }
//fSPDTrackleter->UnloadRecPoints();
  } else {
    IlcWarning("SPDTrackleter not available");
    return kFALSE;
  }
  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcReconstruction::RunVertexFinder(IlcESDEvent*& esd)
{
// run the barrel tracking

  IlcCodeTimerAuto("",0)

  IlcVertexer *vertexer = CreateVertexer();
  if (!vertexer) return kFALSE;

  IlcInfo(Form("running the ITS vertex finder: %s",vertexer->ClassName()));
  IlcESDVertex* vertex = NULL;
  if (fLoader[0]) {
    fLoader[0]->LoadRecPoints();
    TTree* cltree = fLoader[0]->TreeR();
    if (cltree) {
      if(fDiamondProfileSPD) vertexer->SetVtxStart(fDiamondProfileSPD);
      vertex = vertexer->FindVertexForCurrentEvent(cltree);
    }
    else {
      IlcError("Can't get the ITS cluster tree");
    }
    fLoader[0]->UnloadRecPoints();
  }
  else {
    IlcError("Can't get the ITS loader");
  }
  if(!vertex){
    IlcWarning("Vertex not found");
    vertex = new IlcESDVertex();
    vertex->SetName("default");
  }
  else {
    vertex->SetName("reconstructed");
  }

  Double_t vtxPos[3];
  Double_t vtxErr[3];
  vertex->GetXYZ(vtxPos);
  vertex->GetSigmaXYZ(vtxErr);

  esd->SetPrimaryVertexSPD(vertex);
  IlcESDVertex *vpileup = NULL;
  Int_t novertices = 0;
  vpileup = vertexer->GetAllVertices(novertices);
  if(novertices>1){
    for (Int_t kk=1; kk<novertices; kk++)esd->AddPileupVertexSPD(&vpileup[kk]);
  }
  /*
  // if SPD multiplicity has been determined, it is stored in the ESD
  IlcMultiplicity *mult = vertexer->GetMultiplicity();
  if(mult)esd->SetMultiplicity(mult);
  */
  for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
    if (fTracker[iDet]) fTracker[iDet]->SetVertex(vtxPos, vtxErr);
  }  
  delete vertex;

  delete vertexer;

  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcReconstruction::RunMultFinder(IlcESDEvent*& esd)
{
  // run the trackleter for multiplicity study

  IlcCodeTimerAuto("",0)

  IlcTrackleter *trackleter = CreateMultFinder();
  if (!trackleter) return kFALSE;

  IlcInfo(Form("running the ITS multiplicity finder: %s",trackleter->ClassName()));

  if (fLoader[0]) {
    fLoader[0]->LoadRecPoints();
    TTree* cltree = fLoader[0]->TreeR();
    if (cltree) {
      trackleter->Reconstruct(esd,cltree);
      IlcMultiplicity *mult = trackleter->GetMultiplicity();
      if(mult) esd->SetMultiplicity(mult);
    }
    else {
      IlcError("Can't get the ITS cluster tree");
    }
    fLoader[0]->UnloadRecPoints();
  }
  else {
    IlcError("Can't get the ITS loader");
  }

  delete trackleter;

  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcReconstruction::RunHLTTracking(IlcESDEvent*& esd)
{
// run the HLT barrel tracking

  IlcCodeTimerAuto("",0)

  if (!fRunLoader) {
    IlcError("Missing runLoader!");
    return kFALSE;
  }

  IlcInfo("running HLT tracking");

  // Get a pointer to the HLT reconstructor
  IlcReconstructor *reconstructor = GetReconstructor(kNDetectors-1);
  if (!reconstructor) return kFALSE;

  // TPC + ITS
  for (Int_t iDet = 1; iDet >= 0; iDet--) {
    TString detName = fgkDetectorName[iDet];
    IlcDebug(1, Form("%s HLT tracking", detName.Data()));
    reconstructor->SetOption(detName.Data());
    IlcTracker *tracker = reconstructor->CreateTracker();
    if (!tracker) {
      IlcWarning(Form("couldn't create a HLT tracker for %s", detName.Data()));
      if (fStopOnError) return kFALSE;
      continue;
    }
    Double_t vtxPos[3];
    Double_t vtxErr[3]={0.005,0.005,0.010};
    const IlcESDVertex *vertex = esd->GetVertex();
    vertex->GetXYZ(vtxPos);
    tracker->SetVertex(vtxPos,vtxErr);
    if(iDet != 1) {
      fLoader[iDet]->LoadRecPoints("read");
      TTree* tree = fLoader[iDet]->TreeR();
      if (!tree) {
	IlcError(Form("Can't get the %s cluster tree", detName.Data()));
	return kFALSE;
      }
      tracker->LoadClusters(tree);
    }
    if (tracker->Clusters2Tracks(esd) != 0) {
      IlcError(Form("HLT %s Clusters2Tracks failed", fgkDetectorName[iDet]));
      return kFALSE;
    }
    if(iDet != 1) {
      tracker->UnloadClusters();
    }
    delete tracker;
  }

  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcReconstruction::RunMuonTracking(IlcESDEvent*& esd)
{
// run the muon spectrometer tracking

  IlcCodeTimerAuto("",0)

  if (!fRunLoader) {
    IlcError("Missing runLoader!");
    return kFALSE;
  }
  Int_t iDet =  GetDetIndex("MUON"); // for MUON

  // Get a pointer to the MUON reconstructor
  IlcReconstructor *reconstructor = GetReconstructor(iDet);
  if (!reconstructor) return kFALSE;

  
  TString detName = fgkDetectorName[iDet];
  IlcDebug(1, Form("%s tracking", detName.Data()));
  IlcTracker *tracker =  reconstructor->CreateTracker();
  if (!tracker) {
    IlcWarning(Form("couldn't create a tracker for %s", detName.Data()));
    return kFALSE;
  }
     
  // read RecPoints
  fLoader[iDet]->LoadRecPoints("read");  

  tracker->LoadClusters(fLoader[iDet]->TreeR());
  
  Int_t rv = tracker->Clusters2Tracks(esd);
  
  fLoader[iDet]->UnloadRecPoints();

  tracker->UnloadClusters();
  
  if ( rv )
  {
    IlcError(Form("%s Clusters2Tracks failed", fgkDetectorName[iDet]));
    return kFALSE;
  }
  
  return kTRUE;
}


//_____________________________________________________________________________
Bool_t IlcReconstruction::RunTracking(IlcESDEvent*& esd,IlcESDpid &PID)
{
// run the barrel tracking
  static Int_t eventNr=0;
  IlcCodeTimerAuto("",0)

  IlcInfo("running tracking");

  // Set the event info which is used
  // by the trackers in order to obtain
  // information about read-out detectors,
  // trigger etc.
  IlcDebug(1, "Setting event info");
  for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
    if (!fTracker[iDet]) continue;
    fTracker[iDet]->SetEventInfo(&fEventInfo);
  }

// // // //   //Fill the ESD with the T0 info (will be used by the TOF)
// // // //   if (fReconstructor[12] && fLoader[12]) {
// // // //     fLoader[12]->LoadRecPoints("READ");
// // // //     TTree *treeR = fLoader[12]->TreeR();
// // // //     if (treeR) {
// // // //       GetReconstructor(12)->FillESD((TTree *)NULL,treeR,esd);
// // // //     }
// // // //   }

  // pass 1: DCH + ITS inwards
  for (Int_t iDet = 1; iDet >= 0; iDet--) {
    if (!fTracker[iDet]) continue;
    IlcDebug(1, Form("%s tracking", fgkDetectorName[iDet]));

    // load clusters
    fLoader[iDet]->LoadRecPoints("read");
    IlcSysInfo::AddStamp(Form("RLoadCluster%s_%d",fgkDetectorName[iDet],eventNr),iDet,1, eventNr);
    TTree* tree = fLoader[iDet]->TreeR();
    if (!tree) {
      IlcError(Form("Can't get the %s cluster tree", fgkDetectorName[iDet]));
      return kFALSE;
    }
    fTracker[iDet]->LoadClusters(tree);
    IlcSysInfo::AddStamp(Form("TLoadCluster%s_%d",fgkDetectorName[iDet],eventNr), iDet,2, eventNr);
    // run tracking
    if (fTracker[iDet]->Clusters2Tracks(esd) != 0) {
      IlcError(Form("%s Clusters2Tracks failed", fgkDetectorName[iDet]));
      return kFALSE;
    }
    IlcSysInfo::AddStamp(Form("Tracking0%s_%d",fgkDetectorName[iDet],eventNr), iDet,3,eventNr);
    // preliminary PID in DCH needed by the ITS tracker
    if (iDet == 1) {
      GetReconstructor(iDet)->FillESD((TTree*)NULL, (TTree*)NULL, esd);
      PID.MakePID(esd,kTRUE);
      IlcSysInfo::AddStamp(Form("MakePID0%s_%d",fgkDetectorName[iDet],eventNr), iDet,4,eventNr);
    } 
  }

  // pass 2: ALL backwards

  for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
    if (!fTracker[iDet]) continue;
    IlcDebug(1, Form("%s back propagation", fgkDetectorName[iDet]));

    // load clusters
    if (iDet > 2) {     // all except VXD, DCH
      TTree* tree = NULL;
      fLoader[iDet]->LoadRecPoints("read");
      IlcSysInfo::AddStamp(Form("RLoadCluster0%s_%d",fgkDetectorName[iDet],eventNr), iDet,1, eventNr);
      tree = fLoader[iDet]->TreeR();
      if (!tree) {
        IlcError(Form("Can't get the %s cluster tree", fgkDetectorName[iDet]));
        return kFALSE;
      }
      fTracker[iDet]->LoadClusters(tree); 
      IlcSysInfo::AddStamp(Form("TLoadCluster0%s_%d",fgkDetectorName[iDet],eventNr), iDet,2, eventNr);
    }

    // run tracking
    if (iDet>1) // start filling residuals for the "outer" detectors
      if (fRunGlobalQA) {
        IlcTracker::SetFillResiduals(fRecoParam.GetEventSpecie(), kTRUE);     
        TObjArray ** arr = IlcTracker::GetResidualsArray() ; 
	if (arr) {
	  IlcRecoParam::EventSpecie_t es=fRecoParam.GetEventSpecie();
	  TObjArray * elem = arr[IlcRecoParam::AConvert(es)];
	  if ( elem && (! elem->At(0)) ) {
	    IlcQADataMaker *qadm = IlcQAManager::QAManager()->GetQADataMaker(IlcQAv1::kGLOBAL);
	    if (qadm) qadm->InitRecPointsForTracker() ; 
	  }
	}
	//	IlcSysInfo::AddStamp(Form("QAInitResid%s_%d",fgkDetectorName[iDet],eventNr), iDet,0, eventNr);
      }
    if (fTracker[iDet]->PropagateBack(esd) != 0) {
      IlcError(Form("%s backward propagation failed", fgkDetectorName[iDet]));
      //      return kFALSE;
    }
    IlcSysInfo::AddStamp(Form("Tracking1%s_%d",fgkDetectorName[iDet],eventNr), iDet,3, eventNr);

    // unload clusters
    if (iDet > 1) {     // all except VXD, DCH
      fTracker[iDet]->UnloadClusters();
      fLoader[iDet]->UnloadRecPoints();
    }
    // updated PID in DCH needed by the ITS tracker -MI
    if (iDet == 1) {
      //GetReconstructor(1)->FillESD((TTree*)NULL, (TTree*)NULL, esd);
      //IlcESDpid::MakePID(esd);
      PID.MakePID(esd,kTRUE);
      IlcSysInfo::AddStamp(Form("MakePID1%s_%d",fgkDetectorName[iDet],eventNr), iDet,4,eventNr);
    }

  }
  //stop filling residuals for the "outer" detectors
  if (fRunGlobalQA) IlcTracker::SetFillResiduals(fRecoParam.GetEventSpecie(), kFALSE);     

  // pass 3: DCH + ITS refit inwards

  for (Int_t iDet = 1; iDet >= 0; iDet--) {
    if (!fTracker[iDet]) continue;
    IlcDebug(1, Form("%s inward refit", fgkDetectorName[iDet]));

    // run tracking
    if (iDet<2) // start filling residuals for DCH and VXD
      if (fRunGlobalQA) {
        IlcTracker::SetFillResiduals(fRecoParam.GetEventSpecie(), kTRUE);     
        TObjArray ** arr = IlcTracker::GetResidualsArray() ; 
	if (arr) {
	  IlcRecoParam::EventSpecie_t es=fRecoParam.GetEventSpecie();
	  TObjArray * elem = arr[IlcRecoParam::AConvert(es)];
	  if ( elem && (! elem->At(0)) ) {
	    IlcQADataMaker *qadm = IlcQAManager::QAManager()->GetQADataMaker(IlcQAv1::kGLOBAL);
	    if (qadm) qadm->InitRecPointsForTracker() ; 
	  }
	}
      }
    
    if (fTracker[iDet]->RefitInward(esd) != 0) {
      IlcError(Form("%s inward refit failed", fgkDetectorName[iDet]));
      //      return kFALSE;
    }
    // run postprocessing
    if (fTracker[iDet]->PostProcess(esd) != 0) {
      IlcError(Form("%s postprocessing failed", fgkDetectorName[iDet]));
      //      return kFALSE;
    }
    IlcSysInfo::AddStamp(Form("Tracking2%s_%d",fgkDetectorName[iDet],eventNr), iDet,3, eventNr);
  }

  // write space-points to the ESD in case alignment data output
  // is switched on
  if (fWriteAlignmentData) {
    WriteAlignmentData(esd);
    IlcSysInfo::AddStamp(Form("WrtAlignData_%d",eventNr), 0,0, eventNr);
  }
  
  for (Int_t iDet = 1; iDet >= 0; iDet--) {
    if (!fTracker[iDet]) continue;
    // unload clusters
    fTracker[iDet]->UnloadClusters();
    IlcSysInfo::AddStamp(Form("TUnloadCluster%s_%d",fgkDetectorName[iDet],eventNr), iDet,4, eventNr);
    fLoader[iDet]->UnloadRecPoints();
    IlcSysInfo::AddStamp(Form("RUnloadCluster%s_%d",fgkDetectorName[iDet],eventNr), iDet,5, eventNr);
  }
  // stop filling residuals for DCH and VXD
  if (fRunGlobalQA) IlcTracker::SetFillResiduals(fRecoParam.GetEventSpecie(), kFALSE);     

  eventNr++;
  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcReconstruction::CleanESD(IlcESDEvent *esd){
  //
  // Remove the data which are not needed for the physics analysis.
  //

  Int_t nTracks=esd->GetNumberOfTracks();
  Int_t nV0s=esd->GetNumberOfV0s();
  IlcInfo
  (Form("Number of ESD tracks and V0s before cleaning: %d %d",nTracks,nV0s));

  Float_t cleanPars[]={fV0DCAmax,fV0CsPmin,fDmax,fZmax};
  Bool_t rc=esd->Clean(cleanPars);

  nTracks=esd->GetNumberOfTracks();
  nV0s=esd->GetNumberOfV0s();
  IlcInfo
  (Form("Number of ESD tracks and V0s after cleaning %d %d",nTracks,nV0s));

  return rc;
}

//_____________________________________________________________________________
Bool_t IlcReconstruction::FillESD(IlcESDEvent*& esd, const TString& detectors)
{
// fill the event summary data

  IlcCodeTimerAuto("",0)
    static Int_t eventNr=0; 
  TString detStr = detectors;
  
  IlcSysInfo::AddStamp(Form("FillESDb%d",eventNr), -19,-19, eventNr);
  for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
  if (!IsSelected(fgkDetectorName[iDet], detStr)) continue;
    IlcReconstructor* reconstructor = GetReconstructor(iDet);
    if (!reconstructor) continue;
    IlcDebug(1, Form("filling ESD for %s", fgkDetectorName[iDet]));
    TTree* clustersTree = NULL;
    if (fLoader[iDet]) {
      fLoader[iDet]->LoadRecPoints("read");
      clustersTree = fLoader[iDet]->TreeR();
      if (!clustersTree) {
	IlcError(Form("Can't get the %s clusters tree", 
		      fgkDetectorName[iDet]));
	if (fStopOnError) return kFALSE;
      }
    }
    if (fRawReader && !reconstructor->HasDigitConversion()) {
      reconstructor->FillESD(fRawReader, clustersTree, esd);
    } else {
      TTree* digitsTree = NULL;
      if (fLoader[iDet]) {
	fLoader[iDet]->LoadDigits("read");
	digitsTree = fLoader[iDet]->TreeD();
	if (!digitsTree) {
	  IlcError(Form("Can't get the %s digits tree", 
			fgkDetectorName[iDet]));
// // // // // 	  if (fStopOnError) return kFALSE; //FIXME
	}
      }
      reconstructor->FillESD(digitsTree, clustersTree, esd);
      if (fLoader[iDet]) fLoader[iDet]->UnloadDigits();
    }
    if (fLoader[iDet]) {
      fLoader[iDet]->UnloadRecPoints();
    }
  }
  
  if (!IsSelected("CTP", detStr)) IlcDebug(10,"No CTP");
  if ((detStr.CompareTo("ALL") != 0) && !detStr.IsNull()) {
    IlcError(Form("the following detectors were not found: %s", 
                  detStr.Data()));
    if (fStopOnError) return kFALSE;
  }
  IlcSysInfo::AddStamp(Form("FillESDe%d",eventNr), -20,-20, eventNr);
  eventNr++;
  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcReconstruction::FillTriggerESD(IlcESDEvent*& esd)
{
  // Reads the trigger decision which is
  // stored in Trigger.root file and fills
  // the corresponding esd entries

  IlcCodeTimerAuto("",0)
  
  IlcInfo("Filling trigger information into the ESD");

  if (fRawReader) {
    IlcCTPRawStream input(fRawReader);
    if (!input.Next()) {
      IlcWarning("No valid CTP (trigger) DDL raw data is found ! The trigger info is taken from the event header!");
    }
    else {
      if (esd->GetTriggerMask() != input.GetClassMask())
	IlcError(Form("Invalid trigger pattern found in CTP raw-data: %llx %llx",
		      input.GetClassMask(),esd->GetTriggerMask()));
      if (esd->GetOrbitNumber() != input.GetOrbitID())
	IlcError(Form("Invalid orbit id found in CTP raw-data: %x %x",
		      input.GetOrbitID(),esd->GetOrbitNumber()));
      if (esd->GetBunchCrossNumber() != input.GetBCID())
	IlcError(Form("Invalid bunch-crossing id found in CTP raw-data: %x %x",
		      input.GetBCID(),esd->GetBunchCrossNumber()));
      IlcESDHeader* esdheader = esd->GetHeader();
      esdheader->SetL0TriggerInputs(input.GetL0Inputs());
      esdheader->SetL1TriggerInputs(input.GetL1Inputs());
      esdheader->SetL2TriggerInputs(input.GetL2Inputs());
      // IR
      //      UInt_t orbit=input.GetOrbitID();
      for(Int_t i=0 ; i<input.GetNIRs() ; i++ ) {
	esdheader->AddTriggerIR(input.GetIR(i));
      }
       IlcCentralTrigger* rlCTP = fRunLoader->GetTrigger();
       if (rlCTP) {
	 rlCTP->SetL0TriggerInputs(input.GetL0Inputs());
	 rlCTP->SetL1TriggerInputs(input.GetL1Inputs());
	 rlCTP->SetL2TriggerInputs(input.GetL2Inputs());
       }
    }
    if (fIsNewRunLoader) fRunLoader->TreeCT()->Fill();
  }
  return kTRUE;
}
//_____________________________________________________________________________
Bool_t IlcReconstruction::FillTriggerScalers(IlcESDEvent*& esd)
{
  //Scalers
  //fRunScalers->Print();
  if(fRunScalers && fRunScalers->CheckRunScalers()){
     IlcTimeStamp* timestamp = new IlcTimeStamp(esd->GetOrbitNumber(), esd->GetPeriodNumber(), esd->GetBunchCrossNumber());
     //IlcTimeStamp* timestamp = new IlcTimeStamp(10308000, 0, (ULong64_t)486238);
     IlcESDHeader* esdheader = fesd->GetHeader();
     for(Int_t i=0;i<50;i++){
          if((1ull<<i) & esd->GetTriggerMask()){
          IlcTriggerScalersESD* scalesd = fRunScalers->GetScalersForEventClass( timestamp, i+1);
          if(scalesd)esdheader->SetTriggerScalersRecord(scalesd);
        }
     }
     const IlcTriggerScalersRecordESD* scalrecEvent = fRunScalers->GetScalersDeltaForEvent( timestamp);
     const IlcTriggerScalersRecordESD* scalrecRun = fRunScalers->GetScalersDeltaForRun();
     if (scalrecEvent) esdheader->SetTriggerScalersDeltaEvent(scalrecEvent);
     if (scalrecRun) esdheader->SetTriggerScalersDeltaRun(scalrecRun);
  }
  return kTRUE;
}
//_____________________________________________________________________________
Bool_t IlcReconstruction::FillRawEventHeaderESD(IlcESDEvent*& esd)
{
  // 
  // Filling information from RawReader Header
  // 

  if (!fRawReader) return kFALSE;

  IlcInfo("Filling information from RawReader Header");

  esd->SetBunchCrossNumber(fRawReader->GetBCID());
  esd->SetOrbitNumber(fRawReader->GetOrbitID());
  esd->SetPeriodNumber(fRawReader->GetPeriod());

  esd->SetTimeStamp(fRawReader->GetTimestamp());  
  esd->SetEventType(fRawReader->GetType());

  return kTRUE;
}


//_____________________________________________________________________________
Bool_t IlcReconstruction::IsSelected(TString detName, TString& detectors) const
{
// check whether detName is contained in detectors
// if yes, it is removed from detectors

  // check if all detectors are selected
  if ((detectors.CompareTo("ALL") == 0) ||
      detectors.BeginsWith("ALL ") ||
      detectors.EndsWith(" ALL") ||
      detectors.Contains(" ALL ")) {
    detectors = "ALL";
    return kTRUE;
  }

  // search for the given detector
  Bool_t result = kFALSE;
  if ((detectors.CompareTo(detName) == 0) ||
      detectors.BeginsWith(detName+" ") ||
      detectors.EndsWith(" "+detName) ||
      detectors.Contains(" "+detName+" ")) {
    detectors.ReplaceAll(detName, "");
    result = kTRUE;
  }

  // clean up the detectors string
  while (detectors.Contains("  ")) detectors.ReplaceAll("  ", " ");
  while (detectors.BeginsWith(" ")) detectors.Remove(0, 1);
  while (detectors.EndsWith(" ")) detectors.Remove(detectors.Length()-1, 1);

  return result;
}

//_____________________________________________________________________________
Bool_t IlcReconstruction::InitRunLoader()
{
// get or create the run loader

  if (gIlc) delete gIlc;
  gIlc = NULL;

  TFile *gafile = TFile::Open(fGIlcFileName.Data());
  //  if (!gSystem->AccessPathName(fGIlcFileName.Data())) { // gilc.root exists
  if (gafile) { // gilc.root exists
    gafile->Close();
    delete gafile;

    // load all base libraries to get the loader classes
    TString libs = gSystem->GetLibraries();
    for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
      TString detName = fgkDetectorName[iDet];
      if (detName == "HLT") continue;
      if (libs.Contains("lib" + detName + "base.so")) continue;
      gSystem->Load("lib" + detName + "base.so");
    }
    fRunLoader = IlcRunLoader::Open(fGIlcFileName.Data());
    if (!fRunLoader) {
      IlcError(Form("no run loader found in file %s", fGIlcFileName.Data()));
      CleanUp();
      return kFALSE;
    }

    fRunLoader->CdGAFile();
    fRunLoader->LoadgIlc();

    //PH This is a temporary fix to give access to the kinematics
    //PH that is needed for the labels of ITS clusters
    fRunLoader->LoadHeader();
    fRunLoader->LoadKinematics();

  } else {               // gilc.root does not exist
    if (!fRawReader) {
      IlcError(Form("the file %s does not exist", fGIlcFileName.Data()));
    }
    fRunLoader = IlcRunLoader::Open(fGIlcFileName.Data(),
				    IlcConfig::GetDefaultEventFolderName(),
				    "recreate");
    if (!fRunLoader) {
      IlcError(Form("could not create run loader in file %s", 
		    fGIlcFileName.Data()));
      CleanUp();
      return kFALSE;
    }
    fIsNewRunLoader = kTRUE;
    fRunLoader->MakeTree("E");
    fRunLoader->MakeTree("GG");

    if (fNumberOfEventsPerFile > 0)
      fRunLoader->SetNumberOfEventsPerFile(fNumberOfEventsPerFile);
    else
      fRunLoader->SetNumberOfEventsPerFile((UInt_t)-1);
  }

  return kTRUE;
}

//_____________________________________________________________________________
IlcReconstructor* IlcReconstruction::GetReconstructor(Int_t iDet)
{
// get the reconstructor object and the loader for a detector

  if (fReconstructor[iDet]) {
    if (fRecoParam.GetDetRecoParamArray(iDet) && !IlcReconstructor::GetRecoParam(iDet)) {
      const IlcDetectorRecoParam *par = fRecoParam.GetDetRecoParam(iDet);
      fReconstructor[iDet]->SetRecoParam(par);
      fReconstructor[iDet]->SetRunInfo(fRunInfo);
    }
    return fReconstructor[iDet];
  }

  // load the reconstructor object
  TPluginManager* pluginManager = gROOT->GetPluginManager();
  TString detName = fgkDetectorName[iDet];
  TString recName = "Ilc" + detName + "Reconstructor";

  if (!fIsNewRunLoader && !fRunLoader->GetLoader(detName+"Loader") && (detName != "HLT")) return NULL;

  IlcReconstructor* reconstructor = NULL;
  // first check if a plugin is defined for the reconstructor
  TPluginHandler* pluginHandler = 
    pluginManager->FindHandler("IlcReconstructor", detName);
  // if not, add a plugin for it
  if (!pluginHandler) {
    IlcDebug(1, Form("defining plugin for %s", recName.Data()));
    TString libs = gSystem->GetLibraries();
    if (libs.Contains("lib" + detName + "base.so") ||
	(gSystem->Load("lib" + detName + "base.so") >= 0)) {
      pluginManager->AddHandler("IlcReconstructor", detName, 
				recName, detName + "rec", recName + "()");
    } else {
      pluginManager->AddHandler("IlcReconstructor", detName, 
				recName, detName, recName + "()");
    }
    pluginHandler = pluginManager->FindHandler("IlcReconstructor", detName);
  }
  if (pluginHandler && (pluginHandler->LoadPlugin() == 0)) {
    reconstructor = (IlcReconstructor*) pluginHandler->ExecPlugin(0);
  }

   // check if the upgrade reconstructor should be used instead of the standard one
  if(fUpgradeMask[iDet]) {
    if(reconstructor) delete reconstructor;
    TClass *cl = new TClass(Form("Ali%sUpgradeReconstructor",fgkDetectorName[iDet]));
    reconstructor = (IlcReconstructor*)(cl->New());
   }

  if (reconstructor) {
    TObject* obj = fOptions.FindObject(detName.Data());
    if (obj) reconstructor->SetOption(obj->GetTitle());
    reconstructor->SetRunInfo(fRunInfo);
    reconstructor->Init();
    fReconstructor[iDet] = reconstructor;
  }

  // get or create the loader
  if (detName != "HLT") {
    fLoader[iDet] = fRunLoader->GetLoader(detName + "Loader");
    if (!fLoader[iDet]) {
      IlcConfig::Instance()
	->CreateDetectorFolders(fRunLoader->GetEventFolder(), 
				detName, detName);
      // first check if a plugin is defined for the loader
      pluginHandler = 
	pluginManager->FindHandler("IlcLoader", detName);
      // if not, add a plugin for it
      if (!pluginHandler) {
	TString loaderName = "Ilc" + detName + "Loader";
	IlcDebug(1, Form("defining plugin for %s", loaderName.Data()));
	pluginManager->AddHandler("IlcLoader", detName, 
				  loaderName, detName + "base", 
				  loaderName + "(const char*, TFolder*)");
	pluginHandler = pluginManager->FindHandler("IlcLoader", detName);
      }
      if (pluginHandler && (pluginHandler->LoadPlugin() == 0)) {
	fLoader[iDet] = 
	  (IlcLoader*) pluginHandler->ExecPlugin(2, detName.Data(), 
						 fRunLoader->GetEventFolder());
      }
      if (!fLoader[iDet]) {   // use default loader
	fLoader[iDet] = new IlcLoader(detName, fRunLoader->GetEventFolder());
      }
      if (!fLoader[iDet]) {
	IlcWarning(Form("couldn't get loader for %s", detName.Data()));
	if (fStopOnError) return NULL;
      } else {
	fRunLoader->AddLoader(fLoader[iDet]);
	fRunLoader->CdGAFile();
	if (gFile && !gFile->IsWritable()) gFile->ReOpen("UPDATE");
	fRunLoader->Write(0, TObject::kOverwrite);
      }
    }
  }
      
  if (fRecoParam.GetDetRecoParamArray(iDet) && !IlcReconstructor::GetRecoParam(iDet)) {
    const IlcDetectorRecoParam *par = fRecoParam.GetDetRecoParam(iDet);
    if (reconstructor) {
      reconstructor->SetRecoParam(par);
      reconstructor->SetRunInfo(fRunInfo);
    }
  }
  return reconstructor;
}

//_____________________________________________________________________________
IlcVertexer* IlcReconstruction::CreateVertexer()
{
// create the vertexer
// Please note that the caller is the owner of the
// vertexer

  IlcVertexer* vertexer = NULL;
  IlcReconstructor* itsReconstructor = GetReconstructor(0);
  if (itsReconstructor && ((fRunLocalReconstruction.Contains("TARGET")) ||
      fRunTracking.Contains("TARGET") || fFillESD.Contains("TARGET") )) {
    vertexer = itsReconstructor->CreateVertexer();
  }
  if (!vertexer) {
    IlcWarning("couldn't create a vertexer for ITS");
  }

  return vertexer;
}

//_____________________________________________________________________________
IlcTrackleter* IlcReconstruction::CreateMultFinder()
{
// create the ITS trackleter for mult. estimation
// Please note that the caller is the owner of the
// trackleter

  IlcTrackleter* trackleter = NULL;
  IlcReconstructor* itsReconstructor = GetReconstructor(0);
  if (itsReconstructor && ((fRunLocalReconstruction.Contains("TARGET")) ||
      fRunTracking.Contains("TARGET") || fFillESD.Contains("TARGET") )) {
    trackleter = itsReconstructor->CreateMultFinder();
  }
  else {
    IlcWarning("ITS is not in reconstruction, switching off RunMultFinder");
    fRunMultFinder = kFALSE;
  }

  return trackleter;
}

//_____________________________________________________________________________
Bool_t IlcReconstruction::CreateTrackers(const TString& detectors)
{
// create the trackers
	IlcInfo("Creating trackers");

  TString detStr = detectors;
  for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
    if (!IsSelected(fgkDetectorName[iDet], detStr)) continue;
    IlcReconstructor* reconstructor = GetReconstructor(iDet);
    if (!reconstructor) continue;
    TString detName = fgkDetectorName[iDet];
    if (detName == "HLT") {
      fRunHLTTracking = kTRUE;
      continue;
    }
    if (detName == "MUON") {
      fRunMuonTracking = kTRUE;
      continue;
    }

    fTracker[iDet] = reconstructor->CreateTracker();
    if (!fTracker[iDet] && (iDet < 7+1)) {
      IlcWarning(Form("couldn't create a tracker for %s", detName.Data()));
      if (fStopOnError) return kFALSE;
    }
    IlcSysInfo::AddStamp(Form("LTracker%s",fgkDetectorName[iDet]), iDet,0);
  }

  return kTRUE;
}

//_____________________________________________________________________________
void IlcReconstruction::CleanUp()
{
// delete trackers and the run loader and close and delete the file
/*
  for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
    delete fReconstructor[iDet];
    fReconstructor[iDet] = NULL;
    fLoader[iDet] = NULL;
    delete fTracker[iDet];
    fTracker[iDet] = NULL;
  }
*/

  delete fRunInfo;
  fRunInfo = NULL;

  delete fSPDTrackleter;
  fSPDTrackleter = NULL;

  delete ftVertexer;
  ftVertexer = NULL;
  
  delete fRunLoader;
  fRunLoader = NULL;
  delete fRawReader;
  fRawReader = NULL;
  delete fParentRawReader;
  fParentRawReader=NULL;

  if (ffile) {
    ffile->Close();
    delete ffile;
    ffile = NULL;
  }

  if (IlcQAManager::QAManager())
    IlcQAManager::QAManager()->ShowQA() ;
  //  IlcQAManager::Destroy() ;
// // // //   delete fAnalysis; 
// // // //   fAnalysis = NULL;
}

void IlcReconstruction::WriteAlignmentData(IlcESDEvent* esd)
{
  // Write space-points which are then used in the alignment procedures
  // For the moment only ITS, TPC, TRD and TOF

  Int_t ntracks = esd->GetNumberOfTracks();
  for (Int_t itrack = 0; itrack < ntracks; itrack++)
    {
      IlcESDtrack *track = esd->GetTrack(itrack);
      Int_t nsp = 0;
      Int_t idx[200];
      for (Int_t i=0; i<200; ++i) idx[i] = -1; //PH avoid uninitialized values
      for (Int_t iDet = 5; iDet >= 0; iDet--) {// TOF, TRD, TPC, ITS clusters
          nsp += (iDet==GetDetIndex("TRD")) ? track->GetTRDntracklets():track->GetNcls(iDet);

          if (iDet==GetDetIndex("TARGET")) { // VXD "extra" clusters
             track->GetClusters(iDet,idx);
             for (Int_t i=6; i<12; i++) if(idx[i] >= 0) nsp++;
          }  
      }

      if (nsp) {
	IlcTrackPointArray *sp = new IlcTrackPointArray(nsp);
	track->SetTrackPointArray(sp);
	Int_t isptrack = 0;
	for (Int_t iDet = 5; iDet >= 0; iDet--) {
	  IlcTracker *tracker = fTracker[iDet];
	  if (!tracker) continue;
	  Int_t nspdet = (iDet==GetDetIndex("TRD")) ? track->GetTRDtracklets(idx):track->GetClusters(iDet,idx);

      if (iDet==GetDetIndex("TARGET")) // VXD "extra" clusters
             for (Int_t i=6; i<12; i++) if(idx[i] >= 0) nspdet++;

	  if (nspdet <= 0) continue;
	  IlcTrackPoint p;
	  Int_t isp = 0;
	  Int_t isp2 = 0;
	  while (isp2 < nspdet) {
	    Bool_t isvalid=kTRUE;

            Int_t index=idx[isp++];
            if (index < 0) continue;

            TString dets = fgkDetectorName[iDet];
            if ((fUseTrackingErrorsForAlignment.CompareTo(dets) == 0) ||
            fUseTrackingErrorsForAlignment.BeginsWith(dets+" ") ||
            fUseTrackingErrorsForAlignment.EndsWith(" "+dets) ||
            fUseTrackingErrorsForAlignment.Contains(" "+dets+" ")) {
              isvalid = tracker->GetTrackPointTrackingError(index,p,track);
	    } else {
	      isvalid = tracker->GetTrackPoint(index,p); 
	    } 
	    isp2++;
	    if (!isvalid) continue;
	    if (iDet==GetDetIndex("TARGET") && (isp-1)>=6) p.SetExtra();
	    sp->AddPoint(isptrack,&p); isptrack++;
	  }
	}	
      }
    }
}

//_____________________________________________________________________________
void IlcReconstruction::FillRawDataErrorLog(Int_t iEvent, IlcESDEvent* esd)
{
  // The method reads the raw-data error log
  // accumulated within the rawReader.
  // It extracts the raw-data errors related to
  // the current event and stores them into
  // a TClonesArray inside the esd object.

  if (!fRawReader) return;

  for(Int_t i = 0; i < fRawReader->GetNumberOfErrorLogs(); i++) {

    IlcRawDataErrorLog *log = fRawReader->GetErrorLog(i);
    if (!log) continue;
    if (iEvent != log->GetEventNumber()) continue;

    esd->AddRawDataErrorLog(log);
  }

}

//_____________________________________________________________________________
// void IlcReconstruction::CheckQA()
// {
// check the QA of SIM for this run and remove the detectors 
// with status Fatal
  
//	TString newRunLocalReconstruction ; 
//	TString newRunTracking ;
//	TString newFillESD ;
//	 
//	for (Int_t iDet = 0; iDet < IlcQAv1::kNDET; iDet++) {
//		TString detName(IlcQAv1::GetDetName(iDet)) ;
//		IlcQAv1 * qa = IlcQAv1::Instance(IlcQAv1::DETECTORINDEX_t(iDet)) ;       
//      if ( qa->IsSet(IlcQAv1::DETECTORINDEX_t(iDet), IlcQAv1::kSIM, specie, IlcQAv1::kFATAL)) {
//        IlcInfo(Form("QA status for %s %s in Hits and/or SDIGITS  and/or Digits was Fatal; No reconstruction performed", 
//                   detName.Data(), IlcRecoParam::GetEventSpecieName(es))) ;
//			} else {
//			if ( fRunLocalReconstruction.Contains(IlcQAv1::GetDetName(iDet)) || 
//					fRunLocalReconstruction.Contains("ALL") )  {
//				newRunLocalReconstruction += detName ; 
//				newRunLocalReconstruction += " " ; 			
//			}
//			if ( fRunTracking.Contains(IlcQAv1::GetDetName(iDet)) || 
//					fRunTracking.Contains("ALL") )  {
//				newRunTracking += detName ; 
//				newRunTracking += " " ; 			
//			}
//			if ( fFillESD.Contains(IlcQAv1::GetDetName(iDet)) || 
//					fFillESD.Contains("ALL") )  {
//				newFillESD += detName ; 
//				newFillESD += " " ; 			
//			}
//		}
//	}
//	fRunLocalReconstruction = newRunLocalReconstruction ; 
//	fRunTracking            = newRunTracking ; 
//	fFillESD                = newFillESD ; 
// }

//_____________________________________________________________________________
Int_t IlcReconstruction::GetDetIndex(const char* detector)
{
  // return the detector index corresponding to detector
  Int_t index = -1 ; 
  for (index = 0; index < kNDetectors ; index++) {
    if ( strcmp(detector, fgkDetectorName[index]) == 0 )
	break ; 
  }	
  return index ; 
}
//_____________________________________________________________________________
Bool_t IlcReconstruction::FinishPlaneEff() {
 //
 // Here execute all the necessary operationis, at the end of the tracking phase,
 // in case that evaluation of PlaneEfficiencies was required for some detector.
 // E.g., write into a DataBase file the PlaneEfficiency which have been evaluated.
 //
 // This Preliminary version works only FOR ITS !!!!!
 // other detectors (TOF,TRD, etc. have to develop their specific codes)
 //
 //  Input: none
 //  Return: kTRUE if all operations have been done properly, kFALSE otherwise
 //
 Bool_t ret=kFALSE;
 TString detStr = fLoadCDB;
 //for (Int_t iDet = 0; iDet < fgkNDetectors; iDet++) {
 for (Int_t iDet = 0; iDet < 1; iDet++) { // for the time being only ITS
   if (!IsSelected(fgkDetectorName[iDet], detStr)) continue;
   if(fTracker[iDet] && fTracker[iDet]->GetPlaneEff()) {
      IlcPlaneEff *planeeff=fTracker[iDet]->GetPlaneEff();
      TString name=planeeff->GetName();
      name+=".root";
      TFile* pefile = TFile::Open(name, "RECREATE");
      ret=(Bool_t)planeeff->Write();
      pefile->Close();
      if(planeeff->GetCreateHistos()) {
        TString hname=planeeff->GetName();
        hname+="Histo.root";
        ret*=planeeff->WriteHistosToFile(hname,"RECREATE");
      }
   }
   if(fSPDTrackleter) {
     IlcPlaneEff *planeeff=fSPDTrackleter->GetPlaneEff();
      TString name="IlcITSPlaneEffSPDtracklet.root";
      TFile* pefile = TFile::Open(name, "RECREATE");
      ret=(Bool_t)planeeff->Write();
      pefile->Close();
      IlcESDEvent *dummy=NULL;
      ret=(Bool_t)fSPDTrackleter->PostProcess(dummy); // take care of writing other files
   }
 }
 return ret;
}
//_____________________________________________________________________________
Bool_t IlcReconstruction::InitPlaneEff() {
//
 // Here execute all the necessary operations, before of the tracking phase,
 // for the evaluation of PlaneEfficiencies, in case required for some detectors.
 // E.g., read from a DataBase file a first evaluation of the PlaneEfficiency
 // which should be updated/recalculated.
 //
 // This Preliminary version will work only FOR ITS !!!!!
 // other detectors (TOF,TRD, etc. have to develop their specific codes)
 //
 //  Input: none
 //  Return: kTRUE if all operations have been done properly, kFALSE otherwise
 //

  fSPDTrackleter = NULL;
  TString detStr = fLoadCDB;
  if (IsSelected(fgkDetectorName[0], detStr)) {
    IlcReconstructor* itsReconstructor = GetReconstructor(0);
    if (itsReconstructor) {
      fSPDTrackleter = itsReconstructor->CreateTrackleter(); // this is NULL unless required in RecoParam
    }
    if (fSPDTrackleter) {
      IlcInfo("Trackleter for SPD has been created");
    }
  }
 return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcReconstruction::InitIlcEVE()
{
  // This method should be called only in case 
  // IlcReconstruction is run
  // within the ilceve environment.
  // It will initialize IlcEVE in a way
  // so that it can visualize event processed
  // by IlcReconstruction.
  // The return flag shows whenever the
  // IlcEVE initialization was successful or not.

  TString macroStr(getenv("ILCEVE_ONLINE_MACRO"));

  if (macroStr.IsNull())
    macroStr.Form("%s/EVE/macros/ilceve_online.C",gSystem->ExpandPathName("$ILC_ROOT"));

  IlcInfo(Form("Loading IlcEVE macro: %s",macroStr.Data()));

  if (gROOT->LoadMacro(macroStr.Data()) != 0) return kFALSE;

  gROOT->ProcessLine("if (!IlcEveEventManager::GetMaster()){new IlcEveEventManager();IlcEveEventManager::GetMaster()->AddNewEventCommand(\"ilceve_online_on_new_event()\");gEve->AddEvent(IlcEveEventManager::GetMaster());};");
  gROOT->ProcessLine("ilceve_online_init()");

  return kTRUE;
}
  
//_____________________________________________________________________________
void IlcReconstruction::RunIlcEVE()
{
  // Runs IlcEVE visualisation of
  // the current event.
  // Should be executed only after
  // successful initialization of IlcEVE.

  IlcInfo("Running IlcEVE...");
  gROOT->ProcessLine(Form("IlcEveEventManager::GetMaster()->SetEvent((IlcRunLoader*)%p,(IlcRawReader*)%p,(IlcESDEvent*)%p,(IlcESDfriend*)%p);",fRunLoader,fRawReader,fesd,fesdf));
  gSystem->Run();
}

//_____________________________________________________________________________
Bool_t IlcReconstruction::SetRunQA(TString detAndAction) 
{
	// Allows to run QA for a selected set of detectors
	// and a selected set of tasks among RAWS, DIGITSR, RECPOINTS and ESDS
	// all selected detectors run the same selected tasks
	
	if (!detAndAction.Contains(":")) {
		IlcError( Form("%s is a wrong syntax, use \"DetectorList:ActionList\" \n", detAndAction.Data()) ) ;
		fRunQA = kFALSE ;
		return kFALSE ; 		
	}
	Int_t colon = detAndAction.Index(":") ; 
	fQADetectors = detAndAction(0, colon) ; 
	fQATasks   = detAndAction(colon+1, detAndAction.Sizeof() ) ; 
	if (fQATasks.Contains("ALL") ) {
		fQATasks = Form("%d %d %d %d", IlcQAv1::kRAWS, IlcQAv1::kDIGITSR, IlcQAv1::kRECPOINTS, IlcQAv1::kESDS) ; 
	} else {
		fQATasks.ToUpper() ; 
		TString tempo("") ; 
		if ( fQATasks.Contains("RAW") ) 
			tempo = Form("%d ", IlcQAv1::kRAWS) ; 
		if ( fQATasks.Contains("DIGIT") ) 
			tempo += Form("%d ", IlcQAv1::kDIGITSR) ; 
		if ( fQATasks.Contains("RECPOINT") ) 
			tempo += Form("%d ", IlcQAv1::kRECPOINTS) ; 
		if ( fQATasks.Contains("ESD") ) 
			tempo += Form("%d ", IlcQAv1::kESDS) ; 
		fQATasks = tempo ; 
		if (fQATasks.IsNull()) {
			IlcInfo("No QA requested\n")  ;
			fRunQA = kFALSE ;
			return kTRUE ; 
		}
	}	
	TString tempo(fQATasks) ; 
	tempo.ReplaceAll(Form("%d", IlcQAv1::kRAWS), IlcQAv1::GetTaskName(IlcQAv1::kRAWS)) 	;
	tempo.ReplaceAll(Form("%d", IlcQAv1::kDIGITSR), IlcQAv1::GetTaskName(IlcQAv1::kDIGITSR)) ;	
	tempo.ReplaceAll(Form("%d", IlcQAv1::kRECPOINTS), IlcQAv1::GetTaskName(IlcQAv1::kRECPOINTS)) ;	
	tempo.ReplaceAll(Form("%d", IlcQAv1::kESDS), IlcQAv1::GetTaskName(IlcQAv1::kESDS)) ; 	
	IlcInfo( Form("QA will be done on \"%s\" for \"%s\"\n", fQADetectors.Data(), tempo.Data()) ) ;  
	fRunQA = kTRUE ;
	return kTRUE; 
} 

//_____________________________________________________________________________
Bool_t IlcReconstruction::InitRecoParams() 
{
  // The method accesses OCDB and retrieves all
  // the available reco-param objects from there.

  Bool_t isOK = kTRUE;

  if (fRecoParam.GetDetRecoParamArray(kNDetectors)) {
    IlcInfo("Using custom GRP reconstruction parameters");
  }
  else {
    IlcInfo("Loading GRP reconstruction parameter objects");

    IlcCDBPath path("GRP","Calib","RecoParam");
    IlcCDBEntry *entry=IlcCDBManager::Instance()->Get(path.GetPath());
    if(!entry){ 
      IlcWarning("Couldn't find GRP RecoParam entry in OCDB");
      isOK = kFALSE;
    }
    else {
      TObject *recoParamObj = entry->GetObject();
      if (dynamic_cast<TObjArray*>(recoParamObj)) {
	// GRP has a normal TobjArray of IlcDetectorRecoParam objects
	// Registering them in IlcRecoParam
	fRecoParam.AddDetRecoParamArray(kNDetectors,dynamic_cast<TObjArray*>(recoParamObj));
      }
      else if (dynamic_cast<IlcDetectorRecoParam*>(recoParamObj)) {
	// GRP has only onse set of reco parameters
	// Registering it in IlcRecoParam
	IlcInfo("Single set of GRP reconstruction parameters found");
	dynamic_cast<IlcDetectorRecoParam*>(recoParamObj)->SetAsDefault();
	fRecoParam.AddDetRecoParam(kNDetectors,dynamic_cast<IlcDetectorRecoParam*>(recoParamObj));
      }
      else {
	IlcError("No valid GRP RecoParam object found in the OCDB");
	isOK = kFALSE;
      }
      entry->SetOwner(0);
    }
  }

  TString detStr = fLoadCDB;
  for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {

    if (!IsSelected(fgkDetectorName[iDet], detStr)) continue;

    if (fRecoParam.GetDetRecoParamArray(iDet)) {
      IlcInfo(Form("Using custom reconstruction parameters for detector %s",fgkDetectorName[iDet]));
      continue;
    }

    IlcInfo(Form("Loading reconstruction parameter objects for detector %s",fgkDetectorName[iDet]));
  
    IlcCDBPath path(fgkDetectorName[iDet],"Calib","RecoParam");
    IlcCDBEntry *entry=IlcCDBManager::Instance()->Get(path.GetPath());
    if(!entry){
      IlcWarning(Form("Couldn't find RecoParam entry in OCDB for detector %s",fgkDetectorName[iDet]));
      isOK = kFALSE;
    }
    else {
      TObject *recoParamObj = entry->GetObject();
      if (dynamic_cast<TObjArray*>(recoParamObj)) {
	// The detector has a normal TobjArray of IlcDetectorRecoParam objects
	// Registering them in IlcRecoParam
	fRecoParam.AddDetRecoParamArray(iDet,dynamic_cast<TObjArray*>(recoParamObj));
      }
      else if (dynamic_cast<IlcDetectorRecoParam*>(recoParamObj)) {
	// The detector has only onse set of reco parameters
	// Registering it in IlcRecoParam
	IlcInfo(Form("Single set of reconstruction parameters found for detector %s",fgkDetectorName[iDet]));
	dynamic_cast<IlcDetectorRecoParam*>(recoParamObj)->SetAsDefault();
	fRecoParam.AddDetRecoParam(iDet,dynamic_cast<IlcDetectorRecoParam*>(recoParamObj));
      }
      else {
	IlcError(Form("No valid RecoParam object found in the OCDB for detector %s",fgkDetectorName[iDet]));
	isOK = kFALSE;
      }
      entry->SetOwner(0);
      //      FIX ME: We have to disable the unloading of reco-param CDB
      //      entries because QA framework is using them. Has to be fix in
      //      a way that the QA takes the objects already constructed in
      //      this method.
      //      IlcCDBManager::Instance()->UnloadFromCache(path.GetPath());
    }
  }

  if (IlcDebugLevel() > 0) fRecoParam.Print();

  return isOK;
}

//_____________________________________________________________________________
Bool_t IlcReconstruction::GetEventInfo() 
{
  // Fill the event info object
  // ...
  IlcCodeTimerAuto("",0)

  IlcCentralTrigger *aCTP = NULL;
  if (fRawReader) {
    fEventInfo.SetEventType(fRawReader->GetType());

    ULong64_t mask = fRawReader->GetClassMask();
    fEventInfo.SetTriggerMask(mask);
    UInt_t clmask = fRawReader->GetDetectorPattern()[0];
    fEventInfo.SetTriggerCluster(IlcDAQ::ListOfTriggeredDetectors(clmask));

    aCTP = new IlcCentralTrigger();
    TString configstr("");
    if (!aCTP->LoadConfiguration(configstr)) { // Load CTP config from OCDB
      IlcError("No trigger configuration found in OCDB! The trigger configuration information will not be used!");
      delete aCTP;
      return kFALSE;
    }
    aCTP->SetClassMask(mask);
    aCTP->SetClusterMask(clmask);

    if (fRunLoader) {
      IlcCentralTrigger* rlCTP = fRunLoader->GetTrigger();
      if (rlCTP) {
        rlCTP->SetClassMask(mask);
        rlCTP->SetClusterMask(clmask);
      }
    }
  }
  else {
    fEventInfo.SetEventType(IlcRawEventHeaderBase::kPhysicsEvent);

    if (fRunLoader && (!fRunLoader->LoadTrigger())) {
      aCTP = fRunLoader->GetTrigger();
      fEventInfo.SetTriggerMask(aCTP->GetClassMask());
      // get inputs from actp - just get
      IlcESDHeader* esdheader = fesd->GetHeader();
      esdheader->SetL0TriggerInputs(aCTP->GetL0TriggerInputs());
      esdheader->SetL1TriggerInputs(aCTP->GetL1TriggerInputs());
      esdheader->SetL2TriggerInputs(aCTP->GetL2TriggerInputs());
      fEventInfo.SetTriggerCluster(IlcDAQ::ListOfTriggeredDetectors(aCTP->GetClusterMask()));
    }
    else {
      IlcWarning("No trigger can be loaded! The trigger information will not be used!");
      return kFALSE;
    }
  }

  IlcTriggerConfiguration *config = aCTP->GetConfiguration();
  if (!config) {
    IlcError("No trigger configuration has been found! The trigger configuration information will not be used!");
    if (fRawReader) delete aCTP;
    return kFALSE;
  }

  // Load trigger aliases and declare the trigger classes included in aliases
  IlcCDBEntry * entry = IlcCDBManager::Instance()->Get("GRP/CTP/Aliases");
  if (entry) {
    THashList * lst = dynamic_cast<THashList*>(entry->GetObject());
    if (lst) {
      lst->Sort(kSortDescending); // to avoid problems with substrungs
      if (fRawReader) fRawReader->LoadTriggerAlias(lst);
      // Now declare all the triggers present in the aliases
      TIter iter(lst);
      TNamed *nmd = 0;
      while((nmd = dynamic_cast<TNamed*>(iter.Next()))){
	fDeclTriggerClasses += " ";
	fDeclTriggerClasses += nmd->GetName();
      }
    }
    else {
      IlcError("Cannot cast the object with trigger aliases to THashList!");
    }
  }
  else {
    IlcError("No OCDB ebtry for the trigger aliases!");
  }
  // Load trigger classes for this run
  UChar_t clustmask = 0;
  TString trclasses;
  ULong64_t trmask = fEventInfo.GetTriggerMask();
  const TObjArray& classesArray = config->GetClasses();
  Int_t nclasses = classesArray.GetEntriesFast();
  for( Int_t iclass=0; iclass < nclasses; iclass++ ) {
    IlcTriggerClass* trclass = (IlcTriggerClass*)classesArray.At(iclass);
    if (trclass && trclass->GetMask()>0) {
      Int_t trindex = TMath::Nint(TMath::Log2(trclass->GetMask()));
      if (fesd) fesd->SetTriggerClass(trclass->GetName(),trindex);
      if (fRawReader) fRawReader->LoadTriggerClass(trclass->GetName(),trindex);
      if (trmask & (1ull << trindex)) {
	trclasses += " ";
	trclasses += trclass->GetName();
	trclasses += " ";
	clustmask |= trclass->GetCluster()->GetClusterMask();
      }
    }
  }
  fEventInfo.SetTriggerClasses(trclasses);
  // Now put the declared trigger classes (not present in the run)
  // to 0/false in the event selection
  if (!fDeclTriggerClasses.IsNull()) {
    TObjArray *tokens = fDeclTriggerClasses.Tokenize(" ");
    Int_t ntokens = tokens->GetEntriesFast();
    for (Int_t itoken = 0; itoken < ntokens; ++itoken) {
      if (fRawReader) fRawReader->LoadTriggerClass((((TObjString*)tokens->At(itoken))->String()).Data(),-1);
    }
    delete tokens;
  }

  // Write names of active trigger inputs in ESD Header
  const TObjArray& inputsArray = config->GetInputs(); 
  Int_t ninputs = inputsArray.GetEntriesFast();
  for( Int_t iinput=0; iinput < ninputs; iinput++ ) {
    IlcTriggerInput* trginput = (IlcTriggerInput*)inputsArray.At(iinput);
    if (trginput && trginput->GetMask()>0) {
      Int_t inputIndex = (Int_t)TMath::Nint(TMath::Log2(trginput->GetMask()));
      IlcESDHeader* headeresd = 0x0;
      if (fesd) headeresd = fesd->GetHeader();
      if (headeresd) {
        Int_t trglevel = (Int_t)trginput->GetLevel();
        if (trglevel == 0) headeresd->SetActiveTriggerInputs(trginput->GetInputName(), inputIndex);
        if (trglevel == 1) headeresd->SetActiveTriggerInputs(trginput->GetInputName(), inputIndex+24);
        if (trglevel == 2) headeresd->SetActiveTriggerInputs(trginput->GetInputName(), inputIndex+48);
      }
    }
  }

  // Set the information in ESD
  if (fesd) {
    fesd->SetTriggerMask(trmask);
    fesd->SetTriggerCluster(clustmask);
  }

  if (!aCTP->CheckTriggeredDetectors()) {
    if (fRawReader) delete aCTP;
    return kFALSE;
  }    

  if (fRawReader) delete aCTP;

  // We have to fill also the HLT decision here!!
  // ...

  return kTRUE;
}

const char *IlcReconstruction::MatchDetectorList(const char *detectorList, UInt_t detectorMask)
{
  // Match the detector list found in the rec.C or the default 'ALL'
  // to the list found in the GRP (stored there by the shuttle PP which
  // gets the information from ECS)
  static TString resultList;
  TString detList = detectorList;

  resultList = "";

  for(Int_t iDet = 0; iDet < (IlcDAQ::kNDetectors-1); iDet++) {
    if ((detectorMask >> iDet) & 0x1) {
      TString det = IlcDAQ::OfflineModuleName(iDet);
      if ((detList.CompareTo("ALL") == 0) ||
	  ((detList.BeginsWith("ALL ") ||
	    detList.EndsWith(" ALL") ||
	    detList.Contains(" ALL ")) &&
	   !(detList.BeginsWith("-"+det+" ") ||
	     detList.EndsWith(" -"+det) ||
	     detList.Contains(" -"+det+" "))) ||
	  (detList.CompareTo(det) == 0) ||
	  detList.BeginsWith(det+" ") ||
	  detList.EndsWith(" "+det) ||
	  detList.Contains( " "+det+" " )) {
	if (!resultList.EndsWith(det + " ")) {
	  resultList += det;
	  resultList += " ";
	}
      }	       
    }
  }

  // HLT
  if ((detectorMask >> IlcDAQ::kHLTId) & 0x1) {
    TString hltDet = IlcDAQ::OfflineModuleName(IlcDAQ::kNDetectors-1);
    if ((detList.CompareTo("ALL") == 0) ||
	((detList.BeginsWith("ALL ") ||
	  detList.EndsWith(" ALL") ||
	  detList.Contains(" ALL ")) &&
	 !(detList.BeginsWith("-"+hltDet+" ") ||
	   detList.EndsWith(" -"+hltDet) ||
	   detList.Contains(" -"+hltDet+" "))) ||
	(detList.CompareTo(hltDet) == 0) ||
	detList.BeginsWith(hltDet+" ") ||
	detList.EndsWith(" "+hltDet) ||
	detList.Contains( " "+hltDet+" " )) {
      resultList += hltDet;
    }
  }

  return resultList.Data();

}

//______________________________________________________________________________
void IlcReconstruction::Abort(const char *method, EAbort what)
{
  // Abort processing. If what = kAbortProcess, the Process() loop will be
  // aborted. If what = kAbortFile, the current file in a chain will be
  // aborted and the processing will continue with the next file, if there
  // is no next file then Process() will be aborted. Abort() can also  be
  // called from Begin(), SlaveBegin(), Init() and Notify(). After abort
  // the SlaveTerminate() and Terminate() are always called. The abort flag
  // can be checked in these methods using GetAbort().
  //
  // The method is overwritten in IlcReconstruction for better handling of
  // reco specific errors 

  if (!fStopOnError) return;

  CleanUp();

  TString whyMess = method;
  whyMess += " failed! Aborting...";

  IlcError(whyMess.Data());

  fAbort = what;
  TString mess = "Abort";
  if (fAbort == kAbortProcess)
    mess = "AbortProcess";
  else if (fAbort == kAbortFile)
    mess = "AbortFile";

  Info(mess.Data(), "%s", whyMess.Data());
}

//______________________________________________________________________________
Bool_t IlcReconstruction::ProcessEvent(void* event)
{
  // Method that is used in case the event loop
  // is steered from outside, for example by AMORE
  // 'event' is a pointer to the DATE event in the memory

  if (fRawReader) delete fRawReader;
  fRawReader = new IlcRawReaderDate(event);
  fStatus = ProcessEvent(fRunLoader->GetNumberOfEvents());  
  delete fRawReader;
  fRawReader = NULL;

  return fStatus;
}

//______________________________________________________________________________
Bool_t IlcReconstruction::ParseOutput()
{
  // The method parses the output file
  // location string in order to steer
  // properly the selector

  TPMERegexp re1("(\\w+\\.zip#\\w+\\.root):([,*\\w+\\.root,*]+)@dataset://(\\w++)");
  TPMERegexp re2("(\\w+\\.root)?@?dataset://(\\w++)");

  if (re1.Match(fESDOutput) == 4) {
    // root archive with output files stored and regustered
    // in proof dataset
    gProof->AddInput(new TNamed("PROOF_OUTPUTFILE",re1[1].Data()));
    gProof->AddInput(new TNamed("PROOF_OUTPUTFILE_LOCATION",re1[3].Data()));
    gProof->AddInput(new TNamed("PROOF_OUTPUTFILE_DATASET",""));
    gProof->AddInput(new TNamed("PROOF_OUTPUTFILE_ARCHIVE",re1[2].Data()));
    IlcInfo(Form("%s files will be stored within %s in dataset %s",
		 re1[2].Data(),
		 re1[1].Data(),
		 re1[3].Data()));
  }
  else if (re2.Match(fESDOutput) == 3) {
    // output file stored and registered
    // in proof dataset
    gProof->AddInput(new TNamed("PROOF_OUTPUTFILE",(re2[1].IsNull()) ? "IlcESDs.root" : re2[1].Data()));
    gProof->AddInput(new TNamed("PROOF_OUTPUTFILE_LOCATION",re2[2].Data()));
    gProof->AddInput(new TNamed("PROOF_OUTPUTFILE_DATASET",""));
    IlcInfo(Form("%s will be stored in dataset %s",
		 (re2[1].IsNull()) ? "IlcESDs.root" : re2[1].Data(),
		 re2[2].Data()));
  }
  else {
    if (fESDOutput.IsNull()) {
      // Output location not given.
      // Assuming xrootd has been already started and
      // the output file has to be sent back
      // to the client machine
      TString esdUrl(Form("root://%s/%s/",
			  TUrl(gSystem->HostName()).GetHostFQDN(),
			  gSystem->pwd()));
      gProof->AddInput(new TNamed("PROOF_OUTPUTFILE","IlcESDs.root"));
      gProof->AddInput(new TNamed("PROOF_OUTPUTFILE_LOCATION",esdUrl.Data()));
      IlcInfo(Form("IlcESDs.root will be stored in %s",
		   esdUrl.Data()));
    }
    else {
      // User specified an output location.
      // Ones has just to parse it here
      TUrl outputUrl(fESDOutput.Data());
      TString outputFile(gSystem->BaseName(outputUrl.GetFile()));
      gProof->AddInput(new TNamed("PROOF_OUTPUTFILE",outputFile.IsNull() ? "IlcESDs.root" : outputFile.Data()));
      TString outputLocation(outputUrl.GetUrl());
      outputLocation.ReplaceAll(outputFile.Data(),"");
      gProof->AddInput(new TNamed("PROOF_OUTPUTFILE_LOCATION",outputLocation.Data()));
      IlcInfo(Form("%s will be stored in %s",
		   outputFile.IsNull() ? "IlcESDs.root" : outputFile.Data(),
		   outputLocation.Data()));
    }
  }

  return kTRUE;
}

//______________________________________________________________________________
Bool_t IlcReconstruction::IsHighPt() const {
  // Selection of events containing "high" pT tracks
  // If at least one track is found within 1.5 and 100 GeV (pT)
  // that was reconstructed by both ITS and TPC, the event is accepted

  // Track cuts
  const Double_t pTmin = 1.5;
  const Double_t pTmax = 100;
  ULong_t mask = 0;
  mask |= (IlcESDtrack::kITSrefit);
  mask |= (IlcESDtrack::kTPCrefit);
  const Double_t pTminCosmic = 5.;
  const Double_t pTmaxCosmic = 100;
  ULong_t maskCosmic = 0;
  Int_t cosmicCount=0;
  maskCosmic |= (IlcESDtrack::kTPCrefit);

  Bool_t isOK = kFALSE;

  if (fesd && fesd->GetEventType()==IlcRawEventHeaderBase::kPhysicsEvent) {
    // Check if this ia a physics event (code 7)
    Int_t ntrk = fesd->GetNumberOfTracks();
    for (Int_t itrk=0; itrk<ntrk; ++itrk) {
	  
      IlcESDtrack * trk = fesd->GetTrack(itrk);
      if (trk 
	  && trk->Pt() > pTmin 
	  && trk->Pt() < pTmax
	  && (trk->GetStatus() & mask) == mask ) {
	
	isOK = kTRUE;
	break;
      }
      if (trk 
	  && trk->GetInnerParam()
	  && trk->GetInnerParam()->Pt() > pTminCosmic 
	  && trk->GetInnerParam()->Pt() < pTmaxCosmic
	  && (trk->GetStatus() & maskCosmic) == maskCosmic ) {
	
	cosmicCount++;
	break;
      }
    }
    if (cosmicCount>1) isOK=kTRUE;
  }
  return isOK;
}

//______________________________________________________________________________
Bool_t IlcReconstruction::IsCosmicOrCalibSpecie() const {
  // Select cosmic or calibration events

  Bool_t isOK = kFALSE;

  if (fesd && fesd->GetEventType()==IlcRawEventHeaderBase::kPhysicsEvent) {
      // Check if this ia a physics event (code 7)
      
      UInt_t specie = fesd->GetEventSpecie();
      if (specie==IlcRecoParam::kCosmic || specie==IlcRecoParam::kCalib) {
	isOK = kTRUE;
      }
  }
  return isOK;
}

//______________________________________________________________________________
void IlcReconstruction::WriteESDfriend() {
  // Fill the ESD friend in the tree. The required fraction of ESD friends is stored
  // in fFractionFriends. We select events where we store the ESD friends according
  // to the following algorithm:
  // 1. Store all Cosmic or Calibration events within the required fraction
  // 2. Sample "high Pt" events within the remaining fraction after step 1.
  // 3. Sample randomly events if we still have remaining slot

  fNall++;
  Bool_t isSelected = kFALSE;
  //
  // Store all friends for B field OFF 
  if (TMath::Abs(IlcTrackerBase::GetBz())<0.5) isSelected=kTRUE;

  if (IsCosmicOrCalibSpecie()) { // Selection of calib or cosmic events
    fNspecie++;

    isSelected = kTRUE;
    fSspecie++;
  }
  
  Double_t remainingFraction = fFractionFriends;
  remainingFraction -= ((Double_t)(fSspecie)/(Double_t)(fNall));
  
  if (IsHighPt())  { // Selection of "high Pt" events
    fNhighPt++;
    Double_t curentHighPtFraction = ((Double_t)(fNhighPt+1))/((Double_t)(fNall+1));
    // "Bayesian" estimate supposing that without events all the events are of the required type
    
    if (!isSelected) {
      Double_t rnd = gRandom->Rndm()*curentHighPtFraction;
      if (rnd<remainingFraction) {
	isSelected = kTRUE;
	fShighPt++;
      }
    }
  }
  remainingFraction -= ((Double_t)(fShighPt)/(Double_t)(fNall));
  
  // Random selection to fill the remaining fraction (if any)
  if (!isSelected) {
    Double_t rnd = gRandom->Rndm();
    if (rnd<remainingFraction) {	
      isSelected = kTRUE;
    }
  }
  
  if (!isSelected) {
    fesdf->~IlcESDfriend();
    new (fesdf) IlcESDfriend(); // Reset...
    fesdf->SetSkipBit(kTRUE);
  }
  //
  ftreeF->Fill();
}

//_________________________________________________________________
void IlcReconstruction::DeleteDigits(const TString& detectors)
{
  // delete requested digit files produced at current event
  static int iEvent = 0;
  if (detectors.IsNull()) return;
  TString detStr = detectors;
  IlcInfo(Form("Deleting Digits: %s",detectors.Data()));

  for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
    if(!IsSelected(fgkDetectorName[iDet], detStr)) continue;
    unlink(Form("%s.Digits.root",fgkDetectorName[iDet]));
  }
  IlcSysInfo::AddStamp(Form("DelDigits_%d",iEvent), 0,0,iEvent);
  iEvent++;
}

//_________________________________________________________________
void IlcReconstruction::DeleteRecPoints(const TString& detectors)
{
  // delete requested recpoint files produced at current event
  static int iEvent = 0;
  if (detectors.IsNull()) return;
  TString detStr = detectors;
  IlcInfo(Form("Deleting Recpoints: %s",detectors.Data()));
  //
  for (Int_t iDet = 0; iDet < kNDetectors; iDet++) {
    if(!IsSelected(fgkDetectorName[iDet], detStr)) continue;
    unlink(Form("%s.RecPoints.root",fgkDetectorName[iDet]));
  }
  IlcSysInfo::AddStamp(Form("DelRecPoints_%d",iEvent), 0,0,iEvent);
  iEvent++;
}

//_________________________________________________________________
void IlcReconstruction::SetStopOnResourcesExcess(Int_t vRSS,Int_t vVMEM)
{
  // require checking the resources left and stopping on excess
  // if 0  : no check is done
  // if >0 : stop reconstruction if exceeds this value
  // if <0 : use as margin to system limits
  //
  const int kKB2MB = 1024;
  const int kInfMem = 9999999;
  //
  struct rlimit r;
  int pgSize = getpagesize();
  //
  if (vRSS>0) {
    fMaxRSS = vRSS;
    IlcInfo(Form("Setting max. RSS usage to user value %d MB",fMaxRSS));
  }
  else if (vRSS<0) {
    getrlimit(RLIMIT_RSS,&r);
    fMaxRSS = r.rlim_max==RLIM_INFINITY ? kInfMem : int(r.rlim_max*pgSize/kKB2MB/kKB2MB) + vRSS;
    IlcInfo(Form("Setting max. RSS usage to system hard limit %d%s MB (%d margin)",fMaxRSS,r.rlim_max==RLIM_INFINITY ? "(inf)":"",-vRSS));
  }
  else {IlcInfo("No check on RSS memory usage will be applied");}
  //
  if (vVMEM>0) {
    fMaxVMEM = vVMEM;
    IlcInfo(Form("Setting max. VMEM usage to user value %d MB",fMaxVMEM));
  }
  else if (vVMEM<0) {
    getrlimit(RLIMIT_AS,&r);
    fMaxVMEM = r.rlim_max==RLIM_INFINITY ? kInfMem : int(r.rlim_max*pgSize/kKB2MB/kKB2MB) + vVMEM;
    IlcInfo(Form("Setting max. VMEM usage to system hard limit %d%s MB (%d margin)",fMaxVMEM,r.rlim_max==RLIM_INFINITY ? "(inf)":"",-vVMEM));
  }
  else {IlcInfo("No check on RSS memory usage will be applied");}
  //  
}

//_________________________________________________________________
Bool_t IlcReconstruction::HasEnoughResources(int ev)
{
  // check if process consumed more than allowed resources
  const int kKB2MB = 1024;
  Bool_t res = kTRUE;
  if (!fMaxRSS && !fMaxVMEM) return res;
  //
  ProcInfo_t procInfo;
  gSystem->GetProcInfo(&procInfo);
  if (procInfo.fMemResident/kKB2MB > fMaxRSS)  res = kFALSE;
  if (procInfo.fMemVirtual/kKB2MB  > fMaxVMEM) res = kFALSE;  
  //
  if (!res) {
    IlcInfo(Form("Job exceeded allowed limits: RSS:%d (%d) VMEM:%d (%d), will stop",
		 int(procInfo.fMemResident/kKB2MB),fMaxRSS,
		 int(procInfo.fMemVirtual/kKB2MB) ,fMaxVMEM));
    //
    unlink(Form("%s",fgkStopEvFName));
    ofstream outfile(fgkStopEvFName);
    outfile << ev << endl;
    outfile.close();
    fStopped = kTRUE;
  }
  return res;
}
