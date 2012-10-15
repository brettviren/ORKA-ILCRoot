#ifndef ILCJETPRODUCTIONDATA_H
#define ILCJETPRODUCTIONDATA_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
 
 
//---------------------------------------------------------------------
// Service class for jet production data 
// Author: Andreas Morsch
// andreas.morsch@cern.ch
//---------------------------------------------------------------------

#include <TObject.h>
#include <TString.h>
 
class IlcJetProductionData : public TObject
{
 public:
    IlcJetProductionData();
    ~IlcJetProductionData();
    Int_t   GetNumberOfPtHardBins() const {return fNbins;}
    void    GetPtHardLimits(Int_t bin, Float_t& ptmin, Float_t& ptmax);
    TString GetRunTitle(Int_t bin);
    Float_t GetWeight(Int_t bin);
 protected:
    Int_t     fNbins;         // Number of pt_hard bins used in the production
    Float_t*  fPtHardLimits;  //[fNbins+1]
    Float_t*  fWeights;       //[fNbins]
    TString*  fRunTitles;     //[fNbins]
    
 private: 
    IlcJetProductionData(const IlcJetProductionData& rJetPD);
    IlcJetProductionData& operator = (const IlcJetProductionData& rjpd);

    ClassDef(IlcJetProductionData, 1)
};
 
#endif
