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

/* $Id: IlcRun.cxx 77 2012-10-08 11:51:17Z ilcroot $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Control class for Ilc C++                                              //
//  Only one single instance of this class exists.                           //
//  The object is created in main program ilcroot                            //
//  and is pointed by the global gIlc.                                     //
//                                                                           //
//   -Supports the list of all Ilc Detectors (fModules).                 //
//   -Supports the list of particles (fParticles).                           //
//   -Supports the Trees.                                                    //
//   -Supports the geometry.                                                 //
//   -Supports the event display.                                            //
//Begin_Html
/*
<img src="picts/IlcRunClass.gif">
*/
//End_Html
//Begin_Html
/*
<img src="picts/ilcrun.gif">
*/
//End_Html
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TCint.h> 
#include <TROOT.h>
#include <TRandom3.h>
#include <TSystem.h>
#include <TVirtualMC.h>
#include <TGeoManager.h>
#include <TTree.h>
// 
#include "IlcLog.h"
#include "IlcDetector.h"
#include "IlcHeader.h"
#include "IlcMC.h"
#include "IlcPDG.h"
#include "IlcRun.h"
#include "IlcStack.h"
#include "IlcCDBManager.h"
#include "IlcAlignObj.h"
#include "IlcSimulation.h"
#include "IlcLego.h"

IlcRun *gIlc;

ClassImp(IlcRun)

//_______________________________________________________________________
IlcRun::IlcRun():
//  fRun(-1),
  fEventNrInRun(-1),
  fModules(0),
  fMCApp(0),
  fNdets(0),
  fConfigFunction(""),
  fBaseFileName(""),
  fRunLoader(0x0)
{
  //
  // Default constructor for IlcRun
  //
  IlcConfig::Instance();//skowron 29 Feb 2002
                        //ensures that the folder structure is build

}

//_____________________________________________________________________________
IlcRun::IlcRun(const char *name, const char *title):
  TNamed(name,title),
  fEventNrInRun(-1),
  fModules(new TObjArray(77)), // Support list for the Detectors
  fMCApp(new IlcMC(GetName(),GetTitle())),
  fNdets(0),
  fConfigFunction("Config();"),
  fBaseFileName(""),
  fRunLoader(0x0)
{
  //
  //  Constructor for the main processor.
  //  Creates the geometry
  //  Creates the list of Detectors.
  //  Creates the list of particles.
  //

  gIlc     = this;

  // Set random number generator
  gRandom = new TRandom3();

  if (gSystem->Getenv("CONFIG_SEED")) {
     gRandom->SetSeed(static_cast<UInt_t>(atoi(gSystem->Getenv("CONFIG_SEED"))));
  }

  // Add to list of browsable  
  gROOT->GetListOfBrowsables()->Add(this,name);
  
}

//_______________________________________________________________________
IlcRun::~IlcRun()
{
  //
  // Default IlcRun destructor
  //
  gROOT->GetListOfBrowsables()->Remove(this);

  if (fRunLoader)
   {
    TFolder* evfold = fRunLoader->GetEventFolder();
    TFolder* modfold = dynamic_cast<TFolder*>(evfold->FindObjectAny(IlcConfig::GetModulesFolderName()));
    if(!modfold) IlcFatal(Form("Folder %s not found\n",IlcConfig::GetModulesFolderName().Data()));
    TIter next(fModules);
    IlcModule *mod;
    while((mod = (IlcModule*)next()))
     { 
       modfold->Remove(mod);
     }
   }
    
  delete fMCApp;
  delete gMC; gMC=0;
  if (fModules) {
    fModules->Delete();
    delete fModules;
  }
  
}

//_____________________________________________________________________________
void IlcRun::InitLoaders()
{
  //creates list of getters
  IlcDebug(1, "");
  TIter next(fModules);
  IlcModule *mod;
  while((mod = (IlcModule*)next()))
   { 
     mod->SetRunLoader(fRunLoader);
     IlcDetector *det = dynamic_cast<IlcDetector*>(mod);
     if (det) 
      {
        IlcDebug(2, Form("Adding %s", det->GetName()));
        fRunLoader->AddLoader(det);
      }
   }
  IlcDebug(1, "Done");
}

//_______________________________________________________________________
void IlcRun::Announce() const
{
  //
  // Announce the current version of IlcRoot
  //
  printf("%70s",
	 "****************************************************************\n");
  printf("%6s","*");printf("%64s","*\n");

  //printf("%6s","*");
  //printf("    You are running IlcRoot version NewIO\n");

  printf("%6s","*");
//   printf("    You are using ILCroot $Revision$ \n");
  printf("    You are using ILCroot for ORKA \n");

  printf("%6s","*");printf("%64s","*\n");
  printf("%70s",
	 "****************************************************************\n");
}

//_______________________________________________________________________
IlcModule *IlcRun::GetModule(const char *name) const
{
  //
  // Return pointer to detector from name
  //
  return dynamic_cast<IlcModule*>(fModules->FindObject(name));
}
 
//_______________________________________________________________________
IlcDetector *IlcRun::GetDetector(const char *name) const
{
  //
  // Return pointer to detector from name
  //
  return dynamic_cast<IlcDetector*>(fModules->FindObject(name));
}
 
//_______________________________________________________________________
Int_t IlcRun::GetModuleID(const char *name) const
{
  //
  // Return gilc internal detector identifier from name
  //
  Int_t i=-1;
  TObject *mod=fModules->FindObject(name);
  if(mod) i=fModules->IndexOf(mod);
  return i;
}
 
//_______________________________________________________________________
Int_t IlcRun::GetEvent(Int_t event)
{
//
// Reloads data containers in folders # event
// Set branch addresses
//
  if (fRunLoader == 0x0)
   {
     IlcError("RunLoader is not set. Can not load data.");
     return -1;
   }
/*****************************************/ 
/****   P R E    R E L O A D I N G    ****/
/*****************************************/ 
// Reset existing structures
  fMCApp->ResetHits();
  fMCApp->ResetTrackReferences();
  fMCApp->ResetDigits();
  fMCApp->ResetSDigits();

/*****************************************/ 
/****       R  E  L  O  A  D          ****/
/*****************************************/

  IlcRunLoader::Instance()->GetEvent(event);

/*****************************************/ 
/****  P O S T    R E L O A D I N G   ****/
/*****************************************/ 

  // Set Trees branch addresses
  TIter next(fModules);
  IlcDetector *detector;
  while((detector = dynamic_cast<IlcDetector*>(next()))) 
   {
     detector->SetTreeAddress();
   }
 
  return IlcRunLoader::Instance()->GetHeader()->GetNtrack();
}

//_______________________________________________________________________
void IlcRun::SetBaseFile(const char *filename)
{
  fBaseFileName = filename;
}


//_______________________________________________________________________
void IlcRun::Hits2Digits(const char *selected)
{

   // Convert Hits to sumable digits
   // 
   for (Int_t nevent=0; nevent<IlcRunLoader::Instance()->TreeE()->GetEntries(); nevent++) {
     GetEvent(nevent);
     Hits2SDigits(selected);
     SDigits2Digits(selected);
   }  
}


//_______________________________________________________________________
void IlcRun::Tree2Tree(Option_t *option, const char *selected)
{
  //
  // Function to transform the content of
  //  
  // - TreeH to TreeS (option "S")
  // - TreeS to TreeD (option "D")
  // - TreeD to TreeR (option "R")
  // 
  // If multiple options are specified ("SDR"), transformation will be done in sequence for
  // selected detector and for all detectors if none is selected (detector string 
  // can contain blank separated list of detector names). 


   const char *oS = strstr(option,"S");
   const char *oD = strstr(option,"D");
   const char *oR = strstr(option,"R");
   
   TObjArray *detectors = Detectors();

   TIter next(detectors);

   IlcDetector *detector = 0;

   while((detector = dynamic_cast<IlcDetector*>(next()))) {
     if (selected) 
       if (strcmp(detector->GetName(),selected)) continue;
     if (detector->IsActive())
      { 
       
       IlcLoader* loader = detector->GetLoader();
       if (loader == 0x0) continue;
       
       if (oS) 
        {
          IlcDebug(1, Form("Processing Hits2SDigits for %s ...", detector->GetName()));
          loader->LoadHits("read");
          if (loader->TreeS() == 0x0) loader->MakeTree("S");
          detector->MakeBranch(option);
          detector->SetTreeAddress();
          detector->Hits2SDigits();
          loader->UnloadHits();
          loader->UnloadSDigits();
        }  
       if (oD) 
        {
          IlcDebug(1, Form("Processing SDigits2Digits for %s ...", detector->GetName()));
          loader->LoadSDigits("read");
          if (loader->TreeD() == 0x0) loader->MakeTree("D");
          detector->MakeBranch(option);
          detector->SetTreeAddress();
          detector->SDigits2Digits();
          loader->UnloadSDigits();
          loader->UnloadDigits();
        } 
       if (oR) 
        {
          IlcDebug(1, Form("Processing Digits2Reco for %s ...", detector->GetName()));
          loader->LoadDigits("read");
          if (loader->TreeR() == 0x0) loader->MakeTree("R");
          detector->MakeBranch(option);
          detector->SetTreeAddress();
          detector->Digits2Reco(); 
          loader->UnloadDigits();
          loader->UnloadRecPoints();

        }
     }   
   }
}


//_______________________________________________________________________
void IlcRun::Streamer(TBuffer &R__b)
{
  // Stream an object of class IlcRun.

  if (R__b.IsReading()) {
    if (!gIlc) gIlc = this;
    IlcRun::Class()->ReadBuffer(R__b, this);
    gROOT->GetListOfBrowsables()->Add(this,"Run");
    gRandom = new TRandom3();
  } else {
    IlcRun::Class()->WriteBuffer(R__b, this);
  }
}

//_______________________________________________________________________
void IlcRun::SetGenEventHeader(IlcGenEventHeader* header)
{
  IlcRunLoader::Instance()->GetHeader()->SetGenEventHeader(header);
}


//_______________________________________________________________________
Int_t IlcRun::GetEvNumber() const
{ 
//Returns number of current event  
  if (fRunLoader == 0x0)
   {
     IlcError("RunLoader is not set. Can not load data.");
     return -1;
   }

  return fRunLoader->GetEventNumber();
}

//_______________________________________________________________________
void IlcRun::SetRunLoader(IlcRunLoader* rloader)
{
  //
  // Set the loader of the run
  //
  fRunLoader = rloader;
  if (fRunLoader == 0x0) return;
  
  TString evfoldname;
  TFolder* evfold = fRunLoader->GetEventFolder();
  if (evfold) evfoldname = evfold->GetName();
  else IlcFatal("Did not get Event Folder from Run Loader");
  
  if ( fRunLoader->GetIlcRun() )
   {//if alrun already exists in folder
    if (fRunLoader->GetIlcRun() != this )
     {//and is different than this - crash
       IlcFatal("IlcRun is already in Folder and it is not this object");
       return;//pro forma
     }//else do nothing
   }
  else
   {
     evfold->Add(this);//Post this IlcRun to Folder
   }
  
  TIter next(fModules);
  IlcModule *module;
  while((module = (IlcModule*)next())) 
   {
     if (evfold) IlcConfig::Instance()->Add(module,evfoldname);
     module->SetRunLoader(fRunLoader);
     IlcDetector* detector = dynamic_cast<IlcDetector*>(module);
     if (detector)
      {
        IlcLoader* loader = fRunLoader->GetLoader(detector);
        if (loader == 0x0)
         {
           IlcError(Form("Can not get loader for detector %s", detector->GetName()));
         }
        else
         {
           IlcDebug(1, Form("Setting loader for detector %s", detector->GetName()));
           detector->SetLoader(loader);
         }
      }
   }
}

//_______________________________________________________________________
void IlcRun::AddModule(IlcModule* mod)
{
  //
  // Add a module to the module list
  //
  if (mod == 0x0) return;
  if (strlen(mod->GetName()) == 0) return;
  if (GetModuleID(mod->GetName()) >= 0) return;
  
  IlcDebug(1, mod->GetName());
  if (fRunLoader == 0x0) IlcConfig::Instance()->Add(mod);
  else IlcConfig::Instance()->Add(mod,fRunLoader->GetEventFolder()->GetName());

  Modules()->Add(mod);
  
  fNdets++;
}

