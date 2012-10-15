#ifndef ILCESDVERTEX_H
#define ILCESDVERTEX_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcESDVertex.h 51503 2011-09-08 22:36:06Z shahoian $ */

//-------------------------------------------------------
//                    Primary Vertex Class
//          for the Event Data Summary Class
//   Origin: A.Dainese, Padova, andrea.dainese@pd.infn.it
//-------------------------------------------------------

/*****************************************************************************
 *                                                                           *
 * This class deals with primary vertex.                                     *
 * IlcESDVertex objects are created by the class IlcITSVertexer and its      *
 * derived classes.                                                          *
 * Different constructors are provided:                                      *
 * - for primary vertex determined with pixels in pp (only Z)                *
 * - for primary vertex determined with pixels in ion-ion (X,Y,Z)            *
 * - for primary vertex determined with ITS tracks in pp (X,Y,Z)             *
 * This class replaces the previous version of IlcESDVertex, designed        *
 * originally only for A-A collisions. The functionalities of the old class  *
 * are maintained in the IlcITSVertexerIons class                            *
 *                                                                           *
 *****************************************************************************/

#include <TMath.h>

#include "IlcVertex.h"
class IlcVTrack;

class IlcESDVertex : public IlcVertex {
 
 public:
 
  IlcESDVertex();
  IlcESDVertex(Double_t positionZ,Double_t sigmaZ,Int_t nContributors,
	       const Char_t *vtxName="Vertex");
  IlcESDVertex(Double_t position[3],Double_t covmatrix[6],
	       Double_t chi2,Int_t nContributors,
	       const Char_t *vtxName="Vertex");
  IlcESDVertex(Double_t position[3],Double_t sigma[3],
	       const Char_t *vtxName="Vertex");
  IlcESDVertex(Double_t position[3],Double_t sigma[3],Double_t snr[3],
	       const Char_t *vtxName="Vertex");
  IlcESDVertex(const IlcESDVertex &source);
  IlcESDVertex &operator=(const IlcESDVertex &source);
  virtual void Copy(TObject &obj) const;

  virtual ~IlcESDVertex() {}

  void     GetSigmaXYZ(Double_t sigma[3]) const;
  void     GetCovMatrix(Double_t covmatrix[6]) const;
  void     GetCovarianceMatrix(Double_t covmatrix[6]) const 
                    {GetCovMatrix(covmatrix);}
  void     GetSNR(Double_t snr[3]) const;
  void     SetCovarianceMatrix(const Double_t *cov);

  Double_t GetXRes() const {return TMath::Sqrt(fCovXX);}
  Double_t GetYRes() const {return TMath::Sqrt(fCovYY);}
  Double_t GetZRes() const {return TMath::Sqrt(fCovZZ);}
  Double_t GetXSNR() const { return fSNR[0]; }
  Double_t GetYSNR() const { return fSNR[1]; }
  Double_t GetZSNR() const { return fSNR[2]; }
  void     SetSNR(double snr, int i) {if (i<3 && i>=0) fSNR[i] = snr;}

  Double_t GetChi2() const { return fChi2; }
  void     SetChi2(Double_t chi) { fChi2 = chi; }
  Double_t GetChi2toNDF() const 
    { return fChi2/(2.*(Double_t)fNContributors-3.); }
  Double_t GetChi2perNDF() const { return GetChi2toNDF();}
  Int_t    GetNDF() const {return (2*fNContributors-3);}

  void     Print(Option_t* option = "") const;
  void     PrintStatus() const {Print();}

  void     Reset() { SetToZero(); SetName("Vertex"); }

  void     SetID(Char_t id) {fID=id;}
  Char_t   GetID() const {return fID;}
  //
  void     SetBC(Int_t bc)               {fBCID = bc;}
  Int_t    GetBC()              const    {return fBCID;}
  Double_t GetWDist(const IlcESDVertex* v) const;

 protected:

  Double32_t fCovXX,fCovXY,fCovYY,fCovXZ,fCovYZ,fCovZZ;  // vertex covariance matrix
  Double32_t fSNR[3];  // S/N ratio
  Double32_t fChi2;  // chi2 of vertex fit

  Char_t fID;       // ID of this vertex within an ESD event
  Char_t fBCID;     // BC ID assigned to vertex
 private:

  void SetToZero();

  ClassDef(IlcESDVertex,8)  // Class for Primary Vertex
};

#endif






    
