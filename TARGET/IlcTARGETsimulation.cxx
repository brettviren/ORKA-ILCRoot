/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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
//////////////////////////////////////////////////////////////////////////////
// This is the base class for TARGET detector signal simulations. Data members //
// include are a pointer to the IlcTARGETDetTypeSim clas in order to access    //
// segmentation and response objects                                        // 
// classes. See the detector specific implementations for the propper code. //
//////////////////////////////////////////////////////////////////////////////
#include "TClonesArray.h"

#include "IlcTARGETsimulation.h"
#include "IlcTARGETpList.h"

ClassImp(IlcTARGETsimulation)

//______________________________________________________________________
IlcTARGETsimulation::IlcTARGETsimulation(): TObject(),
fpList(0),
fModule(0),
fEvent(0),
fDebug(0){
    // Default constructor
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //    a default constructed IlcTARGETsimulation class
  IlcTARGET *ilcTARGET  = (IlcTARGET*)gIlc->GetModule("TARGET");
  IlcTARGETgeom *gm = ilcTARGET->GetTARGETgeom2();
//if (gm->GetDetVersion() !=3 ) cout <<"in IlcTARGETsimulationSPD: Minor Version: "<<gm->GetDetVersion()<<endl;
  fParam=new IlcTARGETParam(ilcTARGET->GetMinorVersion());
//   if (gm->GetDetVersion() == 3) fParam=new IlcTARGETParam(gm->GetDetVersion());
//   else if (gm->GetDetVersion() == 2) fParam=new IlcTARGETParam(gm->GetDetVersion());
//   else fParam=new IlcTARGETParam(gm->GetDetVersion());

 fpList2=new TObjArray(100);
 fMapA2=new TObjArray(100);
 
  fDetType = 0;
}
//______________________________________________________________________
IlcTARGETsimulation::IlcTARGETsimulation(IlcTARGETDetTypeSim *dettyp): TObject(),
fpList(0),
fModule(0),
fEvent(0),
fDebug(0)
{
    // Default constructor
    // Inputs:
    //    IlcTARGETDetTypeSim * : object used to access segmentation and response
    // Outputs:
    //    none.
    // Return:
    //    a default constructed IlcTARGETsimulation class
  IlcTARGET *ilcTARGET  = (IlcTARGET*)gIlc->GetModule("TARGET");
  IlcTARGETgeom *gm = ilcTARGET->GetTARGETgeom2();
//if (gm->GetDetVersion() !=3 ) cout <<"in IlcTARGETsimulationSPD: Minor Version: "<<gm->GetDetVersion()<<endl;

  fParam=new IlcTARGETParam(ilcTARGET->GetMinorVersion());
//   if (gm->GetDetVersion() == 3) fParam=new IlcTARGETParam(gm->GetDetVersion());
//   else if (gm->GetDetVersion() == 2) fParam=new IlcTARGETParam(gm->GetDetVersion());
//   else fParam=new IlcTARGETParam(gm->GetDetVersion(gm->GetDetVersion()));

  fDetType = dettyp;
  fpList2=new TObjArray(100);
  fMapA2=new TObjArray(100);
}
//__________________________________________________________________________
IlcTARGETsimulation::~IlcTARGETsimulation(){
    // destructor
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //    none.

    if(fpList2){
      fpList2->Delete();
      fpList2 = 0;
    }
    if(fMapA2){
      fMapA2->Delete();
      fMapA2 = 0;
    }
    delete fParam;
   }
//__________________________________________________________________________
IlcTARGETsimulation::IlcTARGETsimulation(const IlcTARGETsimulation &s) : TObject(s){
    //     Copy Constructor
    // Inputs:
    //    const IlcTARGETsimulation &s  simulation class to copy from
    // Outputs:
    //    none.
    // Return:
    //    a standard constructed IlcTARGETsimulation class with values the same
    //    as that of s.
 
    *this = s;
    return;
}

//_________________________________________________________________________
IlcTARGETsimulation&  IlcTARGETsimulation::operator=(const IlcTARGETsimulation &s){
    //    Assignment operator
    // Inputs:
    //    const IlcTARGETsimulation &s  simulation class to copy from
    // Outputs:
    //    none.
    // Return:
    //    a standard constructed IlcTARGETsimulation class with values the same
    //    as that of s.

    if(&s == this) return *this;
    this->fModule       = s.fModule;
    this->fEvent        = s.fEvent;
    this->fpList        = s.fpList;
    this->fpList2        = s.fpList2;
    this->fMapA2        = s.fMapA2;
    return *this;
}
//______________________________________________________________________
Bool_t IlcTARGETsimulation::AddSDigitsToModule(TClonesArray *pItemA,Int_t mask ){
    // Add Summable digits to module maps.
    // Inputs:
    //    TClonesArray *pItemA  Array of IlcTARGETpListItems (SDigits).
    //    Int_t         mask    Track number off set value (see 
    //                          IlcTARGETpList::AddItemTo).
    // Outputs:
    //    none.
    // Return:
    //    kTRUE if there is a signal >0 else kFALSE
    Int_t lay=0,lad=0,det=0;
    Int_t nItems = pItemA->GetEntriesFast();

    Bool_t sig = kFALSE;
   // cout <<"nItems: "<<nItems<<endl;
    // cout << "Adding "<< nItems <<" SDigits to module " << fModule << endl;
    for( Int_t i=0; i<nItems; i++ ) {
      IlcTARGETpListItem * pItem = (IlcTARGETpListItem *)(pItemA->UncheckedAt( i ));
        if( pItem->GetModule() != fModule ) {
            Error( "AddSDigitsToModule","Error reading, SDigits module %d "
                   "!= current module %d: exit",
                   pItem->GetModule(), fModule );
            return sig;
        } // end if
        if(pItem->GetSignal()>0.0 ) sig = kTRUE;

        IlcDebug(1,Form("mask: %d",mask));
        fDetType->GetTARGETgeom2()->GetModuleId(fModule,lay,lad,det);
        ((IlcTARGETpList *)fpList2->UncheckedAt(lay-1))->AddItemTo( mask, pItem );
    } // end for i
    return sig;
}
