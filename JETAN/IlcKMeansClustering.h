#ifndef ILCKMEANSCLUSTERING_H
#define ILCKMEANSCLUSTERING_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

// Implemenatation of the K-Means Clustering Algorithm
// http://en.wikipedia.org/wiki/K-means_clustering
// This particular implementation is the so called Soft K-means algorithm.
// It has been modified to work on the cylindrical topology in eta-phi space.
//
// Author: Andreas Morsch (CERN)
// andreas.morsch@cern.ch

#include <TObject.h>
 
class IlcKMeansClustering : public TObject
{
 public:
  IlcKMeansClustering()          {}
  virtual ~IlcKMeansClustering() {}
  
  static Int_t SoftKMeans (Int_t k, Int_t n, const Double_t* x, const Double_t* y, Double_t* mx, Double_t* my , Double_t* rk );
  static Int_t SoftKMeans2(Int_t k, Int_t n, Double_t* x, Double_t* y, Double_t* mx, Double_t* my , Double_t* sigma2, 
			  Double_t* rk );
  static Int_t SoftKMeans3(Int_t k, Int_t n, Double_t* x, Double_t* y, Double_t* mx, Double_t* my , 
			   Double_t* sigmax2, Double_t* sigmay2, Double_t* rk );
  static void  OptimalInit(Int_t k, Int_t n, const Double_t* x, const Double_t* y, Double_t* mx, Double_t* my);
  static void  SetBeta(Double_t beta) {fBeta = beta;}
  static Double_t d(Double_t mx, Double_t my, Double_t x, Double_t y);
protected:
  static Double_t fBeta; // beta parameter
  
  ClassDef(IlcKMeansClustering, 1)
};

class IlcKMeansResult : public TObject
{
 public:
  IlcKMeansResult(Int_t k);
  IlcKMeansResult(const IlcKMeansResult &res);
  IlcKMeansResult& operator=(const IlcKMeansResult& trclass);

  virtual ~IlcKMeansResult();
  Int_t      GetK()      const  {return fK;}
  Double_t*  GetMx()     const  {return fMx;}
  Double_t*  GetMy()     const  {return fMy;}
  Double_t*  GetSigma2() const  {return fSigma2;}
  Double_t*  GetRk()     const  {return fRk;}
  Int_t*     GetInd()    const  {return fInd;}
  Double_t*  GetTarget() const  {return fTarget;}
  void       CopyResults(const IlcKMeansResult* res);
  void       Sort();
  void       Sort(Int_t n, const Double_t* x, const Double_t* y);  
protected:
  Int_t        fK;        //! Number of clusters
  Double_t*    fMx;       //! Position x
  Double_t*    fMy;       //! Position y
  Double_t*    fSigma2;   //! Sigma2
  Double_t*    fRk;       //! Responsibility
  Double_t*    fTarget;   //! Target for sorting
  Int_t*       fInd;      //! Index for sorting
 
  ClassDef(IlcKMeansResult, 1)
};

#endif
