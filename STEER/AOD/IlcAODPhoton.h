#ifndef IlcAODPHOTON_H
#define IlcAODPHOTON_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAODPhoton.h 54136 2012-01-25 12:33:40Z kleinb $ */

//-------------------------------------------------------------------------
//     AOD photon class
//     Author: Yves Schutz, CERN
//-------------------------------------------------------------------------

#include <TLorentzVector.h>
#include "IlcVParticle.h"
#include "IlcAODVertex.h"


class IlcAODPhoton : public IlcVParticle {

 public:
    IlcAODPhoton();
    IlcAODPhoton(Double_t px, Double_t py, Double_t pz, Double_t e);
    IlcAODPhoton(TLorentzVector & p);  
    virtual ~IlcAODPhoton();
    IlcAODPhoton(const IlcAODPhoton& photon); 
    IlcAODPhoton& operator=(const IlcAODPhoton& photon);
// IlcVParticle methods
    virtual Double_t Px()         const { return fMomentum->Px();      }
    virtual Double_t Py()         const { return fMomentum->Py();      }
    virtual Double_t Pz()         const { return fMomentum->Pz();      }
    virtual Double_t Pt()         const { return fMomentum->Pt();      }
    virtual Double_t P()          const { return fMomentum->P();       }
    virtual Bool_t   PxPyPz(Double_t p[3]) const { p[0] = Px(); p[1] = Py(); p[2] = Pz(); return kTRUE; }
    virtual Double_t OneOverPt()  const { return 1. / fMomentum->Pt(); }
    virtual Double_t Phi()        const;
    virtual Double_t Theta()      const { return fMomentum->Theta();   }
    virtual Double_t E()          const { return fMomentum->E();       }
    virtual Double_t M()          const { return fMomentum->M();       }
    virtual Double_t Eta()        const { return fMomentum->Eta();     }
    virtual Double_t Y()          const { return fMomentum->Rapidity();}
    virtual Double_t Xv()         const {return -999.;} // put reasonable values here
    virtual Double_t Yv()         const {return -999.;} //
    virtual Double_t Zv()         const {return -999.;} //
    virtual Bool_t   XvYvZv(Double_t x[3]) const { x[0] = Xv(); x[1] = Yv(); x[2] = Zv(); return kTRUE; }  
    //
    
    virtual void     Print(Option_t* /*option*/) const;
    
// Dummy  
    virtual Short_t Charge()        const { return 0;}
    virtual const   Double_t* PID() const { return NULL;}
    virtual Int_t   GetLabel()      const { return -1;}
  // Dummy
    virtual Int_t    PdgCode()      const {return 0;}

//
    
    
 private:
    TLorentzVector* fMomentum;           // Photon 4-momentum vector
    ClassDef(IlcAODPhoton,1);
};

inline Double_t IlcAODPhoton::Phi() const
{
    // Return phi
    Double_t phi = fMomentum->Phi();
    if (phi < 0.) phi += 2. * TMath::Pi();
    return phi;
}

#endif
