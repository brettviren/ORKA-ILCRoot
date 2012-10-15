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

/* $Id: IlcDCHsegmentArray.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Ilc segment manager class                                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TTree.h>

#include "IlcRun.h"

#include "IlcDCHgeometry.h"
#include "IlcDCHsegmentArray.h"
#include "IlcDCHdataArray.h"

ClassImp(IlcDCHsegmentArray)

//_____________________________________________________________________________
IlcDCHsegmentArray::IlcDCHsegmentArray():IlcDCHsegmentArrayBase()
{
  //
  // Default constructor
  //

}

//_____________________________________________________________________________
IlcDCHsegmentArray::IlcDCHsegmentArray(const char *classname, Int_t n)
                   :IlcDCHsegmentArrayBase(classname,n)
{
  //
  // Constructor creating an array of IlcDCHdataArray of size <n>
  //

  IlcDCHdataArray *dataArray;  

  for (Int_t i = 0; i < n; i++) {
    dataArray = (IlcDCHdataArray *) AddSegment(i);
  }

}

//_____________________________________________________________________________
IlcDCHsegmentArray::IlcDCHsegmentArray(IlcDCHsegmentArray &a)
:IlcDCHsegmentArrayBase(a)
{
  //
  // IlcDCHsegmentArray copy constructor
  //

  a.Copy(*this);

}

//_____________________________________________________________________________
IlcDCHsegmentArray::~IlcDCHsegmentArray()
{
  //
  // IlcDCHsegmentArray destructor
  //

  Delete();

}

//_____________________________________________________________________________
void IlcDCHsegmentArray::Copy(TObject &a) const
{
  //
  // Copy function
  //

  IlcDCHsegmentArrayBase::Copy(a);

}

//_____________________________________________________________________________
void IlcDCHsegmentArray::Delete()
{
  //
  // Deletes all detector segments from the array
  //

  for (Int_t iDet = 0; iDet < fNSegment; iDet++) {
    ClearSegment(iDet);
  }

}

//_____________________________________________________________________________
Bool_t IlcDCHsegmentArray::LoadArray(const Char_t *branchname, TTree *tree)
{
  //
  // Loads all segments of the array from the branch <branchname> of
  // the digits tree <tree>
  //

  fTree = tree;

  // Connect the digits tree as default
  if (!fTree) {
    printf("IlcDCHsegmentArray::LoadArray -- ");
    printf("Use default TreeD\n");
    fTree = gIlc->TreeD();
    if (!fTree) return kFALSE;
  }

  // Get the branch
  fBranch = fTree->GetBranch(branchname);
  if (!fBranch) return kFALSE;

  // Loop through all segments and read them from the tree
  Bool_t status = kTRUE;
  for (Int_t iSegment = 0; iSegment < fNSegment; iSegment++) {
    IlcDCHdataArray *dataArray = (IlcDCHdataArray *) fSegment->At(iSegment);
    if (!dataArray) {
      status = kFALSE;
      break;    
    }
    fBranch->SetAddress(&dataArray);
    fBranch->GetEntry(iSegment);
  }

  return status;

}

//_____________________________________________________________________________
Bool_t IlcDCHsegmentArray::StoreArray(const Char_t *branchname, TTree *tree)
{
  //
  // Stores all segments of the array in the branch <branchname> of 
  // the digits tree <tree>
  //

  fTree = tree;

  // Connect the digits tree as default
  if (!fTree) {
    printf("IlcDCHsegmentArray::StoreArray -- ");
    printf("Use default TreeD\n");
    fTree = gIlc->TreeD();
    if (!fTree) return kFALSE;
  }

  // Get the branch
  fBranch = fTree->GetBranch(branchname);
  if (!fBranch) return kFALSE;

  // Loop through all segments and fill them into the tree
  Bool_t status = kTRUE;
  for (Int_t iSegment = 0; iSegment < fNSegment; iSegment++) {
    const IlcDCHdataArray *kDataArray = 
         (IlcDCHdataArray *) IlcDCHsegmentArrayBase::At(iSegment);
    if (!kDataArray) {
      status = kFALSE;
      break;
    }
    fBranch->SetAddress(&kDataArray);
    fBranch->Fill();
  }

  return status;

}

//_____________________________________________________________________________
IlcDCHdataArray *IlcDCHsegmentArray::GetDataArray(Int_t det) const
{
  //
  // Returns the data array for a given detector
  //

  return ((IlcDCHdataArray *) IlcDCHsegmentArrayBase::At(det));

}

//_____________________________________________________________________________
IlcDCHdataArray *IlcDCHsegmentArray::GetDataArray(Int_t pla
                                                , Int_t cha, Int_t sec) const
{
  //
  // Returns the data array for a given detector
  //

  if (gIlc) {

    IlcDCHgeometry *geo = IlcDCHgeometry::GetGeometry(gIlc->GetRunLoader());  
    Int_t det = geo->GetDetector(pla,cha,sec);
    return GetDataArray(det);

  }
  else {

    printf("IlcDCHsegmentArray::GetDigits -- ");
    printf("gIlc is not defined\n");
    return NULL;

  }

}
