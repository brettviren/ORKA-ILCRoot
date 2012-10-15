#ifndef IlcOADBContainer_H
#define IlcOADBContainer_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//-------------------------------------------------------------------------
//     Offline Analysis Database Container and Service Class 
//     Author: Andreas Morsch, CERN
//-------------------------------------------------------------------------

#include <TNamed.h>
#include <TList.h>
#include <TArrayI.h>
#include <TObjArray.h>

class TObjArray;
class TArrayI;

class IlcOADBContainer : public TNamed {

 public :
  IlcOADBContainer();
  IlcOADBContainer(const char* name);
  virtual ~IlcOADBContainer();
  IlcOADBContainer(const IlcOADBContainer& cont); 
  IlcOADBContainer& operator=(const IlcOADBContainer& cont);
// Object adding and removal
  void   AppendObject(TObject* obj, Int_t lower, Int_t upper);
  void   UpdateObject(Int_t index, TObject* obj, Int_t lower, Int_t upper);
  void   RemoveObject(Int_t index);
  void   AddDefaultObject(TObject* obj);
  void   CleanDefaultList();
  TList* GetDefaultList() const {return fDefaultList;}
// I/O  
  void  WriteToFile(const char* fname)  const;
  Int_t InitFromFile(const char* fname, const char* key);
// Getters
  Int_t GetNumberOfEntries()    const {return fEntries;}
  Int_t LowerLimit(Int_t idx)   const {return fLowerLimits[idx];}
  Int_t UpperLimit(Int_t idx)   const {return fUpperLimits[idx];}
  TObject* GetObject(Int_t run, const char* def = "") const;
  TObject* GetObjectByIndex(Int_t run) const;
  TObject* GetDefaultObject(const char* key) 
  {return(fDefaultList->FindObject(key));}
// Debugging  
  void List();
// Browsable
  virtual Bool_t	IsFolder() const { return kTRUE; }
  void Browse(TBrowser *b);
  Int_t GetIndexForRun(Int_t run) const;
//
  static const char*   GetOADBPath();
 private:
  Int_t HasOverlap(Int_t lower, Int_t upper) const;
 private :
  TObjArray*               fArray;         // Array with objects corresponding to run ranges
  TList*                   fDefaultList;   // List with default arrays
  TArrayI                  fLowerLimits;   // lower limit of run range
  TArrayI                  fUpperLimits;   // upper limit of run range
  Int_t                    fEntries;       // Number of entries
//  TString                  fRelPath;       // Relative path to object
  
  ClassDef(IlcOADBContainer, 1);
};

#endif
