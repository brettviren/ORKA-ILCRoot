#ifndef ILCTARGETBEAMTESTDIGSSD_H
#define ILCTARGETBEAMTESTDIGSSD_H

////////////////////////////////////////////////////
//  Class to define                               //
//  SSD beam test raw 2 dig conv.                 //
//                                                //
//  Origin: Enrico Fragiacomo                     //
//                                                //
////////////////////////////////////////////////////

#include "IlcTARGETBeamTestDig.h"




class IlcTARGETBeamTestDigSSD: public IlcTARGETBeamTestDig {
 
 public:

 
  IlcTARGETBeamTestDigSSD();
  IlcTARGETBeamTestDigSSD(const Text_t* name, const Text_t* title);
  virtual ~IlcTARGETBeamTestDigSSD();

  virtual void Exec(Option_t* opt);
  
  //  void SetRawReaderDate(IlcRawReaderDate* rd) {fReaderDate=rd;}
  // void SetHeader(Bool_t H){fFlagHeader = H;}

  //void SetTree(TTree* treedig) {fTreeD=treedig;}
    
 protected:      
  
   

  Bool_t fFlagHeader;  //flag for the header


 ClassDef(IlcTARGETBeamTestDigSSD,1)  // An Ilc SSD beam test run

 };


#endif

    

