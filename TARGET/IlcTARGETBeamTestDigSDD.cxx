////////////////////////////////////////////////////
//  Class to define                               //
//  SDD beam test raw 2 dig conv.                 //
//  Origin: E. Crescio crescio@to.infn.it         //
//                                                // 
//                                                //
////////////////////////////////////////////////////
#include "IlcTARGETdigitSDD.h"
#include "IlcRawReader.h"
#include "IlcVMERawStream.h"
#include "IlcTARGETRawStreamSDDv2.h"
#include "IlcTARGETRawStreamSDDv3.h"
#include "IlcTARGETRawStreamSDD.h"
#include "IlcTARGETBeamTestDigSDD.h"
#include "IlcTARGETEventHeader.h"
#include "IlcTARGETgeom.h"
#include <TBranch.h>
#include <TTree.h>
#include <TClonesArray.h>

ClassImp(IlcTARGETBeamTestDigSDD)

//_____________________________________________________________
  IlcTARGETBeamTestDigSDD::IlcTARGETBeamTestDigSDD(): IlcTARGETBeamTestDig()
{
  //
  // Constructor
  //
  SetBtPeriod();
  fSubEventAttributes=0;
  fThreshold=0;
  fStreamer=0;
}

//_____________________________________________________________
IlcTARGETBeamTestDigSDD::IlcTARGETBeamTestDigSDD(const Text_t* name, const Text_t* title): IlcTARGETBeamTestDig(name,title)
{
  //
  // Constructor
  //
  SetBtPeriod();
  fSubEventAttributes=0;
  fThreshold=0;
  fStreamer=0;
}

//__________________________________________________________________
IlcTARGETBeamTestDigSDD::~IlcTARGETBeamTestDigSDD()
{
  //
  // Destructor
  //
  if(fSubEventAttributes) delete fSubEventAttributes;
  if(fStreamer) delete fStreamer;
 }


//_______________________________________________________________________
void IlcTARGETBeamTestDigSDD::Exec(Option_t* /*opt*/)
{

  // Reads raw data and fills the tree of digits

  TBranch* branch = fTreeD->GetBranch("TARGETDigitsSDD");
  Int_t maxn=0;

  if(!fTARGETgeom){
    Error("Exec","fTARGETgeom is null!");
    return;
  }

  Int_t nsdd=0;
  Int_t nspd=0;
  Int_t nssd=0;
  for(Int_t nlay=1;nlay<=fTARGETgeom->GetNlayers();nlay++){
    for(Int_t nlad=1;nlad<=fTARGETgeom->GetNladders(nlay);nlad++){
      for(Int_t ndet=1;ndet<=fTARGETgeom->GetNdetectors(nlay);ndet++){
	Int_t index=fTARGETgeom->GetModuleIndex(nlay,nlad,ndet);
	if(fTARGETgeom->GetModuleTypeName(index)=="kSPD") nspd++;
	if(fTARGETgeom->GetModuleTypeName(index)=="kSDD") nsdd++;
	if(fTARGETgeom->GetModuleTypeName(index)=="kSSD") nssd++;
      }
    }
  }
  if(GetBtPeriod()==kAug04) maxn=nsdd;
  if(GetBtPeriod()==kNov04) maxn=nspd+nsdd+nssd;
  TClonesArray** digits = new TClonesArray*[maxn];
  Int_t* idig = new Int_t[maxn];
  for(Int_t idet=0;idet<maxn;idet++){
    digits[idet]=new TClonesArray("IlcTARGETdigitSDD");
    idig[idet]=0;
  }

  switch(fBtPer){
  case kNov04:
    fStreamer = new IlcTARGETRawStreamSDDv3(fReader);
    break;
  case kAug04:
    IlcVMERawStream vmeStreamer(fReader);
    fReader->RequireHeader(kFALSE);
    while(fReader->ReadHeader()){
      fSubEventAttributes = fReader->GetSubEventAttributes();
    }
    
    fSDDEvType=GetEventType();
    if(fSDDEvType==1) fTARGETHeader->SetEventTypeSDD(kReal);
    if(fSDDEvType==2) fTARGETHeader->SetEventTypeSDD(kCalibration1);
    if(fSDDEvType==3) fTARGETHeader->SetEventTypeSDD(kCalibration2);
    fReader->Reset();
    fStreamer = new IlcTARGETRawStreamSDDv2(fReader);
    break;
  }

 
  fStreamer->SetLowCarlosThreshold(fThreshold,0);
  fStreamer->SetLowCarlosThreshold(fThreshold,1);

  //from raw data the signal is already decompressed..
  //set compressed fSignal of IlcTARGETdigitSDD to -1000
  //set expanded fSignalExpanded of IlcTARGETdigitSDD equal to fStreamer.GetSignal() 
  while(fStreamer->Next()){   
    Int_t ndet =0;
    if(GetBtPeriod()==kNov04) ndet=fStreamer->GetChannel()+nspd;
    if(GetBtPeriod()==kAug04) ndet=fStreamer->GetChannel();
    Int_t anode = fStreamer->GetCoord1();

    /* if we are reading only one det, two wings
       if(fStreamer.GetChannel()==1) anode+=256; //wing 1 0-255, wing 2 256-511
    */

    /* bt august 2004 and november 2004: with only 1 carlos 
       channel 0 for one wing of one
       det, channel 1 for the wing of the second det*/

    const Int_t kdgt[3]={anode,fStreamer->GetCoord2(),-1000};
    const Int_t ktracks[10]={0,0,0,0,0,0,0,0,0,0};
    const Int_t khits[10]={0,0,0,0,0,0,0,0,0,0};
    const Float_t kcharges[10]={0,0,0,0,0,0,0,0,0,0};
 

    new ((*digits[ndet])[idig[ndet]]) IlcTARGETdigitSDD(0,kdgt,ktracks,khits,kcharges,fStreamer->GetSignal());
    idig[ndet]++;
  }

  if(GetBtPeriod()==kNov04){
    Int_t jitter=fStreamer->ReadJitter();
    fTARGETHeader->SetJitterSDD(jitter);
  }
  for(Int_t n = 0;n<maxn;n++){
    branch->SetAddress(&digits[n]);
    branch->Fill();
  
  }
      
  fTreeD->SetEntries(maxn);
  fReader->Reset();
  fTreeD->AutoSave();

  for(Int_t n=0;n<maxn;n++){
    delete digits[n];
  }

  
  delete[] digits;
  delete[] idig;
  delete fStreamer;
}

  
//______________________________________
Int_t IlcTARGETBeamTestDigSDD::GetEventType(){

  // defines the SDD event type:
  // 1: physics event (kReal)
  // 2: calibration 1 (kCalibration1, injector pulse)
  // 3: calibration 2 (kCalibration2, test pulse)
 
  fSDDEvType=0;
  if(fSubEventAttributes[0]==0 && fSubEventAttributes[1]==0 && fSubEventAttributes[2]==0) fSDDEvType = 1;
  if(fSubEventAttributes[0]==1 && fSubEventAttributes[1]==0 && fSubEventAttributes[2]==0) fSDDEvType = 2;

  if(fSubEventAttributes[0]==2 && fSubEventAttributes[1]==0 && fSubEventAttributes[2]==0) fSDDEvType = 3;

  return fSDDEvType;
}

//______________________________________________________________________
IlcTARGETBeamTestDigSDD::IlcTARGETBeamTestDigSDD(const IlcTARGETBeamTestDigSDD &bt):IlcTARGETBeamTestDig(bt){
    // Copy constructor. 

  fSDDEvType = bt.fSDDEvType;
  fSubEventAttributes = bt.fSubEventAttributes;
  fBtPer = bt.fBtPer;
  fThreshold = bt.fThreshold;
  fStreamer = bt.fStreamer;
}
//______________________________________________________________________
IlcTARGETBeamTestDigSDD& IlcTARGETBeamTestDigSDD::operator=(const IlcTARGETBeamTestDigSDD &source){
    // Assignment operator. This is a function which is not allowed to be
    // done to the TARGET beam test dig. It exits with an error.
    // Inputs:
    if(this==&source) return *this;
    Error("operator=","You are not allowed to make a copy of the IlcTARGETBeamTestDig");
    exit(1);
    return *this; //fake return
}




