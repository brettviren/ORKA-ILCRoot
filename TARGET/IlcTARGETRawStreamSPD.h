#ifndef ILCTARGETRAWSTREAMSPD_H
#define ILCTARGETRAWSTREAMSPD_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETRawStreamSPD.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to TARGET SPD digits in raw data.
///
///////////////////////////////////////////////////////////////////////////////

#include "IlcTARGETRawStream.h"


class IlcTARGETRawStreamSPD: public IlcTARGETRawStream {
  public :
    IlcTARGETRawStreamSPD(IlcRawReader* rawReader);
    virtual ~IlcTARGETRawStreamSPD() {};

    virtual Bool_t   Next();

    Int_t            GetRow() const {return fCoord2;};
    Int_t            GetColumn() const {return fCoord1;};

    enum {kDDLOffset = 0x100};    // offset for DDL numbers
    enum {kDDLsNumber = 20};      // number of DDLs in SPD
    enum {kModulesPerDDL = 12};   // number of modules in each DDL

    static Int_t     GetModuleNumber(UInt_t iDDL, UInt_t iModule)
      {return fgkDDLModuleMap[iDDL][iModule];}

  private :
    static const Int_t fgkDDLModuleMap[kDDLsNumber][kModulesPerDDL];  // mapping DDL/module -> module number

    UShort_t         fData;         // data read for file
    Int_t            fDDLNumber;    // current DDL number
    Int_t            fEventNumber;  // event trigger number
    Int_t            fModuleNumber; // sequential module number
    UInt_t           fOffset;       // offset for cell column
    UInt_t           fHitCount;     // counter of hits

    ClassDef(IlcTARGETRawStreamSPD, 0) // class for reading TARGET SPD raw digits
};

#endif
