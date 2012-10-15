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

/* $Id: IlcTARGET.cxx,v 1.6 2009/02/19 10:57:09 cgatto Exp $ */


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//      An overview of the basic philosophy of the TARGET code development      //
// and analysis is show in the figure below.                                 //
//Begin_Html                                                                 //
/*                                               
<img src="picts/TARGET/TARGET_Analysis_schema.gif">
</pre>
<br clear=left>
<font size=+2 color=red>
<p>Roberto Barbera is in charge of the TARGET Offline code (1999).
<a href="mailto:roberto.barbera@ct.infn.it">Roberto Barbera</a>.
</font>
<pre>
*/
//End_Html
//
//  IlcTARGET. Inner Traking System base class.
//  This class contains the base procedures for the Inner Tracking System
//
//Begin_Html
/*
<img src="picts/TARGET/IlcTARGET_Class_Diagram.gif">
</pre>
<br clear=left>
<font size=+2 color=red>
<p>This show the class diagram of the different elements that are part of
the IlcTARGET class.
</font>
<pre>
*/
//End_Html
//
// Version: 0
// Written by Rene Brun, Federico Carminati, and Roberto Barbera
//
// Version: 1
// Modified and documented by Bjorn S. Nilsen
// July 11 1999
//
// Version: 2
// Modified and documented by A. Bologna
// October 18 1999
//
// IlcTARGET is the general base class for the TARGET. Also see IlcDetector for
// futher information.
//
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <TParticle.h>
#include <TString.h>
#include <TTree.h>
#include <TVirtualMC.h>
// #include "IlcMemoryWatcher.h"
#include "IlcDetector.h"
#include "IlcTARGET.h"
#include "IlcTARGETDetTypeSim.h"
#include "IlcTARGETDDLRawData.h"
#include "IlcTARGETLoader.h"
#include "IlcTARGEThit.h"
#include "IlcTARGETmodule.h"
#include "IlcTARGETpListItem.h"
#include "IlcTARGETsimulation.h"
#include "IlcTARGETsegmentation2.h"
#include "IlcTARGETsimulationFastPoints.h"
#include "IlcMC.h"
#include "IlcTARGETDigitizer.h"
#include "IlcTARGETRecPoint.h"
#include "IlcRun.h"
#include "IlcLog.h"


#include <TArrayD.h>
#include <TArrayF.h>
#include <IlcTARGETgeomSPD.h>
#include <IlcTARGETgeomSDD.h>
#include <IlcTARGETgeomSSD.h>
#include <TGeoManager.h>
#include <TGeoVolume.h>
#include <TGeoShape.h>
#include <TGeoBBox.h>
#include <TGeoTrd1.h>
#include <TGeoTrd2.h>
#include <TGeoArb8.h>
#include <TGeoTube.h>
#include <TGeoCone.h>
#include <TGeoSphere.h>
#include <TGeoPara.h>
#include <TGeoPgon.h>
#include <TGeoPcon.h>
#include <TGeoEltu.h>
#include <TGeoHype.h>
#include "IlcTARGETgeom.h"



ClassImp(IlcTARGET)

//______________________________________________________________________
IlcTARGET::IlcTARGET() : IlcDetector(){
  // Default initializer for TARGET
  //      The default constructor of the IlcTARGET class. In addition to
  // creating the IlcTARGET class it zeros the variables fIshunt (a member
  // of IlcDetector class), fEuclidOut, and fIdN, and zeros the pointers
  // fIdSens, and fIdName. The IlcDetector default constructor
  // is also called.
  
  fBackfile=0;
  fNBeemBrunchs=1;
  fEuclidOut=0;
  fOpt="SPD SSD";
  fIdSens=0;
  fIdName=0;
  fDetTypeSim=0;
  fIshunt     = 0;   // not zeroed in IlcDetector.
  fHits =0;
  fNhits=0;
  fTARGETmodules=0;
  fGDMLTARGETGeometry   = kFALSE;
  fMinorVersion = -1;
  
 
// // // //      SetMarkerColor(kRed);
// // // //      SetMarkerStyle(6); //default is 1
}
//______________________________________________________________________
IlcTARGET::IlcTARGET(const char *name, const char *title,Int_t MinorVersion):IlcDetector(name,title){
  //     The standard Constructor for the TARGET class. 
  // It also zeros the variables
  // fIshunt (a member of IlcDetector class), fEuclidOut, and zeros
  // the pointers fIdSens and fIdName. To help in displaying hits via the
  // ROOT macro display.C IlcTARGET also sets the marker color to red. The
  // variables passes with this constructor, const char *name and *title,
  // are used by the constructor of IlcDetector class. See IlcDetector
  // class for a description of these parameters and its constructor
  // functions.
  fMinorVersion = MinorVersion;
  if (fMinorVersion==1) fgkNTYPES=1;
  if (fMinorVersion==2) fgkNTYPES=1;
  if (fMinorVersion==3) fgkNTYPES=2;
  
  fEuclidOut=0;
  fOpt="SPD SSD";
  fIdSens=0;
  fIdName=0;
  fDetTypeSim=0;
  fHits = new TClonesArray("IlcTARGEThit",1560);
  if(gIlc->GetMCApp()) gIlc->GetMCApp()->AddHitList(fHits);
  fNhits=0;
  fTARGETmodules = 0;

  fIshunt     = 0;  // not zeroed in IlcDetector
  // Not done in IlcDetector.

  fEuclidOut  = 0;
  SetDetectors(); // default to fOpt="All". This variable not written out.
  
  fIdName     = 0;
  fIdSens     = 0;
  fDetTypeSim   = new IlcTARGETDetTypeSim();
  fDetTypeSim->SetDetVersion(MinorVersion);
  fDetTypeSim->Init();
  fGDMLTARGETGeometry   = kFALSE;
 
// // // //   SetMarkerColor(kRed);
// // // //   SetMarkerStyle(6); //default is 1

  fBackfile=new TString("");
  fNBeemBrunchs=1;
}
//______________________________________________________________________
IlcTARGET::~IlcTARGET(){
    // Default destructor for TARGET.
    //     The default destructor of the IlcTARGET class. In addition to deleting
    // the IlcTARGET class it deletes the memory pointed to by 
    // fIdSens, fIdName, fDetTypeSim and it's contents.
    // Inputs:
    //      none.
    // Outputs:
    //      none.
    // Return:
    //      none.
    if (fHits) {
      fHits->Delete();
      delete fHits;
      fHits=0;
    }
    if(fTARGETmodules) {
        this->ClearModules();
        delete fTARGETmodules;
	fTARGETmodules = 0;
    }// end if fTARGETmodules!=0

    delete[] fIdName;  // Array of TStrings
    delete[] fIdSens;

    if (fDetTypeSim){
      delete fDetTypeSim;
      fDetTypeSim = 0;
    }
    if (fBackfile) {
      delete fBackfile;
      fBackfile=0;
    }
}

// // // // //______________________________________________________________________
// // // // IlcTARGET::IlcTARGET(const IlcTARGET &source) : IlcDetector(source){
// // // //     // Copy constructor. This is a function which is not allowed to be
// // // //     // done to the TARGET. It exits with an error.
// // // //     // Inputs:
// // // //     //      IlcTARGET &source  An IlcTARGET class.
// // // //     // Outputs:
// // // //     //      none.
// // // //     // Return:
// // // //     //      none.
// // // // 
// // // //     if(this==&source) return;
// // // //     Error("Copy constructor",
// // // //           "You are not allowed to make a copy of the IlcTARGET");
// // // //     exit(1);
// // // // }

//______________________________________________________________________
IlcTARGET& IlcTARGET::operator=(const IlcTARGET &source){
    // Assignment operator. This is a function which is not allowed to be
    // done to the TARGET. It exits with an error.
    // Inputs:
    //      IlcTARGET &source  An IlcTARGET class.
    // Outputs:
    //      none.
    // Return:
    //      none.

    if(this==&source) return *this;
    Error("operator=","You are not allowed to make a copy of the IlcTARGET");
    exit(1);
    return *this; //fake return
}
//______________________________________________________________________
IlcDigitizer* IlcTARGET::CreateDigitizer(IlcDigitizationInput* manager)const{
    // Creates the IlcTARGETDigitizer in a standard way for use via IlcModule.
    // This function can not be included in the .h file because of problems
    // with the order of inclusion (recursive).
    // Inputs:
    //    IlcRunDigitizer *manager  The Manger class for Digitization
    // Output:
    //    none.
    // Return:
    //    A new IlcTARGETRunDigitizer (cast as a IlcDigitizer).
    


     return new IlcTARGETDigitizer(manager);
}
//______________________________________________________________________
void IlcTARGET::Init(){
    // Initializer TARGET after it has been built
    //     This routine initializes the IlcTARGET class. It is intended to be
    // called from the Init function in IlcTARGETv?. Besides displaying a banner
    // indicating that it has been called it initializes the array fIdSens
    // and sets the default segmentation, response, digit and raw cluster
    // classes therefore it should be called after a call to CreateGeometry.
    // Inputs:
    //      none.
    // Outputs:
    //      none.
    // Return:
    //      none.
    
    
    SetDefaults(); 
    WriteGeometry();
}

//______________________________________________________________________
void IlcTARGET::WriteGeometry(){
  
  //Writes TARGET geomtry on gIlc
  


  if(!fLoader) MakeLoader(IlcConfig::GetDefaultEventFolderName());
  IlcRunLoader* rl  = fLoader->GetRunLoader();
  rl->CdGAFile();
  IlcTARGETgeom* geom2 = GetTARGETgeom2();
  geom2->SetDetVersion(GetMinorVersion());
  geom2->Write();


}
//_____________________________________________________________________________
void IlcTARGET::ImportGDMLMaterial()

{



  const char* name=" ";
  Int_t imat=0;
//
  Int_t kmat;
  TString uniquename = GetName();
  uniquename.Append("_"); 
  uniquename.Append(name);
  TGeoMaterial *mat = gGeoManager->GetMaterial(uniquename.Data());
    kmat = mat->GetUniqueID();
    (*fIdmate)[imat]=kmat;
}
//_____________________________________________________________________________

void IlcTARGET::ImportGDMLGeometry()

{

    
    gSystem->Load("libGeom");
    if (!gGeoManager) { 
      if(const char *geomfilename=gSystem->ExpandPathName(GetGDMLGeometryFileName())) {
      IlcInfo(Form("Loading geometry from file:\n %40s\n\n",geomfilename));
      TGeoManager::Import(geomfilename);
    }else{
      IlcInfo(Form("Geometry file %40s not found!\n",geomfilename));
      return;
     }
    }
}
//______________________________________________________________________
void IlcTARGET::SetDefaults(){
    // sets the default segmentation, response, digit and raw cluster classes.
    // Inputs:
    //      none.
    // Outputs:
    //      none.
    // Return:
    //      none.


    
    IlcInfoClass("Setting Defaults");

    if(!fDetTypeSim) { 
     Error("SetDefaults()","fDetTypeSim is 0!"); 
     return;
    }
    fDetTypeSim->SetDefaults();


}
//______________________________________________________________________
void IlcTARGET::SetDefaultSimulation(){
    // sets the default simulation.
    // Inputs:
    //      none.
    // Outputs:
    //      none.
    // Return:
    //      none.
    

    
    if(!fDetTypeSim) { 
     Error("SetDefaultSimulation()","fDetTypeSim is 0!"); 
     return;
    }
    fDetTypeSim->SetDefaultSimulation();

}


//______________________________________________________________________
void IlcTARGET::MakeBranch(Option_t* option){
    // Creates Tree branches for the TARGET.
    // Inputs:
    //      Option_t *option    String of Tree types S,D, and/or R.
    //      const char *file    String of the file name where these branches
    //                          are to be stored. If blank then these branches
    //                          are written to the same tree as the Hits were
    //                          read from.
    // Outputs:
    //      none.
    // Return:
    //      none.
    
    
  if(!fDetTypeSim) {
    Error("MakeBranch","fDetTypeSim is 0!");
    return;
  }

  Bool_t cH = (strstr(option,"H")!=0);
  Bool_t cS = (strstr(option,"S")!=0);
  Bool_t cD = (strstr(option,"D")!=0);
  
  if(cH && (fHits == 0x0)) fHits = new TClonesArray("IlcTARGEThit", 1560);
  IlcDetector::MakeBranch(option);
  
  if(cS) MakeBranchS(0);
  if(cD) MakeBranchD(0);


}
//___________________________________________________________________
void IlcTARGET::MakeBranchS(const char* fl){

  // Creates Tree Branch for the TARGET summable digits.
  // Inputs:
  //      cont char *fl  File name where SDigits branch is to be written
  //                     to. If blank it write the SDigits to the same
  //                     file in which the Hits were found.

  

  
  if(!fDetTypeSim){
    Error("MakeBranchS","fDetTypeSim is 0!");
  }
  Int_t buffersize = 4000;
  char branchname[30];

  // only one branch for SDigits.
  sprintf(branchname,"%s",GetName());

  if(fLoader->TreeS()){
    if(fDetTypeSim->GetSDigits()==0x0) fDetTypeSim->SetSDigits(new TClonesArray("IlcTARGETpListItem",1000));
    TClonesArray* sdig = (TClonesArray*)fDetTypeSim->GetSDigits();
    MakeBranchInTree(fLoader->TreeS(),branchname,&sdig,buffersize,fl);
  } 
}
//______________________________________________________________________
void IlcTARGET::MakeBranchD(const char* file){

  //Make branch for digits
  

  
  if(!fDetTypeSim) {
    Warning("MakeBranchD","fDetTypeSim is 0!");
    return;
  }
  fDetTypeSim->SetLoader(fLoader);
  MakeBranchInTreeD(fLoader->TreeD(),file);
}

//___________________________________________________________________
void IlcTARGET:: MakeBranchInTreeD(TTree* treeD, const char* file){
  // Creates Tree branches for the TARGET.
  


  if(!fDetTypeSim){
    Error("MakeBranchS","fDetTypeSim is 0!");
  }
  fDetTypeSim->SetLoader(fLoader);

  const Char_t *det[3] = {"SPD","SSD","SDD"};
  Char_t* digclass;
  Int_t buffersize = 4000;
  Char_t branchname[30];
  
  if(!fDetTypeSim->GetDigits()){
    fDetTypeSim->SetDigits(new TObjArray(fgkNTYPES));
  }
  for(Int_t i=0;i<fgkNTYPES;i++){
    digclass = fDetTypeSim->GetDigitClassName(i);
    TString classn = digclass;
    if(!((fDetTypeSim->GetDigits())->UncheckedAt(i))){
      (fDetTypeSim->GetDigits())->AddAt(new TClonesArray(classn.Data(),1000),i);
    }
    else ResetDigits(i);  
    if(fgkNTYPES==3) sprintf(branchname,"%sDigits%s",GetName(),det[i]);
    else sprintf(branchname,"%sDigits%d",GetName(),i+1);
    TObjArray* dig = DigitsAddress(i);
    if(GetDigits() && treeD) IlcDetector::MakeBranchInTree(treeD,branchname, &dig,buffersize,file);
  }

}
//______________________________________________________________________
void IlcTARGET::SetTreeAddress(){
    // Set branch address for the Trees.
    // Inputs:
    //      none.
    // Outputs:
    //      none.
    // Return:
    //      none.
    

  if(!fDetTypeSim) {
    Error("SetTreeAddress","fDetTypeSim is 0!");
    return;
  }

  fDetTypeSim->SetLoader(fLoader);

  TTree *treeS = fLoader->TreeS();
  TTree *treeD = fLoader->TreeD();
  if (fLoader->TreeH() && (fHits == 0x0)) {
      fHits = new TClonesArray("IlcTARGEThit", 1560);
  }
  IlcDetector::SetTreeAddress();

  fDetTypeSim->SetTreeAddressS(treeS, (Char_t*)GetName());
  fDetTypeSim->SetTreeAddressD(treeD, (Char_t*)GetName());


}
//______________________________________________________________________
void IlcTARGET::AddHit(Int_t track, Int_t *vol, Float_t *hits){
    // Add an TARGET hit
    //     The function to add information to the IlcTARGEThit class. See the
    // IlcTARGEThit class for a full description. This function allocates the
    // necessary new space for the hit information and passes the variable
    // track, and the pointers *vol and *hits to the IlcTARGEThit constructor
    // function.
    // Inputs:
    //      Int_t   track   Track number which produced this hit.
    //      Int_t   *vol    Array of Integer Hit information. See IlcTARGEThit.h
    //      Float_t *hits   Array of Floating Hit information.  see IlcTARGEThit.h
    // Outputs:
    //      none.
    // Return:
    //      none.
    
    
  TClonesArray &lhits = *fHits;
  new(lhits[fNhits++]) IlcTARGEThit(fIshunt,track,vol,hits);
}

//______________________________________________________________________
void IlcTARGET::FillModules(Int_t evnt,Int_t bgrev,Int_t nbunchs,Int_t nmodules,
                         Option_t *option, const char *filename){
  // fill the modules with the sorted by module hits; add hits from
  // background if option=Add.
  
  
  

  static TTree *trH1=0;                 //Tree with background hits
  static Bool_t first=kTRUE;
  static TFile *file;
  const char *addBgr = strstr(option,"ADD");
  
  evnt = nmodules; // Dummy use of variables to remove warnings
  FillModules(fLoader->TreeH(),0); // fill from this file's tree.
  
  if (addBgr ) {
    if(first) {
      file=new TFile(gSystem->ExpandPathName(filename));
      cout<<"Open background file "<<gSystem->ExpandPathName(filename)<<endl;
    } // end if first
    first=kFALSE;
    file->cd();
    // Get Hits Tree header from file
    
    char treeName[20];
    for(int i=0;i<nbunchs;i++){
      if(trH1) delete trH1;
      trH1=0;
      sprintf(treeName,"Event%d/TreeH",bgrev+i);
      cout <<"background treeName:" <<treeName<<endl;
      trH1 = (TTree*)gDirectory->Get(treeName);
      if (!trH1) {
	Error("FillModules","cannot find background Hits Tree for event:%d",bgrev+i);
      }else{ // end if add
	FillModules(trH1,10000000*(i+1)); // Default mask 10M.
      }
    }
  }
  
}


//______________________________________________________________________
void IlcTARGET::FillModules(TTree *treeH, Int_t mask) {
    // fill the modules with the sorted by module hits; 
    // can be called many times to do a merging
    // Inputs:
    //      TTree *treeH  The tree containing the hits to be copied into
    //                    the modules.
    //      Int_t mask    The track number mask to indicate which file
    //                    this hits came from.
    // Outputs:
    //      none.
    // Return:
    //      none.
    

    if (treeH == 0x0)
     {
       Error("FillModules","Tree is NULL");
     }
    Int_t lay,lad,det,index;
    IlcTARGEThit *itsHit=0;
    IlcTARGETmodule *mod=0;
    char branchname[20];
    sprintf(branchname,"%s",GetName());
    TBranch *branch = treeH->GetBranch(branchname);
    
    if (!branch) {
        Error("FillModules","%s branch in TreeH not found",branchname);
        return;
    } // end if !branch
    branch->SetAddress(&fHits);
    Int_t nTracks =(Int_t) treeH->GetEntries();
    
    if(IlcDebugLevel()>0) IlcDebug(1, Form("nTracks = %d", nTracks));
    
    Int_t iPrimTrack,h;
    for(iPrimTrack=0; iPrimTrack<nTracks; iPrimTrack++){
        ResetHits();
        Int_t nBytes = treeH->GetEvent(iPrimTrack);
        if (nBytes <= 0) continue;
        Int_t nHits = fHits->GetEntriesFast();
	
	if(IlcDebugLevel()>0) IlcDebug(1, Form("nHits = %d", nHits));
	
        for(h=0; h<nHits; h++){
            itsHit = (IlcTARGEThit *)fHits->UncheckedAt(h);
            itsHit->GetDetectorID(lay,lad,det);
	    
	    
	    if(IlcDebugLevel()>0) 
	      IlcDebug(1, Form("hit %d in layer %d, ladder %d detector %d", h, lay, lad, det));
	    
	    if(dynamic_cast<IlcTARGETgeomSSD*> (GetTARGETgeom2()->GetShape2(lay-1))){
	      if(int(mask/10000000)>1) {
		IlcDebug(1,Form(" Skip branchcrossing>1 forr SSD lay=%d",lay));
		continue;
	      }
	    }

            if (GetTARGETgeom2()) {
                index = GetTARGETgeom2()->GetModuleIndex(lay,lad,det);
		
		if(IlcDebugLevel()>0) IlcDebug(1, Form("index = %d", index));
            } else {
                index=det-1; // This should not be used.
            } // end if [You must have fTARGETgeom for this to work!]
	    
            mod = GetModule(index);
	    
            itsHit->SetTrack(itsHit->GetTrack()+mask); // Set track mask.
	    
	    if(IlcDebugLevel()>0) IlcDebug(1, Form(" PrimTrack %d, hit %d", iPrimTrack, h));
	    
            mod->AddHit(itsHit,iPrimTrack,h);
	    
        } // end loop over hits 
    } // end loop over tracks
}

//______________________________________________________________________
void IlcTARGET::InitModules(Int_t size,Int_t &nmodules){
    // Initialize the modules array.
    // Inputs:
    //      Int_t size  Size of array of the number of modules to be
    //                  created. If size <=0 then the number of modules
    //                  is gotten from IlcTARGETgeom class kept in fTARGETgeom.
    // Outputs:
    //      Int_t &nmodules The number of modules existing.
    // Return:
    //      none.
    
    

    if(fTARGETmodules){ 
        fTARGETmodules->Delete();
        delete fTARGETmodules;
    } // end fir fTARGETmoudles

    if(!fDetTypeSim) {
      Error("InitModules","fDetTypeSim is null!");
      return;
    }

    Int_t indexMAX,index;

    if(size<=0){ // default to using data stored in IlcTARGETgeom
        if(fDetTypeSim->GetTARGETgeom2()==0) {
            Error("InitModules","fTARGETgeom not defined");
            return;
        } // end if fTARGETgeom==0
        
	
        indexMAX = fDetTypeSim->GetTARGETgeom2()->GetIndexMax();        
	nmodules = indexMAX;
	IlcDebug(1, Form("Nmodules=%d", nmodules));
	
        fTARGETmodules = new TObjArray(indexMAX);
        for(index=0;index<indexMAX;index++){
            fTARGETmodules->AddAt( new IlcTARGETmodule(index),index);
        } // end for index
    }else{
        fTARGETmodules = new TObjArray(size);
        for(index=0;index<size;index++) {
            fTARGETmodules->AddAt( new IlcTARGETmodule(index),index);
        } // end for index

        nmodules = size;
	
	IlcDebug(1, Form("Size=%d", size));
	
    } // end i size<=0
}
//______________________________________________________________________
void IlcTARGET::Hits2SDigits(){
    // Standard Hits to summable Digits function.
    // Inputs:
    //      none.
    // Outputs:
    //      none.
    
  
   //IlcTARGETgeom* geom = CreateIlcTARGETgeom();
   //SetTARGETgeom(geom);
   if(!fDetTypeSim) {
     Error("Hits2SDigits","fDetTypeSim is null!");
     return;
    
  } 
  SetDefaults();
  fLoader->LoadHits("read");
  fLoader->LoadSDigits("recreate");
  IlcRunLoader* rl = fLoader->GetRunLoader(); 
  fDetTypeSim->SetLoader(fLoader);
  for (Int_t iEvent = 0; iEvent < rl->GetNumberOfEvents(); iEvent++) {
  IlcInfo(Form("SDigitization: Processing event %d ...\n",iEvent));
  fflush(stdout);
        // Do the Hits to Digits operation. Use Standard input values.
        // Event number from file, no background hit merging , use size from
        // IlcTARGETgeom class, option="All", input from this file only.
    rl->GetEvent(iEvent);
    if (!fLoader->TreeS()) fLoader->MakeTree("S");
    MakeBranch("S");
    SetTreeAddress();
    if (GetBackgroundFile()->IsNull())
      HitsToPreDigits(iEvent,0,0,-1," ","SPD SSD"," ");
    else
      {
       Int_t first = 0;
       if (gSystem->Getenv("CONFIG_FIRSTBG"))
        first = atoi(gSystem->Getenv("CONFIG_FIRSTBG"));
       Info("Hits2SDigits","merging bunch crossing from: %d to : %d",
	    first+iEvent*fNBeemBrunchs,first+(iEvent+1)*fNBeemBrunchs-1);
       HitsToPreDigits(iEvent,first+iEvent*fNBeemBrunchs,fNBeemBrunchs,-1,"ADD","SPD SSD",GetBackgroundFile()->Data());
      }
    IlcInfo(Form("SDigitization: ... Event %d terminated. \n",iEvent));
    fflush(stdout);

  } // end for iEvent
    
  fLoader->UnloadHits();
  fLoader->UnloadSDigits();

}
//______________________________________________________________________
void IlcTARGET::Hits2Digits(){




  //Conversion from hits to digits
  if(!fDetTypeSim) {
    Error("Hits2SDigits","fDetTypeSim is 0!");
    return;
  }
   
  fDetTypeSim->SetLoader(fLoader);
   SetDefaults();

  fLoader->LoadHits("read");
  fLoader->LoadDigits("recreate");
  IlcRunLoader* rl = fLoader->GetRunLoader(); 
  for (Int_t iEvent = 0; iEvent < rl->GetNumberOfEvents(); iEvent++) {
    rl->GetEvent(iEvent);
    if (!fLoader->TreeD()) fLoader->MakeTree("D");
    MakeBranch("D");
    SetTreeAddress();   
    HitsToDigits(iEvent,0,0,-1," ","SPD SSD"," ");
  } 
  
  fLoader->UnloadHits();
  fLoader->UnloadDigits();
  
}

//______________________________________________________________________
void IlcTARGET::HitsToDigits(Int_t evNumber,Int_t bgrev,Int_t nbunchs,Int_t size,
                          Option_t *option,Option_t *opt,
                          const char *filename){
    //   Keep gilc.root for signal and name differently the file for 
    // background when add! otherwise the track info for signal will be lost !
    // the condition below will disappear when the geom class will be
    // initialized for all versions - for the moment it is only for v5 !
    // 7 is the SDD beam test version.
    // Inputs:
    //      Int_t evnt       Event to be processed.
    //      Int_t bgrev      Background Hit tree number.
    //      Int_t nbunchs    Number of bunch crossings.
    //      Int_t nmodules   Not used.
    //      Option_t *option String indicating if merging hits or not. To
    //                       merge hits set equal to "Add". Otherwise no
    //                       background hits are considered.
    //      Test_t *filename File name containing the background hits..
    // Outputs:
    //      none.
    // Return:
    //      none.
    

  if(!fDetTypeSim) {
    Error("HitsToDigits","fDetTypeSim is null!");
    return;
  }
  fDetTypeSim->SetLoader(fLoader);
  if(!GetTARGETgeom2()) return; // need transformations to do digitization.
  IlcTARGETgeom *geom = GetTARGETgeom2();

  const char *all = strstr(opt,"All");
  const char *det[3] = {strstr(opt,"SPD"),strstr(opt,"SSD"),
			strstr(opt,"SDD")};
  static Bool_t setDef=kTRUE;
  if (setDef) SetDefaultSimulation();
  setDef=kFALSE;
  
  Int_t nmodules;
  InitModules(size,nmodules);
  FillModules(evNumber,bgrev,nbunchs,nmodules,option,filename);

  IlcTARGETsimulation *sim      = 0;
  IlcTARGETmodule     *mod      = 0;
  Int_t id;
  for(Int_t module=0;module<geom->GetIndexMax();module++){
    id       = geom->GetModuleType(module);
    if (!all && !det[id]) continue;
    sim      = (IlcTARGETsimulation*)fDetTypeSim->GetSimulationModel(id);
    if (!sim) {
      Error("HitsToDigits","The simulation class was not "
	    "instanciated for module %d type %s!",module,
	    geom->GetModuleTypeName(module));
      exit(1);
    } // end if !sim
    mod      = (IlcTARGETmodule *)fTARGETmodules->UncheckedAt(module);
    sim->DigitiseModule(mod,module,evNumber);
    // fills all branches - wasted disk space
    fLoader->TreeD()->Fill(); 
    ResetDigits();
  } // end for module
  
  ClearModules();
  
  fLoader->TreeD()->GetEntries();
  fLoader->TreeD()->AutoSave();
  // reset tree
  fLoader->TreeD()->Reset();
}
//_____________________________________________________________________
void IlcTARGET::Hits2PreDigits(){ 
  // Turn hits into SDigits

  if(!fDetTypeSim) {
    Error("Hits2SDigits","fDetTypeSim is 0!");
    return;
  }
   
  fDetTypeSim->SetLoader(fLoader);
  SetDefaults();
  HitsToPreDigits(fLoader->GetRunLoader()->GetEventNumber(),
		  0,0,-1," ","SPD SSD"," "); 
}

//______________________________________________________________________
void IlcTARGET::HitsToPreDigits(Int_t evNumber,Int_t bgrev,Int_t nbunchs,Int_t size,
                             Option_t *option,Option_t *opt,
                             const char *filename){
    //   Keep gilc.root for signal and name differently the file for 
    // background when add! otherwise the track info for signal will be lost !
    // the condition below will disappear when the geom class will be
    // initialized for all versions - for the moment it is only for v5 !
    // 7 is the SDD beam test version.
    // Inputs:
    //      Int_t evnt       Event to be processed.
    //      Int_t bgrev      Background Hit tree number.
    //      Int_t nbunchs    Number of bunch crossings.
    //      Int_t nmodules   Not used.
    //      Option_t *option String indicating if merging hits or not. To
    //                       merge hits set equal to "Add". Otherwise no
    //                       background hits are considered.
    //      Test_t *filename File name containing the background hits..
    // Outputs:
    //      none.
    // Return:
    //      none.

  if(!fDetTypeSim) {
    Error("HitsToPreDigits","fDetTypeSim is null!");
    return;
  }
  fDetTypeSim->SetLoader(fLoader);
  // IlcTARGETLoader *pTARGETloader = (IlcTARGETLoader*)fLoader;



  if(!GetTARGETgeom2()){
    Error("HitsToPreDigits","fGeom is null!");
    return; // need transformations to do digitization.
  }
  IlcTARGETgeom *geom = GetTARGETgeom2();

  const char *all = strstr(opt,"All");
  const char *det[3] = {strstr(opt,"SPD"),strstr(opt,"SSD"),
			strstr(opt,"SDD")};
  static Bool_t setDef=kTRUE;
  if (setDef) SetDefaultSimulation();
  setDef=kFALSE;
  Int_t nmodules;
  InitModules(size,nmodules);
  FillModules(evNumber,bgrev,nbunchs,nmodules,option,filename);


  IlcTARGETsimulation *sim      = 0;
  IlcTARGETmodule     *mod      = 0;
  Int_t id,module;
  for(module=0;module<geom->GetIndexMax();module++){
    mod      = (IlcTARGETmodule *)fTARGETmodules->UncheckedAt(module);
    id       = geom->GetModuleType(module);
    if (!all && !det[id]) continue;
    sim      = (IlcTARGETsimulation*)GetSimulationModel(id);
    if (!sim) {
      Error("HitsToPreDigits","The simulation class was not "
	    "instanciated for module %d type %s!",module,
	    geom->GetModuleTypeName(module));
      exit(1);
    } // end if !sim
    


    sim->SDigitiseModule(mod,module,evNumber);
    // fills all branches - wasted disk space
    fLoader->TreeS()->Fill(); 
    fDetTypeSim->ResetSDigits();
  } // end for module
//HitsToFastRecPointsV2(evNumber,evNumber/10025,-1," ","END"," ");
  ClearModules();
  
  fLoader->TreeS()->GetEntries();
  fLoader->TreeS()->AutoSave();
  fLoader->WriteSDigits("OVERWRITE");
  // reset tree
  fLoader->TreeS()->Reset();
}

//_____________________________________________________________________
void IlcTARGET::HitsToFastRecPoints(Int_t /*evNumber*/,Int_t /*bgrev*/,Int_t /*nbunchs*/,Int_t /*size*/,
				 Option_t */*opt0*/,Option_t *opt1,
                                 const char */*flnm*/){
    // keep gilc.root for signal and name differently the file for 
    // background when add! otherwise the track info for signal will be lost !
    // the condition below will disappear when the geom class will be
    // initialized for all versions - for the moment it is only for v5 !
    // Inputs:
    //      Int_t evnt       Event to be processed.
    //      Int_t bgrev      Background Hit tree number.
    //      Int_t nbunchs    Number of bunch crossings.
    //      Int_t size       Size used by InitModules. See InitModules.
    //      Option_t *opt0   Option passed to FillModules. See FillModules.
    //      Option_t *opt1   String indicating if merging hits or not. To
    //                       merge hits set equal to "Add". Otherwise no
    //                       background hits are considered.
    //      Test_t *flnm     File name containing the background hits..
    // Outputs:
    //      none.
    // Return:
    //      none.


  if(!GetTARGETgeom2()){
    Error("HitsToFastRecPoints","fGeom2 is null!");
    return; // need transformations to do digitization.
  }

  //IlcTARGETgeom* geom = CreateIlcTARGETgeom();
  //SetTARGETgeom(geom);
  IlcTARGETgeom *geom = GetTARGETgeom2();

  IlcTARGETLoader *pTARGETloader = (IlcTARGETLoader*)fLoader;

  const char *all = strstr(opt1,"All");
  const char *det[3] ={strstr(opt1,"SPD"),strstr(opt1,"SSD"),strstr(opt1,"SDD")};

  IlcTARGETsimulation *sim      = 0;
  IlcTARGETmodule     *mod      = 0;
  Int_t id,module;

  TTree *lTR = pTARGETloader->TreeR();
  if(!lTR) {
    pTARGETloader->MakeTree("R");
    lTR = pTARGETloader->TreeR();
  }
  
  TClonesArray* ptarray = new TClonesArray("IlcTARGETRecPoint",1000);
  TBranch* branch = (TBranch*)lTR->Branch("TARGETRecPoints",&ptarray);
  branch->SetAddress(&ptarray);
  //m.b. : this change is nothing but a nice way to make sure
  //the CPU goes up !
  IlcDebug(1,Form("N mod = %d",geom->GetIndexMax()));    
  for(module=0;module<geom->GetIndexMax();module++){
//  if ((module<192) ||(module>7503)) continue;
     
    ptarray->Clear();
    id       = geom->GetModuleType(module); 

    IlcDebug(1, Form("id = %d; index = %d", id,module));
    
   if (!all && !det[id]) continue; 
   
    sim      = (IlcTARGETsimulation*)GetSimulationModel(id);
    if (!sim) {
      Error("HitsToFastPoints","The simulation class was not "
	    "instanciated for module %d type %x!",module,
	    geom->GetModuleTypeName(module));
      exit(1);
    } // end if !sim
    mod      = (IlcTARGETmodule *)fTARGETmodules->UncheckedAt(module);
    sim->CreateFastRecPoints(mod,module,gRandom,ptarray);
    lTR->Fill();
  } // end for module

  //ClearModules();
  fLoader->WriteRecPoints("OVERWRITE");
  delete ptarray;
}
//_____________________________________________________________________

//_____________________________________________________________________
void IlcTARGET::HitsToFastRecPointsV2(Int_t /*evNumber*/,Int_t /*bgrev*/,Int_t /*nbunchs*/,Int_t /*size*/,
				   Option_t */*opt0*/,Option_t *opt1,
				   const char */*flnm*/){
    // keep gilc.root for signal and name differently the file for 
    // background when add! otherwise the track info for signal will be lost !
    // the condition below will disappear when the geom class will be
    // initialized for all versions - for the moment it is only for v5 !
    // Inputs:
    //      Int_t evnt       Event to be processed.
    //      Int_t bgrev      Background Hit tree number.
    //      Int_t nbunchs    Number of bunch crossings.
    //      Int_t size       Size used by InitModules. See InitModules.
    //      Option_t *opt0   Option passed to FillModules. See FillModules.
    //      Option_t *opt1   String indicating if merging hits or not. To
    //                       merge hits set equal to "Add". Otherwise no
    //                       background hits are considered.
    //      Test_t *flnm     File name containing the background hits..
    // Outputs:
    //      none.
    // Return:
    //      none.

  if(!GetTARGETgeom2()){
    Error("HitsToFastRecPoints","fGeom2 is null!");
    return; // need transformations to do digitization.
  }

  //IlcTARGETgeom* geom = CreateIlcTARGETgeom();
  //SetTARGETgeom(geom);
  IlcTARGETgeom *geom = GetTARGETgeom2();

  IlcTARGETLoader *pTARGETloader = (IlcTARGETLoader*)fLoader;
  Bool_t ToCreateFast=kFALSE;
  const char *all = strstr(opt1,"All");
  const char *det[3] ={strstr(opt1,"SPD"),strstr(opt1,"SDD"),strstr(opt1,"SSD")};

  IlcTARGETsimulation *sim      = 0;
  IlcTARGETmodule     *mod      = 0;
  Int_t id,module;

  TTree *lTR = pTARGETloader->TreeR();
  if(!lTR) {
    pTARGETloader->MakeTree("R");
    lTR = pTARGETloader->TreeR();
  }
  
  TClonesArray* ptarray = new TClonesArray("IlcTARGETRecPoint",1000);
  TBranch* branch = (TBranch*)lTR->Branch("TARGETRecPoints",&ptarray);
  branch->SetAddress(&ptarray);
  //m.b. : this change is nothing but a nice way to make sure
  //the CPU goes up !
  IlcDebug(1,Form("N mod = %d",geom->GetIndexMax()));    
  for(module=0;module<geom->GetIndexMax();module++){
//    if (module<=95) continue;//Dan Endcap

    ptarray->Clear();
    id       = geom->GetModuleType(module); 

    IlcDebug(1, Form("id = %d; index = %d", id,module));
    
   if (!all && !det[id]) continue; 
   
    sim      = (IlcTARGETsimulation*)GetSimulationModel(id);
    if (!sim) {
      Error("HitsToFastPoints","The simulation class was not "
	    "instanciated for module %d type %x!",module,
	    geom->GetModuleTypeName(module));
      exit(1);
    } // end if !sim
    mod      = (IlcTARGETmodule *)fTARGETmodules->UncheckedAt(module);
    ToCreateFast=sim->CreateFastRecPoints(mod,module,gRandom,ptarray);
    if (ToCreateFast) lTR->Fill();
  } // end for module

  //ClearModules();
  fLoader->WriteRecPoints("OVERWRITE");
  delete ptarray;
}
//_____________________________________________________________________

Int_t IlcTARGET::Hits2Clusters(TTree *hTree, TTree *cTree) {
  //------------------------------------------------------------
  // This function creates TARGET clusters
  //------------------------------------------------------------
  
  
  if(!GetTARGETgeom2()){
    Error("HitsToPreDigits","fGeom is null!");
    return 1; // need transformations to do digitization.
  }

  //IlcTARGETgeom* geom = CreateIlcTARGETgeom();
  //SetTARGETgeom(geom);
  IlcTARGETgeom *geom=GetTARGETgeom2();

  Int_t mmax=geom->GetIndexMax();

  InitModules(-1,mmax);
  FillModules(hTree,0);

  TClonesArray *points = new TClonesArray("IlcTARGETRecPoint",1000);
  TBranch *branch=cTree->GetBranch("TARGETRecPoints");
  if (!branch) cTree->Branch("TARGETRecPoints",&points);
  else branch->SetAddress(&points);

  IlcTARGETsimulationFastPoints sim;
  Int_t ncl=0;
  for (Int_t m=0; m<mmax; m++) {
    IlcTARGETmodule *mod=GetModule(m);      
    sim.CreateFastRecPoints(mod,m,gRandom,points);      
    ncl+=points->GetEntriesFast();
    cTree->Fill();
    points->Clear();
  }

  Info("Hits2Clusters","Number of found fast clusters : %d",ncl);

  //cTree->Write();

  delete points;
  return 0;
}

//_____________________________________________________________________
void IlcTARGET::CheckLabels(Int_t lab[3]) const {
  //------------------------------------------------------------
  // Tries to find mother's labels
  //------------------------------------------------------------
  

  if(lab[0]<0 && lab[1]<0 && lab[2]<0) return; // In case of no labels just exit

  Int_t ntracks = gIlc->GetMCApp()->GetNtrack();
  for (Int_t i=0;i<3;i++){
    Int_t label = lab[i];
    if (label>=0 && label<ntracks) {
      TParticle *part=(TParticle*)gIlc->GetMCApp()->Particle(label);
      if (part->P() < 0.005) {
	Int_t m=part->GetFirstMother();
	if (m<0) {	
	  continue;
	}
	if (part->GetStatusCode()>0) {
	  continue;
	}
	lab[i]=m;       
      }
    }    
  }
  
}

//______________________________________________________________________
void IlcTARGET::SDigitsToDigits(Option_t *opt){
    // Standard Summable digits to Digits function.
    // Inputs:
    //      none.
    // Outputs:
    //      none.
    
    
    if(!fDetTypeSim) {
      Error("SDigitsToSDigits","fDetTypeSim is 0!");
      return;
    }
   
    fDetTypeSim->SetLoader(fLoader);
    SetDefaults();
    fDetTypeSim->SDigitsToDigits(opt,(Char_t*)GetName());

}

//______________________________________________________________________
void IlcTARGET::ResetDigits(){
    // Reset number of digits and the digits array for the TARGET detector.
    // Inputs:
    //      none.
    // Outputs:
    //      none.
    
    
    if(!fDetTypeSim) {
      Error("ResetDigits","fDetTypeSim is 0!");
      return;
    }
   
    fDetTypeSim->ResetDigits();


}
//______________________________________________________________________
void IlcTARGET::ResetDigits(Int_t branch){
    // Reset number of digits and the digits array for this branch.
    // Inputs:
    //      none.
    // Outputs:
    //      none.
    

    if(!fDetTypeSim) {
      Error("ResetDigits","fDetTypeSim is 0!");
      return;
    }
   
    fDetTypeSim->ResetDigits(branch);

}
//______________________________________________________________________
void IlcTARGET::AddSumDigit(IlcTARGETpListItem &sdig){
    // Adds the a module full of summable digits to the summable digits tree.
    // Inputs:
    //      IlcTARGETpListItem &sdig   SDigit to be added to SDigits tree.
    // Outputs:
    //      none.
    // Return:
    //      none.
    

    if(!fDetTypeSim) {
      Error("AddSumDigit","fDetTypeSim is 0!");
      return;
    }
    fDetTypeSim->AddSumDigit(sdig);
    
}
//______________________________________________________________________
void IlcTARGET::AddRealDigit(Int_t branch, Int_t *digits){
    //   Add a real digit - as coming from data.
    // Inputs:
    //      Int_t id        Detector type number.
    //      Int_t *digits   Integer array containing the digits info. See 
    //                      IlcTARGETdigit.h
    // Outputs:
    //      none.
    // Return:
    //      none.
    

    if(!fDetTypeSim) {
      Error("AddRealDigit","fDetTypeSim is 0!");
      return;
    }
    fDetTypeSim->AddRealDigit(branch,digits);

}
//______________________________________________________________________
void IlcTARGET::AddSimDigit(Int_t branch, IlcTARGETdigit *d){
    //    Add a simulated digit.
    // Inputs:
    //      Int_t id        Detector type number.
    //      IlcTARGETdigit *d  Digit to be added to the Digits Tree. See 
    //                      IlcTARGETdigit.h
    // Outputs:
    //      none.
    // Return:
    //      none.
    

    if(!fDetTypeSim) {
      Error("AddSimDigit","fDetTypeSim is 0!");
      return;
    }
    fDetTypeSim->AddSimDigit(branch,d);

}
//______________________________________________________________________
void IlcTARGET::AddSimDigit(Int_t branch,Float_t phys,Int_t *digits,Int_t *tracks,
                         Int_t *hits,Float_t *charges){
			 
			 
  //   Add a simulated digit to the list.
  // Inputs:
  //      Int_t id        Detector type number.
  //      Float_t phys    Physics indicator. See IlcTARGETdigits.h
  //      Int_t *digits   Integer array containing the digits info. See 
  //                      IlcTARGETdigit.h
  //      Int_t *tracks   Integer array [IlcTARGETdigitS?D::GetNTracks()] 
  //                      containing the track numbers that contributed to
  //                      this digit.
  //      Int_t *hits     Integer array [IlcTARGETdigitS?D::GetNTracks()]
  //                      containing the hit numbers, from IlcTARGETmodule, that
  //                      contributed to this digit.
  //      Float_t *charge Floating point array of the signals contributed
  //                      to this digit by each track.
  // Outputs:
  //      none.
  // Return:
  //      none.

    if(!fDetTypeSim) {
      Error("AddSimDigit","fDetTypeSim is 0!");
      return;
    }
    //cout <<"Sto quasi per aggiungere il digit per SSD"<<endl;
    fDetTypeSim->AddSimDigit(branch,phys,digits,tracks,hits,charges);

}
//______________________________________________________________________
void IlcTARGET::Digits2Raw(){
    // convert digits of the current event to raw data
    


  if(!fDetTypeSim) {
    Error("Digits2Raw","fDetTypeSim is 0!");
    return;
  }
  fDetTypeSim->SetLoader(fLoader);
  SetDefaults();
  fDetTypeSim->GetLoader()->LoadDigits();
  TTree* digits = fDetTypeSim->GetLoader()->TreeD();
  if (!digits) {
      Error("Digits2Raw", "no digits tree");
      return;
  }
  fDetTypeSim->SetTreeAddressD(digits,(Char_t*)GetName());

  IlcTARGETDDLRawData rawWriter;
  //Verbose level
  // 0: Silent
  // 1: cout messages
  // 2: txt files with digits 
  //BE CAREFUL, verbose level 2 MUST be used only for debugging and
  //it is highly suggested to use this mode only for debugging digits files
  //reasonably small, because otherwise the size of the txt files can reach
  //quickly several MB wasting time and disk space.
  rawWriter.SetVerbose(0);
    
  //SILICON PIXEL DETECTOR
  Info("Digits2Raw", "Formatting raw data for SPD");
  rawWriter.RawDataSPD(digits->GetBranch("TARGETDigitsSPD"));
    
  //SILICON DRIFT DETECTOR
  Info("Digits2Raw", "Formatting raw data for SDD");
  rawWriter.RawDataSDD(digits->GetBranch("TARGETDigitsSSD"));
    
  //SILICON STRIP DETECTOR
  Info("Digits2Raw", "Formatting raw data for SSD");
  rawWriter.RawDataSSD(digits->GetBranch("TARGETDigitsSDD"));

  fLoader->UnloadDigits();
  
}




//______________________________________________________________________
IlcLoader* IlcTARGET::MakeLoader(const char* topfoldername){ 
    //builds TARGETgetter (IlcLoader type)
    //if detector wants to use castomized getter, it must overload this method
    

    Info("MakeLoader","Creating IlcTARGETLoader. Top folder is %s.",
         topfoldername);
    fLoader = new IlcTARGETLoader(GetName(),topfoldername);
    return fLoader;
}



//======================================================================

//  Patch to correct the mismatch between TGeo and IlcTARGETgeom
//  during simulation.
//  To be there only for the PDC06 

//======================================================================



//______________________________________________________________________
IlcTARGETgeom* IlcTARGET::CreateIlcTARGETgeom(){
    // Creates and Initilizes the geometry transformation class IlcTARGETgeom
    // to values appropreate to this specific geometry. Now that
    // the segmentation is part of IlcTARGETgeom, the detector
    // segmentations are also defined here.
    // Inputs:
    //   none.
    // Outputs:
    //   none.
    // Return:
    //   A pointer to a new properly inilized IlcTARGETgeom class. If
    //   pointer = 0 then failed to init.
    

    IlcTARGETgeom *geom = new IlcTARGETgeom();
    if(!InitIlcTARGETgeomPPRasymmFMD(geom)){ // Error initilization failed
	delete geom;
	geom = 0;
    } // end if
    return geom;
}

//______________________________________________________________________
Bool_t IlcTARGET::InitIlcTARGETgeomPPRasymmFMD(IlcTARGETgeom *geom){
    // Initilizes the geometry transformation class IlcTARGETgeom
    // to values appropreate to this specific geometry. Now that
    // the segmentation is part of IlcTARGETgeom, the detector
    // segmentations are also defined here.
    // Inputs:
    //   IlcTARGETgeom *geom  A pointer to the IlcTARGETgeom class
    // Outputs:
    //   IlcTARGETgeom *geom  This pointer recreated and properly inilized.
    // Return:
    //   none.
  //    const Double_t kcm2micron = 1.0E4;
  
  
    const Int_t kItype=0; // Type of transormation defined 0=> Geant
    const Int_t klayers = 6; // number of layers in the TARGET
    const Int_t kladders[klayers]   = {20,40,14,22,34,38}; // Number of ladders
    const Int_t kdetectors[klayers] = {4,4,6,8,22,25};// number of detector/lad
    const IlcTARGETDetector kIdet[6]   = {kSPD,kSPD,kSDD,kSDD,kSSD,kSSD};
    const TString kPathbase = "/ILCC_1/TARGETV_1/TARGETD_1/";
    const TString kNames[2][klayers] = {
	{"%sIT12_1/I12A_%d/I10A_%d/I103_%d/I101_1/TARGET1_1", // lay=1
	 "%sIT12_1/I12A_%d/I20A_%d/I1D3_%d/I1D1_1/TARGET2_1", // lay=2
	 "%sIT34_1/I004_%d/I302_%d/TARGET3_%d/", // lay=3
	 "%sIT34_1/I005_%d/I402_%d/TARGET4_%d/", // lay=4
	 "%sIT56_1/I565_%d/I562_%d/TARGET5_%d/", // lay=5
	 "%sIT56_1/I569_%d/I566_%d/TARGET6_%d/"},// lay=6
	{"%sIT12_1/I12B_%d/I10B_%d/I107_%d/I101_1/TARGET1_1", // lay=1
	 "%sIT12_1/I12B_%d/I20B_%d/I1D7_%d/I1D1_1/TARGET2_1", // lay=2
	 "%sIT34_1/I004_%d/I302_%d/TARGET3_%d", // lay=3
	 "%sIT34_1/I005_%d/I402_%d/TARGET4_%d", // lay=4
	 "%sIT56_1/I565_%d/I562_%d/TARGET5_%d", // lay=5
	 "%sIT56_1/I569_%d/I566_%d/TARGET6_%d"}};// Lay=6
    /*
      Int_t itsGeomTreeCopys[knlayers][3]= {{10, 2, 4},// lay=1
      {10, 4, 4},// lay=2
      {14, 6, 1},// lay=3
      {22, 8, 1},// lay=4
      {34,22, 1},// lay=5
      {38,25, 1}};//lay=6
    */
    Int_t mod,nmods=0,lay,lad,det,cpn0,cpn1,cpn2;
    Double_t tran[3]={0.0,0.0,0.0},rot[10]={9*0.0,1.0};
    TArrayD shapePar;
    TString path,shapeName;
    TGeoHMatrix materix;
    Bool_t initSeg[3]={kFALSE,kFALSE,kFALSE};
//     TStopwatch *time = 0x0;if(fTiming) time=new TStopwatch();

    Int_t minorVersion =2;
//     if(fTiming) time->Start();
    for(mod=0;mod<klayers;mod++) nmods += kladders[mod]*kdetectors[mod];
    geom->Init(kItype,klayers,kladders,kdetectors,nmods,1);
    for(mod=0;mod<nmods;mod++){
        DecodeDetectorLayers(mod,lay,lad,det); // Write
        geom->CreatMatrix(mod,lay,lad,det,kIdet[lay-1],tran,rot);
        RecodeDetector(mod,cpn0,cpn1,cpn2); // Write reusing lay,lad,det.
        path.Form(kNames[minorVersion-1][lay-1].Data(),
                  kPathbase.Data(),cpn0,cpn1,cpn2);
        //geom->GetGeomMatrix(mod)->SetPath(path);
        GetTransformation(path.Data(),materix);
        geom->SetTrans(mod,materix.GetTranslation());
        geom->SetRotMatrix(mod,materix.GetRotationMatrix());
        if(initSeg[kIdet[lay-1]]) continue;
        GetShape(path,shapeName,shapePar);
        if(shapeName.CompareTo("BOX")){
            Error("InitTARGETgeom","Geometry changed without proper code update"
                  "or error in reading geometry. Shape is not BOX.");
            return kFALSE;
        } // end if
	InitGeomShapePPRasymmFMD(kIdet[lay-1],initSeg,shapePar,geom);
    } // end for module

//     if(fTiming){
//         time->Stop();
//         time->Print();
//         delete time;
//     } // end if
    return kTRUE;
}
//______________________________________________________________________
Bool_t IlcTARGET::InitGeomShapePPRasymmFMD(IlcTARGETDetector idet,
						       Bool_t *initSeg,
						       TArrayD &shapePar,
						       IlcTARGETgeom *geom){
    // Initilizes the geometry segmentation class IlcTARGETgeomS?D, or
    // IlcTARGETsegmentationS?D depending on the vaule of fSegGeom,
    // to values appropreate to this specific geometry. Now that
    // the segmentation is part of IlcTARGETgeom, the detector
    // segmentations are also defined here.
    // Inputs:
    //   Int_t      lay    The layer number/name.
    //   IlcTARGETgeom *geom  A pointer to the IlcTARGETgeom class
    // Outputs:
    //   IlcTARGETgeom *geom  This pointer recreated and properly inilized.
    // Return:
    //   none.
  //   const Double_t kcm2micron = 1.0E4;
  

  
    const Double_t kmicron2cm = 1.0E-4;
    Int_t i;
    TArrayF shapeParF;

    shapeParF.Set(shapePar.GetSize());
    for(i=0;i<shapePar.GetSize();i++) shapeParF[i]=shapePar[i];
    switch (idet){
    case kSPD:{
	initSeg[idet] = kTRUE;
	IlcTARGETgeomSPD *geomSPD = new IlcTARGETgeomSPD425Short();
	Float_t bx[256],bz[280];
	for(i=000;i<256;i++) bx[i] =  50.0*kmicron2cm; // in x all are 50 microns.
	for(i=000;i<160;i++) bz[i] = 425.0*kmicron2cm; // most are 425 microns
	// except below
	for(i=160;i<280;i++) bz[i] =   0.0*kmicron2cm; // Outside of detector.
	bz[ 31] = bz[ 32] = 625.0*kmicron2cm; // first chip boundry
	bz[ 63] = bz[ 64] = 625.0*kmicron2cm; // first chip boundry
	bz[ 95] = bz[ 96] = 625.0*kmicron2cm; // first chip boundry
	bz[127] = bz[128] = 625.0*kmicron2cm; // first chip boundry
	bz[160] = 425.0*kmicron2cm;// Set so that there is no zero pixel size for fNz.
	geomSPD->ReSetBins(shapeParF[1],256,bx,160,bz);
	geom->ReSetShape(idet,geomSPD);
    }break;
    case kSDD:{
	initSeg[idet] = kTRUE;
	IlcTARGETgeomSDD *geomSDD = new IlcTARGETgeomSDD256(shapeParF.GetSize(),
						      shapeParF.GetArray());
	geom->ReSetShape(idet,geomSDD);
    }break;
    case kSSD:{
	initSeg[idet] = kTRUE;
	IlcTARGETgeomSSD *geomSSD = new IlcTARGETgeomSSD275and75(
	    shapeParF.GetSize(),(Double_t *)shapeParF.GetArray());
	geom->ReSetShape(idet,geomSSD);
    }break;
    default:{// Others, Note no kSDDp or kSSDp in this geometry.
	geom->ReSetShape(idet,0);
	Info("InitGeomShapePPRasymmFMD",
	     "default Dx=%f Dy=%f Dz=%f default=%d",
	     shapePar[0],shapePar[1],shapePar[2],idet);
    }break;
    } // end switch
    return kTRUE;
}

//______________________________________________________________________
Bool_t IlcTARGET::GetTransformation(const TString &volumePath,
					     TGeoHMatrix &mat){
    // Returns the Transformation matrix between the volume specified
    // by the path volumePath and the Top or mater volume. The format
    // of the path volumePath is as follows (assuming ILCC is the Top volume)
    // "/ILCC_1/DDIP_1/S05I_2/S05H_1/S05G_3". Here ILCC is the top most
    // or master volume which has only 1 instance of. Of all of the daughter
    // volumes of ILC, DDIP volume copy #1 is indicated. Similarly for
    // the daughter volume of DDIP is S05I copy #2 and so on.
    // Inputs:
    //   TString& volumePath  The volume path to the specific volume
    //                        for which you want the matrix. Volume name
    //                        hierarchy is separated by "/" while the
    //                        copy number is appended using a "_".
    // Outputs:
    //  TGeoHMatrix &mat      A matrix with its values set to those
    //                        appropriate to the Local to Master transformation
    // Return:
    //   A logical value if kFALSE then an error occurred and no change to
    //   mat was made.

    // We have to preserve the modeler state

    // Preserve the modeler state.
    
    
    gGeoManager->PushPath();
    if (!gGeoManager->cd(volumePath.Data())) {
	gGeoManager->PopPath();
	Error("GetTransformation","Error in cd-ing to ",volumePath.Data());
	return kFALSE;
    } // end if !gGeoManager
    mat = *gGeoManager->GetCurrentMatrix();
    // Retstore the modeler state.
    gGeoManager->PopPath();
    return kTRUE;
}
//______________________________________________________________________
Bool_t IlcTARGET::GetShape(const TString &volumePath,
				    TString &shapeType,TArrayD &par){
    // Returns the shape and its parameters for the volume specified
    // by volumeName.
    // Inputs:
    //   TString& volumeName  The volume name
    // Outputs:
    //   TString &shapeType   Shape type
    //   TArrayD &par         A TArrayD of parameters with all of the
    //                        parameters of the specified shape.
    // Return:
    //   A logical indicating whether there was an error in getting this
    //   information
    
    
    Int_t npar;
    gGeoManager->PushPath();
    if (!gGeoManager->cd(volumePath.Data())) {
	gGeoManager->PopPath();
	return kFALSE;
    }
    TGeoVolume * vol = gGeoManager->GetCurrentVolume();
    gGeoManager->PopPath();
    if (!vol) return kFALSE;
    TGeoShape *shape = vol->GetShape();
    TClass *classType = shape->IsA();
    if (classType==TGeoBBox::Class()) {
	shapeType = "BOX";
	npar = 3;
	par.Set(npar);
	TGeoBBox *box = (TGeoBBox*)shape;
	par.AddAt(box->GetDX(),0);
	par.AddAt(box->GetDY(),1);
	par.AddAt(box->GetDZ(),2);
	return kTRUE;
    }
    if (classType==TGeoTrd1::Class()) {
	shapeType = "TRD1";
	npar = 4;
	par.Set(npar);
	TGeoTrd1 *trd1 = (TGeoTrd1*)shape;
	par.AddAt(trd1->GetDx1(),0);
	par.AddAt(trd1->GetDx2(),1);
	par.AddAt(trd1->GetDy(), 2);
	par.AddAt(trd1->GetDz(), 3);
	return kTRUE;
    }
    if (classType==TGeoTrd2::Class()) {
	shapeType = "TRD2";
	npar = 5;
	par.Set(npar);
	TGeoTrd2 *trd2 = (TGeoTrd2*)shape;
	par.AddAt(trd2->GetDx1(),0);
	par.AddAt(trd2->GetDx2(),1);
	par.AddAt(trd2->GetDy1(),2);
	par.AddAt(trd2->GetDy2(),3);
	par.AddAt(trd2->GetDz(), 4);
	return kTRUE;
    }
    if (classType==TGeoTrap::Class()) {
	shapeType = "TRAP";
	npar = 11;
	par.Set(npar);
	TGeoTrap *trap = (TGeoTrap*)shape;
	Double_t tth = TMath::Tan(trap->GetTheta()*TMath::DegToRad());
	par.AddAt(trap->GetDz(),0);
	par.AddAt(tth*TMath::Cos(trap->GetPhi()*TMath::DegToRad()),1);
	par.AddAt(tth*TMath::Sin(trap->GetPhi()*TMath::DegToRad()),2);
	par.AddAt(trap->GetH1(),3);
	par.AddAt(trap->GetBl1(),4);
	par.AddAt(trap->GetTl1(),5);
	par.AddAt(TMath::Tan(trap->GetAlpha1()*TMath::DegToRad()),6);
	par.AddAt(trap->GetH2(),7);
	par.AddAt(trap->GetBl2(),8);
	par.AddAt(trap->GetTl2(),9);
	par.AddAt(TMath::Tan(trap->GetAlpha2()*TMath::DegToRad()),10);
	return kTRUE;
    }
    if (classType==TGeoTube::Class()) {
	shapeType = "TUBE";
	npar = 3;
	par.Set(npar);
	TGeoTube *tube = (TGeoTube*)shape;
	par.AddAt(tube->GetRmin(),0);
	par.AddAt(tube->GetRmax(),1);
	par.AddAt(tube->GetDz(),2);
	return kTRUE;
    }
    if (classType==TGeoTubeSeg::Class()) {
	shapeType = "TUBS";
	npar = 5;
	par.Set(npar);
	TGeoTubeSeg *tubs = (TGeoTubeSeg*)shape;
	par.AddAt(tubs->GetRmin(),0);
	par.AddAt(tubs->GetRmax(),1);
	par.AddAt(tubs->GetDz(),2);
	par.AddAt(tubs->GetPhi1(),3);
	par.AddAt(tubs->GetPhi2(),4);
	return kTRUE;
    }
    if (classType==TGeoCone::Class()) {
	shapeType = "CONE";
	npar = 5;
	par.Set(npar);
	TGeoCone *cone = (TGeoCone*)shape;
	par.AddAt(cone->GetDz(),0);
	par.AddAt(cone->GetRmin1(),1);
	par.AddAt(cone->GetRmax1(),2);
	par.AddAt(cone->GetRmin2(),3);
	par.AddAt(cone->GetRmax2(),4);
	return kTRUE;
    }
    if (classType==TGeoConeSeg::Class()) {
	shapeType = "CONS";
	npar = 7;
	par.Set(npar);
	TGeoConeSeg *cons = (TGeoConeSeg*)shape;
	par.AddAt(cons->GetDz(),0);
	par.AddAt(cons->GetRmin1(),1);
	par.AddAt(cons->GetRmax1(),2);
	par.AddAt(cons->GetRmin2(),3);
	par.AddAt(cons->GetRmax2(),4);
	par.AddAt(cons->GetPhi1(),5);
	par.AddAt(cons->GetPhi2(),6);
	return kTRUE;
    }
    if (classType==TGeoSphere::Class()) {
	shapeType = "SPHE";
	npar = 6;
	par.Set(npar);
	
	TGeoSphere *sphe = (TGeoSphere*)shape;
	par.AddAt(sphe->GetRmin(),0);
	par.AddAt(sphe->GetRmax(),1);
	par.AddAt(sphe->GetTheta1(),2);
	par.AddAt(sphe->GetTheta2(),3);
	par.AddAt(sphe->GetPhi1(),4);
	par.AddAt(sphe->GetPhi2(),5);
	return kTRUE;
    }
    if (classType==TGeoPara::Class()) {
	shapeType = "PARA";
	npar = 6;
	par.Set(npar);
	TGeoPara *para = (TGeoPara*)shape;
	par.AddAt(para->GetX(),0);
	par.AddAt(para->GetY(),1);
	par.AddAt(para->GetZ(),2);
	par.AddAt(para->GetTxy(),3);
	par.AddAt(para->GetTxz(),4);
	par.AddAt(para->GetTyz(),5);
	return kTRUE;
    }
    if (classType==TGeoPgon::Class()) {
	shapeType = "PGON";
	TGeoPgon *pgon = (TGeoPgon*)shape;
	Int_t nz = pgon->GetNz();
	const Double_t *rmin = pgon->GetRmin();
	const Double_t *rmax = pgon->GetRmax();
	const Double_t *z = pgon->GetZ();
	npar = 4 + 3*nz;
	par.Set(npar);
	par.AddAt(pgon->GetPhi1(),0);
	par.AddAt(pgon->GetDphi(),1);
	par.AddAt(pgon->GetNedges(),2);
	par.AddAt(pgon->GetNz(),3);
	for (Int_t i=0; i<nz; i++) {
	    par.AddAt(z[i], 4+3*i);
	    par.AddAt(rmin[i], 4+3*i+1);
	    par.AddAt(rmax[i], 4+3*i+2);
	}
	return kTRUE;
    }
    if (classType==TGeoPcon::Class()) {
	shapeType = "PCON";
	TGeoPcon *pcon = (TGeoPcon*)shape;
	Int_t nz = pcon->GetNz();
	const Double_t *rmin = pcon->GetRmin();
	const Double_t *rmax = pcon->GetRmax();
	const Double_t *z = pcon->GetZ();
	npar = 3 + 3*nz;
	par.Set(npar);
	par.AddAt(pcon->GetPhi1(),0);
	par.AddAt(pcon->GetDphi(),1);
	par.AddAt(pcon->GetNz(),2);
	for (Int_t i=0; i<nz; i++) {
	    par.AddAt(z[i], 3+3*i);
	    
	    par.AddAt(rmin[i], 3+3*i+1);
	    par.AddAt(rmax[i], 3+3*i+2);
	}
	return kTRUE;
    }
    if (classType==TGeoEltu::Class()) {
	shapeType = "ELTU";
	npar = 3;
	par.Set(npar);
	TGeoEltu *eltu = (TGeoEltu*)shape;
	par.AddAt(eltu->GetA(),0);
	par.AddAt(eltu->GetB(),1);
	par.AddAt(eltu->GetDz(),2);
	return kTRUE;
    }
    if (classType==TGeoHype::Class()) {
	shapeType = "HYPE";
	npar = 5;
	par.Set(npar);
	TGeoHype *hype = (TGeoHype*)shape;
	par.AddAt(TMath::Sqrt(hype->RadiusHypeSq(0.,kTRUE)),0);
	par.AddAt(TMath::Sqrt(hype->RadiusHypeSq(0.,kFALSE)),1);
	par.AddAt(hype->GetDZ(),2);
	par.AddAt(hype->GetStIn(),3);
	par.AddAt(hype->GetStOut(),4);
	return kTRUE;
    }
    if (classType==TGeoGtra::Class()) {
	shapeType = "GTRA";
	npar = 12;
	par.Set(npar);
	TGeoGtra *trap = (TGeoGtra*)shape;
	Double_t tth = TMath::Tan(trap->GetTheta()*TMath::DegToRad());
	par.AddAt(trap->GetDz(),0);
	par.AddAt(tth*TMath::Cos(trap->GetPhi()*TMath::DegToRad()),1);
	par.AddAt(tth*TMath::Sin(trap->GetPhi()*TMath::DegToRad()),2);
	par.AddAt(trap->GetH1(),3);
	par.AddAt(trap->GetBl1(),4);
	par.AddAt(trap->GetTl1(),5);
	par.AddAt(TMath::Tan(trap->GetAlpha1()*TMath::DegToRad()),6);
	par.AddAt(trap->GetH2(),7);
	par.AddAt(trap->GetBl2(),8);
	par.AddAt(trap->GetTl2(),9);
	par.AddAt(TMath::Tan(trap->GetAlpha2()*TMath::DegToRad()),10);
	par.AddAt(trap->GetTwistAngle(),11);
	return kTRUE;
    }
    if (classType==TGeoCtub::Class()) {
	shapeType = "CTUB";
	npar = 11;
	par.Set(npar);
	TGeoCtub *ctub = (TGeoCtub*)shape;
	const Double_t *lx = ctub->GetNlow();
	const Double_t *tx = ctub->GetNhigh();
	par.AddAt(ctub->GetRmin(),0);
	par.AddAt(ctub->GetRmax(),1);
	par.AddAt(ctub->GetDz(),2);
	par.AddAt(ctub->GetPhi1(),3);
	par.AddAt(ctub->GetPhi2(),4);
	par.AddAt(lx[0],5);
	par.AddAt(lx[1],6);
	par.AddAt(lx[2],7);
	par.AddAt(tx[0],8);
	par.AddAt(tx[1],9);
	par.AddAt(tx[2],10);
	return kTRUE;
    }
    Error("GetShape","Getting shape parameters for shape %s not implemented",
	  shape->ClassName());
    return kFALSE;
}

//______________________________________________________________________
void IlcTARGET::RecodeDetector(Int_t mod,Int_t &cpn0,
                                        Int_t &cpn1,Int_t &cpn2){
    // decode geometry into detector module number. There are two decoding
    // Scheams. Old which does not follow the ILC coordinate system
    // requirements, and New which dose.
    // Inputs:
    //    Int_t mod      The module number assoicated with this set
    //                   of copy numbers.
    // Output:
    //    Int_t cpn0     The lowest copy number
    //    Int_t cpn1     The middle copy number
    //    Int_t cpn2     the highest copy number
    // Return:
    //    none.
    
    
    const Int_t kTARGETgeoTreeCopys[6][3]= {{10, 2, 4},// lay=1
                                         {10, 4, 4},// lay=2
                                         {14, 6, 1},// lay=3
                                         {22, 8, 1},// lay=4
                                         {34,22, 1},// lay=5
                                         {38,25, 1}};//lay=6
    const Int_t kDetPerLadderSPD[2]={2,4};
    //    const Int_t kDetPerLadder[6]={4,4,6,8,22,25};
    //    const Int_t kLadPerLayer[6]={20,40,14,22,34,38};
    Int_t lay,lad,det;

    cpn0 = cpn1 = cpn2 = 0;
    DecodeDetectorLayers(mod,lay,lad,det);
    if(fDecode){ // New decoding scheam
        switch (lay){
        case 1:{
            cpn2 = 5-det;     // Detector 1-4
            cpn1 = 1+(lad-1)%kDetPerLadderSPD[lay-1];
            cpn0 = 5-(lad+kDetPerLadderSPD[lay-1])/kDetPerLadderSPD[lay-1];
            if(mod>27) cpn0 = 15-(lad+kDetPerLadderSPD[lay-1])/
			   kDetPerLadderSPD[lay-1];
        } break;
        case 2:{
            cpn2 = 5-det;     // Detector 1-4
            cpn1 = 4-(lad+2)%kDetPerLadderSPD[lay-1];
            cpn0 = 1+(14-cpn1-lad)/kDetPerLadderSPD[lay-1];
            if(mod>131) cpn0 = 1+(54-lad-cpn1)/kDetPerLadderSPD[lay-1];
        } break;
        case 3:{
            cpn2 = 1;
            if(lad<5) cpn0 = 5-lad;
            else cpn0 = 19-lad;
            cpn1 = 7-det;
        } break;
        case 4:{
            cpn2 = 1;
            if(lad<7) cpn0 = 7-lad;
            else cpn0 = 29-lad;
            cpn1 = 9-det;
        } break;
        case 5:{
            cpn2 = 1;
            if(lad<10) cpn0 = 10-lad;
            else cpn0 = 44-lad;
            cpn1 = 23-det;
        } break;
        case 6:{
            cpn2 = 1;
            if(lad<9) cpn0 = 9-lad;
            else cpn0 = 47-lad;
            cpn1 = 26-det;
        } break;
        default:{
            Error("RecodeDetector","New: mod=%d lay=%d not 1-6.");
            return;
        } break;
        } // end switch
        if(cpn0<1||cpn1<1||cpn2<1||
           cpn0>kTARGETgeoTreeCopys[lay-1][0]||
           cpn1>kTARGETgeoTreeCopys[lay-1][1]||
           cpn2>kTARGETgeoTreeCopys[lay-1][2])
            Error("RecodeDetector",
                  "cpn0=%d cpn1=%d cpn2=%d mod=%d lay=%d lad=%d det=%d",
                  cpn0,cpn1,cpn2,mod,lay,lad,det);
        return;
    } // end if
    // Old encoding
    switch (lay){
    case 1: case 2:{
        cpn2 = det;     // Detector 1-4
        cpn0 = (lad+kDetPerLadderSPD[lay-1]-1)/kDetPerLadderSPD[lay-1];
        cpn1 = (lad+kDetPerLadderSPD[lay-1]-1)%kDetPerLadderSPD[lay-1] + 1;
    } break;
    case 3: case 4: case 5 : case 6:{
        cpn2 = 1;
        cpn1 = det;
        cpn0 = lad;
    } break;
    default:{
        Error("RecodeDetector","Old: mod=%d lay=%d not 1-6.");
        return;
    } break;
    } // end switch
    if(cpn0<1||cpn1<1||cpn2<1||
       cpn0>kTARGETgeoTreeCopys[lay-1][0]||
       cpn1>kTARGETgeoTreeCopys[lay-1][1]||
       cpn2>kTARGETgeoTreeCopys[lay-1][2])
        Error("RecodeDetector",
              "cpn0=%d cpn1=%d cpn2=%d mod=%d lay=%d lad=%d det=%d",
              cpn0,cpn1,cpn2,mod,lay,lad,det);
    return;
}
void IlcTARGET::SetSegmentationModel2(Int_t id, IlcTARGETsegmentation2 *seg) {
     fDetTypeSim->SetSegmentationModel2(id,seg);
}

     
//______________________________________________________________________
void IlcTARGET::DecodeDetectorLayers(Int_t mod,Int_t &lay,
                                              Int_t &lad,Int_t &det){
    // decode geometry into detector module number. There are two decoding
    // Scheams. Old which does not follow the ILC coordinate system
    // requirements, and New which dose. Note, this use of layer ladder
    // and detector numbers are strictly for internal use of this
    // specific code. They do not represent the "standard" layer ladder
    // or detector numbering except in a very old and obsoleate sence.
    // Inputs:
    //    Int_t mod      The module number assoicated with this set
    //                   of copy numbers.
    // Output:
    //    Int_t lay     The layer number
    //    Int_t lad     The ladder number
    //    Int_t det     the dettector number
    // Return:
    //    none.
  //    const Int_t kDetPerLadderSPD[2]={2,4};
  
    
    const Int_t kDetPerLadder[6]={4,4,6,8,22,25};
    const Int_t kLadPerLayer[6]={20,40,14,22,34,38};
    Int_t mod2;

    det  = 0;
    lad  = 0;
    lay  = 0;
    mod2 = 0;
    do{
        mod2 += kLadPerLayer[lay]*kDetPerLadder[lay];
        lay++;
    }while(mod2<=mod); // end while
    if(lay>6||lay<1) Error("DecodeDetectorLayers","0<lay=%d>6",lay);
    mod2 -= kLadPerLayer[lay-1]*kDetPerLadder[lay-1];
    do{
        lad++;
        mod2 += kDetPerLadder[lay-1];
    }while(mod2<=mod); // end while
    if(lad>kLadPerLayer[lay-1]||lad<1) Error("DecodeDetectorLayera",
            "lad=%d>kLadPerLayer[lay-1=%d]=%d mod=%d mod2=%d",lad,lay-1,
                                            kLadPerLayer[lay-1],mod,mod2);
    mod2 -= kDetPerLadder[lay-1];
    det = mod-mod2+1;
    if(det>kDetPerLadder[lay-1]||det<1) Error("DecodeDetectorLayers",
           "det=%d>detPerLayer[lay-1=%d]=%d mod=%d mod2=%d lad=%d",det,
                                  lay-1,kDetPerLadder[lay-1],mod,mod2,lad);
    return;
}






