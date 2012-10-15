#ifndef ILCPVECPID_H
#define ILCPVECPID_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
                            
/* $Id: IlcPVECPID.h 44091 2010-10-05 09:46:25Z prsnko $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.41  2007/08/28 12:55:08  policheh
 * Loaders removed from the reconstruction code (C.Cheshkov)
 *
 * Revision 1.40  2007/08/07 14:12:03  kharlov
 * Quality assurance added (Yves Schutz)
 *
 * Revision 1.39  2007/07/11 13:43:30  hristov
 * New class IlcESDEvent, backward compatibility with the old IlcESD (Christian)
 *
 * Revision 1.38  2007/04/01 15:40:15  kharlov
 * Correction for actual vertex position implemented
 *
 * Revision 1.37  2006/08/29 11:41:19  kharlov
 * Missing implementation of ctors and = operator are added
 *
 * Revision 1.36  2006/08/25 16:00:53  kharlov
 * Compliance with Effective C++IlcPVECHit.cxx
 *
 * Revision 1.35  2005/05/28 14:19:04  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
//  Algorithm class for the identification of particles detected in PVEC        
//  base  class                             
//  of identified particles                
//*-- Author: Yves Schutz (SUBATECH)

// --- ROOT system ---
#include "TObject.h"
class TTree;

// --- Standard library ---

// --- IlcRoot header files ---
class IlcESDEvent ;
class IlcPVECGeometry ;
class IlcPVECClusterizer ;
class IlcPVECTrackSegmentMaker ;

class IlcPVECPID : public TObject {

 public:

  IlcPVECPID() ;          // ctor            
  IlcPVECPID (IlcPVECGeometry *geom);
  IlcPVECPID(const IlcPVECPID & pid) ;
  virtual ~IlcPVECPID() ; // dtor
  IlcPVECPID & operator = (const IlcPVECPID & /*rvalue*/)  {
    Fatal("operator =", "not implemented") ; return *this ; }

  virtual void TrackSegments2RecParticles(Option_t * option) = 0;

  void SetInput(TTree *clustersTree, TClonesArray *trackSegments);
  TClonesArray* GetRecParticles() const { return fRecParticles; }

  virtual void Print(const Option_t * = "") const = 0;

  void SetESD(IlcESDEvent *esd) { fESD = esd; }

  virtual const char * Version() const = 0;

protected:

  IlcPVECGeometry * fGeom;    //! Pointer to PVEC Geometry
  IlcESDEvent * fESD;         //! ESD object

  TObjArray *fEMCRecPoints;      //!Array with EMC clusters
  TObjArray *fCPVRecPoints;      //!Array with CPV clusters

  TClonesArray *fTrackSegments;     //!Array with found track segments
  TClonesArray *fRecParticles;      //!Array with reconstructed particles (PID)
  
private: 

  ClassDef(IlcPVECPID,8)  // Particle Identifier algorithm (base class)

} ;

#endif // ILCPVECPID_H
