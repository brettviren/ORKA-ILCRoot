#ifndef IlcPVECTracker_h
#define IlcPVECTracker_h
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVECTracker.h 35551 2009-10-14 22:41:56Z kharlov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.5  2007/08/07 14:12:03  kharlov
 * Quality assurance added (Yves Schutz)
 *
 * Revision 1.4  2007/08/03 13:52:16  kharlov
 * Working skeleton of matching the ESD tracks and ESD clusters (Iouri Belikov)
 *
 */

//-------------------------------------------------------------------------
//                          PVEC tracker.
// Matches ESD tracks with the PVEC and makes the PID.  
// 
//-------------------------------------------------------------------------

#include <IlcTracker.h>

class IlcRunLoader;   // Bad !

class TClonesArray;
class TTree;

class IlcCluster;
class IlcESDEvent;

class IlcPVECTracker : public IlcTracker
{
public:
  IlcPVECTracker();
  virtual ~IlcPVECTracker();
  
  Int_t LoadClusters(TTree *);
  Int_t PropagateBack(IlcESDEvent *);
  IlcCluster *GetCluster(Int_t idx) const;
  void UnloadClusters();

  Int_t Clusters2Tracks(IlcESDEvent *) {return 0;}
  Int_t RefitInward(IlcESDEvent *)     {return 0;}

  static void                SetDebug()   { fgDebug = kTRUE ; }
  static void                ResetDebug() { fgDebug = kFALSE ; }
  static Bool_t              Debug() { return fgDebug ; }

protected:
  IlcPVECTracker(const IlcPVECTracker & rhs): IlcTracker(rhs){}

private:
  IlcPVECTracker &operator=(const IlcPVECTracker &rhs);

  static Bool_t fgDebug ;    //! Verbosity controller

  TClonesArray *fModules[5];
  
  ClassDef(IlcPVECTracker,2)
};

#endif
