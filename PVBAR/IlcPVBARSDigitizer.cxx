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


/* $Id: IlcPVBARSDigitizer.cxx 53656 2011-12-16 10:49:54Z hristov $ */


//_________________________________________________________________________
// This is a TTask that makes SDigits out of Hits
// The name of the TTask is also the title of the branch that will contain 
// the created SDigits
// The title of the TTAsk is the name of the file that contains the hits from
// which the SDigits are created
// A Summable Digits is the sum of all hits originating 
// from one primary in one active cell
// A threshold for assignment of the primary to SDigit is applied 
// SDigits are written to TreeS, branch "PVBAR"
// IlcPVBARSDigitizer with all current parameters is written 
// to TreeS branch "IlcPVBARSDigitizer".
// Both branches have the same title. If necessary one can produce 
// another set of SDigits with different parameters. Two versions
// can be distunguished using titles of the branches.
// User case:
//  root [0] IlcPVBARSDigitizer * s = new IlcPVBARSDigitizer("gilc.root")
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
#include "TRandom.h"
#include "TBenchmark.h"

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcLog.h"
#include "IlcPVBARGeometry.h" 
#include "IlcPVBARDigit.h"
#include "IlcPVBARLoader.h"
#include "IlcPVBARHit.h"
#include "IlcPVBARSDigitizer.h"
#include "IlcPVBARSimParam.h"

ClassImp(IlcPVBARSDigitizer)

           
//____________________________________________________________________________ 
IlcPVBARSDigitizer::IlcPVBARSDigitizer() : 
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
  // Intialize the quality assurance data maker 	
}

//____________________________________________________________________________ 
IlcPVBARSDigitizer::IlcPVBARSDigitizer(const char * ilcrunFileName, 
				     const char * eventFolderName):
  TNamed("PVBARSDigitizer", ilcrunFileName),
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
IlcPVBARSDigitizer::IlcPVBARSDigitizer(const IlcPVBARSDigitizer& sd) :
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
IlcPVBARSDigitizer& IlcPVBARSDigitizer::operator = (const IlcPVBARSDigitizer& qa)
{
// assignment operator

  this->~IlcPVBARSDigitizer();
  new(this) IlcPVBARSDigitizer(qa);
  return *this;
}

//____________________________________________________________________________ 
IlcPVBARSDigitizer::~IlcPVBARSDigitizer() 
{
  //dtor
}

//____________________________________________________________________________ 
void IlcPVBARSDigitizer::Init()
{
  // Uses the Loader to access the required files
  
  fInit = kTRUE ; 
  
  //to prevent cleaning of this object while GetEvent is called
  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  if (!rl) IlcRunLoader::Open(GetTitle(), fEventFolderName) ; 
}

//____________________________________________________________________________ 
void IlcPVBARSDigitizer::InitParameters()
{ 
  // initializes the parameters for digitization
  fPrimThreshold = IlcPVBARSimParam::GetInstance()->GetPrimaryThreshold() ; //Minimal number of photons to assign primary particle index to digit
  fCollectEff = IlcPVBARSimParam::GetInstance()->GetCollectEff(); //geometric collection efficiency
  fAttenuationLength = IlcPVBARSimParam::GetInstance()->GetAttenuationLength();
  fSiPMPDE = IlcPVBARSimParam::GetInstance()->GetSiPMPDE();
  fSDigitsInRun  = 0 ;
}

//____________________________________________________________________________
void IlcPVBARSDigitizer::Digitize(Option_t *option) 
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
  
  IlcPVBARGeometry *geom;
  if (!(geom = IlcPVBARGeometry::GetInstance()))
        geom = IlcPVBARGeometry::GetInstance("ORKA","");

  if (strstr(option, "print") ) {
    Print() ; 
    return ; 
  }

  if(strstr(option,"tim"))
    gBenchmark->Start("PVBARSDigitizer");
  

// 	// check the QA result for RAWS
//   IlcQAv1 * qa = IlcQAv1::Instance(IlcQAv1::kPVBAR) ; 
//   if ( qa->IsSet(IlcQAv1::kPVBAR, IlcQAv1::kRAW, IlcQAv1::kFATAL)) {
// 	IlcFatal("QA status in RAW was Fatal") ;
//   } else if ( qa->IsSet(IlcQAv1::kPVBAR, IlcQAv1::kRAW, IlcQAv1::kERROR)) {
// 	IlcError("QA status in RAW was Error") ;
//   } else if ( qa->IsSet(IlcQAv1::kPVBAR, IlcQAv1::kRAW, IlcQAv1::kWARNING) ) {
// 	IlcWarning("QA status in RAW was Warning") ;
//   } else if ( qa->IsSet(IlcQAv1::kPVBAR, IlcQAv1::kRAW, IlcQAv1::kINFO) ) {
// 	IlcInfo("QA status in RAW was Info") ;
//   }

  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcPVBARLoader * PVBARLoader = static_cast<IlcPVBARLoader*>(rl->GetLoader("PVBARLoader"));
  
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
    TTree * treeS = PVBARLoader->TreeS(); 
    if(!treeS){
      PVBARLoader->MakeTree("S");
      treeS = PVBARLoader->TreeS();
    }

    PVBARLoader->CleanHits() ; 
    PVBARLoader->LoadHits("READ") ;

    TClonesArray * hits    = PVBARLoader->Hits() ;
    TClonesArray * sdigits = PVBARLoader->SDigits() ;
    if( !sdigits ) {
      PVBARLoader->MakeSDigitsArray() ; 
      sdigits = PVBARLoader->SDigits() ;
    }
    sdigits->Clear();
    Int_t nSdigits = 0 ;

    //Now make SDigits from hits, for PVBAR it is the same, so just copy    
    for ( i = 0 ; i < hits->GetEntries() ; i++ ) {
      
      IlcPVBARHit * hit = static_cast<IlcPVBARHit *>(hits->At(i)) ;
      Int_t relid[4];
      geom->AbsToRelNumbering(hit->GetId(),relid);

      if(relid[3] == 1){ //Scintillating hit
	Float_t DistFromLowerZ = hit->GetDistFromLowerZ();
	Float_t PVBARLength = geom->GetPVBARLength();
	Float_t NHitPhotons = gRandom->Poisson(hit->GetNPhotons());
	Float_t NPhotons[4] = {0.,0.,0.,0.};
	//photons on that reach the left end of the scintillator tile
	NPhotons[2] = 0.5*NHitPhotons * TMath::Exp(-DistFromLowerZ/fAttenuationLength);
	//photons on that reach the right end of the scintillator tile
	NPhotons[3] = 0.5*NHitPhotons * TMath::Exp(-(PVBARLength -  DistFromLowerZ) / fAttenuationLength);
	
	//Apply geometric collection efficiency
	NPhotons[0] = 0.;
	NPhotons[1] = 0.;
	NPhotons[2] *= fCollectEff*fSiPMPDE;
	NPhotons[3] *= fCollectEff*fSiPMPDE;
	// Assign primary number only if contribution is significant
	
	if( hit->GetEnergy() > fPrimThreshold*0+1.e-6)
	  new((*sdigits)[nSdigits]) IlcPVBARDigit(hit->GetPrimary(),hit->GetId(),
						  hit->GetEnergy(), NPhotons ,hit->GetTime()) ;
	else
	  new((*sdigits)[nSdigits]) IlcPVBARDigit(-1               ,hit->GetId(), 
						  hit->GetEnergy(), NPhotons ,hit->GetTime()) ;
        nSdigits++ ;

      }
      else if(relid[3] == 0){ //Cerenkov hit

	Float_t fDollanMCFactor_LG=1.0;
	Float_t fDollanMCFactor_WLS=1.0;
	Float_t DistFromLowerZ = hit->GetDistFromLowerZ();
	Float_t PVBARLength = geom->GetPVBARLength();
	Float_t NHitPhotons = /*gRandom->Poisson(*/hit->GetNPhotons()/*)*/;
	Float_t NPhotons[4] = {0.,0.,0.,0.};

	const Int_t lambdabins = 64;  // Number of bins
	Float_t lambdawidth = 5.;     // Wavelength step
	Float_t lambdazero = 300.;    // Lowest limit of wavelength interval

	// Compute the number of gammas per wavelength range
	for ( Int_t nwl = 0; nwl < lambdabins; nwl++ )  // <-----  Start loop over the wavelength intervals
	{
	  Float_t lambda1 = lambdazero + nwl*lambdawidth;
	  Float_t lambda2 = lambda1 + lambdawidth;

	  // random wavelenght in the interval
	  Float_t r_lambda = gRandom->Rndm();
	  Float_t lambda_rndm = lambda1 * lambda2 / ( lambda2 - r_lambda * ( lambda2 - lambda1 ) );

	  Float_t Ngamma = NHitPhotons;


// Next, it calculates the number of photons that reach the photodetector
	  if ( Ngamma > 0. ) {


//       Final part consider the photon survival physics:
//       numerical aperture, attenuation, HEM, QE, ...
//
/**********************************************************************************************************
      Lead Glass Light Collection Block
**********************************************************************************************************/

// 1) Calculate probability of surviving for the pe along the leadglass (both sides). Takes into account the photon emission angle.
      Float_t z_metersLG[2] = {DistFromLowerZ / 100., (PVBARLength-DistFromLowerZ) / 100.};
      Float_t dBLG[2];
      Float_t Al_LG = AttenuationLength_m_LG( lambda_rndm );

      dBLG[0]= z_metersLG[0]/Al_LG ;
      dBLG[1]= z_metersLG[1]/Al_LG ;
      Float_t probLG[2];
      probLG[0] = TMath::Exp ( - dBLG[0]  );
      probLG[1] = TMath::Exp ( - dBLG[1]  );
       
// 2) Correction factor from Dollan work
//       Float_t DollanFactor_LG = 18.2/65.; 
      Float_t dQE_LG=0.;
       
       dQE_LG =QE_SiPM ( lambda_rndm );
      
       NPhotons[0] += 0.5*Ngamma*probLG[0]*dQE_LG*fDollanMCFactor_LG;
       NPhotons[1] += 0.5*Ngamma*probLG[1]*dQE_LG*fDollanMCFactor_LG;
      
/**********************************************************************************************************
      WLS Light Collection Block
      Fast calculation uses efficiencies by Ralph Dollan (Thesis 2004)
      Full calculation uses paramtrized absorption length and QE
**********************************************************************************************************/
//       Bool_t bFullCalculation = kTRUE;
      
      // 1) Probability of surviving for the pe along the leadglass until it reaches a WLS fiber
//       Float_t xy_metersLG = xyDistanceFromWLS *Cos_Theta_Cerenkov_Recip/ 100.;
      Float_t dBWLSxy[2], probWLSxy[2];
	  // ORKA has transversal WLS fibers/plates. Already calculated above
	  dBWLSxy[0] = dBLG[0];
	  dBWLSxy[1] = dBLG[1];

      probWLSxy[0] = TMath::Exp ( - dBWLSxy[0]  );
      probWLSxy[1] = TMath::Exp ( - dBWLSxy[1]  );
      //IlcDebug(4,Form("xy_metersLG: %f ;   dBWLSxy: %f ; probWLSxy: %f ;", xy_metersLG, dBWLSxy, probWLSxy));

      // 2) Probability of the photon entering the WLS: its angle must be larger than theta_max
      Float_t dProbPenetration = 1.; // for ORKA, assume all cerenkov photons enter the WLS plates
//       Float_t en_OpticalGel = 1.45;
//       Float_t en_Cladding = 1.49;
//       Float_t en_Intermediate = TMath::Sqrt(en_OpticalGel*en_Cladding);
//       Float_t en_Core = 1.59;

      // 3) Probability of the photon being captured by the WLS: it decreases while the photon travels longitudinally along crystal and it depends on photon wavelength
      Float_t AbsorptionEff_WLSz[2];
      // this is really 100% for ORKA
      AbsorptionEff_WLSz[0] = 1.0 ;
      AbsorptionEff_WLSz[1] = 1.0 ;


      // 4) Conversion efficiency the WLS
      Float_t dconversionWLS = 1. ; //Everythingh already in AbsorptionLength_WLS and QE
      // 4.1) Correction factor from Dollan work
//       Float_t DollanFactor_WLS = 1.299; //12 WLS fibers phi=1.5 mm per 4x4 cm^2 cell
//       Float_t DollanFactor_WLS = 0.7; //4 WLS fibers phi=1.4 mm per 4x4 cm^2 cell
//       Float_t DollanFactor_WLS = 0.5; //4 WLS fibers phi=1.0 mm per 4x4 cm^2 cell
//       Float_t WLSxCellFactor = 0.5; //4 WLS fibers phi=1.0 mm per 4x4 cm^2 cell
      Float_t WLSxCellFactor = 0.949; //10 WLS fibers phi=1.2 mm per 4x4 cm^2 cell

      // 5) Probability of surviving for the pe along the WLS because of self absortion
      Float_t z_metersWLS[2] = {DistFromLowerZ / 100., (PVBARLength-DistFromLowerZ) / 100.};  // ligth travel both sides of WLS

      Float_t Al_WLS = AttenuationLength_m_WLS( lambda_rndm );
      Float_t dBWLSz[2] = {z_metersWLS[0]/Al_WLS, z_metersWLS[1]/Al_WLS };
      Float_t probWLSz[2] = {TMath::Exp ( - dBWLSz[0]  ) , TMath::Exp ( - dBWLSz[1]  ) };

      // 6) Shift the frequency in the WLS and compute the new bin
      Float_t r_lambda_shift = Shifter_WLS(lambda_rndm);
//       Int_t nwl_shifted = (Int_t)( lambdabins*(r_lambda_shift-lambdazero)/((lambdabins+1)*lambdawidth) );

      // 7) Probability that the reemitted photon remains in the WLS fiber
	  // This is the solid angle of the fibers and it is about 4.8%
      Float_t acceptance_WLS = 0.048;

      // 8) The QE for the shifted photon
      Float_t dQE_WLS;
      dQE_WLS =QE_SiPM ( r_lambda_shift );

      NPhotons[2] += acceptance_WLS*Ngamma*(probWLSxy[0]+probWLSxy[1])*dProbPenetration*AbsorptionEff_WLSz[0]*dconversionWLS*WLSxCellFactor*probWLSz[0]*dQE_WLS*fDollanMCFactor_WLS;

      NPhotons[3] += acceptance_WLS*Ngamma*(probWLSxy[0]+probWLSxy[1])*dProbPenetration*AbsorptionEff_WLSz[1]*dconversionWLS*WLSxCellFactor*probWLSz[1]*dQE_WLS*fDollanMCFactor_WLS;

    }

  }

// // // // 	//photons on that reach the left end of the scintillator tile
// // // // 	NPhotons[0] = 0.5*NHitPhotons * TMath::Exp(-DistFromLowerZ/fAttenuationLength);
// // // // 	//photons on that reach the right end of the scintillator tile
// // // // 	NPhotons[1] = 0.5*NHitPhotons * TMath::Exp(-(PVBARLength -  DistFromLowerZ) / fAttenuationLength);
// // // // 	NPhotons[2] = 0.5*NHitPhotons * TMath::Exp(-DistFromLowerZ/fAttenuationLength);
// // // // 	//photons on that reach the right end of the scintillator tile
// // // // 	NPhotons[3] = 0.5*NHitPhotons * TMath::Exp(-(PVBARLength -  DistFromLowerZ) / fAttenuationLength);
// // // // 
// // // // 	//Apply geometric collection efficiency
// // // // 	NPhotons[0] *= fCollectEff;
// // // // 	NPhotons[1] *= fCollectEff;
// // // // 	NPhotons[2] *= fCollectEff;
// // // // 	NPhotons[3] *= fCollectEff;

        // Assign primary number only if contribution is significant
	if( hit->GetEnergy() > fPrimThreshold*0+1.e-6)
	  new((*sdigits)[nSdigits]) IlcPVBARDigit(hit->GetPrimary(),hit->GetId(),
						  hit->GetEnergy(), NPhotons ,hit->GetTime()) ;
	else
	  new((*sdigits)[nSdigits]) IlcPVBARDigit(-1               ,hit->GetId(), 
						  hit->GetEnergy(), NPhotons ,hit->GetTime()) ;
        nSdigits++ ;
      }
      
    }
 
    sdigits->Sort() ;

    nSdigits = sdigits->GetEntriesFast() ;

    fSDigitsInRun += nSdigits ;  
    sdigits->Expand(nSdigits) ;
    
    for (i = 0 ; i < nSdigits ; i++) {       
      IlcPVBARDigit * digit = static_cast<IlcPVBARDigit *>(sdigits->At(i)) ;
      digit->SetIndexInList(i) ;     
    }
    
//    // make Quality Assurance data
//
//    if (GetQADataMaker()->IsCycleDone() ) {
//      GetQADataMaker()->EndOfCycle(IlcQAv1::kHITS) ; 
//	  GetQADataMaker()->EndOfCycle(IlcQAv1::kSDIGITS) ; 
//      GetQADataMaker()->StartOfCycle(IlcQAv1::kHITS) ; 
//	  GetQADataMaker()->StartOfCycle(IlcQAv1::kSDIGITS, kTRUE) ; 
//   }
//    GetQADataMaker()->Exec(IlcQAv1::kHITS, hits) ; 
//    GetQADataMaker()->Exec(IlcQAv1::kSDIGITS, sdigits) ; 
//    GetQADataMaker()->Increment() ;
	
    //Now write SDigits

    
    //First list of sdigits

    Int_t bufferSize = 32000 ;
    TBranch * sdigitsBranch = treeS->Branch("PVBAR",&sdigits,bufferSize);
    sdigitsBranch->Fill() ;

    PVBARLoader->WriteSDigits("OVERWRITE");

    if(strstr(option,"deb"))
      PrintSDigits(option) ;
      
  }// event loop
  
//  //Write the quality assurance data 
//  GetQADataMaker()->EndOfCycle(IlcQAv1::kHITS) ;    
//  GetQADataMaker()->EndOfCycle(IlcQAv1::kSDIGITS) ;    
//  GetQADataMaker()->Finish() ;

  Unload();

  if(strstr(option,"tim")){
    gBenchmark->Stop("PVBARSDigitizer");
    Info("Exec","   took %f seconds for SDigitizing  %f seconds per event",
	 gBenchmark->GetCpuTime("PVBARSDigitizer"), 
	 gBenchmark->GetCpuTime("PVBARSDigitizer")/nEvents) ;
  }
  
}

//__________________________________________________________________
void IlcPVBARSDigitizer::Print(const Option_t *)const
{
  // Prints parameters of SDigitizer
  Info("Print", "\n------------------- %s -------------", GetName() ) ; 
  printf("   Writing SDigits to branch with title  %s\n", fEventFolderName.Data()) ;
  printf("   Threshold for Primary assignment= %f\n", fPrimThreshold)  ; 
  printf("---------------------------------------------------\n") ;
  
}

//__________________________________________________________________
Bool_t IlcPVBARSDigitizer::operator==( IlcPVBARSDigitizer const &sd )const
{
  // Equal operator.
  // SDititizers are equal if their pedestal, slope and threshold are equal

  if(fPrimThreshold==sd.fPrimThreshold && fCollectEff==sd.fCollectEff && fAttenuationLength==sd.fAttenuationLength && fSiPMPDE==sd.fSiPMPDE)
    return kTRUE ;
  else
    return kFALSE ;
}

//__________________________________________________________________
void IlcPVBARSDigitizer::PrintSDigits(Option_t * option)
{
  // Prints list of digits produced in the current pass of IlcPVBARDigitizer

  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcPVBARLoader * PVBARLoader = static_cast<IlcPVBARLoader*>(rl->GetLoader("PVBARLoader"));

  // Get PVBAR Geometry object
  IlcPVBARGeometry *geom;
  if (!(geom = IlcPVBARGeometry::GetInstance())) 
        geom = IlcPVBARGeometry::GetInstance("ORKA","");

  const TClonesArray * sdigits = PVBARLoader->SDigits() ;
  
  Info( "\nPrintSDigits", "event # %d %d sdigits", 
	gIlc->GetEvNumber(), sdigits->GetEntriesFast() ) ; 

  if(strstr(option,"all")||strstr(option,"EMC")){
    
    //loop over digits
    IlcPVBARDigit * digit;
    printf("\nEMC sdigits\n") ; 

//     Int_t PVBARNSlicePhi = 80;//FIXME geom->GetPVBARNSlicePhi();
//     Int_t PVBARNTiles = 150;//FIXME geom->GetPVBARNTiles();
    
    Int_t maxEmc = geom->GetNTotalElements();//PVBARNSlicePhi * PVBARNTiles*2 ;//FIXME Sci and Cer
    Int_t index ;
    for (index = 0 ; (index < sdigits->GetEntriesFast()) && 
	 ((static_cast<IlcPVBARDigit *> (sdigits->At(index)))->GetId() <= maxEmc) ; index++) {
      digit = static_cast<IlcPVBARDigit *>( sdigits->At(index) ) ;
      //  if(digit->GetNprimary() == 0) 
      // 	continue;
//       printf("%6d  %8d    %6.5e %4d      %2d :\n", // YVK
      printf("%6d  %.4f    %6.5e %4d      %2d : ",
  	      digit->GetId(), digit->GetEnergy(), digit->GetTime(), digit->GetIndexInList(), digit->GetNprimary()) ;  
      Int_t iprimary;
      for (iprimary=0; iprimary<digit->GetNprimary(); iprimary++) {
	printf("%d ",digit->GetPrimary(iprimary+1) ) ; 
      }
      printf("\n");
    }  
  }

// // // // //   if(strstr(option,"all")||strstr(option,"CPV")){
// // // // //     
// // // // //     //loop over CPV digits
// // // // //     IlcPVBARDigit * digit;
// // // // //     printf("\nCPV sdigits\n") ; 
// // // // //     Int_t maxEmc = geom->GetNModules() * geom->GetNCristalsInModule() ;
// // // // //     Int_t index ;
// // // // //     for (index = 0 ; index < sdigits->GetEntriesFast(); index++) {
// // // // //       digit = static_cast<IlcPVBARDigit *>( sdigits->At(index) ) ;
// // // // //       if(digit->GetId() > maxEmc){
// // // // // 	printf("\n%6d  %8d    %4d      %2d :",
// // // // // 		digit->GetId(), digit->GetAmp(), digit->GetIndexInList(), digit->GetNprimary()) ;  
// // // // // 	Int_t iprimary;
// // // // // 	for (iprimary=0; iprimary<digit->GetNprimary(); iprimary++) {
// // // // // 	  printf("%d ",digit->GetPrimary(iprimary+1) ) ; 
// // // // // 	}
// // // // //       }    
// // // // //     }
// // // // //   }
}

//____________________________________________________________________________ 
void IlcPVBARSDigitizer::Unload() const
{
  // Unloads the objects from the folder
  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcPVBARLoader * PVBARLoader = static_cast<IlcPVBARLoader*>(rl->GetLoader("PVBARLoader"));
  
  PVBARLoader->UnloadHits() ; 
  PVBARLoader->UnloadSDigits() ; 
}

//____________________________________________________________________________
Float_t IlcPVBARSDigitizer::AttenuationLength_m_LG(Float_t lambda){
  // This function returns the attenuation length in meters
  // as a function of wavelength for SF57HHT lead glass.
  
  Float_t wl[30] ={2500.,2325., 1970.,1530.,1060.,700.,660.,620.,580.,546.,500.,460.,436.,420.,405.,400.,390.,380.,370.,365.,350.,334.,320.,310.,300.,290.,280.,270.,260.,250.};
  Float_t al[30] ={0.255,0.319,1.146,9.565,57.536,28.754,19.159,19.159,19.159,19.159,5.728,2.588,1.486,0.793,0.382,0.290,0.125,0.048,0.016,0.0089,0.0024,1.E-6,1.E-6,1.E-6,1.E-6,1.E-6,1.E-6,1.E-6,1.E-6,1.E-6};
  
  Float_t Alength = 1.E-6;
  for ( Int_t n = 0; n <29; n++ )
  {
    if ( lambda <= wl[n])
    {
      Alength = al[n]+(al[n+1]-al[n])*(lambda-wl[n])/(wl[n+1]-wl[n]);
    }
  }
  
  return Alength;

}


//____________________________________________________________________________
Float_t IlcPVBARSDigitizer::AttenuationLength_m_WLS(Float_t /*lambda*/){
  // This function returns the attenuation length in meters
  // as a function of wavelength for xxx WLS.
  
//   Float_t wl[30] ={2500.,2325., 1970.,1530.,1060.,700.,660.,620.,580.,546.,500.,460.,436.,420.,405.,400.,390.,380.,370.,365.,350.,334.,320.,310.,300.,290.,280.,270.,260.,250.};
//   Float_t al[30] ={0.255,0.319,1.146,9.565,57.536,28.754,19.159,19.159,19.159,19.159,5.728,2.588,1.486,0.793,0.382,0.290,0.125,0.048,0.016,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  
  Float_t Alength = 12.0;

  
  return Alength;

}

//____________________________________________________________________________
Float_t IlcPVBARSDigitizer::QE_SiPM(Float_t lambda){
//  QE for SiPM
//  lambda in nanometer
     
  Float_t wl[13] = {360., 380., 400., 420., 440., 460., 480., 500., 520., 540., 560., 580., 600.};
  Float_t stvy[13] = {0.09, 0.14, 0.175, 0.18, 0.215, 0.21, 0.22, 0.22, 0.25, 0.23, 0.225, 0.20, 0.185};
  
  Float_t qe = 0.0;
      
  if (lambda >=  360. && lambda <= 600.) {
      
    for ( Int_t n = 0; n <13; n++ )
    {
      if ( lambda >= wl[n])
        qe = stvy[n]+(stvy[n+1]-stvy[n])*(lambda-wl[n])/(wl[n+1]-wl[n]);
    }
  }
  return qe;

}

//____________________________________________________________________________
Float_t IlcPVBARSDigitizer::Shifter_WLS(Float_t /*lambda*/){
  // This function returns the shifted wavelength of a WLS.
// c   Picks a wavelength for the blue light emitted by the scintillator 
// c  .....this spectrum was measured at Minn
// c      (data are integral probability from 460.91 to 596 nm in 5 nnm steps) 

// From Ralf Dollan 2004 thesis and
// www-zeuthen.desy.de/lcdet/Feb_05_WS/talks/rd_lcdet_sim.pdf
// Bicron BCF 91A

  const Int_t NUMENTRIES2 = 24;

  Float_t PhotonWL_WLS_EM_Bcf91a_core[NUMENTRIES2] =
  { 460.91,464.36,466.11, 469.64, 471.42,
  475.04,	480.56,	484.31,	486.21,	490.06,
  495.94,	499.94,	504.,	506.06,	508.13,
  510.22,	514.46,	523.14,	532.12,	551.04,
  553.5,	566.14,	576.67,	596.08

  };

  Float_t WLS_EMISSION_Bcf91a_core[NUMENTRIES2] =
  {0., 0.02, 0.09, 0.20, 0.29,
  0.40, 0.59, 0.70, 0.80, 0.89,
  1.00, 0.96, 0.88, 0.79, 0.69,
  0.59, 0.50, 0.40, 0.31, 0.22,
  0.19, 0.10, 0.06, 0.
  };

retry:
    Float_t WLVal = 0.;
Int_t EmissIdx = ( Int_t ) ( gRandom->Rndm() *(NUMENTRIES2-1) );
Float_t EmissVal = gRandom->Rndm();


if ( EmissVal < WLS_EMISSION_Bcf91a_core[EmissIdx] )  // success
{

  if ( EmissIdx>0 && EmissIdx<(NUMENTRIES2-1) )
  {
    Float_t WLValMin = ( PhotonWL_WLS_EM_Bcf91a_core[EmissIdx]+PhotonWL_WLS_EM_Bcf91a_core[EmissIdx-1] ) /2.;
    Float_t WLValMax = ( PhotonWL_WLS_EM_Bcf91a_core[EmissIdx+1]+PhotonWL_WLS_EM_Bcf91a_core[EmissIdx] )/2.;

    WLVal = ( WLValMax - WLValMin ) * gRandom->Rndm() + WLValMin;
  }

  if ( EmissIdx == 0 )  // first bin
  {
    Float_t WLValMax = ( PhotonWL_WLS_EM_Bcf91a_core[EmissIdx+1]+PhotonWL_WLS_EM_Bcf91a_core[EmissIdx] ) /2.;


    WLVal = ( WLValMax - PhotonWL_WLS_EM_Bcf91a_core[EmissIdx] ) *gRandom->Rndm() + PhotonWL_WLS_EM_Bcf91a_core[EmissIdx];

  }

  if ( EmissIdx == (NUMENTRIES2-1) )
  {
    Float_t WLValMin = ( PhotonWL_WLS_EM_Bcf91a_core[EmissIdx]+PhotonWL_WLS_EM_Bcf91a_core[EmissIdx-1] ) /2.;

    WLVal = ( PhotonWL_WLS_EM_Bcf91a_core[EmissIdx] - WLValMin ) * gRandom->Rndm() + WLValMin;

  }
}
else
  goto retry;

return WLVal;
}
