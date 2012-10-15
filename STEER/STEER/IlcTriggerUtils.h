#ifndef ILCTRIGGERUTILS_H
#define ILCTRIGGERUTILS_H

class IlcTriggerConfiguration;
class TString;

#include <TObject.h>

class IlcTriggerUtils: public TObject {

 public:

  IlcTriggerUtils() : TObject() {}
  virtual ~IlcTriggerUtils() {}
  
  Bool_t CheckConfiguration( TString & configfile, IlcTriggerConfiguration * cfg );


  ClassDef( IlcTriggerUtils, 0 )  // Trigger utilities

    };

#endif
