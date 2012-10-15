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

/* $Id: IlcDCHmatrix.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Contains the pixel information for one DCH chamber                       //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TObjArray.h>
#include <TH2.h>
#include <TH3.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "IlcDCHmatrix.h"
#include "IlcDCHpixel.h"

ClassImp(IlcDCHmatrix)

//_____________________________________________________________________________
IlcDCHmatrix::IlcDCHmatrix():TObject()
{
  //
  // Create a DCH detector matrix
  // 
 
  fRow        = 0;
  fCol        = 0;
  fTime       = 0;
  fPixel      = 0;
  fSector     = 0;
  fChamber    = 0;
  fPlane      = 0;
  fPixelArray = NULL;

}

//_____________________________________________________________________________
IlcDCHmatrix::IlcDCHmatrix(Int_t nRow, Int_t nCol, Int_t nTime
                         , Int_t iSec, Int_t iCha, Int_t iPla)
             :TObject()
{
  //
  // Create a DCH detector matrix with a given size
  // 

  fRow        = nRow;
  fCol        = nCol;
  fTime       = nTime;
  fPixel      = nRow * nCol * nTime;
  fSector     = iSec;
  fChamber    = iCha;
  fPlane      = iPla;
  fPixelArray = new TObjArray(fPixel);
  for (Int_t iPixel = 0; iPixel < fPixel; iPixel++) {
    IlcDCHpixel *pixel = new IlcDCHpixel();
    fPixelArray->Add(pixel);
  }

}

//_____________________________________________________________________________
IlcDCHmatrix::IlcDCHmatrix(const IlcDCHmatrix &m):TObject(m)
{
  //
  // IlcDCHmatrix copy constructor
  //

  ((IlcDCHmatrix &) m).Copy(*this);

}

//_____________________________________________________________________________
IlcDCHmatrix::~IlcDCHmatrix()
{
  //
  // IlcDCHmatrix destructor
  //

  if (fPixelArray) {
    fPixelArray->Delete();
    delete fPixelArray;
  }

}

//_____________________________________________________________________________
IlcDCHmatrix &IlcDCHmatrix::operator=(const IlcDCHmatrix &m)
{
  //
  // Assignment operator
  //

  if (this != &m) ((IlcDCHmatrix &) m).Copy(*this);
  return *this;

}

//_____________________________________________________________________________
void IlcDCHmatrix::AddSignal(Int_t iRow, Int_t iCol, Int_t iTime, Float_t signal)
{
  //
  // Add a value to the amplitude of the signal for one specific pixel
  //

  IlcDCHpixel *pixel = GetPixel(iRow,iCol,iTime);
  if (pixel) {
    signal += pixel->GetSignal();
    pixel->SetSignal(signal);
  }

}

//_____________________________________________________________________________
void IlcDCHmatrix::Copy(TObject &m) const
{
  //
  // Copy function
  //

  ((IlcDCHmatrix &) m).fRow        = fRow;
  ((IlcDCHmatrix &) m).fCol        = fCol;
  ((IlcDCHmatrix &) m).fTime       = fTime;
  ((IlcDCHmatrix &) m).fPixel      = fPixel;
  ((IlcDCHmatrix &) m).fSector     = fSector;
  ((IlcDCHmatrix &) m).fChamber    = fChamber;
  ((IlcDCHmatrix &) m).fPlane      = fPlane;

  ((IlcDCHmatrix &) m).fPixelArray = new TObjArray(*fPixelArray);

}

//_____________________________________________________________________________
void IlcDCHmatrix::Draw(Option_t *)
{
  //
  // Draws a 3D view of the detector matrix
  //

  Char_t ctitle[50];
  sprintf(ctitle,"Matrix (Sector:%d Chamber:%d Plane:%d)"
                ,fSector,fChamber,fPlane);
  TH3F *hMatrix = new TH3F("hMatrix",ctitle,fRow ,-0.5,fRow -0.5
                                           ,fCol ,-0.5,fCol -0.5
                                           ,fTime,-0.5,fTime-0.5);

  for (Int_t iRow  = 0; iRow  < fRow;  iRow++ ) {
    for (Int_t iCol  = 0; iCol  < fCol;  iCol++ ) {
      for (Int_t iTime = 0; iTime < fTime; iTime++) {
        IlcDCHpixel *pixel = GetPixel(iRow,iCol,iTime);
        if (pixel) hMatrix->Fill(iRow,iCol,iTime,pixel->GetSignal());
      }
    }
  }

  gStyle->SetOptStat(0);
  TCanvas *cMatrix = new TCanvas("cMatrix","Detector matrix 3D-view"
                                          ,50,50,600,400);
  cMatrix->ToggleEventStatus();
  hMatrix->SetXTitle("Pad-row (z)");
  hMatrix->SetYTitle("Pad-column (rphi)");
  hMatrix->SetZTitle("Timebucket");
  hMatrix->Draw("BOX");

}

//_____________________________________________________________________________
void IlcDCHmatrix::DrawRow(Int_t iRow)
{
  //
  // Draws a 2D slice of the detector matrix along one row
  //

  if ((iRow < 0) || (iRow >= fRow)) {
    printf("IlcDCHmatrix::DrawRow -- ");
    printf("Index out of bounds (%d/%d)\n",iRow,fRow);
    return;
  }

  Char_t ctitle[50];
  sprintf(ctitle,"Pad-row %d (Sector:%d Chamber:%d Plane:%d)"
                ,iRow,fSector,fChamber,fPlane);
  TH2F *hSliceRow = new TH2F("hSliceRow",ctitle,fCol ,-0.5,fCol -0.5
                                               ,fTime,-0.5,fTime-0.5);

  for (Int_t iCol  = 0; iCol  < fCol;  iCol++ ) {
    for (Int_t iTime = 0; iTime < fTime; iTime++) {
      IlcDCHpixel *pixel = GetPixel(iRow,iCol,iTime);
      if (pixel) hSliceRow->Fill(iCol,iTime,pixel->GetSignal());
    }
  }

  gStyle->SetOptStat(0);
  TCanvas *cSliceRow = new TCanvas("cSliceRow","Detector matrix 2D-slice"
                                              ,50,50,600,400);
  cSliceRow->ToggleEventStatus();
  hSliceRow->SetXTitle("Pad-column (rphi)");
  hSliceRow->SetYTitle("Timebucket");
  hSliceRow->Draw("COLZ");

}

//_____________________________________________________________________________
void IlcDCHmatrix::DrawCol(Int_t iCol)
{
  //
  // Draws a 2D slice of the detector matrix along one column
  //

  if ((iCol < 0) || (iCol >= fCol)) {
    printf("IlcDCHmatrix::DrawCol -- ");
    printf("Index out of bounds (%d/%d)\n",iCol,fCol);
    return;
  }

  Char_t ctitle[50];
  sprintf(ctitle,"Pad-column %d (Sector:%d Chamber:%d Plane:%d)"
                ,iCol,fSector,fChamber,fPlane);
  TH2F *hSliceCol = new TH2F("hSliceCol",ctitle,fRow ,-0.5,fRow -0.5
                                               ,fTime,-0.5,fTime-0.5);

  for (Int_t iRow  = 0; iRow  < fRow;  iRow++ ) {
    for (Int_t iTime = 0; iTime < fTime; iTime++) {
      IlcDCHpixel *pixel = GetPixel(iRow,iCol,iTime);
      if (pixel) hSliceCol->Fill(iRow,iTime,pixel->GetSignal());
    }
  }

  gStyle->SetOptStat(0);
  TCanvas *cSliceCol = new TCanvas("cSliceCol","Detector matrix 2D-slice"
                                              ,50,50,600,400);
  cSliceCol->ToggleEventStatus();
  hSliceCol->SetXTitle("Pad-row (z)");
  hSliceCol->SetYTitle("Timebucket");
  hSliceCol->Draw("COLZ");

}

//_____________________________________________________________________________
void IlcDCHmatrix::DrawTime(Int_t iTime)
{
  //
  // Draws a 2D slice of the detector matrix along one time slice
  //

  if ((iTime < 0) || (iTime >= fTime)) {
    printf("IlcDCHmatrix::DrawTime -- ");
    printf("Index out of bounds (%d/%d)\n",iTime,fTime);
    return;
  }

  Char_t ctitle[50];
  sprintf(ctitle,"Time-slice %d (Sector:%d Chamber:%d Plane:%d)"
                ,iTime,fSector,fChamber,fPlane);
  TH2F *hSliceTime = new TH2F("hSliceTime",ctitle,fRow,-0.5,fRow-0.5
                                                 ,fCol,-0.5,fCol-0.5);

  for (Int_t iRow = 0; iRow < fRow; iRow++) {
    for (Int_t iCol = 0; iCol < fCol; iCol++) {
      IlcDCHpixel *pixel = GetPixel(iRow,iCol,iTime);
      if (pixel) hSliceTime->Fill(iRow,iCol,pixel->GetSignal());
    }
  }

  gStyle->SetOptStat(0);
  TCanvas *cSliceTime = new TCanvas("cSliceTime","Detector matrix 2D-slice"
                                                ,50,50,600,400);
  cSliceTime->ToggleEventStatus();
  hSliceTime->SetXTitle("Pad-row (z)");
  hSliceTime->SetYTitle("Pad-column (rphi)");
  hSliceTime->Draw("COLZ");

}

//_____________________________________________________________________________
void IlcDCHmatrix::ProjRow()
{
  //
  // Projects the detector matrix along the row-axis
  //

  Char_t ctitle[60];
  sprintf(ctitle,"Row-projection (Sector:%d Chamber:%d Plane:%d)"
                ,fSector,fChamber,fPlane);
  TH2F *hProjRow = new TH2F("hProjRow",ctitle,fCol ,-0.5,fCol -0.5
                                             ,fTime,-0.5,fTime-0.5);

  for (Int_t iRow  = 0; iRow  < fRow;  iRow++ ) {
    for (Int_t iCol  = 0; iCol  < fCol;  iCol++ ) {
      for (Int_t iTime = 0; iTime < fTime; iTime++) {
        IlcDCHpixel *pixel = GetPixel(iRow,iCol,iTime);
        if (pixel) hProjRow->Fill(iCol,iTime,pixel->GetSignal());
      }
    }
  }

  gStyle->SetOptStat(0);
  TCanvas *cProjRow = new TCanvas("cProjRow","Detector matrix 2D-projection"
                                            ,50,50,600,400);
  cProjRow->ToggleEventStatus();
  hProjRow->SetXTitle("Pad-column (rphi)");
  hProjRow->SetYTitle("Timebucket");
  hProjRow->Draw("COLZ");

}

//_____________________________________________________________________________
void IlcDCHmatrix::ProjCol()
{
  //
  // Projects the detector matrix along the column-axis
  //

  Char_t ctitle[60];
  sprintf(ctitle,"Column-projection (Sector:%d Chamber:%d Plane:%d)"
                ,fSector,fChamber,fPlane);
  TH2F *hProjCol = new TH2F("hProjCol",ctitle,fRow ,-0.5,fRow -0.5
                                             ,fTime,-0.5,fTime-0.5);

  for (Int_t iRow  = 0; iRow  < fRow;  iRow++ ) {
    for (Int_t iCol  = 0; iCol  < fCol;  iCol++ ) {
      for (Int_t iTime = 0; iTime < fTime; iTime++) {
        IlcDCHpixel *pixel = GetPixel(iRow,iCol,iTime);
        if (pixel) hProjCol->Fill(iRow,iTime,pixel->GetSignal());
      }
    }
  }

  gStyle->SetOptStat(0);
  TCanvas *cProjCol = new TCanvas("cProjCol","Detector matrix 2D-projection"
                                            ,50,50,600,400);
  cProjCol->ToggleEventStatus();
  hProjCol->SetXTitle("Pad-row (z)");
  hProjCol->SetYTitle("Timebucket");
  hProjCol->Draw("COLZ");

}

//_____________________________________________________________________________
void IlcDCHmatrix::ProjTime()
{
  //
  // Projects the detector matrix along the time-axis
  //

  Char_t ctitle[60];
  sprintf(ctitle,"Time-projection (Sector:%d Chamber:%d Plane:%d)"
                ,fSector,fChamber,fPlane);
  TH2F *hProjTime = new TH2F("hProjTime",ctitle,fRow,-0.5,fRow-0.5
                                               ,fCol,-0.5,fCol-0.5);

  for (Int_t iRow = 0; iRow < fRow; iRow++) {
    for (Int_t iCol = 0; iCol < fCol; iCol++) {
      for (Int_t iTime = 0; iTime < fTime; iTime++) {
        IlcDCHpixel *pixel = GetPixel(iRow,iCol,iTime);
        if (pixel) hProjTime->Fill(iRow,iCol,pixel->GetSignal());
      }
    }
  }

  gStyle->SetOptStat(0);
  TCanvas *cProjTime = new TCanvas("cProjTime","Detector matrix 2D-projection"
                                              ,50,50,600,400);
  cProjTime->ToggleEventStatus();
  hProjTime->SetXTitle("Pad-row (z)");
  hProjTime->SetYTitle("Pad-column (rphi)");
  hProjTime->Draw("COLZ");

}

//_____________________________________________________________________________
void IlcDCHmatrix::SetSignal(Int_t iRow, Int_t iCol, Int_t iTime, Float_t signal)
{
  //
  // Set the amplitude of the signal for one specific pixel
  //

  IlcDCHpixel *pixel = GetPixel(iRow,iCol,iTime);
  if (pixel) {
    pixel->SetSignal(signal);
  }

}

//_____________________________________________________________________________
Bool_t IlcDCHmatrix::AddTrack(Int_t iRow, Int_t iCol, Int_t iTime, Int_t track)
{
  // 
  // Add this track number to the stored tracks passing through this pixel. 
  // If there are already three stored the return status is FALSE.  
  //

  IlcDCHpixel *pixel = GetPixel(iRow,iCol,iTime);
  if (!(pixel)) return kTRUE;

  Bool_t trackSet = kFALSE;
  for (Int_t i = 0; i < IlcDCHpixel::NTrackPixel(); i++) {
    if (pixel->GetTrack(i) == track) {
      trackSet = kTRUE;
      break;
    }
    if (pixel->GetTrack(i) ==    -1) {
      pixel->SetTrack(i,track);
      trackSet = kTRUE;
      break;
    }
  }    

  return trackSet;

}

//_____________________________________________________________________________
void IlcDCHmatrix::SetTrack(Int_t iRow, Int_t iCol, Int_t iTime
                          , Int_t iTrack, Int_t track)
{
  //
  // Store the number of a track which is passing through this pixel
  //

  IlcDCHpixel *pixel = GetPixel(iRow,iCol,iTime);
  if (pixel) {
    pixel->SetTrack(iTrack,track);
  }

}

//_____________________________________________________________________________
Float_t IlcDCHmatrix::GetSignal(Int_t iRow, Int_t iCol, Int_t iTime) const
{
  //
  // Returns the amplitude of the signal for one specific pixel
  //

  IlcDCHpixel *pixel = GetPixel(iRow,iCol,iTime);
  if (pixel) {
    return (pixel->GetSignal());
  }
  else {
    return 0;
  }

}

//_____________________________________________________________________________
Int_t IlcDCHmatrix::GetTrack(Int_t iRow, Int_t iCol, Int_t iTime
                           , Int_t iTrack) const 
{
  //
  // Returns the numbers of the tracks passing through one specific pixel
  //

  if ((iTrack < 0) || (iTrack >= IlcDCHpixel::NTrackPixel())) {
    printf("IlcDCHmatrix::GetTrack -- ");
    printf("Index out of bounds (%d)\n",iTrack);
    return -1;
  }

  IlcDCHpixel *pixel = GetPixel(iRow,iCol,iTime);
  if (pixel) {
    return (pixel->GetTrack(iTrack));
  }
  else {
    return -1;
  }

}

//_____________________________________________________________________________
Int_t IlcDCHmatrix::GetIndex(Int_t iRow, Int_t iCol, Int_t iTime) const
{
  //
  // Get the index of a given pixel
  //

  if ((iRow  >= 0) && (iRow  < fRow ) &&
      (iCol  >= 0) && (iCol  < fCol ) &&
      (iTime >= 0) && (iTime < fTime)) {
    return (iTime + iCol * fTime + iRow * fTime * fCol);
  }
  else {
    return -1;
  }

}

//_____________________________________________________________________________
IlcDCHpixel *IlcDCHmatrix::GetPixel(Int_t iRow, Int_t iCol, Int_t iTime) const
{
  //
  // Get one pixel
  //

  Int_t iPixel = GetIndex(iRow,iCol,iTime);
  if (iPixel < 0) {
    return NULL;
  }
  else {
    return ((IlcDCHpixel *) fPixelArray->At(iPixel));
  }

}
