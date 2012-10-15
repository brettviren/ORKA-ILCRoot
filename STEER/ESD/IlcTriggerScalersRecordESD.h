#ifndef ILCTRIGGERSCALERSRECORDESD_H
#define ILCTRIGGERSCALERSRECORDESD_H

/* 2012-2013 Infrastructure for Large Collider Experiment, All rights reserved. *
* See cxx source for full Copyright notice */
/* $Id$ */

///////////////////////////////////////////////////////////////////////////////
//
// Class to define the ILC Trigger Scalers Record 
//
// Each record consists of 1 time stamp (4 words)  (IlcTimeStamp)
// and an array with the scalers (IlcTriggerScalers) for each trigger class 
// in partition  
//
//////////////////////////////////////////////////////////////////////////////
#include <TObjArray.h>
#include "IlcTimeStamp.h"
class TObjArray;
class IlcTimeStamp;
class IlcTriggerScalersESD;

class IlcTriggerScalersRecordESD : public TObject {

public:
                              IlcTriggerScalersRecordESD();
                   virtual   ~IlcTriggerScalersRecordESD() { fScalers.SetOwner(); fScalers.Delete(); }       
                      void    Reset();
                      void    AddTriggerScalers( IlcTriggerScalersESD* scaler );
                      void    AddTriggerScalers( UChar_t classIndex, ULong64_t LOCB, ULong64_t LOCA,        
                                              ULong64_t L1CB, ULong64_t L1CA, ULong64_t L2CB, ULong64_t L2CA );                            
           const TObjArray*   GetTriggerScalers()  const { return  &fScalers; }
const IlcTriggerScalersESD*   GetTriggerScalersForClass( const Int_t classindex ) const;       
        const IlcTimeStamp*   GetTimeStamp() const {return &fTimestamp;};
	             UInt_t   GetTimeGroup() const {return fTimeGroup; };
                       void   SetTimeStamp(const IlcTimeStamp *stamp){fTimestamp = *stamp;};
		       void   SetTimeGroup(const UInt_t group){fTimeGroup = group;};
               virtual void   Print( const Option_t* opt ="" ) const;
                      
                           IlcTriggerScalersRecordESD( const IlcTriggerScalersRecordESD &rec );
                      IlcTriggerScalersRecordESD&   operator=(const IlcTriggerScalersRecordESD& rec);      
               

private:  
                        IlcTimeStamp fTimestamp;    // record timestamp
                        TObjArray    fScalers;      // Array of scalers (IlcTriggerScalersESD) 
			      UInt_t fTimeGroup;    // Active time group
                        ClassDef( IlcTriggerScalersRecordESD, 3 )  // Define a Record of Trigger Scalers 
};

#endif
