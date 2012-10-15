#ifndef ILCAODCASCADE_H
#define ILCAODCASCADE_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//-------------------------------------------------------------------------
//     Implementation of the Analysis Oriented Data (AOD) Xi vertex class
//     Origin: A.Maire, IReS, antonin.maire@ires.in2p3.fr 
//             G.Van Buren, BNL,  gene@bnl.gov      (original STAR MuDsts)
//
//     Purpose: Having physics observables available for Xis
//-------------------------------------------------------------------------


#include "IlcAODv0.h"

class IlcAODTrack;
class IlcAODVertex;

class IlcAODcascade : public IlcAODv0 {

public:
  IlcAODcascade();
  
  IlcAODcascade(const IlcAODcascade& rSource);
  
  IlcAODcascade( IlcAODVertex* rAODVertexXi,  // No "const" param, constructor "TRef(const TObject*)" doesn't exist.
                      Int_t         rChargeXi,
		      Double_t      rDcaXiDaughters,
		      Double_t      rDcaXiToPrimVertex,
		      Double_t      rDcaBachToPrimVertex,
		const Double_t*     rMomBach,
		
		 IlcAODVertex* rAODVertexV0,  // No "const" param, see above.
		      Double_t      rDcaV0Daughters,    
		      Double_t      rDcaV0ToPrimVertex, 
		const Double_t*     rMomPos,            
		const Double_t*     rMomNeg,
		      Double_t*     rDcaDaughterToPrimVertex ); // const? -> Need agreement at IlcAODRecoDecay level

  		
  IlcAODcascade( IlcAODVertex* rAODVertexXi,  // No "const" param, see above.
                      Int_t         rChargeXi,
		      Double_t      rDcaXiDaughters,
		      Double_t      rDcaXiToPrimVertex,
		      Double_t      rDcaBachToPrimVertex,
		const Double_t*     rMomBach,
		const IlcAODv0&     rAODv0 );
		
		
		
  virtual ~IlcAODcascade();

  IlcAODcascade& operator=(const IlcAODcascade& rSource);
  
  void  Fill(IlcAODVertex* rAODVertexXi,  // No "const" param, see above.
                      Int_t     rChargeXi,
		      Double_t  rDcaXiDaughters,
		      Double_t  rDcaXiToPrimVertex,
		      Double_t  rDcaBachToPrimVertex,
		const Double_t* rMomBach,
			
		IlcAODVertex*   rAODVertexV0, // No "const" param, see above.
		      Double_t  rDcaV0Daughters,
		      Double_t  rDcaV0ToPrimVertex,
		const Double_t* rMomPos,
		const Double_t* rMomNeg,
		      Double_t* rDcaDaughterToPrimVertex ); // const? -> Need agreement at IlcAODRecoDecay level
  
//   void  Fill(   IlcAODVertex*   rAODVertexXi, 
//                       Int_t     rChargeXi,
// 		      Double_t  rDcaXiDaughters,
// 		      Double_t  rDcaXiToPrimVertex,
// 		      Double_t  rDcaBachToPrimVertex,
// 		const Double_t* rMomBach,
// 		const IlcAODv0& rAODv0  );          // -> To be implemented ...       
		      
		      
  void  ResetXi();                                       
  void  PrintXi(const Double_t& rPrimVtxX, 
                const Double_t& rPrimVtxY, 
                const Double_t& rPrimVtxZ) const; 

// ----
  Int_t    ChargeXi()                const;
  Int_t	   GetBachID()               const;
  Int_t    GetLabel()                const {return -1;}
  
  IlcAODVertex* GetDecayVertexXi()   const; 
  Double_t DecayVertexXiX()          const;
  Double_t DecayVertexXiY()          const;
  Double_t DecayVertexXiZ()          const;
  Double_t Chi2Xi()                  const;
    
  Double_t DcaBachToPrimVertex() const;
  Double_t DcaXiDaughters()          const;
  Double_t DcaXiToPrimVertex()       const;
  Double_t DcaXiToPrimVertex(const Double_t& rPrimVtxX, // hopefully, temporary method ...
                             const Double_t& rPrimVtxY, 
                             const Double_t& rPrimVtxZ) const;
  Double_t CosPointingAngleXi(const Double_t& rPrimVtxX, 
                              const Double_t& rPrimVtxY, 
                              const Double_t& rPrimVtxZ) const;  
  
  Double_t DecayLengthV0()           const;
  Double_t DecayLengthXi(const Double_t& rPrimVtxX, 
                         const Double_t& rPrimVtxY, 
                         const Double_t& rPrimVtxZ) const;
                        
  Double_t MomBachX()       const;
  Double_t MomBachY()       const;
  Double_t MomBachZ()       const;
  
  Double_t MomXiX()         const;
  Double_t MomXiY()         const;
  Double_t MomXiZ()         const;

// ---- 
  Double_t Ptot2Bach()      const;
  Double_t Ptot2Xi()        const;
  Double_t Pt2Xi()          const;
  Double_t MomBachAlongXi() const;
  Double_t MomV0AlongXi()   const;
  Double_t AlphaXi()        const;
  Double_t PtArmXi()        const;
  Double_t EBachPion()      const;
  Double_t EBachKaon()      const;
  Double_t EXi()            const;
  Double_t EOmega()         const;
  Double_t MassXi()         const;
  Double_t MassOmega()      const;
  Double_t RapXi()          const;
  Double_t RapOmega()       const;

protected:

  TRef          fDecayVertexXi;           // ref to decay vertex of the cascade (Xi vertex)
  Short_t       fChargeXi;                // charge of Xi
    
  Double32_t    fDcaXiDaughters;          // dca between Xi daughters
  Double32_t    fDcaXiToPrimVertex;       // dca of Xi to primary vertex 
  Double32_t    fDcaBachToPrimVertex; // dca of bachelor to primary vertex 
  
  Double32_t    fMomBachX;            // momemtum of bachelor along X
  Double32_t    fMomBachY;            // momemtum of bachelor along Y
  Double32_t    fMomBachZ;            // momemtum of bachelor along Z
  
  ClassDef(IlcAODcascade,1)   
};

//-----------------------------------------------------------

inline Int_t    IlcAODcascade::ChargeXi()   const     {return fChargeXi; }

inline IlcAODVertex* IlcAODcascade::GetDecayVertexXi() const { return  (IlcAODVertex*)fDecayVertexXi.GetObject(); }
inline Double_t IlcAODcascade::DecayVertexXiX() const {return GetDecayVertexXi()->GetX(); }
inline Double_t IlcAODcascade::DecayVertexXiY() const {return GetDecayVertexXi()->GetY(); }
inline Double_t IlcAODcascade::DecayVertexXiZ() const {return GetDecayVertexXi()->GetZ(); }

inline Double_t IlcAODcascade::Chi2Xi()         const {return GetDecayVertexXi()->GetChi2(); }

inline Double_t IlcAODcascade::DcaBachToPrimVertex() const {return fDcaBachToPrimVertex;}
inline Double_t IlcAODcascade::DcaXiDaughters()          const {return fDcaXiDaughters;}
inline Double_t IlcAODcascade::DcaXiToPrimVertex()       const {return fDcaXiToPrimVertex;}

inline Double_t IlcAODcascade::DecayLengthV0() const {
    return ::sqrt(::pow(DecayVertexV0X() - DecayVertexXiX(),2) +
		  ::pow(DecayVertexV0Y() - DecayVertexXiY(),2) +
		  ::pow(DecayVertexV0Z() - DecayVertexXiZ(),2));
}

inline Double_t IlcAODcascade::DecayLengthXi(const Double_t& rPrimVtxX, 
                                             const Double_t& rPrimVtxY, 
                                             const Double_t& rPrimVtxZ) const {
  return ::sqrt(::pow(DecayVertexXiX() - rPrimVtxX,2) +
		::pow(DecayVertexXiY() - rPrimVtxY,2) +
		::pow(DecayVertexXiZ() - rPrimVtxZ,2));
}

inline Double_t IlcAODcascade::MomBachX() const {return fMomBachX;}
inline Double_t IlcAODcascade::MomBachY() const {return fMomBachY;}
inline Double_t IlcAODcascade::MomBachZ() const {return fMomBachZ;}

inline Double_t IlcAODcascade::MomXiX() const {return MomV0X()+fMomBachX;}
inline Double_t IlcAODcascade::MomXiY() const {return MomV0Y()+fMomBachY;}
inline Double_t IlcAODcascade::MomXiZ() const {return MomV0Z()+fMomBachZ;}

inline Double_t IlcAODcascade::Ptot2Bach() const {
  return (::pow(fMomBachX,2) + ::pow(fMomBachY,2) + ::pow(fMomBachZ,2) );
}
inline Double_t IlcAODcascade::Ptot2Xi() const {return ( Pt2Xi() + ::pow(MomXiZ(),2) );}
inline Double_t IlcAODcascade::Pt2Xi() const {
  return (::pow(MomXiX(),2) + ::pow(MomXiY(),2) );
}

inline Double_t IlcAODcascade::MomBachAlongXi() const {
  Double_t rPtot2Xi = Ptot2Xi();
  if (rPtot2Xi)
    return (MomBachX()*MomXiX() +
	    MomBachY()*MomXiY() +
	    MomBachZ()*MomXiZ()) / ::sqrt(rPtot2Xi);
  else return 0.;
}

inline Double_t IlcAODcascade::MomV0AlongXi() const {
  Double_t rPtot2Xi = Ptot2Xi();
  if (rPtot2Xi)
    return (MomV0X()*MomXiX() +
	    MomV0Y()*MomXiY() +
	    MomV0Z()*MomXiZ()) / ::sqrt(rPtot2Xi);
  return 0.;
}

inline Double_t IlcAODcascade::AlphaXi() const {
  Double_t rMomV0AlongXi   = MomV0AlongXi();
  Double_t rMomBachAlongXi = MomBachAlongXi();

  return (((Float_t) ChargeXi()) * (rMomBachAlongXi - rMomV0AlongXi)/
                                   (rMomBachAlongXi + rMomV0AlongXi));
}

inline Double_t IlcAODcascade::PtArmXi() const {
  return ::sqrt(Ptot2Bach()-MomBachAlongXi()*MomBachAlongXi());
}

inline Double_t IlcAODcascade::MassXi() const {
  return ::sqrt(::pow(ELambda()+EBachPion(),2)-Ptot2Xi());
}

inline Double_t IlcAODcascade::MassOmega() const {
  return ::sqrt(::pow(ELambda()+EBachKaon(),2)-Ptot2Xi());
}

inline Double_t IlcAODcascade::RapXi() const {
  Double_t exi = EXi();
  Double_t rMomXiZ = MomXiZ();
  return 0.5*::log((exi+rMomXiZ)/(exi-rMomXiZ));
}

inline Double_t IlcAODcascade::RapOmega() const {
  Double_t eom = EOmega();
  Double_t rMomXiZ = MomXiZ();
  return 0.5*::log((eom+rMomXiZ)/(eom-rMomXiZ));
}

#endif
