#ifndef ILCTARGETSIMULATION_H
#define ILCTARGETSIMULATION_H
/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id: IlcTARGETsimulation.h,v 1.2 2008/11/06 23:57:35 cgatto Exp $ */

#include <TObject.h>
#include <TH1.h>
#include "IlcTARGETDetTypeSim.h"
#include "IlcTARGET.h"
#include "IlcTARGETpList.h"
#include "IlcTARGETParam.h"
#include "IlcTARGETMapA2.h"
#include "IlcRun.h"
class IlcTARGETCalibration;
class IlcTARGETsegmentation2;
class IlcTARGETmodule;
class TRandom;
class TClonesArray;

// This is the base class for TARGET detector signal simulations. Data members
// include are a pointer to the detectors specific response and segmentation
// classes. See the detector specific implementations for the propper code.

class IlcTARGETsimulation : public TObject {

  public:
    IlcTARGETsimulation(); // Default constructor
    // Standard constructor
    IlcTARGETsimulation(IlcTARGETDetTypeSim *dettyp);
    virtual ~IlcTARGETsimulation(); // destructor
    // copy constructor. See detector specific implementation.
    IlcTARGETsimulation(const IlcTARGETsimulation &source);
    // Assignment opporator. See detector specific implementation.
    virtual IlcTARGETsimulation& operator=(const IlcTARGETsimulation &source);
    // Initialize simulation
    virtual void Init() {};

    // *****************  Hits -> SDigits ******************
    // digitize module using the "slow" detector simulator creating
    // summable digits.
    virtual void SDigitiseModule(IlcTARGETmodule *,Int_t,Int_t){;}

    // ***************** sum of SDigits -> Digits **********
    // Reset module arrays (maps), etc
    virtual void InitSimulationModule(Int_t,Int_t){;}
    // add (sum) a new list of summable digits to module, 
    // add an offset (mask) to the track numbers. Returns kTRUE if there
    // is a "good" signal in this module.
    virtual Bool_t AddSDigitsToModule(TClonesArray *pItemArray, Int_t mask );
    // digitize module using the "slow" detector simulator from
    // the sum of summable digits.
    virtual void FinishSDigitiseModule(){;}

    // **************** Hits -> Digits *********************
    // digitize module using the "slow" detector simulator creating digits.
    virtual void DigitiseModule(IlcTARGETmodule *,Int_t,Int_t) {;}
    // digitizes module using the "fast" detector simulator.
    virtual Bool_t CreateFastRecPoints(IlcTARGETmodule *,Int_t,
				     TRandom *,TClonesArray* /*recp*/) {return kTRUE;}
   // Return pointer to Response model
    virtual IlcTARGETCalibration* GetCalibrationModel(Int_t mod = 0){return fDetType->GetCalibrationModel(mod);}
   // set pointer to Response model
    virtual void SetCalibrationModel(Int_t mod, IlcTARGETCalibration *res){fDetType->SetCalibrationModel(mod,res);}
    // Return pointer to Segmentation object
    virtual IlcTARGETsegmentation2* GetSegmentationModel2(Int_t dt) = 0;
    // set pointer to Segmentation object
    virtual void SetSegmentationModel2(Int_t dt,IlcTARGETsegmentation2 *seg) = 0;
    virtual IlcTARGETpList* GetMap(){return fpList;} // Returns fpList, the map.
    virtual IlcTARGETpList* GetMap2(Int_t lay){return (IlcTARGETpList*)fpList2->UncheckedAt(lay);} // Returns fpList, the map.
    virtual IlcTARGETMapA2* GetMapA2(Int_t lay){return (IlcTARGETMapA2*)fMapA2->UncheckedAt(lay);} // Returns fpList, the map.
    virtual void SetMap(IlcTARGETpList *p){fpList = p;} // Sets fpList, the map.
    virtual void SetMap2(Int_t lay,IlcTARGETpList *p){fpList2->AddAt(p,lay);} // Sets fpList, the map.
    virtual void SetMapA2(Int_t lay,IlcTARGETMapA2 *p){fMapA2->AddAt(p,lay);} // Sets fpList, the map.
    virtual void ClearMap(){ 
      for (Int_t lay=0;lay<fParam->GetNlayers()+fParam->GetNendcaps()+
	     fParam->GetNlayersStrip()+fParam->GetNendcapsStrip();lay++)//Dan Endcap
        if (fpList2->UncheckedAt(lay)) ((IlcTARGETpList*)(fpList2->UncheckedAt(lay)))->ClearMap();
    } // Clear fpList, map.
    virtual void ClearMapA2(){
      for (Int_t lay=0;lay<fParam->GetNlayersStrip()+fParam->GetNendcapsStrip();lay++)//Dan Endcap
        if (fMapA2->UncheckedAt(lay)) ((IlcTARGETMapA2*)(fMapA2->UncheckedAt(lay)))->ClearMap();
    } // Clear fpList, map.
    virtual void ClearMap(int lay){ 
      if (fpList2->UncheckedAt(lay)) ((IlcTARGETpList*)(fpList2->UncheckedAt(lay)))->ClearMap();
    } // Clear fpList, map.
    virtual void ClearMapA2(int lay){
      if (fMapA2->UncheckedAt(lay)) ((IlcTARGETMapA2*)(fMapA2->UncheckedAt(lay)))->ClearMap();
    } // Clear fpList, map.

    virtual void SetModuleNumber(Int_t mod){fModule=mod;} // Set Module number
    virtual Int_t GetModuleNumber()const {return fModule;}// Gets Module number
    virtual void SetEventNumber(Int_t evnt){fEvent=evnt;} // Set Event number
    virtual Int_t GetEventNumber()const {return fEvent;}// Gets Event number
    // Sets the debug flag for debugging output
    void SetDebug(Int_t level=5){fDebug=level;}
    // Clears the debug flag so no debugging output will be generated
    void SetNoDebug(){fDebug=0;}
    // Returns the debug flag value
    Bool_t GetDebug(Int_t level=1)const {return fDebug>=level;}
    void SetDetType(IlcTARGETDetTypeSim* dettyp) {fDetType=dettyp;}

 protected:
    IlcTARGETParam   *fParam;
    IlcTARGETDetTypeSim    *fDetType;        //! Access resp and segm via this obj
    IlcTARGETpList         *fpList;          //!
    TObjArray         *fpList2;           //!
    TObjArray         *fMapA2;            //!
    Int_t                fModule;         //!
    Int_t                fEvent;          //!
    Int_t                fDebug;          //  debug flag

  ClassDef(IlcTARGETsimulation,4)  // Simulation base class 
    
};

#endif
