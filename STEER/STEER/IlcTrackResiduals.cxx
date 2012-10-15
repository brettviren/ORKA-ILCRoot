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

//-----------------------------------------------------------------
//   Implementation of the base class for track residuals
//
//
//-----------------------------------------------------------------

#include "IlcTrackResiduals.h"

#include "IlcAlignObj.h"
#include "IlcAlignObjParams.h"
#include "IlcTrackPointArray.h"

ClassImp(IlcTrackResiduals)

//_____________________________________________________________________________
IlcTrackResiduals::IlcTrackResiduals():
  fN(0),
  fLast(0),
  fIlcgnObj(0),
  fVolArray(0),
  fTrackArray(0),
  fChi2(0),
  fNdf(0),
  fMinNPoints(0),
  fIsOwner(kTRUE)
{
  // Default constructor
  for (Int_t ipar=0; ipar<6; ipar++){
    fBFixed[ipar] = kFALSE;
    fFixed[ipar]  = 0.;
  }  
}

//_____________________________________________________________________________
IlcTrackResiduals::IlcTrackResiduals(Int_t ntracks):
  fN(ntracks),
  fLast(0),
  fIlcgnObj(0),
  fVolArray(0),
  fTrackArray(0),
  fChi2(0),
  fNdf(0),
  fMinNPoints(0),
  fIsOwner(kTRUE)
{
  // Constructor
  if (ntracks > 0) {
    fVolArray = new IlcTrackPointArray*[ntracks];
    fTrackArray = new IlcTrackPointArray*[ntracks];
    for (Int_t itrack = 0; itrack < ntracks; itrack++)
      fVolArray[itrack] = fTrackArray[itrack] = 0x0;
  }

  for (Int_t ipar=0; ipar<6; ipar++){
    fBFixed[ipar] = kFALSE;
    fFixed[ipar]  = 0.;
  }  
}

//_____________________________________________________________________________
IlcTrackResiduals::IlcTrackResiduals(const IlcTrackResiduals &res):
  TObject(res),
  fN(res.fN),
  fLast(res.fLast),
  fIlcgnObj(0),
  fVolArray(new IlcTrackPointArray*[fN]),
  fTrackArray(new IlcTrackPointArray*[fN]),
  fChi2(res.fChi2),
  fNdf(res.fNdf),
  fMinNPoints(res.fMinNPoints),
  fIsOwner(kTRUE)
{
  // Copy constructor
  // By default the created copy owns the track point arrays

  if(res.fIlcgnObj) fIlcgnObj = (IlcAlignObj *)res.fIlcgnObj->Clone();

  memset(fVolArray,0,sizeof(IlcTrackPointArray*)*fN);
  memset(fTrackArray,0,sizeof(IlcTrackPointArray*)*fN);

  for (Int_t itrack = 0; itrack < fN; itrack++) {
    if (res.fVolArray[itrack])
      fVolArray[itrack] = new IlcTrackPointArray(*res.fVolArray[itrack]);
    if (res.fTrackArray[itrack])
      fTrackArray[itrack] = new IlcTrackPointArray(*res.fTrackArray[itrack]);
  }

  memcpy(fBFixed,res.fBFixed,sizeof(Bool_t)*6);
  memcpy(fFixed,res.fFixed,sizeof(Float_t)*6);

}

//_____________________________________________________________________________
IlcTrackResiduals &IlcTrackResiduals::operator =(const IlcTrackResiduals& res)
{
  // assignment operator
  // Does not copy the track point arrays
  if(this!=&res) {
    TObject::operator=(res);
    
    fLast = res.fLast;
    delete fIlcgnObj;
    if(res.fIlcgnObj) 
      fIlcgnObj = (IlcAlignObj *)res.fIlcgnObj->Clone();
    else
      fIlcgnObj = 0;
      
    if (fIsOwner) {
      if (fVolArray) {
	for (Int_t itrack = 0; itrack < fN; itrack++) 
	  delete fVolArray[itrack];
	delete [] fVolArray;
	fVolArray=0;
      }
      if(res.fN) {
	fVolArray = new IlcTrackPointArray*[res.fN];
	memset(fVolArray,0,sizeof(IlcTrackPointArray*)*res.fN);
	for (Int_t itrack = 0; itrack < res.fN; itrack++) 
	  if (res.fVolArray[itrack])
	    fVolArray[itrack] = new IlcTrackPointArray(*res.fVolArray[itrack]);
      }
      if (fTrackArray) {
	for (Int_t itrack = 0; itrack < fN; itrack++) 
	  delete fTrackArray[itrack];
	delete [] fTrackArray;
      }
      if(res.fN) {
	fTrackArray = new IlcTrackPointArray*[res.fN];
	memset(fTrackArray,0,sizeof(IlcTrackPointArray*)*res.fN);
	for (Int_t itrack = 0; itrack < res.fN; itrack++) 
	  if (res.fTrackArray[itrack])
	    fTrackArray[itrack] = new IlcTrackPointArray(*res.fTrackArray[itrack]);
      }
    } else {
      fVolArray = res.fVolArray;
      fTrackArray = res.fTrackArray;
    }
    fN = res.fN;
    fChi2 = res.fChi2;
    fNdf  = res.fNdf;
    fMinNPoints = res.fMinNPoints;
    fIsOwner = kFALSE;

    memcpy(fBFixed,res.fBFixed,sizeof(Bool_t)*6);
    memcpy(fFixed,res.fFixed,sizeof(Float_t)*6);
  }
  return *this;
}

//_____________________________________________________________________________
IlcTrackResiduals::~IlcTrackResiduals()
{
  // Destructor
  delete fIlcgnObj;
  DeleteTrackPointArrays();
}

//_____________________________________________________________________________
void IlcTrackResiduals::SetNTracks(Int_t ntracks)
{
  // Set new size for the track point arrays.
  // Delete the old arrays and allocate the
  // new ones.
  DeleteTrackPointArrays();

  fN = ntracks;
  fLast = 0;
  fChi2 = 0;
  fNdf  = 0;
  fIsOwner = kTRUE;

  if (ntracks > 0) {
    fVolArray = new IlcTrackPointArray*[ntracks];
    fTrackArray = new IlcTrackPointArray*[ntracks];
    for (Int_t itrack = 0; itrack < ntracks; itrack++)
      fVolArray[itrack] = fTrackArray[itrack] = 0x0;
  }
  else {
    fVolArray = fTrackArray = 0x0;
  }
}

//_____________________________________________________________________________
Bool_t IlcTrackResiduals::AddTrackPointArrays(IlcTrackPointArray *volarray, IlcTrackPointArray *trackarray)
{
  // Adds pair of track space point and
  // track extrapolation point arrays
  if (!fVolArray || !fTrackArray) return kFALSE;

  if (!volarray || !trackarray) return kFALSE;

  if (volarray->GetNPoints() < fMinNPoints) return kFALSE;

  if (fLast >= fN) return kFALSE;

  fVolArray[fLast] = volarray;
  fTrackArray[fLast] = trackarray;
  fLast++;

  return kTRUE;
}

//_____________________________________________________________________________
void IlcTrackResiduals::InitAlignObj()
{
  // Create the alignment object 
  // to be updated
  delete fIlcgnObj;
  fIlcgnObj = new IlcAlignObjParams;
}


//_____________________________________________________________________________
Bool_t IlcTrackResiduals::GetTrackPointArrays(Int_t i, IlcTrackPointArray* &volarray, IlcTrackPointArray* &trackarray) const
{
  // Provide an access to a pair of track point arrays
  // with given index
  if (i >= fLast) {
    volarray = trackarray = 0x0;
    return kFALSE;
  }
  else {
    volarray = fVolArray[i];
    trackarray = fTrackArray[i];
    return kTRUE;
  }
}

//_____________________________________________________________________________
void IlcTrackResiduals::DeleteTrackPointArrays()
{
  // Deletes the track point arrays only in case
  // the object is their owner.
  // Called by the destructor and SetNTracks methods.
  if (fIsOwner) {
    if (fVolArray) {
      for (Int_t itrack = 0; itrack < fN; itrack++) 
	delete fVolArray[itrack];
      delete [] fVolArray;
    }
    if (fTrackArray) {
      for (Int_t itrack = 0; itrack < fN; itrack++) 
	delete fTrackArray[itrack];
      delete [] fTrackArray;
    }
  }
}

//_____________________________________________________
Int_t IlcTrackResiduals::GetNFreeParam(){ 
  Int_t unfixedparam=6;
  for(Int_t j=0;j<6;j++){
    if(fBFixed[j]==kTRUE)unfixedparam--;
  }
  return unfixedparam;
}
