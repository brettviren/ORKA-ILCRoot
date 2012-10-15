#ifndef ILCVMFT_H
#define ILCVMFT_H

//-------------------------------------------------------------------------
//     Base class for ESD and AOD MFT data
//     Author: Cvetan Cheshkov
//     cvetan.cheshkov@cern.ch 2/02/2011
//-------------------------------------------------------------------------

#include "TObject.h"

class IlcVMFT : public TObject 
{
public:
  IlcVMFT() { }
  IlcVMFT(const IlcVMFT& source);
  IlcVMFT &operator=(const IlcVMFT& source);

  virtual ~IlcVMFT() { }


protected:  

    
  ClassDef(IlcVMFT,1)
};

#endif
