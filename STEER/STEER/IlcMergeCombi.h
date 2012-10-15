#ifndef ILCMERGECOMBI_H
#define ILCMERGECOMBI_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcMergeCombi.h 50615 2011-07-16 23:19:19Z hristov $ */

////////////////////////////////////////////////////////////////////////
//
//  Class to make combination for merging
//  Returns combination of input event numbers                
//  Author: Jiri Chudoba (CERN), 2001
//
////////////////////////////////////////////////////////////////////////

// --- ROOT system ---

#include <TNamed.h>

// --- IlcRoot header files ---

class IlcMergeCombi: public TNamed {

public:
  IlcMergeCombi();
  IlcMergeCombi(Int_t dim, Int_t sperb);
  virtual ~IlcMergeCombi();
  Bool_t Combination(Int_t evNumber[], Int_t delta[]);
  
private:  
  Int_t fDim;               //! dimension of arrays evNumber and delta
  Int_t fSperb;             //! signal per background ratio
  Int_t fCounter;           //! counter for calls
  
  ClassDef(IlcMergeCombi,1)
};

#endif // ILCMERGECOMBI_H

