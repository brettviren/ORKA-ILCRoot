#ifndef ILCTARGETGEOM_H
#define ILCTARGETGEOM_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETgeom.h,v 1.3 2008/04/22 09:04:29 cgatto Exp $ */

/////////////////////////////////////////////////////////////////////////
//  TARGET geometry manipulation routines.
//  Created April 15 1999.
//  version: 0.0.0
//  By: Bjorn S. Nilsen
//
//     A package of geometry routines to do transformations between
// local, detector active area, and ILC global coordinate system in such
// a way as to allow for detector alignment studies and the like. All of
// the information needed to do the coordinate transformation are kept in
// a speciilczed structure for ease of implementation.
/////////////////////////////////////////////////////////////////////////
#include <Riostream.h>
#include <TObject.h>
#include <TObjArray.h>
#include <TVector.h>
#include <TString.h>
#include <TArrayI.h>
#include <TArrayF.h>
#include <TMath.h>
#include <TGeoMatrix.h>
class TArrayI;

#include "IlcTARGETgeomMatrix.h"
#include "IlcLog.h"

typedef enum {kND=-1,kSPD=0,kSDD=2, kSSD=1, kSSDp=3,kSDDp=4} IlcTARGETDetector;

//_______________________________________________________________________

class IlcTARGETgeom : public TObject {

 public:
    IlcTARGETgeom();                      // Default constructor
    IlcTARGETgeom(const char *filename);  // Constructor
    IlcTARGETgeom(Int_t itype,Int_t nlayers,const Int_t *nlads,const Int_t *ndets,
               Int_t nmods,Int_t MinorVersion); // Constructor
    IlcTARGETgeom(const IlcTARGETgeom &source);    // Copy constructor
    IlcTARGETgeom& operator=(const IlcTARGETgeom &source);// = operator
    virtual ~IlcTARGETgeom();             // Default destructor
    // Zero and reinitilizes this class.
    void Init(Int_t itype,Int_t nlayers,const Int_t *nlads,
              const Int_t *ndets,Int_t mods,Int_t MinorVersion);
    // this function allocates a IlcTARGETgeomMatrix for a particular module.
    void CreatMatrix(Int_t mod,Int_t lay,Int_t lad,Int_t det,
		     IlcTARGETDetector idet,const Double_t tran[3],
		     const Double_t rot[10]);
    void ReadNewFile(const char *filename);  // Constructor for new format.
    void WriteNewFile(const char *filename)const; // Output for new format.
    // Getters
    Int_t GetTransformationType() const {return fTrans;}
    //
    // returns kTRUE if the transformation defined by this class is
    // for Global GEANT coordinate system to the local GEANT coordinate system
    // of the detector. These are the transformation used by GEANT.
    Bool_t IsGeantToGeant()     const {return (fTrans == 0);}
    // returns kTRUE if the transformation defined by this class is
    // for Global GEANT coordinate system to the local "Tracking" coordinate
    // system of the detector. These are the transformation used by the
    // Tracking code.
    Bool_t IsGeantToTracking()  const {return ((fTrans&&0xfffe)!= 0);}
    // returns kTRUE if the transformation defined by this class is
    // for Global GEANT coordinate system to the local GEANT coordinate system
    // of the detector but may have been displaced by some typically small
    // amount. These are modified transformation similar to that used by GEANT.
    Bool_t IsGeantToDisplaced() const {return ((fTrans&&0xfffd)!= 0);}
    // returns kTRUE if the shape defined by ishape has been defined in this
    // set of transformations. Typical values of ishape are kSPD, kSDD, kSSD,
    // kSSD2.
    Bool_t IsShapeDefined(Int_t ishape)const {
      return ((fShape.UncheckedAt(ishape))!=0);}
    //
    //     This function returns a pointer to the particular IlcTARGETgeomMatrix
    // class for a specific module index.
    IlcTARGETgeomMatrix *GetGeomMatrix(Int_t index){
      if(index<fGm.GetSize()&&index>=0)
      return (IlcTARGETgeomMatrix*)(fGm.UncheckedAt(index));else Error("***GetGeomMatrix","index=%d<0||>=GetSize()=%d",index,fGm.GetSize());return 0;}
    IlcTARGETgeomMatrix *GetGeomMatrix(Int_t index)const{
      if(index<fGm.GetSize()&&index>=0)
      return (IlcTARGETgeomMatrix*)(fGm.UncheckedAt(index));else Error("**GetGeomMatrix","index=%d<0||>=GetSize()=%d",index,fGm.GetSize());return 0;}
    Int_t GetDetVersion()const {return fDetVersion;}
    void SetDetVersion(Int_t DetVersion) {fDetVersion = DetVersion;}
    // This function find and return the number of detector types only.
    Int_t GetNDetTypes()const{Int_t max;return GetNDetTypes(max);};
    // This function find and return the number of detector types and the
    // maximum det type value.
    Int_t GetNDetTypes(Int_t &max)const;
    // This function finds and return the number of detector types and the
    // and the number of each type in the TArrayI and their types.
    Int_t GetNDetTypes(TArrayI &maxs,IlcTARGETDetector *types)const;
    //     This function returns the number of detectors/ladder for a give 
    // layer. In particular it returns fNdet[layer-1].
    Int_t GetNdetectors(Int_t lay) const {return fNdet[lay-1];}
    //     This function returns the number of ladders for a give layer. In
    // particular it returns fNlad[layer-1].
    Int_t GetNladders(Int_t lay)   const {return fNlad[lay-1];};
    //     This function returns the number of layers defined in the TARGET
    // geometry. In particular it returns fNlayers.
    Int_t GetNlayers()                   const {return fNlayers;}
    Float_t Dz(Int_t lay)                {return fDz.At(lay);}
    Float_t Posz1(Int_t lay)                {return fPosz1.At(lay);}
    Float_t Posz2(Int_t lay)                {return fPosz2.At(lay);}
    Float_t Posz1_Fake(Int_t lay)                {return fPosz1_Fake.At(lay);}
    Float_t Posz2_Fake(Int_t lay)                {return fPosz2_Fake.At(lay);}
    Int_t   GetNFakeLayers()                     {return fNFakeLayers;}
    Float_t Rmin(Int_t lay)                {return fRmin.At(lay);}
    Float_t Rmax(Int_t lay)                {return fRmax.At(lay);}
    Int_t GetModuleIndex(Int_t lay,Int_t lad,Int_t det)const;
    //     This function returns the module index number given the layer,
    // ladder and detector numbers put into the array id[3].
    Int_t GetModuleIndex(const Int_t *id)const{
        return GetModuleIndex(id[0],id[1],id[2]);}
    void  GetModuleId(Int_t index,Int_t &lay,Int_t &lad,Int_t &det)const;
    // Returns the detector type
    //Int_t GetModuleType(Int_t index)const{
    //    return GetGeomMatrix(index)->GetDetectorIndex();}
    IlcTARGETDetector GetModuleType(Int_t index)const{
        return (IlcTARGETDetector)(GetGeomMatrix(index)->GetDetectorIndex());}
    // Returns the detector type as a string
    const char * GetModuleTypeName(Int_t index)const{
        return GetDetectorTypeName(GetModuleType(index));} 
    // Returns the detector type as a string
    const char * GetDetectorTypeName(Int_t index)const{switch(index) {
    case kSPD : return "kSPD" ; case kSDD : return "kSDD" ;
    case kSSD : return "kSSD" ; case kSSDp: return "kSSDp";
    case kSDDp: return "kSDDp"; default   : return "Undefined";};}
    //
    Int_t GetStartDet(Int_t dtype )const;
    Int_t GetLastDet(Int_t dtype)const;
    //     Returns the starting module index number for SPD detector,
    // assuming the modules are placed in the "standard" cylindrical
    // TARGET structure.
    Int_t GetStartSPD()const{return GetStartDet(kSPD);}
    //     Returns the ending module index number for SPD detector,
    // assuming the modules are placed in the "standard" cylindrical
    // TARGET structure.
    Int_t GetLastSPD()const{return GetLastDet(kSPD);}
    //     Returns the starting module index number for SDD detector,
    // assuming the modules are placed in the "standard" cylindrical
    // TARGET structure.
    Int_t GetStartSDD()const{return GetStartDet(kSDD);}
    //     Returns the ending module index number for SDD detector,
    // assuming the modules are placed in the "standard" cylindrical
    // TARGET structure.
    Int_t GetLastSDD()const{return GetLastDet(kSDD);}
    //     Returns the starting module index number for SSD detector,
    // assuming the modules are placed in the "standard" cylindrical
    // TARGET structure.
    Int_t GetStartSSD()const{return GetStartDet(kSSD);}
    //     Returns the ending module index number for SSD detector,
    // assuming the modules are placed in the "standard" cylindrical
    // TARGET structure.
    Int_t GetLastSSD()const{return GetLastDet(kSSD);}
    //     Returns the last module index number.
    Int_t GetIndexMax() const {return fNmodules;}
    //
    //     This function returns the rotation angles for a give module 
    // in the Double point array ang[3]. The angles are in radians
    void  GetAngles(Int_t index,Double_t *ang)const{
        GetGeomMatrix(index)->GetAngles(ang);}
    //     This function returns the rotation angles for a give module
    // in the three floating point variables provided. rx = frx,
    // fy = fry, rz = frz. The angles are in radians
    void  GetAngles(Int_t index,Float_t &rx,Float_t &ry,Float_t &rz)const{
        Double_t a[3];GetAngles(index,a);rx = a[0];ry = a[1];rz = a[2];}
    //     This function returns the rotation angles for a give detector on
    // a give ladder in a give layer in the three floating point variables
    // provided. rx = frx, fy = fry, rz = frz. The angles are in radians
    void  GetAngles(Int_t lay,Int_t lad,Int_t det,
                    Float_t &rx,Float_t &ry,Float_t &rz)const{
        GetAngles(GetModuleIndex(lay,lad,det),rx,ry,rz);}
    //
    //     This function returns the 6 GEANT rotation angles for a give 
    // module in the double point array ang[3]. The angles are in degrees
    void  GetGeantAngles(Int_t index,Double_t *ang)const{
	GetGeomMatrix(index)->SixAnglesFromMatrix(ang);}
    //
    //     This function returns the Cartesian translation for a give
    // module in the Double array t[3]. The units are
    // those of the Monte Carlo, generally cm.
    void  GetTrans(Int_t index,Double_t *t)const{
        GetGeomMatrix(index)->GetTranslation(t);}
    //     This function returns the Cartesian translation for a give
    // module index in the three floating point variables provided.
    // x = fx0, y = fy0, z = fz0. The units are those of the Mont
    // Carlo, generally cm.
    void  GetTrans(Int_t index,Float_t &x,Float_t &y,Float_t &z)const{
        Double_t t[3];GetTrans(index,t);x = t[0];y = t[1];z = t[2];}
    //     This function returns the Cartesian translation for a give
    // detector on a give ladder in a give layer in the three floating
    // point variables provided. x = fx0, y = fy0, z = fz0. The units are
    // those of the Monte Carlo, generally cm.
    void  GetTrans(Int_t lay,Int_t lad,Int_t det,
                   Float_t &x,Float_t &y,Float_t &z)const{
        GetTrans(GetModuleIndex(lay,lad,det),x,y,z);}
    //
    //     This function returns the Cartesian translation for a give
    // module in the Double array t[3]. The units are
    // those of the Monte Carlo, generally cm.
    void  GetTransCyln(Int_t index,Double_t *t)const{
        GetGeomMatrix(index)->GetTranslationCylinderical(t);}
    //     This function returns the Cartesian translation for a give
    // module index in the three floating point variables provided.
    // x = fx0, y = fy0, z = fz0. The units are those of the Mont
    // Carlo, generally cm.
    void  GetTransCyln(Int_t index,Float_t &x,Float_t &y,Float_t &z)const{
        Double_t t[3];GetTransCyln(index,t);x = t[0];y = t[1];z = t[2];}
    //     This function returns the Cartesian translation for a give
    // detector on a give ladder in a give layer in the three floating
    // point variables provided. x = fx0, y = fy0, z = fz0. The units are
    // those of the Monte Carlo, generally cm.
    void  GetTransCyln(Int_t lay,Int_t lad,Int_t det,
                       Float_t &x,Float_t &y,Float_t &z)const{
        GetTransCyln(GetModuleIndex(lay,lad,det),x,y,z);}
    //
    //      This function returns the Cartesian translation [cm] and the
    // 6 GEANT rotation angles [degrees]for a given layer ladder and
    // detector number, in the TVector x (at least 9 elements large).
    // This function is required to be in-lined for speed.
    void  GetCenterThetaPhi(Int_t lay,Int_t lad,Int_t det,TVector &x)const{
        Double_t t[3],a[6];Int_t i=GetModuleIndex(lay,lad,det);GetTrans(i,t);
        GetGeantAngles(i,a);x(0)=t[0];x(1)=t[1];x(2)=t[2];x(3)=a[0];x(4)=a[1];
        x(5)=a[2];x(6)=a[3];x(7)=a[4];x(8)=a[5];}
    //
    //     This function returns the rotation matrix in Double
    // precision for a given module.
    void  GetRotMatrix(Int_t index,Double_t mat[3][3])const{
        GetGeomMatrix(index)->GetMatrix(mat);}
    //     This function returns the rotation matrix in a Double
    // precision pointer for a given module. mat[i][j] => mat[3*i+j].
    void  GetRotMatrix(Int_t index,Double_t *mat)const{
        Double_t rot[3][3];GetRotMatrix(index,rot);
        for(Int_t i=0;i<3;i++)for(Int_t j=0;j<3;j++) mat[3*i+j] = rot[i][j];}
    //     This function returns the rotation matrix in a floating 
    // precision pointer for a given layer ladder and detector module.
    // mat[i][j] => mat[3*i+j].
    void  GetRotMatrix(Int_t lay,Int_t lad,Int_t det,Float_t *mat)const{
        GetRotMatrix(GetModuleIndex(lay,lad,det),mat);}
    //     This function returns the rotation matrix in a Double
    // precision pointer for a given layer ladder and detector module.
    // mat[i][j] => mat[3*i+j].
    void  GetRotMatrix(Int_t lay,Int_t lad,Int_t det,Double_t *mat)const{
        GetRotMatrix(GetModuleIndex(lay,lad,det),mat);}
    //     This function returns the rotation matrix in a floating
    // precision pointer for a given module. mat[i][j] => mat[3*i+j].
    void  GetPixelDimension(Int_t lay,Float_t &pixelx,Float_t &pixely,Float_t &pixelz) {
      pixelx=fPixelX.At(lay); pixely=fPixelY.At(lay); pixelz=fPixelZ.At(lay); }
    void  GetNPixelX(Int_t lay,Int_t &npixelx) {
      npixelx=fNPixelX.At(lay); }
    void  GetNPixelY(Int_t lay,Int_t &npixely) {
      npixely=fNPixelY.At(lay); }
    void  GetNPixelZ(Int_t lay,Int_t &npixelz) {
      npixelz=fNPixelZ.At(lay); }



    void  GetRotMatrix(Int_t index,Float_t *mat)const{
          Double_t rot[3][3];
          GetGeomMatrix(index)->GetMatrix(rot);
          for(Int_t i=0;i<3;i++)for(Int_t j=0;j<3;j++) mat[3*i+j] = rot[i][j];}
    //     This function sets the rotation matrix in a Double
    // precision pointer for a given module. mat[i][j] => mat[3*i+j].
    void  SetPixelDimension(Int_t lay,Float_t pixelx=20.,Float_t pixely=20.,Float_t pixelz=20.) {
          fPixelX.AddAt(pixelx,lay);fPixelY.AddAt(pixely,lay); fPixelZ.AddAt(pixelz,lay);}  
    void  SetNPixelX(Int_t lay,Int_t npixelx) {
          fNPixelX.AddAt(npixelx,lay);}  
    void  SetNPixelY(Int_t lay,Int_t npixely) {
          fNPixelY.AddAt(npixely,lay);}  
    void  SetNPixelZ(Int_t lay,Int_t npixelz) {
          fNPixelZ.AddAt(npixelz,lay);}  

    void  SetRotMatrix(Int_t index,Double_t *mat){Double_t rot[3][3];
          for(Int_t i=0;i<3;i++)for(Int_t j=0;j<3;j++) rot[i][j]=mat[3*i+j];
          GetGeomMatrix(index)->SetMatrix(rot);}
    //
    //     Will define fShape if it isn't already defined.
    void DefineShapes(Int_t size=34){fShape.Expand(size);}
    //     this function returns a pointer to the array of detector
    // descriptions, Segmentation.
    virtual TObjArray *GetShapeArray(){return &fShape;};
    //     this function returns a pointer to the class describing a particular
    // detector type based on IlcTARGETDetector value. This will return a pointer
    // to one of the classes IlcTARGETgeomSPD, IlcTARGETgeomSDD, or 
    // IlcTARGETgeomSSD, for example.
    virtual TObject *GetShape(IlcTARGETDetector idet){
      return fShape.UncheckedAt((Int_t)idet);};
    virtual TObject *GetShape2(Int_t lay){
      return fShape2.UncheckedAt((Int_t)lay);};
    virtual TObject *GetShape2(Int_t lay) const{
      return fShape2.UncheckedAt((Int_t)lay);};


    virtual TObject *GetShape(IlcTARGETDetector idet)const{
      return fShape.UncheckedAt((Int_t)idet);};
    //     This function returns a pointer to the class describing the
    // detector for a particular module index. This will return a pointer
    // to one of the classes IlcTARGETgeomSPD, IlcTARGETgeomSDD, 
    // or IlcTARGETgeomSSD, for example.
    virtual TObject *GetShape(Int_t index){
      return fShape.UncheckedAt(GetGeomMatrix(index)->GetDetectorIndex());}
    virtual TObject *GetShape(Int_t index)const{
      return fShape.UncheckedAt(GetGeomMatrix(index)->GetDetectorIndex());}
    //     This function returns a pointer to the class describing the
    // detector for a particular layer ladder and detector numbers. This
    // will return a pointer to one of the classes IlcTARGETgeomSPD,
    // IlcTARGETgeomSDD, or IlcTARGETgeomSSD, for example.
    virtual TObject *GetShape(Int_t lay,Int_t lad,Int_t det)
        {return GetShape(GetModuleIndex(lay,lad,det));}
    //
    //  Setters
    //     Sets the rotation angles and matrix for a give module index
    // via the double precision array a[3] [radians].
    void SetByAngles(Int_t index,const Double_t a[]){
        GetGeomMatrix(index)->SetAngles(a);}
    //     Sets the rotation angles and matrix for a give module index
    // via the 3 floating precision variables rx, ry, and rz [radians].
    void SetByAngles(Int_t index,Float_t rx, Float_t ry, Float_t rz) {
        Double_t a[3];a[0] = rx;a[1] = ry;a[2] = rz;
        GetGeomMatrix(index)->SetAngles(a);}
    //     Sets the rotation angles and matrix for a give layer, ladder,
    // and detector numbers via the 3 floating precision variables rx,
    // ry, and rz [radians].
    void SetByAngles(Int_t lay,Int_t lad,Int_t det,
                     Float_t rx, Float_t ry, Float_t rz) {
        SetByAngles(GetModuleIndex(lay,lad,det),rx,ry,rz);}
    //
    //     Sets the rotation angles and matrix for a give module index
    // via the Double precision array a[6] [degree]. The angles are those
    // defined by GEANT 3.12.
    void SetByGeantAngles(Int_t index,const Double_t *ang){
        GetGeomMatrix(index)->MatrixFromSixAngles(ang);}
    //     Sets the rotation angles and matrix for a give layer, ladder
    // and detector, in the array id[3] via the Double precision array
    // a[6] [degree]. The angles are those defined by GEANT 3.12.
    void SetByGeantAngles(const Int_t *id,const Double_t *ang){
        SetByGeantAngles(GetModuleIndex(id),ang);}
    //     Sets the rotation angles and matrix for a give layer, ladder
    // and detector, via the Double precision array a[6] [degree]. The
    // angles are those defined by GEANT 3.12.
    void SetByGeantAngles(Int_t lay,Int_t lad,Int_t det,
                          const Double_t *ang){
        SetByGeantAngles(GetModuleIndex(lay,lad,det),ang);}
    //
    //     This function sets a new translation vector, given by the
    // array x[3], for the Cartesian coordinate transformation
    // for a give module index.
    void SetTrans(Int_t index,Double_t x[]){
        GetGeomMatrix(index)->SetTranslation(x);}
    //     This function sets a new translation vector, given by the three
    // variables x, y, and z, for the Cartesian coordinate transformation
    // for the detector defined by layer, ladder and detector.
    void SetTrans(Int_t lay,Int_t lad,Int_t det,
                  Float_t x,Float_t y,Float_t z){Double_t t[3];
                  t[0] = x;t[1] = y;t[2] = z;
                  SetTrans(GetModuleIndex(lay,lad,det),t);}
    //
    //     This function adds one more shape element to the TObjArray
    // fShape. It is primarily used in the constructor functions of the
    // IlcTARGETgeom class. The pointer *shape can be the pointer to any
    // class that is derived from TObject (this is true for nearly every
    // ROOT class). This does not appear to be working properly at this time.
    void AddShape(TObject *shp){fShape.AddLast(shp);}
    //     This function deletes an existing shape element, of type TObject,
    // and replaces it with the one specified. This is primarily used to
    // changes the parameters to the geom class for a particular
    // type of detector.
    void ReSetShape(Int_t dtype,TObject *shp){
      delete (fShape.UncheckedAt(dtype));fShape.AddAt(shp,dtype);}
    void ReSetShape2(Int_t lay,TObject *shp){
      delete (fShape2.UncheckedAt(lay));fShape2.AddAt(shp,lay);}

    //
    //  transformations
    //     Transforms from the ILC Global coordinate system
    // to the detector local coordinate system for the detector
    // defined by the layer, ladder, and detector numbers. The
    // global and local coordinate are given in two floating point
    // arrays g[3], and l[3].
    void GtoL(Int_t lay,Int_t lad,Int_t det,
              const Float_t *g,Float_t *l)const{
        GtoL(GetModuleIndex(lay,lad,det),g,l);}
    //     Transforms from the ILC Global coordinate system
    // to the detector local coordinate system for the detector
    // defined by the id[0], id[1], and id[2] numbers. The
    // global and local coordinate are given in two floating point
    // arrays g[3], and l[3].
    void GtoL(const Int_t *id,const Float_t *g,Float_t *l)const{
        GtoL(GetModuleIndex(id),g,l);}
    //     Transforms from the ILC Global coordinate system
    // to the detector local coordinate system for the detector
    // module index number. The global and local coordinate are
    // given in two floating point arrays g[3], and l[3].
    void GtoL(Int_t index,const Float_t *g,Float_t *l)const{
        Double_t dg[3],dl[3];Int_t i;for(i=0;i<3;i++) dg[i] = g[i];
        GetGeomMatrix(index)->GtoLPosition(dg,dl);
        for(i=0;i<3;i++) l[i] =dl[i];}
    //     Transforms from the ILC Global coordinate system
    // to the detector local coordinate system for the detector
    // defined by the layer, ladder, and detector numbers. The
    // global and local coordinate are given in two Double point
    // arrays g[3], and l[3].
    void GtoL(Int_t lay,Int_t lad,Int_t det,
              const Double_t *g,Double_t *l)const{
        GtoL(GetModuleIndex(lay,lad,det),g,l);}
    //     Transforms from the ILC Global coordinate system
    // to the detector local coordinate system for the detector
    // defined by the id[0], id[1], and id[2] numbers. The
    // global and local coordinate are given in two Double point
    // arrays g[3], and l[3].
    void GtoL(const Int_t *id,const Double_t *g,Double_t *l)const{
        GtoL(GetModuleIndex(id),g,l);}
    //     Transforms from the ILC Global coordinate system
    // to the detector local coordinate system for the detector
    // module index number. The global and local coordinate are
    // given in two Double point arrays g[3], and l[3].
    void GtoL(Int_t index,const Double_t g[3],Double_t l[3])const{
        GetGeomMatrix(index)->GtoLPosition(g,l);}
    //
    //     Transforms from the ILC Global coordinate system
    // to the detector local coordinate system (used for TARGET tracking)
    // for the detector module index number. The global and local
    // coordinate are given in two Double point arrays g[3], and l[3].
    void GtoLtracking(Int_t index,const Double_t *g,Double_t *l)const{
        if(IsGeantToTracking()) GtoL(index,g,l);
        else GetGeomMatrix(index)->GtoLPositionTracking(g,l);}
    //     Transforms from the ILC Global coordinate system
    // to the detector local coordinate system (used for TARGET tracking)
    // for the detector id[3]. The global and local
    // coordinate are given in two Double point arrays g[3], and l[3].
    void GtoLtracking(const Int_t *id,const Double_t *g,Double_t *l)const{
        GtoLtracking(GetModuleIndex(id),g,l);}
    //     Transforms from the ILC Global coordinate system
    // to the detector local coordinate system (used for TARGET tracking)
    // for the detector layer ladder and detector numbers. The global
    // and local coordinate are given in two Double point arrays g[3],
    // and l[3].
    void GtoLtracking(Int_t lay,Int_t lad,Int_t det,
                      const Double_t *g,Double_t *l)const{
        GtoLtracking(GetModuleIndex(lay,lad,det),g,l);}
    //
    //     Transforms of momentum types of quantities from the ILC
    // Global coordinate system to the detector local coordinate system
    // for the detector layer ladder and detector numbers. The global
    // and local coordinate are given in two float point arrays g[3],
    // and l[3].
    void GtoLMomentum(Int_t lay,Int_t lad,Int_t det,
                      const Float_t *g,Float_t *l)const{
        GtoLMomentum(GetModuleIndex(lay,lad,det),g,l);}
    //     Transforms of momentum types of quantities from the ILC
    // Global coordinate system to the detector local coordinate system
    // for the detector module index number. The global and local
    // coordinate are given in two float point arrays g[3], and l[3].
    void GtoLMomentum(Int_t index,const Float_t *g,Float_t *l)const{
        Double_t dg[3],dl[3];Int_t i;for(i=0;i<3;i++) dg[i] = g[i];
        GetGeomMatrix(index)->GtoLMomentum(dg,dl);
        for(i=0;i<3;i++) l[i] =dl[i];}
    //     Transforms of momentum types of quantities from the ILC
    // Global coordinate system to the detector local coordinate system
    // for the detector layer ladder and detector numbers. The global
    // and local coordinate are given in two Double point arrays g[3],
    // and l[3].
    void GtoLMomentum(Int_t lay,Int_t lad,Int_t det,
                      const Double_t *g,Double_t *l)const{
        GtoLMomentum(GetModuleIndex(lay,lad,det),g,l);}
    //     Transforms of momentum types of quantities from the ILC
    // Global coordinate system to the detector local coordinate system
    // for the detector module index number. The global and local
    // coordinate are given in two Double point arrays g[3], and l[3].
    void GtoLMomentum(Int_t index,const Double_t *g,Double_t *l)const{
        Double_t dg[3],dl[3];Int_t i;for(i=0;i<3;i++) dg[i] = g[i];
        GetGeomMatrix(index)->GtoLMomentum(dg,dl);
        for(i=0;i<3;i++) l[i] =dl[i];}
    //
    //     Transforms of momentum types of quantities from the ILC
    // Global coordinate system to the detector local coordinate system
    // (used for TARGET tracking) for the detector module index number.
    // The global and local coordinate are given in two Double point
    // arrays g[3], and l[3].
    void GtoLMomentumTracking(Int_t index,const Double_t *g,Double_t *l)const{
        if(IsGeantToTracking()) GtoLMomentum(index,g,l);
        else GetGeomMatrix(index)->GtoLMomentumTracking(g,l);}
    //     Transforms of momentum types of quantities from the ILC
    // Global coordinate system to the detector local coordinate system
    // (used for TARGET tracking) for the detector id[3].
    // The global and local coordinate are given in two Double point
    // arrays g[3], and l[3].
    void GtoLMomentumTracking(const Int_t *id,
			      const Double_t *g,Double_t *l)const{
        GtoLMomentumTracking(GetModuleIndex(id),g,l);}
    //     Transforms of momentum types of quantities from the ILC
    // Global coordinate system to the detector local coordinate system
    // (used for TARGET tracking) for the detector layer ladder and detector
    // numbers. The global and local coordinate are given in two Double point
    // arrays g[3], and l[3].
    void GtoLMomentumTracking(Int_t lay,Int_t lad,Int_t det,
                              const Double_t *g,Double_t *l)const{
        GtoLMomentumTracking(GetModuleIndex(lay,lad,det),g,l);}
    //
    //     Transforms from the detector local coordinate system
    // to the ILC Global coordinate  system for the detector
    // defined by the layer, ladder, and detector numbers. The
    // global and local coordinate are given in two floating point
    // arrays g[3], and l[3].
    void LtoG(Int_t lay,Int_t lad,Int_t det,
              const Float_t *l,Float_t *g)const{
        LtoG(GetModuleIndex(lay,lad,det),l,g);}
    //     Transforms from the detector local coordinate system
    // to the ILC Global coordinate system for the detector
    // defined by the id[0], id[1], and id[2] numbers. The
    // global and local coordinate are given in two floating point
    // arrays g[3], and l[3].
    void LtoG(const Int_t *id,const Float_t *l,Float_t *g)const{
        LtoG(GetModuleIndex(id),l,g);}
    //     Transforms from the detector local coordinate system
    // to the ILC Global coordinate system for the detector
    // module index number. The global and local coordinate are
    // given in two floating point arrays g[3], and l[3].
    void LtoG(Int_t index,const Float_t *l,Float_t *g)const{
        Double_t dg[3],dl[3];Int_t i;for(i=0;i<3;i++) dl[i] = l[i];
        GetGeomMatrix(index)->LtoGPosition(dl,dg);
        for(i=0;i<3;i++) g[i] =dg[i];}
    //     Transforms from the detector local coordinate system
    // to the ILC Global coordinate system for the detector
    // defined by the layer, ladder, and detector numbers. The
    // global and local coordinate are given in two Double point
    // arrays g[3], and l[3].
    void LtoG(Int_t lay,Int_t lad,Int_t det,
              const Double_t *l,Double_t *g)const{
        LtoG(GetModuleIndex(lay,lad,det),l,g);}
    //     Transforms from the detector local coordinate system
    // to the ILC Global coordinate system for the detector
    // defined by the id[0], id[1], and id[2] numbers. The
    // global and local coordinate are given in two Double point
    // arrays g[3], and l[3].
    void LtoG(const Int_t *id,const Double_t *l,Double_t *g)const{
        LtoG(GetModuleIndex(id),l,g);}
    //     Transforms from the detector local coordinate system
    // to the ILC Global coordinate system for the detector
    // module index number. The global and local coordinate are
    // given in two Double point arrays g[3], and l[3].
    void LtoG(Int_t index,const Double_t *l,Double_t *g)const{
        GetGeomMatrix(index)->LtoGPosition(l,g);}
    //
    //     Transforms from the detector local coordinate system (used
    // for TARGET tracking) to the ILC Global coordinate system 
    // for the detector module index number. The global and local
    // coordinate are given in two Double point arrays g[3], and l[3].
    void LtoGtracking(Int_t index,const Double_t *l,Double_t *g)const{
        if(IsGeantToTracking()) LtoG(index,l,g);
        else GetGeomMatrix(index)->LtoGPositionTracking(l,g);}
    //     Transforms from the detector local coordinate system (used
    // for TARGET tracking) to the ILC Global coordinate system 
    // for the detector id[3]. The global and local
    // coordinate are given in two Double point arrays g[3], and l[3].
    void LtoGtracking(const Int_t *id,const Double_t *l,Double_t *g)const{
        LtoGtracking(GetModuleIndex(id),l,g);}
    //     Transforms from the detector local coordinate system (used
    // for TARGET tracking) to the detector local coordinate system
    // for the detector layer ladder and detector numbers. The global
    // and local coordinate are given in two Double point arrays g[3],
    // and l[3].
    void LtoGtracking(Int_t lay,Int_t lad,Int_t det,
                      const Double_t *l,Double_t *g)const{
        LtoGtracking(GetModuleIndex(lay,lad,det),l,g);}
    //
    //     Transforms of momentum types of quantities from the detector
    // local coordinate system to the ILC Global coordinate system
    // for the detector layer ladder and detector numbers. The global
    // and local coordinate are given in two float point arrays g[3],
    // and l[3].
    void LtoGMomentum(Int_t lay,Int_t lad,Int_t det,
                      const Float_t *l,Float_t *g)const{
        LtoGMomentum(GetModuleIndex(lay,lad,det),l,g);}
    //     Transforms of momentum types of quantities from the detector
    // local coordinate system to the ILC Global coordinate system
    // for the detector module index number. The global and local
    // coordinate are given in two float point arrays g[3], and l[3].
    void LtoGMomentum(Int_t index,const Float_t *l,Float_t *g)const{
        Double_t dg[3],dl[3];Int_t i;for(i=0;i<3;i++) dl[i] = l[i];
        GetGeomMatrix(index)->LtoGMomentum(dl,dg);
        for(i=0;i<3;i++) g[i] =dg[i];}
    //     Transforms of momentum types of quantities from the detector
    // local coordinate system to the ILC Global coordinate system
    // for the detector layer ladder and detector numbers. The global
    // and local coordinate are given in two Double point arrays g[3],
    // and l[3].
    void LtoGMomentum(Int_t lay,Int_t lad,Int_t det,
                      const Double_t *l,Double_t *g)const{
        LtoGMomentum(GetModuleIndex(lay,lad,det),l,g);}
    //     Transforms of momentum types of quantities from the detector
    // local coordinate system to the ILC Global coordinate system
    // for the detector module index number. The global and local
    // coordinate are given in two Double point arrays g[3], and l[3].
    void LtoGMomentum(Int_t index,const Double_t *l,Double_t *g)const{
        GetGeomMatrix(index)->LtoGMomentum(l,g);}
    //
    //     Transforms of momentum types of quantities from the detector 
    // local coordinate system (used for TARGET tracking) to the detector
    // system ILC Global for the detector module index number.
    // The global and local coordinate are given in two Double point
    // arrays g[3], and l[3].
    void LtoGMomentumTracking(Int_t index,const Double_t *l,Double_t *g)const{
        if(IsGeantToTracking()) LtoGMomentum(index,l,g);
        else GetGeomMatrix(index)->LtoGMomentumTracking(l,g);}
    //     Transforms of momentum types of quantities from the detector
    // local coordinate system (used for TARGET tracking) to the ILC
    // Global coordinate system for the detector id[3].
    // The global and local coordinate are given in two Double point
    // arrays g[3], and l[3].
    void LtoGMomentumTracking(const Int_t *id,const Double_t *l,Double_t *g)const{
        LtoGMomentumTracking(GetModuleIndex(id),l,g);}
    //     Transforms of momentum types of quantities from the detector
    // local coordinate system (used for TARGET tracking) to the ILC
    // Global coordinate system for the detector layer ladder and detector
    // numbers. The global and local coordinate are given in two Double point
    // arrays g[3], and l[3].
    void LtoGMomentumTracking(Int_t lay,Int_t lad,Int_t det,
                              const Double_t *l,Double_t *g)const{
                        LtoGMomentumTracking(GetModuleIndex(lay,lad,det),l,g);}
    //
    //     Transforms from one detector local coordinate system
    // to another detector local coordinate system for the detector
    // module index1 number to the detector module index2 number. The
    //  local coordinates are given in two Double point arrays l1[3],
    // and l2[3].
    void LtoL(Int_t index1,Int_t index2,Double_t *l1,Double_t *l2)const{
        Double_t g[3]; LtoG(index1,l1,g);GtoL(index2,g,l2);}
    //     Transforms from one detector local coordinate system
    // to another detector local coordinate system for the detector
    // id1[3] to the detector id2[3]. The local coordinates are given
    // in two Double point arrays l1[3], and l2[3].
    void LtoL(const Int_t *id1,const Int_t *id2,Double_t *l1,Double_t *l2)const{
        LtoL(GetModuleIndex(id1[0],id1[1],id1[2]),
              GetModuleIndex(id2[0],id2[1],id2[2]),l1,l2);}
    //
    //     Transforms from one detector local coordinate system (used for
    // TARGET tracking) to another detector local coordinate system (used
    // for TARGET tracking) for the detector module index1 number to the
    // detector module index2 number. The local coordinates are given
    // in two Double point arrays l1[3], and l2[3].
    void LtoLtracking(Int_t index1,Int_t index2,
                      Double_t *l1,Double_t *l2)const{
        Double_t g[3]; LtoGtracking(index1,l1,g);GtoLtracking(index2,g,l2);}
    //     Transforms from one detector local coordinate system (used for
    // TARGET tracking) to another detector local coordinate system (used
    // for TARGET tracking) for the detector id1[3] to the detector id2[3].
    // The local coordinates are given in two Double point arrays l1[3],
    // and l2[3].
    void LtoLtracking(const Int_t *id1,const Int_t *id2,
                      Double_t *l1,Double_t *l2)const{
        LtoLtracking(GetModuleIndex(id1[0],id1[1],id1[2]),
                     GetModuleIndex(id2[0],id2[1],id2[2]),l1,l2);}
    //
    //     Transforms of momentum types of quantities from one detector
    // local coordinate system to another detector local coordinate
    // system for the detector module index1 number to the detector
    // module index2 number. The local coordinates are given in two
    // Double point arrays l1[3], and l2[3].
    void LtoLMomentum(Int_t index1,Int_t index2,
                      const Double_t *l1,Double_t *l2)const{
        Double_t g[3]; LtoGMomentum(index1,l1,g);GtoLMomentum(index2,g,l2);}
    //     Transforms of momentum types of quantities from one detector
    // local coordinate system to another detector local coordinate
    // system for the detector id1[3] to the detector id2[3]. The local
    // coordinates are given in two Double point arrays l1[3], and l2[3].
    void LtoLMomentum(const Int_t *id1,const Int_t *id2,
                      const Double_t *l1,Double_t *l2)const{
        LtoLMomentum(GetModuleIndex(id1[0],id1[1],id1[2]),
                     GetModuleIndex(id2[0],id2[1],id2[2]),l1,l2);}
    //
    //     Transforms of momentum types of quantities from one detector
    // local coordinate system (used by TARGET tracking) to another detector
    // local coordinate system (used by TARGET tracking) for the detector
    // module index1 number to the detector module index2 number. The
    // local coordinates are given in two Double point arrays l1[3],
    // and l2[3].
    void LtoLMomentumTracking(Int_t index1,Int_t index2,
                              Double_t *l1,Double_t *l2)const{
        Double_t g[3]; LtoGMomentumTracking(index1,l1,g);
        GtoLMomentumTracking(index2,g,l2);}
    //     Transforms of momentum types of quantities from one detector
    // local coordinate system (used by TARGET tracking) to another detector
    // local coordinate system (used by TARGET tracking) for the detector
    // id1[3] to the detector id2[3]. The local coordinates are given in
    // two Double point arrays l1[3], and l2[3].
    void LtoLMomentumTracking(const Int_t *id1,const Int_t *id2,
                              Double_t *l1,Double_t *l2)const{
        LtoLMomentumTracking(GetModuleIndex(id1[0],id1[1],id1[2]),
                              GetModuleIndex(id2[0],id2[1],id2[2]),l1,l2);}
    //
    //     Transforms a matrix, like an Uncertainty or Error matrix from
    // the ILC Global coordinate system to a detector local coordinate
    // system. The specific detector is determined by the module index
    // number.
    void GtoLErrorMatrix(Int_t index,const Double_t g[3][3],Double_t l[3][3])const{
        GetGeomMatrix(index)->GtoLPositionError(g,l);}
    //
    //     Transforms a matrix, like an Uncertainty or Error matrix from
    // the ILC Global coordinate system to a detector local coordinate
    // system (used by TARGET tracking). The specific detector is determined
    // by the module index number.
    void GtoLErrorMatrixTracking(Int_t index,const Double_t g[3][3],
                                 Double_t l[3][3])const{
        if(IsGeantToTracking()) GetGeomMatrix(index)->GtoLPositionError(g,l);
        else GetGeomMatrix(index)->GtoLPositionErrorTracking(g,l);}
    //
    //     Transforms a matrix, like an Uncertainty or Error matrix from
    // the detector local coordinate system to a ILC Global coordinate
    // system. The specific detector is determined by the module index
    // number.
    void LtoGErrorMatrix(Int_t index,const Double_t l[3][3],Double_t g[3][3])const{
        GetGeomMatrix(index)->LtoGPositionError(l,g);}
    //
    //     Transforms a matrix, like an Uncertainty or Error matrix from
    // the detector local coordinate system (used by TARGET tracking) to a
    // ILC Global coordinate system. The specific detector is determined
    // by the module index number.
    void LtoGErrorMatrixTracking(Int_t index,const Double_t l[3][3],
                                 Double_t g[3][3])const{
      if(IsGeantToTracking()) GetGeomMatrix(index)->LtoGPositionError(l,g);
      else GetGeomMatrix(index)->LtoGPositionErrorTracking(l,g);}
    //
    //     Transforms a matrix, like an Uncertainty or Error matrix from
    // one detector local coordinate system to another detector local
    // coordinate system. The specific detector is determined by the
    // two module index number index1 and index2.
    void LtoLErrorMatrix(Int_t index1,Int_t index2,
                         const Double_t l1[3][3],Double_t l2[3][3])const{
        Double_t g[3][3];
        LtoGErrorMatrix(index1,l1,g);
        GtoLErrorMatrix(index2,g,l2);}
    //
    //     Transforms a matrix, like an Uncertainty or Error matrix from
    // one detector local coordinate system (used by TARGET tracking) to
    // another detector local coordinate system (used by TARGET tracking).
    // The specific detector is determined by the two module index number
    // index1 and index2.
    void LtoLErrorMatrixTraking(Int_t index1,Int_t index2,
                                const Double_t l1[3][3],Double_t l2[3][3])const{
        Double_t g[3][3];
        LtoGErrorMatrixTracking(index1,l1,g);
        GtoLErrorMatrixTracking(index2,g,l2);}
    //  Find Specific Modules
    // Locate the nearest module to the point g, in ILC global Cartesian
    // coordinates [cm] in a give layer. If layer = 0 then it search in
    // all layers.
    Int_t    GetNearest(const Double_t g[3],Int_t lay=0)const;
    // Locates the nearest 27 modules, in nearest order, to the point g, in
    // ILC global Cartesian coordinates [cm] in a give layer. If layer = 0
    // then it searches in all layers. (there are 27 elements in a 3x3x3
    // cube.
    void     GetNearest27(const Double_t g[3],Int_t n[27],Int_t lay=0)const;
    // Returns the distance [cm] between the point g[3] and the center of
    // the detector/module specified by the the module index number.
    Double_t Distance(Int_t index,const Double_t g[3])const{
        return  TMath::Sqrt(GetGeomMatrix(index)->Distance2(g));}
    // loops over modules and computes the average cylindrical
    // radius to a given layer and the range.
    Double_t GetAverageRadiusOfLayer(Int_t layer,Double_t &range)const;
    //  Geometry manipulation
    // This function performs a Cartesian translation and rotation of
    // the full TARGET from its default position by an amount determined by
    // the three element arrays tran and rot.
    void GlobalChange(const Float_t  *tran,const Float_t  *rot);
    // This function performs a Cylindrical translation and rotation of
    // the full TARGET from its default position by an amount determined by
    // the three element arrays tran and rot.
    void GlobalCylindericalChange(const Float_t *tran,const Float_t *rot);
    // This function performs a Gaussian random displacement and/or
    // rotation about the present global position of each active
    // volume/detector of the TARGET with variances given by stran and srot.
    void RandomChange(const Float_t *stran,const Float_t *srot);
    // This function performs a Gaussian random displacement and/or
    // rotation about the present global position of each active
    // volume/detector of the TARGET with variances given by stran and srot.
    // But in Cylindrical coordinates.
    void RandomCylindericalChange(const Float_t *stran,const Float_t *srot);
    // This function converts these transformations from Ilc global and
    // local to Tracking global and local.
    void GeantToTracking(const IlcTARGETgeom &source); // This converts the geometry
    //  Other routines.
    // This routine prints, to a file, the difference between this class
    // and "other".
    void PrintComparison(FILE *fp,IlcTARGETgeom *other)const;
    // This routine prints, to a file, the contents of this class.
    void PrintData(FILE *fp,Int_t lay,Int_t lad,Int_t det)const;
    // This function prints out this class in a single stream. This steam
    // can be read by ReadGeom.
    ofstream &PrintGeom(ofstream &out)const;
    // This function reads in that single steam printed out by PrintGeom.
    ifstream &ReadGeom(ifstream &in);

    //Conversion from det. local coordinates to local ("V2") coordinates
    //used for tracking

    void DetLToTrackingV2(Int_t md, Float_t xin, Float_t zin, Float_t &yout, Float_t &zout); 

    void TrackingV2ToDetL(Int_t md,Float_t yin,Float_t zin,Float_t &xout,Float_t &zout);

 private:
    TString    fVersion; // Transformation version.
    Int_t      fDetVersion;
    Int_t      fTrans;   // Flag to keep track of which transformation 
    Int_t      fNmodules;// The total number of modules
    Int_t      fNlayers; // The number of layers.
    Int_t      fNFakeLayers; // The number of fake layers.
    TArrayF    fDz;
    TArrayF    fPosz1;
    TArrayF    fPosz2;
    TArrayF    fPosz1_Fake;
    TArrayF    fPosz2_Fake;
    TArrayF    fRmin;
    TArrayF    fRmax;
    TArrayI    fNlad;    // Array of the number of ladders/layer(layer)
    TArrayI    fNdet;    // Array of the number of detector/ladder(layer)
    TObjArray  fGm;      // Structure of translation. and rotation.
    TObjArray  fShape;   // Array of shapes and detector information.
    TObjArray  fShape2;   // Array of shapes and detector information.
    TArrayF    fPixelX;
    TArrayF    fPixelY;
    TArrayF    fPixelZ;
    TArrayI    fNPixelX;
    TArrayI    fNPixelY;
    TArrayI    fNPixelZ;

    ClassDef(IlcTARGETgeom,6) // TARGET geometry class
};

#endif
