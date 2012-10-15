#ifndef ILCKINK_H
#define ILCKINK_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//-------------------------------------------------------------------------
//                          Kink Vertex Class
//          This class is part of the reconstruction
//    Origin: Marian Ivanov marian.ivanov@cern.ch
//-------------------------------------------------------------------------

#include "IlcESDkink.h"

class IlcKink : public IlcESDkink {
public:
  IlcKink(){;}             //constructor
  void Update();            //update
  ClassDef(IlcKink,1)      //kink vertex
};

#endif


