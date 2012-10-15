#ifndef ILCPIPE_H
#define ILCPIPE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPIPE.h 1279 2000-06-11 12:38:40Z morsch $ */

////////////////////////////////////////////////
//  Manager class for detector: PIPE          //
////////////////////////////////////////////////
 
#include "IlcModule.h"
 
 
class IlcPIPE : public IlcModule {
 
public:
  IlcPIPE();
  IlcPIPE(const char *name, const char *title);
  virtual      ~IlcPIPE() {}
  
  ClassDef(IlcPIPE,1)  //Beam Pipe base Class
};

#endif
