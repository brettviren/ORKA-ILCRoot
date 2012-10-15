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
 
/* $Id: IlcTARGETFDigitizer.cxx,v 1.2 2009/12/11 11:36:18 vitomeg Exp $ */

//////////////////////////////////////////////////////////////////
// Class for fast reconstruction of recpoints                   //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <TTree.h>

#include <IlcRun.h>
#include <IlcRunLoader.h>
#include <IlcLoader.h>
// #include <IlcRunDigitizer.h>

#include "IlcTARGETFDigitizer.h"
#include "IlcTARGETgeom.h"
#include "IlcTARGETsimulationFastPoints.h"

ClassImp(IlcTARGETFDigitizer)

//______________________________________________________________________
IlcTARGETFDigitizer::IlcTARGETFDigitizer() : IlcDigitizer(){
//
// Default constructor.
//
    fTARGET      = 0;
    fInit     = kFALSE;
}
//______________________________________________________________________
IlcTARGETFDigitizer::IlcTARGETFDigitizer(IlcDigitizationInput *mngr) : IlcDigitizer(mngr){
//
// Standard constructor.
//
    fTARGET      = 0;
    fInit     = kFALSE;
}
//______________________________________________________________________
IlcTARGETFDigitizer::IlcTARGETFDigitizer(const IlcTARGETFDigitizer &/*rec*/):IlcDigitizer(/*rec*/){
    // Copy constructor. 

  Error("Copy constructor","Copy constructor not allowed");
  
}
//______________________________________________________________________
IlcTARGETFDigitizer& IlcTARGETFDigitizer::operator=(const IlcTARGETFDigitizer& /*source*/){

    // Assignment operator. This is a function which is not allowed to be
    // done.
    Error("operator=","Assignment operator not allowed\n");
    return *this; 
}

//______________________________________________________________________
IlcTARGETFDigitizer::~IlcTARGETFDigitizer(){
//
// Default destructor. 
//
    fTARGET = 0; // don't delete fTARGET. Done else where.
}
//______________________________________________________________________
Bool_t IlcTARGETFDigitizer::Init(){
//
// Initialization. 
// loads TARGET and TARGETgeom.
// Inputs:
//      none.
// Outputs:
//      none.

  
  fInit = kFALSE;
  if(!gIlc) {
    fTARGET      = 0;
    Warning("Init","gIlc not found");
    return fInit;
  }
  fTARGET = (IlcTARGET *)(gIlc->GetDetector("TARGET"));
  if(!fTARGET){
    Warning("Init","TARGET not found");
    return fInit;
  } 
  if(!fTARGET->GetTARGETgeom()){
    Warning("Init","TARGET geometry not found");
    return fInit;
  }
  return fInit = kTRUE;
}
////////////////////////////////////////////////////////////////////////
void IlcTARGETFDigitizer::Exec(Option_t* opt){
//
// Main digitization function. 
// Inputs:
//      Option_t * opt  "deb" ... more verbose output 
//

    IlcTARGETsimulationFastPoints *sim = new IlcTARGETsimulationFastPoints();
    IlcRunLoader* outrl = IlcRunLoader::GetRunLoader( fDigInput->GetOutputFolderName() );
    if (outrl == 0x0){
	Error("Exec","Can not find Run Loader in output folder.");
	return;
    }
    IlcLoader* outgime = outrl->GetLoader("TARGETLoader");
    if (outgime == 0x0){
	Error("Exec","Can not get TOF Loader from Output Run Loader.");
	return;
    }
    if(strstr(opt,"deb")){
	Info("Exec","sim=%p, outrl=%p, outgime=%p",sim,outrl,outgime);
    }
    TTree* outputTreeR = outgime->TreeR();
    if (outputTreeR == 0x0){
	outgime->MakeTree("R");
	outputTreeR = outgime->TreeR();
    }
    
    TClonesArray* recPoints = new TClonesArray("IlcTARGETRecPoint",1000);
    TBranch* branch = outputTreeR->GetBranch("TARGETRecPointsF");
    if(branch) branch->SetAddress(recPoints);
    else outputTreeR->Branch("TARGETRecPointsF",&recPoints);
    Int_t nModules;
    fTARGET->InitModules(-1,nModules);

// load hits into modules
    for (Int_t iFile = 0; iFile < fDigInput->GetNinputs(); iFile++){
	IlcRunLoader* rl = IlcRunLoader::GetRunLoader( fDigInput->GetInputFolderName(iFile) );
	if (rl == 0x0){
	    Error("Exec","Can not find Run Loader in input %d folder.",iFile);
	    return;
	}

	IlcLoader* gime = rl->GetLoader("TARGETLoader");
	if (gime == 0x0){
	    Error("Exec","Can not get TOF Loader from Input %d Run Loader.",
		  iFile);
	    return;
	}

	gime->LoadHits();
        fTARGET->FillModules(gime->TreeH(),fDigInput->GetMask(iFile));
	gime->UnloadHits();
    }
  
// transform hits to fast rec points

    IlcTARGETgeom *geom = fTARGET->GetTARGETgeom();
    for(Int_t moduleIndex = 0; moduleIndex<geom->GetIndexMax(); moduleIndex++){
	sim->CreateFastRecPoints(moduleIndex,recPoints);
	outputTreeR->Fill();
	recPoints->Clear();
    }
    outrl->WriteRecPoints("OVERWRITE");
//  outputTreeR->AutoSave();
}
////////////////////////////////////////////////////////////////////////
