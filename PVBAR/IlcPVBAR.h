#ifndef ILCPVBAR_H
#define ILCPVBAR_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice     */
/* $Id: IlcPVBAR.h 52261 2011-10-23 15:46:57Z hristov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.73  2007/08/07 14:12:03  kharlov
 * Quality assurance added (Yves Schutz)
 *
 * Revision 1.72  2007/02/13 10:52:08  policheh
 * Raw2SDigits() implemented
 *
 * Revision 1.71  2007/02/05 10:43:25  hristov
 * Changes for correct initialization of Geant4 (Mihaela)
 *
 * Revision 1.70  2007/01/17 17:28:56  kharlov
 * Extract ALTRO sample generation to a separate class IlcPVBARPulseGenerator
 *
 * Revision 1.69  2006/11/14 17:11:15  hristov
 * Removing inheritances from TAttLine, TAttMarker and IlcRndm in IlcModule. The copy constructor and assignment operators are moved to the private part of the class and not implemented. The corresponding changes are propagated to the detectors
 *
 * Revision 1.68  2006/08/11 12:36:25  cvetan
 * Update of the PVBAR code needed in order to read and reconstruct the beam test raw data (i.e. without an existing gilc.root)
 *
 * Revision 1.67  2006/04/07 08:42:00  hristov
 * Follow IlcAlignObj framework and remove IlcPVBARAlignData (Yu.Kharlov)
 *
 * Revision 1.66  2006/03/24 21:39:33  schutz
 * Modification needed to include PVBAR in the global trigger framework
 *
 * Revision 1.65  2006/03/07 18:56:25  kharlov
 * CDB is passed via environment variable
 *
 * Revision 1.64  2005/11/03 13:09:19  hristov
 * Removing meaningless const declarations (linuxicc)
 *
 * Revision 1.63  2005/07/26 13:32:39  kharlov
 * Restoring raw data fit from version of 29-Aug-2004
 *
 * Revision 1.62  2005/07/06 10:10:32  hristov
 * Moving the functions used to initialize TF1 and TF2 to the pivate part of the class
 *
 * Revision 1.61  2005/05/28 12:10:07  schutz
 * Copy constructor is corrected (by T.P.)
 *
 */


//_________________________________________________________________________
//  Base Class for PVBAR     
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
#include "IlcPVBARGeometry.h" 
#include "IlcPVBARTrigger.h"
class IlcPVBARCalibData ;

class IlcPVBAR : public IlcDetector {

public:

  IlcPVBAR() ;
  IlcPVBAR(const char* name, const char* title="") ;  
  virtual ~IlcPVBAR() ; 
  virtual void   AddHit(Int_t, Int_t*, Float_t *) {
    // do not use this definition but the one below
    IlcFatal(Form("do not use")) ;
    
  }
  virtual void   AddHit( Int_t shunt, Int_t primary, Int_t track, 
			 Int_t id, Float_t *hits ) = 0 ;   
  virtual IlcDigitizer* CreateDigitizer(IlcDigitizationInput* digInput) const;
  virtual void  CreateMaterials() ;            
  virtual void  Digits2Raw();
  virtual Bool_t Raw2SDigits(IlcRawReader* rawReader);
  virtual void  FinishRun() {;}
  virtual IlcPVBARGeometry * GetGeometry() const
  {return IlcPVBARGeometry::GetInstance(GetTitle(),"") ;  }

  virtual TList *   BuildGeometry2(void) {return 0;} ;             // creates the geometry for the ROOT display

  virtual void    Hits2SDigits();
  virtual Int_t   IsVersion(void) const = 0 ;
  virtual void    Init();
  virtual IlcTriggerDetector* CreateTriggerDetector() const 
    { return new IlcPVBARTrigger(); }

  virtual IlcLoader* MakeLoader(const char* topfoldername);
  virtual void    SetTreeAddress();   
  virtual const TString Version() const {return TString(" ") ; } 

 private:                                        
  IlcPVBAR(IlcPVBAR & PVBAR);
  IlcPVBAR & operator = (const IlcPVBAR & /*rvalue*/);

 protected:
  IlcPVBARCalibData * fgCalibData ; //!  Pointer to Calibration DB

  ClassDef(IlcPVBAR,6) // Photon Spectrometer Detector (base class)
} ;

#endif // ILCPVBAR_H
