#ifndef ILCRSTACKQADataMakerSim_H
#define ILCRSTACKQADataMakerSim_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcRSTACKQADataMakerSim.h 33177 2009-06-25 10:36:07Z hristov $ */

/*
  Produces the data needed to calculate the quality assurance. 
  All data must be mergeable objects.
  Y. Schutz CERN July 2007
*/


// --- ROOT system ---
class TH1F ; 
class TH1I ; 
class TObjArray ; 

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcQADataMakerSim.h"

class IlcRSTACKQADataMakerSim: public IlcQADataMakerSim {

public:
  enum HHitType_t    {kHits=0, kHitsMul} ; 
  enum HDigitType_t  {kDigits=0, kDigitsMul} ; 
  enum HSDigitType_t {kSDigits=0, kSDigitsMul} ; 

  IlcRSTACKQADataMakerSim() ;          // ctor
  IlcRSTACKQADataMakerSim(const IlcRSTACKQADataMakerSim& qadm) ;   
  IlcRSTACKQADataMakerSim& operator = (const IlcRSTACKQADataMakerSim& qadm) ;
  virtual ~IlcRSTACKQADataMakerSim() {;} // dtor
  
private:
  virtual void   EndOfDetectorCycle(IlcQAv1::TASKINDEX_t, TObjArray ** list) ;
  virtual void   InitHits() ; 
  virtual void   InitDigits() ; 
  virtual void   InitSDigits() ; 
  virtual void   MakeHits() ;
  virtual void   MakeHits(TTree * hitTree) ;
  virtual void   MakeDigits() ; 
  virtual void   MakeDigits(TTree * digitTree) ; 
  virtual void   MakeSDigits() ; 
  virtual void   MakeSDigits(TTree * sigitTree) ; 
  virtual void   StartOfDetectorCycle() ; 

  ClassDef(IlcRSTACKQADataMakerSim,2)  // description 

};

#endif // IlcRSTACKQADataMakerSim_H
