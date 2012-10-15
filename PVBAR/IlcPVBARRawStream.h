#ifndef ILCPVBARRAWSTREAM_H
#define ILCPVBARRAWSTREAM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARRawStream.h 14794 2006-08-08 16:33:17Z cvetan $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to PVBAR digits in raw data.
///
///////////////////////////////////////////////////////////////////////////////

// --- ROOT system ---

// --- IlcRoot header files ---
#include "IlcAltroRawStream.h"
class IlcRawReader;
class IlcAltroMapping;

class IlcPVBARRawStream: public IlcAltroRawStream {

public :
  IlcPVBARRawStream(IlcRawReader* rawReader);
  virtual ~IlcPVBARRawStream();
 
  virtual void             Reset();
  virtual Bool_t           Next();
  
  Int_t            GetModule()     const {return fModule;}
  Int_t            GetRow()        const {return fRow;}
  Int_t            GetColumn()     const {return fColumn;}
  Int_t            GetPrevModule() const {return fPrevModule;}
  Int_t            GetPrevRow()    const {return fPrevRow;}
  Int_t            GetPrevColumn() const {return fPrevColumn;}
  Bool_t           IsNewModule()   const {return GetModule() != GetPrevModule();}
  Bool_t           IsNewRow()      const {return (GetRow() != GetPrevRow()) || IsNewModule();}
  Bool_t           IsNewColumn()   const {return (GetColumn() != GetPrevColumn()) || IsNewRow();}
  Bool_t           IsLowGain()     const {return (!fGain);} 

protected:

  IlcPVBARRawStream(const IlcPVBARRawStream& stream);
  IlcPVBARRawStream& operator = (const IlcPVBARRawStream& stream);

  virtual void ApplyAltroMapping();

  Int_t            fModule;       // index of current module
  Int_t            fPrevModule;   // index of previous module
  Int_t            fRow;          // index of current row
  Int_t            fPrevRow;      // index of previous row
  Int_t            fColumn;       // index of current column
  Int_t            fPrevColumn;   // index of previous column
  Bool_t           fGain;         // low (0) or (1) high gain
  
  IlcAltroMapping *fMapping[4];   // pointers to ALTRO mapping

  ClassDef(IlcPVBARRawStream, 0)   // class for reading PVBAR raw digits

};

#endif

