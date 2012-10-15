#ifndef ILCTARGETDETTYPEREC_H
#define ILCTARGETDETTYPEREC_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/*
$Id: IlcTARGETDetTypeRec.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ 
*/

////////////////////////////////////////////////////////////////////////
// This class contains all of the "external" information needed to do //
// detector specific reconstruction for the TARGET.                      //
////////////////////////////////////////////////////////////////////////
#include <TObject.h>
#include <TObjArray.h>
#include <TClonesArray.h>
#include <TBranch.h>

#include "IlcTARGETLoader.h"
#include "IlcRunLoader.h"

class IlcTARGETsegmentation;
class IlcTARGETsegmentation2;
class IlcTARGETCalibration;
class IlcTARGETClusterFinder;
class IlcTARGETRawCluster;
class IlcTARGETRecPoint;
class IlcRawReader;
class IlcTARGETgeom;


class IlcTARGETDetTypeRec : public TObject {
  public:
    IlcTARGETDetTypeRec(); // Default constructor
    IlcTARGETDetTypeRec(IlcTARGETLoader *loader); // Standard constructor
    IlcTARGETDetTypeRec(const IlcTARGETDetTypeRec& rec);
    IlcTARGETDetTypeRec& operator=(const IlcTARGETDetTypeRec &source);

    virtual ~IlcTARGETDetTypeRec(); // Proper Destructor
    IlcTARGETgeom* GetTARGETgeom()const{return GetLoader()->GetTARGETgeom();}

    IlcTARGETLoader* GetLoader() const {return fLoader;}
    virtual void SetDefaults();
    virtual void SetDefaultClusterFinders();
    virtual void SetDefaultClusterFindersV2(Bool_t rawdata=kFALSE);
    virtual void MakeBranch(Option_t *opt);
    virtual void SetTreeAddress();
    virtual void SetTreeAddressD(TTree* treeD);

    virtual void SetSegmentationModel(Int_t dettype, IlcTARGETsegmentation *seg);
    virtual void SetSegmentationModelSPD(Int_t dettype,IlcTARGETsegmentation2 *seg);

    virtual void SetCalibrationModel(Int_t iMod, IlcTARGETCalibration *cal);
    virtual void SetReconstructionModel(Int_t dettype, IlcTARGETClusterFinder *rec);
    virtual Bool_t GetCalibration();
    virtual IlcTARGETsegmentation* GetSegmentationModel(Int_t dettype);
    virtual IlcTARGETsegmentation2* GetSegmentationModelSPD(Int_t dettype);

    virtual IlcTARGETCalibration* GetCalibrationModel(Int_t iMod);
    virtual IlcTARGETClusterFinder* GetReconstructionModel(Int_t dettype);

    virtual void SetDigitClassName(Int_t i,Char_t *digit) 
      {fDigClassName[i]=digit;}
    virtual void SetClusterClassName(Int_t i,Char_t *cluster)
      {fClusterClassName[i]=cluster;}
    virtual void SetRecPointClassName(Int_t i,Char_t *recpoint) 
      {fRecPointClassName[i]=recpoint;}
    
    Char_t* GetDigitClassName(Int_t i) const {return fDigClassName[i];}
    Char_t* GetClusterClassName(Int_t i) const {return fClusterClassName[i];}
    Char_t* GetRecPointClassName(Int_t i) const {return fRecPointClassName[i];}
    
    TObjArray* GetDigits() const {return fDigits;} 
    Int_t *Ndtype() {return fNdtype;}
    TClonesArray *DigitsAddress(Int_t id) const {return ((TClonesArray*)(*fDigits)[id]);}
    virtual void SelectVertexer(TString sel=" "){fSelectedVertexer = sel;}
    //
    virtual void MakeTreeC();
    virtual void GetTreeC(Int_t event);
    virtual void AddCluster(Int_t branch, IlcTARGETRawCluster *c);
    virtual void ResetClusters(); 
    virtual void ResetClusters(Int_t branch);
    virtual void MakeBranchC();
    TBranch* MakeBranchInTree(TTree *tree, const char* name, const char *classname, void* address,Int_t size, Int_t splitlevel, const char */*file*/);

    TObjArray    *Ctype()  {return fCtype;}
    Int_t        *Nctype() {return fNctype;}
    TClonesArray *ClustersAddress(Int_t id) const {return ((TClonesArray*)(*fCtype)[id]);}
    virtual void ResetDigits();
    virtual void ResetDigits(Int_t branch);

    void MakeBranchR(const char *file, Option_t *opt=" ");
    void SetTreeAddressR(TTree *treeR);
    void AddRecPoint(const IlcTARGETRecPoint &p);
    void ResetRecPoints(){if(fRecPoints) fRecPoints->Clear();fNRecPoints = 0;};
    // Return pointer to rec points 
    TClonesArray  *RecPoints()   {return fRecPoints;}
    void MakeBranchRF(const char *file){MakeBranchR(file,"Fast");}
    //    void HitsToFastRecPoints(Int_t evNumber,Int_t bgrev,Int_t size,
    //             Option_t *add, Option_t *det, const char *filename);
    void Digits2Reco(){
        DigitsToRecPoints(fLoader->GetRunLoader()->GetEventNumber(),0,"SPD SSD");}
    void DigitsToRecPoints(Int_t evNumber,Int_t lastEntry,Option_t *det,Bool_t v2=kFALSE);
    void DigitsToRecPoints(IlcRawReader* rawReader);

    virtual void SetRunNumber(Int_t rn=0){fRunNumber = rn;}
    virtual Int_t GetRunNumber() const {return fRunNumber;}

  private:
    // private methods
    virtual void SetLoader(IlcTARGETLoader* loader) {fLoader=loader;}
    static const Int_t fgkNdettypes;          // number of det. types
    static const Int_t fgkDefaultNModulesSPD; // Total numbers of SPD modules by default
    static const Int_t fgkDefaultNModulesSDD; // Total numbers of SDD modules by default
    static const Int_t fgkDefaultNModulesSSD; // Total numbers of SSD modules by default
    Int_t *fNMod;     // numbers of modules from different types

    TObjArray    *fReconstruction;//! [NDet]
    TObjArray    *fSegmentation;  //! [NDet]
    TObjArray    *fSegmentationSPD; //! [NDet]

    TObjArray    *fCalibration;   //! [NMod]
    TObjArray    *fPreProcess;    //! [] e.g. Find Calibration values
    TObjArray    *fPostProcess;   //! [] e.g. find primary vertex
    TObjArray    *fDigits;        //! [NMod][NDigits]
    Int_t        *fNdtype;        //! detector types  
    Char_t*       fClusterClassName[3]; //! String with Cluster class name
    Char_t*       fDigClassName[3];     //! String with digit class name.
    Char_t*       fRecPointClassName[3];//! String with RecPoint class name

    TObjArray    *fCtype;      //! List of clusters
    Int_t        *fNctype;     //[fNDetTypes] Num. of clust. per type of det.

    TClonesArray *fRecPoints;  //! List of reconstructed points
    Int_t         fNRecPoints; // Number of rec points

    TString fSelectedVertexer; // Vertexer selected in CreateVertexer
    IlcTARGETLoader* fLoader;     //! TARGET loader
    Int_t         fRunNumber;  //! run number (to access DB)
    Bool_t fFirstcall;         //! flag

    ClassDef(IlcTARGETDetTypeRec,6) // TARGET Reconstruction structure
};

#endif
