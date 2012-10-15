#ifndef ILCTARGETDETTYPESIM_H
#define ILCTARGETDETTYPESIM_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/*
$Id: IlcTARGETDetTypeSim.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $ 
*/

/////////////////////////////////////////////////////////////////////////
// * This class contains all of the "external" information needed to do//
// * detector specific simulations for the TARGET.                        //
/////////////////////////////////////////////////////////////////////////

#include <TObject.h>
#include <TObjArray.h>
#include <TClonesArray.h>
#include "IlcTARGETCalibration.h"
#include "IlcTARGETgeom.h"
#include "IlcTARGETsegmentation2.h"

class TTree;
class IlcCDBMetaData;
class IlcTARGETdigit;
class IlcTARGETmodule;
class IlcTARGETpListItem;
class IlcTARGETsimulation;
class IlcTARGETsegmentation;
class IlcTARGETresponse;
class IlcLoader;

class IlcTARGETDetTypeSim : public TObject {
 public:
  
    IlcTARGETDetTypeSim();
    virtual ~IlcTARGETDetTypeSim(); 
    IlcTARGETDetTypeSim(const IlcTARGETDetTypeSim &source);
    IlcTARGETDetTypeSim& operator=(const IlcTARGETDetTypeSim &source);
    IlcTARGETgeom *GetTARGETgeom() const {return fGeom;}
    IlcTARGETgeom *GetTARGETgeom2() const {return fGeom2;}
    void SetTARGETgeom(IlcTARGETgeom *geom){fGeom=geom;}
    void SetTARGETgeom2(IlcTARGETgeom *geom){fGeom2=geom;}
    
    virtual void SetSimulationModel(Int_t dettype,IlcTARGETsimulation *sim);
    virtual IlcTARGETsimulation* GetSimulationModel(Int_t dettype);        
    virtual IlcTARGETsimulation* GetSimulationModelByModule(Int_t module);
        
    virtual void SetSegmentationModel(Int_t dettype,IlcTARGETsegmentation *seg);
    virtual void SetSegmentationModel2(Int_t dettype,IlcTARGETsegmentation2 *seg);

    virtual IlcTARGETsegmentation* GetSegmentationModel(Int_t dettype);
    virtual IlcTARGETsegmentation2* GetSegmentationModel2(Int_t dettype);

    virtual IlcTARGETsegmentation* GetSegmentationModelByModule(Int_t module);

    virtual void SetCalibrationModel(Int_t iMod,IlcTARGETCalibration *resp);
    virtual IlcTARGETCalibration* GetCalibrationModel(Int_t iMod);
    virtual IlcTARGETresponse* GetResponse(Int_t dettype) {return GetCalibrationModel(GetTARGETgeom()->GetStartDet(dettype))->GetResponse();}

    TObjArray* GetCalibrationArray() const {return fCalibration;}
    TObjArray* GetSegmentation() const {return fSegmentation;}
    TObjArray* GetSegmentationSPD() const {return fSegmentation2;}
    //TObjArray* GetSegmentationSPD() const {return fSegmentationSPD;}

    void ResetCalibrationArray();
    void ResetSegmentation();

    virtual void SetLoader(IlcLoader* loader) {fLoader=loader;}
    IlcLoader* GetLoader() const {return fLoader;}

    virtual void SetDefaults();
    virtual void SetDetVersion(Int_t DetVersion) {fDetVersion=DetVersion;}
    virtual void Init();
    virtual void SetDefaultSimulation();
    virtual void SetRunNumber(Int_t rn=0){fRunNumber = rn;}
    virtual Int_t GetRunNumber() const {return fRunNumber;}
    virtual void SetTreeAddressS(TTree* treeS, Char_t* name);
    virtual void SetTreeAddressD(TTree* treeD, Char_t* name);

    virtual void SetSDigits(TClonesArray* sdigits) {fSDigits=sdigits;}
    virtual void SetDigits(TObjArray* digits) {fDigits=digits;}
    TClonesArray* GetSDigits() const {return fSDigits;}
    TObjArray*    GetDigits() const {return fDigits;}
    Int_t* GetNDigitArray() const {return fNDigits;}
    TClonesArray *DigitsAddress(Int_t id) const { return ((TClonesArray*)(*fDigits)[id]);}

    virtual void ResetSDigits(){fNSDigits=0;if(fSDigits!=0) fSDigits->Clear();}
    virtual void ResetDigits();
    virtual void ResetDigits(Int_t branch);
    virtual void SDigitsToDigits(Option_t *opt,Char_t* name);

    virtual void AddSumDigit(IlcTARGETpListItem &sdig);
    virtual void AddRealDigit(Int_t branch, Int_t *digits);
    virtual void AddSimDigit(Int_t branch, IlcTARGETdigit *d);
    virtual void AddSimDigit(Int_t branch,Float_t phys,Int_t* digits,
			     Int_t* tracks,Int_t *hits,Float_t* trkcharges);

    virtual void SetDigitClassName(Int_t i, Char_t* name) {fDigClassName[i]=name;}
    Char_t* GetDigitClassName(Int_t i) const {return fDigClassName[i];}

    void StoreCalibration(Int_t firstRun, Int_t lastRun, IlcCDBMetaData &md);

 protected:

    virtual void CreateCalibrationArray(); 
    virtual Bool_t GetCalibration();
    
 private:

    void SetDefaultSegmentation(Int_t idet);  // creates def segm.
    Int_t fgkNdettypes;          // number of different det. types
    Int_t fgkDefaultNModulesSPD; // Total numbers of SPD modules by default
    Int_t fgkDefaultNModulesSDD; // Total numbers of SDD modules by default
    Int_t fgkDefaultNModulesSSD; // Total numbers of SSD modules by default
    static const Int_t fgkDefaultNModulesSPDend; // Total numbers of SSD modules by default

    Int_t fNMod[3];                           // numbers of modules from different types
    Int_t        fDetVersion;
    IlcTARGETgeom   *fGeom;         // pointer to TARGET geom
    IlcTARGETgeom   *fGeom2;         // pointer to TARGET geom
    TObjArray    *fSimulation;   //! [NDet]
    TObjArray    *fSegmentation; //! [NDet]
    TObjArray    *fSegmentation2; //! [NDet]
    //TObjArray    *fSegmentationSPD; //! [NDet]

    TObjArray    *fCalibration;  //! [NMod]
    TObjArray    *fPreProcess;   //! [] e.g. Fill fHitModule with hits
    TObjArray    *fPostProcess;  //! [] e.g. Wright Raw data
    Int_t         fNSDigits;     //! number of SDigits
    TClonesArray *fSDigits;      //! [NMod][NSDigits]
    Int_t*        fNDigits;      //! [NDet] number of Digits for det.
    Int_t      fRunNumber;    //! run number (to access DB)
    TObjArray     *fDigits;       //! [NMod][NDigits]
    TString       fHitClassName; //! String with Hit class name
    TString       fSDigClassName;//! String with SDigit class name.
    Char_t*       fDigClassName[3]; //! String with digit class name.
    IlcLoader*    fLoader;          //! loader  
    Bool_t fFirstcall;              //! flag
    
  ClassDef(IlcTARGETDetTypeSim,4) // TARGET Simulation structure
 
};

#endif
