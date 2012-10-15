#ifndef ILCPVBARPIDV0_H
#define ILCPVBARPIDV0_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARPIDv0.h 20497 2007-08-28 12:55:08Z policheh $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.12  2007/03/06 06:57:05  kharlov
 * DP:calculation of distance to CPV done in TSM
 *
 * Revision 1.11  2006/09/07 18:31:08  kharlov
 * Effective c++ corrections (T.Pocheptsov)
 *
 * Revision 1.10  2005/05/28 14:19:04  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
// Implementation version v0 of the PVBAR particle identifier 
// Identification is based on information from PPSD and EMC
// Oh yeah                 
//*-- Author: Yves Schutz (SUBATECH)

// --- ROOT system ---
class TFormula ;
class TVector3 ;

// --- Standard library ---

// --- IlcRoot header files ---
class IlcPVBAREmcRecPoint ;
class IlcPVBARRecPoint ;

#include "IlcPVBARPID.h"

class  IlcPVBARPIDv0 : public IlcPVBARPID {

public:

  IlcPVBARPIDv0() ;          // ctor            
  IlcPVBARPIDv0(IlcPVBARGeometry *geom);
  virtual ~IlcPVBARPIDv0() ; // dtor, empty, memory leak in fFormula member

  //Compiler generated should be ok, because destructor is empty.
  IlcPVBARPIDv0(const IlcPVBARPIDv0 & rhs);
  IlcPVBARPIDv0 & operator = (const IlcPVBARPIDv0 & rhs);

  virtual void TrackSegments2RecParticles(Option_t * option);

  virtual void PlotDispersionCuts()const ;
  virtual void Print(const Option_t * = "")const ; 
  virtual void SetIdentificationMethod(const char * option = "CPV DISP" ){fIDOptions = option ;} 
  virtual void SetShowerProfileCut(const char * formula = "0.35*0.35 - (x-1.386)*(x-1.386) - 1.707*1.707*(y-1.008)*(y-1.008)") ;
  virtual void SetDispersionCut(Float_t cut){fDispersion = cut ; } 
  virtual void SetCpvtoEmcDistanceCut(Float_t cut )      {fCpvEmcDistance = cut ;}
  virtual void SetTimeGate(Float_t gate)                 {fTimeGate = gate ;}

  virtual const char * Version() const { return "pid-v0" ; }  
                     
 private:
  
  void     MakeRecParticles(void ) ;
//  Float_t  GetDistance(IlcPVBAREmcRecPoint * emc, IlcPVBARRecPoint * cpv, Option_t * Axis)const ; // Relative Distance CPV-EMC
  TVector3 GetMomentumDirection(IlcPVBAREmcRecPoint * emc, IlcPVBARRecPoint * cpv)const ;
  void     PrintRecParticles(Option_t * option) ;

 private:

  TString                fIDOptions ;         // PID option

  IlcPVBARClusterizer   * fClusterizer ;       // !
  IlcPVBARTrackSegmentMaker * fTSMaker ;       // !

  TFormula             * fFormula ;           // formula to define cut on the shower elips axis
  Float_t                fDispersion ;        // dispersion cut
  Float_t                fCpvEmcDistance ;    // Max EMC-CPV distance
  Float_t                fTimeGate ;          // Time of the latest EmcRecPoint accepted as EM

  ClassDef( IlcPVBARPIDv0,2)  // Particle identifier implementation version 1

};

#endif // IlcPVBARPIDV0_H
