#ifndef ILCPVECRECONSTRUCTOR_H
#define ILCPVECRECONSTRUCTOR_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVECReconstructor.h 53508 2011-12-10 16:17:18Z policheh $ */

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
 * (See $ILC_ROOT/PVEC/macros/BeamTest2006/RawReconstruction.C).
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
#include "IlcPVECRecoParam.h"
class IlcPVECDigitizer ;
class IlcPVECClusterizer ;
class IlcPVECClusterizerv1 ;
class IlcPVECTrackSegmentMaker ;
class IlcPVECPID ;
class IlcPVECSDigitizer ;
class IlcESDEvent ;
class IlcRawReader; 
class IlcPVECRecoParam;
class IlcPVECGeometry;
class IlcPVECCalibData ;
class IlcPVECTriggerParameters;

// --- Standard library ---

// --- IlcRoot header files ---

class IlcPVECReconstructor : public IlcReconstructor {

public:

  IlcPVECReconstructor() ; //ctor            
  virtual ~IlcPVECReconstructor() ; //dtor            

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
  
  static const IlcPVECRecoParam* GetRecoParam() {
    return dynamic_cast<const IlcPVECRecoParam*>(IlcReconstructor::GetRecoParam(4)); }
  static Float_t CorrectNonlinearity(Float_t oldEnergy) ;
  
  void readTRUParameters(IlcPVECTriggerParameters* parameters) const;
  
private:
  IlcPVECReconstructor(const IlcPVECReconstructor & rec); // Not implemented
  IlcPVECReconstructor & operator = (const IlcPVECReconstructor &); // Not implemented
  
  static Bool_t fgDebug ; //! verbosity controller
  IlcPVECGeometry          *fGeom;           // pointer to the PVEC geometry
  IlcPVECClusterizerv1     *fClusterizer;    //! PVEC clusterizer
  IlcPVECTrackSegmentMaker *fTSM;            //! PVEC TrackSegmentMaker
  IlcPVECPID               *fPID;            //! PVEC PID maker
  TClonesArray             *fTmpDigLG;       //! Temporary array of LG digits
  static TClonesArray      *fgDigitsArray;   //! Array of PVEC digits
  static TObjArray         *fgEMCRecPoints;  //! Array of EMC rec.points
  static IlcPVECCalibData * fgCalibData ;    //! Calibration database if aval.
  static TClonesArray      *fgTriggerDigits; //! Array of PVEC trigger digits

  ClassDef(IlcPVECReconstructor,10)  // PVEC Reconstruction class

}; 

#endif // ILCPVECRECONSTRUCTOR_H
