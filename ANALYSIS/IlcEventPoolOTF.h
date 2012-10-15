#ifndef ILCEVENTPOOLOTF_H
#define ILCEVENTPOOLOTF_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

// Realisation of an IlcVEventPool via
// on the flight (OTF) generation of the bin using IlcTagAnalysis.
// Author Andreas Morsch
// andreas.morsch@cern.ch

#include <IlcVEventPool.h>
class IlcRunTagCuts;
class IlcLHCTagCuts;
class IlcDetectorTagCuts;
class IlcEventTagCuts;
class IlcTagAnalysis;
class TGridResult;

typedef enum {kMultiplicity, kZVertex, kEventPlane, kLeadingParticleEta, kLeadingParticlePhi, kUser1, kUser2}  EventPoolAxis_t;

class IlcEventPoolOTF : public IlcVEventPool
{
 public:
    IlcEventPoolOTF();
    IlcEventPoolOTF(const char* name, const char* title = "AOD");

    virtual ~IlcEventPoolOTF();
    // Interface
    virtual TChain* GetNextChain();
    virtual void  GetCurrentBin(Float_t* /*bin*/);
    virtual Int_t GetDimension();
    virtual void  Init();
    virtual void  SetMultiplicityBin(Int_t min, Int_t max, Int_t step)
	{fValueMin[kMultiplicity] = Float_t(min); fValueMax[kMultiplicity] = Float_t(max); fValueStep[kMultiplicity] = Float_t(step);}

    virtual void  SetMultiplicityBinning(Float_t min, Float_t max, Float_t step)
	{fValueMin[kMultiplicity] = min; fValueMax[kMultiplicity] = max; fValueStep[kMultiplicity] = step;}
    virtual void  SetZVertexBinning(Float_t min, Float_t max, Float_t step)
	{fValueMin[kZVertex] = min; fValueMax[kZVertex] = max; fValueStep[kZVertex] = step;}
    virtual void  SetEventPlaneBinning(Float_t min, Float_t max, Float_t step)
	{fValueMin[kEventPlane] = min; fValueMax[kEventPlane] = max; fValueStep[kEventPlane] = step;}
    virtual void  SetLeadingParticleEtaBinning(Float_t min, Float_t max, Float_t step)
	{fValueMin[kLeadingParticleEta] = min; fValueMax[kLeadingParticleEta] = max; fValueStep[kLeadingParticleEta] = step;}
  virtual void  SetLeadingParticlePhiBinning(Float_t min, Float_t max, Float_t step)
	{fValueMin[kLeadingParticlePhi] = min; fValueMax[kLeadingParticlePhi] = max; fValueStep[kLeadingParticlePhi] = step;}

    //
    void SetTagDirectory(const char* dirname) {fTagDirectory = dirname;};
    void SetGridResult(TGridResult*  gridTag) {fGridTags = gridTag;};
    virtual Int_t BinNumber() const {return fBinNumber;}
	    
 private:
    IlcEventPoolOTF(const IlcEventPoolOTF& obj);
    IlcEventPoolOTF& operator=(const IlcEventPoolOTF& other);
    void InitArrays();
    
 protected:
    IlcTagAnalysis*      fTagAnalysis;   // Pointer to tag analysis
    IlcRunTagCuts*       fRunCuts;       // Run      cuts
    IlcLHCTagCuts*       fLHCCuts;       // LHC      cuts
    IlcDetectorTagCuts*  fDetectorCuts;  // Detector cuts
    IlcEventTagCuts*     fEventCuts;     // Event    cuts
    TGridResult*         fGridTags;      // Tags from a grid file collection
    TChain*              fChain;         // Current chain
    const char*          fTagDirectory;  // Directory with local tag files
    // Common pool cuts
    // Multiplicity
    Float_t              fValueMin[10];  // Minimum value
    Float_t              fValueMax[10];  // Maximum value
    Float_t              fValueStep[10]; // Step size
    Float_t              fValue[10];     // Current value
    //
    Int_t                fBinNumber;    // Current bin number
    Bool_t               fNoMore;       // No more bins 
    
    ClassDef(IlcEventPoolOTF, 0); 
};
 
#endif
