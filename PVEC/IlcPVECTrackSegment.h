#ifndef ILCPVECTRACKSEGMENT_H
#define ILCPVECTRACKSEGMENT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVECTrackSegment.h 53186 2011-11-25 14:28:01Z kharlov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.23  2006/08/28 10:01:56  kharlov
 * Effective C++ warnings fixed (Timur Pocheptsov)
 *
 * Revision 1.22  2005/05/28 14:19:05  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
//  Track segment in PVEC
//  Can be : 1 EmcRecPoint
//           1 EmcRecPoint + 1 CPV
//                  
//*-- Author:  Dmitri Peressounko (RRC KI & SUBATECH)

// --- ROOT system ---

#include "TObject.h"
class TClonesArray ; 

// --- Standard library ---

// --- IlcRoot header files ---
class IlcPVECRecPoint ; 
class IlcPVECEmcRecPoint ; 
class IlcPVECCpvRecPoint ; 
class IlcESDtrack ; 

class IlcPVECTrackSegment : public TObject  {

public:

  IlcPVECTrackSegment() ;
  IlcPVECTrackSegment(IlcPVECEmcRecPoint * EmcRecPoint , 
		      IlcPVECRecPoint * Cpv) ;
  IlcPVECTrackSegment(IlcPVECEmcRecPoint * EmcRecPoint , 
		      IlcPVECRecPoint * Cpv, Int_t track) ;
  IlcPVECTrackSegment(IlcPVECEmcRecPoint * EmcRecPoint , 
		      IlcPVECRecPoint * Cpv, Int_t track, 
                      Float_t dx, Float_t dz) ;
  IlcPVECTrackSegment(const IlcPVECTrackSegment & ts) ;  // ctor                   
  virtual ~IlcPVECTrackSegment() {  } 

  void    SetCpvDistance(Float_t x,Float_t z){fDcpv[0]=x ; fDcpv[1]=z ; }

  void Copy(TObject & obj) const;  

  Int_t   GetIndexInList() const {  return fIndexInList ;   } 
  Int_t   GetEmcIndex()    const {  return fEmcRecPoint ;   }
  Int_t   GetCpvIndex()    const {  return fCpvRecPoint; }
  Int_t   GetTrackIndex()  const {  return fTrack; }
  Float_t GetCpvDistance(const Option_t* dr="r") const ;

  virtual void  Print(const Option_t * = "") const;
  void    SetIndexInList(Int_t val){ fIndexInList = val ;     } 
  void    SetCpvRecPoint(IlcPVECRecPoint * CpvRecPoint ); //sets CPV Rec Point

  typedef TClonesArray TrackSegmentsList ; 
 
private:
  IlcPVECTrackSegment & operator = (const IlcPVECTrackSegment & /*ts*/);
 private:
  
  Int_t fEmcRecPoint ;     // The EMC reconstructed point index in array stored in TreeR/PVECEmcRP
  Int_t fIndexInList ;     // the index of this TrackSegment in the list stored in TreeR (to be set by analysis)
  Int_t fCpvRecPoint ;     // The CPV reconstructed point in array stored in TreeR/PVECCpvRP
  Int_t fTrack ;           // The charged track index (from global tracking) in ESD file 
  Float_t fDcpv[2] ;       // Distance to projection of CPV cluster

 ClassDef(IlcPVECTrackSegment,1)  // Track segment in PVEC

};

#endif // ILCPVECTRACKSEGMENT_H
