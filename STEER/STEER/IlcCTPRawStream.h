#ifndef ILCCTPRAWSTREAM_H
#define ILCCTPRAWSTREAM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
///
/// This is a class for reading the CTP (trigger) DDL raw data
/// The format of the raw data is taken form the trigger TDR
///
///////////////////////////////////////////////////////////////////////////////

#include <TObject.h>
#include <TClonesArray.h>

class IlcRawReader;
class IlcTriggerIR;

class IlcCTPRawStream: public TObject {
  public :
    IlcCTPRawStream(IlcRawReader* rawReader);
    virtual ~IlcCTPRawStream();


    virtual void             Reset();
    virtual Bool_t           Next();

    UInt_t    GetOrbitID() const { return fOrbit; }
    UShort_t  GetBCID()    const { return fBC; }

    UInt_t    GetL0Inputs() const { return fL0TriggerInputs; }
    UInt_t    GetL1Inputs() const { return fL1TriggerInputs; }
    UShort_t  GetL2Inputs() const { return fL2TriggerInputs; }

    ULong64_t GetClassMask()   const { return fClassMask; }  // Provide the trigger class mask
    UChar_t   GetClusterMask() const { return fClusterMask; }// Provide the trigger cluster mask

    Int_t     GetNIRs() const { return fIRArray.GetEntriesFast(); }
    const TClonesArray &GetAllIRs() const { return fIRArray; }
    const IlcTriggerIR *GetIR(Int_t index) const { return (const IlcTriggerIR*)fIRArray.UncheckedAt(index); }


  protected:
    IlcCTPRawStream(const IlcCTPRawStream& stream);
    IlcCTPRawStream& operator = (const IlcCTPRawStream& stream);

  private:
    TClonesArray     fIRArray;     // array with trigger interaction records

    UInt_t           fOrbit;       // orbit ID as sent by CTP
    UShort_t         fBC;          // bunch-crossing number as sent by CTP

    UInt_t           fL0TriggerInputs; // L0 trigger inputs (24 bits)
    UInt_t           fL1TriggerInputs; // L1 trigger inputs (24 bits)
    UShort_t         fL2TriggerInputs; // L2 trigger inputs (12 bits)

    ULong64_t        fClassMask;   // trigger class mask
    UChar_t          fClusterMask; // trigger cluster mask

    IlcRawReader*    fRawReader;   // object for reading the raw data

    ClassDef(IlcCTPRawStream, 0)   // class for reading CTP DDL raw data
};

#endif
