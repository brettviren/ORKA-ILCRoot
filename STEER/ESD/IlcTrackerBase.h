#ifndef ILCTRACKERBASE_H
#define ILCTRACKERBASE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTrackerBase.h 38069 2009-12-24 16:56:18Z belikov $ */

//-------------------------------------------------------------------------
//                          class IlcTrackerBase
//        This is the base tracker class, independent on STEER 
//                      Origin: Marian.Ivanov@cern.ch
//-------------------------------------------------------------------------

#include <TObject.h>
#include <TGeoGlobalMagField.h>

#include "IlcMagF.h"

class IlcExternalTrackParam;
class IlcTrackPoint;
class IlcTrackPointArray;

class IlcTrackerBase : public TObject {
public:
  IlcTrackerBase();
  virtual ~IlcTrackerBase(){}

  void SetVertex(const Double_t *xyz, const Double_t *ers=0) { 
     fX=xyz[0]; fY=xyz[1]; fZ=xyz[2];
     if (ers) { fSigmaX=ers[0]; fSigmaY=ers[1]; fSigmaZ=ers[2]; } 
  }
  Double_t GetX() const {return fX;}
  Double_t GetY() const {return fY;}
  Double_t GetZ() const {return fZ;}
  Double_t GetSigmaX() const {return fSigmaX;}
  Double_t GetSigmaY() const {return fSigmaY;}
  Double_t GetSigmaZ() const {return fSigmaZ;}

  static Double_t GetTrackPredictedChi2(IlcExternalTrackParam *track,
                                        Double_t mass, Double_t step, 
			          const IlcExternalTrackParam *backup);
  static void UpdateTrack(IlcExternalTrackParam&, 
                          const IlcExternalTrackParam&);
  static 
  Double_t MeanMaterialBudget(const Double_t *start, const Double_t *end, 
  Double_t *mparam);
  static
  Bool_t PropagateTrackTo(IlcExternalTrackParam *track, Double_t x, Double_t m,
			  Double_t maxStep, Bool_t rotateTo=kTRUE, Double_t maxSnp=0.8, Int_t sign=0, Bool_t addTimeStep=kFALSE);  
  static Bool_t PropagateTrackToBxByBz(IlcExternalTrackParam *track, Double_t x, 
				       Double_t m,Double_t maxStep, Bool_t rotateTo=kTRUE, Double_t maxSnp=0.8,Int_t sign=0, Bool_t addTimeStep=kFALSE);  
  //
  static Double_t GetBz(const Double_t *r);
  static void GetBxByBz(const Double_t r[3], Double_t b[3]);
  static Double_t GetBz();
  static Bool_t UniformField();


  static Double_t MakeC(Double_t x1,Double_t y1,
                        Double_t x2,Double_t y2,
                        Double_t x3,Double_t y3);
  static Double_t MakeSnp(Double_t x1,Double_t y1,
                        Double_t x2,Double_t y2,
                        Double_t x3,Double_t y3);
  static Double_t MakeTgl(Double_t x1,Double_t y1,
                        Double_t x2,Double_t y2,
                        Double_t z1,Double_t z2);
  static Double_t MakeTgl(Double_t x1,Double_t y1,
                        Double_t x2,Double_t y2,
                        Double_t z1,Double_t z2,Double_t c);
  static IlcExternalTrackParam *MakeSeed(IlcTrackPoint &point0, 
                                         IlcTrackPoint &point1, 
                                         IlcTrackPoint &point2);
  static Double_t FitTrack(IlcExternalTrackParam *trackParam, 
                           IlcTrackPointArray *pointArray, 
                           Double_t mass, Double_t maxStep);  
  //

protected:
  IlcTrackerBase(const IlcTrackerBase &atr);
private:
  IlcTrackerBase & operator=(const IlcTrackerBase & atr);

  Double_t fX;  //X-coordinate of the primary vertex
  Double_t fY;  //Y-coordinate of the primary vertex
  Double_t fZ;  //Z-coordinate of the primary vertex
 
  Double_t fSigmaX; // error of the primary vertex position in X
  Double_t fSigmaY; // error of the primary vertex position in Y
  Double_t fSigmaZ; // error of the primary vertex position in Z
  
  ClassDef(IlcTrackerBase,1) //base tracker
};

//__________________________________________________________________________
inline Bool_t IlcTrackerBase::UniformField()
{
  IlcMagF* fld = (IlcMagF*)TGeoGlobalMagField::Instance()->GetField();
  return fld ? fld->IsUniform():kTRUE;
}

#endif
