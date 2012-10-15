#ifndef ILCTARGETBEAMTESTDIG_H
#define ILCTARGETBEAMTESTDIG_H

////////////////////////////////////////////////////
//  Class to define                               //
//  beam test raw 2 dig conv.                     //
//  Origin: E. Crescio crescio@to.infn.it         //
//  November 2004                                 //
////////////////////////////////////////////////////

#include "TTask.h"


class IlcRawReader;
class TTree;
class IlcTARGETEventHeader;
class IlcTARGETgeom;

class IlcTARGETBeamTestDig: public TTask {
 
 public:

 
  IlcTARGETBeamTestDig();
  IlcTARGETBeamTestDig(const Text_t* name, const Text_t* title);
  IlcTARGETBeamTestDig(const IlcTARGETBeamTestDig& bt);
  IlcTARGETBeamTestDig& operator=(const IlcTARGETBeamTestDig &source);
  virtual ~IlcTARGETBeamTestDig() {}
 
  void SetRawReader(IlcRawReader* rd) {fReader=rd;}
  void SetTree(TTree* treedig) {fTreeD=treedig;}
  void SetTARGETEventHeader(IlcTARGETEventHeader* header){fTARGETHeader = header;}
  void SetTARGETgeom(IlcTARGETgeom* geom) {fTARGETgeom=geom;}  


 protected:      
  
  IlcTARGETEventHeader* fTARGETHeader;     // its event header
  IlcRawReader* fReader;             // !reader ;
  TTree* fTreeD;                     // tree of digits
  IlcTARGETgeom* fTARGETgeom;              // !TARGET geometry


  ClassDef(IlcTARGETBeamTestDig,4)   // its beam test digitization 

 };



#endif

    
