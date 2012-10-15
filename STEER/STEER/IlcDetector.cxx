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

/* $Id: IlcDetector.cxx 50615 2011-07-16 23:19:19Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Base class for ILC modules. Both sensitive modules (detectors) and      //
// non-sensitive ones are described by this base class. This class           //
// supports the hit and digit trees produced by the simulation and also      //
// the objects produced by the reconstruction.                               //
//                                                                           //
// This class is also responsible for building the geometry of the           //
// detectors.                                                                //
//                                                                           //
//Begin_Html
/*
<img src="picts/IlcDetectorClass.gif">
*/
//End_Html
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TBrowser.h>
#include <TClonesArray.h>
#include <TTree.h>

#include "IlcLog.h"
#include "IlcConfig.h"
#include "IlcDetector.h"
#include "IlcHit.h"
#include "IlcLoader.h"
#include "IlcRun.h"
#include "IlcMC.h"


ClassImp(IlcDetector)
 
//_______________________________________________________________________
IlcDetector::IlcDetector():
  IlcModule(),
  fTimeGate(200.e-9),
  fIshunt(0),
  fNhits(0),
  fNdigits(0),
  fBufferSize(1600),
  fMaxIterHit(0),
  fCurIterHit(0),
  fHits(0),
  fDigits(0),
  fLoader(0x0)
{
  //
  // Default constructor for the IlcDetector class
  //
}
 
//_____________________________________________________________________________
IlcDetector::IlcDetector(const char* name,const char *title):
  IlcModule(name,title),
  fTimeGate(200.e-9),
  fIshunt(0),
  fNhits(0),
  fNdigits(0),
  fBufferSize(1600),
  fMaxIterHit(0),
  fCurIterHit(0),
  fHits(0),
  fDigits(0),
  fLoader(0x0)
{
  //
  // Normal constructor invoked by all Detectors.
  // Create the list for detector specific histograms
  // Add this Detector to the global list of Detectors in Run.
  //

  fActive     = kTRUE;
  IlcConfig::Instance()->Add(this);

}
 
//_______________________________________________________________________
IlcDetector::~IlcDetector()
{
  //
  // Destructor
  //

  // Delete digits structure
  if (fDigits) {
    fDigits->Delete();
    delete fDigits;
    fDigits     = 0;
  }

  if (fLoader)
   {
    fLoader->GetModulesFolder()->Remove(this);
   }

}

//_______________________________________________________________________
void IlcDetector::Publish(const char */*dir*/, void */*address*/, const char */*name*/) const
{
//
// Register pointer to detector objects. 
// 
  MayNotUse("Publish");
}

//_______________________________________________________________________
void IlcDetector::AddAlignableVolumes() const
{
  // 
  IlcWarning(Form("%s still has to implement the AddAlignableVolumes method!",GetName()));
}

//_______________________________________________________________________
TBranch* IlcDetector::MakeBranchInTree(TTree *tree, const char* name, 
                                       void* address, Int_t size,
                                       const char *file)
{ 
    return(MakeBranchInTree(tree,name,0,address,size,99,file));
}

//_______________________________________________________________________
TBranch* IlcDetector::MakeBranchInTree(TTree *tree, const char* name, 
                                       const char *classname, 
                                       void* address,Int_t size, 
                                       Int_t splitlevel, const char */*file*/)
{ 
//
// Makes branch in given tree and diverts them to a separate file
// 
//
//
    
 IlcDebug(2,Form("Making Branch %s",name));
 if (tree == 0x0) 
  {
   IlcError(Form("Making Branch %s Tree is NULL",name));
   return 0x0;
  }
 TBranch *branch = tree->GetBranch(name);
 if (branch) 
  {  
    IlcDebug(2,Form("Branch %s is already in tree.",name));
    return branch;
  }
    
 if (classname) 
  {
    branch = tree->Branch(name,classname,address,size,splitlevel);
  } 
 else 
  {
    branch = tree->Bronch(name, "TClonesArray", address, size, splitlevel);
  }
 IlcDebug(2,Form("Branch %s returning branch %p",name,branch));
 return branch;
}

//_______________________________________________________________________
void IlcDetector::Browse(TBrowser *b)
{
  //
  // Insert Detector objects in the list of objects to be browsed
  //
  char name[64]="";
  if( fHits == 0) return;
  TObject *obj;
  Int_t i, nobjects;
  //
  nobjects = fHits->GetEntries();
  for (i=0;i<nobjects;i++) {
    obj = fHits->At(i);
    snprintf(name,63,"%s_%d",obj->GetName(),i);
    b->Add(obj, &name[0]);
  }
}

//_______________________________________________________________________
void IlcDetector::FinishRun()
{
  //
  // Procedure called at the end of a run.
  //
}

//_______________________________________________________________________
IlcHit* IlcDetector::FirstHit(Int_t track)
{
  //
  // Initialise the hit iterator
  // Return the address of the first hit for track
  // If track>=0 the track is read from disk
  // while if track<0 the first hit of the current
  // track is returned
  // 
  if(track>=0) {
    gIlc->GetMCApp()->ResetHits(); //stupid = if N detector this method is called N times
    fLoader->TreeH()->GetEvent(track); //skowron
  }
  //
  fMaxIterHit=fHits->GetEntriesFast();
  fCurIterHit=0;
  if(fMaxIterHit) return dynamic_cast<IlcHit*>(fHits->UncheckedAt(0));
  else            return 0;
}

//_______________________________________________________________________
IlcHit* IlcDetector::NextHit()
{
  //
  // Return the next hit for the current track
  //
  if(fMaxIterHit) {
    if(++fCurIterHit<fMaxIterHit) 
      return dynamic_cast<IlcHit*>(fHits->UncheckedAt(fCurIterHit));
    else        
      return 0;
  } else {
    IlcWarning("Hit Iterator called without calling FistHit before");
    return 0;
  }
}

//_______________________________________________________________________
void IlcDetector::MakeBranch(Option_t *option)
{
  //
  // Create a new branch for this detector in its treeH
  //

  IlcDebug(2,Form(" for %s",GetName()));
  const char *cH = strstr(option,"H");

  if (fHits && fLoader->TreeH() && cH) 
   {
     MakeBranchInTree(fLoader->TreeH(), GetName(), &fHits, fBufferSize, 0);
   }	
}

//_______________________________________________________________________
void IlcDetector::ResetDigits()
{
  //
  // Reset number of digits and the digits array
  //
  fNdigits   = 0;
  if (fDigits) fDigits->Clear();
}

//_______________________________________________________________________
void IlcDetector::ResetHits()
{
  //
  // Reset number of hits and the hits array
  //
  fNhits   = 0;
  if (fHits) fHits->Clear();
}

//_______________________________________________________________________
void IlcDetector::SetTreeAddress()
{
  //
  // Set branch address for the Hits and Digits Trees
  //
  TBranch *branch;
  //
  // Branch address for hit tree
  
  TTree* tree = fLoader->TreeH();
  if (tree && fHits) {
    branch = tree->GetBranch(GetName());
    if (branch) 
     {
       IlcDebug(2,Form("(%s) Setting for Hits",GetName()));
       branch->SetAddress(&fHits);
     }
    else
     { //can be invoked before branch creation
       IlcDebug(2,Form("(%s) Failed for Hits. Can not find branch in tree.",GetName()));
     }
  }
  
  //
  // Branch address for digit tree
  TTree *treeD = fLoader->TreeD();
  if (treeD && fDigits) {
    branch = treeD->GetBranch(GetName());
    if (branch) branch->SetAddress(&fDigits);
  }
}

//_______________________________________________________________________
void IlcDetector::MakeTree(Option_t *option)
 {
 //makes a tree (container) for the data defined in option
 //"H" - hits
 //"D" - digits
 //"S" - summable digits
 //"R" - recontructed points and tracks
 
    IlcLoader* loader = GetLoader();
    if (loader == 0x0)
     {
       IlcError(Form("Can not get loader for %s",GetName()));
       return;
     }
    loader->MakeTree(option); //delegate this job to getter
 }

//_______________________________________________________________________
IlcLoader* IlcDetector::MakeLoader(const char* topfoldername)
{ 
//builds standard getter (IlcLoader type)
//if detector wants to use castomized getter, it must overload this method

 IlcDebug(1,Form("Creating standard getter for detector %s. Top folder is %s.",
         GetName(),topfoldername));
     
 fLoader = new IlcLoader(GetName(),topfoldername);
 return fLoader;
}


