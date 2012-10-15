#ifndef ILCPXCONEJETFINDER_H
#define ILCPXCONEJETFINDER_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
 
 
//---------------------------------------------------------------------
// Pxcone Jet finder 
// manages the search for jets 
// Author: jgcn@mda.cinvestav.mx
//---------------------------------------------------------------------

#include "IlcJetFinder.h"

class IlcPxconeJetHeader;

class IlcPxconeJetFinder : public IlcJetFinder 
{
 public:

  IlcPxconeJetFinder();
  ~IlcPxconeJetFinder();

  // getters

  // setters
  void SetJetHeader(IlcPxconeJetHeader* h) {fHeader= h;}
  // others
  void Reset();
  void FindJets();
  void WriteJHeaderToFile() const;

 protected:
  IlcPxconeJetFinder(const IlcPxconeJetFinder& rPxJet);
  IlcPxconeJetFinder& operator = (const IlcPxconeJetFinder& rhsh);

  IlcPxconeJetHeader* fHeader;         // pointer to jet header

  ClassDef(IlcPxconeJetFinder,1)
};

#endif
