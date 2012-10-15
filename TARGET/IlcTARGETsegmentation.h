#ifndef ILCTARGETSEGMENTATION_H
#define ILCTARGETSEGMENTATION_H

#include <TObject.h>

class TF1;
class IlcTARGETgeom;
//----------------------------------------------
//
// TARGET  segmentation virtual base class
//
class IlcTARGETsegmentation :
public TObject {
 public:
  IlcTARGETsegmentation();
  IlcTARGETsegmentation(const IlcTARGETsegmentation& source);
  virtual ~IlcTARGETsegmentation();
  IlcTARGETsegmentation& operator=(const IlcTARGETsegmentation &source);
    // Set Detector Segmentation Parameters
    //
    // Detector size  
    virtual void    SetDetSize(Float_t p1,Float_t p2,Float_t p3) 
                    {fDx=p1; fDz=p2; fDy=p3;}
    // Cell size   
    virtual void    SetPadSize(Float_t,Float_t) {MayNotUse("SetPadSize");}
    // Maximum number of cells along the two coordinates  
    virtual void    SetNPads(Int_t,Int_t) = 0;
    // Returns the maximum number of cells (digits) posible
    virtual Int_t   GetNPads() const = 0;
    // Set layer
    virtual void SetLayer(Int_t) {MayNotUse("SetLayer");}
    // Transform from real to cell coordinates
    virtual void    GetPadIxz(Float_t,Float_t,Int_t &,Int_t &) const = 0;
    // Transform from cell to real coordinates
    virtual void    GetPadCxz(Int_t,Int_t,Float_t &,Float_t &) const = 0;
    // Transform from real global to local coordinates
    virtual void    GetLocal(Int_t,Float_t *,Float_t *) const 
                                                  {MayNotUse("GetLocal");}
    // Transform from real local to global coordinates
    virtual void    GetGlobal(Int_t,Float_t *,Float_t *) const 
                                                  {MayNotUse("GetGlobal");}
    // Local transformation of real local coordinates -
    virtual void    GetPadTxz(Float_t &,Float_t &) const = 0;
    // Transformation from Geant cm detector center local coordinates
    // to detector segmentation/cell coordiantes starting from (0,0).
    virtual Bool_t LocalToDet(Float_t,Float_t,Int_t &,Int_t &) const = 0;
    // Transformation from detector segmentation/cell coordiantes starting
    // from (0,0) to Geant cm detector center local coordinates.
    virtual void    DetToLocal(Int_t,Int_t,Float_t &,Float_t &) const = 0;
    // Initialisation
    virtual void Init() = 0;
    //
    // Get member data
    //
    // Detector type geometry
    virtual IlcTARGETgeom* Geometry() const {return fGeom;}
    // Detector length
    virtual Float_t Dx() const {return fDx;}
    // Detector width
    virtual Float_t Dz() const {return fDz;}
    // Detector thickness
    virtual Float_t Dy() const {return fDy;}
    // Cell size in x
    virtual Float_t Dpx(Int_t) const = 0;
    // Cell size in z 
    virtual Float_t Dpz(Int_t) const = 0;
    // Maximum number of Cells in x
    virtual Int_t    Npx() const = 0;
    // Maximum number of Cells in z
    virtual Int_t    Npz() const = 0;
    // Layer
    virtual Int_t GetLayer() const {MayNotUse("GetLayer"); return 0;}
    // Set hit position
    //    virtual void     SetHit(Float_t, Float_t) {}
    // angles
    virtual void Angles(Float_t& /* p */, Float_t& /* n */) const
                                          {MayNotUse("Angles");}

    //
    // Get next neighbours 
    virtual void Neighbours(Int_t,Int_t,Int_t*,Int_t[10],Int_t[10]) const
                     {MayNotUse("Neighbours");}

    // Function for systematic corrections
    // Set the correction function
    virtual void SetCorrFunc(TF1* fc) {fCorr = fc;}
    // Get the correction Function
    virtual TF1* CorrFunc() {return fCorr;}
    // Print Default parameters
    virtual void PrintDefaultParameters() const = 0;

 protected:

    virtual void Copy(TObject &obj) const;

    Float_t fDx;    //SPD: Full width of the detector (x axis)- microns
                    //SDD: Drift distance of the 1/2detector (x axis)-microns
                    //SSD: Full length of the detector (x axis)- microns
    Float_t fDz;    //SPD: Full length of the detector (z axis)- microns
                    //SDD: Length of half-detector (z axis) - microns
                    //SSD: Full width of the detector (z axis)- microns
    Float_t fDy;    //SPD:  Full thickness of the detector (y axis) -um 
                    //SDD: Full thickness of the detector (y axis) - microns
                    //SSD: Full thickness of the detector (y axis) -um 
    IlcTARGETgeom *fGeom;  //! pointer to the geometry class
    TF1*       fCorr;   // correction function

    ClassDef(IlcTARGETsegmentation,2) //Segmentation virtual base class 
};
#endif
