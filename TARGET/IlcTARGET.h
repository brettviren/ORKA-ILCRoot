#ifndef ILCTARGET_H
#define ILCTARGET_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGET.h,v 1.2 2008/11/03 07:38:31 cgatto Exp $ */

////////////////////////////////////////////////////////////////////////
//           Manager class for set: TARGET                               //
////////////////////////////////////////////////////////////////////////


#include <TObjArray.h> // used in inline function GetModule.
#include <TGeoManager.h>
#include "IlcDetector.h"
#include "IlcTARGETTrigger.h"
#include "IlcTARGETDetTypeSim.h"
#include "IlcTARGETsegmentation2.h"
#include "IlcLog.h"


class TString;
class TTree;
class IlcTARGETpListItem;
class IlcTARGETsimulation;
class IlcTARGETsegmentation;
class IlcTARGETCalibration;
class IlcTARGEThit;
class IlcTARGETgeom;
class IlcTARGETdigit;
class IlcTARGETmodule;
class IlcDigitizer;
class IlcRunDigitizer;

class IlcTARGETgeom;
class TArrayD;
class TGeoHMatrix;



class IlcTARGET : public IlcDetector {

 public:
    //================= Standard Classes ===============================
    IlcTARGET();  // Default creator.
    IlcTARGET(const char *name, const char *title, Int_t MinorVersion=1); // standard Creator
    virtual ~IlcTARGET(); // destructor
// // // //     IlcTARGET(const IlcTARGET &source); // copy constructor. Not to be used!
    IlcTARGET& operator=(const IlcTARGET &source); // = operator. Not to be used!
    virtual Int_t IsVersion() const {return 1;}
    virtual Int_t DistancetoPrimitive(Int_t , Int_t )  {return 999;}

    //===================== Simulation Geometry ========================
    // get geometry version - detailed (major) or coarse (minor)
    virtual Int_t GetMajorVersion() const {return -1;}
    virtual Int_t GetMinorVersion() const {// return Major Version Number
	return fMinorVersion;}
    virtual void   SetMinorVersion(Int_t v=1){ // Choose between existing minor versions
	fMinorVersion = v;}
    virtual void  GetGeometryVersion(Int_t &a,Int_t &b) const
	                   {a = GetMajorVersion();b=GetMinorVersion();return;}
    virtual void  SetEUCLID(Bool_t euclid=kTRUE) {fEuclidOut = euclid;}
    virtual Bool_t GetEUCLID()const {return fEuclidOut;}
    //-------------------- Geometry Transformations --------------------

    // TARGET geometry functions From Simulation
    IlcTARGETgeom* GetTARGETgeom() const {return fDetTypeSim->GetTARGETgeom();}
    IlcTARGETgeom* GetTARGETgeom2() const {return fDetTypeSim->GetTARGETgeom2();}
    void   SetTARGETgeom(IlcTARGETgeom *geom) {fDetTypeSim->SetTARGETgeom(geom);}
    void   SetTARGETgeom2(IlcTARGETgeom *geom) {fDetTypeSim->SetTARGETgeom2(geom);}
    void   SetBackgroundFile(const char *Backfile,int nbrunchs=1) {
      if(!fBackfile)fBackfile=new TString;(*fBackfile)=Backfile; 
      fNBeemBrunchs=nbrunchs;
    }    
    void   SetNBeemBrunchs(int nbrunchs=1) {fNBeemBrunchs=nbrunchs;}    
    TString *GetBackgroundFile()                            {return fBackfile;}
    virtual void   WriteGeometry();
    // return pointer to the array of modules
    TObjArray *GetModules(){return fTARGETmodules;}

    IlcTARGETmodule *GetModule(Int_t index){
      return (IlcTARGETmodule*)(fTARGETmodules->UncheckedAt(index));}

    virtual void SetDetTypeSim(IlcTARGETDetTypeSim* dts) {fDetTypeSim=dts;}
    IlcTARGETDetTypeSim* GetDetTypeSim() const {return fDetTypeSim;}
    //================ Necessary general Classes =======================
    virtual void Init();
    virtual IlcLoader* MakeLoader(const char* topfoldername);
    virtual void SetDefaults();
    virtual void SetDefaultSimulation();
    virtual void MakeBranch(Option_t *opt=" ");
    virtual void MakeBranchS(const char* fl);
    virtual void MakeBranchD(const char* file);
    virtual void MakeBranchInTreeD(TTree* treeD, const char* file=0);
    virtual void SetTreeAddress();
     //---------- Configuration Methods (per detector type) -------------
    // Determines which TARGET subdetectors will be processed. Effects
    // digitization, and Reconstruction only.
    void SetDetectors(Option_t *opt="All"){fOpt = opt;}
    // Returns the list of TARGET subdetectors that will be processed.
    Option_t* GetDetectors(){return fOpt;}

    // Set calibration
    virtual void SetCalibrationModel(Int_t dettype, IlcTARGETCalibration *cal){
        fDetTypeSim->SetCalibrationModel(dettype,cal);}
    // Set segmentation for Simulation
    virtual void SetSegmentationModel(Int_t id, IlcTARGETsegmentation *seg){
        fDetTypeSim->SetSegmentationModel(id,seg);}
    virtual void SetSegmentationModel2(Int_t id, IlcTARGETsegmentation2 *seg);

    // Set simulation 
    virtual void SetSimulationModel(Int_t id, IlcTARGETsimulation *sim){
        fDetTypeSim->SetSimulationModel(id,sim);}
    // Set simulation 
    virtual IlcTARGETsimulation* GetSimulationModel(Int_t id){
	return fDetTypeSim->GetSimulationModel(id);}
    virtual IlcTARGETsegmentation2* GetSegmentationModel2(Int_t id){
	return fDetTypeSim->GetSegmentationModel2(id);}

    //=================== Hits =========================================
    virtual void StepManager() {} // See Step Manager for specific geometry.
    //------------ sort hits by module for Digitisation ----------------
    virtual void FillModules(Int_t evnt,Int_t bgrev,Int_t nbunchs,Int_t nmodules, Option_t *opt, const char *filename);
 
    virtual void InitModules(Int_t size,Int_t &nmodules);  
    virtual void FillModules(TTree *treeH, Int_t mask = 0);
    virtual void ClearModules(){if(fTARGETmodules) fTARGETmodules->Delete();}
    virtual void AddHit(Int_t track, Int_t *vol, Float_t *hits);
   // Trigger
    virtual IlcTriggerDetector* CreateTriggerDetector() const
       { return new IlcTARGETTrigger(); }
   
    TClonesArray* GetSDigits() const { return fDetTypeSim->GetSDigits();}

    virtual IlcDigitizer* CreateDigitizer(IlcDigitizationInput* manager) const;
    virtual void SDigitsToDigits(Option_t *opt="All");
    virtual void SDigits2Digits(){SDigitsToDigits("All");}
    virtual void Hits2Digits(); 
    virtual void Hits2SDigits();
    virtual void Hits2PreDigits();
    virtual void HitsToDigits(Int_t evNumber,Int_t bgrev,Int_t nbunchs,Int_t size,
                          Option_t *option,Option_t *opt,
                          const char *filename);
    virtual void HitsToPreDigits(Int_t evNumber,Int_t bgrev,Int_t nbunchs,Int_t size,
                             Option_t *option,Option_t *opt,
                             const char *filename);
    void HitsToSDigits(Int_t evNumber,Int_t bgrev,Int_t nbunchs,Int_t size,
                       Option_t *add, Option_t *det, const char *filename)
      { HitsToPreDigits(evNumber,bgrev,nbunchs,size,add,det,filename);}
    
    virtual void ResetDigits();
    virtual void ResetDigits(Int_t branch);
    virtual void AddSumDigit(IlcTARGETpListItem &sdig);
    virtual void AddRealDigit(Int_t branch, Int_t *digits);

    virtual void AddSimDigit(Int_t branch, IlcTARGETdigit *d);			     
    virtual void AddSimDigit(Int_t branch,Float_t phys,Int_t* digits,
		     Int_t* tracks,Int_t *hits,Float_t* trkcharges);
    TObjArray* GetDigits()  const {return fDetTypeSim->GetDigits();}
    Int_t* GetNDigitArray() const {return fDetTypeSim->GetNDigitArray();}
    TClonesArray *DigitsAddress(Int_t id) { return fDetTypeSim->DigitsAddress(id);}
    //Fast simulation
    virtual void HitsToFastRecPoints(Int_t evNumber,Int_t bgrev,Int_t nbunchs,Int_t size,
                 Option_t *add, Option_t *det, const char *filename);
    virtual void HitsToFastRecPointsV2(Int_t evNumber,Int_t bgrev,Int_t nbunchs,Int_t size,
                 Option_t *add, Option_t *det, const char *filename);

    virtual Int_t Hits2Clusters(TTree *in, TTree *out);
    virtual void CheckLabels(Int_t lab[3]) const;

    //===================== Raw Data IO ================================
    // Write digits into raw data format
    virtual void Digits2Raw();
    virtual void       ImportGDMLMaterial();
    virtual void       ImportGDMLGeometry();
    void       SetGDMLTARGETGeometry(Bool_t flag=kTRUE)  {fGDMLTARGETGeometry=flag; };
const char*    GetGDMLGeometryFileName() const {return fGDMLGeometryFileName.
Data();}
void           SetGDMLGeometryFileName(const char *name) {fGDMLGeometryFileName = name;};

private:



    // Patch to correct mismatch between TGeo and IlcTARGETgeom
    // To be there only for the PDC06 
    IlcTARGETgeom* CreateIlcTARGETgeom(); // Create and intilize geometry from TGeom
    Bool_t InitIlcTARGETgeomPPRasymmFMD(IlcTARGETgeom *geom);
    Bool_t InitGeomShapePPRasymmFMD(IlcTARGETDetector idet,Bool_t *initSeg,
				       TArrayD &shapePar,IlcTARGETgeom *geom);
    Bool_t GetTransformation(const TString &volumePath,TGeoHMatrix &mat);
    Bool_t GetShape(const TString &volumePath,TString &shapeType,TArrayD &par);
    void DecodeDetectorLayers(Int_t mod,Int_t &lay,Int_t &lad,Int_t &det);
    void RecodeDetector(Int_t mod,Int_t &cpn0,Int_t &cpn1,Int_t &cpn2);
                              // class in fShape of IlcTARGETgeom class.
    Bool_t    fDecode;        // Flag for new/old decoding

 protected:
    Int_t fgkNTYPES; //number of detector types
 
    //================== Data Members ==================================
    IlcTARGETDetTypeSim *fDetTypeSim; //detector type for simulation
    Bool_t        fEuclidOut;  // Flag to write geometry in euclid format
    Int_t         fMinorVersion;
    Option_t     *fOpt;        //! Detector option ="All" unless changed.
    Int_t         fIdN;        // the number of layers
    Int_t        *fIdSens;     //[fIdN] layer identifier
    TString      *fIdName;     //[fIdN] layer identifier
    TObjArray    *fTARGETmodules; //! Pointer to TARGET modules
    Bool_t        fGDMLTARGETGeometry;
    TString       fGDMLGeometryFileName;  //! Name of the geometry file
    TString      *fBackfile;
    Int_t         fNBeemBrunchs;
    
    ClassDef(IlcTARGET,7) // Base class for TARGET

};

#endif
