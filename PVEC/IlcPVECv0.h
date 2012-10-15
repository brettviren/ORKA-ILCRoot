#ifndef ILCPVECV0_H
#define ILCPVECV0_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVECv0.h 30416 2008-12-16 07:46:03Z fca $ */

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
// Implementation version v0 of PVEC Manager class 
//*--                  
//*-- Author: Yves Schutz (SUBATECH)

// --- ROOT system ---

class TFile;
class TFolder;

// --- IlcRoot header files ---
#include "IlcPVEC.h"

class IlcPVECv0 : public IlcPVEC {

 public:

  IlcPVECv0() {}
  IlcPVECv0(const char *name, const char *title="") ;
  virtual ~IlcPVECv0(void){
    // dtor
  } 

//    virtual void   AddHit( Int_t shunt, Int_t primary, Int_t track, Int_t id, Float_t *hits ) {
  //this function is not a final-overrider for IlcPVEC::AddHit, to
  //supress warning, I use using-declaration :)
  using IlcPVEC::AddHit;
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
  IlcPVECv0(IlcPVECv0 & PVEC);
  IlcPVECv0 & operator = (const IlcPVECv0 & /*rvalue*/);

  ClassDef(IlcPVECv0,1)  // Implementation of PVEC manager class for layout EMC+PPSD
    
    };
    
#endif // IlcPVECV0_H
