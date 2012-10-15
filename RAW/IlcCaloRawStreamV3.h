#ifndef ILCCALORAWSTREAMV3_H
#define ILCCALORAWSTREAMV3_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to Calo digits in raw data.
///
/// Yuri Kharlov. 23 June 2009
/// 
///////////////////////////////////////////////////////////////////////////////

// --- ROOT system ---
#include "TString.h"

// --- IlcRoot header files ---
#include "IlcAltroRawStreamV3.h"
class IlcRawReader;
class IlcAltroMapping;

class IlcCaloRawStreamV3: public IlcAltroRawStreamV3 {

public :
  IlcCaloRawStreamV3(IlcRawReader* rawReader, TString calo, IlcAltroMapping **mapping = NULL);
  virtual ~IlcCaloRawStreamV3();
  
  virtual void             Reset();
  virtual Bool_t           NextChannel();
  
  Int_t   GetModule()      const {return fModule;}
  Int_t   GetRow()         const {return fRow   ;} // EMCAL notation
  Int_t   GetColumn()      const {return fColumn;} // EMCAL notation
  Int_t   GetCellX()       const {return fRow   ;} // PHOS notation
  Int_t   GetCellZ()       const {return fColumn;} // PHOS notation
  Int_t   GetNRCU()        const {return fNRCU  ;}
  Int_t   GetNSides()      const {return fNSides;}
  TString GetCalorimeter() const {return fCalo  ;}

  enum EIlcCaloFlag { kLowGain=0, kHighGain=1, kTRUData=2, kLEDMonData=3 };
  Bool_t  IsLowGain()      const {return (fCaloFlag == kLowGain)   ;}
  Bool_t  IsHighGain()     const {return (fCaloFlag == kHighGain)  ;}
  Bool_t  IsTRUData()      const {return (fCaloFlag == kTRUData)   ;}
  Bool_t  IsLEDMonData()   const {return (fCaloFlag == kLEDMonData);} 

  Int_t   GetCaloFlag() const { return fCaloFlag; } 

protected:

  IlcCaloRawStreamV3& operator = (const IlcCaloRawStreamV3& stream);
  IlcCaloRawStreamV3(const IlcCaloRawStreamV3& stream);

  virtual void ApplyAltroMapping();

  Int_t            fModule;   // index of current module
  Int_t            fRow;      // index of current row
  Int_t            fColumn;   // index of current column
  Int_t            fCaloFlag; // low (0) or (1) high gain; see enum EIlcCaloFlag above
  Int_t            fNModules; // number of (super)modules
  Int_t            fNRCU;     // number of RCU per (super)module
  Int_t            fNSides;   // Division of EMCal in "A" "C" sides
  TString          fCalo;     // Calorimeter name
  Bool_t           fExternalMapping; // use external mapping or create a default one
  IlcAltroMapping *fMapping[20];     // pointers to ALTRO mapping

  ClassDef(IlcCaloRawStreamV3, 3)   // class for reading PHOS/EMCAL raw digits

};

#endif
