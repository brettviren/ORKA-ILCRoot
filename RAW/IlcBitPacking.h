#ifndef ILCBITPACKING_H
#define ILCBITPACKING_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcBitPacking.h 11342 2005-02-17 08:28:59Z tkuhr $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This is a service class for packing and unpacking bits in a 32 bit word.
///
///////////////////////////////////////////////////////////////////////////////

#include <TObject.h>


class IlcBitPacking: public TObject {
  public :
    static Bool_t  PackWord(UInt_t data, UInt_t &word, 
			    Int_t startBit, Int_t stopBit);
    static UInt_t  UnpackWord(UInt_t word, Int_t startBit, Int_t stopBit);

    ClassDef(IlcBitPacking, 0) // class for packing and unpacking bits
};

#endif
