/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/
/* $Id: IlcRSTACKTrackSegment.cxx 53186 2011-11-25 14:28:01Z kharlov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.30  2006/08/28 10:01:56  kharlov
 * Effective C++ warnings fixed (Timur Pocheptsov)
 *
 * Revision 1.29  2005/05/28 14:19:05  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
//  Track segment in RSTACK
//  Can be : 1 EmcRecPoint
//           1 EmcRecPoint + 1 CPV
//           1 EmcRecPoint + 1 CPV + 1 charged track
//                  
//*-- Author:  Dmitri Peressounko (RRC KI & SUBATECH)

// --- ROOT system ---
 

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcRSTACKEmcRecPoint.h" 
#include "IlcRSTACKTrackSegment.h" 
#include "IlcESDtrack.h" 
#include "IlcLog.h" 

ClassImp(IlcRSTACKTrackSegment)

//____________________________________________________________________________
IlcRSTACKTrackSegment::IlcRSTACKTrackSegment()
			: fEmcRecPoint(0),
			  fIndexInList(0),
			  fCpvRecPoint(0),
			  fTrack(0)
{
  //def ctor
  fDcpv[0]=999. ;
  fDcpv[1]=999. ;
}

//____________________________________________________________________________
IlcRSTACKTrackSegment::IlcRSTACKTrackSegment( IlcRSTACKEmcRecPoint * emc , 
					  IlcRSTACKRecPoint * cpvrp1)
			: fEmcRecPoint(0),
			  fIndexInList(0),
			  fCpvRecPoint(0),
			  fTrack(0)
{
  // ctor

  if( emc )   
    fEmcRecPoint =  emc->GetIndexInList() ;
  else 
    fEmcRecPoint = -1 ;

  if( cpvrp1 )  
    fCpvRecPoint = cpvrp1->GetIndexInList() ;
 else 
    fCpvRecPoint = -1 ;

  fTrack = -1 ; 

  fIndexInList = -1 ;
  fDcpv[0]=999. ;
  fDcpv[1]=999. ;
}

//____________________________________________________________________________
IlcRSTACKTrackSegment::IlcRSTACKTrackSegment( IlcRSTACKEmcRecPoint * emc , 
					  IlcRSTACKRecPoint * cpvrp1, 
					  Int_t track)
			: fEmcRecPoint(0),
			  fIndexInList(0),
			  fCpvRecPoint(0),
			  fTrack(0)
{
  // ctor

  if( emc )   
    fEmcRecPoint =  emc->GetIndexInList() ;
  else 
    fEmcRecPoint = -1 ;

  if( cpvrp1 )  
    fCpvRecPoint = cpvrp1->GetIndexInList() ;
 else 
    fCpvRecPoint = -1 ;
  
  fTrack = track ; 

  fIndexInList = -1 ;
  fDcpv[0]=999. ;
  fDcpv[1]=999. ;
}

//____________________________________________________________________________
IlcRSTACKTrackSegment::IlcRSTACKTrackSegment( IlcRSTACKEmcRecPoint * emc , 
					  IlcRSTACKRecPoint * cpvrp1, 
					  Int_t track, Float_t dx,Float_t dz)
			: fEmcRecPoint(0),
			  fIndexInList(0),
			  fCpvRecPoint(0),
			  fTrack(0)
{
  // ctor

  if( emc )   
    fEmcRecPoint =  emc->GetIndexInList() ;
  else 
    fEmcRecPoint = -1 ;

  if( cpvrp1 )  
    fCpvRecPoint = cpvrp1->GetIndexInList() ;
 else 
    fCpvRecPoint = -1 ;
  
  fTrack = track ; 

  fIndexInList = -1 ;

  fDcpv[0] = dx ;
  fDcpv[1] = dz ;
}

//____________________________________________________________________________
IlcRSTACKTrackSegment::IlcRSTACKTrackSegment( const IlcRSTACKTrackSegment & ts) 
  : TObject(ts),
    fEmcRecPoint(0),
    fIndexInList(0),
    fCpvRecPoint(0),
    fTrack(0)
{
  // Copy ctor

  ( (IlcRSTACKTrackSegment &)ts ).Copy(*this) ; 
}


//____________________________________________________________________________
IlcRSTACKTrackSegment & IlcRSTACKTrackSegment::operator = (const IlcRSTACKTrackSegment &)
{
  Fatal("operator =", "not implemented");
  return *this;
}

//____________________________________________________________________________
void IlcRSTACKTrackSegment::Copy(TObject & obj) const
{
  // Copy of a track segment into another track segment

   TObject::Copy(obj) ;
   ( (IlcRSTACKTrackSegment &)obj ).fEmcRecPoint     = fEmcRecPoint ; 
   ( (IlcRSTACKTrackSegment &)obj ).fCpvRecPoint     = fCpvRecPoint ; 
   ( (IlcRSTACKTrackSegment &)obj ).fIndexInList     = fIndexInList ; 
   ( (IlcRSTACKTrackSegment &)obj ).fTrack           = fTrack ;
   ( (IlcRSTACKTrackSegment &)obj ).fDcpv[0]         = fDcpv[0] ;
   ( (IlcRSTACKTrackSegment &)obj ).fDcpv[1]         = fDcpv[1] ;
} 


//____________________________________________________________________________
void IlcRSTACKTrackSegment::Print(const Option_t *) const
{
  // Print all information on this track Segment
  

  IlcInfo("Track segment");
  printf("Stored at position %d\n", fIndexInList) ;
  printf(" Emc RecPoint #     %d\n", fEmcRecPoint) ;
  if(fCpvRecPoint >= 0)
    printf(" CPV RecPoint #     %d\n", fCpvRecPoint) ;
  else
    printf(" No CPV RecPoint\n");
  if (fTrack >= 0) 
    printf(" Charged track #     %d\n", fTrack) ;
  else
    printf(" No Charged track\n");
  printf(" Distance to CPV: x=%f, z=%f\n",fDcpv[0],fDcpv[1]) ;
}

//____________________________________________________________________________
void IlcRSTACKTrackSegment::SetCpvRecPoint(IlcRSTACKRecPoint * cpvRecPoint) 
{
  // gives an id from its position in the list
  if( cpvRecPoint )  
    fCpvRecPoint = cpvRecPoint->GetIndexInList() ;
 else 
    fCpvRecPoint = -1 ;
}
//____________________________________________________________________________
Float_t IlcRSTACKTrackSegment::GetCpvDistance(const Option_t* dr) const
{
 if(strcmp(dr,"x")==0||strcmp(dr,"X")==0) return fDcpv[0] ; 
 if(strcmp(dr,"z")==0||strcmp(dr,"Z")==0) return fDcpv[1] ; 
 if(strcmp(dr,"r")==0||strcmp(dr,"R")==0) return TMath::Sqrt(fDcpv[0]*fDcpv[0]+fDcpv[1]*fDcpv[1]) ; 
 return 999. ;

}
