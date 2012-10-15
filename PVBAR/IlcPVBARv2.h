#ifndef ILCPVBARV2_H
#define ILCPVBARV2_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARv2.h 53186 2011-11-25 14:28:01Z kharlov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.19  2005/07/01 20:01:36  kharlov
 * Warning fix on AddHit in gcc 3.4.2
 *
 * Revision 1.18  2005/05/28 14:19:05  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
// Version of IlcPVBARv0 which keeps all hits in TreeH
// I mean real hits not cumulated hits
//  This version is NOT recommended for Reconstruction analysis
//                  
//*-- Author: Gines MARTINEZ (SUBATECH)

// --- ROOT system ---

// --- IlcRoot header files ---
#include "IlcPVBARv1.h"

class IlcPVBARv2 : public IlcPVBARv1 {

public:

  IlcPVBARv2(void) ;
  IlcPVBARv2(const char *name, const char *title="") ;
  virtual ~IlcPVBARv2(void) ;

  using IlcPVBARv1::AddHit;
  virtual void    AddHit( Int_t shunt, Int_t primary, Int_t id, Float_t *hits); 
  virtual Int_t   IsVersion(void) const { 
    // Gives the version number 
    return 2 ; 
  }
  virtual const TString Version(void)const { 
    // returns the version number 
    return TString("v2") ; 
  }

private:

  IlcPVBARv2(IlcPVBARv2 & PVBAR);
  IlcPVBARv2 & operator = (const IlcPVBARv2 & /*PVBAR*/);

  ClassDef(IlcPVBARv2,1)  // Class IlcPVBARv0 which allows to write ond disk al the information of the hits. 

};

#endif // IlcPVBARV2_H
