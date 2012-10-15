#ifndef ILCTARGETRAWSTREAM_H
#define ILCTARGETRAWSTREAM_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETRawStream.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This is a base class for providing access to TARGET digits in raw data.
///
///////////////////////////////////////////////////////////////////////////////

#include <TObject.h>

class IlcRawReader;


class IlcTARGETRawStream: public TObject {
  public :
    IlcTARGETRawStream(IlcRawReader* rawReader);
    IlcTARGETRawStream(const IlcTARGETRawStream& stream);
    IlcTARGETRawStream& operator = (const IlcTARGETRawStream& stream);
    virtual ~IlcTARGETRawStream() {};

    virtual Bool_t   Next() = 0;

    Int_t            GetModuleID() const {return fModuleID;};
    Int_t            GetPrevModuleID() const {return fPrevModuleID;};
    Bool_t           IsNewModule() const {return fModuleID != fPrevModuleID;};
    Int_t            GetCoord1() const {return fCoord1;};
    Int_t            GetCoord2() const {return fCoord2;};
    Int_t            GetSignal() const {return fSignal;};

  protected :
    IlcRawReader*    fRawReader;    // object for reading the raw data

    Int_t            fModuleID;     // index of current module
    Int_t            fPrevModuleID; // index of previous module
    Int_t            fCoord1;       // current 1st coordinate
                                    //  SPD: column cell number (z)
                                    //  SDD: anode cell number (z)
                                    //  SSD: N/P, flag for side
    Int_t            fCoord2;       // current 2nd coordinate
                                    //  SPD: row cell number (y)
                                    //  SDD: time bin number (y)
                                    //  SSD: strip number
    Int_t            fSignal;       // signal in ADC counts

    ClassDef(IlcTARGETRawStream, 0) // base class for reading TARGET raw digits
};

#endif
