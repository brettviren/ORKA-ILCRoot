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
//     Container class for ESD VZERO data
//     Author: Brigitte Cheynis & Cvetan Cheshkov
//-------------------------------------------------------------------------

#include "IlcESDVZERO.h"
#include "IlcLog.h"

ClassImp(IlcESDVZERO)

//__________________________________________________________________________
IlcESDVZERO::IlcESDVZERO()
  :IlcVVZERO(),
   fBBtriggerV0A(0),
   fBGtriggerV0A(0),
   fBBtriggerV0C(0),
   fBGtriggerV0C(0),
   fV0ATime(-1024),
   fV0CTime(-1024),
   fV0ATimeError(0),
   fV0CTimeError(0),
   fV0ADecision(kV0Invalid),
   fV0CDecision(kV0Invalid),
   fTriggerChargeA(0),
   fTriggerChargeC(0),
   fTriggerBits(0)
{   
   // Default constructor 
   for(Int_t j=0; j<64; j++){ 
      fMultiplicity[j] = 0.0;   
      fAdc[j]   = 0.0;   
      fTime[j]  = 0.0; 
      fWidth[j] = 0.0; 
      fBBFlag[j]= kFALSE;
      fBGFlag[j]= kFALSE;  
   }
}

//__________________________________________________________________________
IlcESDVZERO::IlcESDVZERO(const IlcESDVZERO &o)
  :IlcVVZERO(o),
   fBBtriggerV0A(o.fBBtriggerV0A),
   fBGtriggerV0A(o.fBGtriggerV0A),
   fBBtriggerV0C(o.fBBtriggerV0C),
   fBGtriggerV0C(o.fBGtriggerV0C),
   fV0ATime(o.fV0ATime),
   fV0CTime(o.fV0CTime),
   fV0ATimeError(o.fV0ATimeError),
   fV0CTimeError(o.fV0CTimeError),
   fV0ADecision(o.fV0ADecision),
   fV0CDecision(o.fV0CDecision),
   fTriggerChargeA(o.fTriggerChargeA),
   fTriggerChargeC(o.fTriggerChargeC),
   fTriggerBits(o.fTriggerBits)
{   
   // Default constructor 
   for(Int_t j=0; j<64; j++) {
       fMultiplicity[j] = o.fMultiplicity[j];
       fAdc[j]    = o.fAdc[j];
       fTime[j]   = o.fTime[j];
       fWidth[j]  = o.fWidth[j];
       fBBFlag[j] = o.fBBFlag[j];
       fBGFlag[j] = o.fBGFlag[j];
   }
}

//__________________________________________________________________________
IlcESDVZERO::IlcESDVZERO(UInt_t BBtriggerV0A, UInt_t BGtriggerV0A,
	      UInt_t BBtriggerV0C, UInt_t BGtriggerV0C,
	      Float_t *Multiplicity, Float_t *Adc, 
	      Float_t *Time, Float_t *Width, Bool_t *BBFlag, Bool_t *BGFlag)
  :IlcVVZERO(),
   fBBtriggerV0A(BBtriggerV0A),
   fBGtriggerV0A(BGtriggerV0A),
   fBBtriggerV0C(BBtriggerV0C),
   fBGtriggerV0C(BGtriggerV0C),
   fV0ATime(-1024),
   fV0CTime(-1024),
   fV0ATimeError(0),
   fV0CTimeError(0),
   fV0ADecision(kV0Invalid),
   fV0CDecision(kV0Invalid),
   fTriggerChargeA(0),
   fTriggerChargeC(0),
   fTriggerBits(0)
{
   // Constructor
   for(Int_t j=0; j<64; j++) {
       fMultiplicity[j] = Multiplicity[j];
       fAdc[j]    = Adc[j];
       fTime[j]   = Time[j];
       fWidth[j]  = Width[j];
       fBBFlag[j] = BBFlag[j];
       fBGFlag[j] = BGFlag[j];
   }
}

//__________________________________________________________________________
IlcESDVZERO& IlcESDVZERO::operator=(const IlcESDVZERO& o)
{

  if(this==&o) return *this;
  IlcVVZERO::operator=(o);
  // Assignment operator
  fBBtriggerV0A=o.fBBtriggerV0A;
  fBGtriggerV0A=o.fBGtriggerV0A;
  fBBtriggerV0C=o.fBBtriggerV0C;
  fBGtriggerV0C=o.fBGtriggerV0C;

  fV0ATime = o.fV0ATime;
  fV0CTime = o.fV0CTime;
  fV0ATimeError = o.fV0ATimeError;
  fV0CTimeError = o.fV0CTimeError;
  fV0ADecision = o.fV0ADecision;
  fV0CDecision = o.fV0CDecision;
  fTriggerChargeA = o.fTriggerChargeA;
  fTriggerChargeC = o.fTriggerChargeC;
  fTriggerBits = o.fTriggerBits;

   for(Int_t j=0; j<64; j++) {
       fMultiplicity[j] = o.fMultiplicity[j];
       fAdc[j]    = o.fAdc[j];
       fTime[j]   = o.fTime[j];
       fWidth[j]  = o.fWidth[j];
       fBBFlag[j] = o.fBBFlag[j];
       fBGFlag[j] = o.fBGFlag[j];
   }
  return *this;
}

//______________________________________________________________________________
void IlcESDVZERO::Copy(TObject &obj) const {
  
  // this overwrites the virtual TOBject::Copy()
  // to allow run time copying without casting
  // in IlcESDEvent

  if(this==&obj)return;
  IlcESDVZERO *robj = dynamic_cast<IlcESDVZERO*>(&obj);
  if(!robj)return; // not an IlcESDVZERO
  *robj = *this;

}

//__________________________________________________________________________
Short_t IlcESDVZERO::GetNbPMV0A() const
{
  // Returns the number of
  // fired PM in V0A
  Short_t n=0;
  for(Int_t i=32;i<64;i++) 
    if (fMultiplicity[i]>0) n++;
  return n;
}

//__________________________________________________________________________
Short_t IlcESDVZERO::GetNbPMV0C() const
{
  // Returns the number of
  // fired PM in V0C
  Short_t n=0;
  for(Int_t i=0;i<32;i++) 
    if (fMultiplicity[i]>0) n++;
  return n;
}

//__________________________________________________________________________
Float_t IlcESDVZERO::GetMTotV0A() const
{
  // returns total multiplicity
  // in V0A
  Float_t mul=0.0;
  for(Int_t i=32;i<64;i++) 
    mul+=  fMultiplicity[i];
  return mul;
}

//__________________________________________________________________________
Float_t IlcESDVZERO::GetMTotV0C() const
{
  // returns total multiplicity
  // in V0C
  Float_t mul=0.0;
  for(Int_t i=0;i<32;i++) 
    mul+=  fMultiplicity[i];
  return mul;
}

//__________________________________________________________________________
Float_t IlcESDVZERO::GetMRingV0A(Int_t ring) const
{ 
  // returns multiplicity in a
  // given ring of V0A
  if (OutOfRange(ring, "IlcESDVZERO:::GetMRingV0A",4)) return -1;
  Float_t mul =0.0;

  if (ring == 0) for(Int_t i=32;i<40;i++) mul +=  fMultiplicity[i];
  if (ring == 1) for(Int_t i=40;i<48;i++) mul +=  fMultiplicity[i];
  if (ring == 2) for(Int_t i=48;i<56;i++) mul +=  fMultiplicity[i];
  if (ring == 3) for(Int_t i=56;i<64;i++) mul +=  fMultiplicity[i];
  return mul ;
}

//__________________________________________________________________________
Float_t IlcESDVZERO::GetMRingV0C(Int_t ring) const
{ 
  // returns multiplicity in a
  // given ring of V0C
  if (OutOfRange(ring, "IlcESDVZERO:::GetMRingV0C",4)) return -1;
  Float_t mul =0.0;

  if (ring == 0) for(Int_t i=0;i<8;i++)   mul +=  fMultiplicity[i];
  if (ring == 1) for(Int_t i=8;i<16;i++)  mul +=  fMultiplicity[i];
  if (ring == 2) for(Int_t i=16;i<24;i++) mul +=  fMultiplicity[i];
  if (ring == 3) for(Int_t i=24;i<32;i++) mul +=  fMultiplicity[i];
  return mul ;
}

//__________________________________________________________________________
Float_t IlcESDVZERO::GetMultiplicity(Int_t i) const

{
  // returns multiplicity in a
  // given cell of V0
  if (OutOfRange(i, "IlcESDVZERO::GetMultiplicity:",64)) return -1;
  return fMultiplicity[i];
}

//__________________________________________________________________________
Float_t IlcESDVZERO::GetMultiplicityV0A(Int_t i) const

{
  // returns multiplicity in a
  // given cell of V0A
  if (OutOfRange(i, "IlcESDVZERO::GetMultiplicityV0A:",32)) return -1;
  return fMultiplicity[32+i];
}

//__________________________________________________________________________
Float_t IlcESDVZERO::GetMultiplicityV0C(Int_t i) const

{
  // returns multiplicity in a
  // given cell of V0C
  if (OutOfRange(i, "IlcESDVZERO::GetMultiplicityV0C:",32)) return -1;
  return fMultiplicity[i];
}

//__________________________________________________________________________
Float_t IlcESDVZERO::GetAdc(Int_t i) const

{
  // returns ADC charge in a
  // given cell of V0
  if (OutOfRange(i, "IlcESDVZERO::GetAdc:",64)) return -1;
  return fAdc[i];
}

//__________________________________________________________________________
Float_t IlcESDVZERO::GetAdcV0A(Int_t i) const

{
  // returns ADC charge in a
  // given cell of V0A
  if (OutOfRange(i, "IlcESDVZERO::GetAdcV0A:",32)) return -1;
  return fAdc[32+i];
}

//__________________________________________________________________________
Float_t IlcESDVZERO::GetAdcV0C(Int_t i) const

{
  // returns ADC charge in a
  // given cell of V0C
  if (OutOfRange(i, "IlcESDVZERO::GetAdcV0C:",32)) return -1;
  return fAdc[i];
}

//__________________________________________________________________________
Float_t IlcESDVZERO::GetTime(Int_t i) const

{
  // returns leading time measured by TDC
  // in a given cell of V0
  if (OutOfRange(i, "IlcESDVZERO::GetTime:",64)) return -1;
  return fTime[i];
}

//__________________________________________________________________________
Float_t IlcESDVZERO::GetTimeV0A(Int_t i) const

{
  // returns leading time measured by TDC
  // in a given cell of V0A
  if (OutOfRange(i, "IlcESDVZERO::GetTimeV0A:",32)) return -1;
  return fTime[32+i];
}

//__________________________________________________________________________
Float_t IlcESDVZERO::GetTimeV0C(Int_t i) const

{
  // returns leading time measured by TDC
  // in a given cell of V0C
  if (OutOfRange(i, "IlcESDVZERO::GetTimeV0C:",32)) return -1;
  return fTime[i];
}

//__________________________________________________________________________
Float_t IlcESDVZERO::GetWidth(Int_t i) const

{
  // returns time signal width
  // in a given cell of V0
  if (OutOfRange(i, "IlcESDVZERO::GetWidth:",64)) return -1;
  return fWidth[i];
}

//__________________________________________________________________________
Float_t IlcESDVZERO::GetWidthV0A(Int_t i) const

{
  // returns time signal width
  // in a given cell of V0A
  if (OutOfRange(i, "IlcESDVZERO::GetWidthV0A:",32)) return -1;
  return fWidth[32+i];
}

//__________________________________________________________________________
Float_t IlcESDVZERO::GetWidthV0C(Int_t i) const

{
  // returns time signal width
  // in a given cell of V0C
  if (OutOfRange(i, "IlcESDVZERO::GetWidthV0C:",32)) return -1;
  return fWidth[i];
}

//__________________________________________________________________________
Bool_t IlcESDVZERO::BBTriggerV0A(Int_t i) const
{
  // returns offline beam-beam flags in V0A
  // one bit per cell
  if (OutOfRange(i, "IlcESDVZERO:::BBTriggerV0A",32)) return kFALSE;
  UInt_t test = 1;
  return ( fBBtriggerV0A & (test << i) ? kTRUE : kFALSE );
}

//__________________________________________________________________________
Bool_t IlcESDVZERO::BGTriggerV0A(Int_t i) const
{
  // returns offline beam-gas flags in V0A
  // one bit per cell
  if (OutOfRange(i, "IlcESDVZERO:::BGTriggerV0A",32)) return kFALSE;
  UInt_t test = 1;
  return ( fBGtriggerV0A & (test << i) ? kTRUE : kFALSE );
}

//__________________________________________________________________________
Bool_t IlcESDVZERO::BBTriggerV0C(Int_t i) const
{
  // returns offline beam-beam flags in V0C
  // one bit per cell
  if (OutOfRange(i, "IlcESDVZERO:::BBTriggerV0C",32)) return kFALSE;
  UInt_t test = 1;
  return ( fBBtriggerV0C & (test << i) ? kTRUE : kFALSE );
}

//__________________________________________________________________________
Bool_t IlcESDVZERO::BGTriggerV0C(Int_t i) const
{
  // returns offline beam-gasflags in V0C
  // one bit per cell
  if (OutOfRange(i, "IlcESDVZERO:::BGTriggerV0C",32)) return kFALSE;
  UInt_t test = 1;
  return ( fBGtriggerV0C & (test << i) ? kTRUE : kFALSE );
}

//__________________________________________________________________________
Bool_t IlcESDVZERO::GetBBFlag(Int_t i) const

{
  // returns online beam-beam flag in V0
  // one boolean per cell
  if (OutOfRange(i, "IlcESDVZERO::GetBBFlag:",64)) return kFALSE;
  return fBBFlag[i];
}

//__________________________________________________________________________
Bool_t IlcESDVZERO::GetBGFlag(Int_t i) const

{
  // returns online beam-gas flag in V0
  // one boolean per cell
  if (OutOfRange(i, "IlcESDVZERO::GetBGFlag:",64)) return kFALSE;
  return fBGFlag[i];
}
