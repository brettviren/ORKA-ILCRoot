////////////////////////////////////////////////////
//  Class to define                               //
//  beam test raw 2 dig conv.                     //
//  Origin: E. Crescio crescio@to.infn.it         //
//                                                //
////////////////////////////////////////////////////

#include <stdlib.h>
#include "IlcTARGETBeamTestDig.h"

ClassImp(IlcTARGETBeamTestDig)



//_____________________________________________________________
  IlcTARGETBeamTestDig::IlcTARGETBeamTestDig(): TTask()
{
  //
  // Default constructor
  //
  fReader=0;
  fTreeD=0;
  fTARGETHeader=0;
  fTARGETgeom=0;
}

//_____________________________________________________________
  IlcTARGETBeamTestDig::IlcTARGETBeamTestDig(const Text_t* name, const Text_t* title): TTask(name,title)
{
  //
  // Standard constructor
  //

  fReader=0;
  fTreeD=0;
  fTARGETHeader=0;
  fTARGETgeom = 0;
}

//______________________________________________________________________
IlcTARGETBeamTestDig::IlcTARGETBeamTestDig(const IlcTARGETBeamTestDig &bt):TTask(bt){
    // Copy constructor. 

  fReader=bt.fReader;
  fTreeD=bt.fTreeD;
  fTARGETHeader=bt.fTARGETHeader;
  fTARGETgeom = bt.fTARGETgeom;
}
//______________________________________________________________________
IlcTARGETBeamTestDig& IlcTARGETBeamTestDig::operator=(const IlcTARGETBeamTestDig &source){
    // Assignment operator. This is a function which is not allowed to be
    // done to the TARGET beam test dig. It exits with an error.
    // Inputs:
    if(this==&source) return *this;
    Error("operator=","You are not allowed to make a copy of the IlcTARGETBeamTestDig");
    exit(1);
    return *this; //fake return
}


  
 
