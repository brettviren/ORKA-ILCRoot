#ifndef ILCRECOPARAM_H
#define ILCRECOPARAM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Steering Class for reconstruction parameters                              //
// Revision: cvetan.cheshkov@cern.ch 12/06/2008                              //
// Its structure has been revised and it is interfaced to IlcRunInfo and     //
// IlcEventInfo.                                                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


#include "TObject.h"

class IlcDetectorRecoParam;
class IlcRunInfo;
class IlcEventInfo;
class THashTable;

class IlcRecoParam : public TObject
{

 public: 
  IlcRecoParam();
  virtual ~IlcRecoParam();  
  //
  enum {
    kNSpecies = 5,   // number of event species
// #ifdef MFT_UPGRADE  
//     kNDetectors = 17 // number of detectors (last one is MFT!)
// #else
//     kNDetectors = 16 // number of detectors (last one is GRP!)
// #endif
    kNDetectors = 17 // number of detectors (last one is MFT!)   // AU
  };
  enum EventSpecie_t {kDefault = 1,
		      kLowMult = 2,
		      kHighMult = 4,
		      kCosmic = 8,
		      kCalib = 16};

  static Int_t                  AConvert(EventSpecie_t es) ; 
  static EventSpecie_t          Convert(Int_t ies) ; 
  static EventSpecie_t          ConvertIndex(Int_t index) ;

  virtual void                  Print(Option_t *option="") const;
  const TObjArray              *GetDetRecoParamArray(Int_t iDet) const { return fDetRecoParams[iDet]; }
  void                          SetEventSpecie(const IlcRunInfo*runInfo, const IlcEventInfo &evInfo,
					       const THashTable*cosmicTriggersList);
  EventSpecie_t                 GetEventSpecie() const { return fEventSpecie; }
  static const char*            GetEventSpecieName(EventSpecie_t es);
  static const char*            GetEventSpecieName(Int_t esIndex);
  const char*                   PrintEventSpecie() const;
  const IlcDetectorRecoParam   *GetDetRecoParam(Int_t iDet) const;
  void                          AddDetRecoParam(Int_t iDet, IlcDetectorRecoParam* param);
  Bool_t                        AddDetRecoParamArray(Int_t iDet, TObjArray* parArray);

  IlcRecoParam(const IlcRecoParam&);
  IlcRecoParam& operator=(const IlcRecoParam&);


private:

  Int_t      fDetRecoParamsIndex[kNSpecies][kNDetectors]; // index to fDetRecoParams arrays
  TObjArray *fDetRecoParams[kNDetectors];   // array with reconstruction-parameter objects for all detectors
  EventSpecie_t fEventSpecie;               // current event specie
  static TString fkgEventSpecieName[] ; // the names of the event species
  ClassDef(IlcRecoParam, 6)
};


#endif
