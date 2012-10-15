#ifndef ILCSTRLINE_H
#define ILCSTRLINE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////
//                                                               //
// A straight line is coded as a point (3 Double_t) and           //
// 3 direction cosines                                           //
//                                                               //
///////////////////////////////////////////////////////////////////

#include "IlcLog.h"

class IlcStrLine : public TObject {

public:
    IlcStrLine();        // default constructor
    IlcStrLine(const Double_t *const point, const Double_t *const cd, Bool_t twopoints=kFALSE, UShort_t id1=65535, UShort_t id2=65535);  // standard constructor
    IlcStrLine(const Double_t *const point, const Double_t *const sig2point, const Double_t *const cd, Bool_t twopoints=kFALSE, UShort_t id1=65535, UShort_t id2=65535);
    IlcStrLine(const Double_t *const point, const Double_t *const sig2point, const Double_t *const wmat, const Double_t *const cd, Bool_t twopoints=kFALSE, UShort_t id1=65535, UShort_t id2=65535);

    IlcStrLine(const IlcStrLine& source);
    IlcStrLine& operator=(const IlcStrLine& source);
    virtual ~IlcStrLine(); // destructor
    virtual void Clear(Option_t*){if(fWMatrix)delete[] fWMatrix; fWMatrix = 0;}
    void PrintStatus() const;
    void SetP0(const Double_t *point) {for(Int_t i=0;i<3;i++)fP0[i]=point[i];}
    void SetSigma2P0(const Double_t *sigsq) {for(Int_t i=0;i<3;i++)fSigma2P0[i]=sigsq[i];}
    void SetWMatrix(const Double_t *wmat);
    void SetCd(const Double_t *cd) {for(Int_t i=0;i<3;i++)fCd[i]=cd[i];}
    void SetIdPoints(UShort_t id1, UShort_t id2){
      fIdPoint[0]=id1;
      fIdPoint[1]=id2;
    } 

    void GetP0(Double_t *point) const {for(Int_t i=0;i<3;i++)point[i]=fP0[i];}
    void GetSigma2P0(Double_t *sigsq) const {for(Int_t i=0;i<3;i++)sigsq[i]=fSigma2P0[i];}
    void GetWMatrix(Double_t *wmat) const;
    void GetCd(Double_t *cd) const {for(Int_t i=0;i<3;i++)cd[i]=fCd[i];}
    void GetCurrentPoint(Double_t *point) const;
    UShort_t GetIdPoint(Int_t i) const {
      if(i<2) return fIdPoint[i];
      IlcError("Wrong element: only 2 points are stored in IlcStrLine");
      return 65535;
    }
    Int_t IsParallelTo(const IlcStrLine *line) const;
    Int_t Crossrphi(const IlcStrLine *line);
    Int_t CrossPoints(IlcStrLine *line, Double_t *point1, Double_t *point2);
    Int_t Cross(IlcStrLine *line, Double_t *point);
    Double_t GetDCA(const IlcStrLine *line) const;
    Double_t GetDistFromPoint(const Double_t *point) const;
    Bool_t GetParamAtRadius(Double_t r,Double_t &t1,Double_t &t2) const;
    void ComputePointAtT(Double_t t,Double_t *p) const {
      for(Int_t i=0;i<3;i++)p[i]=fP0[i]+fCd[i]*t; }
 protected:
    void InitDirection(const Double_t *const point, const Double_t *const cd);
    void InitTwoPoints(const Double_t *const pA, const Double_t *const pB);
    Double_t fP0[3];           // given point
    Double_t fSigma2P0[3];           // errors on coordinates of given point
    Double_t *fWMatrix;           //[6] weighting matrix
    /* fWMatrix is a symmetric matrix internally stored as
       0 --> row = 0, col = 0
       1 --> 0,1
       2 --> 0,2
       3 --> 1,1
       4 --> 1,2
       5 --> 2,2
       The external interface (constructor, getter and setter) is:
       0 --> row = 0, col = 0
       1 --> 0,1
       2 --> 0,2
       3 --> 1,0
       4 --> 1,1
       5 --> 1,2
       6 --> 2,0
       7 --> 2,1
       8 --> 2,2                                                 */
    Double_t fCd[3];         // direction cosines
    Double_t fTpar;          //! parameter 
    UShort_t fIdPoint[2];    // Identifiers of RecPoints defining the line
    
 private:
    void SetPar(const Double_t par){fTpar = par;}

  ClassDef(IlcStrLine,5);
};

#endif
