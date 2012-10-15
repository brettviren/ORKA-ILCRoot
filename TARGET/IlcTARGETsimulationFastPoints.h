#ifndef ILCTARGETSIMULATIONFASTPOINTS_H
#define ILCTARGETSIMULATIONFASTPOINTS_H

#include "IlcTARGETsimulation.h"
#include <TArrayD.h>
//////////////////////////////////////////////////////////
// implements fast simulation 
/////////////////////////////////////////////////////////
//
class TClonesArray;
class IlcTARGETmodule;
class TRandom;

class IlcTARGETsimulationFastPoints : public IlcTARGETsimulation
{

public:
  IlcTARGETsimulationFastPoints(); // default constructor
  virtual ~IlcTARGETsimulationFastPoints() {;} 
  Bool_t CreateFastRecPoints(IlcTARGETmodule *mod,Int_t module,TRandom *rndm, 
			   TClonesArray* recp);
  void CreateFastRecPoints(Int_t module,TClonesArray* recp);

  virtual void SetSegmentationModel2(Int_t dt, IlcTARGETsegmentation2 *seg){fDetType->SetSegmentationModel2(dt,seg);}
  virtual IlcTARGETsegmentation2* GetSegmentationModel2(Int_t dt){return fDetType->GetSegmentationModel2(dt);}
  void SetExactPoints(bool exact=true) {fExactPoints=exact;}
  
private:

  virtual IlcTARGETsimulation& operator=(const IlcTARGETsimulation &)
    {return *this;} 
  
    virtual void SetSigmaRPhi(Int_t lay,Double_t srphi);  
    virtual void SetSigmaZ(Int_t lay,Double_t sz);  
    virtual void SetSigmaDe(Int_t lay,Double_t sde);  
    virtual void SetThrDe(Int_t lay,Double_t thrde); 
    Double_t SigmaRPhi(Int_t layer) const {return fSigmaRPhi[layer-1];}  
    Double_t SigmaZ(Int_t layer) const  {return fSigmaZ[layer-1];}  
    Double_t SigmaDe(Int_t layer) const {return fSigmaDe[layer-1];} 
    Double_t ThrDe(Int_t layer) const {return fThrDe[layer-1];} 


private:
    bool     fExactPoints;               // Without pixel resolution
    TArrayD fSigmaRPhi;              // Sigmas in rphi for the 6 layers
    TArrayD fSigmaZ;                 // Sigmas in Z for the 6 layers
    TArrayD fSigmaDe;                // Sigmas in energy loss for the 6 layers
    TArrayD fThrDe;                  // Energy thresholds for the 6 layers
    Double_t fHitx1;
    Double_t fHity1;
    Double_t fHitz1;


  ClassDef(IlcTARGETsimulationFastPoints,1) // Fast point simulator.

};

#endif
