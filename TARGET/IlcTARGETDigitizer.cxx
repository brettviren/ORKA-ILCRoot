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
 
/* $Id: IlcTARGETDigitizer.cxx,v 1.2 2009/12/11 11:36:18 vitomeg Exp $ */
///////////////////////////////////////////////////////////////////////////
//Piotr.Skowronski@cern.ch :                                             //
//Corrections applied in order to compile (only)                         // 
//   with new I/O and folder structure                                   //
//To be implemented correctly by responsible                             //
//                                                                       //
//  Class used to steer                                                  //
//  the digitization for TARGET                                             //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <TClonesArray.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TFile.h>

#include <IlcRun.h>
#include <IlcRunLoader.h>
#include <IlcLoader.h>
// #include <IlcRunDigitizer.h>

#include "IlcTARGETDigitizer.h"
#include "IlcTARGETgeom.h"
#include "IlcTARGETsimulation.h"
#include "IlcTARGETsimulationSSD.h"

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

    fTARGET      = 0;
    fModActive   = 0;
    fRoif     = -1;
    fRoiifile = 0;
    fInit     = kFALSE;
    fFlagFirstEv =kTRUE;
}
//______________________________________________________________________
IlcTARGETDigitizer::IlcTARGETDigitizer(IlcDigitizationInput *mngr) : IlcDigitizer(mngr){
    // Standard constructor. Assign fTARGET since it is never written out from
    // here. 
    // Inputs:
    //      Option_t * opt   Not used
    // Outputs:
    //      none.
    // Return:
    //      An IlcItSDigitizer class.

    fTARGET      = 0;
    fModActive   = 0;
    fRoif     = -1;
    fRoiifile = 0;
    fInit     = kFALSE;
    fFlagFirstEv =kTRUE;
}

//______________________________________________________________________
IlcTARGETDigitizer::IlcTARGETDigitizer(const IlcTARGETDigitizer &/*rec*/):IlcDigitizer(/*rec*/){
    // Copy constructor. 

  Error("Copy constructor","Copy constructor not allowed");
  
}
//______________________________________________________________________
IlcTARGETDigitizer& IlcTARGETDigitizer::operator=(const IlcTARGETDigitizer& /*source*/){
    // Assignment operator. This is a function which is not allowed to be
    // done.
    Error("operator=","Assignment operator not allowed\n");
    return *this; 
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
    if(fModActive) delete[] fModActive;
}
//______________________________________________________________________
Bool_t IlcTARGETDigitizer::Init(){
    // Initialization. Set up region of interest, if switched on, and
    // loads TARGET and TARGETgeom.
    // Inputs:
    //      none.
    // Outputs:
    //      none.
    // Return:
    //      none.

    fInit = kTRUE; // Assume for now init will work.
    if(!gIlc) {
	fTARGET      = 0;
	fRoiifile = 0;
	fInit     = kFALSE;
	Warning("Init","gIlc not found");
	return fInit;
    } // end if
    fTARGET = (IlcTARGET *)(gIlc->GetDetector("TARGET"));
    if(!fTARGET){
	fRoiifile = 0;
	fInit     = kFALSE;
	Warning("Init","TARGET not found");
	return fInit;
    } else if(fTARGET->GetTARGETgeom2()){
	//cout << "fRoif,fRoiifile="<<fRoif<<" "<<fRoiifile<<endl;
	fModActive = new Bool_t[fTARGET->GetTARGETgeom2()->GetIndexMax()];
    } else{
	fRoiifile = 0;
	fInit     = kFALSE;
	Warning("Init","TARGET geometry not found");
	return fInit;
    } // end if
    // fModActive needs to be set to a default all kTRUE value
    for(Int_t i=0;i<fTARGET->GetTARGETgeom2()->GetIndexMax();i++) fModActive[i] = kTRUE;
    return fInit;
}
//______________________________________________________________________
void IlcTARGETDigitizer::Digitize(Option_t* opt){
    // Main digitization function. 
    // Inputs:
    //      Option_t * opt   list of sub detector to digitize. =0 all.
    // Outputs:
    //      none.
    // Return:
    //      none.

    char name[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    const char *all;
    opt="SPD SSD";
    const char *det[3] = {strstr(opt,"SPD"),strstr(opt,"SSD"),
                          strstr(opt,"SDD")};
    if( !det[0] && !det[1] && !det[2] ) all = "All";
    else all = 0;
    Int_t nfiles = GetManager()->GetNinputs();
    Int_t event  = GetManager()->GetOutputEventNr();
    IlcTARGETsimulation *sim      = 0;
    if(fFlagFirstEv){
      fTARGET->SetDefaults();    
      fTARGET->SetDefaultSimulation();
      fFlagFirstEv=kFALSE;
    }
    if(!fInit){
	Error("Exec","Init not successful, aborting.");
	return;
    } // end if

    sprintf(name,"%s",fTARGET->GetName());

    Int_t size   = fTARGET->GetTARGETgeom2()->GetIndexMax();
    Int_t module,id,ifiles,mask;
    Bool_t lmod;
    Int_t *fl = new Int_t[nfiles];
    fl[0] = fRoiifile;
    mask = 1;
    for(id=0;id<nfiles;id++) 
     if(id!=fRoiifile)
      {
       // just in case fRoiifile!=0.
        fl[mask] = id;
        mask++;
      } // end for,if
    TClonesArray * sdig = new TClonesArray( "IlcTARGETpListItem",6000 );
    
    TString loadname(name);
    loadname+="Loader";
    
    IlcRunLoader *inRL = 0x0, *outRL = 0x0;
    IlcLoader *ingime = 0x0, *outgime = 0x0;    
    
    outRL = IlcRunLoader::GetRunLoader(fDigInput->GetOutputFolderName());
    if ( outRL == 0x0)
     {
       Error("Exec","Can not get Output Run Loader");
       return;
     }
    outRL->GetEvent(event);
    outgime = outRL->GetLoader(loadname);
    if ( outgime == 0x0)
     {
       Error("Exec","Can not get Output TARGET Loader");
       return;
     }

    outgime->LoadDigits("UPDATE");

    if (outgime->TreeD() == 0x0) outgime->MakeTree("D");

    // Digitize
    fTARGET->MakeBranchInTreeD(outgime->TreeD());
    if(fRoif!=0) Info("IlcTARGETDigitizer","Region of Interest digitization selected");
    else Info("IlcTARGETDigitizer","No Region of Interest selected. Digitizing everything");
    //cout <<"fModActive="<<fModActive<<" fRoif="<<fRoif;
    if(fModActive==0) fRoif = 0; // fModActive array must be define for RIO cuts.
    //cout <<" fRoif="<<fRoif<<endl;

    for(ifiles=0; ifiles<nfiles; ifiles++ )
     {
       inRL =  IlcRunLoader::GetRunLoader(fDigInput->GetInputFolderName(fl[ifiles]));
       ingime = inRL->GetLoader(loadname);
       if (ingime->TreeS() == 0x0) ingime->LoadSDigits();
     }

    for(module=0; module<size; module++ )
     {
//       if (module>95) continue; //Dan Endcap
       if(fModActive && fRoif!=0) if(!fModActive[module]) continue;
       id = fTARGET->GetTARGETgeom2()->GetModuleType(module);
       if(!all && !det[id]) continue;
       sim      = (IlcTARGETsimulation*)fTARGET->GetSimulationModel(id);
       if(!sim) {
            Error( "Exec", "The simulation class was not instanciated!" );
            exit(1);
        } // end if !sim
           // Fill the module with the sum of SDigits
        sim->InitSimulationModule(module, event);
	//cout << "Module=" << module;
        for(ifiles=0; ifiles<nfiles; ifiles++ )
         {
//           cout <<"Entra in ifiles: "<<ifiles<<endl;
           if(fRoif!=0) if(!fModActive[module]) continue;
            
           inRL =  IlcRunLoader::GetRunLoader(fDigInput->GetInputFolderName(fl[ifiles]));
           ingime = inRL->GetLoader(loadname);
           
           TTree *treeS = ingime->TreeS();
           fTARGET->SetTreeAddress();
           
           if( !(treeS && fTARGET->GetSDigits()) ) continue; 
           TBranch *brchSDigits = treeS->GetBranch( name );
           if( brchSDigits ) 
            {
                brchSDigits->SetAddress( &sdig ); 
            } else {
                Error( "Exec", "branch TARGET not found in TreeS, input file %d ",
                       ifiles );
		return;
            } // end if brchSDigits
            sdig->Clear();
            mask = GetManager()->GetMask(ifiles);
            // add summable digits to module
            brchSDigits->GetEvent( module );
            lmod = sim->AddSDigitsToModule(sdig,mask);
            fDigInput->SetRegionOfInterest(kTRUE);
            if(fDigInput->GetRegionOfInterest() && (ifiles==0))
             {
          //  if (ifiles==0)
               fModActive[module] = lmod;
             } // end if
        } // end for ifiles
	//cout << " end ifiles loop" << endl;
        // Digitize current module sum(SDigits)->Digits
        if (fModActive[module]) 
	sim->FinishSDigitiseModule();
        // fills all branches - wasted disk space
        outgime->TreeD()->Fill();
	fTARGET->ResetDigits();
        
    } // end for module
    
    outgime->TreeD()->AutoSave();
    outgime->WriteDigits("OVERWRITE");
    outgime->UnloadDigits();
    for(ifiles=0; ifiles<nfiles; ifiles++ )
     {
       inRL =  IlcRunLoader::GetRunLoader(fDigInput->GetInputFolderName(fl[ifiles]));
       ingime = inRL->GetLoader(loadname);
       ingime->UnloadSDigits();
     }

    delete[] fl;
    sdig->Clear();
    delete sdig;
    for(Int_t i=0;i<fTARGET->GetTARGETgeom2()->GetIndexMax();i++){
//    if (module>95) continue;  //Dan Endcap
    fModActive[i] = kTRUE;
}
    

    return;
}
//______________________________________________________________________
void IlcTARGETDigitizer::SetByRegionOfInterest(TTree *ts){
    // Scans through the TARGET branch of the SDigits tree, ts, for modules
    // which have SDigits in them. For these modules, a flag is set to
    // digitize only these modules. The value of fRoif determines how many
    // neighboring modules will also be turned on. fRoif=0 will turn on only
    // those modules with SDigits in them. fRoif=1 will turn on, in addition,
    // those modules that are +-1 module from the one with the SDigits. And
    // So on. This last feature is not supported yet.
    // Inputs:
    //      TTree *ts  The tree in which the existing SDigits will define the
    //                 region of interest.
    // Outputs:
    //      none.
    // Return:
    //      none.
    Int_t m,nm,i;

    if(fRoif==0) return;
    if(ts==0) return;
    TBranch *brchSDigits = ts->GetBranch(fTARGET->GetName());
    TClonesArray * sdig = new TClonesArray( "IlcTARGETpListItem",1000 );
    //cout << "Region of Interest ts="<<ts<<" brchSDigits="<<brchSDigits<<" sdig="<<sdig<<endl;

    if( brchSDigits ) {
      brchSDigits->SetAddress( &sdig );
    } else {
      Error( "SetByRegionOfInterest","branch TARGET not found in TreeS");
      return;
    } // end if brchSDigits

    nm = fTARGET->GetTARGETgeom2()->GetIndexMax();
    for(m=0;m<nm;m++){
      //cout << " fModActive["<<m<<"]=";
      fModActive[m] = kFALSE; // Not active by default
      sdig->Clear();
      brchSDigits->GetEvent(m);
      if(sdig->GetLast()>=0) for(i=0;i<sdig->GetLast();i++){
          // activate the necessary modules
        if(((IlcTARGETpList*)sdig->UncheckedAt(m))->GetpListItem(i)->GetSignal()>0.0){ // Must have non zero signal.
            fModActive[m] = kTRUE;
            break;
          } // end if
      } // end if. end for i.
      //cout << fModActive[m];
      //cout << endl;
    } // end for m
    Info("IlcTARGETDigitizer","Digitization by Region of Interest selected");
    sdig->Clear();
    delete sdig;
    return;
}
