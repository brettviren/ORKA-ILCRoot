#ifndef IlcOADBFillingScheme_H
#define IlcOADBFillingScheme_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//-------------------------------------------------------------------------
//     OADB container for filling scheme information (BX ids, name ...)
//     Author: Michele Floris, CERN
//-------------------------------------------------------------------------

#include <TNamed.h>
#include "TMap.h"
#include "TObjString.h"


class IlcOADBFillingScheme : public TNamed {

 public :
  IlcOADBFillingScheme();
  IlcOADBFillingScheme(char* name);
  virtual ~IlcOADBFillingScheme();
  void Init();
  
  // Getters
  const char * GetBXIDs(const char * beamSide) const; 
  const char * GetFillingSchemeName() const { return fFSName; } 
  // Setters
  void SetBXIDs(const char * beamSide, const char * bxids) { fBXIds->Add(new TObjString(beamSide), new TObjString(bxids)); }
  void SetFillingSchemeName(const char * name) { fFSName = name; }
  // Browse
  virtual Bool_t	IsFolder() const { return kTRUE; }
  void Browse(TBrowser *b);
  // Print
  virtual void	Print(Option_t* option = "") const;

 private :

  IlcOADBFillingScheme(const IlcOADBFillingScheme& cont);            // to be implemented
  IlcOADBFillingScheme& operator=(const IlcOADBFillingScheme& cont); // to be implemented
  
  TString fFSName               ; // Name of the filling scheme 
  TMap * fBXIds              ; // Map from the beam side bunch crossing number. Beam side is "B", "A", "C", "E".

  ClassDef(IlcOADBFillingScheme, 1);
};

#endif
