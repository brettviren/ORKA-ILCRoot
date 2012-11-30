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


//_________________________________________________________________________
// This is a TTask that makes SDigits out of Hits
// The name of the TTask is also the title of the branch that will contain 
// the created SDigits
// The title of the TTAsk is the name of the file that contains the hits from
// which the SDigits are created
// A Summable Digits correspond to one hits originated 
// from one primary in one scintillating tile in 1 ps time gate
// At the number of photon it is applyed Poisson smearing, attenuation length
// and geometric collection efficiency
// A threshold for assignment of the primary to SDigit is applied 
// SDigits are written to TreeS, branch "RSTACK"
// IlcRSTACKSDigitizer with all current parameters is written 
// to TreeS branch "IlcRSTACKSDigitizer".
// Both branches have the same title. If necessary one can produce 
// another set of SDigits with different parameters. Two versions
// can be distunguished using titles of the branches.
//
//////////////////////////////////////////////////////////////////////////////


// --- ROOT system ---
#include "TRandom.h"
#include "TBenchmark.h"

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcLog.h"
#include "IlcRSTACKGeometry.h" 
#include "IlcRSTACKDigit.h"
#include "IlcRSTACKLoader.h"
#include "IlcRSTACKHit.h"
#include "IlcRSTACKSDigitizer.h"
#include "IlcRSTACKSimParam.h"

ClassImp(IlcRSTACKSDigitizer)

           
//____________________________________________________________________________ 
IlcRSTACKSDigitizer::IlcRSTACKSDigitizer() : 
  TNamed("",""),
  fPrimThreshold(0.f),
  fCollectEff(0.f),
  fAttenuationLength(0.f),
  fSiPMPDE(0.f),
  fDefaultInit(kTRUE),
  fEventFolderName(""),
  fInit(kFALSE),
  fSDigitsInRun(0),
  fFirstEvent(0),
  fLastEvent(0)
{
  // ctor
}

//____________________________________________________________________________ 
IlcRSTACKSDigitizer::IlcRSTACKSDigitizer(const char * ilcrunFileName, 
				     const char * eventFolderName):
  TNamed("RSTACKSDigitizer", ilcrunFileName),
  fPrimThreshold(0.f),
  fCollectEff(0.f),
  fAttenuationLength(0.f),
  fSiPMPDE(0.f),
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
IlcRSTACKSDigitizer::IlcRSTACKSDigitizer(const IlcRSTACKSDigitizer& sd) :
  TNamed(sd.GetName(), sd.GetTitle()),
  fPrimThreshold(sd.fPrimThreshold),
  fCollectEff(sd.fCollectEff),
  fAttenuationLength(sd.fAttenuationLength),
  fSiPMPDE(sd.fSiPMPDE),
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
IlcRSTACKSDigitizer& IlcRSTACKSDigitizer::operator = (const IlcRSTACKSDigitizer& qa)
{
// assignment operator

  this->~IlcRSTACKSDigitizer();
  new(this) IlcRSTACKSDigitizer(qa);
  return *this;
}

//____________________________________________________________________________ 
IlcRSTACKSDigitizer::~IlcRSTACKSDigitizer() 
{
  //dtor
}

//____________________________________________________________________________ 
void IlcRSTACKSDigitizer::Init()
{
  // Uses the Loader to access the required files
  
  fInit = kTRUE ; 
  
  //to prevent cleaning of this object while GetEvent is called
  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  if (!rl) IlcRunLoader::Open(GetTitle(), fEventFolderName) ; 
}

//____________________________________________________________________________ 
void IlcRSTACKSDigitizer::InitParameters()
{ 
  // initializes the parameters for digitization
  fPrimThreshold = IlcRSTACKSimParam::GetInstance()->GetPrimaryThreshold() ; //Minimal number of photons to assign primary particle index to digit
  fCollectEff = IlcRSTACKSimParam::GetInstance()->GetCollectEff(); //geometric collection efficiency
  fAttenuationLength = IlcRSTACKSimParam::GetInstance()->GetAttenuationLength();
  fSiPMPDE = IlcRSTACKSimParam::GetInstance()->GetSiPMPDE();
  fSDigitsInRun  = 0 ;
}

//____________________________________________________________________________
void IlcRSTACKSDigitizer::Digitize(Option_t *option) 
{ 
  // Steering method to produce summable digits for events
  // in the range from fFirstEvent to fLastEvent.
  // This range is optionally set by SetEventRange().
  // if fLastEvent=-1 (by default), then process events until the end.
  //
  // A Summable Digits correspond to one hits originated 
  // from one primary in one scintillating tile in 1 ps time gate
  // At the number of photon it is applyed Poisson smearing, attenuation length
  // and geometric collection efficiency

  IlcRSTACKGeometry *geom;
  if (!(geom = IlcRSTACKGeometry::GetInstance()))
        geom = IlcRSTACKGeometry::GetInstance("ORKA","");

  if (strstr(option, "print") ) {
    Print() ; 
    return ; 
  }

  if(strstr(option,"tim"))
    gBenchmark->Start("RSTACKSDigitizer");
  
  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcRSTACKLoader * RSTACKLoader = static_cast<IlcRSTACKLoader*>(rl->GetLoader("RSTACKLoader"));
  
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
    TTree * treeS = RSTACKLoader->TreeS(); 
    if(!treeS){
      RSTACKLoader->MakeTree("S");
      treeS = RSTACKLoader->TreeS();
    }

    RSTACKLoader->CleanHits() ; 
    RSTACKLoader->LoadHits("READ") ;

    TClonesArray * hits    = RSTACKLoader->Hits() ;
    TClonesArray * sdigits = RSTACKLoader->SDigits() ;
    if( !sdigits ) {
      RSTACKLoader->MakeSDigitsArray() ; 
      sdigits = RSTACKLoader->SDigits() ;
    }
    sdigits->Clear();
    Int_t nSdigits = 0 ;

    //Now make SDigits from hits    
    for ( i = 0 ; i < hits->GetEntries() ; i++ ) {
      
      IlcRSTACKHit * hit = static_cast<IlcRSTACKHit *>(hits->At(i)) ;
      
      
      Float_t DistFromLowerZ = hit->GetDistFromLowerZ();
      Float_t RSTACKLength = geom->GetRSTACKLength();
      Float_t NHitPhotons = gRandom->Poisson(hit->GetNPhotons());
      Float_t NPhotons[2];
      //photons on that reach the left end of the scintillator tile
      NPhotons[0] = 0.5*NHitPhotons * TMath::Exp(-DistFromLowerZ/fAttenuationLength);
      //photons on that reach the right end of the scintillator tile
      NPhotons[1] = 0.5*NHitPhotons * TMath::Exp(-(RSTACKLength -  DistFromLowerZ) / fAttenuationLength);

      //Apply geometric collection efficiency
      NPhotons[0] *= fCollectEff*fSiPMPDE;
      NPhotons[1] *= fCollectEff*fSiPMPDE;
      // Assign primary track index number only if contribution is above the threshold 
      if( NHitPhotons > fPrimThreshold )
	new((*sdigits)[nSdigits]) IlcRSTACKDigit(hit->GetPrimary(),hit->GetId(), hit->GetEnergy(), NPhotons, hit->GetTime()) ;
      else
	new((*sdigits)[nSdigits]) IlcRSTACKDigit(-1               ,hit->GetId(), hit->GetEnergy(), NPhotons, hit->GetTime()) ;

      nSdigits++ ;
      
    }
 
    sdigits->Sort() ;

    nSdigits = sdigits->GetEntriesFast() ;

    fSDigitsInRun += nSdigits ;  
    sdigits->Expand(nSdigits) ;
    
    for (i = 0 ; i < nSdigits ; i++) {       
      IlcRSTACKDigit * digit = static_cast<IlcRSTACKDigit *>(sdigits->At(i)) ;
      digit->SetIndexInList(i) ;     
    }
    
	
    //Now write SDigits
    
    //First list of sdigits

    Int_t bufferSize = 32000 ;
    TBranch * sdigitsBranch = treeS->Branch("RSTACK",&sdigits,bufferSize);
    sdigitsBranch->Fill() ;

    RSTACKLoader->WriteSDigits("OVERWRITE");

    if(strstr(option,"deb"))
      PrintSDigits(option) ;
      
  }// event loop
  

  Unload();

  if(strstr(option,"tim")){
    gBenchmark->Stop("RSTACKSDigitizer");
    Info("Exec","   took %f seconds for SDigitizing  %f seconds per event",
	 gBenchmark->GetCpuTime("RSTACKSDigitizer"), 
	 gBenchmark->GetCpuTime("RSTACKSDigitizer")/nEvents) ;
  }
  
}

//__________________________________________________________________
void IlcRSTACKSDigitizer::Print(const Option_t *)const
{
  // Prints parameters of SDigitizer
  Info("Print", "\n------------------- %s -------------", GetName() ) ; 
  printf("   Writing SDigits to branch with title  %s\n", fEventFolderName.Data()) ;
  printf("   Threshold for Primary assignment= %f\n", fPrimThreshold)  ; 
  printf("---------------------------------------------------\n") ;
  
}

//__________________________________________________________________
Bool_t IlcRSTACKSDigitizer::operator==( IlcRSTACKSDigitizer const &sd )const
{
  // Equal operator.
  // SDititizers are equal if their threshold, geometric collection efficiency, and attenuation length are equal

  if(fPrimThreshold==sd.fPrimThreshold && fCollectEff==sd.fCollectEff && fAttenuationLength==sd.fAttenuationLength && fSiPMPDE==sd.fSiPMPDE )
    return kTRUE ;
  else
    return kFALSE ;
}

//__________________________________________________________________
void IlcRSTACKSDigitizer::PrintSDigits(Option_t * /*option*/)
{
  // Prints list of digits produced in the current pass of IlcRSTACKDigitizer

  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcRSTACKLoader * RSTACKLoader = static_cast<IlcRSTACKLoader*>(rl->GetLoader("RSTACKLoader"));

  // Get RSTACK Geometry object
  IlcRSTACKGeometry *geom;
  if (!(geom = IlcRSTACKGeometry::GetInstance())) 
        geom = IlcRSTACKGeometry::GetInstance("ORKA","");

  const TClonesArray * sdigits = RSTACKLoader->SDigits() ;
  
  Info( "\nPrintSDigits", "event # %d %d sdigits", gIlc->GetEvNumber(), sdigits->GetEntriesFast() ) ; 


  //loop over digits
  IlcRSTACKDigit * digit;
  printf("\nRSTACK sdigits\n") ; 
  Int_t maxTiles = geom->GetRSTACKNTilesInSector()*geom->GetRSTACKNSectorPhi();
  Int_t index ;
  for (index = 0 ; (index < sdigits->GetEntriesFast()) && 
    ((static_cast<IlcRSTACKDigit *> (sdigits->At(index)))->GetId() <= maxTiles) ; index++) {
    digit = static_cast<IlcRSTACKDigit *>( sdigits->At(index) ) ;
  //  if(digit->GetNprimary() == 0) 
    // 	continue;
    //       printf("%6d  %8d    %6.5e %4d      %2d :\n", // YVK
    printf("%6d  %.4f    %.4f    %.4f    %6.5e %4d      %2d :\n",
	   digit->GetId(), digit->GetEnergy(), digit->GetNPE()[0], digit->GetNPE()[1], digit->GetTime(), digit->GetIndexInList(), digit->GetNprimary()) ;  
	   Int_t iprimary;
	   for (iprimary=0; iprimary<digit->GetNprimary(); iprimary++) {
	     printf("%d ",digit->GetPrimary(iprimary+1) ) ; 
	   }  
    }

}

//____________________________________________________________________________ 
void IlcRSTACKSDigitizer::Unload() const
{
  // Unloads the objects from the folder
  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcRSTACKLoader * RSTACKLoader = static_cast<IlcRSTACKLoader*>(rl->GetLoader("RSTACKLoader"));
  
  RSTACKLoader->UnloadHits() ; 
  RSTACKLoader->UnloadSDigits() ; 
}
