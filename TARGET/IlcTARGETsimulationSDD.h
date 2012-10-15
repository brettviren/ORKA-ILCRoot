#ifndef ILCTARGETSIMULATIONSDD_H
#define ILCTARGETSIMULATIONSDD_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice     */


/* $Id: IlcTARGETsimulationSDD.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */

////////////////////////////////////////////////////////////
// Simulation class for SDD                               //
////////////////////////////////////////////////////////////

#include <TNtuple.h>
#include "IlcTARGETsimulation.h"

class TH1F;
class TFile;
class TArrayI;
class TArrayF;
class IlcTARGET;
class IlcTARGETpList;
class IlcTARGETMap;
class IlcTARGETMapA1;
class IlcTARGETMapA2;
class IlcTARGETetfSDD;
class IlcTARGETsegmentationSDD;
class IlcTARGETInStream;
class IlcTARGETCalibration;
class IlcTARGETCalibrationSDD;

class IlcTARGETsimulationSDD : public IlcTARGETsimulation {
  public:
    IlcTARGETsimulationSDD(); // default constructor
    //Standard Constructor
    IlcTARGETsimulationSDD(IlcTARGETDetTypeSim* dettyp);
    // Copy operator
    IlcTARGETsimulationSDD(IlcTARGETsimulationSDD &source);
    virtual ~IlcTARGETsimulationSDD(); // Destructor
    // = operator
    IlcTARGETsimulationSDD& operator=(const IlcTARGETsimulationSDD &source);
    virtual IlcTARGETsimulation& operator=(const IlcTARGETsimulation &source);
    // Initilize variables for this simulation
    void Init();

    // Get a pointer to the segmentation object
    virtual IlcTARGETsegmentation2* GetSegmentationModel2(Int_t /*dt*/){return fDetType->GetSegmentationModel2(2);}
    // set pointer to segmentation object
    virtual void SetSegmentationModel2(Int_t /*dt*/, IlcTARGETsegmentation2 *seg){fDetType->SetSegmentationModel2(2,seg);}

    // set the scale size factor for the smples in FFT
    virtual void SetScaleFourier(Int_t scale=4) {fScaleSize=scale;}
    Int_t ScaleFourier() const {return fScaleSize;} // returns the scale factor
    // set perpendicular tracks flag
    virtual void SetPerpendTracksFlag(Bool_t flag=kFALSE) {fFlag=flag;}
    // returns perpendicular track flag.
    Bool_t PerpendTracksFlag() const {return fFlag;} 
    // set crosstalk flag
    virtual void SetCrosstalkFlag(Bool_t flag=kFALSE) {fCrosstalkFlag=flag;}
    // return crosstalk flag
    Bool_t CrosstalkFlag() const {return fCrosstalkFlag;}
    // retrieve compression parameters for 2D or 1D
    void CompressionParam(Int_t i, Int_t &db, Int_t &tl, Int_t &th) const;
    // retrieve compression parameters for 2D or 1D
    void CompressionParam(Int_t i, Int_t &db, Int_t &tl) const;

    virtual Int_t Convert10to8(Int_t signal) const;//10 to 8 bit SDD compresion
    virtual void ZeroSuppression(const char *opt); // Apply zero suppresion
    virtual void Init2D();   // initiilzes 2D compresion algorithm
    virtual void Compress2D(); // Applies 2D compresion algorithm
    virtual void Init1D();   // initilizes 1D compresion algorithm
    virtual void Compress1D(); // Applies 1D compresion algorithm
    virtual void StoreAllDigits(); // if No compresion run this.
    // returns baseline and noise for a given anode i.
    //virtual void GetAnodeBaseline(Int_t i,Double_t &baseline,Double_t &noise) const;
    // local implementation of TARGET->AddDigit. Specific for SDD
    virtual void AddDigit(Int_t i, Int_t j, Int_t signal);
    // Finds clulsters of signals. Use with regards to Compresion algorithms
    virtual void  FindCluster(Int_t i, Int_t j,Int_t signal,
                              Int_t minval,Bool_t &cond);

    // get parameters for 1D - this could be changed when we get more
    // input from Torino after they have a look at the code 
    virtual Int_t Tolerance(Int_t i) const {return fTol[i];}//returns tolerance
    virtual Int_t Disable(Int_t i)  const {return fT2[i];}//high threshold  2D
    // Set the output file name - for 1D encoding 
    virtual void SetFileName(const char *filnam) {fFileName=filnam;}

    // add baseline, noise, electronics and ADC saturation effects
    void ChargeToSignal(Int_t mod,Bool_t bAddNoise=kFALSE);
    // add dead channels
    void ApplyDeadChannels(Int_t mod);
    // add crosstalk effect
    void ApplyCrosstalk(Int_t mod);
    
    // create maps to build the lists of tracks for each summable digit
    void InitSimulationModule( Int_t module, Int_t event );
    // clear maps
    void ClearMaps();
    // Summable Digitses a SDD module
    void SDigitiseModule(IlcTARGETmodule *mod,Int_t md,Int_t ev);
    // Add Summable digits to module maps.
    Bool_t AddSDigitsToModule( TClonesArray *pItemArray, Int_t mask );
    // digitize module from the sum of summable digits.
    void FinishSDigitiseModule();
    // Writes summable digits
    void WriteSDigits();
    // Introduces electronics effects and does zero-suppresion if required
    void FinishDigits();
    // Digitses a SDD module
    void DigitiseModule(IlcTARGETmodule *mod,Int_t md,Int_t ev);
    // Spread charge in a SDD module
    void HitsToAnalogDigits(IlcTARGETmodule *mod);
    // Sorts tracks for the 3 most highly contributed one to be added to digit.
    //void SortTracks(Int_t *tracks,Float_t *charges,Int_t *hits
    //                Int_t ntracks);
    // collects and returns the fired SDD cells (uses IlcTARGETMapA2...).
    //void ListOfFiredCells(Int_t *arg,Double_t timeAmplitude,TObjArray *list,
    //		  TClonesArray *padr);

    // Creates histograms of maps for debugging
    void CreateHistograms(Int_t scale);
    // Fills histograms of maps for debugging
    void FillHistograms();
    // Resets histograms of maps for debugging
    void ResetHistograms();
    // Get the pointer to the array of histograms
    TObjArray*  GetHistArray() {return fHis;}
    // create a separate tree for background monitoring (2D) 
    virtual  void  MakeTreeB(Option_t *option="B") 
        { if(strstr(option,"B"))
            fTreeB = new TNtuple("ntuple", "2D backgr","nz:nl:nh:low:anode");}
    // presently a dummy routine use TreeB() instead
    void GetTreeB(Int_t) { }
    // Return pointer to TreeB
    TNtuple *TreeB() {return fTreeB;}
    void WriteToFile(TFile *fp);// Writes the histograms to a file
    // Get's histogram of a particular anode.
    TH1F *GetAnode(Int_t wing, Int_t anode);

    // Sets the check noise flag.
    void SetCheckNoise(Bool_t check=kFALSE) {fCheckNoise=check;}
    // Returns the noise value
    Float_t GetNoise();
    // sets DoFFT value.
    void SetDoFFT(Int_t doFFT=1) {fDoFFT=doFFT;}

    // Print SSD simulation Parameters
    virtual void PrintStatus() const;

  private:
    // virtual void GetBaseline(Int_t mod);  // read baseline values from a file
   // set compression parameters for 2D or 1D via response functions
    void SetCompressParam();
    // Variables and pointers for local use only. Not Streamed out.
    IlcTARGET         *fTARGET;          //! local pointer to TARGET
    IlcTARGETMapA2    *fHitMap2;      //! local pointer to map of signals
    IlcTARGETMapA2    *fHitSigMap2;   //! local pointer to map of signals
    IlcTARGETMapA2    *fHitNoiMap2;   //! local pointer to map of signals
    IlcTARGETInStream *fStream;       //! input file stream
    IlcTARGETetfSDD   *fElectronics;  //! local pointer to electronics simulation
    Double_t       *fInZR;         //! [fScaleSize*fMaxNofSamples] input of the
                                   // real part of FFT
    Double_t       *fInZI;         //! [fScaleSize*fMaxNofSamples] 
                                   // input of the imaginary part of FFT
    Double_t       *fOutZR;        //! [fScaleSize*fMaxNofSamples] 
                                   // output of the real part of FFT
    Double_t       *fOutZI;        //! [fScaleSize*fMaxNofSamples] 
                                   // output of the imaginary part of FFT
    Bool_t         *fAnodeFire;     //! [#of anodes] Flag if there is a signal

    TObjArray *fHis;          // just in case for histogramming
    TArrayI    fD;            // decrease values for baseline eq.
    TArrayI    fT1;           // low thresholds
    TArrayI    fT2;           // high thresholds(2D) or disable (1D) 
    TArrayI    fTol;          // tolerance
    // TArrayF    fBaseline;     // Baseline
    //TArrayF    fNoise;        // Noise value
    TNtuple   *fTreeB;        // Background info tree for 2D
    TString    fParam;        // Compresion algorithm options
    TString    fFileName;     // File name for possible options above
    Bool_t     fFlag;         // Flag used to simulate perpendicular tracks
    Bool_t     fCheckNoise;   // Flag used to check the simulated noise
    Bool_t     fCrosstalkFlag; // Flag used to apply the crosstalk effect
    Int_t      fDoFFT;        // Flag used to switch off electronics when 0
    Int_t      fNofMaps;      // Number of anodes used ( 1-2*nanodes per wing )
    Int_t      fMaxNofSamples;// Number of time samples
    Int_t      fScaleSize;    // scale size factor for the samples in FFT

    ClassDef(IlcTARGETsimulationSDD,2)  // Simulation of SDD clusters

};
#endif
