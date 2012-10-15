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

//-------------------------------------------------------------------------
//     Analysis Oriented Data (AOD) V0 vertex class
//     Authors: B.Hippolyte, IReS, hippolyt@in2p3.fr 
//              G.Van Buren, BNL,  gene@bnl.gov      (original STAR MuDsts)
//-------------------------------------------------------------------------

#include "IlcAODv0.h"
#include "IlcAODTrack.h"

ClassImp(IlcAODv0)

  IlcAODv0::IlcAODv0() : 
    IlcAODRecoDecay(),
    fDcaV0ToPrimVertex(999),
    fOnFlyStatus(kFALSE)
{
}

IlcAODv0::IlcAODv0(IlcAODVertex* rAODVertex, Double_t rDcaV0Daughters, Double_t rDcaV0ToPrimVertex,
	   const Double_t *rMomPos, const Double_t *rMomNeg, Double_t *rDcaDaughterToPrimVertex) :
  IlcAODRecoDecay(rAODVertex,2,0,rDcaDaughterToPrimVertex),
  fDcaV0ToPrimVertex(rDcaV0ToPrimVertex),
  fOnFlyStatus(kFALSE)
{
  //--------------------------------------------------------------------
  // Constructor via setting each data member
  //--------------------------------------------------------------------
  fCharge  = 0;
  fNProngs = 2;
  fNDCA    = 1;
  fNPID    = 0; // used to be 2!

  fDCA = new Double_t[fNDCA];

  fDCA[0] = rDcaV0Daughters;
  fDcaV0ToPrimVertex = rDcaV0ToPrimVertex;

  fPx = new Double_t[GetNProngs()];
  fPy = new Double_t[GetNProngs()];
  fPz = new Double_t[GetNProngs()];

  fPx[0] = rMomPos[0] ;
  fPy[0] = rMomPos[1];
  fPz[0] = rMomPos[2];

  fPx[1] = rMomNeg[0];
  fPy[1] = rMomNeg[1];
  fPz[1] = rMomNeg[2];
}

IlcAODv0::IlcAODv0(const IlcAODv0& rIlcAODv0) :
  IlcAODRecoDecay(rIlcAODv0),
  fDcaV0ToPrimVertex(rIlcAODv0.fDcaV0ToPrimVertex),
  fOnFlyStatus(rIlcAODv0.fOnFlyStatus)
 {
  //--------------------------------------------------------------------
  // Copy constructor
  //--------------------------------------------------------------------
}

IlcAODv0& IlcAODv0::operator=(const IlcAODv0& rIlcAODv0){
  //--------------------------------------------------------------------
  // Assignment overload
  //--------------------------------------------------------------------
  if(this!=&rIlcAODv0) {
    IlcAODRecoDecay::operator=(rIlcAODv0);
    fDcaV0ToPrimVertex  = rIlcAODv0.fDcaV0ToPrimVertex ;
    fOnFlyStatus        = rIlcAODv0.fOnFlyStatus;
  }
  return *this;
}

IlcAODv0::~IlcAODv0(){
  //--------------------------------------------------------------------
  // Empty destructor
  //--------------------------------------------------------------------
}

void IlcAODv0::Fill(IlcAODVertex *rAODVertex, Double_t rDcaV0Daughters, Double_t rDcaV0ToPrimVertex,
		    const Double_t *rMomPos, const Double_t *rMomNeg, const Double_t *rDcaDaughterToPrimVertex){
  //--------------------------------------------------------------------
  // Filling with all needed info
  //--------------------------------------------------------------------
  this->SetSecondaryVtx(rAODVertex);

  fDCA[0] = rDcaV0Daughters;
  fDcaV0ToPrimVertex = rDcaV0ToPrimVertex;

  fPx[0] = rMomPos[0] ;
  fPy[0] = rMomPos[1];
  fPz[0] = rMomPos[2];

  fPx[1] = rMomNeg[0];
  fPy[1] = rMomNeg[1];
  fPz[1] = rMomNeg[2];

  fd0[0] = rDcaDaughterToPrimVertex[0];
  fd0[1] = rDcaDaughterToPrimVertex[1];
}

void IlcAODv0::ResetV0(){
  //--------------------------------------------------------------------
  // Resetting all the info
  //--------------------------------------------------------------------
  GetSecondaryVtx()->SetChi2perNDF(999);
  GetSecondaryVtx()->RemoveCovMatrix();
  GetSecondaryVtx()->RemoveDaughters();
  GetSecondaryVtx()->SetParent((TObject*) 0x0);
  GetSecondaryVtx()->SetID(-1);
  GetSecondaryVtx()->SetPosition(999,999,999);
  GetSecondaryVtx()->SetType(IlcAODVertex::kUndef);

  fDCA[0] = 999;
  fDcaV0ToPrimVertex  = 999;

  fPx[0] = 999;
  fPy[0] = 999;
  fPz[0] = 999;

  fPx[1] = 999;
  fPy[1] = 999;
  fPz[1] = 999;

  fd0[0] = 999;
  fd0[1] = 999;
}

Short_t IlcAODv0::GetPosID() const {
  	IlcAODTrack *posTrack = (IlcAODTrack *) (this->GetSecondaryVtx()->GetDaughter(0));
	Short_t posID = posTrack->GetID();
	return posID;
}

Short_t IlcAODv0::GetNegID() const {
  	IlcAODTrack *negTrack = (IlcAODTrack *) (this->GetSecondaryVtx()->GetDaughter(1));
	Short_t negID = negTrack->GetID();
	return negID;
}

void IlcAODv0::Print(Option_t* /*option*/) const {
  //
  // Print some information
  //
  IlcAODRecoDecay::Print();
  printf("IlcAODv0: invariant mass (k0s %.6f, lambda %.6f, anti-lambda %.6f) \n",MassK0Short(),MassLambda(),MassAntiLambda());
  printf("IlcAODv0: dca (v0d %.6f, v0tpv %.6f, postpv %.6f, negtpv %.6f ) \n",DcaV0Daughters(),DcaV0ToPrimVertex(),DcaPosToPrimVertex(),DcaNegToPrimVertex());
  printf("IlcAODv0: mom (ptot2 %.6f, pt2 %.6f, rapk0 %.6f, rapla %.6f ) \n",Ptot2V0(),Pt2V0(),RapK0Short(),RapLambda());
  printf("IlcAODv0: cin (mpav0 %.6f, mnav0 %.6f, alpha %.6f, ptarm %.6f ) \n",MomPosAlongV0(),MomNegAlongV0(),AlphaV0(),PtArmV0());
  printf("IlcAODv0: nrg (eppro %.6f, enpro %.6f, eppio %.6f, enpio %.6f ) \n",EPosProton(),ENegProton(),EPosPion(),ENegPion());

  return;
}
