#ifndef ILCPVECV2_H
#define ILCPVECV2_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVECv2.h 53186 2011-11-25 14:28:01Z kharlov $ */

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
// Version of IlcPVECv0 which keeps all hits in TreeH
// I mean real hits not cumulated hits
//  This version is NOT recommended for Reconstruction analysis
//                  
//*-- Author: Gines MARTINEZ (SUBATECH)

// --- ROOT system ---

// --- IlcRoot header files ---
#include "IlcPVECv1.h"

class IlcPVECv2 : public IlcPVECv1 {

public:

  IlcPVECv2(void) ;
  IlcPVECv2(const char *name, const char *title="") ;
  virtual ~IlcPVECv2(void) ;

  using IlcPVECv1::AddHit;
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

  IlcPVECv2(IlcPVECv2 & PVEC);
  IlcPVECv2 & operator = (const IlcPVECv2 & /*PVEC*/);

  ClassDef(IlcPVECv2,1)  // Class IlcPVECv0 which allows to write ond disk al the information of the hits. 

};

#endif // IlcPVECV2_H
