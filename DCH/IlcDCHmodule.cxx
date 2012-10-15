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

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                                                           //
//  DCH 6-chambers stack                                                     //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCHgeometry.h"
#include "IlcDCHmodule.h"
#include "IlcDCHltuTracklet.h"
#include "IlcDCHgtuTrack.h"
#include "IlcDCHtrigParam.h"
#include "IlcDCHzmaps.h"

ClassImp(IlcDCHmodule)

//_____________________________________________________________________________
IlcDCHmodule::IlcDCHmodule(IlcDCHtrigParam *trigp) 
{

  //
  // IlcDCHmodule default constructor
  //

  fDeltaY     = trigp->GetDeltaY();
  fDeltaS     = trigp->GetDeltaS();
  fXprojPlane = trigp->GetXprojPlane();
  fField      = trigp->GetField();
  fLTUtrk     = 0;
  fGTUtrk     = 0;
  fTracklets  = new TObjArray(400);
  fTracks     = new TObjArray(400);

  for (Int_t iPlan = 0; iPlan < IlcDCHgeometry::Nplan(); iPlan++) {
    for (Int_t i = 0; i < kNsubZchan; i++) {
      fZnchan[iPlan][i] = 0;
      for (Int_t j = 0; j < kNmaxZchan; j++) {
        fZtrkid[iPlan][j][i] = -1;
      }
    }
  }

}

//_____________________________________________________________________________
IlcDCHmodule::~IlcDCHmodule()
{
  //
  // destructor
  //

}

//_____________________________________________________________________________
IlcDCHmodule &IlcDCHmodule::operator=(const IlcDCHmodule &m)
{
  //
  // assignment operator
  //

  if (this != &m) ((IlcDCHmodule &) m).Copy(*this); 
  return *this;

}

//_____________________________________________________________________________
void IlcDCHmodule::Copy(TObject &m) const
{
  //
  // copy function
  //

  ((IlcDCHmodule &) m).fXprojPlane  = fXprojPlane;
  ((IlcDCHmodule &) m).fField       = fField;
  ((IlcDCHmodule &) m).fTracklets   = NULL;
  ((IlcDCHmodule &) m).fTracks      = NULL;
  ((IlcDCHmodule &) m).fDeltaY      = fDeltaY;
  ((IlcDCHmodule &) m).fDeltaS      = fDeltaS;
  ((IlcDCHmodule &) m).fLTUtrk      = NULL;
  ((IlcDCHmodule &) m).fGTUtrk      = NULL;

  for (Int_t iPlan = 0; iPlan < IlcDCHgeometry::Nplan(); iPlan++) {
    for (Int_t i = 0; i < kNsubZchan; i++) {
      ((IlcDCHmodule &) m).fZnchan[iPlan][i] = 0;
      for (Int_t j = 0; j < kNmaxZchan; j++) {
        ((IlcDCHmodule &) m).fZtrkid[iPlan][j][i] = -1;
      }
    }
  }

}

//_____________________________________________________________________________
void IlcDCHmodule::Reset() 
{
  //
  // Reset the tracks and tracklets in the module
  //

  ResetTracklets();
  ResetTracks();

  fLTUtrk     = 0;
  fGTUtrk     = 0;
  fTracklets  = new TObjArray(400);
  fTracks     = new TObjArray(400);

}

//_____________________________________________________________________________
void IlcDCHmodule::ResetTracks() 
{
  // 
  // Reset the tracks in the module
  //

  if (fTracks) {

    IlcDCHgtuTrack *trk;
    for (Int_t i = 0; i < GetNtracks(); i++) {
      
      trk = GetTrack(i);
      trk->Reset();
      
    }

    fTracks->Delete();

  }

}

//_____________________________________________________________________________
IlcDCHgtuTrack* IlcDCHmodule::GetTrack(Int_t pos) const
{
  //
  // Return track at position "pos"
  //

  if (fTracks == 0) return 0;
  void * trk = fTracks->UncheckedAt(pos);
  if (trk == 0) return 0;

  return (IlcDCHgtuTrack*)trk;

}

//_____________________________________________________________________________
void IlcDCHmodule::RemoveTrack(Int_t pos)
{
  //
  // Remove the track at position "pos"
  //

  if (fTracks == 0) return;
  fTracks->RemoveAt(pos);
  fTracks->Compress();

}

//_____________________________________________________________________________
void IlcDCHmodule::AddTracklet(Int_t det, 
			       Int_t row, 
			       Float_t rowz,
			       Float_t slope, 
			       Float_t offset, 
			       Float_t time, 
			       Int_t ncl,
			       Int_t label,
			       Float_t q) 
{
  // 
  // Add a tracklet to this track
  //
  
  fLTUtrk = new IlcDCHltuTracklet(det,row,rowz,slope,offset,time,ncl,label,q);
  
  Tracklets()->Add(fLTUtrk);

}

//_____________________________________________________________________________
IlcDCHltuTracklet* IlcDCHmodule::GetTracklet(Int_t pos) const
{
  //
  // Get the tracklet at position "pos"
  //

  if (fTracklets == 0) return 0;
  void * trk = fTracklets->UncheckedAt(pos);
  if (trk == 0) return 0;

  return (IlcDCHltuTracklet*)trk;

}

//_____________________________________________________________________________
void IlcDCHmodule::RemoveTracklet(Int_t pos)
{
  //
  // Remove the tracklet at position "pos"
  //

  if (fTracklets == 0) return;
  fTracklets->RemoveAt(pos);
  fTracklets->Compress();

}

//_____________________________________________________________________________
void IlcDCHmodule::RemoveMultipleTracklets()
{
  //
  // Remove multiple found tracklets
  //

  Float_t offDiffMin = 0.5;  // [cm]

  IlcDCHltuTracklet *trk;
  Int_t det1, det2, row1, row2, ncl1, ncl2, label1, label2;
  Float_t off1, off2;
  Int_t itrk = 0;
  while (itrk < (GetNtracklets()-1)) {

    trk = GetTracklet(itrk  );

    det1 = trk->GetDetector();
    row1 = trk->GetRow();
    off1 = trk->GetOffset();
    ncl1 = trk->GetNclusters();
    label1 = trk->GetLabel();

    trk = GetTracklet(itrk+1);

    det2 = trk->GetDetector();
    row2 = trk->GetRow();
    off2 = trk->GetOffset();
    ncl2 = trk->GetNclusters();
    label2 = trk->GetLabel();

    if (det1 == det2 && row1 == row2) {
      if ((off2-off1) < offDiffMin) {
	if (ncl1 < ncl2) {
	  RemoveTracklet(itrk  );
	} else {
	  RemoveTracklet(itrk+1);
	}
      }
    }

    itrk++;

  }

}

//_____________________________________________________________________________
void IlcDCHmodule::SortZ(Int_t cha)
{
  //
  // Match tracklets in the x-z plane (pad row sorting)
  //

  InitZLUT();

  IlcDCHltuTracklet *trk;
  Int_t row, pla, det;
  for (Int_t iTrk = 0; iTrk < GetNtracklets(); iTrk++) {

    trk = GetTracklet(iTrk);

    row = trk->GetRow();
    det = trk->GetDetector();
    pla = trk->GetPlane(det);

    for (Int_t iZchan = 0; iZchan < kNsubZchan; iZchan++) {
      if (fZChannelMap[cha][iZchan][pla][row] == 1) {
	fZtrkid[pla][fZnchan[pla][iZchan]][iZchan] = iTrk;
	fZnchan[pla][iZchan]++;
      }
    }

  }

}

//_____________________________________________________________________________
void IlcDCHmodule::InitZLUT()
{
  //
  // Initialize the pad row sorting look-up-table
  //

  for (Int_t iPlan = 0; iPlan < IlcDCHgeometry::Nplan(); iPlan++) {
    for (Int_t i = 0; i < kNsubZchan; i++) {
      fZnchan[iPlan][i] = 0;
      for (Int_t j = 0; j < kNmaxZchan; j++) {
        fZtrkid[iPlan][j][i] = -1;
      }
    }
  }

}

//_____________________________________________________________________________
void IlcDCHmodule::FindTracks() 
{
  //
  // Find tracks from tracklets
  //

  for (Int_t iZchan = 0; iZchan < kNsubZchan; iZchan++) {
    FindTracksCombi(iZchan);
  }

}

//_____________________________________________________________________________
void IlcDCHmodule::FindTracksCombi(Int_t zchan) 
{
  //
  // find tracks by pure combinatorics...
  //
  
  static Int_t trkTrack[12];
  
  Int_t nTracklets, nPlanes;
  Int_t ntrk1, trkId1, ntrk2, trkId2;
  Float_t y1, y1min, y1max, s1, z1, s1min, s1max, y2, s2, z2;
  IlcDCHltuTracklet *trk1;
  IlcDCHltuTracklet *trk2;
  IlcDCHltuTracklet *trk ;

  Bool_t isPlane[kNplan];

  for (Int_t iPlan1 = 0; iPlan1 < kNplan; iPlan1++) {

    ntrk1 = fZnchan[iPlan1][zchan];

    for (Int_t iTrk1 = 0; iTrk1 < ntrk1; iTrk1++) {

      for (Int_t iPlan = 0; iPlan < kNplan; iPlan++) isPlane[iPlan] = kFALSE;

      trkId1 = fZtrkid[iPlan1][iTrk1][zchan];

      nTracklets = 0;
      for (Int_t iList = 0; iList < kNmaxTrk; iList++) {
	trkTrack[iList] = -1;
      }

      trkTrack[nTracklets++] = trkId1;

      isPlane[iPlan1] = kTRUE;

      trk1 = GetTracklet(trkId1);

      y1    = trk1->GetYproj(fXprojPlane);
      y1min = y1 - fDeltaY;
      y1max = y1 + fDeltaY;
      s1    = trk1->GetSlope();
      s1min = s1 - fDeltaS;
      s1max = s1 + fDeltaS;
      z1    = trk1->GetZproj(fXprojPlane);      

      for (Int_t iPlan2 = 0; iPlan2 < kNplan; iPlan2++) {

	if (iPlan2 == iPlan1) continue;

	ntrk2 = fZnchan[iPlan2][zchan];

	for (Int_t iTrk2 = 0; iTrk2 < ntrk2; iTrk2++) {

	  trkId2 = fZtrkid[iPlan2][iTrk2][zchan];

	  if (trkId2 == trkId1) continue;

	  trk2 = GetTracklet(trkId2);

	  y2 = trk2->GetYproj(fXprojPlane);
	  s2 = trk2->GetSlope();
	  z2 = trk2->GetZproj(fXprojPlane);

	  if ((y1min < y2 && y2 < y1max) && 
	      (s1min < s2 && s2 < s1max)) {

	    if (nTracklets >= kNmaxTrk) {
	      Warning("FindTracksCombi","Too many tracklets for this track.");
	    } else {
	      trkTrack[nTracklets++] = trkId2;
	      isPlane[iPlan2] = kTRUE;
	    }

	  }

	}  // end trk 2

      }  // end plan 2

      nPlanes = 0;
      for (Int_t iPlan = 0; iPlan < kNplan; iPlan++) {
	nPlanes += (Int_t)isPlane[iPlan];
      }
      
      if (nPlanes >= 4) {

	Int_t cha1, cha2, npoints1, npoints2;
	for (Int_t iList = 0; iList < (nTracklets-1); iList++) {

	  if (trkTrack[iList] == -1 || trkTrack[iList+1] == -1) continue;
	  trk1 = GetTracklet(trkTrack[iList  ]);
	  trk2 = GetTracklet(trkTrack[iList+1]);

	  cha1 = trk1->GetDetector();
	  cha2 = trk2->GetDetector();
	  if (cha1 != cha2) continue;

	  npoints1 = trk1->GetNclusters();
	  npoints2 = trk2->GetNclusters();

	  if (npoints1 == npoints2) {
	    trkTrack[iList] = -1;
	  } else {
	    if (npoints1 > npoints2) trkTrack[iList+1] = -1;
	    if (npoints1 < npoints2) trkTrack[iList  ] = -1;
	  }

	}

	fGTUtrk = new IlcDCHgtuTrack();
	for (Int_t iList = 0; iList < nTracklets; iList++) {
	  if (trkTrack[iList] == -1) continue;
	  trk = GetTracklet(trkTrack[iList]);
	  fGTUtrk->AddTracklet(trk);
	}
	fGTUtrk->Track(fXprojPlane,fField);
	AddTrack();
      }
           
    }  // end trk 1

  }  // end plan 1

}

//_____________________________________________________________________________
void IlcDCHmodule::AddTrack() 
{
  //
  // Add a found track to the module
  //
  
  Tracks()->Add(fGTUtrk);

}

//_____________________________________________________________________________
void IlcDCHmodule::RemoveMultipleTracks()
{
  //
  // Remove multiple found tracks
  //

  IlcDCHgtuTrack *trk1;
  IlcDCHgtuTrack *trk2;

  Float_t yproj1, yproj2, alpha1, alpha2;
  Int_t ntrk1, ntrk2;
  Int_t iTrack = 0;

  while (iTrack < (GetNtracks()-1)) {

    trk1 = GetTrack(iTrack  );
    trk2 = GetTrack(iTrack+1);

    ntrk1  = trk1->GetNtracklets();
    yproj1 = trk1->GetYproj();
    alpha1 = trk1->GetSlope();
    ntrk2  = trk2->GetNtracklets();
    yproj2 = trk2->GetYproj();
    alpha2 = trk2->GetSlope();

    if (TMath::Abs(yproj1-yproj2) < fDeltaY && TMath::Abs(alpha1-alpha2) < fDeltaS) {
      if (ntrk1 < ntrk2) {
	RemoveTrack(iTrack  );
      } else {
	RemoveTrack(iTrack+1);
      }
    } else {
      iTrack++;
    }
    
  }

}

