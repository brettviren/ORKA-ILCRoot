#ifndef ILCVERTEXGENERATOR_H
#define ILCVERTEXGENERATOR_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcVertexGenerator.h 50615 2011-07-16 23:19:19Z hristov $ */

#include <TObject.h>
#include <TVector3.h>


class IlcVertexGenerator: public TObject {
 public:
  virtual TVector3 GetVertex() = 0;

  ClassDef(IlcVertexGenerator, 1)    // Base class for vertex generators
};

#endif














