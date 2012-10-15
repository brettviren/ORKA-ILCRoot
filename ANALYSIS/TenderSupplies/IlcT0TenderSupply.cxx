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

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//  T0 Tender supply    //
//  //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include <IlcESDEvent.h>
#include <IlcESDtrack.h>

#include <IlcTender.h>
#include <IlcT0TenderSupply.h>
#include <IlcCDBManager.h>
#include <IlcCDBEntry.h>
#include <IlcT0CalibSeasonTimeShift.h>
#include <IlcESDInputHandler.h>

ClassImp(IlcT0TenderSupply)

//________________________________________________________________________
IlcT0TenderSupply::IlcT0TenderSupply():
  IlcTenderSupply(),
  fCorrectMeanTime(kFALSE),
  fCorrectStartTimeOnAmplSatur(kFALSE),
  fAmplitudeThreshold(100), 
  fPass4LHC11aCorrection(kFALSE)
{
  //
  // default constructor
  //
  for(int i=0; i<4; i++) fTimeOffset[i]=0;
}

//________________________________________________________________________
IlcT0TenderSupply::IlcT0TenderSupply(const char *name, const IlcTender *tender):
  IlcTenderSupply(name,tender),
  fCorrectMeanTime(kFALSE),
  fCorrectStartTimeOnAmplSatur(kFALSE),
  fAmplitudeThreshold(100),
  fPass4LHC11aCorrection(kFALSE)
{
  //
  // constructor
  //
  for(int i=0; i<3; i++) fTimeOffset[i]=0;

}

//________________________________________________________________________
IlcT0TenderSupply::~IlcT0TenderSupply(){
  //
  // destructor
  //
  
}

//________________________________________________________________________
void IlcT0TenderSupply::Init(){
  // Init
  //
  Int_t run = fTender->GetRun();
  if (run == 0) return;    // to skip first init, when we don't have yet a run number
  fPass4LHC11aCorrection=kFALSE;
  

  fCorrectMeanTime = kFALSE; //reset
  for(int i=0; i<4; i++) fTimeOffset[i]=0;


  fCorrectStartTimeOnAmplSatur = kFALSE;
  fAmplitudeThreshold = 100; //in mips
  
  if(167693<= run && run<=170593){  // LHC11h
    fCorrectStartTimeOnAmplSatur = kTRUE;
    fAmplitudeThreshold = 50; //in mips
  }

}

//________________________________________________________________________
void IlcT0TenderSupply::ProcessEvent(){
    //
    // loop over all online T0 candidates and flag
    // selected daughter tracks using the status bis of the TObject
    //

    IlcESDEvent *event=fTender->GetEvent();
    if (!event) return;
     //...........................................
   //Do something when the run number changed, like loading OCDB entries etc.
    if(fTender->RunChanged()){
      Init();
      if (fTender->GetRun()>=139699&&  fTender->GetRun()<=146860){
        IlcESDInputHandler *esdIH = dynamic_cast<IlcESDInputHandler*>  (fTender->GetESDhandler());
        if (esdIH) {
          TTree *tree= (TTree*)esdIH->GetTree();
          TFile *file= (TFile*)tree->GetCurrentFile();
         if (file){
            TString fileName(file->GetName());
            if (fileName.Contains("pass4") ) fPass4LHC11aCorrection=kTRUE;
	  }
	}
      }
    }
    
    if(fPass4LHC11aCorrection) {
      const Double32_t* mean = event->GetT0TOF();
      event->SetT0TOF(0, (mean[1]+mean[2])/2.);
 
    }
    //...........................................
    if(fCorrectStartTimeOnAmplSatur){
        //correct A side ORA on amplitude saturation
        const Double32_t* time = event->GetT0time();
        const Double32_t* amplitude = event->GetT0amplitude();

        Int_t idxOfFirstPmtA = -1;
        Double32_t timeOrA   = 99999;
        for(int ipmt=12; ipmt<24; ipmt++){ //loop over A side
            if( amplitude[ipmt] < fAmplitudeThreshold){
                if( time[ipmt] > -200 && time[ipmt]!=0 && time[ipmt] < timeOrA ){ 
                    timeOrA        = time[ipmt];
                    idxOfFirstPmtA = ipmt;
                }
            }
        }

        if(idxOfFirstPmtA>-1){ //a hit in aside with less than 40 mips
            const Double32_t* mean = event->GetT0TOF();
            Double32_t timeOrC = mean[2];
            Double32_t timeOrAplusOrC = (timeOrA+timeOrC)/2;

            event->SetT0TOF(0, timeOrAplusOrC);
            event->SetT0TOF(1, timeOrA);
        }
    }

    //...........................................
    Float_t *t0means=0x0;
    if(fCorrectMeanTime) {
      IlcCDBManager* ocdbMan = IlcCDBManager::Instance();
      ocdbMan->SetRun(fTender->GetRun());    
      IlcCDBEntry *entry = ocdbMan->Get("T0/Calib/TimeAdjust/");
      if(entry) {
	IlcT0CalibSeasonTimeShift *clb = (IlcT0CalibSeasonTimeShift*) entry->GetObject();
	t0means= clb->GetT0Means();
      } else {
	for (Int_t i=0;i<4;i++) t0means[i]=0;
	IlcWarning("T0Tender no T0 entry found T0shift set to 0");
      }
      
      // correct mean time offsets  
      const Double32_t* mean = event->GetT0TOF();
      for(int it0=0; it0<3; it0++){
	if(-2000 < mean[it0]){
	  event->SetT0TOF(it0, mean[it0] - t0means[it0]); 
	}
      }
    }
    //...........................................


}


