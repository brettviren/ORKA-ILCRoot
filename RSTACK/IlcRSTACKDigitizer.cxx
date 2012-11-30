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
//////////////////////////////////////////////////////////////////////////////
// This class performs digitization of Summable digits (in the RSTACK this is
// the sum of contributions from all primary particles into a given tile). 
// In addition it performs mixing of summable digits from different events.
// The name of the class is also the title of the branch that will contain 
// the created SDigits
// The title of the class is the name of the file that contains the hits from
// which the SDigits are created
//
// For each event two branches are created in TreeD:
//   "RSTACK" - list of digits
//   "IlcRSTACKDigitizer" - IlcRSTACKDigitizer with all parameters used in digitization
//
// Note, that one can set a title for new digits branch, and repeat digitization with
// another set of parameters.
//
// Use case:
// root[0] IlcRSTACKDigitizer * d = new IlcRSTACKDigitizer() ;
// root[1] d->Digitize()             
// Warning in <TDatabasePDG::TDatabasePDG>: object already instantiated
//                       //Digitizes SDigitis in all events found in file gilc.root 
//
// root[2] IlcRSTACKDigitizer * d1 = new IlcRSTACKDigitizer("gilc1.root") ;  
//                       // Will read sdigits from gilc1.root
// root[3] d1->MixWith("gilc2.root")       
// Warning in <TDatabasePDG::TDatabasePDG>: object already instantiated
//                       // Reads another set of sdigits from gilc2.root
// root[3] d1->MixWith("gilc3.root")       
//                       // Reads another set of sdigits from gilc3.root
// root[4] d->Digitize("deb timing")    
//                       // Reads SDigits from files gilc1.root, gilc2.root ....
//                       // mixes them and stores produced Digits in file gilc1.root          
//                       // deb - prints number of produced digits
//                       // deb all - prints list of produced digits
//                       // timing  - prints time used for digitization
//

// --- ROOT system ---
#include "TTree.h"
#include "TSystem.h"
#include "TBenchmark.h"
#include "TRandom.h"
#include "TMath.h"

// --- Standard library ---

// --- IlcRoot header files ---
#include <TGeoManager.h>                                                                                                                   
#include "IlcLog.h"
#include "IlcDigitizationInput.h"
#include "IlcRSTACKDigit.h"
#include "IlcRSTACKDigitizer.h"
#include "IlcRSTACKGeometry.h"
#include "IlcRSTACKTick.h"
#include "IlcRSTACKSimParam.h"
#include "IlcRSTACKCalibData.h"
#include "IlcRunLoader.h"
#include "IlcRSTACKLoader.h"
#include "IlcRSTACKPulseGenerator.h"

ClassImp(IlcRSTACKDigitizer)


//____________________________________________________________________________ 
IlcRSTACKDigitizer::IlcRSTACKDigitizer() :
  IlcDigitizer("",""),
  fDefaultInit(kTRUE),
  fDigitsInRun(0),
  fInit(kFALSE),
  fInput(0),
  fInputFileNames(0x0),
  fEventNames(0x0),
  fTiles(0),
  fEventFolderName(""),
  fFirstEvent(0),
  fLastEvent(0), 
  fcdb(0x0),
  fEventCounter(0),
  fPulse(0),
  fADCValuesLG(0),
  fADCValuesHG(0),
  fSiPMPixels(0.),
  fSiPMNoise(0.),
  fElectronicGain(0.),
  fConversionFactor(0.),
  fENF(0.),
  fDigitsThreshold(0.),
  fADCchannel(0.),
  fADCbits(0)
{
  // ctor
  InitParameters() ; 
  fDigInput = 0 ;                     // We work in the standalong mode
}

//____________________________________________________________________________ 
IlcRSTACKDigitizer::IlcRSTACKDigitizer(TString ilcrunFileName, 
				   TString eventFolderName):
  IlcDigitizer("RSTACKDigitizer", ilcrunFileName), 
  fDefaultInit(kFALSE),
  fDigitsInRun(0),
  fInit(kFALSE),
  fInput(0),
  fInputFileNames(0x0),
  fEventNames(0x0),
  fTiles(0),
  fEventFolderName(eventFolderName),
  fFirstEvent(0),
  fLastEvent(0), 
  fcdb(0x0),
  fEventCounter(0),
  fPulse(0),
  fADCValuesLG(0),
  fADCValuesHG(0),
  fSiPMPixels(0.),
  fSiPMNoise(0.),
  fElectronicGain(0.),
  fConversionFactor(0.),
  fENF(0.),
  fDigitsThreshold(0.),
  fADCchannel(0.),
  fADCbits(0)
{
  // ctor
  InitParameters() ; 
  Init() ;
  fDefaultInit = kFALSE ; 
  fDigInput = 0 ;                     // We work in the standalone mode
  fcdb = new IlcRSTACKCalibData(-1);
}

//____________________________________________________________________________ 
IlcRSTACKDigitizer::IlcRSTACKDigitizer(IlcDigitizationInput * rd) :
  IlcDigitizer(rd,"RSTACKDigitizer"),
  fDefaultInit(kFALSE),
  fDigitsInRun(0),
  fInit(kFALSE),
  fInput(0),
  fInputFileNames(0x0),
  fEventNames(0x0),
  fTiles(0),
  fEventFolderName(fDigInput->GetInputFolderName(0)),
  fFirstEvent(0),
  fLastEvent(0), 
  fcdb (0x0), 
  fEventCounter(0),
  fPulse(0),
  fADCValuesLG(0),
  fADCValuesHG(0),
  fSiPMPixels(0.),
  fSiPMNoise(0.),
  fElectronicGain(0.),
  fConversionFactor(0.),
  fENF(0.),
  fDigitsThreshold(0.),
  fADCchannel(0.),
  fADCbits(0)
{
  // ctor Init() is called by RunDigitizer
  fDigInput = rd ; 
  SetTitle(static_cast<IlcStream*>(fDigInput->GetInputStream(0))->GetFileName(0));
  InitParameters() ; 
  fDefaultInit = kFALSE ; 
//   fcdb = new IlcRSTACKCalibData(-1);
}

//____________________________________________________________________________ 
  IlcRSTACKDigitizer::~IlcRSTACKDigitizer()
{
  // dtor  
  delete [] fInputFileNames ; 
  delete [] fEventNames ; 

  delete fPulse;
  delete [] fADCValuesLG;
  delete [] fADCValuesHG;

  if(fcdb){ delete fcdb ; fcdb=0;} 

}

//____________________________________________________________________________
void IlcRSTACKDigitizer::Digitize(Int_t event) 
{ 
  
  // Makes the digitization of the collected summable digits.
  // It first creates the array of all RSTACK modules
  // filled with noise and then adds contributions from SDigits. 
  // This design avoids scanning over the list of digits to add 
  // contribution to new SDigits only.

  Bool_t toMakeNoise = kTRUE ; //Do not create noisy digits if merge with real data

  //First stream 
  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcRSTACKLoader * RSTACKLoader = static_cast<IlcRSTACKLoader*>(rl->GetLoader("RSTACKLoader"));

  Int_t readEvent = event ; 
  if (fDigInput) 
    readEvent = static_cast<IlcStream*>(fDigInput->GetInputStream(0))->GetCurrentEventNumber() ; 
  IlcDebug(1,Form("Adding event %d from input stream 0 %s %s", 
		  readEvent, GetTitle(), fEventFolderName.Data())) ; 
  rl->GetEvent(readEvent) ;
  RSTACKLoader->CleanSDigits() ; 
  RSTACKLoader->LoadSDigits("READ") ;

  //Prepare Output
  TClonesArray * digits = RSTACKLoader->Digits() ;
  if( !digits ) {
    RSTACKLoader->MakeDigitsArray() ; 
    digits = RSTACKLoader->Digits() ;
  }
 
  digits->Clear() ;

  //
  const IlcRSTACKGeometry *geom = IlcRSTACKGeometry::GetInstance() ;
  //Making digits with noise

  Int_t nTiles = geom->GetRSTACKNTilesInSector()*geom->GetRSTACKNSectorPhi();
  
  Int_t absID ;
  
  digits->Expand(nTiles) ;

  //take all the inputs to add together and load the SDigits
  TObjArray * sdigArray = new TObjArray(fInput) ;
  sdigArray->AddAt(RSTACKLoader->SDigits(), 0) ;
 
  for(Int_t i = 1 ; i < fInput ; i++){
    TString tempo(fEventNames[i]) ; 
    tempo += i ;
    IlcRunLoader* rl2 = IlcRunLoader::GetRunLoader(tempo) ;
    if(!rl2){ 
      rl2 = IlcRunLoader::Open(fInputFileNames[i], tempo) ;
      if (!rl2) {
        Fatal("IlcRSTACKDigitizer", "Could not find the Run Loader for %s - %s",fInputFileNames[i].Data(), tempo.Data()) ;
        return ;
      }
      rl2->LoadHeader();
    }
    IlcRSTACKLoader * RSTACKLoader2 = static_cast<IlcRSTACKLoader*>(rl2->GetLoader("RSTACKLoader"));
 
    if(fDigInput){ 
      readEvent = static_cast<IlcStream*>(fDigInput->GetInputStream(i))->GetCurrentEventNumber() ; 
    }
    TClonesArray * digs ;
    if(IlcRSTACKSimParam::GetInstance()->IsStreamDigits(i)){ //This is Digits Stream
      IlcInfo(Form("Adding event %d from input stream %d %s %s", 
		 readEvent, i, fInputFileNames[i].Data(), tempo.Data())) ; 
       rl2->GetEvent(readEvent) ;
       RSTACKLoader2->CleanDigits() ;
       RSTACKLoader2->LoadDigits("READ") ;
       digs = RSTACKLoader2->Digits() ;
       toMakeNoise=0 ; //Do not add noise, it is already in stream
    }
    else{
      IlcInfo(Form("Adding event %d (SDigits) from input stream %d %s %s",
                 readEvent, i, fInputFileNames[i].Data(), tempo.Data())) ;
       rl2->GetEvent(readEvent) ;
       RSTACKLoader2->CleanSDigits() ; 
       RSTACKLoader2->LoadSDigits("READ") ;
       digs = RSTACKLoader2->SDigits() ;
    } 
    sdigArray->AddAt(digs, i) ;
  }

  //Find the first tile with signal
  Int_t nextSig = 200000 ; 
  TClonesArray * sdigits ;  
  for(Int_t i = 0 ; i < fInput ; i++){
    sdigits = static_cast<TClonesArray *>(sdigArray->At(i)) ;
    if ( !sdigits->GetEntriesFast() )
      continue ; 
    Int_t curNext = static_cast<IlcRSTACKDigit *>(sdigits->At(0))->GetId() ;
    if(curNext < nextSig) 
      nextSig = curNext ;
  }
  
  TArrayI index(fInput) ;
  index.Reset() ;  //Set all indexes to zero
  
  IlcRSTACKDigit * digit ;
  IlcRSTACKDigit * curSDigit ;
  
//  TClonesArray * ticks = new TClonesArray("IlcRSTACKTick",1000) ;
  
  //Put Noise contribution
  Double_t SiPMNoise = 0. ;
  Double_t ENF = 0. ;
  if(toMakeNoise){
     SiPMNoise = fSiPMNoise ; 
     ENF = fENF;
  }

  Int_t idigit= 0;
  for(absID = 1; absID <= nTiles; absID++){
    Float_t noise[2]={0.,0.};
    //the dark current noise will be later in the flow
    
    new((*digits)[idigit]) IlcRSTACKDigit( -1, absID, 0.f, noise, TimeOfNoise() ) ;
    //look if we have to add signal?
    digit = static_cast<IlcRSTACKDigit *>(digits->At(idigit)) ;
    idigit++ ;
    
    if(absID==nextSig){
      //Add SDigits from all inputs 
      //      ticks->Clear() ;
      //      Int_t contrib = 0 ;
      
      //New Timing model is necessary
      //      Float_t a = digit->GetEnergy() ;
      //      Float_t b = TMath::Abs( a / fTimeSignalLength) ;
      //      //Mark the beginning of the signal
      //      new((*ticks)[contrib++]) IlcRSTACKTick(digit->GetTime(),0, b);  
      //      //Mark the end of the signal     
      //      new((*ticks)[contrib++]) IlcRSTACKTick(digit->GetTime()+fTimeSignalLength, -a, -b); 
      
      // Calculate time as time of the largest digit
      Float_t time = digit->GetTime() ;
      Float_t eTime= digit->GetEnergy() ;
      
      //loop over inputs
      for(Int_t i = 0 ; i < fInput ; i++){
	if( static_cast<TClonesArray *>(sdigArray->At(i))->GetEntriesFast() > index[i] ){
	  curSDigit = static_cast<IlcRSTACKDigit*>(static_cast<TClonesArray *>(sdigArray->At(i))->At(index[i])) ; 	
	  if(IlcRSTACKSimParam::GetInstance()->IsStreamDigits(i)){ //This is Digits Stream
	    //curSDigit->SetEnergy(Calibrate(curSDigit->GetEnergy(),curSDigit->GetId())) ;
	    //curSDigit->SetTime(CalibrateT(curSDigit->GetTime(),curSDigit->GetId())) ;
	  }
	}
	else
	  curSDigit = 0 ;
	//May be several digits will contribute from the same input
	  while(curSDigit && curSDigit->GetId() == absID){	   
	    //Shift primary to separate primaries belonging different inputs
	    Int_t primaryoffset ;
	    if(fDigInput)
	      primaryoffset = fDigInput->GetMask(i) ; 
	    else
	      primaryoffset = 10000000*i ;
	    curSDigit->ShiftPrimary(primaryoffset) ;
	    
	    //New Timing model is necessary
	    //	  a = curSDigit->GetEnergy() ;
	    //	  b = a /fTimeSignalLength ;
	    //	  new((*ticks)[contrib++]) IlcRSTACKTick(curSDigit->GetTime(),0, b);  
	    //	  new((*ticks)[contrib++]) IlcRSTACKTick(curSDigit->GetTime()+fTimeSignalLength, -a, -b); 
	    if(curSDigit->GetEnergy()>eTime){
	      eTime=curSDigit->GetEnergy() ;
	      time=curSDigit->GetTime() ;
	    }
	    *digit += *curSDigit ;  //add energies
	    
	    index[i]++ ;
	    if( static_cast<TClonesArray *>(sdigArray->At(i))->GetEntriesFast() > index[i] )
	      curSDigit = static_cast<IlcRSTACKDigit*>(static_cast<TClonesArray *>(sdigArray->At(i))->At(index[i])) ; 	
	    else
	      curSDigit = 0 ;
	  }
      }
      
      //calculate and set time
      //New Timing model is necessary
      //      Float_t time = FrontEdgeTime(ticks) ;
      digit->SetTime(time) ;
      
      //Find next signal module
      nextSig = 200000 ;
      for(Int_t i = 0 ; i < fInput ; i++){
	sdigits = static_cast<TClonesArray *>(sdigArray->At(i)) ;
	Int_t curNext = nextSig ;
	if(sdigits->GetEntriesFast() > index[i] ){
	  curNext = static_cast<IlcRSTACKDigit *>(sdigits->At(index[i]))->GetId() ;
	}
	if(curNext < nextSig) nextSig = curNext ;
      }
    }
    
    
    
    //Evaluate excess noise factor contribution to the signal and update the digit
    //apply QE, consider the limited number of SiPM pixels
    Float_t NPE[2];
    for(Int_t idx=0; idx<2; idx++){
      NPE[idx] = digit->GetNPE()[idx];
      NPE[idx] = TMath::Min(fSiPMPixels, NPE[idx]) ;
      NPE[idx] += gRandom->Gaus(0., TMath::Sqrt(SiPMNoise*SiPMNoise + ((1.-ENF)*NPE[idx])*((1.-ENF)*NPE[idx]))) ;
    }

    digit->SetNPE(NPE);
    
  }


//   //Apply non-linearity
//   if(IlcRSTACKSimParam::GetInstance()->IsCellNonlinearityOn()){ //Apply non-lineairyt on tile level
//     const Double_t aNL = IlcRSTACKSimParam::GetInstance()->GetCellNonLineariryA() ;
//     const Double_t bNL = IlcRSTACKSimParam::GetInstance()->GetCellNonLineariryB() ;
//     const Double_t cNL = IlcRSTACKSimParam::GetInstance()->GetCellNonLineariryC() ;
//     for(Int_t i = 0 ; i < nTiles ; i++){
//       digit = static_cast<IlcRSTACKDigit*>( digits->At(i) ) ;
//       Double_t e= digit->GetEnergy() ;
//       // version(1)      digit->SetEnergy(e*(1+a*TMath::Exp(-e/b))) ;
//       digit->SetEnergy(e*cNL*(1.+aNL*TMath::Exp(-e*e/2./bNL/bNL))) ; //Better agreement with data...
//     }  
//   }


  //discretize energy if necessary
  IlcRSTACKSimParam::GetInstance()->SetEDigitizationOn();
  if(IlcRSTACKSimParam::GetInstance()->IsEDigitizationOn()){
    //Float_t adcW = fADCchannel;
    Float_t ConversionFactor = fConversionFactor;
    for(Int_t i = 0 ; i < nTiles ; i++){                                                                                                       
      digit = static_cast<IlcRSTACKDigit*>( digits->At(i) ) ;

      Float_t Amp = (digit->GetNPE()[0] + digit->GetNPE()[1])/ConversionFactor;
      digit->SetAmp(TMath::CeilNint(Amp)) ;
      
      Float_t NPE[2];
      for(Int_t idx=0; idx<2; idx++)
	NPE[idx] = TMath::Min((Double_t)(1<<fADCbits), TMath::Ceil((digit->GetNPE()[idx] / ConversionFactor)) );

      digit->SetNPE(NPE);

    }
  }
 
//   //Apply decalibration if necessary
//   for(Int_t i = 0 ; i < nTiles ; i++){
//     digit = static_cast<IlcRSTACKDigit*>( digits->At(i) ) ;
//     Decalibrate(digit) ;
//   }
  
//  ticks->Delete() ;
//  delete ticks ;
  
  delete sdigArray ; //We should not delete its contents 
  
//   Int_t relId[4];

//   //set amplitudes in bad channels to zero
//   for(Int_t i = 0 ; i <digits->GetEntriesFast(); i++){
//     digit = static_cast<IlcRSTACKDigit*>( digits->At(i) ) ;
//     geom->AbsToRelNumbering(digit->GetId(),relId);
//     if(fcdb->IsBadChannelEmc(relId[0],relId[3],relId[2])) digit->SetEnergy(0.); 
//   }

  //remove digits below thresholds
  for(Int_t i = 0 ; i < nTiles ; i++){
    digit = static_cast<IlcRSTACKDigit*>( digits->At(i) ) ;

    if(digit->GetNPE()[0] < fDigitsThreshold && digit->GetNPE()[1] < fDigitsThreshold){
      digits->RemoveAt(i) ;
      continue ;
    }

//     geom->AbsToRelNumbering(digit->GetId(),relId);

//    digit->SetEnergy(TMath::Ceil(digit->GetEnergy())-0.9999) ;

//     Float_t tres = TimeResolution(digit->GetEnergy()) ; 
    Float_t tres = TimeResolution(TMath::Sqrt(digit->GetNPE()[0]*digit->GetNPE()[0] + digit->GetNPE()[1]*digit->GetNPE()[1])) ; 
    digit->SetTime(gRandom->Gaus(digit->GetTime(), tres) ) ;

    fPulse->Reset();
//     fPulse->SetAmplitude(digit->GetEnergy()/fcdb->GetADCchannelEmc(relId[0],relId[3],relId[2]));
    fPulse->SetAmplitude(digit->GetEnergy()/1.);  //FIXME
    fPulse->SetTZero(digit->GetTimeR());
    fPulse->MakeSamples();
    fPulse->GetSamples(fADCValuesHG, fADCValuesLG) ; 
    Int_t nSamples = fPulse->GetRawFormatTimeBins();
    digit->SetALTROSamplesHG(nSamples,fADCValuesHG);
    digit->SetALTROSamplesLG(nSamples,fADCValuesLG);
  }

  digits->Compress() ;  
  Int_t ndigits = digits->GetEntriesFast() ;
  
  //Set indexes in list of digits and make true digitization of the energy
  for (Int_t i = 0 ; i < ndigits ; i++) { 
    digit = static_cast<IlcRSTACKDigit*>( digits->At(i) ) ; 
    digit->SetIndexInList(i) ;
  }

}
//____________________________________________________________________________
Float_t IlcRSTACKDigitizer::Calibrate(Float_t amp,Int_t /*absId*/){
  //Apply calibration

  //   const IlcRSTACKGeometry *geom = IlcRSTACKGeometry::GetInstance() ;

  //Determine rel.position of the cell absolute ID
  //   Int_t relId[4];
  //   geom->AbsToRelNumbering(absId,relId);
  //   Int_t sector=relId[0];
  //   Int_t row   =relId[1];
  //     Float_t calibration = fcdb->GetADCchannelEmc(sector,row);
  return amp*fADCchannel ;
//   }
//   return 0 ;
}
//____________________________________________________________________________
void IlcRSTACKDigitizer::Decalibrate(IlcRSTACKDigit *digit)
{
  // Decalibrate EMC digit, i.e. change its energy by a factor read from CDB

//   const IlcRSTACKGeometry *geom = IlcRSTACKGeometry::GetInstance() ;

  //Determine rel.position of the cell absolute ID
//   Int_t relId[4];
//   geom->AbsToRelNumbering(digit->GetId(),relId);
//   Int_t module=relId[0];
//   Int_t row   =relId[2];
//   Int_t column=relId[3];
//   if(relId[1]==0){ //This Is EMC
//     Float_t decalib     = fcdb->GetADCchannelEmcDecalib(module,column,row); // O(1)
//     Float_t calibration = fcdb->GetADCchannelEmc(module,column,row)*decalib;
//     Float_t decalib     = 1.; //FIXME
    Float_t calibration = 1.; //FIXME
    Float_t energy = digit->GetEnergy()/calibration;
    digit->SetEnergy(energy); //Now digit measures E in ADC counts
    Float_t time = digit->GetTime() ;
//     time-=fcdb->GetTimeShiftEmc(module,column,row);
    time-=0.; //FIXME
    digit->SetTime(time) ;
//   }
}
//____________________________________________________________________________
Float_t IlcRSTACKDigitizer::CalibrateT(Float_t time,Int_t /*absId*/){
  //Apply time calibration
//   const IlcRSTACKGeometry *geom = IlcRSTACKGeometry::GetInstance() ;
// 
//   //Determine rel.position of the cell absolute ID
//   Int_t relId[4];
//   geom->AbsToRelNumbering(absId,relId);
//   Int_t module=relId[0];
//   Int_t row   =relId[2];
//   Int_t column=relId[3];
//   time += fcdb->GetTimeShiftEmc(module,column,row);
  time += 0.; //FIXME
  return time ;
}

//____________________________________________________________________________
void IlcRSTACKDigitizer::Digitize(Option_t *option) 
{ 
  // Steering method to process digitization for events
  // in the range from fFirstEvent to fLastEvent.
  // This range is optionally set by SetEventRange().
  // if fLastEvent=-1, then process events until the end.
  // by default fLastEvent = fFirstEvent (process only one event)

  if (!fInit) { // to prevent overwrite existing file
    IlcError(Form("Give a version name different from %s", 
		  fEventFolderName.Data() )) ;
    return ;
  }   

  if (strstr(option,"print")) {
    Print();
    return ; 
  }
  
 if(strstr(option,"tim"))
     gBenchmark->Start("RSTACKDigitizer");
  
  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcRSTACKLoader * RSTACKLoader = static_cast<IlcRSTACKLoader*>(rl->GetLoader("RSTACKLoader"));
  
  if (fLastEvent == -1) 
    fLastEvent = rl->GetNumberOfEvents() - 1 ;
  else if (fDigInput) 
    fLastEvent = fFirstEvent ; 
 
  Int_t nEvents   = fLastEvent - fFirstEvent + 1;
  
  Int_t ievent ;

  for (ievent = fFirstEvent; ievent <= fLastEvent; ievent++) {
    fEventCounter++ ; 

    Digitize(ievent) ; //Add prepared SDigits to digits and add the noise

    WriteDigits() ;

    if(strstr(option,"deb"))
      PrintDigits(option);
    
    //increment the total number of Digits per run 
    fDigitsInRun += RSTACKLoader->Digits()->GetEntriesFast() ;  
 }
 
  if(strstr(option,"tim")){
    gBenchmark->Stop("RSTACKDigitizer");
    TString message ; 
    message = "  took %f seconds for Digitizing %f seconds per event\n" ; 
    IlcInfo(Form( message.Data(), 
	 gBenchmark->GetCpuTime("RSTACKDigitizer"), 
	 gBenchmark->GetCpuTime("RSTACKDigitizer")/nEvents )); 
  } 
}
//____________________________________________________________________________ 
Float_t IlcRSTACKDigitizer::TimeResolution(Float_t e){
  //calculate TOF resolution using beam-test resutls
  Float_t a=IlcRSTACKSimParam::GetInstance()->GetTOFa() ;
  Float_t b=IlcRSTACKSimParam::GetInstance()->GetTOFb() ;
  return TMath::Sqrt(a*a+b*b/e) ;
}

////____________________________________________________________________________ 
//Float_t IlcRSTACKDigitizer::FrontEdgeTime(TClonesArray * ticks) const
//{
//  // Returns the shortest time among all time ticks
//
//  ticks->Sort() ; //Sort in accordance with times of ticks
//  TIter it(ticks) ;
//  IlcRSTACKTick * ctick = (IlcRSTACKTick *) it.Next() ;
//  Float_t time = ctick->CrossingTime(fTimeThreshold) ;    
//
//  IlcRSTACKTick * t ;  
//  while((t=(IlcRSTACKTick*) it.Next())){
//    if(t->GetTime() < time)  //This tick starts before crossing
//      *ctick+=*t ;
//    else
//      return time ;
//
//    time = ctick->CrossingTime(fTimeThreshold) ;    
//  }
//  return time ;
//}

//____________________________________________________________________________ 
Bool_t IlcRSTACKDigitizer::Init()
{
  // Makes all memory allocations
  fInit = kTRUE ; 

  IlcRSTACKGeometry *geom;
  if (!(geom = IlcRSTACKGeometry::GetInstance())) 
        geom = IlcRSTACKGeometry::GetInstance("ORKA","");

  fTiles = geom->GetRSTACKNSectorPhi() * geom->GetRSTACKNTilesInSector() ;
  
  fFirstEvent = 0 ; 
  fLastEvent = fFirstEvent ; 
  if (fDigInput) 
    fInput = fDigInput->GetNinputs() ; 
  else 
    fInput           = 1 ;
  
  fInputFileNames  = new TString[fInput] ; 
  fEventNames      = new TString[fInput] ; 
  fInputFileNames[0] = GetTitle() ; 
  fEventNames[0]     = fEventFolderName.Data() ; 
  Int_t index ; 
  for (index = 1 ; index < fInput ; index++) {
    fInputFileNames[index] = static_cast<IlcStream*>(fDigInput->GetInputStream(index))->GetFileName(0); 
    TString tempo = fDigInput->GetInputFolderName(index) ;
    fEventNames[index] = tempo.Remove(tempo.Length()-1) ; // strip of the stream number added by fDigInput
  }

  //to prevent cleaning of this object while GetEvent is called
  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  if(!rl){
    IlcRunLoader::Open(GetTitle(), fEventFolderName) ; 
  }
  return fInit ; 
}

//____________________________________________________________________________ 
void IlcRSTACKDigitizer::InitParameters()
{
  // Set initial parameters Digitizer

  fSiPMPixels = IlcRSTACKSimParam::GetInstance()->GetSiPMPixels()  ;        
  fSiPMNoise = IlcRSTACKSimParam::GetInstance()->GetSiPMNoise()  ;         
  fConversionFactor = IlcRSTACKSimParam::GetInstance()->GetConversionFactor()  ;        
  fENF = IlcRSTACKSimParam::GetInstance()->GetENF()  ;               
  fDigitsThreshold = IlcRSTACKSimParam::GetInstance()->GetDigitsThreshold()  ;  
  fADCchannel = IlcRSTACKSimParam::GetInstance()->GetADCchannelW()  ;       
  fADCbits = IlcRSTACKSimParam::GetInstance()->GetADCbits() ;
  fDigitsInRun  = 0 ; 
  SetEventRange(0,-1) ;
  fPulse = new IlcRSTACKPulseGenerator();
  fADCValuesLG = new Int_t[fPulse->GetRawFormatTimeBins()];
  fADCValuesHG = new Int_t[fPulse->GetRawFormatTimeBins()];
    
}

//__________________________________________________________________
void IlcRSTACKDigitizer::Print(const Option_t *)const 
{
  // Print Digitizer's parameters
  IlcInfo(Form("\n------------------- %s -------------", GetName() )) ; 
  if( strcmp(fEventFolderName.Data(), "") != 0 ){
    printf(" Writing Digits to branch with title  %s\n", fEventFolderName.Data()) ;
    
    Int_t nStreams ; 
    if (fDigInput) 
      nStreams =  GetNInputStreams() ;
    else 
      nStreams = fInput ; 
    
    Int_t index = 0 ;  
    for (index = 0 ; index < nStreams ; index++) {  
      TString tempo(fEventNames[index]) ; 
      tempo += index ;
      printf ("Adding SDigits from %s \n", fInputFileNames[index].Data()) ; 
    }
    
    printf(" ---------------------------------------------------\n") ;   
  }
  else
    IlcInfo(Form("IlcRSTACKDigitizer not initialized" )) ;
  
}

//__________________________________________________________________
 void IlcRSTACKDigitizer::PrintDigits(Option_t * /*option*/)
{
  // Print a table of digits
  

  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcRSTACKLoader * RSTACKLoader = static_cast<IlcRSTACKLoader*>(rl->GetLoader("RSTACKLoader"));
  TClonesArray * digits = RSTACKLoader->Digits() ; 
  const IlcRSTACKGeometry *geom = IlcRSTACKGeometry::GetInstance() ;
  
  IlcInfo(Form("%d", digits->GetEntriesFast())) ; 
  printf("\nevent %d", gIlc->GetEvNumber()) ;
  printf("\n       Number of entries in Digits list %d", digits->GetEntriesFast() )  ;  

 
  //loop over digits
  IlcRSTACKDigit * digit;
  printf("\nEMC digits (with primaries):\n")  ;
  printf("\n   Id  Amplitude    Time          Index Nprim: Primaries list \n") ;    
  Int_t nTiles = geom->GetRSTACKNTilesInSector()*geom->GetRSTACKNSectorPhi();
  Int_t index ;
  for (index = 0 ; (index < digits->GetEntriesFast()) && 
    (static_cast<IlcRSTACKDigit *>(digits->At(index))->GetId() <= nTiles) ; index++) {
    digit = (IlcRSTACKDigit * )  digits->At(index) ;
  if(digit->GetNprimary() == 0) 
    continue;
  //       printf("%6d  %8d    %6.5e %4d      %2d :",
    // 	      digit->GetId(), digit->GetAmp(), digit->GetTime(), digit->GetIndexInList(), digit->GetNprimary()) ;  // YVK
    printf("%6d  %4d    %6.5e %4d      %2d :",
	   digit->GetId(), digit->GetAmp(), digit->GetTime(), digit->GetIndexInList(), digit->GetNprimary()) ;  
	   Int_t iprimary;
	   for (iprimary=0; iprimary<digit->GetNprimary(); iprimary++) {
	     printf("%d ",digit->GetPrimary(iprimary+1) ) ; 
	   }    
	   printf("\n") ;
    }

}

//__________________________________________________________________
Float_t IlcRSTACKDigitizer::TimeOfNoise(void) const
{  // Calculates the time signal generated by noise
  //PH  Info("TimeOfNoise", "Change me") ; 
  return gRandom->Rndm() * 1.28E-5;
}

//__________________________________________________________________
void IlcRSTACKDigitizer::Unload() 
{  
  
  Int_t i ; 
  for(i = 1 ; i < fInput ; i++){
    TString tempo(fEventNames[i]) ; 
    tempo += i ;
    IlcRunLoader* rl = IlcRunLoader::GetRunLoader(tempo) ; 
    IlcRSTACKLoader * RSTACKLoader = static_cast<IlcRSTACKLoader*>(rl->GetLoader("RSTACKLoader"));
    RSTACKLoader->UnloadSDigits() ; 
  }
  
  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcRSTACKLoader * RSTACKLoader = static_cast<IlcRSTACKLoader*>(rl->GetLoader("RSTACKLoader"));
  RSTACKLoader->UnloadDigits() ; 
}

//____________________________________________________________________________
void IlcRSTACKDigitizer::WriteDigits()
{

  // Makes TreeD in the output file. 
  // Check if branch already exists: 
  //   if yes, exit without writing: ROOT TTree does not support overwriting/updating of 
  //      already existing branches. 
  //   else creates branch with Digits, named "RSTACK", title "...",
  //      and branch "IlcRSTACKDigitizer", with the same title to keep all the parameters
  //      and names of files, from which digits are made.

  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcRSTACKLoader * RSTACKLoader = static_cast<IlcRSTACKLoader*>(rl->GetLoader("RSTACKLoader"));
 
  const TClonesArray * digits = RSTACKLoader->Digits() ; 
  TTree * treeD = RSTACKLoader->TreeD();
  if(!treeD){
    RSTACKLoader->MakeTree("D");
    treeD = RSTACKLoader->TreeD();
  }
  
  // -- create Digits branch
  Int_t bufferSize = 32000 ;    
  TBranch * digitsBranch = treeD->Branch("RSTACK","TClonesArray",&digits,bufferSize);
  digitsBranch->SetTitle(fEventFolderName);
  digitsBranch->Fill() ;
  
  RSTACKLoader->WriteDigits("OVERWRITE");

  Unload() ; 

}
