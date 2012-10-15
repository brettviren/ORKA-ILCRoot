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

/* $Id: IlcRecPoint.cxx 53922 2012-01-16 09:19:28Z fca $ */

//-*-C++-*-
//_________________________________________________________________________
// Base Class for reconstructed space points 
// usually coming from the clusterisation algorithms
// run on the digits
//
//*-- Author : Yves Schutz  SUBATECH 
//////////////////////////////////////////////////////////////////////////////


// --- ROOT system ---

// --- Standard library ---

// --- IlcRoot header files ---

#include "IlcRecPoint.h"
#include "IlcGeometry.h"
#include "IlcDigitNew.h"

ClassImp(IlcRecPoint)


//_______________________________________________________________________
IlcRecPoint::IlcRecPoint():
  fAmp(0),
  fGeom(0),
  fIndexInList(-1), // to be set when the point is already stored
  fLocPos(0,0,0),
  fLocPosM(0),
  fMaxDigit(100),
  fMulDigit(0),
  fMaxTrack(5),
  fMulTrack(0),
  fDigitsList(0),
  fTracksList(0)
{
  //
  // default ctor  
  //
}

//_______________________________________________________________________
IlcRecPoint::IlcRecPoint(const char * ):
  fAmp(0),
  fGeom(0),
  fIndexInList(-1), // to be set when the point is already stored
  fLocPos(0,0,0),
  fLocPosM(new TMatrixF(3,3)),
  fMaxDigit(100),
  fMulDigit(0),
  fMaxTrack(5),
  fMulTrack(0),
  fDigitsList(new int[fMaxDigit]),
  fTracksList(new int[fMaxTrack])
{
  //
  // Standard ctor  
  //
}

//_______________________________________________________________________
IlcRecPoint::IlcRecPoint(const IlcRecPoint& recp):
  TObject(recp),
  fAmp(recp.fAmp),
  fGeom(recp.fGeom),
  fIndexInList(-1), // to be set when the point is already stored
  fLocPos(recp.fLocPos),
  fLocPosM(new TMatrixF(*(recp.fLocPosM))),
  fMaxDigit(recp.fMaxDigit),
  fMulDigit(recp.fMulDigit),
  fMaxTrack(recp.fMaxTrack),
  fMulTrack(recp.fMulTrack),
  fDigitsList(new Int_t[fMulDigit]),
  fTracksList(new Int_t[fMulTrack])
{
  //
  // Copy constructor
  //
  memcpy(fDigitsList,recp.fDigitsList,sizeof(Int_t)*fMulDigit);
  memcpy(fTracksList,recp.fTracksList,sizeof(Int_t)*fMulTrack);
}

//_______________________________________________________________________
IlcRecPoint& IlcRecPoint::operator=(const IlcRecPoint& recp)
{
  //
  // Assignement constructor
  //
  if(this!=&recp) {
    TObject::operator=(recp);
    fAmp=recp.fAmp;
    fGeom=recp.fGeom;
    fIndexInList=-1; // to be set when the point is already stored
    fLocPos=recp.fLocPos;
    delete fLocPosM;
    fLocPosM=recp.fLocPosM;
    fMaxDigit=recp.fMaxDigit;
    fMulDigit=recp.fMulDigit;
    fMaxTrack=recp.fMaxTrack;
    fMulTrack=recp.fMulTrack;
    delete [] fDigitsList;
    fDigitsList=new Int_t[fMulDigit];
    delete [] fTracksList;
    fTracksList=new Int_t[fMulTrack];

    memcpy(fDigitsList,recp.fDigitsList,sizeof(Int_t)*fMulDigit);
    memcpy(fTracksList,recp.fTracksList,sizeof(Int_t)*fMulTrack);
  }
  return *this;
}

//_______________________________________________________________________
IlcRecPoint::~IlcRecPoint()
{
  // dtor
  
  delete fLocPosM ; 
  delete [] fDigitsList ; 
  delete [] fTracksList ;  
  
}
  
//_______________________________________________________________________
void IlcRecPoint::AddDigit(IlcDigitNew & digit)
{
  // adds a digit to the digits list
  // and accumulates the total amplitude and the multiplicity 
  
  
  if ( fMulDigit >= fMaxDigit ) { // increase the size of the list 
    int * tempo = new int[fMaxDigit*2]; 
    
    Int_t index ; 
    
    for ( index = 0 ; index < fMulDigit ; index++ )
      tempo[index] = fDigitsList[index] ; 
    
    delete [] fDigitsList ; 
    fDigitsList = tempo ; 
  }
  
  fDigitsList[fMulDigit] = digit.GetIndexInList()  ; 
  fMulDigit++ ; 
  fAmp += digit.GetAmp() ; 
}

//_______________________________________________________________________
// void IlcRecPoint::AddTrack(IlcTrack & track)
// {
//   // adds a digit to the digits list
//   // and accumulates the total amplitude and the multiplicity 


//   if ( fMulTrack >= fMaxTrack ) { // increase the size of the list 
//     int * tempo = new int[fMaxTrack*=2] ; 
//     Int_t index ; 
//     for ( index = 0 ; index < fMulTrack ; index++ )
//       tempo[index] = fTracksList[index] ; 
//     delete fTracksList ; 
//     fTracksList = tempo ; 
//   }

//   fTracksList[fMulTrack++]=  (int) &Track  ; 
// }

//_______________________________________________________________________
void IlcRecPoint::Copy(TObject& recp) const
{
  //
  // Copy *this onto pts
  //
  // Copy all first
  if((TObject*)this != &recp) {
    ((TObject*) this)->Copy(recp);
    (dynamic_cast<IlcRecPoint&>(recp)).fAmp = fAmp;
    (dynamic_cast<IlcRecPoint&>(recp)).fGeom = fGeom;
    (dynamic_cast<IlcRecPoint&>(recp)).fIndexInList = fIndexInList;
    (dynamic_cast<IlcRecPoint&>(recp)).fLocPos = fLocPos;
    (dynamic_cast<IlcRecPoint&>(recp)).fLocPosM = new TMatrixF(*fLocPosM);
    (dynamic_cast<IlcRecPoint&>(recp)).fMaxDigit = fMaxDigit;
    (dynamic_cast<IlcRecPoint&>(recp)).fMulDigit = fMulDigit;
    (dynamic_cast<IlcRecPoint&>(recp)).fMaxTrack = fMaxTrack;
    (dynamic_cast<IlcRecPoint&>(recp)).fMulTrack = fMulTrack;
    
    // Duplicate pointed objects
    (dynamic_cast<IlcRecPoint&>(recp)).fDigitsList = new Int_t[fMulDigit];
    memcpy((dynamic_cast<IlcRecPoint&>(recp)).fDigitsList,fDigitsList,fMulDigit*sizeof(Int_t));
    (dynamic_cast<IlcRecPoint&>(recp)).fTracksList = new Int_t[fMulTrack];
    memcpy((dynamic_cast<IlcRecPoint&>(recp)).fTracksList,fTracksList,fMulTrack*sizeof(Int_t));
  }
}

//_______________________________________________________________________
void IlcRecPoint::GetCovarianceMatrix(TMatrixF & mat) const
{
  // returns the covariant matrix for the local position
  
  mat = *fLocPosM ; 

}

//____________________________________________________________________________
void IlcRecPoint::GetLocalPosition(TVector3 & pos) const
{
  // returns the position of the cluster in the local reference system of the sub-detector

  pos = fLocPos;

 
}

//____________________________________________________________________________
void IlcRecPoint::GetGlobalPosition(TVector3 & gpos, TMatrixF & gmat) const
{
  // returns the position of the cluster in the global reference system of ILC
  // and the uncertainty on this position
  

  fGeom->GetGlobal(this, gpos, gmat) ;
 
}


