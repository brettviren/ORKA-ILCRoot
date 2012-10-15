#ifndef ILCDCHTRIGGERL1_H
#define ILCDCHTRIGGERL1_H

///////////////////////////////////////////////////////
//                                                   //
//  DCH trigger for L1                               //
//                                                   //
///////////////////////////////////////////////////////

#include "IlcTriggerDetector.h"

class IlcDCHTriggerL1 : public IlcTriggerDetector
{

 public:

  IlcDCHTriggerL1();  // constructor
  virtual ~IlcDCHTriggerL1(){}  // destructor
  virtual void    CreateInputs();
  virtual void    Trigger();

  ClassDef(IlcDCHTriggerL1,1)  // DCH Trigger Detector class

};

#endif
