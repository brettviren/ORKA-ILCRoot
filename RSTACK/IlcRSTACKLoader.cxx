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

/* $Id: IlcRSTACKLoader.cxx 52261 2011-10-23 15:46:57Z hristov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.18  2006/08/28 10:01:56  kharlov
 * Effective C++ warnings fixed (Timur Pocheptsov)
 *
 * Revision 1.17  2006/08/25 16:00:53  kharlov
 * Compliance with Effective C++IlcRSTACKHit.cxx
 *
 * Revision 1.16  2006/08/01 12:15:04  cvetan
 * Adding a constructor from TFolder. Needed by IlcReconstruction plugin scheme
 *
 * Revision 1.15  2005/07/12 20:07:35  hristov
 * Changes needed to run simulation and reconstrruction in the same IlcRoot session
 *
 * Revision 1.14  2005/05/28 14:19:04  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
//  A singleton. This class should be used in the analysis stage to get 
//  reconstructed objects: Digits, RecPoints, TrackSegments and RecParticles,
//  instead of directly reading them from gilc.root file. This container 
//  ensures, that one reads Digits, made of these particular digits, RecPoints, 
//  made of these particular RecPoints, TrackSegments and RecParticles. 
//  This becomes non trivial if there are several identical branches, produced with
//  different set of parameters. 
//
//  An example of how to use (see also class IlcRSTACKAnalyser):
//  for(Int_t irecp = 0; irecp < gime->NRecParticles() ; irecp++)
//     IlcRSTACKRecParticle * part = gime->RecParticle(1) ;
//     ................
//  please->GetEvent(event) ;    // reads new event from gilc.root
//                  
//-- Author: Yves Schutz (SUBATECH) & Dmitri Peressounko (RRC KI & SUBATECH)
//--         Completely redesigned by Dmitri Peressounko March 2001  
//
//-- YS June 2001 : renamed the original IlcRSTACKIndexToObject and make
//--         systematic usage of TFolders without changing the interface        
//////////////////////////////////////////////////////////////////////////////


// --- ROOT system ---

#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcObjectLoader.h"
#include "IlcLog.h"
#include "IlcRSTACKLoader.h"
#include "IlcRSTACK.h"
#include "IlcRSTACKHit.h"

ClassImp(IlcRSTACKLoader)


const TString IlcRSTACKLoader::fgkHitsName("HITS");//Name for TClonesArray with hits from one event
const TString IlcRSTACKLoader::fgkSDigitsName("SDIGITS");//Name for TClonesArray 
const TString IlcRSTACKLoader::fgkDigitsName("DIGITS");//Name for TClonesArray 
const TString IlcRSTACKLoader::fgkEmcRecPointsName("EMCRECPOINTS");//Name for TClonesArray 
const TString IlcRSTACKLoader::fgkCpvRecPointsName("CPVRECPOINTS");//Name for TClonesArray 
const TString IlcRSTACKLoader::fgkTracksName("TRACKS");//Name for TClonesArray 
const TString IlcRSTACKLoader::fgkRecParticlesName("RECPARTICLES");//Name for TClonesArray

const TString IlcRSTACKLoader::fgkEmcRecPointsBranchName("RSTACKEmcRP");//Name for branch with EMC Reconstructed Points
const TString IlcRSTACKLoader::fgkCpvRecPointsBranchName("RSTACKCpvRP");//Name for branch with CPV Reconstructed Points
const TString IlcRSTACKLoader::fgkTrackSegmentsBranchName("RSTACKTS");//Name for branch with TrackSegments
const TString IlcRSTACKLoader::fgkRecParticlesBranchName("RSTACKRP");//Name for branch with Reconstructed Particles
//____________________________________________________________________________ 
IlcRSTACKLoader::IlcRSTACKLoader() : fBranchTitle(), fDebug(0), fTmpHits(0x0)
{
  //def ctor
  fTmpHits =  new TClonesArray("IlcRSTACKHit",1000);
}
//____________________________________________________________________________ 
IlcRSTACKLoader::IlcRSTACKLoader(const Char_t *detname,const Char_t *eventfoldername) :
      IlcLoader(detname, eventfoldername),
      fBranchTitle(), fDebug(0), fTmpHits(0x0)
{
  //ctor
}
//____________________________________________________________________________ 
IlcRSTACKLoader::IlcRSTACKLoader(const Char_t *detname,TFolder *topfolder):
  IlcLoader(detname,topfolder),
  fBranchTitle(), fDebug(0), fTmpHits(0x0)

{
  //ctor
  fTmpHits =  new TClonesArray("IlcRSTACKHit",1000);
}

//____________________________________________________________________________ 

IlcRSTACKLoader::~IlcRSTACKLoader()
{
  //remove and delete arrays
  Clean(fgkHitsName);
  Clean(fgkSDigitsName);
  Clean(fgkDigitsName);
  Clean(fgkEmcRecPointsName);
  Clean(fgkCpvRecPointsName);
  Clean(fgkTracksName);
  Clean(fgkRecParticlesName);
  CleanFolders() ;
}

//____________________________________________________________________________ 
void IlcRSTACKLoader::CleanFolders()
 {
   CleanRecParticles();
   IlcLoader::CleanFolders();
 }

//____________________________________________________________________________ 
Int_t IlcRSTACKLoader::SetEvent()
{
//Cleans loaded stuff and and sets Files and Directories
// do not post any data to folder


 Int_t retval = IlcLoader::SetEvent();
  if (retval)
   {
     IlcError("returned error");
     return retval;
   }


  if (Hits()) Hits()->Clear();
  if (SDigits()) SDigits()->Clear();
  if (Digits()) Digits()->Clear();
  if (EmcRecPoints()) EmcRecPoints()->Clear();
  if (CpvRecPoints()) CpvRecPoints()->Clear();
  if (TrackSegments()) TrackSegments()->Clear();
  if (RecParticles()) RecParticles()->Clear();
   
  return 0;
}

//____________________________________________________________________________ 
Int_t IlcRSTACKLoader::GetEvent()
{
//Overloads GetEvent method called by IlcRunLoader::GetEvent(Int_t) method
//to add Rec Particles specific for RSTACK

//First call the original method to get whatever from std. setup is needed
  Int_t retval;
  
  retval = IlcLoader::GetEvent();
  if (retval)
   {
     IlcError("returned error");
     return retval;
   }
  
  if (GetHitsDataLoader()->GetBaseDataLoader()->IsLoaded()) ReadHits();
  if (GetSDigitsDataLoader()->GetBaseDataLoader()->IsLoaded()) ReadSDigits();
  if (GetDigitsDataLoader()->GetBaseDataLoader()->IsLoaded()) ReadDigits();
  if (GetRecPointsDataLoader()->GetBaseDataLoader()->IsLoaded()) ReadRecPoints();
  if (GetTracksDataLoader()->GetBaseDataLoader()->IsLoaded()) ReadTracks();
  if (GetRecParticlesDataLoader()->GetBaseDataLoader()->IsLoaded()) ReadRecParticles();


//Now, check if RecPart were loaded  
  return 0;
}

// //____________________________________________________________________________ 
// const IlcRSTACK * IlcRSTACKLoader::RSTACK() 
// {
//   // returns the RSTACK object 
//   IlcRSTACK * RSTACK = dynamic_cast<IlcRSTACK*>(GetModulesFolder()->FindObject(fDetectorName));
//   if ( RSTACK == 0x0) 
//     if (fDebug)
//       cout << "WARNING: IlcRSTACKLoader::RSTACK -> RSTACK module not found in Folders" << endl ; 
//   return RSTACK ; 
// }  

//____________________________________________________________________________ 
Int_t IlcRSTACKLoader::LoadHits(Option_t* opt)
{  
//------- Hits ----------------------
//Overload (extends) LoadHits implemented in IlcLoader
//
  Int_t res;
  
  //First call the IlcLoader's method to send the TreeH to folder
  res = IlcLoader::LoadHits(opt);
  
  if (res)
   {//oops, error
     IlcError("returned error");
     return res;
   }

  //read the data from tree in folder and send it to folder
  res = ReadHits();
  return 0;
}


//____________________________________________________________________________ 
Int_t IlcRSTACKLoader::LoadSDigits(Option_t* opt)
{
  //---------- SDigits -------------------------
  Int_t res;
  //First call the IlcLoader's method to send the TreeS to folder
  res = IlcLoader::LoadSDigits(opt);
  if (res)
   {//oops, error
     IlcError("returned error");
     return res;
   }
  return ReadSDigits();
   
} 
//____________________________________________________________________________ 
Int_t IlcRSTACKLoader::LoadDigits(Option_t* opt)
{ 
  //---------- Digits -------------------------
  Int_t res;
  //First call the IlcLoader's method to send the TreeS to folder
  res = IlcLoader::LoadDigits(opt);
  if (res)
   {//oops, error
     IlcError("returned error");
     return res;
   }
  return ReadDigits();
}
//____________________________________________________________________________ 
Int_t IlcRSTACKLoader::LoadRecPoints(Option_t* opt) 
{
  // -------------- RecPoints -------------------------------------------
  Int_t res;
  //First call the IlcLoader's method to send the TreeR to folder
  res = IlcLoader::LoadRecPoints(opt);
  if (res)
   {//oops, error
     IlcError("returned error");
     return res;
   }

  TFolder * RSTACKFolder = GetDetectorDataFolder();
  if ( RSTACKFolder  == 0x0 ) 
   {
     IlcError("Can not get detector data folder");
     return 1;
   }
  return ReadRecPoints();
}
//____________________________________________________________________________ 

Int_t  IlcRSTACKLoader::LoadTracks(Option_t* opt)
{
 //Loads Tracks: Open File, Reads Tree and posts, Read Data and Posts
  IlcDebug(1, Form("opt = %s",opt));
 Int_t res;
 res = IlcLoader::LoadTracks(opt);
 if (res)
   {//oops, error
      IlcError("returned error");
      return res;
   }  
 return ReadTracks();
}

//____________________________________________________________________________ 
Int_t IlcRSTACKLoader::LoadRecParticles(Option_t* opt) 
{
  // -------------- RecPoints -------------------------------------------
  Int_t res;
  //First call the IlcLoader's method to send the TreeT to folder
  res = IlcLoader::LoadRecParticles(opt);
  if (res)
   {//oops, error
     IlcError("returned error");
     return res;
   }
  return ReadRecParticles();
}

//____________________________________________________________________________ 
//PostHits etc. PostXXX must be const - not to hide virtual functions
//from base class IlcLoader, but they call non-constant functions ReadXXX
//so I have to const_cast this pointer
Int_t IlcRSTACKLoader::PostHits()const
{
  // -------------- Hits -------------------------------------------
  Int_t reval = IlcLoader::PostHits();
  if (reval)
   {
     IlcError("returned error");
     return reval;
   }
   
  return const_cast<IlcRSTACKLoader *>(this)->ReadHits();
}
//____________________________________________________________________________ 

Int_t IlcRSTACKLoader::PostSDigits()const
{
  // -------------- SDigits -------------------------------------------
  Int_t reval = IlcLoader::PostSDigits();
  if (reval)
   {
     IlcError("returned error");
     return reval;
   }
  return const_cast<IlcRSTACKLoader *>(this)->ReadSDigits();
}
//____________________________________________________________________________ 

Int_t IlcRSTACKLoader::PostDigits()const
{
  // -------------- Digits -------------------------------------------
  Int_t reval = IlcLoader::PostDigits();
  if (reval)
   {
     IlcError("returned error");
     return reval;
   }
  return const_cast<IlcRSTACKLoader *>(this)->ReadDigits();
}
//____________________________________________________________________________ 

Int_t IlcRSTACKLoader::PostRecPoints()const
{
  // -------------- RecPoints -------------------------------------------
  Int_t reval = IlcLoader::PostRecPoints();
  if (reval)
   {
     IlcError("returned error");
     return reval;
   }
  return const_cast<IlcRSTACKLoader *>(this)->ReadRecPoints();
}

//____________________________________________________________________________ 

Int_t IlcRSTACKLoader::PostRecParticles()const
{
  // -------------- RecParticles -------------------------------------------
  Int_t reval = IlcLoader::PostRecParticles();
  if (reval)
   {
     IlcError("returned error");
     return reval;
   }
  return const_cast<IlcRSTACKLoader *>(this)->ReadRecParticles();
}
//____________________________________________________________________________ 

Int_t IlcRSTACKLoader::PostTracks()const
{
  // -------------- Tracks -------------------------------------------
  Int_t reval = IlcLoader::PostTracks();
  if (reval)
   {
     IlcError("returned error");
     return reval;
   }
  return const_cast<IlcRSTACKLoader *>(this)->ReadTracks();
}
//____________________________________________________________________________ 



//____________________________________________________________________________ 
Int_t IlcRSTACKLoader::ReadHits()
{
// If there is no Clones Array in folder creates it and sends to folder
// then tries to read
// Reads the first entry of RSTACK branch in hit tree TreeH()
// Reads data from TreeH and stores it in TClonesArray that sits in DetectorDataFolder
//
  TObject** hitref = HitsRef();
  if(hitref == 0x0) {
    MakeHitsArray();
    hitref = HitsRef();
  }

  TClonesArray* hits = static_cast<TClonesArray*>(*hitref);

  TTree* treeh = TreeH();
  
  if(treeh == 0) {
    IlcError("Cannot read TreeH from folder");
    return 1;
  }
  
  TBranch * hitsbranch = treeh->GetBranch(fDetectorName);
  if (hitsbranch == 0) {
    IlcError("Cannot find branch RSTACK"); 
    return 1;
  }

  IlcDebug(1, "Reading Hits");
  
  if (hitsbranch->GetEntries() > 1) {

    hitsbranch->SetAddress(&fTmpHits);
    Int_t index = 0 ; 
    for (Int_t i = 0 ; i < hitsbranch->GetEntries(); i++) {
      hitsbranch->GetEntry(i) ;
      for (Int_t j = 0 ; j < fTmpHits->GetEntriesFast() ; j++) {
	IlcRSTACKHit* hit = (IlcRSTACKHit*)fTmpHits->At(j); 
	new((*hits)[index]) IlcRSTACKHit( *hit ) ;
	index++ ; 
      }
    }
    fTmpHits->Clear();
  }
  else {
    hitsbranch->SetAddress(hitref);
    hitsbranch->GetEntry(0) ;
  }
  
  return 0;
}
//____________________________________________________________________________ 
Int_t IlcRSTACKLoader::ReadSDigits()
{
// Read the summable digits tree TreeS():
// Check if TClones is in folder
// if not create and add to folder
// connect to tree if available
// Read the data

  TObject** sdref = SDigitsRef();
  if(sdref == 0x0)
   {
     MakeSDigitsArray();
     sdref = SDigitsRef();
   }
   
  TTree * treeS = TreeS();
  if(treeS==0)
   {
     //May happen if file is truncated or new in LoadSDigits
     //IlcError("There is no SDigit Tree");
     return 0;
   }
  
  TBranch * branch = treeS->GetBranch(fDetectorName);
  if (branch == 0) 
   {//easy, maybe just a new tree
    //IlcError("Cannot find branch RSTACK"); 
    return 0;
  }
    
  branch->SetAddress(sdref);
  branch->GetEntry(0);
  return 0;
}

//____________________________________________________________________________ 
Int_t IlcRSTACKLoader::ReadDigits()
{
// Read the summable digits tree TreeS():
// Check if TClones is in folder
// if not create and add to folder
// connect to tree if available
// Read the data
  
  TObject** dref = DigitsRef();
  if(dref == 0x0)
   {//if there is not array in folder, create it and put it there
     MakeDigitsArray();
     dref = DigitsRef();
   }

  TTree * treeD = TreeD();
  if(treeD==0)
   {
     //May happen if file is truncated or new in LoadSDigits
     //IlcError("There is no Digit Tree");
     return 0;
   }

  TBranch * branch = treeD->GetBranch(fDetectorName);
  if (branch == 0) 
   {//easy, maybe just a new tree
    //IlcError("Cannot find branch ",fDetectorName.Data()); 
    return 0;
   }
  
  branch->SetAddress(dref);//connect branch to buffer sitting in folder
  branch->GetEntry(0);//get first event 

  return 0;  
}

//____________________________________________________________________________ 

void IlcRSTACKLoader::Track(Int_t itrack)
{
// Read the first entry of RSTACK branch in hit tree gIlc->TreeH()
  if(TreeH()== 0)
   {
     if (LoadHits())
      {
        IlcError("Can not load hits.");
        return;
      } 
   }
  
  TBranch * hitsbranch = dynamic_cast<TBranch*>(TreeH()->GetListOfBranches()->FindObject("RSTACK")) ;
  if ( !hitsbranch ) {
    if (fDebug)
      cout << "WARNING:  IlcRSTACKLoader::ReadTreeH -> Cannot find branch RSTACK" << endl ; 
    return ;
  }  
  if(!Hits()) PostHits();

  hitsbranch->SetAddress(HitsRef());
  hitsbranch->GetEntry(itrack);

}

//____________________________________________________________________________ 
Int_t IlcRSTACKLoader::ReadRecPoints()
{
//Creates and posts to folder an array container, 
//connects branch in tree (if exists), and reads data to array
  
  MakeRecPointsArray();
  
  TObjArray * cpva = 0x0 ; 
  TObjArray * emca = 0x0 ; 
  
  TTree * treeR = TreeR();
 
  if(treeR==0)
   {
     //May happen if file is truncated or new in LoadSDigits
     return 0;
   }

  Int_t retval = 0;
  TBranch * emcbranch = treeR->GetBranch(fgkEmcRecPointsBranchName);

  if (emcbranch == 0x0)
   {
     IlcError(Form("Can not get branch with EMC Rec. Points named %s",
		   fgkEmcRecPointsBranchName.Data()));
     retval = 1;
   }
  else
   {
     emcbranch->SetAddress(&emca) ;
     emcbranch->GetEntry(0) ;
   }
  TBranch * cpvbranch = treeR->GetBranch(fgkCpvRecPointsBranchName);
  if (cpvbranch == 0x0)
   {
     IlcError(Form("Can not get branch with CPV Rec. Points named %s",
		   fgkCpvRecPointsBranchName.Data()));
     retval = 2;
   }
  else
   {
     cpvbranch->SetAddress(&cpva);
     cpvbranch->GetEntry(0) ;
   }

  Int_t ii ; 
  if (emca != 0) {
    Int_t maxemc = emca->GetEntries() ; 
    for ( ii= 0 ; ii < maxemc ; ii++ ) 
      EmcRecPoints()->Add(emca->At(ii)) ;
  }

  if (cpva != 0) {
    Int_t maxcpv = cpva->GetEntries() ;
    for ( ii= 0 ; ii < maxcpv ; ii++ )
      CpvRecPoints()->Add(cpva->At(ii)) ; 
  }

  return retval;
}

//____________________________________________________________________________ 
Int_t IlcRSTACKLoader::ReadTracks()
{
//Creates and posts to folder an array container, 
//connects branch in tree (if exists), and reads data to arry

  TObject** trkref = TracksRef();
  if ( trkref == 0x0 )   
   {//Create and post array
    MakeTrackSegmentsArray();
    trkref = TracksRef();
   }

  TTree * treeT = TreeT();
  if(treeT==0)
   {
     //May happen if file is truncated or new in LoadSDigits, or the file is in update mode, 
     //but tracking was not performed yet for a current event
     //IlcError("There is no Tree with Tracks");
     return 0;
   }
  
  TBranch * branch = treeT->GetBranch(fgkTrackSegmentsBranchName);
//   IlcInfo(Form("Branch named %s is opened: 0x%z",
// 		  fgkTrackSegmentsBranchName.Data(),branch));
  if (branch == 0) 
   {//easy, maybe just a new tree
    IlcError(Form("Cannot find branch named %s",
		  fgkTrackSegmentsBranchName.Data()));
    return 0;
  }

  branch->SetAddress(trkref);//connect branch to buffer sitting in folder
  branch->GetEntry(0);//get first event 
  
  return 0;
}
//____________________________________________________________________________ 

Int_t IlcRSTACKLoader::ReadRecParticles()
{
//Reads Reconstructed  Particles from file
//Creates and posts to folder an array container, 
//connects branch in tree (if exists), and reads data to arry
  
  TObject** recpartref = RecParticlesRef();
  
  if ( recpartref == 0x0 )   
   {//Create and post array
    MakeRecParticlesArray();
    recpartref = RecParticlesRef();
   }

  TTree * treeP = TreeP();
  if(treeP==0)
   {
     //May happen if file is truncated or new in LoadSDigits, 
     //or the file is in update mode, 
     //but tracking was not performed yet for a current event
     //     IlcError("There is no Tree with Tracks and Reconstructed Particles");
     return 0;
   }
  
  TBranch * branch = treeP->GetBranch(fgkRecParticlesBranchName);
  if (branch == 0) 
   {//easy, maybe just a new tree
    IlcError(Form("Cannot find branch %s",
		  fgkRecParticlesBranchName.Data())); 
    return 0;
  }

  branch->SetAddress(recpartref);//connect branch to buffer sitting in folder
  branch->GetEntry(0);//get first event 
  
  return 0;
}


/***************************************************************************************/

IlcRSTACKLoader* IlcRSTACKLoader::GetRSTACKLoader(const  char* eventfoldername)
{
  // Return RSTACK loader
  IlcRunLoader* rn  = IlcRunLoader::GetRunLoader(eventfoldername);
  if (rn == 0x0) {
    printf("Can not find Run Loader in folder %s", eventfoldername);
    return 0x0 ; 
  }
  return dynamic_cast<IlcRSTACKLoader*>(rn->GetLoader("RSTACKLoader"));
}
/***************************************************************************************/

Bool_t IlcRSTACKLoader::BranchExists(const TString& recName)
{
  // Check if a branch named redName exists
  if (fBranchTitle.IsNull()) return kFALSE;
  TString dataname, zername ;
  TTree* tree;
  if(recName == "SDigits")
   {
    tree = TreeS();
    dataname = GetDetectorName();
    zername = "IlcRSTACKSDigitizer" ;
   }
  else
    if(recName == "Digits"){
      tree = TreeD();
      dataname = GetDetectorName();
      zername = "IlcRSTACKDigitizer" ;
    }
    else
      if(recName == "RecPoints"){
       tree = TreeR();
       dataname = fgkEmcRecPointsBranchName;
       zername = "IlcRSTACKClusterizer" ;
      }
      else
       if(recName == "TrackSegments"){
         tree = TreeT();
         dataname = fgkTrackSegmentsBranchName;
         zername = "IlcRSTACKTrackSegmentMaker";
       }        
       else
         if(recName == "RecParticles"){
           tree = TreeP();
           dataname = fgkRecParticlesBranchName;
           zername = "IlcRSTACKPID";
         }
         else
           return kFALSE ;

  
  if(!tree ) 
    return kFALSE ;

  TObjArray * lob = static_cast<TObjArray*>(tree->GetListOfBranches()) ;
  TIter next(lob) ; 
  TBranch * branch = 0 ;  
  TString titleName(fBranchTitle);
  titleName+=":";

  while ((branch = (static_cast<TBranch*>(next())))) {
    TString branchName(branch->GetName() ) ; 
    TString branchTitle(branch->GetTitle() ) ;  
    if ( branchName.BeginsWith(dataname) && branchTitle.BeginsWith(fBranchTitle) ){  
      IlcWarning(Form("branch %s  with title  %s ",
		      dataname.Data(),fBranchTitle.Data()));
      return kTRUE ;
    }
    if ( branchName.BeginsWith(zername) &&  branchTitle.BeginsWith(titleName) ){
      IlcWarning(Form("branch IlcRSTACK... with title  %s ",
		      branch->GetTitle()));
      return kTRUE ; 
    }
  }
  return kFALSE ;

 }

void IlcRSTACKLoader::SetBranchTitle(const TString& btitle)
{
  // Set branch title
  if (btitle.CompareTo(fBranchTitle) == 0) return;
  fBranchTitle = btitle;
  ReloadAll();
 }
 
//____________________________________________________________________________ 
//Again, must be const not to hide virtual functions from IlcLoader
//but there are calls to non-const functions, so I have to const_cast this pointer
void IlcRSTACKLoader::CleanHits()const
{
  // Clean Hits array
  IlcLoader::CleanHits();
  //Clear an array 
  TClonesArray* hits = const_cast<IlcRSTACKLoader *>(this)->Hits();
  if (hits) hits->Clear();
}
//____________________________________________________________________________ 

void IlcRSTACKLoader::CleanSDigits()const
{
  // Clean SDigits array
  IlcLoader::CleanSDigits();
  TClonesArray* sdigits = const_cast<IlcRSTACKLoader *>(this)->SDigits();
  if (sdigits) sdigits->Clear();
  
}
//____________________________________________________________________________ 

void IlcRSTACKLoader::CleanDigits()const
{
  // Clean Digits array
  IlcLoader::CleanDigits();
  TClonesArray* digits = const_cast<IlcRSTACKLoader *>(this)->Digits();
  if (digits) digits->Clear();
}
//____________________________________________________________________________ 

void IlcRSTACKLoader::CleanRecPoints()const
{
  // Clean RecPoints array
  IlcLoader::CleanRecPoints();
  TObjArray* recpoints = const_cast<IlcRSTACKLoader *>(this)->EmcRecPoints();
  if (recpoints) recpoints->Clear();
  recpoints = const_cast<IlcRSTACKLoader *>(this)->CpvRecPoints();
  if (recpoints) recpoints->Clear();
}
//____________________________________________________________________________ 

void IlcRSTACKLoader::CleanTracks()const
{
  //Cleans Tracks stuff
  IlcLoader::CleanTracks();//tree
  
  //and clear the array
  TClonesArray* tracks = const_cast<IlcRSTACKLoader *>(this)->TrackSegments();
  if (tracks) tracks->Clear();

}
//____________________________________________________________________________ 

void IlcRSTACKLoader::CleanRecParticles()
 {
   // Clean RecParticles array
   TClonesArray *recpar = RecParticles();
   if (recpar) recpar->Clear();
  
 
 }
//____________________________________________________________________________ 

void IlcRSTACKLoader::MakeHitsArray()
{
  // Add Hits array to the data folder
  if (Hits()) return;
  TClonesArray* hits = new TClonesArray("IlcRSTACKHit",1000);
  hits->SetName(fgkHitsName);
  GetDetectorDataFolder()->Add(hits);
}

//____________________________________________________________________________ 
void IlcRSTACKLoader::MakeSDigitsArray()
{
  // Add SDigits array to the data folder
  if ( SDigits()) return;
  TClonesArray* sdigits = new TClonesArray("IlcRSTACKDigit",1);
  sdigits->SetName(fgkSDigitsName);
  GetDetectorDataFolder()->Add(sdigits);
}

//____________________________________________________________________________ 
void IlcRSTACKLoader::MakeDigitsArray()
{
  // Add Digits array to the data folder
  if ( Digits()) return;
  TClonesArray* digits = new TClonesArray("IlcRSTACKDigit",1);
  digits->SetName(fgkDigitsName);
  GetDetectorDataFolder()->Add(digits);
  
}

//____________________________________________________________________________ 
void IlcRSTACKLoader::MakeRecPointsArray()
{
  // Add RecPoints array to the data folder
  if ( EmcRecPoints() == 0x0)
   {
     IlcDebug(9, "Making array for EMC");
    TObjArray* emc = new TObjArray(100) ;
    emc->SetName(fgkEmcRecPointsName) ;
    GetDetectorDataFolder()->Add(emc);
   }

  if ( CpvRecPoints() == 0x0)
   { 
     IlcDebug(9, "Making array for CPV");
     TObjArray* cpv = new TObjArray(100) ;
     cpv->SetName(fgkCpvRecPointsName);
     GetDetectorDataFolder()->Add(cpv);
   }
}

//____________________________________________________________________________ 
void IlcRSTACKLoader::MakeTrackSegmentsArray()
{
  // Add TrackSegments array to the data folder
  if ( TrackSegments()) return;
  TClonesArray * ts = new TClonesArray("IlcRSTACKTrackSegment",100) ;
  ts->SetName(fgkTracksName);
  GetDetectorDataFolder()->Add(ts);

}

//____________________________________________________________________________ 
void IlcRSTACKLoader::MakeRecParticlesArray()
{
  // Add RecParticles array to the data folder
  if ( RecParticles()) return;
  TClonesArray * rp = new TClonesArray("IlcRSTACKRecParticle",100) ;
  rp->SetName(fgkRecParticlesName);
  GetDetectorDataFolder()->Add(rp);
}
