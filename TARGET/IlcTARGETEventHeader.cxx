#include "IlcTARGETEventHeader.h"
////////////////////////////////////////////////////
//  Base class to define                          //
//  TARGET Event Header                              //
//                                                //
//  Origin: E. Crescio crescio@to.infn.it         //
//                                                //
////////////////////////////////////////////////////

ClassImp(IlcTARGETEventHeader)

  
//_____________________________________________________________
IlcTARGETEventHeader::IlcTARGETEventHeader():IlcDetectorEventHeader()
{
  //
  // Defaulst Constructor
  //
  
  SetEventTypeSDD();
  
  for(Int_t idet=0;idet<3;idet++){
    fL1TriggerType[idet]=0;
    fOrbitNumber[idet]=0;
    fBunchCross[idet]=0;
    fBlockAttr[idet]=0;
    fTriggerClass[idet]=0;
    fStatusBits[idet]=0;
    fMiniEvId[idet]=0;
    fSubDet[idet]=0;
    fVersion[idet]=0;
    fJitterSDD=0;
  }
  
  
}

//_____________________________________________________________
IlcTARGETEventHeader::IlcTARGETEventHeader(const char* name):IlcDetectorEventHeader(name)
{
  //
  // Constructor
  //
  SetEventTypeSDD();
  for(Int_t idet=0;idet<3;idet++){
    fL1TriggerType[idet]=0;
    fOrbitNumber[idet]=0;
    fBunchCross[idet]=0;
    fBlockAttr[idet]=0;
    fTriggerClass[idet]=0;
    fStatusBits[idet]=0;
    fMiniEvId[idet]=0;
    fSubDet[idet]=0;
    fVersion[idet]=0;
    fJitterSDD=-123;
  }
  

}





