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
 
/* $Id: IlcTARGETreconstruction.cxx,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $ */
/////////////////////////////////////////////////////////////////////////
//                                                                     //
// Class for TARGET RecPoint reconstruction                               //
//                                                                     //
////////////////////////////////////////////////////////////////////////

#include <TString.h>
#include <TTree.h>

#include "IlcRun.h"
#include "IlcRunLoader.h"
#include "IlcTARGETDetTypeRec.h"
#include "IlcTARGETLoader.h"
#include "IlcTARGETreconstruction.h"
#include "IlcTARGETgeom.h"


ClassImp(IlcTARGETreconstruction)

//______________________________________________________________________
IlcTARGETreconstruction::IlcTARGETreconstruction():
 fInit(kFALSE),
 fEnt(0),
 fEnt0(0),
 //fTARGET(0x0),
 fDfArp(kFALSE),
 fLoader(0x0),
 fRunLoader(0x0)
{
    // Default constructor.
    // Inputs:
    //  none.
    // Outputs:
    //   none.
    // Return:
    //    A zero-ed constructed IlcTARGETreconstruction class.
    fDet[0] = fDet[1] = fDet[2] = kTRUE;
}
//______________________________________________________________________

IlcTARGETreconstruction::IlcTARGETreconstruction(IlcRunLoader *rl):
 fInit(kFALSE),
 fEnt(0),
 fEnt0(0),
 //fTARGET(0x0),
 fDfArp(kFALSE),
 fLoader(0x0),
 fRunLoader(rl)
{
  fDet[0] = fDet[1] = fDet[2] = kTRUE;
}
//______________________________________________________________________
IlcTARGETreconstruction::IlcTARGETreconstruction(const char* filename):
 fInit(kFALSE),
 fEnt(0),
 fEnt0(0),
 //fTARGET(0x0),
 fDfArp(kFALSE),
 fLoader(0x0),
 fRunLoader(0x0)
{
    // Standard constructor.
    // Inputs:
    //  const char* filename    filename containing the digits to be
    //                          reconstructed. If filename = 0 (nil)
    //                          then no file is opened but a file is
    //                          assumed to already be opened. This 
    //                          already opened file will be used.
    // Outputs:
    //   none.
    // Return:
    //    A standardly constructed IlcTARGETreconstruction class.

    fDet[0] = fDet[1] = fDet[2] = kTRUE;

    fRunLoader = IlcRunLoader::Open(filename);
    if (fRunLoader == 0x0)
     {
       Error("IlcTARGETreconstruction","Can not load the session",filename);
       return;
     }
    fRunLoader->LoadgIlc();
    gIlc = fRunLoader->GetIlcRun();

    if(!gIlc) {
          Error("IlcTARGETreconstruction","gIlc not found on file. Aborting.");
          fInit = kFALSE;
          return;
      } // end if !gIlc

}
//______________________________________________________________________
IlcTARGETreconstruction::IlcTARGETreconstruction(const IlcTARGETreconstruction &/*rec*/):TTask(/*rec*/){
    // Copy constructor. 

  Error("Copy constructor","Copy constructor not allowed");
  
}
//______________________________________________________________________
IlcTARGETreconstruction& IlcTARGETreconstruction::operator=(const IlcTARGETreconstruction& /*source*/){
    // Assignment operator. This is a function which is not allowed to be
    // done.
    Error("operator=","Assignment operator not allowed\n");
    return *this; 
}

//______________________________________________________________________
IlcTARGETreconstruction::~IlcTARGETreconstruction(){
    //    A destroyed IlcTARGETreconstruction class.
    
    //fTARGET      = 0;
    delete fRunLoader;
    
}
//______________________________________________________________________
Bool_t IlcTARGETreconstruction::Init(){
    // Class Initilizer.
    // Inputs:
    //  none.
    // Outputs:
    //   none.
    // Return:
    //    kTRUE if no errors initilizing this class occurse else kFALSE
    Info("Init","");
    if (fRunLoader == 0x0)
     {
       Error("Init","Run Loader is NULL");
       return kFALSE;
     }
    fRunLoader->LoadgIlc();
    fRunLoader->LoadHeader();  

    fLoader = (IlcTARGETLoader*) fRunLoader->GetLoader("TARGETLoader");
    if(!fLoader) {
      Error("Init","TARGET loader not found");
      fInit = kFALSE;
    }

    // Now ready to init.
 
    fRunLoader->CdGAFile();
    fTARGETgeom = (IlcTARGETgeom*)gDirectory->Get("IlcTARGETgeom");

    fDetTypeRec = new IlcTARGETDetTypeRec(fLoader);
    fDetTypeRec->SetDefaults();
    fDet[0] = fDet[1] = fDet[2] = kTRUE;
    fEnt0 = 0;

    //fEnt  = gIlc->GetEventsPerRun();
    fEnt = Int_t(fRunLoader->TreeE()->GetEntries());

    fLoader->LoadDigits("read");
    fLoader->LoadRecPoints("recreate");
    fLoader->LoadRawClusters("recreate");
    if (fLoader->TreeR() == 0x0) fLoader->MakeTree("R");
    if (fLoader->TreeC() == 0x0) fLoader->MakeTree("C");
 
    fDetTypeRec->MakeBranchR(0);
    fDetTypeRec->MakeBranchC();
    fDetTypeRec->SetTreeAddress();
    fDetTypeRec->SetTreeAddressR(fLoader->TreeR());

    fInit = InitRec();

    Info("Init","  Done\n\n\n");

    return fInit;
}
//______________________________________________________________________
Bool_t IlcTARGETreconstruction::InitRec(){
    // Sets up Reconstruction part of IlcTARGETDetType..
    // Inputs:
    //      none.
    // Outputs:
    //      none.
    // Return:
    //      none.
  /*
  //IlcTARGETDetType *idt;
  fDetTypeRec->SetLoader(fLoader);
    // SPD
  if(fDet[kSPD]){
    Info("InitRec","SPD");
    //idt = fTARGET->DetType(kSPD);
    IlcTARGETsegmentationSPD *segSPD = (IlcTARGETsegmentationSPD*)fDetTypeRec->GetSegmentationModel(0);
      TClonesArray *digSPD = fDetTypeRec->DigitsAddress(kSPD);
      TClonesArray *recpSPD = fDetTypeRec->ClustersAddress(kSPD);
      Info("InitRec","idt = %#x; digSPD = %#x; recpSPD = %#x",fDetTypeRec,digSPD,recpSPD);
      IlcTARGETClusterFinderSPD *recSPD = new IlcTARGETClusterFinderSPD(segSPD,digSPD,recpSPD);
      fDetTypeRec->SetReconstructionModel(kSPD,recSPD);
    } // end if fDet[kSPD].
  // SDD
  if(fDet[kSDD]){
    Info("InitRec","SDD");
    //    idt = fTARGET->DetType(kSDD);
    IlcTARGETsegmentationSDD *segSDD = (IlcTARGETsegmentationSDD*)
      fDetTypeRec->GetSegmentationModel(1);
    IlcTARGETresponseSDD *resSDD = (IlcTARGETresponseSDD*)
      fDetTypeRec->GetCalibrationModel(fDetTypeRec->GetTARGETgeom()->GetStartSDD()); 
    TClonesArray *digSDD = fDetTypeRec->DigitsAddress(kSDD);
    TClonesArray *recpSDD = fDetTypeRec->ClustersAddress(kSDD);
    IlcTARGETClusterFinderSDD *recSDD =new IlcTARGETClusterFinderSDD(segSDD,
							       resSDD,
							       digSDD,recpSDD);
    fDetTypeRec->SetReconstructionModel(kSDD,recSDD);
  } // end if fDet[kSDD]
    // SSD
  if(fDet[kSSD]){
    Info("InitRec","SSD");
    //idt = fTARGET->DetType(kSSD);
    IlcTARGETsegmentationSSD *segSSD = (IlcTARGETsegmentationSSD*)
                                       fDetTypeRec->GetSegmentationModel(2);
      TClonesArray *digSSD = fDetTypeRec->DigitsAddress(kSSD);
      IlcTARGETClusterFinderSSD *recSSD =new IlcTARGETClusterFinderSSD(segSSD,
								 digSSD);
      recSSD->SetTARGETgeom(fDetTypeRec->GetTARGETgeom());
      fDetTypeRec->SetReconstructionModel(kSSD,recSSD);
    } // end if fDet[kSSD]
  */
  fDetTypeRec->SetDefaultClusterFinders();
    Info("InitRec","    Done\n");
    return kTRUE;
}
//______________________________________________________________________ 
void IlcTARGETreconstruction::Exec(const Option_t *opt){
    // Main reconstruction function.
    // Inputs:
    //      Option_t * opt   list of subdetector to digitize. =0 all.
    // Outputs:
    //      none.
    // Return:
    //      none.
    Option_t *lopt;
    Int_t evnt;

    if(strstr(opt,"All")||strstr(opt,"ALL")||strstr(opt,"TARGET")||opt==0){
      fDet[0] = fDet[1] = fDet[2] = kTRUE;
      lopt = "All";
    }else{
      fDet[0] = fDet[1] = fDet[2] = kFALSE;
      if(strstr(opt,"SPD")) fDet[kSPD] = kTRUE;
      if(strstr(opt,"SDD")) fDet[kSDD] = kTRUE;
      if(strstr(opt,"SSD")) fDet[kSSD] = kTRUE;
      if(fDet[kSPD] && fDet[kSDD] && fDet[kSSD]) lopt = "All";
      else lopt = opt;
    } // end if strstr(opt,...)

    if(!fInit){
      cout << "Initilization Failed, Can't run Exec." << endl;
      return;
    } // end if !fInit
    for(evnt=0;evnt<fEnt;evnt++)
     {
      Info("Exec","");
      Info("Exec","Processing Event %d",evnt);
      Info("Exec","");

      fRunLoader->GetEvent(evnt);
      if (fLoader->TreeR() == 0x0) fLoader->MakeTree("R");
      fDetTypeRec->MakeBranchR(0);
      if (fLoader->TreeC() == 0x0){
	fDetTypeRec->MakeTreeC();
	fDetTypeRec->MakeBranchC();
      }
      fDetTypeRec->SetTreeAddressR(fLoader->TreeR());
      fDetTypeRec->SetTreeAddressD(fLoader->TreeD());
      fDetTypeRec->DigitsToRecPoints(evnt,0,lopt);
    } // end for evnt
}
//______________________________________________________________________ 
void IlcTARGETreconstruction::SetOutputFile(TString filename){
  // Set a new file name for recpoints. 
  // It must be called before Init()
  if(!fLoader)fLoader = (IlcTARGETLoader*) fRunLoader->GetLoader("TARGETLoader");
  if(fLoader){
    Info("SetOutputFile","name for rec points is %s",filename.Data());
    fLoader->SetRecPointsFileName(filename);
  }
  else {
    Error("SetOutputFile",
    "TARGET loader not available. Not possible to set name: %s",filename.Data());
  }
}
