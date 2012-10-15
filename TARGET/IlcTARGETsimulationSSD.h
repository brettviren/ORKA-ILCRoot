#ifndef ILCTARGETSIMULATIONSSD_H
#define ILCTARGETSIMULATIONSSD_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice     */

/* $Id: IlcTARGETsimulationSSD.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $ */

/////////////////////////////////////////////////////////////
// Simulation class for SSD                                //
/////////////////////////////////////////////////////////////

#include <TH1.h>
#include "IlcTARGETsimulation.h"
#include "IlcTARGETsegmentationSSD.h" // function used in inline functions

class IlcTARGETMapA2;
class IlcTARGETpList;
class IlcTARGETTableSSD;
//class IlcTARGETdcsSSD;
class IlcTARGETsegmentationSSD;
class IlcTARGETCalibrationSSD;

class IlcTARGETsimulationSSD: public IlcTARGETsimulation {

 public:
    IlcTARGETsimulationSSD(); // Default constructor
    IlcTARGETsimulationSSD(const IlcTARGETsimulationSSD &source); // copy constructor
    // operator =
    IlcTARGETsimulationSSD& operator=(const IlcTARGETsimulationSSD &source);
    virtual IlcTARGETsimulation& operator=(const IlcTARGETsimulation &source);
    //Standard Constructor
    IlcTARGETsimulationSSD(IlcTARGETDetTypeSim* dettyp);
    //Destructor
    virtual ~IlcTARGETsimulationSSD();
    // Get a pointer to the segmentation object
    virtual IlcTARGETsegmentation2* GetSegmentationModel2(Int_t /*dt*/){return fDetType->GetSegmentationModel2(1);}
    // set pointer to segmentation objec
    virtual void SetSegmentationModel2(Int_t /*dt*/, IlcTARGETsegmentation2 *seg){fDetType->SetSegmentationModel2(1,seg);}
    // Initilize variables for this simulation
    void Init();
    // Initilize variables for this simulation
    //void Init(IlcTARGETsegmentationSSD *seg,IlcTARGETCalibrationSSD *resp);
    // Create maps to build the lists of tracks for each summable digit
    void InitSimulationModule(Int_t module,Int_t events);
    // Digitize module from the sum of summable digits.
    void FinishSDigitiseModule();
    //Digitizes all of the hits in a module
    void DigitiseModule(IlcTARGETmodule *mod,Int_t dummy0,Int_t dummy1);
    // Computes the Summable Digits
    void SDigitiseModule(IlcTARGETmodule *mod,Int_t module,Int_t dummy);
    // Computes the Charge on each Strip/ Analog/summable digits
    void HitsToAnalogDigits(IlcTARGETmodule *mod,IlcTARGETpList *pList);
    //Computes the signal from one hit
    void HitToDigit(Int_t lay,Int_t module,Double_t x0,Double_t y0,Double_t z0, 
		    Double_t x,Double_t y,Double_t z,Double_t de,
		    IlcTARGETTableSSD *tav);
    //returns a pointer to the SSD segmentation.
    /*IlcTARGETsegmentationSSD *GetSegmentation() {
	return (IlcTARGETsegmentationSSD*) fSegmentation;}
    */
    //Returns the ionization energy for Si in GeV.
    Double_t GetIonizeE() const {return fIonE;}

    //Sets the ionization energy for Si in GeV.
    void SetIonizeE(Double_t e=3.62E-09){fIonE = e;}
    //Returns the Diffusion constant h in cm**2/sec
    Double_t GetDiffConst(Int_t i) const {return fDifConst[i];}
    //Sets the Diffusion constant h in cm**2/sec
    void SetDiffConst(Double_t h=150.,Double_t e=300.)
	{fDifConst[0] = h;fDifConst[1]=e;}
    //Returns the Drift velocity for the side i
    Double_t GetDriftVelocity(Int_t i) const {return fDriftVel[i];}
    //Sets the Drift velocity for the P and N sides
    void SetDriftVelocity(Double_t v0=1.65E+06,Double_t v1=2.3E+06)
	{fDriftVel[0] = v0;fDriftVel[1] = v1;}
    // Standard ascii class print function
    void Print(ostream *os);
    // Standard ascii class read function
    void Read(istream *is);
    virtual void Print(Option_t *option="") const {TObject::Print(option);}
    virtual Int_t Read(const char *name) {return TObject::Read(name);}
    // Data members
 protected:

    //    IlcTARGETdcsSSD *fDCS;   // Class containing detector controle paramters

 private:
    // Return the Response class
    //    IlcTARGETCalibrationSSD* GetResp(){return (IlcTARGETCalibrationSSD*)fResponse;}
    // Return the Segmentation class
    //IlcTARGETsegmentationSSD* GetSeg(){
    //  return (IlcTARGETsegmentationSSD*)fSegmentation;}
    // returns the number of steps needed to proplerly distribute the charge
    // in a step
   
    Int_t NumOfSteps(Double_t x,Double_t y,Double_t z,
		     Double_t  &dex,Double_t &dey,Double_t &dez);
    // Keepts track and orders tracks for a give strip.
    void GetList(Int_t lay,Int_t trk,Int_t ht,Int_t mod,IlcTARGETpList *pLt,
		 IlcTARGETTableSSD *tav);
    // sets thresholds and fills digits
    void ChargeToSignal(Int_t lay,Int_t module,IlcTARGETpList *pList);
    // Writes Summable Digits to a root file for later use.
    void WriteSDigits(IlcTARGETpList *pList);
    // ReadSDigits and create Digits
    void SDigitToDigit(Int_t lay,Int_t module,IlcTARGETpList *pList);
    // Fills fMapA2 from pList IlcTARGETpList
    void FillMapFrompList(Int_t lay,IlcTARGETpList *pList);
    // Diffuses the charge onto neighboring strips.
    void    IntegrateGaussian(Int_t lay,Int_t k,Double_t par,Double_t av,Double_t sigma, 
			      Double_t inf, Double_t sup,
			      IlcTARGETTableSSD *tav);
     // Applies noise to strips randomly
    void    ApplyNoise(Int_t lay,IlcTARGETpList *pList,Int_t mod);
     // Applies posible signal coupling between strips
    void    ApplyCoupling(Int_t lay,IlcTARGETpList *pList,Int_t mod);
    // Kill dead channels
    void ApplyDeadChannels(Int_t mod);
    // Computes the integral of a gaussian using Error Function
    Float_t F(Float_t av, Float_t x, Float_t s);
    // returns, from the segmentation, the number of stips

    Int_t GetNStrips(Int_t lay) {IlcTARGETsegmentationSSD* seg = (IlcTARGETsegmentationSSD*)GetSegmentationModel2(1);return seg->Npx(lay);}
    // returns, from the segmentation, the strip pitch
    Float_t GetStripPitch(Int_t lay) {IlcTARGETsegmentationSSD* seg = (IlcTARGETsegmentationSSD*)GetSegmentationModel2(1);return seg->Dpx(lay,0);}


    IlcTARGETMapA2 *fMapA2;      //! Map of ionization, used localy only
    Double_t    fIonE;        // ionization energy of Si in GeV
    Double_t    fDifConst[2]; // Diffusion constants [h,e] in cm**2/sec
    Double_t    fDriftVel[2]; // Drift velocities [P,N sides] cm/sec

    ClassDef(IlcTARGETsimulationSSD,2) // SSD signal simulation class

};
// Input and output functions for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETsimulationSSD &source);
istream &operator>>(istream &is,IlcTARGETsimulationSSD &source);
#endif
