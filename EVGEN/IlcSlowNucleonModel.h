#ifndef ILCSLOWNUCLEONMODEL_H
#define ILCSLOWNUCLEONMODEL_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcSlowNucleonModel.h 10113 2004-06-05 02:35:41Z hristov $ */

#include "TObject.h"
class IlcCollisionGeometry;

class IlcSlowNucleonModel : public TObject
{
public:
    IlcSlowNucleonModel() {;}
    virtual ~IlcSlowNucleonModel(){;}
    virtual void GetNumberOfSlowNucleons(IlcCollisionGeometry* /*geo*/,
					 Int_t& /*ngp*/, Int_t& /*ngn*/,
					 Int_t& /*nbp*/, Int_t& /*nbn*/) const {;}
    
 protected:
  ClassDef(IlcSlowNucleonModel,1) // Gray Particle Model
};
#endif






