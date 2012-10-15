#ifndef ILCTARGETCILCBRATIONSDD_H
#define ILCTARGETCILCBRATIONSDD_H
 
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */
 

#include "IlcTARGETCalibration.h"
#include "IlcTARGETresponseSDD.h"
#include "TArrayI.h"

class IlcTARGETresponseSDD;
///////////////////////////////////////////////////////
//  Response for SDD                                 //
///////////////////////////////////////////////////////

class IlcTARGETCalibrationSDD : public IlcTARGETCalibration {
  public:
    //
    // Configuration methods
    //
    IlcTARGETCalibrationSDD();
    IlcTARGETCalibrationSDD(const char *dataType);
    virtual ~IlcTARGETCalibrationSDD() {;}
    virtual void  SetNoiseParam(Double_t /*n*/, Double_t /*b*/){
      NotImplemented("SetNoiseParam");}
 
    virtual void  GetNoiseParam(Double_t &/*n*/, Double_t &/*b*/) const {
      NotImplemented("GetNoiseParam");}

    virtual Double_t GetBaseline(Int_t anode) const {return fBaseline[anode];}
    virtual void SetBaseline(Int_t anode,Double_t bas) {fBaseline[anode]=bas;}
    virtual Double_t GetNoise(Int_t anode) const {return fNoise[anode];}
    virtual void SetNoise(Int_t anode, Double_t noise) {fNoise[anode]=noise;}

    virtual void   SetThresholds(Double_t  mv, Double_t /* b */){
       // Min value used in 2D - could be used as a threshold setting
	fMinVal = mv;}
    virtual void   Thresholds(Double_t &  mv, Double_t & /* b */) const 
      {mv = fMinVal;}
    virtual void  GiveCompressParam(Int_t *x,Int_t ian) const;

    void  SetNoiseAfterElectronics(Int_t anode,Double_t n=2.38){
	// Noise after electronics (ADC units)
	// 2.36 for ILC from beam test measurements 2001
	fNoiseAfterEl[anode]=n;}
    Double_t  GetNoiseAfterElectronics(Int_t anode) const {
	// Noise after electronics (ADC units)
	return fNoiseAfterEl[anode];} 
    //void SetDeadChannels(Int_t nchips=0, Int_t nchannels=0);
    void SetDeadChannels(Int_t ndead=0){fDeadChannels=ndead; fBadChannels.Set(ndead);}
    Int_t GetDeadChips() const { return fDeadChips; }
    Int_t GetDeadChannels() const { return fDeadChannels; }
    Double_t Gain(Int_t wing,Int_t chip,Int_t ch)const 
        {return fGain[wing][chip][ch]; }
    virtual void SetGain(Double_t g,Int_t wing,Int_t chip, Int_t ch) 
      {fGain[wing][chip][ch]=g;}
    
    void    PrintGains() const;
    void    Print();
    virtual void Print(ostream *os) const {IlcTARGETCalibrationSDD::Print(os);}
    virtual void Print(Option_t *option="") const {IlcTARGETCalibrationSDD::Print(option);}
    // not implemented virtual methods (devlared in the mother class
    virtual  void   SetDetParam(Double_t *) 
      {NotImplemented("SetDetParam");}
    virtual void   GetDetParam(Double_t *) const 
      {NotImplemented("GetDetParam");}
    virtual  void   SetNDetParam(Int_t /* n */)
      {NotImplemented("SetNDetParam");}
    virtual Int_t  NDetParam() const
      {NotImplemented("NDetParam"); return 0;}
    virtual void    SetSigmaSpread(Double_t, Double_t) 
      {NotImplemented("SetSigmaSpread");}
    virtual void    SigmaSpread(Double_t & /* p1 */,Double_t & /* p2 */) const 
      {NotImplemented("SigmaSpread");}

    void   SetDead() { fIsDead = kTRUE; };
    Bool_t IsDead() const { return fIsDead; };
    Int_t Wings()const{return fgkWings;}//Total number of SDD wings
    Int_t Chips() const{return fgkChips;} // Number of chips/module
    Int_t Channels() const{ return fgkChannels;}//Number of channels/chip
    
    virtual void SetBadChannel(Int_t i,Int_t anode);
    Int_t GetBadChannel(Int_t i) const {return fBadChannels[i];}
    Bool_t IsBadChannel(Int_t anode); 

    Float_t GetMapACell(Int_t i,Int_t j) const {return fMapA[i][j];}
    virtual void SetMapACell(Int_t i,Int_t j,Float_t dev) {fMapA[i][j]=dev;} 
    Float_t GetMapTCell(Int_t i,Int_t j) const {return fMapT[i][j];}
    virtual void SetMapTCell(Int_t i,Int_t j,Float_t dev) {fMapT[i][j]=dev;} 
    static Int_t GetMapTimeNBin() {return fgkMapTimeNBin;} 

    virtual void SetElectronics(Int_t p1=1) {((IlcTARGETresponseSDD*)fResponse)->SetElectronics(p1);}
    virtual Int_t GetElectronics() const {return ((IlcTARGETresponseSDD*)fResponse)->Electronics();}
    virtual void SetMaxAdc(Double_t p1) {((IlcTARGETresponseSDD*)fResponse)->SetMaxAdc(p1);}
    virtual Double_t GetMaxAdc() const {return ((IlcTARGETresponseSDD*)fResponse)->MaxAdc();} 
    virtual void SetChargeLoss(Double_t p1) {((IlcTARGETresponseSDD*)fResponse)->SetChargeLoss(p1);}
    virtual Double_t GetChargeLoss() const {return ((IlcTARGETresponseSDD*)fResponse)->ChargeLoss();}
    virtual void SetDynamicRange(Double_t p1) {((IlcTARGETresponseSDD*)fResponse)->SetDynamicRange(p1);}
    virtual Double_t GetDynamicRange() const {return ((IlcTARGETresponseSDD*)fResponse)->DynamicRange();} 
    virtual void SetDriftSpeed(Double_t p1) {((IlcTARGETresponseSDD*)fResponse)->SetDriftSpeed(p1);}
    virtual Double_t GetDriftSpeed() const {return ((IlcTARGETresponseSDD*)fResponse)->DriftSpeed();}
    virtual void SetParamOptions(const char *opt1,const char *opt2) {((IlcTARGETresponseSDD*)fResponse)->SetParamOptions(opt1,opt2);}
    virtual void GetParamOptions(char *opt1,char *opt2) const {((IlcTARGETresponseSDD*)fResponse)->ParamOptions(opt1,opt2);}
    virtual Bool_t Do10to8() const {return ((IlcTARGETresponseSDD*)fResponse)->Do10to8();}
    virtual void SetZeroSupp (const char *opt) {((IlcTARGETresponseSDD*)fResponse)->SetZeroSupp(opt);} 
    virtual const char *GetZeroSuppOption() const {return ((IlcTARGETresponseSDD*)fResponse)->ZeroSuppOption();}
    virtual void SetNSigmaIntegration(Double_t p1) {((IlcTARGETresponseSDD*)fResponse)->SetNSigmaIntegration(p1);}
    virtual Double_t GetNSigmaIntegration() const {return ((IlcTARGETresponseSDD*)fResponse)->NSigmaIntegration();}
    virtual void SetNLookUp(Int_t p1) {((IlcTARGETresponseSDD*)fResponse)->SetNLookUp(p1);}
    virtual Int_t GetGausNLookUp() const {return ((IlcTARGETresponseSDD*)fResponse)->GausNLookUp();}
    virtual Double_t GetGausLookUp(Int_t i) const {return ((IlcTARGETresponseSDD*)fResponse)->GausLookUp(i);}
    virtual Int_t Convert8to10(Int_t signal) const {return ((IlcTARGETresponseSDD*)fResponse)->Convert8to10(signal);}
    virtual void  SetJitterError(Double_t jitter=20) {((IlcTARGETresponseSDD*)fResponse)->SetJitterError(jitter);}
    virtual Double_t GetJitterError() const {return ((IlcTARGETresponseSDD*)fResponse)->JitterError();}
    virtual void  SetDo10to8(Bool_t bitcomp=kTRUE) {((IlcTARGETresponseSDD*)fResponse)->SetDo10to8(bitcomp);}
 protected:


    // these statis const should be move to IlcTARGETsegmentationSDD
    static const Int_t fgkWings = 2;     // Number of wings per module
    static const Int_t fgkChips = 4;        // Number of chips/module
    static const Int_t fgkChannels = 64;    // Number of channels/chip
    static const Double_t fgkTemperatureDefault; // default for fT (Kelvin)
    static const Double_t fgkNoiseDefault; // default for fNoise
    static const Double_t fgkBaselineDefault; // default for fBaseline
    static const Double_t fgkMinValDefault; // default for fMinVal
    static const Double_t fgkGainDefault; //default for gain
    static const Int_t fgkMapTimeNBin = 72; //map granularity along drift direction
    Int_t fDeadChips;                     // Number of dead chips
    Int_t fDeadChannels;                  // Number of dead channels
    Double_t fGain[fgkWings][fgkChips][fgkChannels];//Array for channel gains
    Double_t fNoise[fgkWings*fgkChips*fgkChannels];          // Noise array
    Double_t fBaseline[fgkWings*fgkChips*fgkChannels];       // Baseline array
    Double_t fNoiseAfterEl[fgkWings*fgkChips*fgkChannels];   // Noise after electronics
    Double_t fMinVal;        // Min value used in 2D zero-suppression algo

    Bool_t   fIsDead;  // module is dead or ilcve ?
    TArrayI  fBadChannels; //Array with bad anodes number (0-512) 
    Float_t fMapA[fgkChips*fgkChannels][fgkMapTimeNBin]; //array with deviations on anode coordinate
    Float_t fMapT[fgkChips*fgkChannels][fgkMapTimeNBin]; //array with deviations on time coordinate

 private:
    IlcTARGETCalibrationSDD(const IlcTARGETCalibrationSDD &ob); // copy constructor
    IlcTARGETCalibrationSDD& operator=(const IlcTARGETCalibrationSDD & /* source */); // ass. op.


    ClassDef(IlcTARGETCalibrationSDD,4) // SDD response 
    
    };
#endif
