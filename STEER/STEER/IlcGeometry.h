#ifndef ILCGEOMETRY_H
#define ILCGEOMETRY_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGeometry.h 50615 2011-07-16 23:19:19Z hristov $ */

////////////////////////////////////////////////
//  IlcGeometry Base Class pABC               //
//                                            //
//  Author Yves Schutz     SUBATECH           //
//                                            //  
//                                            //
////////////////////////////////////////////////

// --- ROOT system ---

class TParticle;
class TVector3;
#include <TNamed.h>
#include <TMatrixFfwd.h>

class IlcRecPoint;

class IlcGeometry : public TNamed {

public:

  IlcGeometry() ;          // ctor            
  virtual ~IlcGeometry() ; // dtor
 
  virtual void GetGlobal(const IlcRecPoint * p, TVector3 & pos, TMatrixF & mat) const = 0; 
  virtual void GetGlobal(const IlcRecPoint * p, TVector3 & pos) const = 0; 
  virtual Bool_t Impact(const TParticle * particle) const             = 0;

protected:

  IlcGeometry(const Text_t* name, const Text_t* title) : TNamed (name,title) {}                                   

  ClassDef(IlcGeometry,1)  // Base class for detector geometry

};

#endif // ILCGEOMETRY_H



