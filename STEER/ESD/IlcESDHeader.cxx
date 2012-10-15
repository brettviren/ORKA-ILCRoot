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

//-------------------------------------------------------------------------
//                      Implementation of   Class IlcESDHeader
//   Header data
//   for the ESD   
//   Origin: Christian Klein-Boesing, CERN, Christian.Klein-Boesing@cern.ch 
//-------------------------------------------------------------------------

#include "IlcESDHeader.h"
#include "IlcTriggerScalersESD.h"
#include "IlcTriggerScalersRecordESD.h"
#include "IlcTriggerIR.h"
#include "IlcTriggerConfiguration.h"
#include "IlcLog.h" 

ClassImp(IlcESDHeader)

//______________________________________________________________________________
IlcESDHeader::IlcESDHeader() :
  IlcVHeader(),
  fTriggerMask(0),
  fOrbitNumber(0),
  fTimeStamp(0),
  fEventType(0),
  fEventSpecie(0),
  fPeriodNumber(0),
  fEventNumberInFile(0),
  fBunchCrossNumber(0),
  fTriggerCluster(0),
  fL0TriggerInputs(0),
  fL1TriggerInputs(0),
  fL2TriggerInputs(0),
  fTriggerScalers(),
  fTriggerScalersDeltaEvent(),
  fTriggerScalersDeltaRun(),
  fTriggerInputsNames(kNTriggerInputs),
  fCTPConfig(NULL),
  fIRBufferArray()
{
  // default constructor

  SetName("IlcESDHeader");
  for(Int_t i = 0; i<kNMaxIR ; i++) fIRArray[i] = 0;
  fTriggerInputsNames.SetOwner(kTRUE);
}

IlcESDHeader::~IlcESDHeader() 
{
  // destructor
  for(Int_t i=0;i<kNMaxIR;i++)if(fIRArray[i])delete fIRArray[i];
  delete fCTPConfig;

  fIRBufferArray.Delete();
}


IlcESDHeader::IlcESDHeader(const IlcESDHeader &header) :
  IlcVHeader(header),
  fTriggerMask(header.fTriggerMask),
  fOrbitNumber(header.fOrbitNumber),
  fTimeStamp(header.fTimeStamp),
  fEventType(header.fEventType),
  fEventSpecie(header.fEventSpecie),
  fPeriodNumber(header.fPeriodNumber),
  fEventNumberInFile(header.fEventNumberInFile),
  fBunchCrossNumber(header.fBunchCrossNumber),
  fTriggerCluster(header.fTriggerCluster),
  fL0TriggerInputs(header.fL0TriggerInputs),
  fL1TriggerInputs(header.fL1TriggerInputs),
  fL2TriggerInputs(header.fL2TriggerInputs),
  fTriggerScalers(header.fTriggerScalers),
  fTriggerScalersDeltaEvent(header.fTriggerScalersDeltaEvent),
  fTriggerScalersDeltaRun(header.fTriggerScalersDeltaRun),
  fTriggerInputsNames(TObjArray(kNTriggerInputs)),
  fCTPConfig(header.fCTPConfig),
  fIRBufferArray()
{
  // copy constructor
  for(Int_t i = 0; i<kNMaxIR ; i++) {
    if(header.fIRArray[i])fIRArray[i] = new IlcTriggerIR(*header.fIRArray[i]);
    else fIRArray[i]=0;
  }
  for(Int_t i = 0; i < kNTriggerInputs; i++) {
    TNamed *str = (TNamed *)((header.fTriggerInputsNames).At(i));
    if (str) fTriggerInputsNames.AddAt(new TNamed(*str),i);
  }

  for(Int_t i = 0; i < (header.fIRBufferArray).GetEntries(); ++i) {
    IlcTriggerIR *ir = (IlcTriggerIR*)((header.fIRBufferArray).At(i));
    if (ir) fIRBufferArray.Add(new IlcTriggerIR(*ir));
  }
}

IlcESDHeader& IlcESDHeader::operator=(const IlcESDHeader &header)
{ 
  // assigment operator
  if(this!=&header) {
    IlcVHeader::operator=(header);
    fTriggerMask = header.fTriggerMask;
    fOrbitNumber = header.fOrbitNumber;
    fTimeStamp = header.fTimeStamp;
    fEventType = header.fEventType;
    fEventSpecie = header.fEventSpecie;
    fPeriodNumber = header.fPeriodNumber;
    fEventNumberInFile = header.fEventNumberInFile;
    fBunchCrossNumber = header.fBunchCrossNumber;
    fTriggerCluster = header.fTriggerCluster;
    fL0TriggerInputs = header.fL0TriggerInputs;
    fL1TriggerInputs = header.fL1TriggerInputs;
    fL2TriggerInputs = header.fL2TriggerInputs;
    fTriggerScalers = header.fTriggerScalers;
    fTriggerScalersDeltaEvent = header.fTriggerScalersDeltaEvent;
    fTriggerScalersDeltaRun = header.fTriggerScalersDeltaRun;
    delete fCTPConfig;
    fCTPConfig = header.fCTPConfig;

    fTriggerInputsNames.Clear();
    for(Int_t i = 0; i < kNTriggerInputs; i++) {
      TNamed *str = (TNamed *)((header.fTriggerInputsNames).At(i));
      if (str) fTriggerInputsNames.AddAt(new TNamed(*str),i);
    }
    for(Int_t i = 0; i<kNMaxIR ; i++) {
      delete fIRArray[i];
       if(header.fIRArray[i])fIRArray[i] = new IlcTriggerIR(*header.fIRArray[i]);
       else fIRArray[i]=0;
    }

    fIRBufferArray.Delete();
    for(Int_t i = 0; i < (header.fIRBufferArray).GetEntries(); ++i) {
      IlcTriggerIR *ir = (IlcTriggerIR*)((header.fIRBufferArray).At(i));
      if (ir) fIRBufferArray.Add(new IlcTriggerIR(*ir));
    }
  }
  return *this;
}

void IlcESDHeader::Copy(TObject &obj) const 
{  
  // this overwrites the virtual TOBject::Copy()
  // to allow run time copying without casting
  // in IlcESDEvent

  if(this==&obj)return;
  IlcESDHeader *robj = dynamic_cast<IlcESDHeader*>(&obj);
  if(!robj)return; // not an IlcESDHeader
  *robj = *this;

}
//______________________________________________________________________________
void IlcESDHeader::Reset()
{
  // reset all data members
  fTriggerMask       = 0;
  fOrbitNumber       = 0;
  fTimeStamp         = 0;
  fEventType         = 0;
  fEventSpecie       = 0;
  fPeriodNumber      = 0;
  fEventNumberInFile = 0;
  fBunchCrossNumber  = 0;
  fTriggerCluster    = 0;
  fL0TriggerInputs   = 0;
  fL1TriggerInputs   = 0;
  fL2TriggerInputs   = 0;
  fTriggerScalers.Reset();
  fTriggerScalersDeltaEvent.Reset();
  fTriggerScalersDeltaRun.Reset();
  fTriggerInputsNames.Clear();
  delete fCTPConfig;
  fCTPConfig = 0;
  for(Int_t i=0;i<kNMaxIR;i++)if(fIRArray[i]){
   delete fIRArray[i];
   fIRArray[i]=0;
  }

  fIRBufferArray.Delete();
}
//______________________________________________________________________________
Bool_t IlcESDHeader::AddTriggerIR(const IlcTriggerIR* ir)
{
  // Add an IR object into the array
  // of IRs in the ESD header

 fIRBufferArray.Add(new IlcTriggerIR(*ir));

 return kTRUE;
}
//______________________________________________________________________________
void IlcESDHeader::Print(const Option_t *) const
{
  // Print some data members
  printf("Event # %d in file Bunch crossing # %d Orbit # %d Trigger %lld \n",
	 GetEventNumberInFile(),
	 GetBunchCrossNumber(),
	 GetOrbitNumber(),
	 GetTriggerMask());
         printf("List of the active trigger inputs: ");
  	 for(Int_t i = 0; i < kNTriggerInputs; i++) {
    	   TNamed *str = (TNamed *)((fTriggerInputsNames).At(i));
    	   if (str) printf("%s ",str->GetName());
         }
         printf("\n");
}

//______________________________________________________________________________
void IlcESDHeader::SetActiveTriggerInputs(const char*name, Int_t index)
{
  // Fill the active trigger inputs names
  // into the corresponding fTriggerInputsNames (TObjArray of TNamed)
  if (index >= kNTriggerInputs || index < 0) {
    IlcError(Form("Index (%d) is outside the allowed range (0,59)!",index));
    return;
  }

  fTriggerInputsNames.AddAt(new TNamed(name,NULL),index);
}
//______________________________________________________________________________
const char* IlcESDHeader::GetTriggerInputName(Int_t index, Int_t trglevel) const
{
  // Get the trigger input name
  // at the specified position in the trigger mask and trigger level (0,1,2)
  TNamed *trginput = 0;
  if (trglevel == 0) trginput = (TNamed *)fTriggerInputsNames.At(index);
  if (trglevel == 1) trginput = (TNamed *)fTriggerInputsNames.At(index+24);  
  if (trglevel == 2) trginput = (TNamed *)fTriggerInputsNames.At(index+48); 
  if (trginput) return trginput->GetName();
  else return "";
}
//______________________________________________________________________________
TString IlcESDHeader::GetActiveTriggerInputs() const
{
  // Returns the list with the names of the active trigger inputs
  TString trginputs;
  for(Int_t i = 0; i < kNTriggerInputs; i++) {
    TNamed *str = (TNamed *)((fTriggerInputsNames).At(i));
    if (str) {
      trginputs += " ";
      trginputs += str->GetName();
      trginputs += " ";
    }
  }

  return trginputs;
}
//______________________________________________________________________________
TString IlcESDHeader::GetFiredTriggerInputs() const
{
  // Returns the list with the names of the fired trigger inputs
  TString trginputs;
  for(Int_t i = 0; i < kNTriggerInputs; i++) {
      TNamed *str = (TNamed *)((fTriggerInputsNames.At(i)));
      if (i < 24 && (fL0TriggerInputs & (1 << i))) {
        if (str) {
	  trginputs += " ";
	  trginputs += str->GetName();
          trginputs += " ";
        }
      }
      if (i >= 24 && i < 48 && (fL1TriggerInputs & (1 << (i-24)))) {
        if (str) {
	  trginputs += " ";
	  trginputs += str->GetName();
          trginputs += " ";
        }
      }
      if (i >= 48 && (fL2TriggerInputs & (1 << (i-48)))) {
        if (str) {
	  trginputs += " ";
	  trginputs += str->GetName();
          trginputs += " ";
        }
      }

  }
  return trginputs;
}
//______________________________________________________________________________
Bool_t IlcESDHeader::IsTriggerInputFired(const char *name) const
{
  // Checks if the trigger input is fired 
 
  TNamed *trginput = (TNamed *)fTriggerInputsNames.FindObject(name);
  if (!trginput) return kFALSE;

  Int_t inputIndex = fTriggerInputsNames.IndexOf(trginput);
  if (inputIndex < 0) return kFALSE;
  
  if (fL0TriggerInputs & (1 << inputIndex)) return kTRUE;
  else if (fL1TriggerInputs & (1 << (inputIndex-24))) return kTRUE;
  else if (fL2TriggerInputs & (1 << (inputIndex-48))) return kTRUE;
  else return kFALSE;
}
//________________________________________________________________________________
Int_t  IlcESDHeader::GetTriggerIREntries(Int_t int1, Int_t int2, Float_t deltaTime) const
{
  // returns number of IR-s within time window deltaTime
  // all possible combinations of int1 and int2 int1 - zdc bit, int2 v0 bit
  //
  const IlcTriggerIR *IR;
  // triggered event 
  Int_t nIR = GetTriggerIREntries();
  UInt_t orbit1 = GetOrbitNumber();
  const Double_t ot=0.0889218; //orbit time msec
  Float_t timediff; // time difference between orbits (msec)
  //
  Int_t nofIR;
  nofIR=0;
  // loop over IR-s
    for(Int_t i=0;i<nIR;i++){//1
      IR=GetTriggerIR(i);
      //
      UInt_t orbit2 = IR->GetOrbit();
      timediff = (orbit2<=orbit1) ? (Float_t)((orbit1-orbit2))*ot : 
	(Float_t)((16777215-orbit1+orbit2))*ot;
      if (timediff>deltaTime) continue; //timediff outside time window
      if((int1&int2) == -1){ //ignore both bits, just count IR-s within time window
	nofIR++;
        continue;
      }
      // now check if int1, int2 bits are set
      UInt_t nw = IR->GetNWord();
      Bool_t *bint1 = IR->GetInt1s();
      Bool_t *bint2 = IR->GetInt2s();
      //
      Int_t flag1,flag2;
      flag1=0;
      flag2=0;
      for(UInt_t j=0;j<nw;j++){//2
	if(bint1[j]) flag1=1; // at least one int1 set
	if(bint2[j]) flag2=1; //  at least one int2 set
        //printf("IR %d, bint1 %d, bint2 %d\n",i,bint1[j],bint2[j]);
      }//2
      // checking combinations
      //
      
      if((flag1*int1*flag2*int2)==1){// int1=1 & int2=1	 
          nofIR++;
          continue;       
      }
      if(int1 == -1){// ignore int1
        if(flag2&int2){// int2=1
          nofIR++;
          continue;
	}
        else if (!flag2&!int2){ //int2=0 
          nofIR++;
          continue;          
	}
      }
      
      if(int2 ==-1){//ignore int2
        if(flag1&int1){//int1=1
          nofIR++;
          continue;  
	}
        else if(!flag1&!int1){ //int1=0
          nofIR++;
          continue;  
	}
      }
      
      if((flag1*int1)&!flag2&!int2){// int1=1, int2=0
          nofIR++;
          continue;  
      }
      
      if((int2*flag2)&!int1&!flag1){// int1=0, int2=1
          nofIR++;
          continue;  
      } 
         
      

    }//1
  
    return nofIR;
}
//__________________________________________________________________________
TObjArray IlcESDHeader::GetIRArray(Int_t int1, Int_t int2, Float_t deltaTime) const
{
  //
  // returns an array of IR-s within time window deltaTime
  // all possible combinations of int1 and int2 int1 - zdc bit, int2 v0 bit
  //
  const IlcTriggerIR *IR;
  TObjArray arr;
  // triggered event 
  Int_t nIR = GetTriggerIREntries();
  UInt_t orbit1 = GetOrbitNumber();
  const Double_t ot=0.0889218; //orbit time msec
  Float_t timediff; // time difference between orbits (msec)
  //
  // loop over IR-s
    for(Int_t i=0;i<nIR;i++){//1
      IR=GetTriggerIR(i);
      //
      UInt_t orbit2 = IR->GetOrbit();
      timediff = (orbit2<=orbit1) ? (Float_t)((orbit1-orbit2))*ot : 
	(Float_t)((16777215-orbit1+orbit2))*ot;
      if (timediff>deltaTime) continue; //timediff outside time window
      if((int1&int2) == -1){ //ignore both bits, just count IR-s within time window
	arr.Add((IlcTriggerIR*)IR); //add this IR
        continue;
      }
      // now check if int1, int2 bits are set
      UInt_t nw = IR->GetNWord();
      Bool_t *bint1 = IR->GetInt1s();
      Bool_t *bint2 = IR->GetInt2s();
      //
      Int_t flag1,flag2;
      flag1=0;
      flag2=0;
      for(UInt_t j=0;j<nw;j++){//2
	if(bint1[j]) flag1=1; // at least one int1 set
	if(bint2[j]) flag2=1; //  at least one int2 set
      }//2
      // checking combinations
      //
      if((flag1*int1*flag2*int2)==1){// int1=1 & int2=1
	  arr.Add((IlcTriggerIR*)IR); //add this IR
          continue;       
      }
      if(int1 == -1){// ignore int1
        if(flag2&int2){// int2=1
 	  arr.Add((IlcTriggerIR*)IR); //add this IR
          continue;
	}
        else if (!flag2&!int2){ //int2=0 
          arr.Add((IlcTriggerIR*)IR); //add this IR
          continue;          
	}
      }
      if(int2 ==-1){//ignore int2
        if(flag1&int1){//int1=1
	  arr.Add((IlcTriggerIR*)IR); //add this IR
          continue;  
	}
        else if(!flag1&!int1){ //int1=0
	  arr.Add((IlcTriggerIR*)IR); //add this IR
          continue;  
	}
      }
      if ((flag1*int1)&!flag2&!int2){// int1=1, int2=0
	  arr.Add((IlcTriggerIR*)IR); //add this IR
          continue;  
      }
      if ((int2*flag2)&!int1&!flag1){// int1=0, int2=1
	  arr.Add((IlcTriggerIR*)IR); //add this IR
          continue;  
      }      

    }//1
  
  return arr;
}
