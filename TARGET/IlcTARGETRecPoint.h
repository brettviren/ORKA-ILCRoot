#ifndef ILCTARGETRECPOINT_H
#define ILCTARGETRECPOINT_H 
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/*
  $Id: IlcTARGETRecPoint.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $
*/

///////////////////////////////////////////////////////////////////////////////
//  Reconstructed space point class for set:TARGET   
//  Reconstructed points are expressed simultaneously in two different 
//  reference frames, both differing from the global system.
//  The first is referred to the sensor (see IlcTARGETsegmentation for the
//  definition) and each point is represented by two coordinates: fXloc and
//  fZloc. This system in the code is referred to as "local"
//  The second is used for tracking (V2, SA and MI versions) and the X axis 
//  represents the radial coordinate (this system is, in the bending plane, 
//  a rotated system w.r.t. the global reference system). 
//  Each reaconstructed point is represented by two coordinates: fY and fZ, 
//  inherited from IlcCluster. This system in the code is referred to as 
//  "trackingV2".
///////////////////////////////////////////////////////////////////////////////

#include <IlcCluster.h>
#include <Riostream.h>
#include <IlcLog.h>
#include <IlcTARGETgeom.h>


class IlcTARGETRecPoint : public IlcCluster {
 public:
  IlcTARGETRecPoint();
  IlcTARGETRecPoint(IlcTARGETgeom* geom);
  IlcTARGETRecPoint(Int_t *lab,Float_t *hit, Int_t *info);
  IlcTARGETRecPoint(Int_t module,IlcTARGETgeom* geom,Int_t *lab,
		 Float_t *hit, Int_t *info);
  IlcTARGETRecPoint(const IlcTARGETRecPoint& pt);
  IlcTARGETRecPoint& operator=(const IlcTARGETRecPoint &source);
  
  virtual ~IlcTARGETRecPoint() {}; // distructor
  Bool_t IsSortable() const {return kTRUE;} // allows for sorting
// // // //   Int_t  *GetTracks(){return fTracks;}// Returns pointer to track array
  Int_t   GetNTracks() const {return 3;} // returns track array size
  Float_t GetDetLocalX() const {return fXloc;} // gets fX
  Float_t GetDetLocalZ() const {return fZloc;} // gets fZ
  Float_t GetY()         const {return GetDetLocalX();}
  Float_t GetZ()         const {return GetDetLocalZ();}
  Float_t GetdEdX() const {return fdEdX;} // gets fdEdX
  Float_t GetSigmaDetLocX2() const {return GetSigmaY2();} // gets fSigmaX2
  void SetdEdX(Float_t dedx){fdEdX=dedx;} // sets fdEdX
  void SetSigmaDetLocX2(Float_t sx2){SetSigmaY2(sx2);} // sets fSigmaX2
  Int_t Compare(const TObject *) const {return 0;} //to be defined
  void Print(ostream *os); 
  // Reads in the content of this class in the format of Print
  void Read(istream *is);
  virtual void Print(Option_t *option="") const {TObject::Print(option);}
  virtual Int_t Read(const char *name) {return TObject::Read(name);}
  
  void SetTARGETgeom(IlcTARGETgeom* geom) {fGeom=geom;}
  void SetYZ(Int_t module, Float_t y, Float_t z){
    IlcCluster::SetY(y);IlcCluster::SetZ(z);
    if(fGeom)fGeom->TrackingV2ToDetL(module,y,z,fXloc,fZloc);
    else IlcError("Geometry not set. \n");
  }
  void SetXZ(Int_t module, Float_t x, Float_t z){
    fXloc=x;fZloc=z;
    Float_t yout;
    Float_t zout;
    if(fGeom){
      fGeom->DetLToTrackingV2(module,x,z,yout,zout);
      SetYZ(module,yout,zout);
    }
    else IlcError("Geometry not set. Nothing done!!!!!\n");
  }

  void SetXY(/*Int_t module,*/ Float_t x, Float_t y){
    fXloc=x;fYloc=y;
  }

  void Use(Int_t = 0) {fQ=-fQ;}
  void UnUse() {fQ=TMath::Abs(fQ);}
  void SetQ(Float_t q) {fQ=q;}
  void SetDetectorIndex(Int_t i) { fIndex=i; }
  void SetLayer(Int_t layer) {fLayer=layer;}
  void SetNz(Int_t nz) {fNz =nz;}
  void SetNy(Int_t ny){fNy=ny;}
  void SetChargeRatio(Float_t ratio) { fChargeRatio = ratio;}
  void SetPhiR(Float_t y) { fChargeRatio=y; }
  void SetModule(Int_t module) { fModule=module; }
  void SetDetector(Int_t det) { fDet=det; }
  void SetType(Int_t type){ fType=type;}
  void SetDeltaProbability(Float_t prob){fDeltaProb = prob;}
  
  Int_t IsUsed() const {return (fQ<0)?1:0;}
  Float_t GetQ() const {return TMath::Abs(fQ);}
  Int_t GetDetectorIndex() const { return 0xFFF&fIndex; }
  Int_t GetLayer() const {return fLayer;}
  Int_t GetModule() const {return fModule;}
  Int_t GetDetector() const {return fDet;}
  Int_t GetNz() const {return fNz;}
  Int_t GetNy() const {return fNy;}
  Float_t GetChargeRatio() const {return fChargeRatio;}
  Float_t GetPhiR() const {return fChargeRatio;}
  Int_t GetPindex() const { return 0xFFC00000&fIndex; }  //SSD clusters only
  Int_t GetNindex() const { return 0x3FF000&fIndex; }  //SSD clusters only
  Int_t GetType() const {return fType;}  // type of the cluster
  Float_t GetDeltaProbability() const{return fDeltaProb;} //probability to belong to the delta ray
  


 protected:

  Float_t   fXloc ;        //X of cluster (local coordinates)
  Float_t   fZloc ;        //Z of cluster (local coordinates)
  Float_t   fYloc ;        //Y of cluster (local coordinates) used for endcaps
  Float_t   fdEdX;      //dE/dX inside this cluster

  Int_t    fModule;    //module index
  Int_t    fIndex;    // detector index
  Int_t    fDet;
  Float_t  fQ ;       // Q of cluster (in ADC counts)
  Char_t   fLayer;    // layer number
  Short_t  fNz;       //number of digits in Z direction
  Short_t  fNy;       //number of digits in y direction 
  Float_t  fChargeRatio; //charge ratio
  Int_t    fType;         //quilcty factor of the cluster
  Float_t  fDeltaProb;    // probability to be delta electron
    
  IlcTARGETgeom* fGeom;     //!pointer to TARGET geometry

  ClassDef(IlcTARGETRecPoint,2)  // IlcTARGETRecPoint class
};
// Input and output function for standard C++ input/output.
ostream& operator<<(ostream &os,IlcTARGETRecPoint &source);
istream& operator>>(istream &is,IlcTARGETRecPoint &source);
#endif
