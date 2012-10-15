#ifndef ILCTARGETCILCBRATIONSSD_H
#define ILCTARGETCILCBRATIONSSD_H
 
#include "IlcTARGETCalibration.h"
#include "IlcTARGETresponseSSD.h"
#include "TArrayF.h"
#include "TArrayI.h"

//////////////////////////////////////////////
// Response class for SSD                   //
//                                          //
//////////////////////////////////////////////
class IlcTARGETCalibrationSSD : public IlcTARGETCalibration {

 public:
    IlcTARGETCalibrationSSD();
    IlcTARGETCalibrationSSD(const char *dataType);
    virtual ~IlcTARGETCalibrationSSD();

    virtual  void   SetNoiseParam(Double_t np, Double_t nn) {
    	// set noise par
    	fNoiseP=np; fNoiseN=nn;
    }

    virtual void    GetNoiseParam(Double_t &np, Double_t &nn) const {
    	// get noise par
    	np=fNoiseP; nn=fNoiseN;
    }

    // EF
    void SetNNoiseP(Int_t n) { fNoisP.Set(n); }
    void AddNoiseP(Int_t c, Float_t n) { fNoisP.AddAt(n,c);}       
    TArrayF GetNoiseP() const {return fNoisP; }
    Float_t GetNoiseP(Int_t n) {return fNoisP.At(n); }
    void SetNNoiseN(Int_t n) { fNoisN.Set(n); }
    void AddNoiseN(Int_t c, Float_t n) { fNoisN.AddAt(n,c);}
    TArrayF GetNoiseN() const {return fNoisN; }
    Float_t GetNoiseN(Int_t n) {return fNoisN.At(n); }

    void SetNGainP(Int_t n) { fGainP.Set(n); }
    void AddGainP(Int_t c, Float_t n) { fGainP.AddAt(n,c);}       
    TArrayF GetGainP() const {return fGainP; }
    Float_t GetGainP(Int_t n) {return fGainP.At(n); }
    void SetNGainN(Int_t n) { fGainN.Set(n); }
    void AddGainN(Int_t c, Float_t n) { fGainN.AddAt(n,c);}
    TArrayF GetGainN() const {return fGainN; }
    Float_t GetGainN(Int_t n) {return fGainN.At(n); }

    void SetNoisePThreshold(Int_t threshold) { fNoisePThreshold = threshold;}
    void AddNoisyPChannel(Int_t c, Int_t n) { fNoisyPChannelsList.AddAt(n,c);}
    TArrayI GetNoisyPChannelsList() const {return fNoisyPChannelsList; }
    void SetNoiseNThreshold(Int_t threshold) { fNoiseNThreshold = threshold;}
    void AddNoisyNChannel(Int_t c, Int_t n) { fNoisyNChannelsList.AddAt(n,c);}
    TArrayI GetNoisyNChannelsList() const {return fNoisyNChannelsList; }

    void SetNDeadPChannelsList(Int_t n) { fDeadPChannelsList.Set(n); }
    void AddDeadPChannel(Int_t c, Int_t n) { fDeadPChannelsList.AddAt(n,c);}
    TArrayI GetDeadPChannelsList() const {return fDeadPChannelsList; }
    void SetNDeadNChannelsList(Int_t n) { fDeadNChannelsList.Set(n); }
    void AddDeadNChannel(Int_t c, Int_t n) { fDeadNChannelsList.AddAt(n,c);}
    TArrayI GetDeadNChannelsList() const {return fDeadNChannelsList; }
    //


    virtual  void   SetNDetParam(Int_t npar) {
	// set number of param
	fNPar=npar;
    }
    virtual  void   SetDetParam(Double_t *par);

    // Parameters options
    virtual Int_t   NDetParam() const {
	// number of param
	return fNPar;
    }
    virtual void    GetDetParam(Double_t *dpar) const; 

    virtual void    SetSigmaSpread(Double_t p1, Double_t p2) {
    	// Set sigmas of the charge spread function: Pside-Nside
    	// square of (microns)
    	fSigmaP=p1; fSigmaN=p2;
    }

    virtual void    SigmaSpread(Double_t &sP, Double_t &sN) const {
      // Get sigmas for the charge spread 
      sP=fSigmaP; sN=fSigmaN;
    }

    virtual void   SetThresholds(Double_t /* a */, Double_t /* b */)
      {NotImplemented("SetThresholds");}
    virtual void   Thresholds(Double_t & /* a */, Double_t & /* b */) const 
      {NotImplemented("Thresholds");}
  
    virtual void SetParamOptions(const char *opt1, const char *opt2) {((IlcTARGETresponseSSD*)fResponse)->SetParamOptions(opt1,opt2);}
    virtual void GetParamOptions(char *opt1,char *opt2) const {((IlcTARGETresponseSSD*)fResponse)->ParamOptions(opt1,opt2);}
    virtual void SetADCpereV(Double_t a=200./30000.0) {((IlcTARGETresponseSSD*)fResponse)->SetADCpereV(a);}
    virtual Double_t GetDEvToADC(Double_t eV) const {return ((IlcTARGETresponseSSD*)fResponse)->DEvToADC(eV);}
    virtual Int_t IEvToADC(Double_t eV) const {return ((IlcTARGETresponseSSD*)fResponse)->IEvToADC(eV);} 

  virtual Double_t GetCouplingPR() const {return ((IlcTARGETresponseSSD*)fResponse)->GetCouplingPR();}
  virtual Double_t GetCouplingPL() const {return ((IlcTARGETresponseSSD*)fResponse)->GetCouplingPL();}
  virtual Double_t GetCouplingNR() const {return ((IlcTARGETresponseSSD*)fResponse)->GetCouplingNR();}
  virtual Double_t GetCouplingNL() const {return ((IlcTARGETresponseSSD*)fResponse)->GetCouplingNL();}
  virtual void SetCouplings(Double_t pr, Double_t pl, Double_t nr, Double_t nl) 
    { ((IlcTARGETresponseSSD*)fResponse)->SetCouplings(pr,pl,nr,nl);}

  virtual Int_t GetZSThreshold() const {return ((IlcTARGETresponseSSD*)fResponse)->GetZSThreshold();}
  virtual void SetZSThreshold(Int_t zsth) 
    { ((IlcTARGETresponseSSD*)fResponse)->SetZSThreshold(zsth);}

protected:
    static const Double_t fgkNoiseNDefault; // default for fNoiseN
    static const Double_t fgkNoisePDefault; // default for fNoiseP
    static const Int_t fgkNParDefault; // default for fNPar
    static const Double_t fgkSigmaPDefault; //default for fSigmaP
    static const Double_t fgkSigmaNDefault; //default for fSigmaP

    Int_t   fNPar;            // Number of detector param 
    Double_t *fDetPar;         //[fNPar] Array of parameters

    Double_t fNoiseP;          // Noise on Pside
    Double_t fNoiseN;          // Noise on Nside
    Double_t fSigmaP;          // Sigma charge spread on Pside
    Double_t fSigmaN;          // Sigma charge spread on Nside
    
    TArrayF fGainP;           // Gain for P side channels
    TArrayF fGainN;           // Gain for N side channels

    TArrayF fNoisP;           // Noise for P side channels
    TArrayF fNoisN;           // Noise for N side channels

    Float_t fNoisePThreshold;     // need to decide if channel is noisy  
    TArrayI  fNoisyPChannelsList; // list of P side noisy channels
    Float_t fNoiseNThreshold;     // need to decide if channel is noisy  
    TArrayI  fNoisyNChannelsList; // list of N side noisy channels

    TArrayI  fDeadNChannelsList;  // list of P side dead channels
    TArrayI  fDeadPChannelsList;  // list of N side dead channels

 private:
    IlcTARGETCalibrationSSD(const IlcTARGETCalibrationSSD &source); // copy constructor
    IlcTARGETCalibrationSSD& operator=(const IlcTARGETCalibrationSSD &source); // ass. op.

    ClassDef(IlcTARGETCalibrationSSD,1) //Response class for SSD
};
#endif
