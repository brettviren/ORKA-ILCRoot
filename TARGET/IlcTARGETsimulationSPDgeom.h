#ifndef ILCTARGETSIMULATIONSPD_H
#define ILCTARGETSIMULATIONSPD_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/*
$Id: IlcTARGETsimulationSPDgeom.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $
*/
////////////////////////////////////////////////////////////////////////
// Version: 0                                                         //
// Written by Rocco Caliandro                                         //
// from a model developed with T. Virgili and R.A. Fini               //
// June 15 2000                                                       //
//                                                                    //
// IlcTARGETsimulationSPD is the simulation of SPDs                      //
////////////////////////////////////////////////////////////////////////

#include "IlcTARGETCalibrationSPD.h"
#include "IlcTARGETsegmentationSPD.h"
#include "IlcTARGETsimulation.h"
#include "IlcTARGETresponseSPD.h"

class IlcTARGETMapA2;
class IlcTARGETpList;
class IlcTARGETmodule;

//-------------------------------------------------------------------

class IlcTARGETsimulationSPD : public IlcTARGETsimulation {

 public:        
    IlcTARGETsimulationSPD(); // Default constructor
    // Standard constructor
    IlcTARGETsimulationSPD(IlcTARGETDetTypeSim *dettyp);
    virtual ~IlcTARGETsimulationSPD();// destructor
    IlcTARGETsimulationSPD(const IlcTARGETsimulationSPD &source); // copy constructo
    // assignment operator
    IlcTARGETsimulationSPD& operator=(const IlcTARGETsimulationSPD &source);
    virtual IlcTARGETsimulation& operator=(const IlcTARGETsimulation &source);
    // Get a pointer to the segmentation object
    virtual IlcTARGETsegmentation* GetSegmentationModel(Int_t /*dt*/){return fDetType->GetSegmentationModel(0);}
    // set pointer to segmentation objec
    virtual void SetSegmentationModel(Int_t /*dt*/, IlcTARGETsegmentation *seg){fDetType->SetSegmentationModel(0,seg);}
    // Initilizes the variables
    void Init();
    // Initilizes the variables with replacement segmentation/response class
    //    void Init(IlcTARGETsegmentationSPD *seg, IlcTARGETCalibrationSPD *resp);

    // Sum digitize module
    // Create maps to build the lists of tracks for each summable digit
    void InitSimulationModule(Int_t module,Int_t events);
    // Digitize module from the sum of summable digits.
    void FinishSDigitiseModule();
    void SDigitiseModule(IlcTARGETmodule *mod, Int_t dummy0,Int_t dummy1);
    // digitize module. Also need to digitize modules with only noise.
    void DigitiseModule(IlcTARGETmodule *mod,Int_t dummy0, Int_t dummy1);
    // sum digits to Digits.
    void SDigitsToDigits(Int_t module,IlcTARGETpList *pList);
    // updates the Map of signal, adding the energy  (ene) released by
    // the current track
    void UpdateMapSignal(Int_t row,Int_t col,Int_t trk,Int_t hit,Int_t mod,
			 Double_t ene,IlcTARGETpList *pList);
    // updates the Map of noise, adding the energy  (ene) give my noise
    void UpdateMapNoise(Int_t row,Int_t col,Int_t mod,Double_t ene,
			IlcTARGETpList *pList);
    // Loops over all hits to produce Analog/floting point digits. This
    // is also the first task in producing standard digits.
    void HitsToAnalogDigits(IlcTARGETmodule *mod,Int_t *frowpixel,
			    Int_t *fcolpixel,Double_t *fenepixel,
			    IlcTARGETpList *pList);
    //  Steering function to determine the digits associated to a given
    // hit (hitpos)
    // The digits are created by charge sharing (ChargeSharing) and by
    // capacitive coupling (SetCoupling). At all the created digits is
    // associated the track number of the hit (ntrack)
    void HitToDigit(IlcTARGETmodule *mod, Int_t hitpos,Int_t *frowpixel,
		    Int_t *fcolpixel, Double_t *fenepixel,IlcTARGETpList *pList);
    //  Take into account the geometrical charge sharing when the track
    //  crosses more than one pixel.
    void ChargeSharing(Float_t x1l,Float_t z1l,Float_t x2l,Float_t z2l,
		       Int_t c1,Int_t r1,Int_t c2,Int_t r2,Float_t etot,
		       Int_t &npixel,Int_t *frowpixel,Int_t *fcolpixel,
		       Double_t *fenepixel);
    //  Take into account the coupling between adiacent pixels.
    //  The parameters probcol and probrow are the fractions of the
    //  signal in one pixel shared in the two adjacent pixels along
    //  the column and row direction, respectively. Now done in a statistical
    //  way and not "mechanical" as in the Old version.
    void SetCoupling(Int_t row,Int_t col,Int_t ntrack,Int_t idhit,Int_t module,
		     IlcTARGETpList *pList);
    //  Take into account the coupling between adiacent pixels.
    //  The parameters probcol and probrow are the fractions of the
    //  signal in one pixel shared in the two adjacent pixels along
    //  the column and row direction, respectively.
    void SetCouplingOld(Int_t row,Int_t col,Int_t ntrack,Int_t idhit,
			Int_t module,IlcTARGETpList *pList);
    // The pixels are fired if the energy deposited inside them is above
    // the threshold parameter ethr. Fired pixed are interpreted as digits
    // and stored in the file digitfilename. One also needs to write out
    // cases when there is only noise (nhits==0).
    void CreateDigit(Int_t module,IlcTARGETpList *pList);
    //  Set the electronic noise and threshold non-uniformities to all the
    //  pixels in a detector.
    //  The parameter fSigma is the squared sum of the sigma due to noise
    //  and the sigma of the threshold distribution among pixels.
    void SetFluctuations(IlcTARGETpList *pList,Int_t module);
    //  Apply a mask to the SPD module. 1% of the pixel channels are
    //  masked. When the database will be ready, the masked pixels
    //  should be read from it.
    void SetMask(Int_t mod);
    // Create Histograms
    void CreateHistograms();
    // Reset histograms for this detector
    void ResetHistograms();
    // Fills the Summable digits Tree
    void WriteSDigits(IlcTARGETpList *pList);
    // Fills fMap2A from the pList of Summable digits
    void FillMapFrompList(IlcTARGETpList *pList);
    // get hist array
    TObjArray*  GetHistArray() {return fHis;}

 private:
    // Getters for data kept in fSegmentation and fResponse.
    // Returns the Threshold in electrons
    Double_t GetThreshold(){Double_t a=0.0,b=0.0;
    IlcTARGETCalibrationSPD* res = (IlcTARGETCalibrationSPD*)GetCalibrationModel(GetModuleNumber());
    res->Thresholds(a,b); return a;}
    // Returns the threshold and rms noise.
    void GetThresholds(Double_t &t,Double_t &s){
      IlcTARGETCalibrationSPD* res = (IlcTARGETCalibrationSPD*)GetCalibrationModel(GetModuleNumber());
      res->Thresholds(t,s);}
    // Returns the couplings Columb and Row.
    void GetCouplings(Double_t &cc,Double_t &cr){
      IlcTARGETCalibrationSPD* res = (IlcTARGETCalibrationSPD*)GetCalibrationModel(GetModuleNumber());
      res->GetCouplingParam(cc,cr);}
    // Returns the number of pixels in x
    Int_t GetNPixelsX(){IlcTARGETsegmentationSPD* seg= (IlcTARGETsegmentationSPD*)GetSegmentationModel(0);return seg->Npx();}
    // Returns the number of pixels in z
    Int_t GetNPixelsZ(){IlcTARGETsegmentationSPD* seg= (IlcTARGETsegmentationSPD*)GetSegmentationModel(0);return seg->Npz();}

 private:
    IlcTARGETMapA2  *fMapA2;   //! MapA2 for Local internal use only
    TObjArray    *fHis;     //! just in case for histogramming for Local
                            // internal use only

    ClassDef(IlcTARGETsimulationSPD,1)  // Simulation of SPD clusters

};

#endif
