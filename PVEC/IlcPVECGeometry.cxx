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

/* $Id: IlcPVECGeometry.cxx 46104 2010-12-16 18:00:01Z policheh $ */

//_________________________________________________________________________
// Geometry class  for PVEC : singleton  
// PVEC consists of the electromagnetic calorimeter (EMCA)
// and a charged particle veto either in the Subatech's version (PPSD)
// or in the IHEP's one (CPV).
// The EMCA/PPSD/CPV modules are parametrized so that any configuration
// can be easily implemented 
// The title is used to identify the version of CPV used.
//                  
// -- Author: Yves Schutz (SUBATECH) & Dmitri Peressounko (RRC "KI" & SUBATECH)

// --- ROOT system ---

#include "TVector3.h"
#include "TRotation.h" 
#include "TParticle.h"
#include <TGeoManager.h>
#include <TGeoMatrix.h>

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcLog.h"
#include "IlcPVECGeometry.h"

ClassImp(IlcPVECGeometry)

// these initialisations are needed for a singleton
IlcPVECGeometry  * IlcPVECGeometry::fgGeom = 0 ;
Bool_t             IlcPVECGeometry::fgInit = kFALSE ;

//____________________________________________________________________________
IlcPVECGeometry::IlcPVECGeometry(): TNamed(),
	            fAngle(0.f),
	            fPVECRmin(0),
		    fPVECRmax(0),
		    fPVECZPosition(0),
		    fPVECLength(0),
		    fPVECNSlicePhi(0),
		    fPVECNLayers(0),
		    fPVECNXtalPhi(0),
		    fPVECLayerThickness(0),
		    fPVECXTalEnvelopThickness(0)
{
    // default ctor 
    // must be kept public for root persistency purposes, but should never be called by the outside world
    fgGeom          = 0 ;

}  

//____________________________________________________________________________
IlcPVECGeometry::IlcPVECGeometry(const IlcPVECGeometry & rhs): TNamed(rhs),
		      fAngle(rhs.fAngle),
		      fPVECRmin(rhs.fPVECRmin),
		      fPVECRmax(rhs.fPVECRmax),
		      fPVECZPosition(rhs.fPVECZPosition),
		      fPVECLength(rhs.fPVECLength),
		      fPVECNSlicePhi(rhs.fPVECNSlicePhi),
		      fPVECNLayers(rhs.fPVECNLayers),
		      fPVECNXtalPhi(rhs.fPVECNXtalPhi),
		      fPVECLayerThickness(rhs.fPVECLayerThickness),
		      fPVECXTalEnvelopThickness(rhs.fPVECXTalEnvelopThickness)
{
  Fatal("cpy ctor", "not implemented") ; 
}

//____________________________________________________________________________
IlcPVECGeometry::IlcPVECGeometry(const Text_t* name, const Text_t* title) 
: TNamed(name, title),
	            fAngle(0.f),
	            fPVECRmin(0),
	            fPVECRmax(0),
	            fPVECZPosition(0),
	            fPVECLength(0),
	            fPVECNSlicePhi(0),
	            fPVECNLayers(0),
	            fPVECNXtalPhi(0),
	            fPVECLayerThickness(0),
	            fPVECXTalEnvelopThickness(0)
{ 
  // ctor only for internal usage (singleton)
  Init() ; 
  fgGeom = this;
}

//____________________________________________________________________________
IlcPVECGeometry::~IlcPVECGeometry(void)
{
  // dtor

  if (fPVECNXtalPhi) delete[]fPVECNXtalPhi;
  if (fPVECLayerThickness) delete []fPVECLayerThickness;
}

//____________________________________________________________________________
void IlcPVECGeometry::Init(void)
{
  // Initializes the PVEC parameters :
  //  IHEP is the Protvino CPV (cathode pad chambers)
  
  fgInit     = kTRUE ; 

  fPVECRmin = 10.;
  fPVECRmax = 43.4;
  fPVECZPosition = 50.;
  fPVECLength = 25.;
  fPVECNSlicePhi = 36;
  fPVECNLayers = 4;
  
  fPVECNXtalPhi = new Int_t [fPVECNLayers];
  fPVECNXtalPhi[0] = 13;
  fPVECNXtalPhi[1] = 14;
  fPVECNXtalPhi[2] = 21;
  fPVECNXtalPhi[3] = 27;

  fPVECLayerThickness = new Double_t [fPVECNLayers];
  fPVECLayerThickness[0] = 6.12;
  fPVECLayerThickness[1] = 8.56;
  fPVECLayerThickness[2] = 8.56;
  fPVECLayerThickness[3] = 8.56;

  fPVECXTalEnvelopThickness = 0.2;

}

//____________________________________________________________________________
IlcPVECGeometry *  IlcPVECGeometry::GetInstance() 
{ 
  // Returns the pointer of the unique instance; singleton specific
  
  return static_cast<IlcPVECGeometry *>( fgGeom ) ; 
}

//____________________________________________________________________________
IlcPVECGeometry *  IlcPVECGeometry::GetInstance(const Text_t* name, const Text_t* title) 
{
  // Returns the pointer of the unique instance
  // Creates it with the specified options (name, title) if it does not exist yet

  IlcPVECGeometry * rv = 0  ; 
  if ( fgGeom == 0 ) {
    if ( strcmp(name,"") == 0 ) 
      rv = 0 ;
    else {    
      fgGeom = new IlcPVECGeometry(name, title) ;
      if ( fgInit )
	rv = (IlcPVECGeometry * ) fgGeom ;
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
      rv = (IlcPVECGeometry *) fgGeom ; 
  } 
  return rv ; 
}

//____________________________________________________________________________
Bool_t IlcPVECGeometry::AbsToRelNumbering(Int_t absId, Int_t * relid) const
{
  // Converts the absolute Cell Id number into the following array
  //  relid[0] = PVEC Module number 1 for left endcap, 2 for right endcap
  //  relid[1] = ring number in PVEC module
  //  relid[2] = Xtal number inside PVEC  ring
  //  relid[3] = 0 not used
  
  relid[3] = 0;

  Bool_t rv  = kTRUE ;

  const Int_t nLay = fPVECNLayers;
  Int_t NXtalPhi[nLay];
  Int_t NXtalInModule=0;
  for(Int_t iLay=0; iLay<nLay; iLay++){
    NXtalPhi[iLay] = fPVECNXtalPhi[iLay];
    NXtalInModule += NXtalPhi[iLay];
  }

  if(absId>2*NXtalInModule){
      rv = kFALSE;
      return rv;
  }
  
  if(absId<=NXtalInModule)
      relid[0] = 1;
  else{
      relid[0] = 2;
      absId -= NXtalInModule;
  }

  relid[1] = 1;
  for(Int_t iLay=0; iLay<nLay; iLay++){
      if(absId>NXtalPhi[iLay]){
          relid[1]++;
          absId -= NXtalPhi[iLay];
      }
      else
          break;
  
  }
 
  relid[2] = absId;

 return rv ;

}

//____________________________________________________________________________
Bool_t IlcPVECGeometry::RelToAbsNumbering(const Int_t * relid, Int_t &  absId) const
{
  // Converts the relative Cell Id number into the absolute cell number
  //  relid[0] = PVEC Module number 1 for left endcap, 2 for right endcap
  //  relid[1] = ring number in PVEC module
  //  relid[2] = Xtal number inside PVEC  ring
  //  relid[3] = 0 not used

  Bool_t rv = kTRUE ;

  absId = 0;
  
  const Int_t nLay = fPVECNLayers;
  Int_t NXtalPhi[nLay];
  Int_t NXtalInModule=0;
  for(Int_t iLay=0; iLay<nLay; iLay++){
    NXtalPhi[iLay] = fPVECNXtalPhi[iLay];
    NXtalInModule += NXtalPhi[iLay];
  }

  if(relid[0] == 2) absId += NXtalInModule;
//   IlcInfo(Form("relid[0]: %d AbsId:%d   NXtalInModule: %d", relid[0], absId, NXtalInModule));
  for(Int_t iLay=0; iLay<relid[1]-1; iLay++){
    absId += NXtalPhi[iLay];
//     IlcInfo(Form("relid[1]: %d AbsId:%d   NXtalPhi[iLay]: %d", relid[1], absId, NXtalPhi[iLay]));
  }

  absId += relid[2];
//   IlcInfo(Form("relid[2]: %d AbsId:%d ", relid[2], absId));
  
  return rv;  
}
