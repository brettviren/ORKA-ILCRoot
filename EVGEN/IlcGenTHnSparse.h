#ifndef ILCGENTHNSPARSE_H
#define ILCGENTHNSPARSE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
// Particle generator according to 4 correlated variables : here
// z, ptot, r, theta. The input is a THnSparse object included in
// the root file (path and name to be set via the SetTHnSparse method).
// This class is similar to IlcGenFunction.

#include "IlcGenerator.h"
#include "THnSparse.h"

class IlcGenTHnSparse : public IlcGenerator
{
public:

  IlcGenTHnSparse();
  IlcGenTHnSparse(const IlcGenTHnSparse& func);
  IlcGenTHnSparse &operator=(const IlcGenTHnSparse& func);
  virtual ~IlcGenTHnSparse();
  virtual void Generate();
  virtual void Init();
  virtual void SetPart(Int_t part, Bool_t both) {fIpart=part; fBoth=both;}
  virtual void SetThnSparse(char *file_name, char *thn_name);
  
private:

  THnSparse *fHn; // Pointer to THnSparse object
  TFile *fFile;   // Pointer to input file
  Int_t fIpart;   // Particle type
  Bool_t fBoth;   // Particle and anti-particle type
      
  ClassDef(IlcGenTHnSparse,2)
};

#endif
