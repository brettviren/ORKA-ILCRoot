#ifndef ILCDCHSEGMENTARRAYBASE_H
#define ILCDCHSEGMENTARRAYBASE_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHsegmentArrayBase.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

////////////////////////////////////////////////
//  Manager class for a general Ilc segment // 
////////////////////////////////////////////////

#include <TNamed.h>

class TTree;
class TBranch;
class IlcDCHarrayI;
class IlcDCHsegmentID;
class TObjArray;
 
class IlcDCHsegmentArrayBase: public TNamed {

 public:

  IlcDCHsegmentArrayBase();
  IlcDCHsegmentArrayBase(const char *classname, Int_t n); 
  IlcDCHsegmentArrayBase(const IlcDCHsegmentArrayBase &a);
  virtual ~IlcDCHsegmentArrayBase();
  IlcDCHsegmentArrayBase &operator=(const IlcDCHsegmentArrayBase &a);
 
  const IlcDCHsegmentID *At(Int_t i) const; 
  const IlcDCHsegmentID *operator[](Int_t i) const; 

          Bool_t           AddSegment(IlcDCHsegmentID *segment);
          IlcDCHsegmentID *AddSegment(Int_t index);  
          void             ClearSegment(Int_t index); 
  virtual void             Copy(TObject &a) const;
  virtual Bool_t           ConnectTree(const char *treeName);
          Bool_t           MakeArray(Int_t n);    
  virtual IlcDCHsegmentID *NewSegment(); 
  virtual void             MakeTree(char *file = 0);           
  virtual IlcDCHsegmentID *LoadSegment(Int_t index);
  virtual IlcDCHsegmentID *LoadEntry(Int_t index); 
  virtual void             StoreSegment(Int_t index);
          Bool_t           MakeDictionary(Int_t size);

          Bool_t           SetClass(const char *classname);
 
          TClass          *GetClass() const { return fClass; };
          TTree           *GetTree() const  { return fTree;  };   

 protected:

  TObjArray    *fSegment;            //! Pointer to an array of pointers to a segment
  IlcDCHarrayI *fTreeIndex;          //! Pointers(index) table
  Int_t         fNSegment;           //  Number of segments 
  TTree        *fTree;               //! Tree with segment objects
  TBranch      *fBranch;             //! Branchaddress
  TClass       *fClass;              //! Class type of included objects 

  ClassDef(IlcDCHsegmentArrayBase,1) // DCH detextor segment array base class

};

#endif 
