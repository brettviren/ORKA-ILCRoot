#ifndef ILCTARGETRAWSTREAMSSD_H
#define ILCTARGETRAWSTREAMSSD_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETRawStreamSSD.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to TARGET SSD digits in raw data.
///
///////////////////////////////////////////////////////////////////////////////

#include "IlcTARGETRawStream.h"

class IlcRawReader;


class IlcTARGETRawStreamSSD: public IlcTARGETRawStream {
  public :
    IlcTARGETRawStreamSSD(IlcRawReader* rawReader);
    virtual ~IlcTARGETRawStreamSSD() {};

    virtual Bool_t   Next();

    Int_t            GetSideFlag() const {return fCoord1;};
    Int_t            GetStrip() const {return fCoord2;};

    enum {kDDLOffset = 0x300};    // offset for DDL numbers
    enum {kDDLsNumber = 16};      // number of DDLs in SSD
    enum {kModulesPerDDL = 109};  // number of modules in each DDL

    static Int_t     GetModuleNumber(UInt_t iDDL, UInt_t iModule)
      {return fgkDDLModuleMap[iDDL][iModule];}

  protected :
    static const Int_t fgkDDLModuleMap[kDDLsNumber][kModulesPerDDL];  // mapping DDL/module -> module number

    UInt_t           fData;         // data read for file

    ClassDef(IlcTARGETRawStreamSSD, 0) // class for reading TARGET SSD raw digits
};

#endif
