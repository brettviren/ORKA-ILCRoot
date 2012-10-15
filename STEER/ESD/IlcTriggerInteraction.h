#ifndef ILCTRIGGERINTERACTION_H
#define ILCTRIGGERINTERACTION_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// This class represents the CTP interaction objects                         //
//                                                                           //
// The Interaction consists of Name and Logical function of 4 CTP inputs.    //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TNamed.h>


class IlcTriggerInteraction : public TNamed {

public:
                          IlcTriggerInteraction();
                          IlcTriggerInteraction( TString & name, TString & logic );
                          IlcTriggerInteraction( const IlcTriggerInteraction& interact );
               virtual   ~IlcTriggerInteraction();
  IlcTriggerInteraction&   operator=(const IlcTriggerInteraction& interact);

                  Bool_t  CheckInputs(const TObjArray &inputs) const;
                  Bool_t  IsActive(const TObjArray &inputs) const;
		  Bool_t  Trigger(const TObjArray& inputs ) const;
           virtual void   Print( const Option_t* opt ="" ) const;
private:

  ClassDef( IlcTriggerInteraction, 1 )  // Define a trigger interaction object
};

#endif
