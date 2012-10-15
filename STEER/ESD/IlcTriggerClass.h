#ifndef ILCTRIGGERCLASS_H
#define ILCTRIGGERCLASS_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// This class represents the CTP class objects                               //
//                                                                           //
// The Class consists of Name, index in the trigger mask counted from 1,     //
// descriptor, cluster,past-future, mask, downscale, allrare,                //
// time group, time window                                                   //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TNamed.h>

class IlcTriggerConfiguration;
class IlcTriggerDescriptor;
class IlcTriggerCluster;
class IlcTriggerPFProtection;
class IlcTriggerBCMask;

class IlcTriggerClass : public TNamed {

public:
                          IlcTriggerClass();
                          IlcTriggerClass( TString & name, UChar_t index,
					   IlcTriggerDescriptor *desc, IlcTriggerCluster *clus,
					   IlcTriggerPFProtection *pfp, IlcTriggerBCMask *mask,
					   UInt_t prescaler, Bool_t allrare);
                          IlcTriggerClass( IlcTriggerConfiguration *config,
					   TString & name, UChar_t index,
					   TString &desc, TString &clus,
					   TString &pfp, TString &mask,
					   UInt_t prescaler, Bool_t allrare);
                          IlcTriggerClass( IlcTriggerConfiguration *config,
					   TString & name, UChar_t index,
					   TString &desc, TString &clus,
					   TString &pfp,
					   UInt_t prescaler, Bool_t allrare,
					   UInt_t timegroup, UInt_t timewindow);

                          IlcTriggerClass( const IlcTriggerClass& trclass );
               virtual   ~IlcTriggerClass();
  IlcTriggerClass&   operator=(const IlcTriggerClass& trclass);

                  void    Reset() { fStatus = kFALSE; }

             ULong64_t    GetValue() const { return (fStatus) ? fClassMask : 0; }
                Bool_t    GetStatus() const { return fStatus; }
               ULong64_t  GetMask() const { return fClassMask; }
    IlcTriggerDescriptor* GetDescriptor() const { return fDescriptor; }
       IlcTriggerCluster* GetCluster() const { return fCluster; }
        IlcTriggerBCMask* GetBCMask() const { return fMask[0]; }
	           UInt_t GetTimeGroup() const { return fTimeGroup; }
	           UInt_t GetTimeWindow() const { return fTimeGroup; }

		   Bool_t SetMasks(IlcTriggerConfiguration *config,TString &mask);
                    void  Trigger( const TObjArray& inputs , const TObjArray& functions);
		    void  Print( const Option_t* ) const;

                  Bool_t  CheckClass(IlcTriggerConfiguration *config) const;
		  Bool_t  IsActive( const TObjArray& inputs, const TObjArray& functions) const;
		  enum {kNMaxMasks = 13};  // CTP handles up to 12 different BC masks + NONE

private:
	       ULong64_t  fClassMask;    // trigger mask (1<< (index-1))
	       	 UChar_t  fIndex;        // position of class in mask
    IlcTriggerDescriptor* fDescriptor;   // pointer to the descriptor
       IlcTriggerCluster* fCluster;      // pointer to the cluster
  IlcTriggerPFProtection* fPFProtection; // pointer to the past-future protection
        IlcTriggerBCMask* fMask[kNMaxMasks];         // array of pinters pointer to bunch-crossing mask
                  UInt_t  fPrescaler;    // Downscaling factor
                  Bool_t  fAllRare;      // All or Rare trigger
		  Bool_t  fStatus;       //! true = Condition has been satisfied after Trigger
		  UInt_t  fTimeGroup;    // time group
		  UInt_t  fTimeWindow;   // the size of time window for its group

  ClassDef( IlcTriggerClass, 5 )  // Define a trigger class object
};

#endif
