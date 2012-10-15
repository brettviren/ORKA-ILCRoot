#ifndef ILCDCHRAWSTREAM_H
#define ILCDCHRAWSTREAM_H 
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */ 
 
/* $Id: IlcDCHRawStream.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */ 
 
/////////////////////////////////////////////////////////////////////////////// 
/// 
/// This class provides access to DCH digits in raw data. 
/// 
/////////////////////////////////////////////////////////////////////////////// 
 
#include <TObject.h> 
 
class IlcRawReader; 
class IlcDCHparameter; 
 
class IlcDCHRawStream: public TObject { 
  public : 
    IlcDCHRawStream(IlcRawReader* rawReader); 
    virtual ~IlcDCHRawStream(); 
 
    virtual Bool_t   Next(); 
 
    Int_t            GetDetector() const {return fDetector;}; 
    Int_t            GetPrevDetector() const {return fPrevDetector;}; 
    Bool_t           IsNewDetector() const {return fDetector != fPrevDetector;}; 
    Int_t            GetNPads() const {return fNPads;}; 
    Int_t            GetRow() const {return fRow;}; 
    Int_t            GetPrevRow() const {return fPrevRow;}; 
    Bool_t           IsNewRow() const {return (fRow != fPrevRow) || IsNewDetector();}; 
    Int_t            GetColumn() const {return fColumn;}; 
    Int_t            GetPrevColumn() const {return fPrevColumn;}; 
    Bool_t           IsNewColumn() const {return (fColumn != fPrevColumn) || IsNewRow();}; 
    Int_t            GetTime() const {return fTime-1;}; 
    Int_t            GetSignal() const {return fSignal;}; 
 
    enum {kDDLOffset = 0x400};    // offset for DDL numbers 
 
  private : 
    IlcDCHRawStream(const IlcDCHRawStream& stream); 
    IlcDCHRawStream& operator = (const IlcDCHRawStream& stream); 
 
    IlcRawReader*    fRawReader;    // object for reading the raw data 
 
    Int_t            fCount;        // counter of bytes to be read for current detector 
 
    Int_t            fDetector;     // index of current detector 
    Int_t            fPrevDetector; // index of previous detector 
    Int_t            fNPads;        // number of active pads 
    Int_t            fRow;          // index of current pad row 
    Int_t            fPrevRow;      // index of previous pad row 
    Int_t            fColumn;       // index of current pad column 
    Int_t            fPrevColumn;   // index of previous pad column 
    Int_t            fTime;         // index of current time bin 
    Int_t            fSignal;       // signal in ADC counts 
 
    ClassDef(IlcDCHRawStream, 1)    // class for reading DCH raw digits 
}; 
 
#endif 
