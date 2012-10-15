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
 
/* $Id: IlcTARGEThitDigitizer.cxx,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */

#include <stdlib.h>
#include <Riostream.h>
#include <TObjArray.h>
#include <TTree.h>
#include <TBranch.h>

#include <IlcRun.h>
#include <IlcRunDigitizer.h>

#include "IlcTARGETDigitizer.h"
#include "IlcTARGEThit.h"
#include "IlcTARGETmodule.h"
#include "IlcTARGETsimulation.h"
#include "IlcTARGETDetType.h"
#include "IlcTARGETgeom.h"

ClassImp(IlcTARGETDigitizer)

//______________________________________________________________________
IlcTARGETDigitizer::IlcTARGETDigitizer() : IlcDigitizer(){
    // Default constructor. Assign fTARGET since it is never written out from
    // here. 
    // Inputs:
    //      Option_t * opt   Not used
    // Outputs:
    //      none.
    // Return:
    //      A blank IlcTARGETDigitizer class.

    fTARGET = 0;
}
//______________________________________________________________________
IlcTARGETDigitizer::IlcTARGETDigitizer(IlcRunDigitizer *mngr) : IlcDigitizer(mngr){
    // Standard constructor. Assign fTARGET since it is never written out from
    // here. 
    // Inputs:
    //      Option_t * opt   Not used
    // Outputs:
    //      none.
    // Return:
    //      An IlcItSDigitizer class.

    fTARGET = 0;
}
//______________________________________________________________________
IlcTARGETDigitizer::~IlcTARGETDigitizer(){
    // Default destructor. 
    // Inputs:
    //      Option_t * opt   Not used
    // Outputs:
    //      none.
    // Return:
    //      none.

    fTARGET = 0; // don't delete fTARGET. Done else where.
}

//______________________________________________________________________
Bool_t IlcTARGETDigitizer::Init(){
    // Iniliztion 
    // Inputs:
    //      none.
    // Outputs:
    //      none.
    // Return:
    //      none.

//    if(GetHits()) fTARGET->fHits = new TClonesArray("IlcTARGETHit",1000);
    return kTRUE;
}
//______________________________________________________________________
void IlcTARGETDigitizer::Exec(Option_t* opt){
    // Main digitizing function. 
    // Inputs:
    //      Option_t * opt   list of subdetector to digitize. =0 all.
    // Outputs:
    //      none.
    // Return:
    //      none.
    Int_t size=0,ifls=0,trk=0,ntracks=0,h=0,nhit=0;
    TTree *treeH=0;
    TBranch *brchHits=0;
    IlcTARGEThit *itsHit=0;
    char name[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    char *all;
    const char *det[3] = {strstr(opt,"SPD"),strstr(opt,"SDD"),
                          strstr(opt,"SSD")};
    if(!det[0] && !det[1] && !det[2]) all = "All";
    else all = 0;
    IlcTARGETsimulation *sim      = 0;
    IlcTARGETDetType    *iDetType = 0;
    IlcTARGETmodule     *mod      = 0;
    Int_t id=0,module=0,nfls=0,mask=0;
    static Bool_t setDef=kTRUE;

    if(!fTARGET) fTARGET = (IlcTARGET*)(gIlc->GetDetector("TARGET"));
    if(!(fTARGET->GetTARGETgeom())){
	Warning("Exec","Need TARGET geometry to be properly defined first.");
	return; // need transformations to do digitization.
    } // end if !GetTARGETgeom()
    if (setDef) fTARGET->SetDefaultSimulation();
    setDef=kFALSE;
    sprintf(name,"%s",fTARGET->GetName());
    if(!GetModules()) {
	fTARGET->InitModules(0,size);
    } // end if

    nfls = GetManager()->GetNinputs();
    for(ifls=0;ifls<nfls;ifls++){
	treeH = GetManager()->GetInputTreeH(ifls);
	if(!(treeH && GetHits())) continue;
	brchHits = treeH->GetBranch(name);
	if(brchHits){
	    GetHits()->Clear();
	    fTARGET->SetHitsAddressBranch(brchHits);
	} else{
	    Error("Exec","branch TARGET not found");
	} // end if brchHits

	ntracks = (Int_t) treeH->GetEntries();
	for(trk=0;trk<ntracks;trk++){
	    GetHits()->Clear();
	    brchHits->GetEntry(trk);
	    nhit = GetHits()->GetEntries();
	    mask = GetManager()->GetMask(ifls);
	    for(h=0;h<nhit;h++){
		itsHit = GetHit(h);
		mod = GetModule(itsHit->GetModule());
		id       = fTARGET->GetTARGETgeom()->GetModuleType(module);
		if (!all && !det[id]) continue;
		mod->AddHit(itsHit,trk+mask,h);
	    } // end for h
	} // end for trk
    } // end for ifls

    // Digitize 
    fTARGET->MakeBranchInTreeD(GetManager()->GetTreeD());
    for(module=0;module<size;module++){
        id       = fTARGET->GetTARGETgeom()->GetModuleType(module);
        if (!all && !det[id]) continue;
        iDetType = fTARGET->DetType(id);
        sim      = (IlcTARGETsimulation*)iDetType->GetSimulationModel();
        if (!sim) {
            Error("Exec","The simulation class was not instanciated!");
            exit(1);
        } // end if !sim
        mod      = GetModule(module);
        sim->DigitiseModule(mod,module,0);
        // fills all branches - wasted disk space
        GetManager()->GetTreeD()->Fill();
        fTARGET->ResetDigits();
    } // end for module
 
    fTARGET->ClearModules();
 
    GetManager()->GetTreeD()->GetEntries();
    GetManager()->GetTreeD()->Write(0,TObject::kOverwrite);
    // reset tree
    GetManager()->GetTreeD()->Reset();
}
