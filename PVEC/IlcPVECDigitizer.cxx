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

/* $Id: IlcPVECDigitizer.cxx 54863 2012-02-28 12:10:29Z hristov $ */

//_________________________________________________________________________
//*-- Author :  Dmitri Peressounko (SUBATECH & Kurchatov Institute) 
//////////////////////////////////////////////////////////////////////////////
// This class performs digitization of Summable digits (in the PVEC this is
// the sum of contributions from all primary particles into a given cell). 
// In addition it performs mixing of summable digits from different events.
// The name of the class is also the title of the branch that will contain 
// the created SDigits
// The title of the class is the name of the file that contains the hits from
// which the SDigits are created
//
// For each event two branches are created in TreeD:
//   "PVEC" - list of digits
//   "IlcPVECDigitizer" - IlcPVECDigitizer with all parameters used in digitization
//
// Note, that one can set a title for new digits branch, and repeat digitization with
// another set of parameters.
//
// Use case:
// root[0] IlcPVECDigitizer * d = new IlcPVECDigitizer() ;
// root[1] d->Digitize()             
// Warning in <TDatabasePDG::TDatabasePDG>: object already instantiated
//                       //Digitizes SDigitis in all events found in file gilc.root 
//
// root[2] IlcPVECDigitizer * d1 = new IlcPVECDigitizer("gilc1.root") ;  
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
#include "IlcPVECDigit.h"
#include "IlcPVECDigitizer.h"
#include "IlcPVECGeometry.h"
#include "IlcPVECTick.h"
#include "IlcPVECSimParam.h"
#include "IlcPVECCalibData.h"
#include "IlcRunLoader.h"
#include "IlcPVECLoader.h"
#include "IlcPVECPulseGenerator.h"

ClassImp(IlcPVECDigitizer)


//____________________________________________________________________________ 
IlcPVECDigitizer::IlcPVECDigitizer() :
  IlcDigitizer("",""),
  fDefaultInit(kTRUE),
  fDigitsInRun(0),
  fInit(kFALSE),
  fInput(0),
  fInputFileNames(0x0),
  fEventNames(0x0),
  fEmcCrystals(0),
  fEventFolderName(""),
  fFirstEvent(0),
  fLastEvent(0), 
  fcdb(0x0),
  fEventCounter(0),
  fPulse(0),
  fADCValuesLG(0),
  fADCValuesHG(0)
{
  // ctor
  InitParameters() ; 
  fDigInput = 0 ;                     // We work in the standalong mode
}

//____________________________________________________________________________ 
IlcPVECDigitizer::IlcPVECDigitizer(TString ilcrunFileName, 
				   TString eventFolderName):
  IlcDigitizer("PVECDigitizer", ilcrunFileName), 
  fDefaultInit(kFALSE),
  fDigitsInRun(0),
  fInit(kFALSE),
  fInput(0),
  fInputFileNames(0x0),
  fEventNames(0x0),
  fEmcCrystals(0),
  fEventFolderName(eventFolderName),
  fFirstEvent(0),
  fLastEvent(0), 
  fcdb(0x0),
  fEventCounter(0),
  fPulse(0),
  fADCValuesLG(0),
  fADCValuesHG(0)
{
  // ctor
  InitParameters() ; 
  Init() ;
  fDefaultInit = kFALSE ; 
  fDigInput = 0 ;                     // We work in the standalone mode
  fcdb = new IlcPVECCalibData(-1);
}

//____________________________________________________________________________ 
IlcPVECDigitizer::IlcPVECDigitizer(IlcDigitizationInput * rd) :
  IlcDigitizer(rd,"PVECDigitizer"),
  fDefaultInit(kFALSE),
  fDigitsInRun(0),
  fInit(kFALSE),
  fInput(0),
  fInputFileNames(0x0),
  fEventNames(0x0),
  fEmcCrystals(0),
  fEventFolderName(fDigInput->GetInputFolderName(0)),
  fFirstEvent(0),
  fLastEvent(0), 
  fcdb (0x0), 
  fEventCounter(0),
  fPulse(0),
  fADCValuesLG(0),
  fADCValuesHG(0)

{
  // ctor Init() is called by RunDigitizer
  fDigInput = rd ; 
  SetTitle(static_cast<IlcStream*>(fDigInput->GetInputStream(0))->GetFileName(0));
  InitParameters() ; 
  fDefaultInit = kFALSE ; 
//   fcdb = new IlcPVECCalibData(-1);
}

//____________________________________________________________________________ 
  IlcPVECDigitizer::~IlcPVECDigitizer()
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
void IlcPVECDigitizer::Digitize(Int_t event) 
{ 
  
  // Makes the digitization of the collected summable digits.
  // It first creates the array of all PVEC modules
  // filled with noise and then adds contributions from SDigits. 
  // This design avoids scanning over the list of digits to add 
  // contribution to new SDigits only.

  Bool_t toMakeNoise = kTRUE ; //Do not create noisy digits if merge with real data

  //First stream 
  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcPVECLoader * PVECLoader = static_cast<IlcPVECLoader*>(rl->GetLoader("PVECLoader"));

  Int_t readEvent = event ; 
  if (fDigInput) 
    readEvent = static_cast<IlcStream*>(fDigInput->GetInputStream(0))->GetCurrentEventNumber() ; 
  IlcDebug(1,Form("Adding event %d from input stream 0 %s %s", 
		  readEvent, GetTitle(), fEventFolderName.Data())) ; 
  rl->GetEvent(readEvent) ;
  PVECLoader->CleanSDigits() ; 
  PVECLoader->LoadSDigits("READ") ;

  //Prepare Output
  TClonesArray * digits = PVECLoader->Digits() ;
  if( !digits ) {
    PVECLoader->MakeDigitsArray() ; 
    digits = PVECLoader->Digits() ;
  }
 
  digits->Clear() ;

  //
  const IlcPVECGeometry *geom = IlcPVECGeometry::GetInstance() ;
  //Making digits with noise, first EMC

  const Int_t nLay = geom->GetPVECNLayers();
  Int_t NXtalInModule=0;
  for(Int_t iLay=1; iLay<=nLay; iLay++)
    NXtalInModule += geom->GetPVECNXtalPhi(iLay);

  Int_t nXtal = NXtalInModule;
  
  Int_t absID ;
  
  digits->Expand(nXtal) ;

  //take all the inputs to add together and load the SDigits
  TObjArray * sdigArray = new TObjArray(fInput) ;
  sdigArray->AddAt(PVECLoader->SDigits(), 0) ;
 
  for(Int_t i = 1 ; i < fInput ; i++){
    TString tempo(fEventNames[i]) ; 
    tempo += i ;
    IlcRunLoader* rl2 = IlcRunLoader::GetRunLoader(tempo) ;
    if(!rl2){ 
      rl2 = IlcRunLoader::Open(fInputFileNames[i], tempo) ;
      if (!rl2) {
        Fatal("IlcPVECDigitizer", "Could not find the Run Loader for %s - %s",fInputFileNames[i].Data(), tempo.Data()) ;
        return ;
      }
      rl2->LoadHeader();
    }
    IlcPVECLoader * PVECLoader2 = static_cast<IlcPVECLoader*>(rl2->GetLoader("PVECLoader"));
 
    if(fDigInput){ 
      readEvent = static_cast<IlcStream*>(fDigInput->GetInputStream(i))->GetCurrentEventNumber() ; 
    }
    TClonesArray * digs ;
    if(IlcPVECSimParam::GetInstance()->IsStreamDigits(i)){ //This is Digits Stream
      IlcInfo(Form("Adding event %d from input stream %d %s %s", 
		 readEvent, i, fInputFileNames[i].Data(), tempo.Data())) ; 
       rl2->GetEvent(readEvent) ;
       PVECLoader2->CleanDigits() ;
       PVECLoader2->LoadDigits("READ") ;
       digs = PVECLoader2->Digits() ;
       toMakeNoise=0 ; //Do not add noise, it is already in stream
    }
    else{
      IlcInfo(Form("Adding event %d (SDigits) from input stream %d %s %s",
                 readEvent, i, fInputFileNames[i].Data(), tempo.Data())) ;
       rl2->GetEvent(readEvent) ;
       PVECLoader2->CleanSDigits() ; 
       PVECLoader2->LoadSDigits("READ") ;
       digs = PVECLoader2->SDigits() ;
    } 
    sdigArray->AddAt(digs, i) ;
  }

  //Find the first crystal with signal
  Int_t nextSig = 200000 ; 
  TClonesArray * sdigits ;  
  for(Int_t i = 0 ; i < fInput ; i++){
    sdigits = static_cast<TClonesArray *>(sdigArray->At(i)) ;
    if ( !sdigits->GetEntriesFast() )
      continue ; 
    Int_t curNext = static_cast<IlcPVECDigit *>(sdigits->At(0))->GetId() ;
    if(curNext < nextSig) 
      nextSig = curNext ;
  }
  
  TArrayI index(fInput) ;
  index.Reset() ;  //Set all indexes to zero
  
  IlcPVECDigit * digit ;
  IlcPVECDigit * curSDigit ;
  
//  TClonesArray * ticks = new TClonesArray("IlcPVECTick",1000) ;
  
  //Put Noise contribution
  Double_t apdNoise = 0. ;
  if(toMakeNoise)
     apdNoise = IlcPVECSimParam::GetInstance()->GetAPDNoise() ; 

  Int_t idigit= 0;
    for(absID = 1; absID <= nXtal; absID++){
      Float_t noise = gRandom->Gaus(0.,apdNoise) ; 
      new((*digits)[idigit]) IlcPVECDigit( -1, absID, noise, TimeOfNoise() ) ;
      //look if we have to add signal?
      digit = static_cast<IlcPVECDigit *>(digits->At(idigit)) ;
      idigit++ ;
    
      if(absID==nextSig){
      //Add SDigits from all inputs 
//      ticks->Clear() ;
//      Int_t contrib = 0 ;

//New Timing model is necessary
//      Float_t a = digit->GetEnergy() ;
//      Float_t b = TMath::Abs( a / fTimeSignalLength) ;
//      //Mark the beginning of the signal
//      new((*ticks)[contrib++]) IlcPVECTick(digit->GetTime(),0, b);  
//      //Mark the end of the signal     
//      new((*ticks)[contrib++]) IlcPVECTick(digit->GetTime()+fTimeSignalLength, -a, -b); 

// Calculate time as time of the largest digit
        Float_t time = digit->GetTime() ;
        Float_t eTime= digit->GetEnergy() ;
      
        //loop over inputs
        for(Int_t i = 0 ; i < fInput ; i++){
  	  if( static_cast<TClonesArray *>(sdigArray->At(i))->GetEntriesFast() > index[i] ){
  	    curSDigit = static_cast<IlcPVECDigit*>(static_cast<TClonesArray *>(sdigArray->At(i))->At(index[i])) ; 	
            if(IlcPVECSimParam::GetInstance()->IsStreamDigits(i)){ //This is Digits Stream
              curSDigit->SetEnergy(Calibrate(curSDigit->GetEnergy(),curSDigit->GetId())) ;
              curSDigit->SetTime(CalibrateT(curSDigit->GetTime(),curSDigit->GetId())) ;
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
	    //	  new((*ticks)[contrib++]) IlcPVECTick(curSDigit->GetTime(),0, b);  
	    //	  new((*ticks)[contrib++]) IlcPVECTick(curSDigit->GetTime()+fTimeSignalLength, -a, -b); 
          if(curSDigit->GetEnergy()>eTime){
            eTime=curSDigit->GetEnergy() ;
            time=curSDigit->GetTime() ;
          }
	    *digit += *curSDigit ;  //add energies

	    index[i]++ ;
	    if( static_cast<TClonesArray *>(sdigArray->At(i))->GetEntriesFast() > index[i] )
	      curSDigit = static_cast<IlcPVECDigit*>(static_cast<TClonesArray *>(sdigArray->At(i))->At(index[i])) ; 	
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
	    curNext = static_cast<IlcPVECDigit *>(sdigits->At(index[i]))->GetId() ;
	  }
	  if(curNext < nextSig) nextSig = curNext ;
        }
      }
  }


  //Apply non-linearity
  if(IlcPVECSimParam::GetInstance()->IsCellNonlinearityOn()){ //Apply non-lineairyt on cell level
    const Double_t aNL = IlcPVECSimParam::GetInstance()->GetCellNonLineairyA() ;
    const Double_t bNL = IlcPVECSimParam::GetInstance()->GetCellNonLineairyB() ;
    const Double_t cNL = IlcPVECSimParam::GetInstance()->GetCellNonLineairyC() ;
    for(Int_t i = 0 ; i < nXtal ; i++){
      digit = static_cast<IlcPVECDigit*>( digits->At(i) ) ;
      Double_t e= digit->GetEnergy() ;
      // version(1)      digit->SetEnergy(e*(1+a*TMath::Exp(-e/b))) ;
      digit->SetEnergy(e*cNL*(1.+aNL*TMath::Exp(-e*e/2./bNL/bNL))) ; //Better agreement with data...
    }  
  }


  //distretize energy if necessary
  if(IlcPVECSimParam::GetInstance()->IsEDigitizationOn()){
    Float_t adcW=IlcPVECSimParam::GetInstance()->GetADCchannelW() ;
    for(Int_t i = 0 ; i < nXtal ; i++){                                                                                                       
      digit = static_cast<IlcPVECDigit*>( digits->At(i) ) ;
      digit->SetEnergy(adcW*ceil(digit->GetEnergy()/adcW)) ;
    } 
  }
 
  //Apply decalibration if necessary
  for(Int_t i = 0 ; i < nXtal ; i++){
    digit = static_cast<IlcPVECDigit*>( digits->At(i) ) ;
    Decalibrate(digit) ;
  }
  
//  ticks->Delete() ;
//  delete ticks ;
  
  delete sdigArray ; //We should not delete its contents 
  
  Int_t relId[4];

  //set amplitudes in bad channels to zero

  for(Int_t i = 0 ; i <digits->GetEntriesFast(); i++){
    digit = static_cast<IlcPVECDigit*>( digits->At(i) ) ;
//     geom->AbsToRelNumbering(digit->GetId(),relId);
//     if(relId[1] == 0) // Emc
//       if(fcdb->IsBadChannelEmc(relId[0],relId[3],relId[2])) digit->SetEnergy(0.); 
  }

  //remove digits below thresholds
//   Float_t XtalThreshold = IlcPVECSimParam::GetInstance()->GetEmcDigitsThreshold() ;
  Float_t XtalThreshold = .01; //FIXME
  for(Int_t i = 0 ; i < nXtal ; i++){
    digit = static_cast<IlcPVECDigit*>( digits->At(i) ) ;

    if(digit->GetEnergy() < XtalThreshold){
      digits->RemoveAt(i) ;
      continue ;
    }

    geom->AbsToRelNumbering(digit->GetId(),relId);

//    digit->SetEnergy(TMath::Ceil(digit->GetEnergy())-0.9999) ;

    Float_t tres = TimeResolution(digit->GetEnergy()) ; 
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
    digit = static_cast<IlcPVECDigit*>( digits->At(i) ) ; 
    digit->SetIndexInList(i) ;     
  }

}
//____________________________________________________________________________
Float_t IlcPVECDigitizer::Calibrate(Float_t amp,Int_t /*absId*/){
  //Apply calibration
//   const IlcPVECGeometry *geom = IlcPVECGeometry::GetInstance() ;
// 
//   //Determine rel.position of the cell absolute ID
//   Int_t relId[4];
//   geom->AbsToRelNumbering(absId,relId);
//   Int_t module=relId[0];
//   Int_t row   =relId[2];
//   Int_t column=relId[3];
//   if(relId[1]==0){ //This Is EMC
//     Float_t calibration = fcdb->GetADCchannelEmc(module,column,row);
    Float_t calibration = 1.; //FIXME
    return amp*calibration ;
//   }
//   return 0 ;
}
//____________________________________________________________________________
void IlcPVECDigitizer::Decalibrate(IlcPVECDigit *digit)
{
  // Decalibrate EMC digit, i.e. change its energy by a factor read from CDB

//   const IlcPVECGeometry *geom = IlcPVECGeometry::GetInstance() ;
// 
//   //Determine rel.position of the cell absolute ID
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
Float_t IlcPVECDigitizer::CalibrateT(Float_t time,Int_t /*absId*/){
  //Apply time calibration
//   const IlcPVECGeometry *geom = IlcPVECGeometry::GetInstance() ;
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
void IlcPVECDigitizer::Digitize(Option_t *option) 
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
     gBenchmark->Start("PVECDigitizer");
  
  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcPVECLoader * PVECLoader = static_cast<IlcPVECLoader*>(rl->GetLoader("PVECLoader"));
  
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
    fDigitsInRun += PVECLoader->Digits()->GetEntriesFast() ;  
 }
 
  if(strstr(option,"tim")){
    gBenchmark->Stop("PVECDigitizer");
    TString message ; 
    message = "  took %f seconds for Digitizing %f seconds per event\n" ; 
    IlcInfo(Form( message.Data(), 
	 gBenchmark->GetCpuTime("PVECDigitizer"), 
	 gBenchmark->GetCpuTime("PVECDigitizer")/nEvents )); 
  } 
}
//____________________________________________________________________________ 
Float_t IlcPVECDigitizer::TimeResolution(Float_t e){
  //calculate TOF resolution using beam-test resutls
  Float_t a=IlcPVECSimParam::GetInstance()->GetTOFa() ;
  Float_t b=IlcPVECSimParam::GetInstance()->GetTOFb() ;
  return TMath::Sqrt(a*a+b*b/e) ;
}

////____________________________________________________________________________ 
//Float_t IlcPVECDigitizer::FrontEdgeTime(TClonesArray * ticks) const
//{
//  // Returns the shortest time among all time ticks
//
//  ticks->Sort() ; //Sort in accordance with times of ticks
//  TIter it(ticks) ;
//  IlcPVECTick * ctick = (IlcPVECTick *) it.Next() ;
//  Float_t time = ctick->CrossingTime(fTimeThreshold) ;    
//
//  IlcPVECTick * t ;  
//  while((t=(IlcPVECTick*) it.Next())){
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
Bool_t IlcPVECDigitizer::Init()
{
  // Makes all memory allocations
  fInit = kTRUE ; 

  IlcPVECGeometry *geom;
  if (!(geom = IlcPVECGeometry::GetInstance())) 
        geom = IlcPVECGeometry::GetInstance("ORKA","");
//   const IlcPVECGeometry *geom = IlcPVECGeometry::GetInstance() ;

  fEmcCrystals = 100 ; //FIXME
  
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
void IlcPVECDigitizer::InitParameters()
{
  // Set initial parameters Digitizer

  fDigitsInRun  = 0 ; 
  SetEventRange(0,-1) ;
  fPulse = new IlcPVECPulseGenerator();
  fADCValuesLG = new Int_t[fPulse->GetRawFormatTimeBins()];
  fADCValuesHG = new Int_t[fPulse->GetRawFormatTimeBins()];
    
}

//__________________________________________________________________
void IlcPVECDigitizer::Print(const Option_t *)const 
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
    IlcInfo(Form("IlcPVECDigitizer not initialized" )) ;
  
}

//__________________________________________________________________
 void IlcPVECDigitizer::PrintDigits(Option_t * /*option*/)
{
  // Print a table of digits
  

  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcPVECLoader * PVECLoader = static_cast<IlcPVECLoader*>(rl->GetLoader("PVECLoader"));
  TClonesArray * digits = PVECLoader->Digits() ; 
//   const IlcPVECGeometry *geom = IlcPVECGeometry::GetInstance() ;
  
  IlcInfo(Form("%d", digits->GetEntriesFast())) ; 
  printf("\nevent %d", gIlc->GetEvNumber()) ;
  printf("\n       Number of entries in Digits list %d", digits->GetEntriesFast() )  ;  
  
  
  //loop over digits
  IlcPVECDigit * digit;
  printf("\nEMC digits (with primaries):\n")  ;
  printf("\n   Id  Amplitude    Time          Index Nprim: Primaries list \n") ;    
  Int_t maxEmc = 100;//FIXME
  Int_t index ;
  for (index = 0 ; (index < digits->GetEntriesFast()) && 
    (static_cast<IlcPVECDigit *>(digits->At(index))->GetId() <= maxEmc) ; index++) {
    digit = (IlcPVECDigit * )  digits->At(index) ;
  if(digit->GetNprimary() == 0) 
    continue;
  //       printf("%6d  %8d    %6.5e %4d      %2d :",
    // 	      digit->GetId(), digit->GetAmp(), digit->GetTime(), digit->GetIndexInList(), digit->GetNprimary()) ;  // YVK
    printf("%6d  %.4f    %6.5e %4d      %2d :",
	   digit->GetId(), digit->GetEnergy(), digit->GetTime(), digit->GetIndexInList(), digit->GetNprimary()) ;  
	   Int_t iprimary;
	   for (iprimary=0; iprimary<digit->GetNprimary(); iprimary++) {
	     printf("%d ",digit->GetPrimary(iprimary+1) ) ; 
	   }    
	   printf("\n") ;
    }
 
}

//__________________________________________________________________
Float_t IlcPVECDigitizer::TimeOfNoise(void) const
{  // Calculates the time signal generated by noise
  //PH  Info("TimeOfNoise", "Change me") ; 
  return gRandom->Rndm() * 1.28E-5;
}

//__________________________________________________________________
void IlcPVECDigitizer::Unload() 
{  
  
  Int_t i ; 
  for(i = 1 ; i < fInput ; i++){
    TString tempo(fEventNames[i]) ; 
    tempo += i ;
    IlcRunLoader* rl = IlcRunLoader::GetRunLoader(tempo) ; 
    IlcPVECLoader * PVECLoader = static_cast<IlcPVECLoader*>(rl->GetLoader("PVECLoader"));
    PVECLoader->UnloadSDigits() ; 
  }
  
  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcPVECLoader * PVECLoader = static_cast<IlcPVECLoader*>(rl->GetLoader("PVECLoader"));
  PVECLoader->UnloadDigits() ; 
}

//____________________________________________________________________________
void IlcPVECDigitizer::WriteDigits()
{

  // Makes TreeD in the output file. 
  // Check if branch already exists: 
  //   if yes, exit without writing: ROOT TTree does not support overwriting/updating of 
  //      already existing branches. 
  //   else creates branch with Digits, named "PVEC", title "...",
  //      and branch "IlcPVECDigitizer", with the same title to keep all the parameters
  //      and names of files, from which digits are made.

  IlcRunLoader* rl = IlcRunLoader::GetRunLoader(fEventFolderName) ;
  IlcPVECLoader * PVECLoader = static_cast<IlcPVECLoader*>(rl->GetLoader("PVECLoader"));
 
  const TClonesArray * digits = PVECLoader->Digits() ; 
  TTree * treeD = PVECLoader->TreeD();
  if(!treeD){
    PVECLoader->MakeTree("D");
    treeD = PVECLoader->TreeD();
  }
  
  // -- create Digits branch
  Int_t bufferSize = 32000 ;    
  TBranch * digitsBranch = treeD->Branch("PVEC","TClonesArray",&digits,bufferSize);
  digitsBranch->SetTitle(fEventFolderName);
  digitsBranch->Fill() ;
  
  PVECLoader->WriteDigits("OVERWRITE");

  Unload() ; 

}
