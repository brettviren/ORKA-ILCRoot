#ifndef ILCTARGETMIXTURE_H
#define ILCTARGETMIXTURE_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/*
  $Id: IlcTARGETBaseGeometry.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $
 */
#include <TGeoMaterial.h>

class IlcTARGETMixture : public TGeoMixture{
 public:
    IlcTARGETMixture(){};
    IlcTARGETMixture(const char *name,Int_t N,Double_t *w,TObjArray *m,
		  Double_t rho=-1.,Double_t radlen=0.,Double_t intleng=0.);
    virtual ~IlcTARGETMixture(){};
 private:
    ClassDef(IlcTARGETMixture,1) // Extension of TGeoMixture class
}
;
#endif

#ifndef ILCTARGETGEOCABLE_H
#define ILCTARGETGEOCABLE_H

/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/*
  $Id: IlcTARGETBaseGeometry.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $
 */
#include <TGeoTube.h>
#include <TObjArray.h>
#include <TObject.h>
#include <TVector3.h>

class IlcTARGETGeoCable : public TObject{
  public:
    IlcTARGETGeoCable();
    IlcTARGETGeoCable(const char *name,const TObjArray *vect,const Double_t Rmin,
                   const Double_t Rmax,const TVector3 ns = TVector3(0.,0.,1.),
                   const TVector3 ne = TVector3(0.,0.,1.));
    virtual ~IlcTARGETGeoCable();
    virtual Double_t GetRmin(){return fRmin;}
    virtual Double_t GetRmax(){return fRmax;}
    virtual TVector3& GetNormStart(){return fNs;}
    virtual TVector3& GetNormEnd(){return fNe;}
    virtual TObjArray* GetArrayOfTubes(){return fTubes;}
    virtual TObjArray* GetArrayOfCombiTrans(){return fTranRot;}
  private:
    Double_t  fRmin; // Minimum radius
    Double_t  fRmax; // Maximum radius
    TVector3  fNs;  // Starting normal vector
    TVector3  fNe;  // Ending normal vector
    TObjArray *fTubes; // Array of Ctub objects
    TObjArray *fTranRot;  // Array of Rotations
    ClassDef(IlcTARGETGeoCable,1) // Extension of TGeoMixture class
        ;
};
#endif
