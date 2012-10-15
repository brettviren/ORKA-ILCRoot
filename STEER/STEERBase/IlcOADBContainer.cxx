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

/* $Id$ */

//-------------------------------------------------------------------------
//     Offline Analysis Database Container and Service Class 
//     Author: Andreas Morsch, CERN
//-------------------------------------------------------------------------




#include "IlcOADBContainer.h"
#include "IlcLog.h"
#include <TObjArray.h>
#include <TArrayI.h>
#include <TFile.h>
#include <TList.h>
#include <TBrowser.h>
#include <TSystem.h>
#include <TError.h>

ClassImp(IlcOADBContainer);

//______________________________________________________________________________
IlcOADBContainer::IlcOADBContainer() : 
  TNamed(),
  fArray(0),
  fDefaultList(0),
  fLowerLimits(),
  fUpperLimits(),
  fEntries(0)
{
  // Default constructor
}

IlcOADBContainer::IlcOADBContainer(const char* name) : 
  TNamed(name, "OADBContainer"),
  fArray(new TObjArray(100)),
  fDefaultList(new TList()),
  fLowerLimits(),
  fUpperLimits(),
  fEntries(0)
{
  // Constructor
}


//______________________________________________________________________________
IlcOADBContainer::~IlcOADBContainer() 
{
  // destructor
}

//______________________________________________________________________________
IlcOADBContainer::IlcOADBContainer(const IlcOADBContainer& cont) :
  TNamed(cont),
  fArray(cont.fArray),
  fDefaultList(cont.fDefaultList),
  fLowerLimits(cont.fLowerLimits),
  fUpperLimits(cont.fUpperLimits),
  fEntries(cont.fEntries)
{
  // Copy constructor.
}

//______________________________________________________________________________
IlcOADBContainer& IlcOADBContainer::operator=(const IlcOADBContainer& cont)
{
  //
  // Assignment operator
  // Copy objects related to run ranges
  if(this!=&cont) {
    TNamed::operator=(cont);
    fEntries = cont.fEntries;
    fLowerLimits.Set(fEntries);
    fUpperLimits.Set(fEntries);
    for (Int_t i = 0; i < fEntries; i++) {
	fLowerLimits[i] = cont.fLowerLimits[i]; 
	fUpperLimits[i] = cont.fUpperLimits[i];
	fArray->AddAt(cont.fArray->At(i), i);
    }
  }
  //
  // Copy default objects
  TList* list = cont.GetDefaultList();
  TIter next(list);
  TObject* obj;
  while((obj = next())) fDefaultList->Add(obj);
  //
  return *this;
}

void IlcOADBContainer::AppendObject(TObject* obj, Int_t lower, Int_t upper)
{
  //
  // Append a new object to the list 
  //
  // Check that there is no overlap with existing run ranges
  Int_t index = HasOverlap(lower, upper);
  
  if (index != -1) {
    IlcFatal(Form("Ambiguos validity range (%5d, %5.5d-%5.5d) !\n", index,lower,upper));
    return;
  }
  //
  // Adjust arrays
  fEntries++;
  fLowerLimits.Set(fEntries);
  fUpperLimits.Set(fEntries);

  // Add the object
  fLowerLimits[fEntries - 1] = lower;
  fUpperLimits[fEntries - 1] = upper;
  fArray->Add(obj);
}

void IlcOADBContainer::RemoveObject(Int_t idx)
{
  //
  // Remove object from the list 

  //
  // Check that index is inside range 
  if (idx < 0 || idx >= fEntries) 
    {
      IlcError(Form("Index out of Range %5d >= %5d", idx, fEntries));
      return;
    }
  //
  // Remove the object
  TObject* obj = fArray->RemoveAt(idx);
  delete obj;
  //
  // Adjust the run ranges and shrink the array
  for (Int_t i = idx; i < (fEntries-1); i++) {
    fLowerLimits[i] = fLowerLimits[i + 1]; 
    fUpperLimits[i] = fUpperLimits[i + 1];
    fArray->AddAt(fArray->At(i+1), i);
  }
  fArray->RemoveAt(fEntries - 1);
  fEntries--;
}

void IlcOADBContainer::UpdateObject(Int_t idx, TObject* obj, Int_t lower, Int_t upper)
{
  //
  // Update an existing object, at a given position 

  // Check that index is inside range
  if (idx < 0 || idx >= fEntries) 
    {
      IlcError(Form("Index out of Range %5d >= %5d", idx, fEntries));
      return;
    }
  //
  // Remove the old object and reset the range
  //  TObject* obj2 = 
  fArray->RemoveAt(idx);
  // don't delete it: if you are updating it may be pointing to the same location of obj...
  //  delete obj2;
  fLowerLimits[idx] = -1;
  fUpperLimits[idx] = -1;
  // Check that there is no overlap with existing run ranges  
  Int_t index = HasOverlap(lower, upper);
  if (index != -1) {
    IlcFatal(Form("Ambiguos validity range (%5d, %5.5d-%5.5d) !\n", index,lower,upper));
    return;
  }
  //
  // Add object at the same position
  //printf("idx %d obj %llx\n", idx, obj);
  fLowerLimits[idx] = lower;
  fUpperLimits[idx] = upper;
  fArray->AddAt(obj, idx);

}
    
 
void  IlcOADBContainer::AddDefaultObject(TObject* obj)
{
  // Add a default object
  fDefaultList->Add(obj);
}

void  IlcOADBContainer::CleanDefaultList()
{
  // Clean default list
  fDefaultList->Delete();
}

Int_t IlcOADBContainer::GetIndexForRun(Int_t run) const
{
  //
  // Find the index for a given run 
  
  Int_t found = 0;
  Int_t index = -1;
  for (Int_t i = 0; i < fEntries; i++) 
    {
      if (run >= fLowerLimits[i] && run <= fUpperLimits[i])
	{
	  found++;
	  index = i;
	}
    }

  if (found > 1) {
    IlcError(Form("More than one (%5d) object found; return last (%5d) !\n", found, index));
  } else if (index == -1) {
    IlcWarning(Form("No object found for run %5d !\n", run));
  }
  
  return index;
}

TObject* IlcOADBContainer::GetObject(Int_t run, const char* def) const
{
  // Return object for given run or default if not found
  TObject* obj = 0;
  Int_t idx = GetIndexForRun(run);
  if (idx == -1) {
    // no object found, try default
    obj = fDefaultList->FindObject(def);
    if (!obj) {
      IlcError("Default Object not found !\n");
      return (0);
    } else {
      return (obj);
    }
  } else {
    return (fArray->At(idx));
  }
}

TObject* IlcOADBContainer::GetObjectByIndex(Int_t run) const
{
  // Return object for given index
  return (fArray->At(run));
}

void IlcOADBContainer::WriteToFile(const char* fname) const
{
  //
  // Write object to file
  TFile* f = new TFile(fname, "update");
  Write();
  f->Purge();
  f->Close();
}

Int_t IlcOADBContainer::InitFromFile(const char* fname, const char* key)
{
    // 
    // Initialize object from file
    TFile* file = TFile::Open(fname);
    if (!file) return (1);
    IlcOADBContainer* cont  = 0;
    file->GetObject(key, cont);
    if (!cont)
    {
	IlcError("Object not found in file \n");	
	return 1;
    }

    SetName(cont->GetName());
    SetTitle(cont->GetTitle());

    fEntries = cont->GetNumberOfEntries();
    fLowerLimits.Set(fEntries);
    fUpperLimits.Set(fEntries);
    if(fEntries > fArray->GetSize()) fArray->Expand(fEntries);

    for (Int_t i = 0; i < fEntries; i++) {
	fLowerLimits[i] = cont->LowerLimit(i); 
	fUpperLimits[i] = cont->UpperLimit(i);
	fArray->AddAt(cont->GetObjectByIndex(i), i);
    }
    if (!fDefaultList) fDefaultList = new TList(); 
    TIter next(cont->GetDefaultList());
    TObject* obj;
    while((obj = next())) fDefaultList->Add(obj);

    return 0;
    
}


void IlcOADBContainer::List()
{
  //
  // List Objects
  printf("Entries %d\n", fEntries);
  
  for (Int_t i = 0; i < fEntries; i++) {
    printf("Lower %5d Upper %5d \n", fLowerLimits[i], fUpperLimits[i]);
    (fArray->At(i))->Dump();
  }
  TIter next(fDefaultList);
  TObject* obj;
  while((obj = next())) obj->Dump();

}

Int_t IlcOADBContainer::HasOverlap(Int_t lower, Int_t upper) const
{
  //
  // Checks for overlpapping validity regions
  for (Int_t i = 0; i < fEntries; i++) {
    if ((lower >= fLowerLimits[i] && lower <= fUpperLimits[i]) ||
	(upper >= fLowerLimits[i] && upper <= fUpperLimits[i]))
      {
	return (i);
      }
  }
  return (-1);
}

void IlcOADBContainer::Browse(TBrowser *b)
{
   // Browse this object.
   // If b=0, there is no Browse call TObject::Browse(0) instead.
   //         This means TObject::Inspect() will be invoked indirectly


  if (b) {
    for (Int_t i = 0; i < fEntries; i++) {
      b->Add(fArray->At(i),Form("%9.9d - %9.9d", fLowerLimits[i], fUpperLimits[i]));
    }
    TIter next(fDefaultList);
    TObject* obj;
    while((obj = next())) b->Add(obj);
        
  }     
   else
      TObject::Browse(b);
}

//______________________________________________________________________________
const char* IlcOADBContainer::GetOADBPath()
{
// returns the path of the OADB
// this static function just depends on environment variables

   static TString oadbPath;

   if (gSystem->Getenv("OADB_PATH"))
      oadbPath = gSystem->Getenv("OADB_PATH");
   else if (gSystem->Getenv("ILC_ROOT"))
      oadbPath.Form("%s/OADB", gSystem->Getenv("ILC_ROOT"));
   else
   ::Fatal("IlcAnalysisManager::GetOADBPath", "Cannot figure out AODB path. Define ILC_ROOT or OADB_PATH!");
   return oadbPath;
}
