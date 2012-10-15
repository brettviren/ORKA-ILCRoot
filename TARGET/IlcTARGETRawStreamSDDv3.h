#ifndef ILCTARGETRAWSTREAMSDDV3_H
#define ILCTARGETRAWSTREAMSDDV3_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETRawStreamSDDv3.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to TARGET SDD digits in test beam raw data.
///
///////////////////////////////////////////////////////////////////////////////

#include "IlcTARGETRawStreamSDD.h"

class IlcRawReader;


class IlcTARGETRawStreamSDDv3: public IlcTARGETRawStreamSDD {
  public :
    IlcTARGETRawStreamSDDv3(IlcRawReader* rawReader);
    virtual ~IlcTARGETRawStreamSDDv3() {};

    virtual Bool_t   Next();
    virtual Int_t    GetJitter();
  private :


    ClassDef(IlcTARGETRawStreamSDDv3, 1) // class for reading TARGET SDD raw digits
};

#endif
