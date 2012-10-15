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
//     Implementation of the Analysis Oriented Data (AOD) Xi vertex class
//     Origin: A.Maire, IReS, antonin.maire@ires.in2p3.fr 
//             G.Van Buren, BNL,  gene@bnl.gov      (original STAR MuDsts)
//
//     Purpose: Having physics observables available for Xis
//-------------------------------------------------------------------------

#include <TVector3.h>
#include <TMath.h>
#include <TDatabasePDG.h>

#include "IlcAODcascade.h"
#include "IlcAODTrack.h"

ClassImp(IlcAODcascade)


IlcAODcascade::IlcAODcascade() : 
  IlcAODv0(),
     
  fDecayVertexXi(0x0),
  fChargeXi(0),

  fDcaXiDaughters(999),
  fDcaXiToPrimVertex(999),
  fDcaBachToPrimVertex(999),
  
  fMomBachX(999),
  fMomBachY(999),
  fMomBachZ(999)
   
{
  //--------------------------------------------------------------------
  // Default constructor
  //--------------------------------------------------------------------
 
}



IlcAODcascade::IlcAODcascade(const IlcAODcascade& rSource) : 
   IlcAODv0( rSource ),
  
  fDecayVertexXi( rSource.fDecayVertexXi ),
  fChargeXi(      rSource.fChargeXi ),
  
  fDcaXiDaughters(      rSource.fDcaXiDaughters ),
  fDcaXiToPrimVertex(   rSource.fDcaXiToPrimVertex ),
  fDcaBachToPrimVertex( rSource.fDcaBachToPrimVertex ),
  
  fMomBachX( rSource.fMomBachX ),
  fMomBachY( rSource.fMomBachY ),
  fMomBachZ( rSource.fMomBachZ )
  
{
  //--------------------------------------------------------------------
  // Copy constructor
  //--------------------------------------------------------------------
  
}



IlcAODcascade::IlcAODcascade( IlcAODVertex* rAODVertexXi, 
                      Int_t         rChargeXi,
		      Double_t      rDcaXiDaughters,
		      Double_t      rDcaXiToPrimVertex,
		      Double_t      rDcaBachToPrimVertex,
		const Double_t*     rMomBach,
		
		              IlcAODVertex* rAODVertexV0,
		      Double_t rDcaV0Daughters,
		      Double_t rDcaV0ToPrimVertex,
		const Double_t *rMomPos,
		const Double_t *rMomNeg,
		      Double_t *rDcaDaughterToPrimVertex
		) :
  IlcAODv0(rAODVertexV0, rDcaV0Daughters, rDcaV0ToPrimVertex, rMomPos, rMomNeg, rDcaDaughterToPrimVertex),
  fDecayVertexXi( rAODVertexXi ),
  fChargeXi( rChargeXi ),    
  fDcaXiDaughters( rDcaXiDaughters ),     
  fDcaXiToPrimVertex( rDcaXiToPrimVertex ),
  fDcaBachToPrimVertex( rDcaBachToPrimVertex ),
  fMomBachX( rMomBach[0] ),          
  fMomBachY( rMomBach[1] ),      
  fMomBachZ( rMomBach[2] )  
{
  //--------------------------------------------------------------------
  // Constructor via setting each data member
  //--------------------------------------------------------------------
 
}




IlcAODcascade::IlcAODcascade( IlcAODVertex* rAODVertexXi,  
                      Int_t         rChargeXi,
		      Double_t      rDcaXiDaughters,
		      Double_t      rDcaXiToPrimVertex,
		      Double_t      rDcaBachToPrimVertex,
		const Double_t*     rMomBach,
		const IlcAODv0&     rAODv0 ) :
  IlcAODv0(rAODv0),
  fDecayVertexXi(rAODVertexXi),
  fChargeXi( rChargeXi ),    
  fDcaXiDaughters( rDcaXiDaughters ),     
  fDcaXiToPrimVertex( rDcaXiToPrimVertex ),
  fDcaBachToPrimVertex( rDcaBachToPrimVertex ),
  fMomBachX( rMomBach[0] ),          
  fMomBachY( rMomBach[1] ),      
  fMomBachZ( rMomBach[2] )
{
  //--------------------------------------------------------------------
  // Constructor via setting each Xi data member + setting AODv0 
  //--------------------------------------------------------------------
 
}





IlcAODcascade& IlcAODcascade::operator=(const IlcAODcascade& rSource){
  //--------------------------------------------------------------------
  // Assignment overload
  //--------------------------------------------------------------------
  
  if (this == &rSource) return *this;
     
  IlcAODv0::operator=(rSource);
  
  this->fDecayVertexXi       = rSource.fDecayVertexXi;
  this->fChargeXi            = rSource.fChargeXi;

  this->fDcaXiDaughters      = rSource.fDcaXiDaughters;
  this->fDcaXiToPrimVertex   = rSource.fDcaXiToPrimVertex;
  this->fDcaBachToPrimVertex = rSource.fDcaBachToPrimVertex;

  this->fMomBachX            = rSource.fMomBachX;
  this->fMomBachY            = rSource.fMomBachY;
  this->fMomBachZ            = rSource.fMomBachZ;
  
  return *this;
}



IlcAODcascade::~IlcAODcascade(){
  //--------------------------------------------------------------------
  // Empty destructor
  //--------------------------------------------------------------------
}

void  IlcAODcascade::Fill(IlcAODVertex* rAODVertexXi, 
                      Int_t         rChargeXi,
		      Double_t      rDcaXiDaughters,
		      Double_t      rDcaXiToPrimVertex,
		      Double_t      rDcaBachToPrimVertex,
		const Double_t*     rMomBach,
		
		      IlcAODVertex* rAODVertexV0,
		      Double_t      rDcaV0Daughters,
		      Double_t      rDcaV0ToPrimVertex,
		const Double_t*     rMomPos,
		const Double_t*     rMomNeg,
		      Double_t*     rDcaDaughterToPrimVertex )
{
  //--------------------------------------------------------------------
  //  Fill the AODcascade
  //--------------------------------------------------------------------

  IlcAODv0::Fill(rAODVertexV0,rDcaV0Daughters,rDcaV0ToPrimVertex,rMomPos,rMomNeg,rDcaDaughterToPrimVertex);
      fDecayVertexXi =  rAODVertexXi;
      fChargeXi      =  rChargeXi;

      fDcaXiDaughters       = rDcaXiDaughters;   
      fDcaXiToPrimVertex    = rDcaXiToPrimVertex;   
      fDcaBachToPrimVertex  = rDcaBachToPrimVertex;

      fMomBachX = rMomBach[0];   
      fMomBachY = rMomBach[1];          
      fMomBachZ = rMomBach[2];        
}                



void IlcAODcascade::ResetXi(){
  //--------------------------------------------------------------------
  // Reset the values of the AOD data members to the default ones
  //--------------------------------------------------------------------
  
  ResetV0();
      
  GetDecayVertexXi()->SetChi2perNDF(-999);
  GetDecayVertexXi()->RemoveCovMatrix();              
  GetDecayVertexXi()->RemoveDaughters();               
  GetDecayVertexXi()->SetID(-1);                      
  GetDecayVertexXi()->SetParent((TObject*) 0x0);      
  GetDecayVertexXi()->SetPosition(-999, -999, -999);
  GetDecayVertexXi()->SetType( IlcAODVertex::kUndef );

  fChargeXi = 0;
  
  fDcaXiDaughters = 999;
  fDcaXiToPrimVertex = 999;
  fDcaBachToPrimVertex = 999;
  
  fMomBachX = 999;
  fMomBachY = 999;
  fMomBachZ = 999;
  
  
}

void IlcAODcascade::PrintXi(const Double_t& rPrimVtxX, 
                            const Double_t& rPrimVtxY, 
                            const Double_t& rPrimVtxZ) const
{
  //--------------------------------------------------------------------
  // Print the AOD data members
  //--------------------------------------------------------------------
   IlcAODv0::Print();  
   printf("- \n");
   printf("IlcAODcascade : posXiVtx (%.6f, %.6f, %.6f) \n", DecayVertexXiX(), DecayVertexXiY(), DecayVertexXiZ() );
   printf("IlcAODcascade : chargeXi =   %d \n", ChargeXi() );   
   printf("IlcAODcascade : dca (bachtpv %.6f, xid %.6f, xitpv-calc %.6f, xitpv-mb %.6f) \n",
                     DcaBachToPrimVertex(), 
		     DcaXiDaughters(), 
		     DcaXiToPrimVertex( rPrimVtxX, rPrimVtxY, rPrimVtxZ),
		     DcaXiToPrimVertex()  );
   printf("IlcAODcascade : cos(PtgAngle Xi) =     %.6f \n", CosPointingAngleXi(rPrimVtxX, rPrimVtxY, rPrimVtxZ) );
   
     
   printf("IlcAODcascade : posVtxXI  (x  %.6f, y  %.6f, z  %.6f) \n", DecayVertexXiX(),DecayVertexXiY(),DecayVertexXiZ() );
   printf("IlcAODcascade : decaylgth (V0 %.6f, Xi %.6f) \n", DecayLengthV0(),DecayLengthXi(rPrimVtxX, rPrimVtxY, rPrimVtxZ) );
   printf("IlcAODcascade : momBach   (px %.6f, py %.6f, pz %.6f, ptot2 %.6f) \n",  
                      MomBachX(), 
		      MomBachY(), 
		      MomBachZ(),
		      Ptot2Bach() );
   printf("IlcAODcascade : momXi     (px %.6f, py %.6f, pz %.6f, ptot2 %.6f, pt2 %.6f) \n", 
                      MomXiX(), 
		      MomXiY(),
		      MomXiZ(),
		      Ptot2Xi(),
		      Pt2Xi()   );
   printf("IlcAODcascade :  momAlongXi  (Bach     %.6f, V0 %.6f) \n", MomBachAlongXi(), MomV0AlongXi() );
   printf("IlcAODcascade :  cin (alphaXi %.6f, PtArmXi     %.6f) \n", AlphaXi(), PtArmXi() );
   printf("IlcAODcascade :  rap (Xi      %.6f, Omega       %.6f) \n", RapXi(),RapOmega() );
   printf("IlcAODcascade :  nrg (BachPi  %.6f, BachK-      %.6f, Omega %.6f, Xi  %.6f ) \n", 
                      EBachPion(), 
		      EBachKaon(), 
		      EOmega(),
		      EXi() );
   printf("IlcAODcascade : inv mass (Xi  %.6f, Omega       %.6f) \n",  MassXi(), MassOmega()  );
   printf("- \n");
   // Methods Not printed =  GetBachID(),  Chi2Xi()
 
 
}

Double_t IlcAODcascade::CosPointingAngleXi(const Double_t& rPrimVtxX, 
                                           const Double_t& rPrimVtxY, 
                                           const Double_t& rPrimVtxZ) const { 
 
  // Cosine of Xi pointing angle in 3D space, with respect to a point 
  // (primary vtx ...)
  
  TVector3 lMomXi( MomXiX(),MomXiY(),MomXiZ() );
  TVector3 lVectPrimVtxToXi(DecayVertexXiX() - rPrimVtxX,
			    DecayVertexXiY() - rPrimVtxY,
			    DecayVertexXiZ() - rPrimVtxZ);
  		
  Double_t lPtgAngle = lMomXi.Angle(lVectPrimVtxToXi);

  return TMath::Cos(lPtgAngle); 

}

Double_t IlcAODcascade::DcaXiToPrimVertex(const Double_t& rPrimVtxX, 
                                          const Double_t& rPrimVtxY, 
                                          const Double_t& rPrimVtxZ) const {
  //
  // Compute the DCA between this Xi and the primary vertex
  //
    Double_t rMomXiX = MomXiX();
    Double_t rMomXiY = MomXiY();
    Double_t rMomXiZ = MomXiZ();
    Double_t dx = (rPrimVtxY- DecayVertexXiY() )*rMomXiZ - (rPrimVtxZ- DecayVertexXiZ() )*rMomXiY; 
    Double_t dy = (rPrimVtxZ- DecayVertexXiZ() )*rMomXiX - (rPrimVtxX- DecayVertexXiX() )*rMomXiZ;
    Double_t dz = (rPrimVtxX- DecayVertexXiX() )*rMomXiY - (rPrimVtxY- DecayVertexXiY() )*rMomXiX;
  return TMath::Sqrt((dx*dx+dy*dy+dz*dz)/ Ptot2Xi() );
}

Int_t	IlcAODcascade::GetBachID()  const     {
  //
  // Return the ID of the bachelor
  //

	if( GetDecayVertexXi() == 0) return -1;

	IlcAODTrack *rBachTrack = (IlcAODTrack *) ( GetDecayVertexXi()->GetDaughter(0) );
	// The fDecayVertexXi should just have one stored daughter. To be managed within the IlcAnalysisTaskESDFilter
	Short_t rBachId = rBachTrack->GetID();
	return rBachId;
}

Double_t IlcAODcascade::EBachPion() const {
  static Double_t lMassPi = TDatabasePDG::Instance()->GetParticle("pi-")->Mass();
  return ::sqrt(Ptot2Bach() + lMassPi*lMassPi);
}

Double_t IlcAODcascade::EBachKaon() const {
  static Double_t lMassKaon = TDatabasePDG::Instance()->GetParticle("K-")->Mass();
  return ::sqrt(Ptot2Bach() + lMassKaon*lMassKaon);
}

Double_t IlcAODcascade::EXi() const {
  static Double_t lMassXi = TDatabasePDG::Instance()->GetParticle("Xi-")->Mass();
  return ::sqrt(Ptot2Xi() + lMassXi*lMassXi);
}

Double_t IlcAODcascade::EOmega() const {
  static Double_t lMassOmega = TDatabasePDG::Instance()->GetParticle("Omega-")->Mass();
  return ::sqrt(Ptot2Xi() + lMassOmega*lMassOmega);
}
