#ifndef ILCCALOALTROMAPPING_H
#define ILCCALOALTROMAPPING_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcCaloAltroMapping.h 21929 2007-11-02 16:34:20Z cvetan $ */

//////////////////////////////////////////////////////////
// Class used to setup the mapping of hardware adresses //
// in ALTRO to Calo cells (column and row index +       //
// low/high gain.                                       //
// The mapping is defined in an external mapping files  //
// separately. The class derives from the base altro    //
// mapping class defined in the RAW package.            //
//////////////////////////////////////////////////////////

/// Exported from PHOS to be used also by EMCAL
/// November 2006 Gustavo Conesa Balbastre

#include "IlcAltroMapping.h"

class IlcCaloAltroMapping: public IlcAltroMapping {
 public:
  IlcCaloAltroMapping();
  IlcCaloAltroMapping(const char *mappingFile);
  virtual ~IlcCaloAltroMapping();

  // In case of PHOS/EMCAL the relevant segmentation is row-column-gain
  // or eta-phi-gain
  virtual Int_t GetHWAddress(Int_t row, Int_t column, Int_t gain);
  // Get Row (not pad-row as in the base class)
  virtual Int_t GetPadRow(Int_t hwAddress) const;
  // Get Column (not pad as in the base class)
  virtual Int_t GetPad(Int_t hwAddress) const;
  // Get Gain (0/1) (not sector as in the base class)
  virtual Int_t GetSector(Int_t hwAddress) const;

 protected:
  virtual Bool_t ReadMapping();
  virtual Bool_t CreateInvMapping();

  Int_t     fMinRow;        // Minimum row index
  Int_t     fMaxRow;        // Maximum row index
  Int_t     fMinCol;        // Minimum column index
  Int_t     fMaxCol;        // Maximum column index
  Short_t  *fInvMappingLow; //! Inverse of fMapping (Low gain)
  Short_t  *fInvMappingHigh;//! Inverse of fMapping (High gain)

 private:

  IlcCaloAltroMapping(const IlcCaloAltroMapping& mapping);
  IlcCaloAltroMapping& operator = (const IlcCaloAltroMapping& mapping);

  ClassDef(IlcCaloAltroMapping,3)  // Altro mapping handler class
};

#endif
