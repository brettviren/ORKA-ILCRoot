#ifndef ILCRSTACKV0_H
#define ILCRSTACKV0_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


//_________________________________________________________________________
// Implementation version v0 of RSTACK Manager class 
//*--                  
//*-- Author: Yves Schutz (SUBATECH)

// --- ROOT system ---

class TFile;
class TFolder;

// --- IlcRoot header files ---
#include "IlcRSTACK.h"

class IlcRSTACKv0 : public IlcRSTACK {

 public:

  IlcRSTACKv0() {}
  IlcRSTACKv0(const char *name, const char *title="") ;
  virtual ~IlcRSTACKv0(void){
    // dtor
  } 

//    virtual void   AddHit( Int_t shunt, Int_t primary, Int_t track, Int_t id, Float_t *hits ) {
  //this function is not a final-overrider for IlcRSTACK::AddHit, to
  //supress warning, I use using-declaration :)
  using IlcRSTACK::AddHit;
  virtual void   AddHit( Int_t, Int_t, Int_t, Int_t, Float_t*) {
    // useless since there are no hits
    Fatal("AddHit", "not to be used with v0") ;
  }

  TList *   BuildGeometry2(void) ;             // creates the geometry for the ROOT display
  
  virtual void   CreateGeometry(void) ;            // creates the geometry for GEANT

  void           CreateGeometryforSupport(void) ;  // creates the Support geometry for GEANT
  virtual void   AddAlignableVolumes() const;      // define sym.names for alignable volumes

  virtual Float_t ZMin() const;                    // overall dimension of the module (min)
  virtual Float_t ZMax() const;                    // overall dimension of the module (max)

  virtual void   Init(void) ;                      // does nothing
  virtual Int_t  IsVersion(void) const { 
    // Gives the version number 
    return 0 ; 
  }
  virtual const TString Version(void)const { 
    // As above
    return TString("v0") ; 
  }
  
  
 private:
  IlcRSTACKv0(IlcRSTACKv0 & RSTACK);
  IlcRSTACKv0 & operator = (const IlcRSTACKv0 & /*rvalue*/);

  ClassDef(IlcRSTACKv0,1)  // Implementation of RSTACK manager class for layout EMC+PPSD
    
    };
    
#endif // IlcRSTACKV0_H
