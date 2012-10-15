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
#include "IlcTARGETsegmentation.h"

ClassImp(IlcTARGETsegmentation)

//_____________________________________________________________
IlcTARGETsegmentation::IlcTARGETsegmentation(){
  // Default constructor
  SetDetSize(0.,0.,0.);
  fGeom = 0;
  fCorr = 0;
}

//_____________________________________________________________
IlcTARGETsegmentation::~IlcTARGETsegmentation(){
  // destructor
  if(fCorr)delete fCorr;
}

//_____________________________________________________________
void IlcTARGETsegmentation::Copy(TObject &obj) const {
  // copy this to obj
  ((IlcTARGETsegmentation& ) obj).fDz      = fDz;
  ((IlcTARGETsegmentation& ) obj).fDx      = fDx;
  ((IlcTARGETsegmentation& ) obj).fDy      = fDy;
  ((IlcTARGETsegmentation& ) obj).fGeom    = fGeom; // copy only the pointer
  if(fCorr){
    ((IlcTARGETsegmentation& ) obj).fCorr    = new TF1(*fCorr); // make a proper copy
  }
  else {
    ((IlcTARGETsegmentation& ) obj).fCorr = 0;
  }
}
//______________________________________________________________________
IlcTARGETsegmentation& IlcTARGETsegmentation::operator=(
                        const IlcTARGETsegmentation &source){
// Operator =
  if(this != &source){
    source.Copy(*this);
  }
  return *this;
}
//______________________________________________________________________
IlcTARGETsegmentation::IlcTARGETsegmentation(const IlcTARGETsegmentation &source):
    TObject(source){
    // copy constructor
  source.Copy(*this);
}
