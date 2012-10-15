#ifndef ILCTARGETRAWSTREAMSDDV2_H
#define ILCTARGETRAWSTREAMSDDV2_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETRawStreamSDDv2.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to TARGET SDD digits in test beam raw data.
///
///////////////////////////////////////////////////////////////////////////////

#include "IlcTARGETRawStreamSDD.h"

class IlcRawReader;


class IlcTARGETRawStreamSDDv2: public IlcTARGETRawStreamSDD {
  public :
    IlcTARGETRawStreamSDDv2(IlcRawReader* rawReader);
    virtual ~IlcTARGETRawStreamSDDv2() {};

    virtual Bool_t   Next();

  private :
 

    ClassDef(IlcTARGETRawStreamSDDv2, 1) // class for reading TARGET SDD raw digits
};

#endif
