#ifndef ILCRSTACK_H
#define ILCRSTACK_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice     */


//_________________________________________________________________________
//  Base Class for RSTACK     
//                  
//*-- Author: Laurent Aphecetche & Yves Schutz (SUBATECH)


// --- ROOT system ---
class TString ; 
class TFolder ;
class TTree ; 
class TRandom ; 

// --- IlcRoot header files ---
#include "IlcDetector.h" 
#include "IlcLog.h"
#include "IlcRSTACKGeometry.h" 
#include "IlcRSTACKTrigger.h"
class IlcRSTACKCalibData ;

class IlcRSTACK : public IlcDetector {

public:

  IlcRSTACK() ;
  IlcRSTACK(const char* name, const char* title="") ;  
  virtual ~IlcRSTACK() ; 
  virtual void   AddHit(Int_t, Int_t*, Float_t *) {
    // do not use this definition but the one below
    IlcFatal(Form("do not use")) ;
    
  }
  virtual void   AddHit( Int_t shunt, Int_t primary, Int_t track, 
			 Int_t id, Float_t *hits) = 0 ;   
  virtual IlcDigitizer* CreateDigitizer(IlcDigitizationInput* digInput) const;
  virtual void  CreateMaterials() ;            
  virtual void  Digits2Raw();
  virtual Bool_t Raw2SDigits(IlcRawReader* rawReader);
  virtual void  FinishRun() {;}
  virtual IlcRSTACKGeometry * GetGeometry() const
  {return IlcRSTACKGeometry::GetInstance(GetTitle(),"") ;  }

  virtual TList *   BuildGeometry2(void) {return 0;} ;             // creates the geometry for the ROOT display

  virtual void    Hits2SDigits();
  virtual Int_t   IsVersion(void) const = 0 ;
  virtual void    Init();
  virtual IlcTriggerDetector* CreateTriggerDetector() const 
    { return new IlcRSTACKTrigger(); }

  virtual IlcLoader* MakeLoader(const char* topfoldername);
  virtual void    SetTreeAddress();   
  virtual const TString Version() const {return TString(" ") ; } 

 private:                                        
  IlcRSTACK(IlcRSTACK & RSTACK);
  IlcRSTACK & operator = (const IlcRSTACK & /*rvalue*/);

 protected:
  IlcRSTACKCalibData * fgCalibData ; //!  Pointer to Calibration DB

  ClassDef(IlcRSTACK,6) // Photon Spectrometer Detector (base class)
} ;

#endif // ILCRSTACK_H
