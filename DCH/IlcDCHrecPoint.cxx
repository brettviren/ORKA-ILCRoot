/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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

/* $Id: IlcDCHrecPoint.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH reconstructed point                                                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcRun.h"

#include "IlcDCHgeometry.h"
#include "IlcDCHrecPoint.h"

ClassImp(IlcDCHrecPoint)

//_____________________________________________________________________________
IlcDCHrecPoint::IlcDCHrecPoint():IlcRecPoint()
{
  //
  // Standard constructor
  //

  fDetector = 0;

  fGeom = IlcDCHgeometry::GetGeometry();

}

//_____________________________________________________________________________
IlcDCHrecPoint::IlcDCHrecPoint(const char * opt):IlcRecPoint(opt)
{
  //
  // Standard constructor
  //

  fDetector = 0;

  fGeom = IlcDCHgeometry::GetGeometry();

}

//_____________________________________________________________________________
IlcDCHrecPoint::~IlcDCHrecPoint()
{
  //
  // IlcDCHrecPoint destructor
  //

}

//_____________________________________________________________________________
void IlcDCHrecPoint::AddDigit(Int_t digit)
{
  //
  // Adds the index of a digit to the digits list
  //

  // First resize the list 
  // (no clusters with more than 3 digits for the DCH
  if ((fMulDigit == 0) && (fMaxDigit >= 5)) {
    fMaxDigit = 5;
    delete fDigitsList;
    fDigitsList = new int[fMaxDigit];
  }

  // Increase the size of the list if necessary
  if (fMulDigit >= fMaxDigit) { 
    fMaxDigit *= 2;
    Int_t *tempo = new Int_t[fMaxDigit]; 
    Int_t index; 
    for (index = 0; index < fMulDigit; index++)
      tempo[index] = fDigitsList[index]; 
    delete fDigitsList; 
    fDigitsList = tempo; 
  }
  
  fDigitsList[fMulDigit++] = digit;

}

//_____________________________________________________________________________
void IlcDCHrecPoint::SetLocalPosition(TVector3 & /*pos*/)
{
  //
  // Sets the position of the point in the local coordinate system
  // (row,col,time) and calculates the error matrix in the same
  // system.
  //

  //const Float_t kSq12 = 3.464101615;

  // Set the position
  //fLocPos = pos;

  // Set the error matrix
  // row:  pad-size / sqrt(12)
  // col:  not defined yet
  // time: bin-size / sqrt(12)
  //Int_t plane   = ((IlcDCHgeometry *) fGeom)->GetPlane(fDetector);
  //Int_t chamber = ((IlcDCHgeometry *) fGeom)->GetChamber(fDetector);
  //Int_t sector  = ((IlcDCHgeometry *) fGeom)->GetSector(fDetector);
  //fLocPosM->operator()(0,0) = ((IlcDCHgeometry *) fGeom)->GetRowPadSize(plane
  //                                                                     ,chamber
  //                                                                     ,sector) 
  //                          / kSq12;
  //fLocPosM->operator()(1,1) = 0.0;
  //fLocPosM->operator()(2,2) = ((IlcDCHgeometry *) fGeom)->GetTimeBinSize() 
  //                          / kSq12;

  //  printf("rec. point: row = %f, col = %f, time = %f \n",
  //           fLocPos[0],fLocPos[1],fLocPos[2]); 

}

//_____________________________________________________________________________
void IlcDCHrecPoint::SetTrackingYZ(Float_t /*sigmaY*/, Float_t /*sigmaZ*/)
{
 //
 // Sets the position of the point in the local coordinate system
 // of tracking sector
 //

  //Int_t plane = ((IlcDCHgeometry *) fGeom)->GetPlane(fDetector);
  //Int_t chamber = ((IlcDCHgeometry *) fGeom)->GetChamber(fDetector);
  //Int_t sector = ((IlcDCHgeometry *) fGeom)->GetSector(fDetector);


 // Set the position

  //Float_t   padRow    = fLocPos[0];             // Pad Row position
  //Float_t   padCol    = fLocPos[1];             // Pad Column position

  //Float_t   col0 = ((IlcDCHgeometry *) fGeom)->GetCol0(plane);
  //Float_t   row0 = ((IlcDCHgeometry *) fGeom)->GetRow0(plane,chamber,sector);

  //  Float_t   offset = 0.5 * ((IlcDCHgeometry *) fGeom)->GetChamberWidth(plane);

  //fY = - (col0 + padCol * ((IlcDCHgeometry *) fGeom)->GetColPadSize(plane));
  //fZ =    row0 + padRow * ((IlcDCHgeometry *) fGeom)->GetRowPadSize(plane
  //                                                                   ,chamber
  //                                                                 ,sector);

  //  fSigmaY = sigmaY * sigmaY;
  //  fSigmaZ = sigmaZ * sigmaZ;

//fSigmaY2 = 0.05 * 0.05;

//fSigmaZ2 = ((IlcDCHgeometry *) fGeom)->GetRowPadSize(plane,chamber,sector)
//         * ((IlcDCHgeometry *) fGeom)->GetRowPadSize(plane,chamber,sector) 
//         / 12.;

}                                    

//_____________________________________________________________________________
void IlcDCHrecPoint::AddTrackIndex(Int_t *track)
{
 // Adds track index. Currently assumed that track is an array of
 // size 9, and up to 3 track indexes are stored in fTracks[3].
 // Indexes are sorted according to:
 //  1) index of max number of appearances is stored first
 //  2) if two or more indexes appear equal number of times, the lowest
 //     ones are stored first;

  const Int_t kSize = 9;

  Int_t entries[kSize][2], i, j, index;

  Bool_t indexAdded;

  for (i=0; i<kSize; i++) {
    entries[i][0]=-1;
    entries[i][1]=0;
  }


  for (Int_t k=0; k<kSize; k++) {
    index=track[k];
    indexAdded=kFALSE; j=0;
    if (index >= 0) {
      while ( (!indexAdded) && ( j < kSize ) ) {
        if ((entries[j][0]==index) || (entries[j][1]==0)) {
          entries[j][0]=index;
          entries[j][1]=entries[j][1]+1;
          indexAdded=kTRUE;
        }
        j++;
      }
    }
  }                   

  // sort by number of appearances and index value
  Int_t swap=1, tmp0, tmp1;
  while ( swap > 0) {
    swap=0;
    for(i=0; i<(kSize-1); i++) {
      if ((entries[i][0] >= 0) && (entries[i+1][0] >= 0)) {
        if ((entries[i][1] < entries[i+1][1]) ||
            ((entries[i][1] == entries[i+1][1]) &&
             (entries[i][0] > entries[i+1][0]))) {
               tmp0=entries[i][0];
               tmp1=entries[i][1];
               entries[i][0]=entries[i+1][0];
               entries[i][1]=entries[i+1][1];
               entries[i+1][0]=tmp0;
               entries[i+1][1]=tmp1;
               swap++;
        }
      }
    }
  }

  // set track indexes

  for(i=0; i<3; i++) {
    fTracks[i] = entries[i][0];
  }

  return;

}                    







