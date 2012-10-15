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


/* $Id: IlcPVECSDigitizer.cxx 53656 2011-12-16 10:49:54Z hristov $ */


//_________________________________________________________________________
// This is a TTask that makes SDigits out of Hits
// The name of the TTask is also the title of the branch that will contain 
// the created SDigits
// The title of the TTAsk is the name of the file that contains the hits from
// which the SDigits are created
// A Summable Digits is the sum of all hits originating 
// from one primary in one active crystal
// A threshold for assignment of the primary to SDigit is applied 
// SDigits are written to TreeS, branch "PVEC"
// IlcPVECSDigitizer with all current parameters is written 
// to TreeS branch "IlcPVECSDigitizer".
// Both branches have the same title. If necessary one can produce 
// another set of SDigits with different parameters. Two versions
// can be distunguished using titles of the branches.
// User case:
//  root [0] IlcPVECSDigitizer * s = new IlcPVECSDigitizer("gilc.root")
//  Warning in <TDatabasePDG::TDatabasePDG>: object already instantiated
//  root [1] s->Digitize()
//             // Makes SDigitis for all events stored in gilc.root
//  root [2] s->SetPedestalParameter(0.001)
//             // One can change parameters of digitization
// root [3] s->SetSDigitsBranch("Pedestal 0.001")
//             // and write them into the new branch
// root [4] s->ExecuteTask("deb all tim")
//             // available parameters:
//             deb - print # of produced SDigitis
//             deb all  - print # and list of produced SDigits
//             tim - print benchmarking information
//
//-- Author :  Dmitri Peressounko (SUBATECH & KI) 
//////////////////////////////////////////////////////////////////////////////


// --- ROOT system ---
#include "TBenchmark.h"

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcLog.h"
#include "IlcPVECGeometry.h" 
#include "IlcPVECDigit.h"
#include "IlcPVECLoader.h"
#include "IlcPVECHit.h"
#include "IlcPVECSDigitizer.h"

ClassImp(IlcPVECSDigitizer)

           
//____________________________________________________________________________ 
IlcPVECSDigitizer::IlcPVECSDigitizer() : 
  TNamed("",""),
  fPrimThreshold(0.f),
  fDefaultInit(kTRUE),
  fEventFolderName(""),
  fInit(kFALSE),
  fSDigitsInRun(0),
  fFirstEvent(0),
  fLastEvent(0)
{
  // ctor
  // Intialize the quality assurance data maker 	
}

//____________________________________________________________________________ 
IlcPVECSDigitizer::IlcPVECSDigitizer(const char * ilcrunFileName, 
				     const char * eventFolderName):
  TNamed("PVECSDigitizer", ilcrunFileName),
  fPrimThreshold(0.f),
  fDefaultInit(kFALSE),
  fEventFolderName(eventFolderName),
  fInit(kFALSE),
  fSDigitsInRun(0),
  fFirstEvent(0),
  fLastEvent(0)
{
  // ctor
  InitParameters() ; 
  Init();
  fDefaultInit = kFALSE ; 
}

//____________________________________________________________________________
IlcPVECSDigitizer::IlcPVECSDigitizer(const IlcPVECSDigitizer& sd) :
  TNamed(sd.GetName(), sd.GetTitle()),
  fPrimThreshold(sd.fPrimThreshold),
  fDefaultInit(kFALSE),
  fEventFolderName(sd.fEventFolderName),
  fInit(kFALSE),
  fSDigitsInRun(sd.fSDigitsInRun),
  fFirstEvent(sd.fFirstEvent),
  fLastEvent(sd.fLastEvent)
{ 
  // cpy ctor
}

//_____________________________________________________________________________
IlcPVECSDigitizer& IlcPVECSDigitizer::operator = (const IlcPVECSDigitizer& qa)
{
// assignment operator

  this->~IlcPVECSDigitizer();
  new(this) IlcPVECSDigitizer(qa);
  return *this;
}

//____________________________________________________________________________ 
IlcPVECSDigitizer::~IlcPVECSDigitizer() 
{
  //dtor
}

//____________________________________________________________________________ 
void IlcPVECSDigitizer::Init()
{
  // Uses the Loader to access the required files
  
  fInit = kTRUE ; 
  
  //to prevent cleaning of this object while GetEvent is called
  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  if (!rl) IlcRunLoader::Open(GetTitle(), fEventFolderName) ; 
}

//____________________________________________________________________________ 
void IlcPVECSDigitizer::InitParameters()
{ 
  // initializes the parameters for digitization
  fPrimThreshold = 0.01 ;
  fSDigitsInRun  = 0 ;
}

//____________________________________________________________________________
void IlcPVECSDigitizer::Digitize(Option_t *option) 
{ 
  // Steering method to produce summable digits for events
  // in the range from fFirstEvent to fLastEvent.
  // This range is optionally set by SetEventRange().
  // if fLastEvent=-1 (by default), then process events until the end.
  //
  // Summable digit is a sum of all hits in the same active
  // volume into digit
  
  if (strstr(option, "print") ) {
    Print() ; 
    return ; 
  }

  if(strstr(option,"tim"))
    gBenchmark->Start("PVECSDigitizer");
  
  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcPVECLoader * PVECLoader = static_cast<IlcPVECLoader*>(rl->GetLoader("PVECLoader"));
  
  //switch off reloading of this task while getting event
  if (!fInit) { // to prevent overwrite existing file
    IlcError( Form("Give a version name different from %s", fEventFolderName.Data()) ) ;
    return ;
  }
  
  if (fLastEvent == -1) 
    fLastEvent = rl->GetNumberOfEvents() - 1 ;
  else 
    fLastEvent = TMath::Min(fFirstEvent, rl->GetNumberOfEvents()); // only one event at the time 
  Int_t nEvents   = fLastEvent - fFirstEvent + 1;
  
  Int_t ievent, i;

  for (ievent = fFirstEvent; ievent <= fLastEvent; ievent++) {    
    rl->GetEvent(ievent) ;
    TTree * treeS = PVECLoader->TreeS(); 
    if(!treeS){
      PVECLoader->MakeTree("S");
      treeS = PVECLoader->TreeS();
    }

    PVECLoader->CleanHits() ; 
    PVECLoader->LoadHits("READ") ;

    TClonesArray * hits    = PVECLoader->Hits() ;
    TClonesArray * sdigits = PVECLoader->SDigits() ;
    if( !sdigits ) {
      PVECLoader->MakeSDigitsArray() ; 
      sdigits = PVECLoader->SDigits() ;
    }
    sdigits->Clear();
    Int_t nSdigits = 0 ;

    //Now make SDigits from hits, for PVEC it is the same, so just copy    
    for ( i = 0 ; i < hits->GetEntries() ; i++ ) {
      
      IlcPVECHit * hit = static_cast<IlcPVECHit *>(hits->At(i)) ;
      // Assign primary number only if contribution is significant

      //********************************************************************************
      //********************************************************************************
      //****
      //****  Here hits information have to be elaborated electrons
      //****  to convert ligth yeld in photo electrons
      //****  ( hit->GetEnergy() ) in the light yield
      //****  
      //****  This conversion needs to be implemented, now raw light yield is used
      //****
      //********************************************************************************
      //********************************************************************************


      if( hit->GetEnergy() > fPrimThreshold)
	new((*sdigits)[nSdigits]) IlcPVECDigit(hit->GetPrimary(),hit->GetId(),
					       hit->GetEnergy() ,hit->GetTime()) ;
      else
	new((*sdigits)[nSdigits]) IlcPVECDigit(-1               ,hit->GetId(), 
					       hit->GetEnergy() ,hit->GetTime()) ;
      nSdigits++ ;	
      
    }
 
    sdigits->Sort() ;

    nSdigits = sdigits->GetEntriesFast() ;

    fSDigitsInRun += nSdigits ;  
    sdigits->Expand(nSdigits) ;
    
    for (i = 0 ; i < nSdigits ; i++) {       
      IlcPVECDigit * digit = static_cast<IlcPVECDigit *>(sdigits->At(i)) ;
      digit->SetIndexInList(i) ;     
    }
    
    //Now write SDigits

    
    //First list of sdigits

    Int_t bufferSize = 32000 ;
    TBranch * sdigitsBranch = treeS->Branch("PVEC",&sdigits,bufferSize);
    sdigitsBranch->Fill() ;

    PVECLoader->WriteSDigits("OVERWRITE");

    if(strstr(option,"deb"))
      PrintSDigits(option) ;
      
  }// event loop
  

  Unload();

  if(strstr(option,"tim")){
    gBenchmark->Stop("PVECSDigitizer");
    Info("Exec","   took %f seconds for SDigitizing  %f seconds per event",
	 gBenchmark->GetCpuTime("PVECSDigitizer"), 
	 gBenchmark->GetCpuTime("PVECSDigitizer")/nEvents) ;
  }
  
}

//__________________________________________________________________
void IlcPVECSDigitizer::Print(const Option_t *)const
{
  // Prints parameters of SDigitizer
  Info("Print", "\n------------------- %s -------------", GetName() ) ; 
  printf("   Writing SDigits to branch with title  %s\n", fEventFolderName.Data()) ;
  printf("   Threshold for Primary assignment= %f\n", fPrimThreshold)  ; 
  printf("---------------------------------------------------\n") ;
  
}

//__________________________________________________________________
Bool_t IlcPVECSDigitizer::operator==( IlcPVECSDigitizer const &sd )const
{
  // Equal operator.
  // SDititizers are equal if their pedestal, slope and threshold are equal

  if(fPrimThreshold==sd.fPrimThreshold)
    return kTRUE ;
  else
    return kFALSE ;
}

//__________________________________________________________________
void IlcPVECSDigitizer::PrintSDigits(Option_t * /*option*/)
{
  // Prints list of digits produced in the current pass of IlcPVECDigitizer

  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcPVECLoader * PVECLoader = static_cast<IlcPVECLoader*>(rl->GetLoader("PVECLoader"));

  // Get PVEC Geometry object
  IlcPVECGeometry *geom;
  if (!(geom = IlcPVECGeometry::GetInstance())) 
        geom = IlcPVECGeometry::GetInstance("ORKA","");

  const TClonesArray * sdigits = PVECLoader->SDigits() ;
  
  Info( "\nPrintSDigits", "event # %d %d sdigits", gIlc->GetEvNumber(), sdigits->GetEntriesFast() ) ; 


  //loop over digits
  IlcPVECDigit * digit;
  printf("\nEMC sdigits\n") ; 
  Int_t maxEmc = 100;//FIXME
  Int_t index ;
  for (index = 0 ; (index < sdigits->GetEntriesFast()) && 
    ((static_cast<IlcPVECDigit *> (sdigits->At(index)))->GetId() <= maxEmc) ; index++) {
    digit = static_cast<IlcPVECDigit *>( sdigits->At(index) ) ;
  //  if(digit->GetNprimary() == 0) 
    // 	continue;
    //       printf("%6d  %8d    %6.5e %4d      %2d :\n", // YVK
    printf("%6d  %.4f    %6.5e %4d      %2d :\n",
	   digit->GetId(), digit->GetEnergy(), digit->GetTime(), digit->GetIndexInList(), digit->GetNprimary()) ;  
	   Int_t iprimary;
	   for (iprimary=0; iprimary<digit->GetNprimary(); iprimary++) {
	     printf("%d ",digit->GetPrimary(iprimary+1) ) ; 
	   }  
    }    

}

//____________________________________________________________________________ 
void IlcPVECSDigitizer::Unload() const
{
  // Unloads the objects from the folder
  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcPVECLoader * PVECLoader = static_cast<IlcPVECLoader*>(rl->GetLoader("PVECLoader"));
  
  PVECLoader->UnloadHits() ; 
  PVECLoader->UnloadSDigits() ; 
}
