#ifndef ILCPVBARV0_H
#define ILCPVBARV0_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARv0.h 30416 2008-12-16 07:46:03Z fca $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.44  2006/09/27 19:55:57  kharlov
 * Alignment object with symbolic volume names are introduced
 *
 * Revision 1.43  2005/05/28 14:19:05  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
// Implementation version v0 of PVBAR Manager class 
//*--                  
//*-- Author: Yves Schutz (SUBATECH)

// --- ROOT system ---

class TFile;
class TFolder;

// --- IlcRoot header files ---
#include "IlcPVBAR.h"

class IlcPVBARv0 : public IlcPVBAR {

 public:

  IlcPVBARv0() {}
  IlcPVBARv0(const char *name, const char *title="") ;
  virtual ~IlcPVBARv0(void){
    // dtor
  } 

//    virtual void   AddHit( Int_t shunt, Int_t primary, Int_t track, Int_t id, Float_t *hits ) {
  //this function is not a final-overrider for IlcPVBAR::AddHit, to
  //supress warning, I use using-declaration :)
  using IlcPVBAR::AddHit;
  virtual void   AddHit( Int_t, Int_t, Int_t, Int_t, Float_t*) {
    // useless since there are no hits
    Fatal("AddHit", "not to be used with v0") ;
  }

  TList *   BuildGeometry2(void) ;             // creates the geometry for the ROOT display
  TList *   BuildGeometryforEMC(void) ;      // creates the PVBAR EMC geometry for the ROOT display

  virtual void   CreateGeometry(void) ;            // creates the geometry for GEANT
  void           CreateGeometryforEMC(void) ;     // creates the PVBAR geometry for GEANT
  //  void           CreateGeometryforPPSD(void) ;     // creates the PPSD geometry for GEANT
  void           CreateGeometryforCPV(void) ;      // creates the CPV  geometry for GEANT
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
  IlcPVBARv0(IlcPVBARv0 & PVBAR);
  IlcPVBARv0 & operator = (const IlcPVBARv0 & /*rvalue*/);

  ClassDef(IlcPVBARv0,1)  // Implementation of PVBAR manager class for layout EMC+PPSD
    
    };
    
#endif // IlcPVBARV0_H
