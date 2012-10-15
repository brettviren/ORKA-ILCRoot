#ifndef ILCV0_H
#define ILCV0_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


//-------------------------------------------------------------------------
//                          V0 Vertex Class
//          This class is part of the reconstruction
//    Origin: Marian Ivanov marian.ivanov@cern.ch
//-------------------------------------------------------------------------

#include "IlcESDv0.h"

class IlcV0: public IlcESDv0 {
public:
  IlcV0(): IlcESDv0() {} 
  void Update(Float_t vertex[3]);            //update
  ClassDef(IlcV0,1)      // V0 MI vertex
};


#endif
