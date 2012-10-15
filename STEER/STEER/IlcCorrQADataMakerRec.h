#ifndef ILCCORRQADataMakerRec_H
#define ILCCORRQADataMakerRec_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcCORRQADataMakerRec.h 27570 2008-07-24 21:49:27Z cvetan $ */

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
#include "IlcQADataMakerRec.h"

class IlcCorrQADataMakerRec: public IlcQADataMakerRec {

public:
  IlcCorrQADataMakerRec(IlcQADataMaker **) ;          // ctor
  IlcCorrQADataMakerRec(const IlcCorrQADataMakerRec& qadm) ;   
  IlcCorrQADataMakerRec& operator = (const IlcCorrQADataMakerRec& qadm) ;
  virtual ~IlcCorrQADataMakerRec() ; // dtor
  
private:

  virtual void   EndOfDetectorCycle(IlcQAv1::TASKINDEX_t, TObjArray ** list) ;
  virtual void   InitESDs() ; 
  virtual void   InitRaws() ; 
  virtual void   InitRecPoints() ; 
  virtual void   InitRecoParams() ; 
  virtual void   MakeESDs(IlcESDEvent * esd) ;
  virtual void   MakeRecPoints(TTree * recpoTree) ; 
  virtual void   MakeRaws(IlcRawReader *) ; 
  virtual void   StartOfDetectorCycle() ; 

  Int_t fMaxRawVar ;              //! number of raw parameters in the ntuple
  IlcQADataMaker **    fqadm ;    //! array of detectors QA data makers pointers
  Double_t  * fVarvalue ;         //! array of value of the raw parameters
  ClassDef(IlcCorrQADataMakerRec,1)  // description 

};

#endif // IlcCORRQADataMakerRec_H
