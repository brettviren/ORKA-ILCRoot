#ifndef ILCRSTACKRECONSTRUCTOR_H
#define ILCRSTACKRECONSTRUCTOR_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcRSTACKReconstructor.h 53508 2011-12-10 16:17:18Z policheh $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.15  2007/10/01 20:24:08  kharlov
 * Memory leaks fixed
 *
 * Revision 1.14  2007/09/26 14:22:18  cvetan
 * Important changes to the reconstructor classes. Complete elimination of the run-loaders, which are now steered only from IlcReconstruction. Removal of the corresponding Reconstruct() and FillESD() methods.
 *
 * Revision 1.13  2007/08/30 10:40:27  cvetan
 * Minor
 *
 * Revision 1.12  2007/08/28 12:55:08  policheh
 * Loaders removed from the reconstruction code (C.Cheshkov)
 *
 * Revision 1.11  2007/07/24 17:20:35  policheh
 * Usage of RecoParam objects instead of hardcoded parameters in reconstruction.
 * (See $ILC_ROOT/RSTACK/macros/BeamTest2006/RawReconstruction.C).
 *
 * Revision 1.10  2007/07/11 13:43:30  hristov
 * New class IlcESDEvent, backward compatibility with the old IlcESD (Christian)
 *
 * Revision 1.9  2006/11/15 16:05:03  kharlov
 * New FillESD() for raw data is added
 *
 * Revision 1.8  2005/05/28 14:19:04  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
//  Wrapping class for reconstruction
//--
//-- Author: Yves Schutz (SUBATECH) 
// Reconstruction class. Redesigned from the old IlcReconstructionner class and 
// derived from STEER/IlcReconstructor. 
//_________________________________________________________________________

// --- ROOT system ---

#include <Riostream.h>
#include "IlcReconstructor.h" 
#include "IlcRSTACKRecoParam.h"
class IlcRSTACKDigitizer ;
class IlcRSTACKClusterizer ;
class IlcRSTACKClusterizerv1 ;
class IlcRSTACKTrackSegmentMaker ;
class IlcRSTACKPID ;
class IlcRSTACKSDigitizer ;
class IlcESDEvent ;
class IlcRawReader; 
class IlcRSTACKRecoParam;
class IlcRSTACKGeometry;
class IlcRSTACKCalibData ;
class IlcRSTACKTriggerParameters;

// --- Standard library ---

// --- IlcRoot header files ---

class IlcRSTACKReconstructor : public IlcReconstructor {

public:

  IlcRSTACKReconstructor() ; //ctor            
  virtual ~IlcRSTACKReconstructor() ; //dtor            

  static void                SetDebug()   { fgDebug = kTRUE ; }
  static void                ResetDebug() { fgDebug = kFALSE ; }
  static Bool_t              Debug() { return fgDebug ; }
  IlcTracker *CreateTracker() const;
  using IlcReconstructor::FillESD;
  virtual void               FillESD(TTree* digitsTree, TTree* clustersTree, 
				     IlcESDEvent* esd) const;
  using IlcReconstructor::Reconstruct;
  virtual void               Reconstruct(TTree* digitsTree, TTree* clustersTree) const;

  virtual Bool_t             HasDigitConversion() const {return kTRUE;};
  virtual void               ConvertDigits(IlcRawReader* rawReader, TTree* digitsTree) const;
  virtual Float_t            Calibrate(Float_t amp, Int_t absId) const ;
  virtual Float_t            CalibrateT(Float_t time, Int_t absId) const ;

  void FillMisalMatrixes(IlcESDEvent* esd)const ;
  
  static const IlcRSTACKRecoParam* GetRecoParam() {
    return dynamic_cast<const IlcRSTACKRecoParam*>(IlcReconstructor::GetRecoParam(4)); }
  static Float_t CorrectNonlinearity(Float_t oldEnergy) ;
  
  void readTRUParameters(IlcRSTACKTriggerParameters* parameters) const;
  
private:
  IlcRSTACKReconstructor(const IlcRSTACKReconstructor & rec); // Not implemented
  IlcRSTACKReconstructor & operator = (const IlcRSTACKReconstructor &); // Not implemented
  
  static Bool_t fgDebug ; //! verbosity controller
  IlcRSTACKGeometry          *fGeom;           // pointer to the RSTACK geometry
  IlcRSTACKClusterizerv1     *fClusterizer;    //! RSTACK clusterizer
  IlcRSTACKTrackSegmentMaker *fTSM;            //! RSTACK TrackSegmentMaker
  IlcRSTACKPID               *fPID;            //! RSTACK PID maker
  TClonesArray             *fTmpDigLG;       //! Temporary array of LG digits
  static TClonesArray      *fgDigitsArray;   //! Array of RSTACK digits
  static TObjArray         *fgEMCRecPoints;  //! Array of EMC rec.points
  static IlcRSTACKCalibData * fgCalibData ;    //! Calibration database if aval.
  static TClonesArray      *fgTriggerDigits; //! Array of RSTACK trigger digits

  ClassDef(IlcRSTACKReconstructor,10)  // RSTACK Reconstruction class

}; 

#endif // ILCRSTACKRECONSTRUCTOR_H
