#ifndef IlcVCuts_H
#define IlcVCuts_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//-------------------------------------------------------------------------
//     Event cuts base class
//     Author: Andreas Morsch, CERN
//-------------------------------------------------------------------------

#include <TNamed.h>

class IlcVCuts : public TNamed {

 public :
  IlcVCuts(); 
  IlcVCuts(const char* name, const char* title); 
  virtual ~IlcVCuts() { };
  IlcVCuts(const IlcVCuts& evt); 
  IlcVCuts& operator=(const IlcVCuts& evt);
  virtual Bool_t IsSelected(TObject* /* obj  */)  = 0;
  virtual UInt_t GetSelectionMask(const TObject* /* obj */) { return 0; }
  virtual TObject *GetStatistics(Option_t *) const {return 0;}
  ClassDef(IlcVCuts,1);
};

#endif
