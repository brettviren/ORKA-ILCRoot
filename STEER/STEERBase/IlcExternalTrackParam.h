#ifndef ILCEXTERNALTRACKPARAM_H
#define ILCEXTERNALTRACKPARAM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcExternalTrackParam.h 53891 2012-01-13 10:34:05Z hristov $ */

/*****************************************************************************
 *              "External" track parametrisation class                       *
 *                                                                           *
 *      external param0:   local Y-coordinate of a track (cm)                *
 *      external param1:   local Z-coordinate of a track (cm)                *
 *      external param2:   local sine of the track momentum azimuthal angle  *
 *      external param3:   tangent of the track momentum dip angle           *
 *      external param4:   1/pt (1/(GeV/c))                                  *
 *                                                                           *
 * The parameters are estimated at an exact position x in a local coord.     *
 * system rotated by angle alpha with respect to the global coord.system.    *
 *        Origin: I.Belikov, CERN, Jouri.Belikov@cern.ch                     *
 *****************************************************************************/
#include "TMath.h"

#include "IlcVTrack.h"

const Double_t kSmallPt=0.001;

const Double_t kAlmost0Curv = 2*1e-4/1e6;

const Double_t kVeryBig=1./kAlmost0;
const Double_t kMostProbablePt=0.35;

class IlcESDVertex;

  inline double Phi_mpi_pi(double phi){return phi-TMath::Nint(phi/2*TMath::InvPi())*TMath::TwoPi();}
  inline double Phi_0_2pi(double phi){return phi-TMath::Nint(phi/2*TMath::InvPi()-0.5)*TMath::TwoPi();}

class IlcVVertex;
class TPolyMarker3D; 

const Double_t kC0max=100*100, // SigmaY<=100cm
               kC2max=100*100, // SigmaZ<=100cm
               kC5max=1*1,     // SigmaSin<=1
               kC9max=1*1,     // SigmaTan<=1
               kC14max=100*100; // Sigma1/Pt<=100 1/GeV

class IlcExternalTrackParam: public IlcVTrack {
 public:
  IlcExternalTrackParam();
  IlcExternalTrackParam(const IlcExternalTrackParam &);
  IlcExternalTrackParam& operator=(const IlcExternalTrackParam & trkPar);
  IlcExternalTrackParam(Double_t x, Double_t alpha, 
			const Double_t param[5], const Double_t covar[15]);
  IlcExternalTrackParam(Double_t xyz[3],Double_t pxpypz[3],
			Double_t cv[21],Short_t sign);
  virtual ~IlcExternalTrackParam(){}
  void CopyFromVTrack(const IlcVTrack *vTrack);
  
  template <typename T>
  void Set(T x, T alpha, const T param[5], const T covar[15]) {
    //  Sets the parameters
    if      (alpha < -TMath::Pi()) alpha += 2*TMath::Pi();
    else if (alpha >= TMath::Pi()) alpha -= 2*TMath::Pi();
    fX=x; fAlpha=alpha;
    for (Int_t i = 0; i < 5; i++)  fP[i] = param[i];
    for (Int_t i = 0; i < 15; i++) fC[i] = covar[i];

    CheckCovariance();

  }

  void Set(Double_t xyz[3],Double_t pxpypz[3],Double_t cv[21],Short_t sign);

  static void SetMostProbablePt(Double_t pt) { fgMostProbablePt=pt; }
  static Double_t GetMostProbablePt() { return fgMostProbablePt; }

  void Reset();
  void ResetCovariance(Double_t s2);
  void AddCovariance(const Double_t cov[15]);

  const Double_t *GetParameter() const {return fP;}
  const Double_t *GetCovariance() const {return fC;}
  virtual  Bool_t IsStartedTimeIntegral() const {return kFALSE;}
  virtual  void   AddTimeStep(Double_t ) {} // dummy method, real stuff is done in IlcKalmanTrack
  Double_t GetAlpha() const {return fAlpha;}
//   Double_t GetX() const {return fX;}
  Double_t GetX() const {return kEndCap?fP[1]:fX;}
  Double_t GetY()    const {return fP[0];}
//   Double_t GetZ()    const {return fP[1];}
  Double_t GetZ()    const {return kEndCap?fX:fP[1];}
  Double_t GetSnp()  const {return fP[2];}
  Double_t GetTgl()  const {return fP[3];}
  Double_t GetSigned1Pt()  const {return fP[4];}

  Double_t GetSigmaY2() const {return fC[0];}
  Double_t GetSigmaZY() const {return fC[1];}
  Double_t GetSigmaZ2() const {return fC[2];}
  Double_t GetSigmaSnpY() const {return fC[3];}
  Double_t GetSigmaSnpZ() const {return fC[4];}
  Double_t GetSigmaSnp2() const {return fC[5];}
  Double_t GetSigmaTglY() const {return fC[6];}
  Double_t GetSigmaTglZ() const {return fC[7];}
  Double_t GetSigmaTglSnp() const {return fC[8];}
  Double_t GetSigmaTgl2() const {return fC[9];}
  Double_t GetSigma1PtY() const {return fC[10];}
  Double_t GetSigma1PtZ() const {return fC[11];}
  Double_t GetSigma1PtSnp() const {return fC[12];}
  Double_t GetSigma1PtTgl() const {return fC[13];}
  Double_t GetSigma1Pt2() const {return fC[14];}

  // additional functions for IlcVParticle
  Double_t Px() const;
  Double_t Py() const;
  Double_t Pz() const { return Pt()*GetTgl(); }
  Double_t Pt() const { return TMath::Abs(GetSignedPt()); }
  Double_t P() const { return GetP(); }
  Bool_t   PxPyPz(Double_t p[3]) const { return GetPxPyPz(p); }
  
  Double_t Xv() const;
  Double_t Yv() const;
  Double_t Zv() const {return GetZ();}
  Bool_t   XvYvZv(Double_t x[3]) const { return GetXYZ(x); }

  Double_t OneOverPt() const { return 1./Pt(); }
  Double_t Phi() const;
  Double_t Theta() const;
  virtual Double_t E() const;
  virtual Double_t M() const;
  Double_t Eta() const;
  virtual Double_t Y() const;
  virtual Short_t  Charge() const { return (Short_t)GetSign(); }
  virtual const Double_t *PID() const { return 0x0; }

  // additional functions from IlcVTrack
  virtual Int_t    GetID() const { return -999; }
  virtual UChar_t  GetITSClusterMap() const {return 0; }
  virtual UChar_t  GetVXDClusterMap() const {return 0; }
  virtual ULong_t  GetStatus() const { return 0; }

  Double_t GetSign() const {return (fP[4]>0) ? 1 : -1;}
  Double_t GetP() const;
  Double_t GetSignedPt() const {
    return (TMath::Abs(fP[4])>kAlmost0) ? 1./fP[4]:TMath::Sign(kVeryBig,fP[4]);
  }
  Double_t Get1P() const;
  virtual Double_t GetC(Double_t b) const {return fP[4]*b*kB2C;}
  void GetDZ(Double_t x,Double_t y,Double_t z,Double_t b,Float_t dz[2]) const; 
  Double_t GetD(Double_t xv, Double_t yv, Double_t b) const; 
  Double_t GetLinearD(Double_t xv, Double_t yv) const; 

  Bool_t CorrectForMeanMaterial(Double_t xOverX0, Double_t xTimesRho, 
        Double_t mass,  Bool_t anglecorr=kFALSE,
	Double_t (*f)(Double_t)=IlcExternalTrackParam::BetheBlochSolid);

  Bool_t CorrectForMeanMaterialdEdx(Double_t xOverX0, Double_t xTimesRho, 
	Double_t mass, Double_t dEdx, Bool_t anglecorr=kFALSE);

  Bool_t CorrectForMeanMaterialZA(Double_t xOverX0, Double_t xTimesRho, 
                                  Double_t mass,
                                  Double_t zOverA=0.49848,
                                  Double_t density=2.33,
                                  Double_t exEnergy=173e-9,
                                  Double_t jp1=0.20,
                                  Double_t jp2=3.00,
                                  Bool_t anglecorr=kFALSE
  );

  //
  // Bethe-Bloch formula parameterizations
  //
  static Double_t BetheBlochAleph(Double_t bg,
                                  Double_t kp1=0.76176e-1,
                                  Double_t kp2=10.632,
                                  Double_t kp3=0.13279e-4,
                                  Double_t kp4=1.8631,
                                  Double_t kp5=1.9479
				  );
  static Double_t BetheBlochGeant(Double_t bg,
                                  Double_t kp0=2.33,
                                  Double_t kp1=0.20,
                                  Double_t kp2=3.00,
                                  Double_t kp3=173e-9,
                                  Double_t kp4=0.49848
				  );
    
  static Double_t BetheBlochSolid(Double_t bg);
  static Double_t BetheBlochGas(Double_t bg);

  Double_t GetPredictedChi2(Double_t p[2],Double_t cov[3]) const;

  Double_t GetPredictedChi2(Double_t p[3],Double_t covyz[3],Double_t covxyz[3]) const;

  Double_t GetPredictedChi2(const IlcExternalTrackParam *t) const;

  Bool_t PropagateTo(Double_t p[3],Double_t covyz[3],Double_t covxyz[3],Double_t b);

  Double_t *GetResiduals(Double_t *p,Double_t *cov,Bool_t updated=kTRUE) const;
  Bool_t Update(Double_t p[2],Double_t cov[3]);


  Bool_t Rotate(Double_t alpha,Double_t bz);
  Bool_t RotateZ(bool toendcap,Double_t bz);
  Bool_t PropagateTo(Double_t x, Double_t b,bool byR/*=false*/,bool checkSignX/*=true*/);
  Bool_t Propagate(Double_t alpha, Double_t x, Double_t b,bool byR/*=false*/) {
    if (Rotate(alpha,b))
      if (PropagateTo(x,b,byR,true)) return kTRUE;
    return kFALSE;
  }
  Double_t GetNLoopsToZ(Double_t z,Double_t b);

//   Double_t GetPredictedChi2(Double_t p[3],Double_t covyz[3],Double_t covxyz[3]) const;
//   Bool_t PropagateTo(Double_t p[3],Double_t covyz[3],Double_t covxyz[3],Double_t b);

  void   Propagate(Double_t len,Double_t x[3],Double_t p[3],Double_t bz) const;
  Bool_t Intersect(Double_t pnt[3], Double_t norm[3], Double_t bz) const;

  void GetHelixParameters(Double_t h[6], Double_t b) const;
  Double_t GetDCA(const IlcExternalTrackParam *p, Double_t b,
		  Double_t &xthis,Double_t &xp) const;
  Double_t PropagateToDCA(IlcExternalTrackParam *p, Double_t b);
  Bool_t PropagateToDCA(const IlcESDVertex *vtx, Double_t b, Double_t maxd);

  void GetDirection(Double_t d[3]) const;
  Bool_t GetPxPyPz(Double_t *p) const;
  Double_t GetPhi() const {return fAlpha+TMath::ATan2(fP[0],fX);};
  Double_t GetR() const {return TMath::Hypot(fP[0],fX);};
  Bool_t GetXYZ(Double_t *p,bool local/*=kFALSE*/) const;
  Bool_t GetCovarianceXYZPxPyPz(Double_t cv[21]) const;
  Bool_t GetPxPyPzAt(Double_t x, Double_t b, Double_t p[3]) const;
  Bool_t GetXYZAt(Double_t x, Double_t b, Double_t xyz[3],bool local/*=kFALSE*/) const;
  Bool_t GetXYZAtZ(Double_t z, Double_t b, Double_t xyz[3],bool local=kFALSE) const;
  Bool_t GetXYZAtR(Double_t r, Double_t b, Double_t xyz[3],int dir=0,bool local=kFALSE) const;
  Bool_t GetYAt(Double_t x,  Double_t b,  Double_t &y) const;
  Bool_t GetZAt(Double_t x,  Double_t b,  Double_t &z) const;
  void Print(Option_t* option = "") const;
  Double_t GetSnpAt(Double_t x,Double_t b) const;

  // forr wires
  Bool_t UpdateWithWire(Double_t rztrack[2], Double_t deriv[10],
			Double_t p[2], Double_t cov[3],bool withZ=false);
  Bool_t UpdateWithWire(Double_t r,Double_t phi,Double_t tanstereoangle, Double_t b,
			Double_t p[2], Double_t cov[3],bool withZ=false,bool localZ=false);
  Double_t GetPredictedChi2(Double_t rztrack[2],Double_t deriv[10],
			    Double_t p[2],Double_t cov[3],bool withZ=false) const;
  Double_t GetPredictedChi2(Double_t r,Double_t phi,Double_t tanstereoangle, Double_t b,
			    Double_t p[2],Double_t cov[3],bool withZ=false,bool localZ=false) const;
  Double_t GetDistance2Wire(Double_t r,Double_t phi,Double_t tanstereoangle, Double_t b,
			    Double_t* dderiv=0,Double_t* rztrack=0,bool localZ=false) const;
  Bool_t PropagateToR(Double_t r, Double_t b);
  Bool_t GetXYZAtStereo(Double_t r, Double_t tanstereoangle,Double_t b, Double_t xyz[3],bool local=kFALSE) const;

  void Set(const IlcExternalTrackParam *t);
  Double_t Get1Pt()  const {return fP[4];}

  Bool_t PropagateToWithCorrection(Double_t xToGo, Double_t b, Double_t mass,
				   Double_t maxStep, Bool_t rotateTo, Double_t maxSnp);

  void SetEndCap(bool flag=true){kEndCap=flag;}
  bool IsEndCap() const {return kEndCap;}
  Double_t GetDir() const {return fDir;}
  Double_t GetCosRDir() const {return fDir*TMath::Sqrt(1-fP[2]*fP[2])*fX+fP[2]*fP[0];}

  Bool_t Rotate(Double_t alpha);
  Bool_t PropagateTo(Double_t x, Double_t b);
  Bool_t Propagate(Double_t alpha, Double_t x, Double_t b);
  Bool_t PropagateBxByBz(Double_t alpha, Double_t x, Double_t b[3]);
//   void   Propagate(Double_t len,Double_t x[3],Double_t p[3],Double_t bz) const;
//   Bool_t Intersect(Double_t pnt[3], Double_t norm[3], Double_t bz) const;

  static void g3helx3(Double_t qfield, Double_t step, Double_t vect[7]); 
  Bool_t PropagateToBxByBz(Double_t x, const Double_t b[3]);

//   void GetHelixParameters(Double_t h[6], Double_t b) const;
//   Double_t GetDCA(const IlcExternalTrackParam *p, Double_t b, Double_t &xthis,Double_t &xp) const;
//   Double_t PropagateToDCA(IlcExternalTrackParam *p, Double_t b);
  Bool_t PropagateToDCA(const IlcVVertex *vtx, Double_t b, Double_t maxd,
                        Double_t dz[2]=0, Double_t cov[3]=0);
  Bool_t PropagateToDCABxByBz(const IlcVVertex *vtx, Double_t b[3], 
         Double_t maxd, Double_t dz[2]=0, Double_t cov[3]=0);
  Bool_t ConstrainToVertex(const IlcVVertex* vtx, Double_t b[3]);
  
//   void GetDirection(Double_t d[3]) const;
//   Bool_t GetPxPyPz(Double_t *p) const;
  Bool_t GetXYZ(Double_t *p) const;
//   Bool_t GetCovarianceXYZPxPyPz(Double_t cv[21]) const;
//   Bool_t GetPxPyPzAt(Double_t x, Double_t b, Double_t p[3]) const;
  Bool_t GetXYZAt(Double_t x, Double_t b, Double_t r[3]) const;
//   Bool_t GetYAt(Double_t x,  Double_t b,  Double_t &y) const;
//   Bool_t GetZAt(Double_t x,  Double_t b,  Double_t &z) const;
//   void Print(Option_t* option = "") const;
//   Double_t GetSnpAt(Double_t x,Double_t b) const;

  //Deprecated
  Bool_t CorrectForMaterial(Double_t d, Double_t x0, Double_t mass,
	 Double_t (*f)(Double_t)=IlcExternalTrackParam::BetheBlochSolid);

  Bool_t GetDistance(IlcExternalTrackParam *param2, Double_t x, Double_t dist[3], Double_t b);
  Int_t GetIndex(Int_t i, Int_t j) const;
  Int_t GetLabel() const {return -1;} 
  Int_t PdgCode()  const {return 0;}

  //
  // visualization (M. Ivanov)
  //
  virtual void FillPolymarker(TPolyMarker3D *pol, Float_t magf, Float_t minR, Float_t maxR, Float_t stepR);
  virtual void DrawTrack(Float_t magF, Float_t minR, Float_t maxR, Float_t stepR);

  virtual Bool_t Translate(Double_t *vTrasl,Double_t *covV);

  void CheckCovariance();

  static Bool_t  GetUseLogTermMS()                {return fgUseLogTermMS;} 
  static void    SetUseLogTermMS(Bool_t v=kTRUE)  {fgUseLogTermMS = v;} 

//   Double_t GetDir() const {return fDir;}
//   Double_t GetCosRDir() const {return fDir*TMath::Sqrt(1-fP[2]*fP[2])*fX+fP[2]*fP[0];}

  void Set(Double_t x,Double_t alpha, const Double_t param[5], const Double_t covar[15],Double_t fDirection=1.);


 protected:
  IlcExternalTrackParam(const IlcVTrack *vTrack);

/*  protected: */
 private:
  Double_t &Par(Int_t i) {return fP[i];}
  Double_t &Cov(Int_t i) {return fC[i];}
 private:
  Bool_t               kEndCap; // Propagation in endcap in Z -direction
  Double32_t           fX;     // X coordinate for the point of parametrisation
  Double32_t           fAlpha; // Local <-->global coor.system rotation angle
  Double32_t           fP[5];  // The track parameters
  Double_t             fDir;   // fP[5]- sign of cos(phi)
  Double32_t           fC[15]; // The track parameter covariance matrix

  static Double32_t    fgMostProbablePt; // "Most probable" pt
                                         // (to be used if Bz=0)
  static Bool_t        fgUseLogTermMS;   // use log term in Mult.Stattering evaluation
  ClassDef(IlcExternalTrackParam, 8)
};

inline void IlcExternalTrackParam::ResetCovariance(Double_t s2) {
  //
  // Reset the covarince matrix to "something big"
  //

  s2 = TMath::Abs(s2);
  Double_t fC0=fC[0]*s2,
           fC2=fC[2]*s2,
           fC5=fC[5]*s2,
           fC9=fC[9]*s2,
           fC14=fC[14]*s2;
 
  if (fC0>kC0max)  fC0 = kC0max;
  if (fC2>kC2max)  fC2 = kC2max;
  if (fC5>kC5max)  fC5 = kC5max;
  if (fC9>kC9max)  fC9 = kC9max;
  if (fC14>kC14max)  fC14 = kC14max;


    fC[0] = fC0;
    fC[1] = 0.;  fC[2] = fC2;
    fC[3] = 0.;  fC[4] = 0.;  fC[5] = fC5;
    fC[6] = 0.;  fC[7] = 0.;  fC[8] = 0.;  fC[9] = fC9;
    fC[10]= 0.;  fC[11]= 0.;  fC[12]= 0.;  fC[13]= 0.;  fC[14] = fC14;
}




#endif
