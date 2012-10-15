#ifndef ILCVMERAWSTREAM_H
#define ILCVMERAWSTREAM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcVMERawStream.h 15079 2006-09-18 14:46:38Z cvetan $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to VME data in test beam raw data.
///
///////////////////////////////////////////////////////////////////////////////

#include <TObject.h>

class IlcRawReader;


class IlcVMERawStream: public TObject {
  public :
    IlcVMERawStream(IlcRawReader* rawReader);
    virtual ~IlcVMERawStream() {};

    virtual Bool_t   Next();

    UShort_t         GetBlock() const {return fBlock;};
    UShort_t         GetNumber() const {return fNumber;};
    UShort_t         GetChannel() const {return fChannel;};
    UShort_t         GetValue() const {return fValue;};

    UInt_t           GetTDCChannel(Int_t iTDC) const
      {return fTDCChannel[iTDC];};
    UInt_t           GetTDCValue(Int_t iTDC) const
      {return fTDCValue[iTDC];};

    UInt_t           GetTime() const {return fTime;};
    UInt_t           GetTimeMuSec() const {return fTimeMuSec;};

  private :
    IlcVMERawStream(const IlcVMERawStream& stream);
    IlcVMERawStream& operator = (const IlcVMERawStream& stream);

    Bool_t           CheckString(const char* str) const;
    Bool_t           ReadTDC();
    Bool_t           ReadTime();

    IlcRawReader*    fRawReader;    // object for reading the raw data

    UInt_t           fData;         // data read for file

    Int_t            fNChannels;    // number of c-ram channels
    UShort_t         fBlock;        // C-ram block
    UShort_t         fNumber;       // C-ram serial number
    UShort_t         fChannel;      // C-ram channel
    UShort_t         fValue;        // C-ram ADC value

    static const Int_t fgkNTDC = 3; // number of TDCs
    UInt_t           fTDCChannel[fgkNTDC];      // TDC channels
    UInt_t           fTDCValue[fgkNTDC];        // TDC values

    UInt_t           fTime;         // timing information
    UInt_t           fTimeMuSec;    // micro seconds

    ClassDef(IlcVMERawStream, 0) // class for reading VME raw digits
};

#endif
