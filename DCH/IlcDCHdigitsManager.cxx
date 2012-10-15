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

/* $Id: IlcDCHdigitsManager.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Manages the digits and the track dictionary in the form of               //
//  IlcDCHdataArray objects.                                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <Riostream.h>
 
#include <TROOT.h>
#include <TTree.h>                                                              
#include <TFile.h>

#include "IlcRun.h"

#include "IlcDCHdigitsManager.h"
#include "IlcDCHsegmentArray.h"
#include "IlcDCHdataArrayI.h"
#include "IlcDCHdigit.h"
#include "IlcDCHgeometry.h"

ClassImp(IlcDCHdigitsManager)

//_____________________________________________________________________________

  // Number of track dictionary arrays
  const Int_t IlcDCHdigitsManager::fgkNDict = kNDict;

//_____________________________________________________________________________
IlcDCHdigitsManager::IlcDCHdigitsManager():TObject()
{
  //
  // Default constructor
  //

  fIsRaw   = kFALSE;
  fEvent   = 0;
  fDebug   = 0;
  fSDigits = 0;

  fTree    = NULL;
  fDigits  = NULL;
  for (Int_t iDict = 0; iDict < kNDict; iDict++) {
    fDictionary[iDict] = NULL;
  }

}

//_____________________________________________________________________________
IlcDCHdigitsManager::IlcDCHdigitsManager(const IlcDCHdigitsManager &m)
:TObject(m)
{
  //
  // IlcDCHdigitsManager copy constructor
  //

  ((IlcDCHdigitsManager &) m).Copy(*this);

}

//_____________________________________________________________________________
IlcDCHdigitsManager::~IlcDCHdigitsManager()
{
  //
  // IlcDCHdigitsManager destructor
  //

  if (fDigits) {
    fDigits->Delete();
    delete fDigits;
    fDigits = NULL;
  }

  for (Int_t iDict = 0; iDict < kNDict; iDict++) {
    fDictionary[iDict]->Delete();
    delete fDictionary[iDict];
    fDictionary[iDict] = NULL;
  }

}

//_____________________________________________________________________________
void IlcDCHdigitsManager::Copy(TObject &m) const
{
  //
  // Copy function
  //

  ((IlcDCHdigitsManager &) m).fIsRaw   = fIsRaw;
  ((IlcDCHdigitsManager &) m).fEvent   = fEvent;
  ((IlcDCHdigitsManager &) m).fDebug   = fDebug;
  ((IlcDCHdigitsManager &) m).fSDigits = fSDigits;

  TObject::Copy(m);

}

//_____________________________________________________________________________
void IlcDCHdigitsManager::CreateArrays()
{
  //
  // Create the data arrays
  //

  fDigits = new IlcDCHsegmentArray("IlcDCHdataArrayI",36500);

  for (Int_t iDict = 0; iDict < kNDict; iDict++) {
    fDictionary[iDict] = new IlcDCHsegmentArray("IlcDCHdataArrayI"
                                               ,36500);
  }

}
//_____________________________________________________________________________
void IlcDCHdigitsManager::ResetArrays()
{
  //
  // Reset the data arrays
  //

  if (fDigits) {
    delete fDigits;
  }
  fDigits = new IlcDCHsegmentArray("IlcDCHdataArrayI",36500);

  for (Int_t iDict = 0; iDict < kNDict; iDict++) {
    if (fDictionary[iDict]) {  
      delete fDictionary[iDict];
    }
    fDictionary[iDict] = new IlcDCHsegmentArray("IlcDCHdataArrayI"
                                               ,36500);
  }

}

//_____________________________________________________________________________
void IlcDCHdigitsManager::SetRaw()
{
  //
  // Switch on the raw digits flag
  //

  fIsRaw = kTRUE;
  if (fDigits)
    fDigits->SetBit(IlcDCHdigit::RawDigit());
  
}

//_____________________________________________________________________________
Short_t IlcDCHdigitsManager::GetDigitAmp(Int_t row, Int_t col,Int_t time
                                       , Int_t det) const
{
  //
  // Returns the amplitude of a digit
  //

  if (!GetDigits(det)) return 0;
  return ((Short_t) GetDigits(det)->GetData(row,col,time));

}
 
//_____________________________________________________________________________
Bool_t IlcDCHdigitsManager::MakeBranch(TTree *tree)
{
  //
  // Creates the tree and branches for the digits and the dictionary
  //

  Int_t buffersize = 64000;

  Bool_t status = kTRUE;

  if (tree) {
    fTree = tree;
  }

  // Make the branch for the digits
  if (fDigits) {
    const IlcDCHdataArray *kDigits = (IlcDCHdataArray *) fDigits->At(0);
    if (kDigits) {
      if (!fTree) return kFALSE;
      TBranch* branch = fTree->GetBranch("DCHdigits");
      if (!branch) fTree->Branch("DCHdigits",kDigits->IsA()->GetName(),
                                 &kDigits,buffersize,99);
      if (fDebug > 0) {
        printf("<IlcDCHdigitsManager::MakeBranch> ");
        printf("Making branch DCHdigits\n");
      }
    }
    else {
      status = kFALSE;
    }
  }
  else {
    status = kFALSE;
  }

  // Make the branches for the dictionaries
  for (Int_t iDict = 0; iDict < kNDict; iDict++) {
    Char_t branchname[15];
    sprintf(branchname,"DCHdictionary%d",iDict);
    if (fDictionary[iDict]) {
      const IlcDCHdataArray *kDictionary = 
              (IlcDCHdataArray *) fDictionary[iDict]->At(0);
      if (kDictionary) {
	if (!fTree) return kFALSE;
	TBranch* branch = fTree->GetBranch(branchname);
	if (!branch) fTree->Branch(branchname,kDictionary->IsA()->GetName(),
				   &kDictionary,buffersize,99);
        if (fDebug > 0) {
          printf("<IlcDCHdigitsManager::MakeBranch> ");
          printf("Making branch %s\n",branchname);
	}
      }
      else {
        status = kFALSE;
      }
    }
    else {
      status = kFALSE;
    }
  }

  return status;

}

//_____________________________________________________________________________
Bool_t IlcDCHdigitsManager::ReadDigits(TTree *tree)
{
  //
  // Reads the digit information from the input file
  //

  Bool_t status = kTRUE;

  if (tree) {

    fTree = tree;

  }

  if (!fDigits) {
    if (fDebug > 0) {
      printf("<IlcDCHdigitsManager::ReadDigits> ");
      printf("Create the data arrays.\n");
    }
    CreateArrays();
  }

  status = fDigits->LoadArray("DCHdigits",fTree);

  for (Int_t iDict = 0; iDict < kNDict; iDict++) {
    Char_t branchname[15];
    sprintf(branchname,"DCHdictionary%d",iDict);
    status = fDictionary[iDict]->LoadArray(branchname,fTree);
  }  

  if (fDigits->TestBit(IlcDCHdigit::RawDigit())) {
    fIsRaw = kTRUE;
  }
  else {
    fIsRaw = kFALSE;
  }

  return kTRUE;

}

//_____________________________________________________________________________
Bool_t IlcDCHdigitsManager::WriteDigits()
{
  //
  // Writes out the DCH-digits and the dictionaries
  //

  // Store the contents of the segment array in the tree
  if (!fDigits->StoreArray("DCHdigits",fTree)) {
    printf("<IlcDCHdigitsManager::WriteDigits> ");
    printf("Error while storing digits in branch DCHdigits\n");
    return kFALSE;
  }
  for (Int_t iDict = 0; iDict < kNDict; iDict++) {
    Char_t branchname[15];
    sprintf(branchname,"DCHdictionary%d",iDict);
    if (!fDictionary[iDict]->StoreArray(branchname,fTree)) {
      printf("<IlcDCHdigitsManager::WriteDigits> ");
      printf("Error while storing dictionary in branch %s\n",branchname);
      return kFALSE;
    }
  }

  // Write the new tree to the output file
  //fTree->Write();
  fTree->AutoSave();  // Modification by Jiri

  return kTRUE;

}

//_____________________________________________________________________________
IlcDCHdigit *IlcDCHdigitsManager::GetDigit(Int_t row, Int_t col
                                         , Int_t time, Int_t det) const
{
  // 
  // Creates a single digit object 
  //

  Int_t digits[4];
  Int_t amp[1];

  digits[0] = det;
  digits[1] = row;
  digits[2] = col;
  digits[3] = time;

  amp[0]    = GetDigits(det)->GetData(row,col,time);
  
  return (new IlcDCHdigit(fIsRaw,digits,amp));

}

//_____________________________________________________________________________
Int_t IlcDCHdigitsManager::GetTrack(Int_t track
                                  , Int_t row, Int_t col, Int_t time
                                  , Int_t det) const
{
  // 
  // Returns the MC-track numbers from the dictionary.
  //

  if ((track < 0) || (track >= kNDict)) {
    TObject::Error("GetTracks"
                  ,"track %d out of bounds (size: %d, this: 0x%08x)"
                  ,track,kNDict,this);
    return -1;
  }

  // Array contains index+1 to allow data compression
  return (GetDictionary(det,track)->GetData(row,col,time) - 1);

}

//_____________________________________________________________________________
IlcDCHdataArrayI *IlcDCHdigitsManager::GetDigits(Int_t det) const
{
  //
  // Returns the digits array for one detector
  //

  if (!fDigits) return 0x0;
  return (IlcDCHdataArrayI *) fDigits->At(det);

}

//_____________________________________________________________________________
IlcDCHdataArrayI *IlcDCHdigitsManager::GetDictionary(Int_t det, Int_t i) const
{
  //
  // Returns the dictionary for one detector
  //

  return (IlcDCHdataArrayI *) fDictionary[i]->At(det);

}

//_____________________________________________________________________________
Int_t IlcDCHdigitsManager::GetTrack(Int_t track, IlcDCHdigit *Digit) const
{
  // 
  // Returns the MC-track numbers from the dictionary for a given digit
  //

  Int_t row  = Digit->GetRow();
  Int_t col  = Digit->GetCol();
  Int_t time = Digit->GetTime();
  Int_t det  = Digit->GetDetector();

  return GetTrack(track,row,col,time,det);

}

//_____________________________________________________________________________
IlcDCHdigitsManager &IlcDCHdigitsManager::operator=(const IlcDCHdigitsManager &m)
{
  //
  // Assignment operator
  //

  if (this != &m) ((IlcDCHdigitsManager &) m).Copy(*this);
  return *this;

}
