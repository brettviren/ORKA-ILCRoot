#ifndef ILCTRIGGERSCALERSRECORD_H
#define ILCTRIGGERSCALERSRECORD_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTriggerScalersRecord.h 22322 2007-11-22 11:43:14Z cvetan $ */

///////////////////////////////////////////////////////////////////////////////
//
// Class to define the ILC Trigger Scalers Record 
//
// Each record consists of 1 time stamp (4 words)  (IlcTimeStamp)
// and an array with the scalers (IlcTriggerScalers) for each trigger class 
// in partition  
//
//////////////////////////////////////////////////////////////////////////////
class TObjArray;
class  IlcTimeStamp;

class IlcTriggerScalersRecord : public TObject {

public:
                            IlcTriggerScalersRecord();
                 virtual   ~IlcTriggerScalersRecord() { fScalers.SetOwner(); fScalers.Delete(); }
                 
                 
                    void    SetTimeStamp( UInt_t orbit, UInt_t period, UInt_t seconds, UInt_t microsecs );
		    void    SetTimeGroup(UInt_t tgr){fTimeGroup=tgr;};
                    void    AddTriggerScalers( IlcTriggerScalers* scaler );
                    void    AddTriggerScalers( UChar_t classIndex, UInt_t LOCB, UInt_t LOCA,        
                                              UInt_t L1CB, UInt_t L1CA, UInt_t L2CB, UInt_t L2CA );
                            
      const IlcTimeStamp*   GetTimeStamp() const { return &fTimestamp; }
         const TObjArray*   GetTriggerScalers()  const { return  &fScalers; }
 const IlcTriggerScalers*   GetTriggerScalersForClass( const Int_t classindex ) const;       
       IlcTriggerScalers*   GetTriggerScalersForClassBinary( const Int_t classindex ) ;     
                   UInt_t   GetTimeGroup(){return fTimeGroup;}
          virtual Bool_t    IsSortable() const { return kTRUE; }
                                
           virtual Int_t    Compare( const TObject* obj ) const;
            virtual void    Print( const Option_t* opt ="" ) const;

       
             
     
private:  

            IlcTimeStamp    fTimestamp;    // record timestamp
               TObjArray    fScalers;      // Array of scalers (IlcTriggerScalers) 
	          UInt_t    fTimeGroup;    // Time group of record


                            IlcTriggerScalersRecord( const IlcTriggerScalersRecord &rec );
 IlcTriggerScalersRecord&   operator=(const IlcTriggerScalersRecord& rec);

   ClassDef( IlcTriggerScalersRecord, 2 )  // Define a Record of Trigger Scalers 
};

#endif
