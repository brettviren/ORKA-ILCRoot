#ifndef ILCDCHSEGMENTARRAY_H
#define ILCDCHSEGMENTARRAY_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHsegmentArray.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

#include "IlcDCHsegmentArrayBase.h"

////////////////////////////////////////////////////////
//  Array for DCH detector segments containing digits //
////////////////////////////////////////////////////////

class TTree;

class IlcDCHdataArray;

//_____________________________________________________________________________
class IlcDCHsegmentArray : public IlcDCHsegmentArrayBase {

 public:

  IlcDCHsegmentArray();
  IlcDCHsegmentArray(const char *classname, Int_t n);
  IlcDCHsegmentArray(IlcDCHsegmentArray &a);
  virtual ~IlcDCHsegmentArray();

  virtual void             Copy(TObject &a) const;
  virtual void             Delete();
  virtual void             Delete(const char *) { Delete(); };

  virtual Bool_t           LoadArray(const Char_t *branchname, TTree *tree = 0);
  virtual Bool_t           StoreArray(const Char_t *branchname, TTree *tree = 0);

  virtual IlcDCHdataArray *GetDataArray(Int_t det) const;
  virtual IlcDCHdataArray *GetDataArray(Int_t sec, Int_t cha, Int_t pla) const;

 protected:

  ClassDef(IlcDCHsegmentArray,1)        // DCH detector segment array 

};

#endif
