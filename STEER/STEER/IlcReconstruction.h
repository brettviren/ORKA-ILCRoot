#ifndef ILCRECONSTRUCTION_H
#define ILCRECONSTRUCTION_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcReconstruction.h 58198 2012-08-15 15:12:52Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// class for running the reconstruction                                      //
// Clusters and tracks are created for all detectors and all events by       //
// typing:                                                                   //
//                                                                           //
//   IlcReconstruction rec;                                                  //
//   rec.Run();                                                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


#include <TSelector.h>
#include <TString.h>
#include <TObjArray.h>

class IlcReconstructor;
class IlcRunLoader;
class IlcRawReader;
class IlcLoader;
class IlcTracker;
class IlcMagF;
class IlcVertexer;
class IlcTrackleter;
class IlcESDVertex;
class IlcESDEvent;
class IlcESDfriend;
class IlcESDpid;
class IlcVertexerTracks;
class TFile;
class TTree;
class TList;
class IlcQAManager; 
class TMap;
class IlcRecoParam;
class IlcDetectorRecoParam;
class IlcRunInfo;
class IlcGRPObject;
class THashTable;
class IlcTriggerRunScalers;
class IlcCTPTimeParams;
class IlcAnalysisManager;
class IlcRecoInputHandler;

#include "IlcQAv1.h"
#include "IlcEventInfo.h"
#include "IlcRecoParam.h"

using std::ofstream;

class IlcReconstruction: public TSelector {
public:
  IlcReconstruction(const char* gIlcFilename = "gilc.root");
  virtual ~IlcReconstruction();

  void           SetGIlcFile(const char* fileName);
  void           SetInput(const char* input);
  void           SetOutput(const char* output);

  void           SetEquipmentIdMap(const char *mapFile) {fEquipIdMap = mapFile;};
  void           SetEventRange(Int_t firstEvent = 0, Int_t lastEvent = -1) 
    {fFirstEvent = firstEvent; fLastEvent = lastEvent;};
  void           SetNumberOfEventsPerFile(UInt_t nEvents)
    {fNumberOfEventsPerFile = nEvents;};
  void           SetFractionFriends(Double32_t frac = 0.04)
    {fFractionFriends = frac;};
  void           SetOption(const char* detector, const char* option);
  void           SetRecoParam(const char* detector, IlcDetectorRecoParam *par);

  void           SetRunLocalReconstruction(const char* detectors) {
    fRunLocalReconstruction = detectors;};
  void           SetRunTracking(const char* detectors) {
    fRunTracking = detectors;};
  void           SetFillESD(const char* detectors) {fFillESD = detectors;};
  void           SetRunReconstruction(const char* detectors) {
    SetRunLocalReconstruction(detectors); 
    SetRunTracking(detectors);
    SetFillESD(detectors);};
  void           SetDeleteRecPoints(const char* dets) {fDeleteRecPoints = dets;}
  void           SetDeleteDigits(const char* dets)    {fDeleteDigits = dets;}
  void           SetUseTrackingErrorsForAlignment(const char* detectors) 
    {fUseTrackingErrorsForAlignment = detectors;};
  void           SetLoadAlignFromCDB(Bool_t load)  {fLoadAlignFromCDB = load;};
  void           SetLoadAlignData(const char* detectors) 
    {fLoadAlignData = detectors;};

  //*** Global reconstruction flag setters
  void SetRunMultFinder(Bool_t flag=kTRUE) {fRunMultFinder=flag;};
  void SetRunVertexFinder(Bool_t flag=kTRUE) {fRunVertexFinder=flag;};
  void SetRunVertexFinderTracks(Bool_t flag=kTRUE) {fRunVertexFinderTracks=flag;};
  void SetRunHLTTracking(Bool_t flag=kTRUE) {fRunHLTTracking=flag;};
  void SetRunV0Finder(Bool_t flag=kTRUE) {fRunV0Finder=flag;};
  void SetRunCascadeFinder(Bool_t flag=kTRUE) {fRunCascadeFinder=flag;};
  void SetStopOnError(Bool_t flag=kTRUE) {fStopOnError=flag;}
  void SetWriteAlignmentData(Bool_t flag=kTRUE){fWriteAlignmentData=flag;}
  void SetWriteESDfriend(Bool_t flag=kTRUE){fWriteESDfriend=flag;}
  void SetFillTriggerESD(Bool_t flag=kTRUE){fFillTriggerESD=flag;}
  void SetDiamondProfileSPD(IlcESDVertex *dp) {fDiamondProfileSPD=dp;}
  void SetDiamondProfile(IlcESDVertex *dp) {fDiamondProfile=dp;}
  void SetDiamondProfileTPC(IlcESDVertex *dp) {fDiamondProfileTPC=dp;}
		   
  void SetCleanESD(Bool_t flag=kTRUE){fCleanESD=flag;}
  void SetUseHLTData(const char* detectors){fUseHLTData=detectors;}
  void SetV0DCAmax(Float_t d) {fV0DCAmax=d;}
  void SetV0CsPmin(Float_t d) {fV0CsPmin=d;}
  void SetDmax(Float_t d) {fDmax=d;}
  void SetZmax(Float_t z) {fZmax=z;}
  Float_t GetV0DCAmax() const {return fV0DCAmax;}
  Float_t GetV0CsPmin() const {return fV0CsPmin;}
  Float_t GetDmax() const {return fDmax;}
  Float_t GetZmax() const {return fZmax;}
  //
  Bool_t  IsRunMultFinder()   const {return fRunMultFinder;}
  
  // CDB storage activation
  void SetDefaultStorage(const char* uri);
  void SetSpecificStorage(const char* calibType, const char* uri);
  void SetCDBSnapshotMode(const char* snapshotFileName);

  Bool_t MisalignGeometry(const TString& detectors);

  void           SetAlignObjArray(TObjArray *array)
                   {fIlcgnObjArray = array;
		   fLoadAlignFromCDB = kFALSE;}

  virtual Int_t  Version() const {return 2;}
  virtual void   Begin(TTree*);
  virtual void   SlaveBegin(TTree*);
  virtual void   Init(TTree *tree);
  virtual Bool_t Process(Long64_t entry);
  virtual Bool_t ProcessEvent(Int_t iEvent);
  virtual void   SlaveTerminate();
  virtual void   Terminate();
  virtual Bool_t Run(const char* input = NULL);
  void           Abort(const char *method, EAbort what);
  virtual void	 SetOption(const char* option) {
    TSelector::SetOption(option);
  }

  // Trackers
  IlcTracker* GetTracker(Int_t idx) const { return fTracker[idx]; }
  Bool_t      CreateTrackers(const TString& detectors);
  void        ImportRunLoader(IlcRunLoader* rl) { fRunLoader = rl; }

  // Quality Assurance 
  void    SetQACycles(IlcQAv1::DETECTORINDEX_t det, Int_t cycles) { fQACycles[det] = cycles ; }
  void    SetQAWriteExpert(IlcQAv1::DETECTORINDEX_t det) { fQAWriteExpert[det] = kTRUE ; }
  Bool_t  SetRunQA(TString detAndAction="ALL:ALL") ; 
  void    SetRunGlobalQA(Bool_t flag=kTRUE){fRunGlobalQA = flag;}
  void    SetQARefDefaultStorage(const char* uri);
  void    InitQA();
  void    MergeQA(const char *fileName = NULL); 
  void    SetWriteQAExpert() { fWriteQAExpertData = kTRUE ; }
  Bool_t  IsRunQA() const { return fRunQA ; } 
  Bool_t  IsRunGlobalQA() const { return fRunGlobalQA ; }
  Bool_t  IsInTasks(IlcQAv1::TASKINDEX_t tsk) { return fQATasks.Contains(Form("%d", tsk)) ; }
  
  // Plane Efficiency Evaluation
  void    SetRunPlaneEff(Bool_t flag=kFALSE)  {fRunPlaneEff = flag;}

  enum {
// #ifdef MFT_UPGRADE 
//     kNDetectors = 16   // number of detectors
// #else
//     kNDetectors = 15   // number of detectors
// #endif 
    kNDetectors = 2   // number of detectors    // AU
  };
  static Int_t   GetDetIndex(const char * detector);

  // Upgrade
 void SetUpgradeModule(const char* detectors)  {fUpgradeModule = detectors; MatchUpgradeDetector() ; }
 void MatchUpgradeDetector();

  // Connection to analysis manager. 
  // If a macro is set it should contain a function returning an analysis 
  // manager object. The macro is ignored in case the analysis manager is 
  // set via SetAnalysisManager
  void        SetAnalysisMacro(const char *macropath) {fAnalysisMacro = macropath;}
  const char *GetAnalysisMacro() const {return fAnalysisMacro.Data();}
// // // //   void        SetAnalysisManager(IlcAnalysisManager *mgr) {fAnalysis = mgr;}
// // // //   IlcAnalysisManager *GetAnalysisManager() const {return fAnalysis;}

  // A method to declare defined trigger classes even if they are not
  // present in the run, needed for proper event selection based on trigger
  void        DeclareTriggerClasses(const char *trClasses) {fDeclTriggerClasses = trClasses;}
  //
  //
  Bool_t       HasEnoughResources(int ev);
  void         SetStopOnResourcesExcess(int vRSS=3000,int vVMEM=4000);
  //
  //
protected:
  virtual Bool_t ProcessEvent(void* event);
  void           InitRun(const char* input);
  // Functions needed to select events for which we store the ESD friends
  Bool_t IsHighPt() const;
  Bool_t IsCosmicOrCalibSpecie() const;
  void WriteESDfriend();

private:
  IlcReconstruction(const IlcReconstruction& rec);
  IlcReconstruction& operator = (const IlcReconstruction& rec);

  void           InitRawReader(const char* input);
  void           InitCDB();
  Bool_t         InitGRP();
  void           SetCDBLock();
  Bool_t         SetRunNumberFromData();
  Bool_t         LoadCDB();
  Bool_t         LoadTriggerScalersCDB();
  Bool_t         LoadCTPTimeParamsCDB();
  Bool_t         ReadIntensityInfoCDB();
  Bool_t         RunLocalEventReconstruction(const TString& detectors);
  Bool_t         RunVertexFinder(IlcESDEvent*& esd);
  Bool_t         RunHLTTracking(IlcESDEvent*& esd);
  Bool_t         RunMuonTracking(IlcESDEvent*& esd);
  Bool_t         RunSPDTrackleting(IlcESDEvent*& esd);
  Bool_t         RunMultFinder(IlcESDEvent*& esd);
  Bool_t         RunTracking(IlcESDEvent*& esd, IlcESDpid &PID);
  Bool_t         CleanESD(IlcESDEvent *esd);
  Bool_t         FillESD(IlcESDEvent*& esd, const TString& detectors);
  Bool_t         FillTriggerESD(IlcESDEvent*& esd);
  Bool_t         FillTriggerScalers(IlcESDEvent*& esd);
  Bool_t         FillRawEventHeaderESD(IlcESDEvent*& esd);
  void           DeleteRecPoints(const TString& detectors);
  void           DeleteDigits(const TString& detectors);

  Bool_t         IsSelected(TString detName, TString& detectors) const;
  Bool_t         InitRunLoader();
  IlcReconstructor* GetReconstructor(Int_t iDet);
  IlcVertexer*   CreateVertexer();
  IlcTrackleter* CreateMultFinder();
  void           CleanUp();

  Bool_t         ParseOutput();

  //==========================================//
  void           WriteAlignmentData(IlcESDEvent* esd);

  void           FillRawDataErrorLog(Int_t iEvent, IlcESDEvent* esd);

  //Quality Assurance
  //  void                 CheckQA() ;

  // Plane Efficiency evaluation
  Bool_t  FinishPlaneEff(); //ultimate tasks related to Plane Eff. evaluation 
  Bool_t  InitPlaneEff();   // initialize what is needed for Plane Eff. evaluation

  Bool_t               InitIlcEVE();
  void                 RunIlcEVE();

  Bool_t         InitRecoParams(); // init the array with the reconstruciton parameters
  Bool_t         GetEventInfo();   // fill the event info inside the event loop

  const char    *MatchDetectorList(const char *detectorList, UInt_t detectorMask);

  //*** Global reconstruction flags *******************
  Bool_t         fRunVertexFinder;    // run the vertex finder
  Bool_t         fRunVertexFinderTracks;    // run the vertex finder with tracks
  Bool_t         fRunHLTTracking;     // run the HLT tracking
  Bool_t         fRunMuonTracking;    // run the HLT tracking
  Bool_t         fRunV0Finder;        // run the ESD V0 finder
  Bool_t         fRunCascadeFinder;   // run the ESD cascade finder
  Bool_t         fRunMultFinder;      // run the trackleter for ITS clusters
  Bool_t         fStopOnError;        // stop or continue on errors
  Bool_t         fWriteAlignmentData; // write track space-points flag
  Bool_t         fWriteESDfriend;     // write ESD friend flag
  Bool_t         fFillTriggerESD;     // fill trigger info into ESD

  //*** Clean ESD flag and parameters *******************
  Bool_t         fCleanESD;      // clean ESD flag
  Float_t        fV0DCAmax;      // max. allowed DCA between V0 daugthers 
  Float_t        fV0CsPmin;      // min. allowed cosine of V0 pointing angle 
  Float_t        fDmax;          // max. allowed transverse impact parameter 
  Float_t        fZmax;          // max. allowed longitudinal impact parameter 

  TString        fRunLocalReconstruction; // run the local reconstruction for these detectors
  TString        fRunTracking;        // run the tracking for these detectors
  TString        fFillESD;            // fill ESD for these detectors
  TString        fDeleteRecPoints;    // delete recpoints after each event
  TString        fDeleteDigits;       // delete digits after each event
  TString        fLoadCDB;            // prefetch CDB entries and init reco-params for these detectors
  TString        fUseTrackingErrorsForAlignment; // for these detectors
  TString        fGIlcFileName;     // name of the gilc file
  TString        fRawInput;           // name of input raw-data file or directory
  TString        fESDOutput;          //! name of output ESD file
  TString        fProofOutputFileName;//! name of output file in PROOF mode
  TString        fProofOutputLocation;//! output location in PROOF mode
  Bool_t         fProofOutputDataset; //! keep output file in PROOF mode in dataset or merge
  TString        fProofOutputArchive; //! list of file to be kept in output zip archive in PROOF mode
  TString        fEquipIdMap;         // name of file with equipment id map
  Int_t          fFirstEvent;         // index of first event to be reconstr.
  Int_t          fLastEvent;          // index of last event to be reconstr.
  UInt_t         fNumberOfEventsPerFile; // number of events per file in case of raw-data reconstruction
  Double32_t     fFractionFriends; // fraction of ESD friends to be stored
  TObjArray      fOptions;            // options for reconstructor objects
  Bool_t         fLoadAlignFromCDB;   // Load alignment data from CDB and apply it to geometry or not
  TString        fLoadAlignData;      // Load alignment data from CDB for these detectors
  TString        fUseHLTData;        // Detectors for which the HLT data is used as input
  IlcRunInfo*    fRunInfo;            // an object which contains essential global conditions information
  IlcEventInfo   fEventInfo;          // an object which contains essential event information
  IlcTriggerRunScalers* fRunScalers;  // an object which contains the trigger counters for xsection
  IlcCTPTimeParams* fCTPTimeParams;   // an object which contains the trigger delays
  IlcCTPTimeParams* fCTPTimeAlign;    // an object which contains the CTP (mis)alignment data
  IlcRunLoader*  fRunLoader;          //! current run loader object
  IlcRawReader*  fRawReader;          //! current raw data reader
  IlcRawReader*  fParentRawReader;    //! parent raw data reader in case of IlcRawReaderHLT

  static const char* fgkDetectorName[kNDetectors]; //! names of detectors
  IlcReconstructor*  fReconstructor[kNDetectors];  //! array of reconstructor objects
  IlcRecoParam   fRecoParam;                      // container for the reco-param objects for detectors
  IlcLoader*     fLoader[kNDetectors];   //! detector loaders
  IlcTracker*    fTracker[kNDetectors];  //! trackers
  IlcTracker*    fSPDTrackleter;           //! trackleter [for SPD Plane Efficiency purposes]
  IlcESDVertex*  fDiamondProfileSPD;       // (x,y) diamond profile from SPD for IlcITSVertexer3D(Z)
  IlcESDVertex*  fDiamondProfile;          // (x,y) diamond profile for IlcVertexerTracks (ITS+TPC)
  IlcESDVertex*  fDiamondProfileTPC;       // (x,y) diamond profile from TPC for IlcVertexerTracks
  THashTable*    fListOfCosmicTriggers;    // list of cosmic triggers as defined by trigger coordination

  IlcGRPObject*  fGRPData;              // Data from the GRP/GRP/Data CDB folder

  TObjArray* 	 fIlcgnObjArray;      //! array with the alignment objects to be applied to the geometry

  TString	 fCDBUri;	      //! Uri of the default CDB storage
  TString	 fQARefUri;	    //! Uri of the default QA reference storage
  TObjArray      fSpecCDBUri;         //! Array with detector specific CDB storages
  Bool_t 	 fInitCDBCalled;               //! flag to check if CDB storages are already initialized
  Bool_t         fCDBSnapshotMode;             //! flag true if we are setting the CDB Manager in snapshot mode
  Bool_t 	 fSetRunNumberFromDataCalled;  //! flag to check if run number is already loaded from run loader

  //Quality Assurance
  Int_t  fQACycles[     IlcQAv1::kNDET];  // # events over which QA data are accumulated
  Bool_t fQAWriteExpert[IlcQAv1::kNDET];  // Flag to save or not expert QA data
  TString               fQADetectors ;  // list of detectors to be QA'ed 	
  TString               fQATasks ;      // list of QA tasks to be performed	
  Bool_t                fRunQA ;        // Run QA flag
  Bool_t                fRunGlobalQA;   // Run global QA flag
  Bool_t                fSameQACycle;   //! open a new QA data file or not
  Bool_t                fInitQACalled;  //! control of QA intialisation
  Bool_t                fWriteQAExpertData ; //! decides wheter or not to write experts QA data; true by default

  // Plane Efficiency Evaluation
  Bool_t         fRunPlaneEff ;      // Evaluate Plane Efficiency

  // New members needed in order to split Run method
  // into InitRun,RunEvent,FinishRun methods
  IlcESDEvent*         fesd;        //! Pointer to the ESD event object
  IlcESDEvent*         fhltesd;     //! Pointer to the HLT ESD event object
  IlcESDfriend*        fesdf;       //! Pointer to the ESD friend object
  TFile*               ffile;       //! Pointer to the ESD file
  TFile*               ffileF;      //! Pointer to the ESD friend file
  TTree*               ftree;       //! Pointer to the ESD tree
  TTree*               ftreeF;      //! Pointer to the ESD friend tree
  TTree*               fhlttree;    //! Pointer to the HLT ESD tree
  IlcVertexerTracks*   ftVertexer;  //! Pointer to the vertexer based on ESD tracks
  Bool_t               fIsNewRunLoader; // gilc.root created from scratch (real raw data case)
  Bool_t               fRunIlcEVE;  // Run IlcEVE or not
  Double_t             fBeamInt[2][2]; //! mean beam intensities from IlcLHCData
  
  TTree*              fChain;      //! The raw-data chain in case of IlcRawReaderChain

  // Counters used in WriteESDfriend()
  Int_t fNall;    //! Number of reconstructed events
  Int_t fNspecie; //! Number of events, selected by IsCosmicOrCalibSpecie
  Int_t fSspecie; //! Number of events, sampled from fNspecie
  Int_t fNhighPt; //! Number of events, selected by IsHighPt 
  Int_t fShighPt; //! Number of events, sampled from fNhighPt
 
  // Upgrade detector reconstruction
  TString fUpgradeModule;
  Bool_t  fUpgradeMask[kNDetectors];
  // Connect an analysis manager in the event loop
  TString              fAnalysisMacro; // Full path to a macro creating an analysis manager train
// // // //   IlcAnalysisManager  *fAnalysis;      //! Analysis manager
  IlcRecoInputHandler *fRecoHandler;   //! Input handler adapted for reconstruction
  TString              fDeclTriggerClasses; // Declared defined trigger classes
  //
  Bool_t               fStopped;        //  flag that reco is stopped due to the limited resources 
  Int_t                fMaxRSS;         //  max RSS memory, MB
  Int_t                fMaxVMEM;        //  max VMEM memory, MB
  static const char*   fgkStopEvFName;  //  filename for stop.event stamp
  //
  ClassDef(IlcReconstruction, 43)      // class for running the reconstruction
};

#endif
