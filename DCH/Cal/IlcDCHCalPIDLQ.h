#ifndef ILCDCHCALPIDLQ_H
#define ILCDCHCALPIDLQ_H 
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */ 
/*----------------------------------------------------------------- 
   Class for dE/dx and Time Bin of Max. Cluster for Electrons and  
   pions in DCH.  
   It is instantiated in class IlcDCHpidESD for particle identification 
   in DCH 
   Prashant Shukla <shukla@pi0.physi.uni-heidelberg.de> 
   -----------------------------------------------------------------*/ 
 
#include <TNamed.h> 
#include <IlcPID.h> 
 
class TH1F; 
class TObjArray; 
 
class IlcDCHCalPIDLQ : public TNamed { 
  public: 
    IlcDCHCalPIDLQ();  
    IlcDCHCalPIDLQ(const Text_t *name, const Text_t *title); 
     
    IlcDCHCalPIDLQ(const IlcDCHCalPIDLQ& pd);  // Copy Constructor 
    virtual ~IlcDCHCalPIDLQ();               // Destructor 
     
    IlcDCHCalPIDLQ &operator=(const IlcDCHCalPIDLQ &c); 
    virtual void Copy(TObject &c) const; 
 
    Double_t GetMeanChargeRatio() const { return fMeanChargeRatio; }  
 
    Double_t GetMomentum(Int_t ip) const {return fTrackMomentum[ip];} 
                      // Gets the momentum for given histogram number ip 
    Double_t GetMean(Int_t iType, Int_t ip) const;         
                      // Particle type is iType and histogram number is ip          
    Double_t GetNormilczation(Int_t iType, Int_t ip) const; 
 
    TH1F* GetHistogram(Int_t iType, Int_t ip) const; 
 
    Double_t GetProbability(Int_t iType, Double_t mom, Double_t dedx) const; 
                      // Gets the Probability of having dedx 
    Double_t GetProbabilityT(Int_t iType, Double_t mom, Int_t timbin) const; 
                      // Gets the Probability of having timbin 
    Int_t GetNbins() const {return fNbins;}         // Number of Energy bins 
    Double_t GetBinSize() const {return fBinSize;}  // Size of Energy bin 
 
    Bool_t ReadData(Char_t *responseFile);       // Read histograms 
                      // Update the histograms from responseFile 
    void SetMeanChargeRatio(Double_t ratio) { fMeanChargeRatio = ratio; }   
 
  protected: 
    void Init();                // Reset data 
    void CleanUp();             // Delete pointers; 
    inline Int_t GetHistID(Int_t particle, Int_t mom) const { return particle*fNMom + mom; } 
     
    static Char_t *fpartName[IlcPID::kSPECIES]; //! Names of particle species 
     
    Int_t fNMom;                // Number of momenta   
    Double_t* fTrackMomentum;   //[fNMom] Track momenta for which response functions are available 
    Double_t fMeanChargeRatio;  // Ratio of mean charge from real Det. to prob. dist. 
 
    Int_t fNbins;               // Number of Energy bins 
    Double_t fBinSize;          // Size of Energy bin 
     
    TObjArray *fHistdEdx;           //-> Prob. of dEdx for 5 particles (e, pi, muon, kaon, proton) and for several momenta 
    TObjArray *fHistTimeBin;        //-> Prob. of max time bin for 5 particles (e, pi, muon, kaon, proton) and for several momenta 
     
    ClassDef(IlcDCHCalPIDLQ, 1) 
}; 
 
 
#endif 
 
