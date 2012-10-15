#ifndef ILCCOLLISIONGEOMETRY_H
#define ILCCOLLISIONGEOMETRY_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcCollisionGeometry.h 53937 2012-01-16 15:01:00Z fca $ */
//-------------------------------------------------------------------------
//                          Class IlcCollisionGeometry
//   This is a class to handle the collison geometry defined by
//   the generator
//-------------------------------------------------------------------------

#include <Rtypes.h>

class IlcCollisionGeometry
{
public:
    IlcCollisionGeometry();
    IlcCollisionGeometry(const IlcCollisionGeometry& cgeo);
    virtual ~IlcCollisionGeometry(){;}
    // Getters
    Float_t ImpactParameter() const  {return fImpactParameter;}
    Float_t ReactionPlaneAngle() const {return fReactionPlaneAngle;}
    Int_t   HardScatters() const {return fNHardScatters;}
    Int_t   ProjectileParticipants() const {return fNProjectileParticipants;}
    Int_t   TargetParticipants() const {return fNTargetParticipants;}
    Int_t   ProjSpectatorsn() const {return fProjectileSpecn;}
    Int_t   ProjSpectatorsp() const {return fProjectileSpecp;}
    Int_t   TargSpectatorsn() const {return fTargetSpecn;	 }
    Int_t   TargSpectatorsp() const {return fTargetSpecp;	 }
    Int_t   NN() const  {return fNNColl;}
    Int_t   NNw() const {return fNNwColl;}
    Int_t   NwN() const {return fNwNColl;}
    Int_t   NwNw() const {return fNwNwColl;}
    // Setters
    void SetImpactParameter(Float_t b)     {fImpactParameter=b;}
    void SetReactionPlaneAngle(Float_t phi)     {fReactionPlaneAngle = phi;}
    void SetHardScatters(Int_t n)  {fNHardScatters=n;}
    void SetParticipants(Int_t np, Int_t nt)
	{fNProjectileParticipants=np, fNTargetParticipants=nt;}
    void SetCollisions(Int_t nn, Int_t nnw, Int_t nwn, Int_t nwnw)
	{fNNColl=nn, fNNwColl=nnw, fNwNColl=nwn,  fNwNwColl=nwnw;}
    void SetSpectators(Int_t nprojspecn, Int_t nprojspecp, Int_t ntargspecn, Int_t ntargspecp)
	{fProjectileSpecn=nprojspecn, fProjectileSpecp=nprojspecp, 
	 fTargetSpecn=ntargspecn, fTargetSpecp=ntargspecp;}
 protected:
    Int_t   fNHardScatters;            // Number of hard scatterings
    Int_t   fNProjectileParticipants;  // Number of projectiles participants
    Int_t   fNTargetParticipants;      // Number of target participants
    Int_t   fNNColl;                   // Number of N-N collisions
    Int_t   fNNwColl;                  // Number of N-Nwounded collisions
    Int_t   fNwNColl;                  // Number of Nwounded-N collisons
    Int_t   fNwNwColl;                 // Number of Nwounded-Nwounded collisions
    Int_t   fProjectileSpecn;	       // Num. of spectator neutrons from projectile nucleus
    Int_t   fProjectileSpecp;	       // Num. of spectator protons from projectile nucleus
    Int_t   fTargetSpecn;    	       // Num. of spectator neutrons from target nucleus
    Int_t   fTargetSpecp;    	       // Num. of spectator protons from target nucleus
    Float_t fImpactParameter;          // Impact Parameter
    Float_t fReactionPlaneAngle;       // Reaction plane angle

private:
    IlcCollisionGeometry& operator=(const IlcCollisionGeometry& cg); //Not implemented
    
  ClassDef(IlcCollisionGeometry,3)     // Collision Geometry
};
#endif






