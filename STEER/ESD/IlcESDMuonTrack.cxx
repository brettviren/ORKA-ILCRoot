/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id: IlcESDMuonTrack.cxx 55451 2012-03-30 17:10:39Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//
/// \class IlcESDMuonTrack
///  Class to describe the MUON tracks in the Event Summary Data class
///  This is where the results of reconstruction are stored for the muons
///
/// \author G.Martinez
//
///////////////////////////////////////////////////////////////////////////////

#include "IlcESDMuonTrack.h"
#include "IlcESDMuonCluster.h"
#include "IlcESDEvent.h"

#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TMath.h>

ClassImp(IlcESDMuonTrack)

//_____________________________________________________________________________
IlcESDMuonTrack::IlcESDMuonTrack ():
  IlcVParticle(),
  fInverseBendingMomentum(FLT_MAX),
  fThetaX(0),
  fThetaY(0),
  fZ(0),
  fBendingCoor(0),
  fNonBendingCoor(0),
  fInverseBendingMomentumAtDCA(FLT_MAX),
  fThetaXAtDCA(0),
  fThetaYAtDCA(0),
  fBendingCoorAtDCA(0),
  fNonBendingCoorAtDCA(0),
  fInverseBendingMomentumUncorrected(FLT_MAX),
  fThetaXUncorrected(0),
  fThetaYUncorrected(0),
  fZUncorrected(0),
  fBendingCoorUncorrected(0),
  fNonBendingCoorUncorrected(0),
  fRAtAbsorberEnd(0),
  fChi2(0),
  fChi2MatchTrigger(0),
  fLocalTrigger(0),
  fX1Pattern(0),
  fY1Pattern(0),
  fX2Pattern(0),
  fY2Pattern(0),
  fX3Pattern(0),
  fY3Pattern(0),
  fX4Pattern(0),
  fY4Pattern(0),
  fMuonClusterMap(0),
  fHitsPatternInTrigCh(0),
  fHitsPatternInTrigChTrk(0),
  fNHit(0),
  fClusters(0x0),
  fClustersId(0x0),
  fLabel(-1),
  fESDEvent(0)

{
  //
  /// Default constructor
  //
  for (Int_t i = 0; i < 15; i++) fCovariances[i] = 0;
}


//_____________________________________________________________________________
IlcESDMuonTrack::IlcESDMuonTrack (const IlcESDMuonTrack& muonTrack):
  IlcVParticle(muonTrack),
  fInverseBendingMomentum(muonTrack.fInverseBendingMomentum),
  fThetaX(muonTrack.fThetaX),
  fThetaY(muonTrack.fThetaY),
  fZ(muonTrack.fZ),
  fBendingCoor(muonTrack.fBendingCoor),
  fNonBendingCoor(muonTrack.fNonBendingCoor),
  fInverseBendingMomentumAtDCA(muonTrack.fInverseBendingMomentumAtDCA),
  fThetaXAtDCA(muonTrack.fThetaXAtDCA),
  fThetaYAtDCA(muonTrack.fThetaYAtDCA),
  fBendingCoorAtDCA(muonTrack.fBendingCoorAtDCA),
  fNonBendingCoorAtDCA(muonTrack.fNonBendingCoorAtDCA),
  fInverseBendingMomentumUncorrected(muonTrack.fInverseBendingMomentumUncorrected),
  fThetaXUncorrected(muonTrack.fThetaXUncorrected),
  fThetaYUncorrected(muonTrack.fThetaYUncorrected),
  fZUncorrected(muonTrack.fZUncorrected),
  fBendingCoorUncorrected(muonTrack.fBendingCoorUncorrected),
  fNonBendingCoorUncorrected(muonTrack.fNonBendingCoorUncorrected),
  fRAtAbsorberEnd(muonTrack.fRAtAbsorberEnd),
  fChi2(muonTrack.fChi2),
  fChi2MatchTrigger(muonTrack.fChi2MatchTrigger),
  fLocalTrigger(muonTrack.fLocalTrigger),
  fX1Pattern(muonTrack.fX1Pattern),
  fY1Pattern(muonTrack.fY1Pattern),
  fX2Pattern(muonTrack.fX2Pattern),
  fY2Pattern(muonTrack.fY2Pattern),
  fX3Pattern(muonTrack.fX3Pattern),
  fY3Pattern(muonTrack.fY3Pattern),
  fX4Pattern(muonTrack.fX4Pattern),
  fY4Pattern(muonTrack.fY4Pattern),
  fMuonClusterMap(muonTrack.fMuonClusterMap),
  fHitsPatternInTrigCh(muonTrack.fHitsPatternInTrigCh),
  fHitsPatternInTrigChTrk(muonTrack.fHitsPatternInTrigChTrk),
  fNHit(muonTrack.fNHit),
  fClusters(0x0),
  fClustersId(0x0),
  fLabel(muonTrack.fLabel),
  fESDEvent(muonTrack.fESDEvent)
{
  //
  /// Copy constructor
  /// Deep copy implemented
  //
  for (Int_t i = 0; i < 15; i++) fCovariances[i] = muonTrack.fCovariances[i];
  
  // necessary to make a copy of the objects and not only the pointers in TClonesArray
  if (muonTrack.fClusters) {
    fClusters = new TClonesArray("IlcESDMuonCluster",muonTrack.fClusters->GetEntriesFast());
    IlcESDMuonCluster *cluster = (IlcESDMuonCluster*) muonTrack.fClusters->First();
    while (cluster) {
      new ((*fClusters)[fClusters->GetEntriesFast()]) IlcESDMuonCluster(*cluster);
      cluster = (IlcESDMuonCluster*) muonTrack.fClusters->After(cluster);
    }
  }
  
  // copy of cluster Ids
  if (muonTrack.fClustersId) fClustersId = new TArrayI(*(muonTrack.fClustersId));
  
}

//_____________________________________________________________________________
IlcESDMuonTrack& IlcESDMuonTrack::operator=(const IlcESDMuonTrack& muonTrack)
{
  // 
  /// Equal operator for a deep copy
  //
  if (this == &muonTrack)
    return *this;

  IlcVParticle::operator=(muonTrack); // don't forget to invoke the base class' assignment operator
  
  fInverseBendingMomentum = muonTrack.fInverseBendingMomentum; 
  fThetaX                 = muonTrack.fThetaX;           
  fThetaY                 = muonTrack.fThetaY;           
  fZ                      = muonTrack.fZ;                
  fBendingCoor            = muonTrack.fBendingCoor;      
  fNonBendingCoor         = muonTrack.fNonBendingCoor;   
  
  fInverseBendingMomentumAtDCA = muonTrack.fInverseBendingMomentumAtDCA; 
  fThetaXAtDCA                 = muonTrack.fThetaXAtDCA;           
  fThetaYAtDCA                 = muonTrack.fThetaYAtDCA;           
  fBendingCoorAtDCA            = muonTrack.fBendingCoorAtDCA;      
  fNonBendingCoorAtDCA         = muonTrack.fNonBendingCoorAtDCA;   
  
  fInverseBendingMomentumUncorrected = muonTrack.fInverseBendingMomentumUncorrected; 
  fThetaXUncorrected                 = muonTrack.fThetaXUncorrected;           
  fThetaYUncorrected                 = muonTrack.fThetaYUncorrected;           
  fZUncorrected                      = muonTrack.fZUncorrected;                
  fBendingCoorUncorrected            = muonTrack.fBendingCoorUncorrected;      
  fNonBendingCoorUncorrected         = muonTrack.fNonBendingCoorUncorrected;   
  
  for (Int_t i = 0; i < 15; i++) fCovariances[i] = muonTrack.fCovariances[i];
  
  fRAtAbsorberEnd            = muonTrack.fRAtAbsorberEnd;
  
  fChi2                   = muonTrack.fChi2;             
  fNHit                   = muonTrack.fNHit; 

  fLocalTrigger           = muonTrack.fLocalTrigger;  
  fX1Pattern              = muonTrack.fX1Pattern;  
  fY1Pattern              = muonTrack.fY1Pattern;  
  fX2Pattern              = muonTrack.fX2Pattern;  
  fY2Pattern              = muonTrack.fY2Pattern;  
  fX3Pattern              = muonTrack.fX3Pattern;  
  fY3Pattern              = muonTrack.fY3Pattern;  
  fX4Pattern              = muonTrack.fX4Pattern;  
  fY4Pattern              = muonTrack.fY4Pattern;  
  fChi2MatchTrigger       = muonTrack.fChi2MatchTrigger; 

  fHitsPatternInTrigCh    = muonTrack.fHitsPatternInTrigCh;
  fHitsPatternInTrigChTrk    = muonTrack.fHitsPatternInTrigChTrk;
 
  fMuonClusterMap	  = muonTrack.fMuonClusterMap;

  fLabel                  = muonTrack.fLabel;
  
  fESDEvent               = muonTrack.fESDEvent;

  // necessary to make a copy of the objects and not only the pointers in TClonesArray
  delete fClusters;
  if (muonTrack.fClusters) {
    fClusters = new TClonesArray("IlcESDMuonCluster",muonTrack.fClusters->GetEntriesFast());
    IlcESDMuonCluster *cluster = (IlcESDMuonCluster*) muonTrack.fClusters->First();
    while (cluster) {
      new ((*fClusters)[fClusters->GetEntriesFast()]) IlcESDMuonCluster(*cluster);
      cluster = (IlcESDMuonCluster*) muonTrack.fClusters->After(cluster);
    }
  } else fClusters = 0x0;
  
  // copy of cluster Ids
  if (muonTrack.fClustersId) {
    if (fClustersId) *fClustersId = *(muonTrack.fClustersId);
    else fClustersId = new TArrayI(*(muonTrack.fClustersId));
  } else {
    delete fClustersId;
    fClustersId = 0x0;
  }
  
  return *this;
}

//__________________________________________________________________________
void IlcESDMuonTrack::Copy(TObject &obj) const {
  
  /// This overwrites the virtual TOBject::Copy()
  /// to allow run time copying without casting
  /// in IlcESDEvent

  if(this==&obj)return;
  IlcESDMuonTrack *robj = dynamic_cast<IlcESDMuonTrack*>(&obj);
  if(!robj)return; // not an IlcESDMuonTrack
  *robj = *this;

}

//__________________________________________________________________________
IlcESDMuonTrack::~IlcESDMuonTrack()
{
  /// Destructor
  delete fClusters;
  delete fClustersId;
}

//__________________________________________________________________________
void IlcESDMuonTrack::Clear(Option_t* opt)
{
  /// Clear arrays
  if (opt && opt[0] == 'C') {
    if (fClusters) fClusters->Clear("C");
  } else {
    delete fClusters; fClusters = 0x0;
  }
  delete fClustersId; fClustersId = 0x0;
  fNHit = 0;
}

//__________________________________________________________________________
void IlcESDMuonTrack::Reset()
{
  /// Reset to default values
  SetUniqueID(0);
  fInverseBendingMomentum = FLT_MAX;
  fThetaX = 0.;
  fThetaY = 0.;
  fZ = 0.;
  fBendingCoor = 0.;
  fNonBendingCoor = 0.;
  fInverseBendingMomentumAtDCA = FLT_MAX;
  fThetaXAtDCA = 0.;
  fThetaYAtDCA = 0.;
  fBendingCoorAtDCA = 0.;
  fNonBendingCoorAtDCA = 0.;
  fInverseBendingMomentumUncorrected = FLT_MAX;
  fThetaXUncorrected = 0.;
  fThetaYUncorrected = 0.;
  fZUncorrected = 0.;
  fBendingCoorUncorrected = 0.;
  fNonBendingCoorUncorrected = 0.;
  fRAtAbsorberEnd = 0.;
  fChi2 = 0.;
  fChi2MatchTrigger = 0.;
  fLocalTrigger = 0;
  fX1Pattern = 0;
  fY1Pattern = 0;
  fX2Pattern = 0;
  fY2Pattern = 0;
  fX3Pattern = 0;
  fY3Pattern = 0;
  fX4Pattern = 0;
  fY4Pattern = 0;
  fMuonClusterMap = 0;
  fHitsPatternInTrigCh = 0;
  fHitsPatternInTrigChTrk = 0;
  fNHit = 0;
  delete fClusters; fClusters = 0x0;
  delete fClustersId; fClustersId = 0x0;
  for (Int_t i = 0; i < 15; i++) fCovariances[i] = 0.;
  fLabel = -1;
  fESDEvent = 0;
}

//_____________________________________________________________________________
void IlcESDMuonTrack::GetCovariances(TMatrixD& cov) const
{
  /// return covariance matrix of uncorrected parameters
  cov.ResizeTo(5,5);
  for (Int_t i = 0; i < 5; i++)
    for (Int_t j = 0; j <= i; j++)
      cov(i,j) = cov (j,i) = fCovariances[i*(i+1)/2 + j];
}

//_____________________________________________________________________________
void IlcESDMuonTrack::SetCovariances(const TMatrixD& cov)
{
  /// set reduced covariance matrix of uncorrected parameters
  for (Int_t i = 0; i < 5; i++)
    for (Int_t j = 0; j <= i; j++)
      fCovariances[i*(i+1)/2 + j] = cov(i,j);

}

//_____________________________________________________________________________
void IlcESDMuonTrack::GetCovarianceXYZPxPyPz(Double_t cov[21]) const
{
  /// return reduced covariance matrix of uncorrected parameters in (X,Y,Z,Px,Py,Pz) coordinate system
  /// 
  /// - Cov(x,x) ... :   cov[0]
  /// - Cov(y,x) ... :   cov[1]  cov[2]
  /// - Cov(z,x) ... :   cov[3]  cov[4]  cov[5]
  /// - Cov(px,x)... :   cov[6]  cov[7]  cov[8]  cov[9]
  /// - Cov(py,x)... :   cov[10] cov[11] cov[12] cov[13] cov[14]
  /// - Cov(pz,x)... :   cov[15] cov[16] cov[17] cov[18] cov[19] cov[20]
  ///
  /// Get ESD covariance matrix into a TMatrixD
  TMatrixD covESD(5,5);
  GetCovariances(covESD);

  // compute Jacobian to change the coordinate system
  // from (X,thetaX,Y,thetaY,c/pYZ) to (X,Y,Z,pX,pY,pZ)
  Double_t tanThetaX = TMath::Tan(fThetaXUncorrected);
  Double_t tanThetaY = TMath::Tan(fThetaYUncorrected);
  Double_t cosThetaX2 = TMath::Cos(fThetaXUncorrected) * TMath::Cos(fThetaXUncorrected);
  Double_t cosThetaY2 = TMath::Cos(fThetaYUncorrected) * TMath::Cos(fThetaYUncorrected);
  Double_t pZ = PzUncorrected();
  Double_t dpZdthetaY = - fInverseBendingMomentumUncorrected * fInverseBendingMomentumUncorrected *
			  pZ * pZ * pZ * tanThetaY / cosThetaY2;
  Double_t dpZdinvpYZ = (fInverseBendingMomentumUncorrected != 0.) ? - pZ / fInverseBendingMomentumUncorrected : - FLT_MAX;
  TMatrixD jacob(6,5);
  jacob.Zero();
  jacob(0,0) = 1.;
  jacob(1,2) = 1.;
  jacob(3,1) = pZ / cosThetaX2;
  jacob(3,3) = dpZdthetaY * tanThetaX;
  jacob(3,4) = dpZdinvpYZ * tanThetaX;
  jacob(4,3) = dpZdthetaY * tanThetaY + pZ / cosThetaY2;
  jacob(4,4) = dpZdinvpYZ * tanThetaY;
  jacob(5,3) = dpZdthetaY;
  jacob(5,4) = dpZdinvpYZ;
  
  // compute covariance matrix in AOD coordinate system
  TMatrixD tmp(covESD,TMatrixD::kMultTranspose,jacob);
  TMatrixD covAOD(jacob,TMatrixD::kMult,tmp);
  
  // Get AOD covariance matrix into co[21]
  for (Int_t i = 0; i < 6; i++)
    for (Int_t j = 0; j <= i; j++)
      cov[i*(i+1)/2 + j] = covAOD(i,j);
  
}

//_____________________________________________________________________________
Double_t IlcESDMuonTrack::Px() const
{
  /// return p_x from track parameters
  Double_t nonBendingSlope = TMath::Tan(fThetaX);
  Double_t bendingSlope    = TMath::Tan(fThetaY);
  Double_t pYZ = (fInverseBendingMomentum != 0.) ? TMath::Abs(1. / fInverseBendingMomentum) : - FLT_MAX;
  Double_t pZ  = -pYZ / TMath::Sqrt(1.0 + bendingSlope*bendingSlope);  // spectro. (z<0)
  return pZ * nonBendingSlope;
}

//_____________________________________________________________________________
Double_t IlcESDMuonTrack::Py() const
{
  /// return p_y from track parameters
  Double_t bendingSlope = TMath::Tan(fThetaY);
  Double_t pYZ = (fInverseBendingMomentum != 0.) ? TMath::Abs(1. / fInverseBendingMomentum) : - FLT_MAX;
  Double_t pZ  = -pYZ / TMath::Sqrt(1.0 + bendingSlope*bendingSlope);  // spectro. (z<0)
  return pZ * bendingSlope;
}

//_____________________________________________________________________________
Double_t IlcESDMuonTrack::Pz() const
{
  /// return p_z from track parameters
  Double_t bendingSlope = TMath::Tan(fThetaY);
  Double_t pYZ = (fInverseBendingMomentum != 0.) ? TMath::Abs(1. / fInverseBendingMomentum) : - FLT_MAX;
  return -pYZ / TMath::Sqrt(1.0 + bendingSlope*bendingSlope);  // spectro. (z<0)
}

//_____________________________________________________________________________
Double_t IlcESDMuonTrack::P() const
{
  /// return p from track parameters
  Double_t nonBendingSlope = TMath::Tan(fThetaX);
  Double_t bendingSlope    = TMath::Tan(fThetaY);
  Double_t pYZ = (fInverseBendingMomentum != 0.) ? TMath::Abs(1. / fInverseBendingMomentum) : - FLT_MAX;
  Double_t pZ  = -pYZ / TMath::Sqrt(1.0 + bendingSlope*bendingSlope);  // spectro. (z<0)
  return -pZ * TMath::Sqrt(1.0 + bendingSlope*bendingSlope + nonBendingSlope*nonBendingSlope);
}

//_____________________________________________________________________________
void IlcESDMuonTrack::LorentzP(TLorentzVector& vP) const
{
  /// return Lorentz momentum vector from track parameters
  Double_t muonMass = M();
  Double_t nonBendingSlope = TMath::Tan(fThetaX);
  Double_t bendingSlope    = TMath::Tan(fThetaY);
  Double_t pYZ = (fInverseBendingMomentum != 0.) ? TMath::Abs(1. / fInverseBendingMomentum) : - FLT_MAX;
  Double_t pZ  = -pYZ / TMath::Sqrt(1.0 + bendingSlope*bendingSlope);  // spectro. (z<0)
  Double_t pX  = pZ * nonBendingSlope;
  Double_t pY  = pZ * bendingSlope;
  Double_t e   = TMath::Sqrt(muonMass*muonMass + pX*pX + pY*pY + pZ*pZ);
  vP.SetPxPyPzE(pX, pY, pZ, e);
}

//_____________________________________________________________________________
Double_t IlcESDMuonTrack::PxAtDCA() const
{
  /// return p_x from track parameters
  Double_t nonBendingSlope = TMath::Tan(fThetaXAtDCA);
  Double_t bendingSlope    = TMath::Tan(fThetaYAtDCA);
  Double_t pYZ = (fInverseBendingMomentumAtDCA != 0.) ? TMath::Abs(1. / fInverseBendingMomentumAtDCA) : - FLT_MAX;
  Double_t pZ  = -pYZ / TMath::Sqrt(1.0 + bendingSlope*bendingSlope);  // spectro. (z<0)
  return pZ * nonBendingSlope;
}

//_____________________________________________________________________________
Double_t IlcESDMuonTrack::PyAtDCA() const
{
  /// return p_y from track parameters
  Double_t bendingSlope = TMath::Tan(fThetaYAtDCA);
  Double_t pYZ = (fInverseBendingMomentumAtDCA != 0.) ? TMath::Abs(1. / fInverseBendingMomentumAtDCA) : - FLT_MAX;
  Double_t pZ  = -pYZ / TMath::Sqrt(1.0 + bendingSlope*bendingSlope);  // spectro. (z<0)
  return pZ * bendingSlope;
}

//_____________________________________________________________________________
Double_t IlcESDMuonTrack::PzAtDCA() const
{
  /// return p_z from track parameters
  Double_t bendingSlope = TMath::Tan(fThetaYAtDCA);
  Double_t pYZ = (fInverseBendingMomentumAtDCA != 0.) ? TMath::Abs(1. / fInverseBendingMomentumAtDCA) : - FLT_MAX;
  return -pYZ / TMath::Sqrt(1.0 + bendingSlope*bendingSlope);  // spectro. (z<0)
}

//_____________________________________________________________________________
Double_t IlcESDMuonTrack::PAtDCA() const
{
  /// return p from track parameters
  Double_t nonBendingSlope = TMath::Tan(fThetaXAtDCA);
  Double_t bendingSlope    = TMath::Tan(fThetaYAtDCA);
  Double_t pYZ = (fInverseBendingMomentumAtDCA != 0.) ? TMath::Abs(1. / fInverseBendingMomentumAtDCA) : - FLT_MAX;
  Double_t pZ  = -pYZ / TMath::Sqrt(1.0 + bendingSlope*bendingSlope);  // spectro. (z<0)
  return -pZ * TMath::Sqrt(1.0 + bendingSlope*bendingSlope + nonBendingSlope*nonBendingSlope);
}

//_____________________________________________________________________________
void IlcESDMuonTrack::LorentzPAtDCA(TLorentzVector& vP) const
{
  /// return Lorentz momentum vector from track parameters
  Double_t muonMass = M();
  Double_t nonBendingSlope = TMath::Tan(fThetaXAtDCA);
  Double_t bendingSlope    = TMath::Tan(fThetaYAtDCA);
  Double_t pYZ = (fInverseBendingMomentumAtDCA != 0.) ? TMath::Abs(1. / fInverseBendingMomentumAtDCA) : - FLT_MAX;
  Double_t pZ  = -pYZ / TMath::Sqrt(1.0 + bendingSlope*bendingSlope);  // spectro. (z<0)
  Double_t pX  = pZ * nonBendingSlope;
  Double_t pY  = pZ * bendingSlope;
  Double_t e   = TMath::Sqrt(muonMass*muonMass + pX*pX + pY*pY + pZ*pZ);
  vP.SetPxPyPzE(pX, pY, pZ, e);
}

//_____________________________________________________________________________
Double_t IlcESDMuonTrack::PxUncorrected() const
{
  /// return p_x from track parameters
  Double_t nonBendingSlope = TMath::Tan(fThetaXUncorrected);
  Double_t bendingSlope    = TMath::Tan(fThetaYUncorrected);
  Double_t pYZ = (fInverseBendingMomentumUncorrected != 0.) ? TMath::Abs(1. / fInverseBendingMomentumUncorrected) : - FLT_MAX;
  Double_t pZ  = -pYZ / TMath::Sqrt(1.0 + bendingSlope*bendingSlope);  // spectro. (z<0)
  return pZ * nonBendingSlope;
}

//_____________________________________________________________________________
Double_t IlcESDMuonTrack::PyUncorrected() const
{
  /// return p_y from track parameters
  Double_t bendingSlope = TMath::Tan(fThetaYUncorrected);
  Double_t pYZ = (fInverseBendingMomentumUncorrected != 0.) ? TMath::Abs(1. / fInverseBendingMomentumUncorrected) : - FLT_MAX;
  Double_t pZ  = -pYZ / TMath::Sqrt(1.0 + bendingSlope*bendingSlope);  // spectro. (z<0)
  return pZ * bendingSlope;
}

//_____________________________________________________________________________
Double_t IlcESDMuonTrack::PzUncorrected() const
{
  /// return p_z from track parameters
  Double_t bendingSlope = TMath::Tan(fThetaYUncorrected);
  Double_t pYZ = (fInverseBendingMomentumUncorrected != 0.) ? TMath::Abs(1. / fInverseBendingMomentumUncorrected) : - FLT_MAX;
  return -pYZ / TMath::Sqrt(1.0 + bendingSlope*bendingSlope);  // spectro. (z<0)
}

//_____________________________________________________________________________
Double_t IlcESDMuonTrack::PUncorrected() const
{
  /// return p from track parameters
  Double_t nonBendingSlope = TMath::Tan(fThetaXUncorrected);
  Double_t bendingSlope    = TMath::Tan(fThetaYUncorrected);
  Double_t pYZ = (fInverseBendingMomentumUncorrected != 0.) ? TMath::Abs(1. / fInverseBendingMomentumUncorrected) : - FLT_MAX;
  Double_t pZ  = -pYZ / TMath::Sqrt(1.0 + bendingSlope*bendingSlope);  // spectro. (z<0)
  return -pZ * TMath::Sqrt(1.0 + bendingSlope*bendingSlope + nonBendingSlope*nonBendingSlope);
}

//_____________________________________________________________________________
void IlcESDMuonTrack::LorentzPUncorrected(TLorentzVector& vP) const
{
  /// return Lorentz momentum vector from track parameters
  Double_t muonMass = M();
  Double_t nonBendingSlope = TMath::Tan(fThetaXUncorrected);
  Double_t bendingSlope    = TMath::Tan(fThetaYUncorrected);
  Double_t pYZ = (fInverseBendingMomentumUncorrected != 0.) ? TMath::Abs(1. / fInverseBendingMomentumUncorrected) : - FLT_MAX;
  Double_t pZ  = -pYZ / TMath::Sqrt(1.0 + bendingSlope*bendingSlope);  // spectro. (z<0)
  Double_t pX  = pZ * nonBendingSlope;
  Double_t pY  = pZ * bendingSlope;
  Double_t e   = TMath::Sqrt(muonMass*muonMass + pX*pX + pY*pY + pZ*pZ);
  vP.SetPxPyPzE(pX, pY, pZ, e);
}

//_____________________________________________________________________________
Int_t IlcESDMuonTrack::GetNDF() const
{
  /// return the number of degrees of freedom
  
  Int_t ndf = 2 * static_cast<Int_t>(fNHit) - 5;
  return (ndf > 0) ? ndf : 0;
}

//_____________________________________________________________________________
Double_t IlcESDMuonTrack::GetNormalizedChi2() const
{
  /// return the chi2 value divided by the number of degrees of freedom
  
  Int_t ndf = GetNDF();
  return (ndf > 0) ? fChi2 / static_cast<Double_t>(ndf) : 0.;
}

//_____________________________________________________________________________
Int_t IlcESDMuonTrack::GetMatchTrigger() const
{
  ///  backward compatibility after replacing fMatchTrigger by fLocalTrigger
  ///  - 0 track does not match trigger
  ///  - 1 track match but does not pass pt cut
  ///  - 2 track match Low pt cut
  ///  - 3 track match High pt cut

  if (!LoCircuit()) {
    return 0;
  } else if (LoLpt() == 0 && LoHpt() == 0) {
    return 1;
  } else if (LoLpt() >  0 && LoHpt() == 0) {
    return 2;
  } else {
    return 3;
  }

}

//_____________________________________________________________________________
Bool_t IlcESDMuonTrack::MatchTriggerDigits(Bool_t fromTrack) const
{
  // return kTRUE if the track matches a digit on both planes of at least 2 trigger chambers
  
  UShort_t pattern = ( fromTrack ) ? fHitsPatternInTrigChTrk : fHitsPatternInTrigCh;
  Int_t nMatchedChambers = 0;
  for (Int_t ich=0; ich<4; ich++)
    if (IsChamberHit(pattern, 0, ich) &&
	IsChamberHit(pattern, 1, ich)) nMatchedChambers++;
  
  return (nMatchedChambers >= 2);
}

//_____________________________________________________________________________
void IlcESDMuonTrack::AddClusterId(UInt_t clusterId)
{
  /// add the given cluster Id to the list associated to the track
  if (!fClustersId) fClustersId = new TArrayI(5);
  if (fClustersId->GetSize() <= fNHit) fClustersId->Set(fNHit+1);
  fClustersId->AddAt(static_cast<Int_t>(clusterId), fNHit++);
}

//_____________________________________________________________________________
void IlcESDMuonTrack::MoveClustersToESD(IlcESDEvent &esd)
{
  /// move the clusters (and attached pads) to the new ESD structure
  if (!fClusters) return;
  fNHit = 0;
  for (Int_t i = 0; i < fClusters->GetEntriesFast(); i++) {
    IlcESDMuonCluster *cluster = static_cast<IlcESDMuonCluster*>(fClusters->UncheckedAt(i));
    cluster->MovePadsToESD(esd);
    IlcESDMuonCluster *newCluster = esd.NewMuonCluster();
    *newCluster = *cluster;
    AddClusterId(newCluster->GetUniqueID());
  }
  delete fClusters;
  fClusters = 0x0;
}

//_____________________________________________________________________________
void IlcESDMuonTrack::SetFiredChamber(UInt_t& pattern, Int_t cathode, Int_t chamber)
{
  /// Turn on the bit corresponding to fired chameber
  pattern |= (0x1 << ( 7 - ( 4*cathode + chamber )));
}

//_____________________________________________________________________________
void IlcESDMuonTrack::AddEffInfo(UInt_t& pattern, Int_t slatOrInfo, Int_t board, EIlcTriggerChPatternFlag effType)
{
  /// Add efficiency flag and crossed RPC or info on rejected track
  pattern |= effType << 8;
  pattern |= slatOrInfo << 10;
  pattern |= board << 15;
}

//_____________________________________________________________________________
Bool_t IlcESDMuonTrack::IsChamberHit(UInt_t pattern, Int_t cathode, Int_t chamber)
{ 
  /// Check if chamber was was hit
  return (pattern >> (7 - ( 4*cathode + chamber ))) & 0x1;
}

//_____________________________________________________________________________
Int_t IlcESDMuonTrack::GetEffFlag(UInt_t pattern)
{
  /// Get Efficiency flag
  return (pattern >> 8) & 0x03;
}

//_____________________________________________________________________________
Int_t IlcESDMuonTrack::GetSlatOrInfo(UInt_t pattern) 
{
  /// Getting crossed slat or info
  return (pattern >> 10) & 0x1F;
}

//_____________________________________________________________________________
Int_t IlcESDMuonTrack::GetCrossedBoard(UInt_t pattern) 
{
  /// Getting crossed board
  return ( pattern >> 15 ) & 0xFF;
}
