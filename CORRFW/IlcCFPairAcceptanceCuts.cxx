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
//          ----   CORRECTION FRAMEWORK   ----
// Class to cut on the number of IlcTrackReference's 
// for each detector. Applies on pair of tracks (IlcCFPair)
///////////////////////////////////////////////////////////////////////////
// author : R. Vernet (renaud.vernet@cern.ch)
///////////////////////////////////////////////////////////////////////////

#include "IlcMCParticle.h"
#include "IlcCFPairAcceptanceCuts.h"
#include "IlcMCEvent.h"
#include "TBits.h"
#include "IlcLog.h"

ClassImp(IlcCFPairAcceptanceCuts)

//______________________________
IlcCFPairAcceptanceCuts::IlcCFPairAcceptanceCuts() : 
  IlcCFCutBase(),
  fMCInfo(0x0),
  fCutNeg(new IlcCFAcceptanceCuts()),
  fCutPos(new IlcCFAcceptanceCuts()),
  fBitmap(new TBits(0))
{
  //
  //Default Constructor
  //
}

//______________________________
IlcCFPairAcceptanceCuts::IlcCFPairAcceptanceCuts(const Char_t* name, const Char_t* title) : 
  IlcCFCutBase(name,title),
  fMCInfo(0x0),
  fCutNeg(new IlcCFAcceptanceCuts(name,title)),
  fCutPos(new IlcCFAcceptanceCuts(name,title)),
  fBitmap(new TBits(0))
{
  //
  //Named Constructor
  //
}

//______________________________
IlcCFPairAcceptanceCuts::IlcCFPairAcceptanceCuts(const IlcCFPairAcceptanceCuts& c) : 
  IlcCFCutBase(c),
  fMCInfo(c.fMCInfo),
  fCutNeg(c.fCutNeg),
  fCutPos(c.fCutPos),
  fBitmap(c.fBitmap)
{
  //
  //Copy Constructor
  //
}

//______________________________
IlcCFPairAcceptanceCuts& IlcCFPairAcceptanceCuts::operator=(const IlcCFPairAcceptanceCuts& c)
{
  //
  // Assignment operator
  //
  if (this != &c) {
    IlcCFCutBase::operator=(c) ;
    fMCInfo = c.fMCInfo ;
    fCutNeg = c.fCutNeg ;
    fCutPos = c.fCutPos ;
    fBitmap = c.fBitmap ;
  }
  return *this ;
}

//__________________________________________________________
Bool_t IlcCFPairAcceptanceCuts::IsSelected(TObject* obj) {
  //
  // checks the number of track references associated to 'obj'
  // 'obj' must be an IlcMCParticle
  //
  //
  // check if selections on 'obj' are passed
  // 'obj' must be an IlcMCParticle
  //
  
  SelectionBitMap(obj);

  //   if (fIsQAOn) FillHistograms(obj,kFALSE);
  Bool_t isSelected = kTRUE;

  for (UInt_t icut=0; icut<fBitmap->GetNbits(); icut++) {
    if (!fBitmap->TestBitNumber(icut)) {
      isSelected = kFALSE;
      break;
    }
  }  

  if (!isSelected) return kFALSE ;
  //   if (fIsQAOn) FillHistograms(obj,kTRUE);
  return kTRUE;
}

//__________________________________________________________
void IlcCFPairAcceptanceCuts::SelectionBitMap(TObject* obj) 
{
  //
  // test if the track passes the single cuts
  // and store the information in a bitmap
  //

  for (UInt_t i=0; i<kNCuts; i++) fBitmap->SetBitNumber(i,kFALSE);

  IlcMCParticle* mcpart = dynamic_cast<IlcMCParticle*>(obj) ;

  if (!mcpart) return;
  TString className(mcpart->ClassName());
  if (className.CompareTo("IlcMCParticle") != 0) {
    IlcError("obj must point to an IlcMCParticle !");
    return ;
  }

  TParticle* part = mcpart->Particle() ;
  if (!part || part->GetNDaughters() !=2) return ;

  Int_t lab0 = part->GetDaughter(0);
  Int_t lab1 = part->GetDaughter(1);
  IlcMCParticle* negDaughter =  (IlcMCParticle*) fMCInfo->GetTrack(lab0) ;
  IlcMCParticle* posDaughter =  (IlcMCParticle*) fMCInfo->GetTrack(lab1) ;

  Int_t iCutBit = 0;

  if (fCutNeg->IsSelected(negDaughter)) fBitmap->SetBitNumber(iCutBit,kTRUE);
  iCutBit++;
  
  if (fCutPos->IsSelected(posDaughter)) fBitmap->SetBitNumber(iCutBit,kTRUE);
}

//______________________________
void IlcCFPairAcceptanceCuts::SetMCEventInfo(const TObject* mcInfo) {
  //
  // Sets pointer to MC event information (IlcMCEvent)
  //

  if (!mcInfo) {
    Error("SetMCEventInfo","Pointer to MC Event is null !");
    return;
  }
  
  TString className(mcInfo->ClassName());
  if (className.CompareTo("IlcMCEvent") != 0) {
    Error("SetMCEventInfo","argument must point to an IlcMCEvent !");
    return ;
  }
  
  fMCInfo = (IlcMCEvent*) mcInfo ;
}
