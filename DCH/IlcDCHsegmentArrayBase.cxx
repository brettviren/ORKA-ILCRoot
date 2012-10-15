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

/* $Id: IlcDCHsegmentArrayBase.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Ilc segment manager base class                                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCHsegmentArrayBase.h"
#include <TROOT.h>
#include <TTree.h>
#include <TClonesArray.h>
#include <TDirectory.h>
#include <TError.h>
#include <TClass.h>

#include "IlcDCHarrayI.h"
#include "IlcDCHsegmentID.h"

ClassImp(IlcDCHsegmentArrayBase)
  
//_____________________________________________________________________________
IlcDCHsegmentArrayBase::IlcDCHsegmentArrayBase():TNamed()
{
  //
  // IlcDCHsegmentArrayBase default constructor
  //

  fNSegment  = 0;
  fSegment   = 0; 
  fTreeIndex = 0;
  fTree      = 0;
  fClass     = 0;
  fBranch    = 0;

}

//_____________________________________________________________________________
IlcDCHsegmentArrayBase::IlcDCHsegmentArrayBase(const char *classname, Int_t n)
{
  //
  //  Create an array of objects of <classname>. The class must inherit from
  //  IlcDCHsegmentID. The second argument sets the number of entries in 
  //  the array.
  //

  fNSegment  = 0;
  fSegment   = 0; 
  fTreeIndex = 0;
  fTree      = 0;
  fClass     = 0;
  fBranch    = 0;

  SetClass(classname);

  if (MakeArray(n) == kFALSE) {
    Error("IlcDCHsegmentArrayBase","Cannot allocate %d segments in memory",n);
    return;
  }

}

//_____________________________________________________________________________
IlcDCHsegmentArrayBase::IlcDCHsegmentArrayBase(const IlcDCHsegmentArrayBase &a)
:TNamed(a)
{
  //
  // IlcDCHsegmentArrayBase copy constructor
  //
  
  ((IlcDCHsegmentArrayBase &) a).Copy(*this);

}

//_____________________________________________________________________________
IlcDCHsegmentArrayBase::~IlcDCHsegmentArrayBase()
{
  //
  // IlcDCHsegmentArrayBase destructor
  //

  if (fNSegment){
    fSegment->Delete();
    delete fSegment;
  }

  //if (fTree)      delete fTree;
  if (fTreeIndex) delete fTreeIndex;

}

//_____________________________________________________________________________
IlcDCHsegmentArrayBase &IlcDCHsegmentArrayBase
                        ::operator=(const IlcDCHsegmentArrayBase &a)
{
  //
  // Assignment operator
  //

  if (this != &a) ((IlcDCHsegmentArrayBase &) a).Copy(*this);
  return *this;

}

//_____________________________________________________________________________
void IlcDCHsegmentArrayBase::Copy(TObject &a) const
{
  //
  // Copy function
  //

  TNamed::Copy(a);

  fSegment->Copy(*((IlcDCHsegmentArrayBase &) a).fSegment);
  fTreeIndex->Copy(*((IlcDCHsegmentArrayBase &) a).fTreeIndex);
  fClass->Copy(*((IlcDCHsegmentArrayBase &) a).fClass);

  ((IlcDCHsegmentArrayBase &) a).fNSegment = fNSegment;

}

//_____________________________________________________________________________
Bool_t IlcDCHsegmentArrayBase::SetClass(const char *classname)
{
  //
  // Sets the classname of the stored object
  //

  if (fTree    != 0) {
    delete fTree;
    fTree      = 0;
    fBranch    = 0;
    delete fTreeIndex;
    fTreeIndex = 0;
  } 
  if (fSegment != 0) {
    fSegment->Delete();
    delete fSegment;
    fSegment   = 0;
  }

  if (!gROOT) ::Fatal("IlcDCHsegmentArrayBase::IlcDCHsegmentArrayBase"
                     ,"ROOT system not initialized");
   
   fClass = gROOT->GetClass(classname);
   if (!fClass) {
     Error("IlcDCHsegmentArrayBase","%s is not a vilcd class name",classname);
     return kFALSE;
   }
   if (!fClass->InheritsFrom(IlcDCHsegmentID::Class())) {
     Error("IlcDCHsegmentArrayBase"
          ,"%s does not inherit from IlcDCHsegmentID",classname);
     return kFALSE;
   }
  
   return kTRUE;

}

//_____________________________________________________________________________
IlcDCHsegmentID *IlcDCHsegmentArrayBase::NewSegment()
{
  //
  // Create a new object according to the class information
  //

  if (fClass  == 0) return 0;

  IlcDCHsegmentID *segment = (IlcDCHsegmentID *) fClass->New();
  if (segment == 0) return 0;

  return segment;

}

//_____________________________________________________________________________
Bool_t IlcDCHsegmentArrayBase::AddSegment(IlcDCHsegmentID *segment)
{
  //
  // Add a segment to the array
  //

  if (segment  == 0) return kFALSE;
  if (fSegment == 0) return kFALSE;
  if (fClass   == 0) return kFALSE;

  if (!(segment->IsA()->InheritsFrom(fClass))) {
    Error("IlcDCHsegmentArrayBase","added class %s is not of proper type",
	  segment->IsA()->GetName());
    return kFALSE;
  }

  fSegment->AddAt(segment,segment->GetID());
  fNSegment = fSegment->GetLast() + 1;

  return kTRUE;

}

//_____________________________________________________________________________
IlcDCHsegmentID * IlcDCHsegmentArrayBase::AddSegment(Int_t index)
{
  //
  // Add a segment to the array
  //

  if (fSegment == 0) return 0;
  if (fClass   == 0) return 0;

  IlcDCHsegmentID *segment = NewSegment();
  if (segment  == 0) return 0;

  fSegment->AddAt(segment,index);
  segment->SetID(index);
  fNSegment = fSegment->GetLast() + 1;

  return segment;

}

//_____________________________________________________________________________
Bool_t IlcDCHsegmentArrayBase::MakeArray(Int_t n)
{
  //
  // Create an array of pointers to the segments
  //

  if (fSegment) {
    fSegment->Delete();
    delete fSegment;
  }
  if (fTreeIndex) delete fTreeIndex;  

  fSegment   = new TObjArray(n);
  fTreeIndex = new IlcDCHarrayI();
  fTreeIndex->Set(n);
  fNSegment  = n;
  if ((fSegment) && (fTreeIndex)) 
    return kTRUE;
  else 
    return kFALSE;
		  
}

//_____________________________________________________________________________
void IlcDCHsegmentArrayBase::ClearSegment(Int_t index)
{
  //
  // Remove a segment from the active memory    
  //

  //PH  if ((*fSegment)[index]){
  //PH    delete (*fSegment)[index]; // because problem with deleting TClonesArray
  //PH    fSegment->RemoveAt(index);
  //PH  }
  if (fSegment->At(index)){
    delete fSegment->RemoveAt(index);
  }

}

//_____________________________________________________________________________
void IlcDCHsegmentArrayBase::MakeTree(char *file)
{
  //
  // Create a tree for the segment
  //

  IlcDCHsegmentID *psegment = NewSegment();  

  if (fTree) delete fTree;
  fTree   = new TTree("Segment Tree","Tree with segments");

  fBranch = fTree->Branch("Segment",psegment->IsA()->GetName(),&psegment,64000);
  if (file) 
      fBranch->SetFile(file);      

  delete psegment;

}              

//_____________________________________________________________________________
Bool_t IlcDCHsegmentArrayBase::ConnectTree(const char * treeName)
{
  //
  // Connect a tree from current directory  
  //

  if (fTree){
    delete fTree;
    fTree   = 0;
    fBranch = 0;
  }

  fTree   = (TTree*) gDirectory->Get(treeName);
  if (fTree   == 0) return kFALSE;
  fBranch = fTree->GetBranch("Segment");
  if (fBranch == 0) return kFALSE;

  MakeDictionary(TMath::Max(fNSegment,Int_t(fTree->GetEntries())));

  return kTRUE;

}

//_____________________________________________________________________________
IlcDCHsegmentID *IlcDCHsegmentArrayBase::LoadSegment(Int_t index)
{
  //
  // Load a segment with index <index> into the memory
  //

  if (fTreeIndex == 0) MakeDictionary(3000);

  // First try to load dictionary 
  if (fTreeIndex == 0)        return 0;
  if (fBranch    == 0)        return 0;
  if (index > fTreeIndex->fN) return 0;
  //PH  IlcDCHsegmentID *s = (IlcDCHsegmentID*) (*fSegment)[index];
  IlcDCHsegmentID *s = (IlcDCHsegmentID*) fSegment->At(index);
  if (s == 0) s = NewSegment();
  s->SetID(index);
  
  if (s != 0) {
    Int_t treeIndex = (*fTreeIndex)[index];
    if (treeIndex < 1) 
      return 0;
    else 
      treeIndex--;   
    fBranch->SetAddress(&s);
    fTree->GetEvent(treeIndex);
    //PH    (*fSegment)[index] = (TObject*) s;
    fSegment->AddAt((TObject*) s, index);
  }
  else 
    return 0;

  return s;

}

//_____________________________________________________________________________
IlcDCHsegmentID *IlcDCHsegmentArrayBase::LoadEntry(Int_t index)
{
  //
  // Load a segment at position <index> in the tree into the memory
  //

  if (fBranch == 0)                return 0;
  if (index > fTree->GetEntries()) return 0;

  IlcDCHsegmentID *s = NewSegment();  
  if (s) {
    fBranch->SetAddress(&s);
    fTree->GetEvent(index);
  }
  else 
    return 0;

  Int_t nindex = s->GetID();
  ClearSegment(nindex);
  //PH  (*fSegment)[nindex] = (TObject *) s;
  fSegment->AddAt((TObject *) s, nindex);

  return s;

}

//_____________________________________________________________________________
void IlcDCHsegmentArrayBase::StoreSegment(Int_t index)
{
  //
  // Make a segment persistent 
  //

  const IlcDCHsegmentID *kSegment = (*this)[index];
  if (kSegment == 0) return;
  if (fTree    == 0) MakeTree();
  fBranch->SetAddress(&kSegment);
  fTree->Fill();

}

//_____________________________________________________________________________
Bool_t  IlcDCHsegmentArrayBase::MakeDictionary(Int_t size)
{
  //
  // Create an index table for the tree
  //  

  if (size < 1)   return kFALSE;
  if (fTreeIndex) delete fTreeIndex;

  fTreeIndex = new IlcDCHarrayI(); 
  fTreeIndex->Set(size);
  
  IlcDCHsegmentID   segment;
  IlcDCHsegmentID *psegment = &segment;

  fBranch->SetAddress(&psegment);
  TBranch *brindix = fTree->GetBranch("fSegmentID");

  Int_t nevent = (Int_t) fTree->GetEntries();  
  for (Int_t i = 0; i < nevent; i++){
    brindix->GetEvent(i);
    Int_t treeIndex = segment.GetID();
    if (fTreeIndex->fN < treeIndex) 
      fTreeIndex->Expand(Int_t (Float_t(treeIndex) * 1.5) + 1);
    (*fTreeIndex)[treeIndex] = i + 1; 
  }

  return kTRUE;

}

//_____________________________________________________________________________
const IlcDCHsegmentID * IlcDCHsegmentArrayBase::operator[](Int_t i) const
{
  //
  // Returns a segment with the given index <i>
  //

  if ((i < 0) || (i >= fNSegment)) return 0; 
  return (IlcDCHsegmentID *) fSegment->At(i);

}

//_____________________________________________________________________________
const IlcDCHsegmentID *IlcDCHsegmentArrayBase::At(Int_t i) const
{
  //
  // Returns a segment with the given index <i>
  //

  if ((i < 0) || (i >= fNSegment)) return 0; 
  //PH  return (IlcDCHsegmentID *)((*fSegment)[i]);
  return (IlcDCHsegmentID *) fSegment->At(i);

}
