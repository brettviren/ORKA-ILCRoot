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
 
/* $Id: IlcTARGETsDigitize.cxx,v 1.2 2008/11/03 07:43:47 cgatto Exp $ */

#include <Riostream.h>
 
#include <TROOT.h>
#include <TFile.h>
#include <TSeqCollection.h>
#include <TString.h>
#include <TClonesArray.h>
 
#include "IlcHeader.h"
#include "IlcRun.h"
#include "IlcLoader.h"
 
#include "IlcTARGET.h"
#include "IlcTARGETsDigitize.h"
#include "IlcTARGETgeom.h"
 
ClassImp(IlcTARGETsDigitize)
//______________________________________________________________________
IlcTARGETsDigitize::IlcTARGETsDigitize(){
    // Default constructor.
    // Inputs:
    //  none.
    // Outputs:
    //   none.
    // Return:
    //    A zero-ed constructed IlcTARGETsDigitize class.
 
    fRunLoader = 0x0;
    fTARGET      = 0;
    fDet[0] = fDet[1] = fDet[2] = kTRUE;
    fInit     = kFALSE;
}
//______________________________________________________________________
IlcTARGETsDigitize::IlcTARGETsDigitize(const char* filename){
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
    //    A standardly constructed IlcTARGETsDigitize class.
 
    if(gIlc) 
     {
      delete gIlc;
      gIlc = 0;
     }
    fRunLoader = IlcRunLoader::Open(filename);
    fRunLoader->LoadgIlc();
    fRunLoader->LoadHeader();
    
 
    Init();
}
//______________________________________________________________________
IlcTARGETsDigitize::~IlcTARGETsDigitize(){
    // Default constructor.
    // Inputs:
    //  none.
    // Outputs:
    //   none.
    // Return:
    //    A destroyed IlcTARGETsDigitize class.
 
    if(fRunLoader) delete fRunLoader;
    fTARGET      = 0;
 
}
//______________________________________________________________________
Bool_t IlcTARGETsDigitize::Init(){
    // Class Initilizer.
    // Inputs:
    //  none.
    // Outputs:
    //   none.
    // Return:
    //    kTRUE if no errors initilizing this class occurse else kFALSE
    //Int_t nparticles;
   
    fTARGET = (IlcTARGET*) gIlc->GetDetector("TARGET");
    if(!fTARGET){
        cout << "TARGET not found aborting. fTARGET=" << fTARGET << endl;
        fInit = kFALSE;
        return fInit;
    } // end if !fTARGET
    if(!(fTARGET->GetTARGETgeom())){
        cout << "TARGETgeom not found aborting."<< endl;
        fInit = kFALSE;
        return fInit;
    } // end if !GetTARGETgeom()
    // Now ready to init.
 
    fDet[0] = fDet[1] = fDet[2] = kTRUE;
    fEnt0 = 0;
    fEnt  = IlcRunLoader::Instance()->GetNumberOfEvents();
 
    IlcLoader* loader = fRunLoader->GetLoader("TARGETLoader");
    
    if(!loader->TreeS()){
        cout << "Having to create the SDigits Tree." << endl;
        loader->MakeTree("S");
    } // end if !gIlc->TreeS()
    //make branch
    fTARGET->MakeBranch("S");
    fTARGET->SetTreeAddress();
    
    fRunLoader->GetEvent(fEnt0);
    // finished init.
    fInit = InitSDig();
    return fInit;
}
//______________________________________________________________________
Bool_t IlcTARGETsDigitize::InitSDig(){
    // Sets up SDigitization part of IlcTARGETDetType..
    // Inputs:
    //      none.
    // Outputs:
    //      none.
    // Return:
    //      none.
 
    return kTRUE;
}
 
//______________________________________________________________________
void IlcTARGETsDigitize::Exec(const Option_t *opt){
    // Main SDigitization function.
    // Inputs:
    //      Option_t * opt   list of subdetector to digitize. =0 all.
    // Outputs:
    //      none.
    // Return:
    //      none.
    Option_t *lopt;
//    Int_t nparticles,evnt;
 
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

    fTARGET->HitsToSDigits(fRunLoader->GetHeader()->GetEvent(),0,0,-1," ",lopt," ");
}
