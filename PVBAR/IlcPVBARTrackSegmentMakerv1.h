#ifndef ILCPVBARTRACKSEGMENTMAKERV1_H
#define ILCPVBARTRACKSEGMENTMAKERV1_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARTrackSegmentMakerv1.h 35594 2009-10-15 15:40:35Z prsnko $ */
/* History of cvs commits:
 *
 * $Log$
 * Revision 1.51  2007/03/28 19:18:15  kharlov
 * RecPoints recalculation in TSM removed
 *
 * Revision 1.50  2007/03/06 06:54:48  kharlov
 * DP:Calculation of cluster properties dep. on vertex added
 *
 * Revision 1.49  2007/02/01 13:59:11  hristov
 * Forward declaration
 *
 * Revision 1.48  2006/08/28 10:01:56  kharlov
 * Effective C++ warnings fixed (Timur Pocheptsov)
 *
 * Revision 1.47  2005/11/17 12:35:27  hristov
 * Use references instead of objects. Avoid to create objects when they are not really needed
 *
 * Revision 1.46  2005/05/28 14:19:05  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
// Implementation version 1 of algorithm class to construct PVBAR track segments
// Associates EMC and CPV lusters
// Unfolds the EMC cluster   
//                  
//*-- Author: Dmitri Peressounko (RRC Ki & SUBATECH)

// --- ROOT system ---
#include <TVector3.h>

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcPVBARTrackSegmentMaker.h"

class IlcPVBAREmcRecPoint ;
class IlcPVBARCpvRecPoint ;
class TClonesArray;

class  IlcPVBARTrackSegmentMakerv1 : public IlcPVBARTrackSegmentMaker {

public:

  IlcPVBARTrackSegmentMakerv1() ;                     
  IlcPVBARTrackSegmentMakerv1(IlcPVBARGeometry *geom);
  IlcPVBARTrackSegmentMakerv1(const IlcPVBARTrackSegmentMakerv1 & tsm);

  virtual ~ IlcPVBARTrackSegmentMakerv1() ; // dtor
  
  virtual void   Clusters2TrackSegments(Option_t *option); // Does the job
          void   FillOneModule() ;       // Finds range in which RecPoints belonging current PVBAR module are

          void   MakeLinks() const;      //Evaluates distances(links) between EMC and CPV
          void   MakePairs() ;           //Finds pairs(triplets) with smallest link
  virtual void   Print(const Option_t * = "") const ;
  //Switch to "on flyght" mode, without writing to TreeR and file  
  void SetWriting(Bool_t toWrite = kFALSE){fWrite = toWrite;} 
  virtual void   SetMaxEmcCPVDistance(Float_t r){ fRcpv = r ;} //Maximal distance (in PVBAR plane) 
                                                               //between EMCrp and CPVrp
  virtual void   SetMaxCPVTPCDistance(Float_t r){ fRtpc = r ;} //Maximal distance 
                                                               //between EMCrp and extrapolation of TPC track
  virtual const char * Version() const { return "tsm-v1" ; }  

  IlcPVBARTrackSegmentMakerv1 & operator = (const IlcPVBARTrackSegmentMakerv1 & )  {
    // assignement operator requested by coding convention but not needed
    Fatal("operator =", "not implemented") ;
    return *this ; 
  }

  virtual TClonesArray * GetTrackSegments() const { return fTrackSegments; }

private:

  void  GetDistanceInPVBARPlane(IlcPVBAREmcRecPoint * EmcClu , IlcPVBARCpvRecPoint * Cpv, 
                               Int_t & track, Float_t &dx, Float_t &dz ) const ; // see R0
  void    Init() ;
  void    InitParameters() ;
  void    PrintTrackSegments(Option_t *option) ;
 

private:  

  Bool_t  fDefaultInit;               //! Says if the task was created by defaut ctor (only parameters are initialized)
  Bool_t  fWrite ;                   // Write Tracks to TreeT  
 
  Int_t fNTrackSegments ; // number of track segments found 

  Float_t fRcpv ;        // Maximum distance between a EMC RecPoint and a CPV RecPoint   
  Float_t fRtpc ;        // Maximum distance between a EMC RecPoint and extrapolation of a TPC track   
  
  TVector3 fVtx ;        //! Vertex in current position

  TClonesArray * fLinkUpArray  ;  //!
  Int_t fEmcFirst;     //! Index of first EMC RecPoint belonging to currect PVBAR module
  Int_t fEmcLast ;     //!
  Int_t fCpvFirst;     //! Cpv upper layer     
  Int_t fCpvLast;      //! 
  Int_t fModule ;      //! number of module being processed

  TClonesArray * fTrackSegments; // Array with found track-segments

  ClassDef( IlcPVBARTrackSegmentMakerv1,4)  // Implementation version 1 of algorithm class to make PVBAR track segments 

 };

#endif // IlcPVBARTRACKSEGMENTMAKERV1_H
