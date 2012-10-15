#ifndef ILCCENTRALTRIGGER_H
#define ILCCENTRALTRIGGER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcCentralTrigger.h 50615 2011-07-16 23:19:19Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// This class for running the Central Trigger Processor                      //
//                                                                           //
//                                                                           //
//    Load Configuration                                                     //
//    Make a list the trigger detectors involved ( from the configuration)   //
//    For the each event                                                     //
//           Run the Trigger for the each detector                           //
//           Get the inputs                                                  //
//           Check the trigger classes                                       //
//           Create the class mask                                           //
//           Save result                                                     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TObject.h>
#include <TObjArray.h>

class TTree;
class IlcRunLoader;
class IlcTriggerConfiguration;

class IlcCentralTrigger : public TObject {

public:
                          IlcCentralTrigger();
                          IlcCentralTrigger( TString & config );
               virtual   ~IlcCentralTrigger();

                Bool_t    LoadConfiguration( TString & config );
                Bool_t    RunTrigger( IlcRunLoader * runloader , const char* detectors);
             ULong64_t    TriggerClasses();
	          void    TriggerInputs();
                  void    Reset();
		  void    DeleteConfiguration();
                  void    MakeBranch( TString name, TTree * tree );
  //  Getters
               TString    GetDetectors();
             ULong64_t    GetClassMask() const { return fClassMask; }
	        UInt_t    GetClusterMask() const { return fClusterMask; }
	     	UInt_t    GetL0TriggerInputs() const { return fL0TriggerInputs; }
	     	UInt_t    GetL1TriggerInputs() const { return fL1TriggerInputs; }
	      UShort_t    GetL2TriggerInputs() const { return fL2TriggerInputs; }
 IlcTriggerConfiguration* GetConfiguration() { return fConfiguration; }
             TObjArray*   GetFiredClasses() const;
                  void    Print( const Option_t* opt ="" ) const;
	        Bool_t    CheckTriggeredDetectors() const;

	       // Setters to be used in case raw data when the trigger information
	       // is read from the event header
	       void       SetClassMask(ULong64_t mask) { fClassMask = mask; }
	       void       SetClusterMask(UInt_t mask)  { fClusterMask = mask; }
	       void       SetL0TriggerInputs(UInt_t mask)  { fL0TriggerInputs = mask; }
	       void       SetL1TriggerInputs(UInt_t mask)  { fL1TriggerInputs = mask; }
	       void       SetL2TriggerInputs(UShort_t mask)  { fL2TriggerInputs = mask; }
protected:
             ULong64_t    fClassMask;          // UID ( bitwise OR of conditions mask )
                UInt_t    fClusterMask;        // UID ( bitwise OR of clusters mask - detector pattern)
    	        UInt_t    fL0TriggerInputs;    // L0 trigger inputs (24 bits)
                UInt_t    fL1TriggerInputs;    // L1 trigger inputs (24 bits)
              UShort_t    fL2TriggerInputs;    // L2 trigger inputs (12 bits)

 IlcTriggerConfiguration* fConfiguration;      // Trigger Configuration used

private:
                void      SetOwner(Bool_t x=kTRUE){SetBit(22,x);} // Bit 22 indicates that the object owns fConfiguration
                Bool_t    IsOwner() const {return TestBit(22);} // Test bit 22 to check that the object owns fConfiguration
                Bool_t    IsSelected( TString detName, TString& detectors ) const;
		IlcCentralTrigger( const IlcCentralTrigger& ctp ); // Implemented
		IlcCentralTrigger& operator=( const IlcCentralTrigger& ctp ); // Not implemented

   ClassDef( IlcCentralTrigger, 5 )  // class for running the Central Trigger Processor
};


#endif
