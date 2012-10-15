/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                                                           //
//  DCH trigger class                                                        //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TTree.h>
#include <TBranch.h>
#include <TMatrixD.h>

#include "IlcLog.h"
#include "IlcRun.h"
#include "IlcLoader.h"

#include "IlcDCHdigitsManager.h"
#include "IlcDCHgeometry.h"
#include "IlcDCHdataArrayI.h"
#include "IlcDCHcalibDB.h"
#include "IlcDCHCommonParam.h"
#include "Cal/IlcDCHCalPIDLQ.h"
#include "IlcDCHrawData.h"

#include "IlcDCHtrigger.h"
#include "IlcDCHmodule.h"
#include "IlcDCHmcmTracklet.h"
#include "IlcDCHtrigParam.h"
#include "IlcDCHmcm.h"
#include "IlcDCHzmaps.h"

//#include "IlcHeader.h"

ClassImp(IlcDCHtrigger)

//_____________________________________________________________________________
IlcDCHtrigger::IlcDCHtrigger():
  TNamed(),
  fTracks("IlcDCHgtuTrack",0)
{
  //
  // IlcDCHtrigger default constructor
  //

  fDigitsManager = NULL;
  fTrackletTree = NULL;
  fTracklets     = NULL;

  fNROB = 0;
  fTrigParam = NULL;
  fMCM = NULL;
  fTrk = NULL;
  fTrkTest = NULL;
  fGTUtrk = NULL;

  fNtracklets = 0;

  fDigits = NULL;
  fTrack0 = NULL;
  fTrack1 = NULL;
  fTrack2 = NULL;

  fModule = NULL;

  fNPrimary = 0;

  fField  = 0.0;
  fGeo    = NULL;
  fCalib  = NULL;
  fCParam = NULL;

}

//_____________________________________________________________________________
IlcDCHtrigger::IlcDCHtrigger(const Text_t *name, const Text_t *title):
  TNamed(name,title),
  fTracks("IlcDCHgtuTrack",1000)
{
  //
  // IlcDCHtrigger constructor
  //

  fDigitsManager = new IlcDCHdigitsManager();
  fTrackletTree = NULL;
  fTracklets = new TObjArray(400);

  fNROB = 0;
  fTrigParam = NULL;
  fMCM = NULL;
  fTrk = NULL;
  fTrkTest = NULL;
  fGTUtrk = NULL;

  fNtracklets = 0;

  fDigits = NULL;
  fTrack0 = NULL;
  fTrack1 = NULL;
  fTrack2 = NULL;

  fModule = NULL;

  fNPrimary = 0;

  fField  = 0.0;
  fGeo    = NULL;
  fCalib  = NULL;
  fCParam = NULL;

}

//_____________________________________________________________________________
IlcDCHtrigger::IlcDCHtrigger(const IlcDCHtrigger &p):TNamed(p)
{
  //
  // IlcDCHtrigger copy constructor
  //

  ((IlcDCHtrigger &) p).Copy(*this);

}

///_____________________________________________________________________________
IlcDCHtrigger::~IlcDCHtrigger()
{
  //
  // IlcDCHtrigger destructor
  //

  if (fTracklets) {
    fTracklets->Delete();
    delete fTracklets;
  }

  fTracks.Delete();

}

//_____________________________________________________________________________
IlcDCHtrigger &IlcDCHtrigger::operator=(const IlcDCHtrigger &p)
{
  //
  // Assignment operator
  //

  if (this != &p) ((IlcDCHtrigger &) p).Copy(*this);
  return *this;

}

//_____________________________________________________________________________
void IlcDCHtrigger::Copy(TObject &) const
{
  //
  // Copy function
  //

  IlcFatal("Not implemented");

}

//_____________________________________________________________________________
void IlcDCHtrigger::Init()
{

  fModule = new IlcDCHmodule(fTrigParam); 
  /*
  IlcHeader *header = fRunLoader->GetHeader();
  fNPrimary = header->GetNprimary();
  */
  fTracks.Clear();

  fField = fTrigParam->GetField();
  fGeo = (IlcDCHgeometry*)IlcDCHgeometry::GetGeometry(fRunLoader);

  fCalib = IlcDCHcalibDB::Instance();
  if (!fCalib)
  {
    Error("Init","No instance of IlcDCHcalibDB.");
    return;  
  }

  fCParam = IlcDCHCommonParam::Instance();
  if (!fCParam)
  {
    Error("Init","No common params.");
    return;
  }

}

//_____________________________________________________________________________
Bool_t IlcDCHtrigger::Open(const Char_t *name, Int_t nEvent)
{
  //
  // Opens the IlcROOT file.
  //

  TString evfoldname = IlcConfig::GetDefaultEventFolderName();
  fRunLoader = IlcRunLoader::GetRunLoader(evfoldname);

  if (!fRunLoader)
    fRunLoader = IlcRunLoader::Open(name);

  if (!fRunLoader) {
    Error("Open","Can not open session for file %s.",name);
    return kFALSE;
  }

  // Open input

  if (fRunLoader->GetIlcRun() == 0x0) fRunLoader->LoadgIlc();
  gIlc = fRunLoader->GetIlcRun();

  if (!(gIlc)) {
    fRunLoader->LoadgIlc();
    gIlc = fRunLoader->GetIlcRun();
    if (!(gIlc)) {
      Error("Open","Could not find IlcRun object.");
      return kFALSE;
    }
  }

  // Import the Trees for the event nEvent in the file
  fRunLoader->GetEvent(nEvent);

  // Open output

  TObjArray *ioArray = 0;

  IlcLoader* loader = fRunLoader->GetLoader("DCHLoader");
  loader->MakeTree("T");
  fTrackletTree = loader->TreeT();
  fTrackletTree->Branch("DCHmcmTracklet","TObjArray",&ioArray,32000,0);
  /*
  fRunLoader->LoadHeader();
  */
  Init();

  return kTRUE;

}


//_____________________________________________________________________________
Bool_t IlcDCHtrigger::ReadDigits() 
{
  //
  // Reads the digits arrays from the input ilcroot file
  //

  if (!fRunLoader) {
    Error("ReadDigits","Can not find the Run Loader");
    return kFALSE;
  }

  IlcLoader* loader = fRunLoader->GetLoader("DCHLoader");
  if (!loader->TreeD()) loader->LoadDigits();

  if (!loader->TreeD()) return kFALSE;

  return (fDigitsManager->ReadDigits(loader->TreeD()));

}

//_____________________________________________________________________________
Bool_t IlcDCHtrigger::ReadDigits(IlcRawReader* rawReader)
{
  //
  // Reads the digits arrays from the ddl file
  //

  IlcDCHrawData *raw = new IlcDCHrawData();
  raw->SetDebug(1);

  fDigitsManager = raw->Raw2Digits(rawReader);

  return kTRUE;

}

//_____________________________________________________________________________
Bool_t IlcDCHtrigger::ReadTracklets(IlcRunLoader *rl) 
{
  //
  // Reads the tracklets find the tracks
  //

  Int_t idet;

  IlcLoader *loader = rl->GetLoader("DCHLoader");
  loader->LoadTracks();
  fTrackletTree = loader->TreeT();
  if(!fTrackletTree){
    Error("ReadTracklets","Can't get the tree with tracks !");
    return kFALSE;    
  }
  TBranch *branch = fTrackletTree->GetBranch("DCHmcmTracklet");
  if (!branch) {
    Error("ReadTracklets","Can't get the branch !");
    return kFALSE;
  }
  TObjArray *tracklets = new TObjArray(400);
  branch->SetAddress(&tracklets);

  Int_t nEntries = (Int_t) fTrackletTree->GetEntries();
  Int_t iEntry, itrk;
  Int_t iStack, iStackPrev = -1;
  
  for (iEntry = 0; iEntry < nEntries; iEntry++) {    
    fTrackletTree->GetEvent(iEntry);
    
    for (itrk = 0; itrk < tracklets->GetEntriesFast(); itrk++){

      fTrk = (IlcDCHmcmTracklet*)tracklets->UncheckedAt(itrk);
      
      idet = fTrk->GetDetector();

      iStack = idet / (IlcDCHgeometry::Nplan());
      if (iStackPrev != iStack) {
	if (iStackPrev == -1) {
	  iStackPrev = iStack;
	} else {
	  MakeTracks(idet-IlcDCHgeometry::Nplan());
	  ResetTracklets();
	  iStackPrev = iStack;
	}
      }
      
      Tracklets()->Add(fTrk);

      if (iEntry == (nEntries-1) && itrk == (tracklets->GetEntriesFast()-1)) {
	idet++;
	MakeTracks(idet-IlcDCHgeometry::Nplan());
	ResetTracklets();
      }

    }

  }

  loader->UnloadTracks();

  return kTRUE;

}

//_____________________________________________________________________________
Bool_t IlcDCHtrigger::MakeTracklets(Bool_t makeTracks)
{
  //
  // Create tracklets from digits
  //

  Int_t    chamBeg = 0;
  Int_t    chamEnd = IlcDCHgeometry::Ncham();
  Int_t    planBeg = 0;
  Int_t    planEnd = IlcDCHgeometry::Nplan();
  Int_t    sectBeg = 0;
  Int_t    sectEnd = IlcDCHgeometry::Nsect();

  fTrkTest = new IlcDCHmcmTracklet(0,0,0);

  fMCM = new IlcDCHmcm(fTrigParam,0);

  Int_t time, col, row, col1, col2;
  Float_t amp;
  Int_t idet, iStack, iStackPrev;
  idet       = -1;
  iStack     = -1;
  iStackPrev = -1;
  for (Int_t isect = sectBeg; isect < sectEnd; isect++) {

    for (Int_t icham = chamBeg; icham < chamEnd; icham++) {

      // number of ROBs in the chamber
      if( icham == 2 ) {
	fNROB = 6;
      } else {
	fNROB = 8;
      }

      for (Int_t iplan = planBeg; iplan < planEnd; iplan++) {

        idet = fGeo->GetDetector(iplan,icham,isect);
	ResetTracklets();
	
	if (makeTracks) {
	  iStack = idet / (IlcDCHgeometry::Nplan());
	  if (iStackPrev != iStack) {
	    if (iStackPrev == -1) {
	      iStackPrev = iStack;
	    } else {
	      MakeTracks(idet-IlcDCHgeometry::Nplan());
	      ResetTracklets();
	      iStackPrev = iStack;
	    }
	  }
	}

        Int_t    nRowMax     = fCParam->GetRowMax(iplan,icham,isect);
	Int_t    nColMax     = fCParam->GetColMax(iplan);
        Int_t    nTimeTotal  = fCalib->GetNumberOfTimeBins();

        // Get the digits
        fDigits = fDigitsManager->GetDigits(idet);
	if (!fDigits) return kFALSE;
        fDigits->Expand();
        fTrack0 = fDigitsManager->GetDictionary(idet,0);
	if (!fTrack0) return kFALSE;
        fTrack0->Expand();
        fTrack1 = fDigitsManager->GetDictionary(idet,1);
	if (!fTrack1) return kFALSE;
        fTrack1->Expand();
        fTrack2 = fDigitsManager->GetDictionary(idet,2); 
	if (!fTrack2) return kFALSE;
        fTrack2->Expand();

	for (Int_t iRob = 0; iRob < fNROB; iRob++) {

	  for (Int_t iMcm = 0; iMcm < kNMCM; iMcm++) {

	    fMCM->Reset();

	    fMCM->SetRobId(iRob);
	    fMCM->SetChaId(idet);

	    SetMCMcoordinates(iMcm);

	    row = fMCM->GetRow();

	    if (row < 0 || row >= nRowMax) {
	      Error("MakeTracklets","MCM row number out of range.");
	      continue;
	    }

	    fMCM->GetColRange(col1,col2);
	    
            for (time = 0; time < nTimeTotal; time++) {
	      for (col = col1; col < col2; col++) {
		if (col >= 0 && col < nColMax) {
		  amp = TMath::Abs(fDigits->GetDataUnchecked(row,col,time));
		} else {
		  amp = 0.0;
		}
		fMCM->SetADC(col-col1,time,amp);

	      }
	    }

	    if (fTrigParam->GetTailCancelation()) {
	      fMCM->Filter(fTrigParam->GetNexponential(),fTrigParam->GetFilterType());
	    }
	    
	    if (fMCM->Run()) {

	      for (Int_t iSeed = 0; iSeed < kMaxTrackletsPerMCM; iSeed++) {
		
		if (fMCM->GetSeedCol()[iSeed] < 0) continue;

		if ( fTrigParam->GetDebugLevel() > 1 ) 
		  printf("Add tracklet %d in col %02d \n",fNtracklets,fMCM->GetSeedCol()[iSeed]);

		if ( fTrigParam->GetDebugLevel() == -1 ) {
		  printf("Add tracklet %d in col %02d \n",fNtracklets,fMCM->GetSeedCol()[iSeed]);
		  for (time = 0; time < nTimeTotal; time++) {
		    for (col = 0; col < kMcmCol; col++) {		    
		      printf("%03.0f  ",fMCM->GetADC(col,time));
		    }
		    printf("\n");
		  }
		}

		if (TestTracklet(idet,row,iSeed,0)) {
		  AddTracklet(idet,row,iSeed,fNtracklets++);
		}

	      }

	    }

	  }

      
	}

	// Compress the arrays
        fDigits->Compress(1,0);
        fTrack0->Compress(1,0);
        fTrack1->Compress(1,0);
        fTrack2->Compress(1,0);

	WriteTracklets(idet);

     }
    }
  }

  if (makeTracks) {
    idet++;
    MakeTracks(idet-IlcDCHgeometry::Nplan());
    ResetTracklets();
  }

  return kTRUE;

}

//_____________________________________________________________________________
void IlcDCHtrigger::SetMCMcoordinates(Int_t imcm)
{
  //
  // Configure MCM position in the pad plane
  //

  Int_t robid = fMCM->GetRobId();

  // setting the Row and Col range

  const Int_t kNcolRob = 2;  // number of ROBs per chamber in column direction
  const Int_t kNmcmRob = 4;  // number of MCMs per ROB in column/row direction

  Int_t mcmid = imcm%(kNmcmRob*kNmcmRob);

  if (robid%kNcolRob == 0) {

    if ( mcmid%kNmcmRob == 0 ) {
      fMCM->SetColRange(18*0-1,18*1-1+2+1);
    }
    if ( mcmid%kNmcmRob == 1 ) {
      fMCM->SetColRange(18*1-1,18*2-1+2+1);
    }
    if ( mcmid%kNmcmRob == 2 ) {
      fMCM->SetColRange(18*2-1,18*3-1+2+1);
    }
    if ( mcmid%kNmcmRob == 3 ) {
      fMCM->SetColRange(18*3-1,18*4-1+2+1);
    }

  } else {

    if ( mcmid%kNmcmRob == 0 ) {
      fMCM->SetColRange(18*4-1,18*5-1+2+1);
    }
    if ( mcmid%kNmcmRob == 1 ) {
      fMCM->SetColRange(18*5-1,18*6-1+2+1);
    }
    if ( mcmid%kNmcmRob == 2 ) {
      fMCM->SetColRange(18*6-1,18*7-1+2+1);
    }
    if ( mcmid%kNmcmRob == 3 ) {
      fMCM->SetColRange(18*7-1,18*8-1+2+1);
    }

  } 

  fMCM->SetRow(kNmcmRob*(robid/kNcolRob)+mcmid/kNmcmRob);

}

//_____________________________________________________________________________
Bool_t IlcDCHtrigger::TestTracklet(Int_t det, Int_t row, Int_t seed, Int_t n)
{
  //
  // Check first the tracklet pt
  //

  Int_t nTimeTotal  = fCalib->GetNumberOfTimeBins();

  fTrkTest->Reset();

  fTrkTest->SetDetector(det);
  fTrkTest->SetRow(row);
  fTrkTest->SetN(n);

  Int_t iCol, iCol1, iCol2, track[3];
  iCol = fMCM->GetSeedCol()[seed];  // 0....20 (MCM)
  fMCM->GetColRange(iCol1,iCol2);   // range in the pad plane
	    
  Float_t amp[3];
  for (Int_t iTime = 0; iTime < nTimeTotal; iTime++) {

    amp[0] = fMCM->GetADC(iCol-1,iTime);
    amp[1] = fMCM->GetADC(iCol  ,iTime);
    amp[2] = fMCM->GetADC(iCol+1,iTime);

    // extract track contribution only from the central pad
    track[0] = fTrack0->GetDataUnchecked(row,iCol+iCol1,iTime);
    track[1] = fTrack1->GetDataUnchecked(row,iCol+iCol1,iTime);
    track[2] = fTrack2->GetDataUnchecked(row,iCol+iCol1,iTime);

    if (fMCM->IsCluster(iCol,iTime)) {

      fTrkTest->AddCluster(iCol+iCol1,iTime,amp,track);

    } else if ((iCol+1+1) < kMcmCol) {

      amp[0] = fMCM->GetADC(iCol-1+1,iTime);
      amp[1] = fMCM->GetADC(iCol  +1,iTime);
      amp[2] = fMCM->GetADC(iCol+1+1,iTime);

      if (fMCM->IsCluster(iCol+1,iTime)) {

	// extract track contribution only from the central pad
	track[0] = fTrack0->GetDataUnchecked(row,iCol+1+iCol1,iTime);
	track[1] = fTrack1->GetDataUnchecked(row,iCol+1+iCol1,iTime);
	track[2] = fTrack2->GetDataUnchecked(row,iCol+1+iCol1,iTime);

	fTrkTest->AddCluster(iCol+1+iCol1,iTime,amp,track);

      }

    } else {
    }

  }

  fTrkTest->CookLabel(0.8);  
  /*
  if (fTrkTest->GetLabel() >= fNPrimary) {
    Info("AddTracklet","Only primaries are stored!");
    return;
  }
  */
  // LTU Pt cut
    
  fTrkTest->MakeTrackletGraph(fGeo,fField);
  fTrkTest->MakeClusAmpGraph();
  if (TMath::Abs(fTrkTest->GetPt()) < fTrigParam->GetLtuPtCut()) {
    return kFALSE;
  }
  
  return kTRUE;  

}

//_____________________________________________________________________________
void IlcDCHtrigger::AddTracklet(Int_t det, Int_t row, Int_t seed, Int_t n)
{
  //
  // Add a found tracklet
  //

  Int_t nTimeTotal  = fCalib->GetNumberOfTimeBins();

  fTrk = new IlcDCHmcmTracklet(det,row,n);

  Int_t iCol, iCol1, iCol2, track[3];
  iCol = fMCM->GetSeedCol()[seed];  // 0....20 (MCM)
  fMCM->GetColRange(iCol1,iCol2);   // range in the pad plane
	    
  Float_t amp[3];
  for (Int_t iTime = 0; iTime < nTimeTotal; iTime++) {

    amp[0] = fMCM->GetADC(iCol-1,iTime);
    amp[1] = fMCM->GetADC(iCol  ,iTime);
    amp[2] = fMCM->GetADC(iCol+1,iTime);

    // extract track contribution only from the central pad
    track[0] = fTrack0->GetDataUnchecked(row,iCol+iCol1,iTime);
    track[1] = fTrack1->GetDataUnchecked(row,iCol+iCol1,iTime);
    track[2] = fTrack2->GetDataUnchecked(row,iCol+iCol1,iTime);

    if (fMCM->IsCluster(iCol,iTime)) {

      fTrk->AddCluster(iCol+iCol1,iTime,amp,track);

    } else if ((iCol+1+1) < kMcmCol) {

      amp[0] = fMCM->GetADC(iCol-1+1,iTime);
      amp[1] = fMCM->GetADC(iCol  +1,iTime);
      amp[2] = fMCM->GetADC(iCol+1+1,iTime);

      if (fMCM->IsCluster(iCol+1,iTime)) {

	// extract track contribution only from the central pad
	track[0] = fTrack0->GetDataUnchecked(row,iCol+1+iCol1,iTime);
	track[1] = fTrack1->GetDataUnchecked(row,iCol+1+iCol1,iTime);
	track[2] = fTrack2->GetDataUnchecked(row,iCol+1+iCol1,iTime);

	fTrk->AddCluster(iCol+1+iCol1,iTime,amp,track);

      }

    } else {
    }

  }

  fTrk->CookLabel(0.8);  
  /*
  if (fTrk->GetLabel() >= fNPrimary) {
    Info("AddTracklet","Only primaries are stored!");
    return;
  }
  */
  // LTU Pt cut
  fTrk->MakeTrackletGraph(fGeo,fField);
  fTrk->MakeClusAmpGraph();
  if (TMath::Abs(fTrk->GetPt()) < fTrigParam->GetLtuPtCut()) {
    return;
  }
      
  Tracklets()->Add(fTrk);

}

//_____________________________________________________________________________
Bool_t IlcDCHtrigger::WriteTracklets(Int_t det) 
{
  //
  // Fills DCHmcmTracklet branch in the tree with the Tracklets 
  // found in detector = det. For det=-1 writes the tree. 
  //

  if ((det < -1) || (det >= IlcDCHgeometry::Ndet())) {
    Error("WriteTracklets","Unexpected detector index %d.",det);
    return kFALSE;
  }

  TBranch *branch = fTrackletTree->GetBranch("DCHmcmTracklet");
  if (!branch) {
    TObjArray *ioArray = 0;
    branch = fTrackletTree->Branch("DCHmcmTracklet","TObjArray",&ioArray,32000,0);
  }

  if ((det >= 0) && (det < IlcDCHgeometry::Ndet())) {

    Int_t nTracklets = Tracklets()->GetEntriesFast();
    TObjArray *detTracklets = new TObjArray(400);

    for (Int_t i = 0; i < nTracklets; i++) {
      IlcDCHmcmTracklet *trk = (IlcDCHmcmTracklet *) Tracklets()->UncheckedAt(i);
      
      if (det == trk->GetDetector()) {
        detTracklets->AddLast(trk);
      }
      else {
      }
    }

    branch->SetAddress(&detTracklets);
    fTrackletTree->Fill();

    delete detTracklets;

    return kTRUE;

  }

  if (det == -1) {

    Info("WriteTracklets","Writing the Tracklet tree %s for event %d."
	 ,fTrackletTree->GetName(),fRunLoader->GetEventNumber());

    IlcLoader* loader = fRunLoader->GetLoader("DCHLoader");
    loader->WriteTracks("OVERWRITE");
    
    return kTRUE;  

  }

  return kFALSE;

}

//_____________________________________________________________________________
void IlcDCHtrigger::MakeTracks(Int_t det)
{
  //
  // Create GTU tracks per module (stack of 6 chambers)
  //
  
  fModule->Reset();

  Int_t nRowMax, iplan, icham, isect, row;

  if ((det < 0) || (det >= IlcDCHgeometry::Ndet())) {
    Error("MakeTracks","Unexpected detector index %d.",det);
    return;
  }
  
  Int_t nTracklets = Tracklets()->GetEntriesFast();
  
  IlcDCHmcmTracklet *trk;
  for (Int_t i = 0; i < nTracklets; i++) {
    
    trk = (IlcDCHmcmTracklet *) Tracklets()->UncheckedAt(i);
    
    iplan = fGeo->GetPlane(trk->GetDetector());
    icham = fGeo->GetChamber(trk->GetDetector());
    isect = fGeo->GetSector(trk->GetDetector());

    nRowMax = fCParam->GetRowMax(iplan,icham,isect);
    row = trk->GetRow();

    fModule->AddTracklet(trk->GetDetector(),
			 row,
			 trk->GetRowz(),
			 trk->GetSlope(),
			 trk->GetOffset(),
			 trk->GetTime0(),
			 trk->GetNclusters(),
			 trk->GetLabel(),
			 trk->GetdQdl());
    
  }

  fModule->SortTracklets();
  fModule->RemoveMultipleTracklets();
  fModule->SortZ((Int_t)fGeo->GetChamber(det));
  fModule->FindTracks();
  fModule->SortTracks();
  fModule->RemoveMultipleTracks();

  Int_t nModTracks = fModule->GetNtracks();
  IlcDCHgtuTrack *gtutrk;
  for (Int_t i = 0; i < nModTracks; i++) {
    gtutrk = (IlcDCHgtuTrack*)fModule->GetTrack(i);
    if (TMath::Abs(gtutrk->GetPt()) < fTrigParam->GetGtuPtCut()) continue;
    gtutrk->CookLabel();
    gtutrk->MakePID();
    AddTrack(gtutrk,det);
  }
  
}


