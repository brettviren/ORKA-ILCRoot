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


//_________________________________________________________________________
// Geometry class  for RSTACK : singleton  
//                  

// --- ROOT system ---

#include "TVector3.h"
#include "TRotation.h" 
#include "TParticle.h"
#include <TGeoManager.h>
#include <TGeoMatrix.h>

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcLog.h"
#include "IlcRSTACKGeometry.h"

ClassImp(IlcRSTACKGeometry)

// these initialisations are needed for a singleton
IlcRSTACKGeometry  * IlcRSTACKGeometry::fgGeom = 0 ;
Bool_t             IlcRSTACKGeometry::fgInit = kFALSE ;

//____________________________________________________________________________
IlcRSTACKGeometry::IlcRSTACKGeometry(): TNamed(),
	            fAngle(0.f),
		    fRSTACKRmin(0),
		    fRSTACKRmax(0),
		    fRSTACKLength(0),
		    fRSTACKTileScintThickness(0),
		    fRSTACKNSectorPhi(0)
{
    // default ctor 
    // must be kept public for root persistency purposes, but should never be called by the outside world
    fgGeom          = 0 ;

}  

//____________________________________________________________________________
IlcRSTACKGeometry::IlcRSTACKGeometry(const IlcRSTACKGeometry & rhs): TNamed(rhs),
		      fAngle(rhs.fAngle),
		      fRSTACKRmin(rhs.fRSTACKRmin),
		      fRSTACKRmax(rhs.fRSTACKRmax),
		      fRSTACKLength(rhs.fRSTACKLength),
		      fRSTACKTileScintThickness(rhs.fRSTACKTileScintThickness),
		      fRSTACKNSectorPhi(rhs.fRSTACKNSectorPhi)
{
  Fatal("cpy ctor", "not implemented") ; 
}

//____________________________________________________________________________
IlcRSTACKGeometry::IlcRSTACKGeometry(const Text_t* name, const Text_t* title)
: TNamed(name, title),
	            fAngle(0.f),
		    fRSTACKRmin(0),
		    fRSTACKRmax(0),
		    fRSTACKLength(0),
		    fRSTACKTileScintThickness(0),
		    fRSTACKNSectorPhi(0)
{ 
  // ctor only for internal usage (singleton)
  Init() ; 
  fgGeom = this;
}

//____________________________________________________________________________
IlcRSTACKGeometry::~IlcRSTACKGeometry(void)
{
  // dtor

}

//____________________________________________________________________________
void IlcRSTACKGeometry::Init(void)
{
  // Initializes the RSTACK parameters :
  
  fgInit     = kTRUE ; 

  fRSTACKRmin = 45.;
  fRSTACKRmax = 75.;
  fRSTACKLength = 185.;
  fRSTACKTileScintThickness = 1.;
  fRSTACKNSectorPhi = 48;
  fAngle = 360./fRSTACKNSectorPhi;

}

//____________________________________________________________________________
IlcRSTACKGeometry *  IlcRSTACKGeometry::GetInstance() 
{ 
  // Returns the pointer of the unique instance; singleton specific
  
  return static_cast<IlcRSTACKGeometry *>( fgGeom ) ; 
}

//____________________________________________________________________________
IlcRSTACKGeometry *  IlcRSTACKGeometry::GetInstance(const Text_t* name, const Text_t* title) 
{
  // Returns the pointer of the unique instance
  // Creates it with the specified options (name, title) if it does not exist yet

  IlcRSTACKGeometry * rv = 0  ; 
  if ( fgGeom == 0 ) {
    if ( strcmp(name,"") == 0 ) 
      rv = 0 ;
    else {    
      fgGeom = new IlcRSTACKGeometry(name, title) ;
      if ( fgInit )
	rv = (IlcRSTACKGeometry * ) fgGeom ;
      else {
	rv = 0 ; 
	delete fgGeom ; 
	fgGeom = 0 ; 
      }
    }
  }
  else {
    if ( strcmp(fgGeom->GetName(), name) != 0 ) 
      ::Error("GetInstance", "Current geometry is %s. You cannot call %s", 
		      fgGeom->GetName(), name) ; 
    else
      rv = (IlcRSTACKGeometry *) fgGeom ; 
  } 
  return rv ; 
}

Bool_t IlcRSTACKGeometry::AbsToRelNumbering(Int_t absId, Int_t * relid) const
{
  // Converts the absolute numbering into the following array
  //  relid[0] = RSTACK sector number 1:fRSTACKNSectorPhi
  //  relid[1] = tile number
  //  relid[2] = not used
  //  relid[3] = not used

  Float_t id = absId ;

  Int_t RSTACKSectNumber = (Int_t)TMath:: Ceil( id / GetRSTACKNTilesInSector() ) ; 

  relid[0] = RSTACKSectNumber ;

  id -= ( RSTACKSectNumber - 1 ) * GetRSTACKNTilesInSector() ; 
  relid[1] = id ;

  relid[2] = 0;
  relid[3] = 0; 

  return kTRUE ; 
}

//____________________________________________________________________________
Bool_t IlcRSTACKGeometry::RelToAbsNumbering(const Int_t * relid, Int_t &  absId) const
{
  // Converts the relative numbering
  //  relid[0] = RSTACK sector number 1:fRSTACKNSectorPhi
  //  relid[1] = tile number  1:RSTACKNTilesInSector
  //  relid[2] = not used
  //  relid[3] = not used
  //  into the absolute numbering
  

    absId =
      ( relid[0] - 1 ) * GetRSTACKNTilesInSector()  // the offset of RSTACK sectors
      + ( relid[1] );                              // the offset along a sector
  
  return kTRUE ; 
}
