#ifndef ILCRSTACKLINK_H
#define ILCRSTACKLINK_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcRSTACKLink.h 17222 2007-03-06 06:53:55Z kharlov $ */

//_________________________________________________________________________
//  Algorithm class used only by IlcRSTACKTrackSegmentMaker 
//  Links recpoints
// into tracksegments                
//*-- Author: Dmitri Peressounko (SUBATECH)

// --- ROOT system ---

#include "TObject.h"

// --- Standard library ---

// --- IlcRoot header files ---

class IlcRSTACKLink : public  TObject{
  
public:
  
  IlcRSTACKLink() ;  // ctor            
  IlcRSTACKLink( Float_t x, Float_t z, Int_t emc, Int_t cpv, Int_t track) ;  // ctor            
  virtual ~IlcRSTACKLink(){
    // dtor
  }
  Int_t   Compare(const TObject * obj) const;
  Int_t   GetEmc(void) const { return fEmcN; }
  Int_t   GetCpv(void) const { return fCpvN ; }
  void    GetXZ(Float_t &dx, Float_t &dz) const { dx=fx ; dz = fz ; } 
  Int_t   GetTrack(void) const { return fTrack ; }
  Bool_t  IsSortable() const{ 
    // tells if this is a sortable object 
    return kTRUE ; 
  }
  
private:
  
  Int_t fEmcN ;  // Emc index
  Int_t fCpvN ;  // Cpv index 
  Int_t fTrack;  // Charged tracked within a minimum distance of the EMC
  Float_t fx ;   // Distance between EMC and CPV RecPoints in a track segment
  Float_t fz ;   // Distance between EMC and CPV RecPoints in a track segment
  
  ClassDef(IlcRSTACKLink,2)  // Auxilliary algorithm class used by IlcRSTACKTrackSegmentMaker

};

#endif // IlcRSTACKLINK_H
