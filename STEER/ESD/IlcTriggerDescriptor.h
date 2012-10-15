#ifndef ILCTRIGGERDESCRIPTOR_H
#define ILCTRIGGERDESCRIPTOR_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// This class represents the CTP descriptor objects                          //
//                                                                           //
// The Descriptor consists of Name and 1 or Logical function of 4 CTP inputs+//
// additional inputs (added with an AND operator)                            // 
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TNamed.h>


class IlcTriggerDescriptor : public TNamed {

public:
                          IlcTriggerDescriptor();
                          IlcTriggerDescriptor( TString & name, TString & cond );
                          IlcTriggerDescriptor( const IlcTriggerDescriptor& desc );
               virtual   ~IlcTriggerDescriptor();
  IlcTriggerDescriptor&   operator=(const IlcTriggerDescriptor& desc);

                  Bool_t  CheckInputsAndFunctions(const TObjArray &inputs,const TObjArray &functions) const;
                  Bool_t  IsActive(const TObjArray &inputs,const TObjArray &functions) const;
		  Bool_t  Trigger( const TObjArray &inputs, const TObjArray &functions) const;
           virtual void   Print( const Option_t* opt ="" ) const;
private:

  ClassDef( IlcTriggerDescriptor, 1 )  // Define a trigger descriptor object
};

#endif
