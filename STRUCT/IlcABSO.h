#ifndef ILCABSO_H
#define ILCABSO_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcABSO.h 47909 2011-02-28 15:05:53Z fca $ */

////////////////////////////////////////////////
//  Manager class for detector: ABSO          //
////////////////////////////////////////////////
 
#include "IlcModule.h"
 
 
class IlcABSO : public IlcModule {
    
 public:
    IlcABSO();
    IlcABSO(const char *name, const char *title);
    virtual      ~IlcABSO() {}
    virtual void    CreateGeometry();
    virtual void    CreateMaterials();
    virtual void    Init();
    virtual Int_t   IsVersion() const {return 0;}
    virtual Int_t   GetMatId(Int_t imat) const;
    virtual Int_t   NumberOfLayers(Int_t i) const {return fNLayers[i];}
    virtual Float_t ZPositionOfLayer(Int_t i, Int_t il) const 
      {return fZLayers[i][il];}    
    virtual Int_t   MaterialOfLayer (Int_t i, Int_t il) const 
      {return fMLayers[i][il];}    	  
 protected:
    Int_t   fNLayers[2];        // Number of Material Layers in the tracking Region
    Float_t fZLayers[2][15];     // z-position of layers
    Int_t   fMLayers[2][15];     // Material type of layers
  ClassDef(IlcABSO,1)  // Muon Absorber Class
};

#endif
