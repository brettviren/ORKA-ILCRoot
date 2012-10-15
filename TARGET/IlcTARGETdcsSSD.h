#ifndef ILCTARGETDCSSSD_H
#define ILCTARGETDCSSSD_H

/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//  Class IlcTARGETdcsSSD                                                   //
//  describes Detector Control System parameters for one SSD module.     //
//                                                                       //
//  This class stores parametrers such as gain, threshold                //
//  capacitive coupling.                                                 //
//                                                                       //
//  Class takes care of invilcd strip menagement during                  //
//  simulation and runtime                                               //
//                                                                       //
//                                                                       //
//  created at: Warsaw University of Technology                          //
//  ver. 1.0    WARSAW, 23.12.1999                                       //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include <TObject.h>

class TArrayS;
class TRandom;
class IlcTARGETsegmentation;
class IlcTARGETCalibration;

class IlcTARGETdcsSSD: public TObject {

 public:
    IlcTARGETdcsSSD(); // Default constructor
    // Standard constructor
    IlcTARGETdcsSSD(IlcTARGETsegmentation *s, IlcTARGETCalibration *r); 
    virtual ~IlcTARGETdcsSSD(); // Destructor
    IlcTARGETdcsSSD(const IlcTARGETdcsSSD &source); // copy constructor
    IlcTARGETdcsSSD& operator=(const IlcTARGETdcsSSD &source); // assignment operator
    //________________________________________________________________
    //	
    // Invilcd strips management methods
    //________________________________________________________________
    // Parameters for invilcd strips MonteCarlo
    void SetInvilcdParam(Float_t mean, Float_t sigma);
    void GetInvilcdParam(Float_t &mean, Float_t &sigma) const;
    // Methods for creating invilcd strips
    void SetInvilcdMC(Float_t mean, Float_t sigma);
    void SetInvilcdMC();
    // Testing if strip is vilcd
    Bool_t  IsValidN(Int_t strip);      //True if strip works properly
    Bool_t  IsValidP(Int_t strip);      //True if strip works properly    
    // Access to invilcd strips
    TArrayS *GetInvilcdP();             //Array of invilcd P strips
    TArrayS *GetInvilcdN();             //Array of invilcd N strips
    Int_t    GetNInvilcdP();            //Number of invilcd P strips
    Int_t    GetNInvilcdN();            //Number of invilcd N strips    
    // Creating invilcd strips
    void    SetInvilcdP(Int_t,Bool_t){}//Set invilcd if true
    void    SetInvilcdN(Int_t,Bool_t){}//Set invilcd if true
    Float_t  GetCouplingPR() const {// couplings
      return fCouplingPR;}
    Float_t  GetCouplingPL() const {// couplings
      return fCouplingPL;}
    Float_t  GetCouplingNR() const {// couplings
      return fCouplingNR;}
    Float_t  GetCouplingNL() const {// couplings
      return fCouplingNL;}
    
 protected:
    //_____________________________________________
    //
    // Parameters for invilcd strips simulatation 
    //_____________________________________________    
    Float_t  fCouplingPR;  // couplings
    Float_t  fCouplingPL;  // couplings
    Float_t  fCouplingNR;  // couplings
    Float_t  fCouplingNL;  // couplings   

    Float_t   fNstrips;    //Number of strips
    Float_t   fNInvilcd;   //Mean number of invilcd strips (for simulation) 
    Float_t   fISigma;     //RMS of invilcd strips (Gaussian)

    TArrayS  *fInvilcdP;   //Array of invilcd strips on P-side
    TArrayS  *fInvilcdN;   //Array of invilcd strips on N-side

    ClassDef(IlcTARGETdcsSSD, 1)     // TARGET SSD DCS specific class

};
#endif
