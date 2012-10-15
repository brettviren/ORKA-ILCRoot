#ifndef ILCVTXTENDERSUPPLY_H
#define ILCVTXTENDERSUPPLY_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

////////////////////////////////////////////////////////////////////////
//                                                                    //
//  Vertex tender, redo primary vertex on the fly                     //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#include <IlcTenderSupply.h>

class IlcESDVertex;

class IlcVtxTenderSupply: public IlcTenderSupply {
  
public:
  IlcVtxTenderSupply();
  IlcVtxTenderSupply(const char *name, const IlcTender *tender=NULL);
  
  virtual ~IlcVtxTenderSupply(){;}
  
  virtual void              Init(){;}
  virtual void              ProcessEvent();
  //
  Int_t   GetRefitAlgo()              const {return fRefitAlgo;}
  void    SetRefitAlgo(Int_t alg=-1)        {fRefitAlgo = alg;}
  //
private:
  
  IlcVtxTenderSupply(const IlcVtxTenderSupply&c);
  IlcVtxTenderSupply& operator= (const IlcVtxTenderSupply&c);

  IlcESDVertex *fDiamond;           //!Information about mean vertex  
  Int_t         fRefitAlgo;         //! optional request for vertex refit 

  ClassDef(IlcVtxTenderSupply, 1);  // Primary vertex tender task
};


#endif

