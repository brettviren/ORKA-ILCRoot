////////////////////////////////////////////////
//  Segmentation class for set:TARGET            //
//  All methods implemented in the derived    //
//  classes are set = 0 in the header file    //
//  so this class cannot be instantiated      //
//  methods implemented in a part of the      //
// derived classes are implemented here as    //
// TObject::MayNotUse                         // 
////////////////////////////////////////////////

#include <TF1.h>
#include "IlcRun.h"
#include "IlcTARGETsegmentation2.h"
#include "IlcTARGETgeom.h"
#include "IlcTARGET.h"
#include "IlcLog.h"

ClassImp(IlcTARGETsegmentation2)

//_____________________________________________________________
IlcTARGETsegmentation2::IlcTARGETsegmentation2(){
  // Default constructor


IlcTARGET *ilcTARGET  = (IlcTARGET*)gIlc->GetModule("TARGET");
IlcTARGETgeom *gm = ilcTARGET->GetTARGETgeom2();
//if (gm->GetDetVersion() !=3 ) cout <<"in IlcTARGETsegmentation2: Minor Version: "<<gm->GetDetVersion()<<endl;
  fParam=new IlcTARGETParam(ilcTARGET->GetMinorVersion());

//   if (gm->GetDetVersion() == 3) fParam=new IlcTARGETParam(gm->GetDetVersion());
//   else if (gm->GetDetVersion() == 2) fParam=new IlcTARGETParam(gm->GetDetVersion());
//   else fParam=new IlcTARGETParam(gm->GetDetVersion());


  fDx.Set(fParam->GetNlayers()+fParam->GetNendcaps()+fParam->GetNlayersStrip()+fParam->GetNendcapsStrip());
  fDy.Set(fParam->GetNlayers()+fParam->GetNendcaps()+fParam->GetNlayersStrip()+fParam->GetNendcapsStrip());
  fDz.Set(fParam->GetNlayers()+fParam->GetNendcaps()+fParam->GetNlayersStrip()+fParam->GetNendcapsStrip());

  fGeom2 = 0;
  fCorr = 0;

}

//_____________________________________________________________
IlcTARGETsegmentation2::~IlcTARGETsegmentation2(){
  // destructor
  if(fCorr)delete fCorr;
  delete fParam;
}

//_____________________________________________________________
void IlcTARGETsegmentation2::Copy(TObject &obj) const {
  // copy this to obj
  ((IlcTARGETsegmentation2& ) obj).fDz      = fDz;
  ((IlcTARGETsegmentation2& ) obj).fDx      = fDx;
  ((IlcTARGETsegmentation2& ) obj).fDy      = fDy;
  ((IlcTARGETsegmentation2& ) obj).fGeom2    = fGeom2; // copy only the pointer
  if(fCorr){
    ((IlcTARGETsegmentation2& ) obj).fCorr    = new TF1(*fCorr); // make a proper copy
  }
  else {
    ((IlcTARGETsegmentation2& ) obj).fCorr = 0;
  }
}
//______________________________________________________________________
IlcTARGETsegmentation2& IlcTARGETsegmentation2::operator=(
                        const IlcTARGETsegmentation2 &source){
// Operator =
  if(this != &source){
    source.Copy(*this);
  }
  return *this;
}
//______________________________________________________________________
IlcTARGETsegmentation2::IlcTARGETsegmentation2(const IlcTARGETsegmentation2 &source):
    TObject(source){
    // copy constructor
  source.Copy(*this);
}
