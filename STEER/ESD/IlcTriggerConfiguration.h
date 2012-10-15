#ifndef ILCTRIGGERCONFIGURATION_H
#define ILCTRIGGERCONFIGURATION_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// This class represents the CTP configuration                               //
//                                                                           //
//                                                                           //
// A Trigger Configuration define a trigger setup for particula run          //
// We have default one for different running modes                           //
// (Pb-Pb, p-p, p-A, Calibration, etc).                                      //
// It keep:                                                                  //
//   All the information contained in the real CTP configuration file        //
//   used online during the data taking.                                     //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TNamed.h>
#include <TObjArray.h>
class TString;

class IlcTriggerCluster;
class IlcTriggerDescriptor;
class IlcTriggerInput;
class IlcTriggerInteraction;
class IlcTriggerPFProtection;
class IlcTriggerBCMask;
class IlcTriggerClass;

class IlcTriggerConfiguration : public TNamed {

public:
                          IlcTriggerConfiguration();
                          IlcTriggerConfiguration( TString & name, TString & description );
               virtual   ~IlcTriggerConfiguration();
   //  Setters
       IlcTriggerInput*   AddInput(TString &name, TString &det, UChar_t level, UInt_t signature, UChar_t number);
		Bool_t    AddInput(IlcTriggerInput *input);

 IlcTriggerInteraction*   AddInteraction(TString &name, TString &logic);
                Bool_t    AddInteraction(IlcTriggerInteraction *interact);

 IlcTriggerInteraction*   AddFunction(TString &name, TString &logic);
                Bool_t    AddFunction(IlcTriggerInteraction *func);

		Bool_t    AddPFProtection( IlcTriggerPFProtection* pf );

      IlcTriggerBCMask*   AddMask( TString &name, TString &mask );
		Bool_t    AddMask( IlcTriggerBCMask* mask );

     IlcTriggerCluster*   AddCluster( TString &name, UChar_t index, TString &detectors );
                Bool_t    AddCluster( IlcTriggerCluster* cluster );

  IlcTriggerDescriptor*   AddDescriptor( TString & name, TString & cond);
                Bool_t    AddDescriptor( IlcTriggerDescriptor* desc );

       IlcTriggerClass*   AddClass( TString &name, UChar_t index,
				    IlcTriggerDescriptor *desc, IlcTriggerCluster *clus,
				    IlcTriggerPFProtection *pfp, IlcTriggerBCMask *mask,
				    UInt_t prescaler, Bool_t allrare);
       IlcTriggerClass*   AddClass( TString &name, UChar_t index,
				    TString &desc, TString &clus,
				    TString &pfp, TString &mask,
				    UInt_t prescaler, Bool_t allrare);
                 Bool_t   AddClass( IlcTriggerClass *trclass);

		   void   Reset();

  //  Getters
               TString    GetActiveDetectors() const;
               TString    GetTriggeringDetectors() const;
               TString    GetTriggeringModules() const;

       const TObjArray&   GetInputs() const { return fInputs; }
       const TObjArray&   GetInteractions() const { return fInteractions; }
       const TObjArray&   GetFunctions() const { return fFunctions; }
       const TObjArray&   GetPFProtections() const { return fPFProtections; }
       const TObjArray&   GetMasks() const { return fMasks; }
       const TObjArray&   GetClusters() const { return fClusters; }
       const TObjArray&   GetDescriptors() const { return fDescriptors; }
       const TObjArray&   GetClasses() const { return fClasses; }

                  Int_t   GetVersion() const { return fVersion; }
	          Int_t   GetClassIndexFromName(const char* className) const;
             const char*  GetClassNameFromIndex(Int_t classIndex) const;
	IlcTriggerClass*  GetTriggerClass(Int_t classIndex) const;
       //     IlcTriggerCluster*   GetTriggerCluster(UInt_t index)
       //       { return (index < kNMaxClusters) ? (IlcTriggerCluster*)fClusters[index] : NULL; }

       //IlcTriggerPFProtection*   GetPFProtection(UInt_t index)
       //	       { return (index < kNMaxPFProtections) ? (IlcTriggerPFProtection*)fPFProtections[index] : NULL; }
                  void    Print( const Option_t* opt ="" ) const;

  //  Configurations Database (root file)
                  void    WriteConfiguration( const char* filename="" );
      static TObjArray*   GetAvailableConfigurations( const char* filename="" );

      static IlcTriggerConfiguration* LoadConfiguration(TString & des);
      static IlcTriggerConfiguration* LoadConfigurationFromString(const char* configuration);
      Bool_t    IsSelected( TString detName, TString & detectors ) const;

      enum {kNMaxInputs = 60}; // CTP handles up to 60 trigger detector inputs
      enum {kNMaxInteractions = 2}; // CTP handles up to two different interactions
      enum {kNMaxFunctions = 4}; // CTP handles up to 4 different logical functions
      enum {kNMaxClasses = 50}; // Maximum number of trigger classes = 50
      enum {kNMaxClusters = 6}; // Maximum number of different detector clusters that can be handled by CTP
      enum {kNMaxPFProtections = 4}; // Maximum number of different past-future protections that can be handled by CTP
      enum {kNMaxMasks = 13};  // CTP handles up to 12 different BC masks + NONE

private:
      Bool_t ProcessConfigurationLine(const char* line, Int_t& level);

      TObjArray            fInputs;                           // Array with active CTP inputs
      TObjArray            fInteractions;                     // Array with the trigger interactions
      TObjArray            fFunctions;                        // Array with the logical functions of the first 4 inputs
      TObjArray            fPFProtections;                    // Array of Past-Future Protections
      TObjArray            fMasks;                            // Array with BC masks
      TObjArray            fDescriptors;                      // Array with trigger descriptors
      TObjArray            fClusters;                         // Array of Detector Trigger Clusters
      TObjArray            fClasses;                          // Array of Trigger Classes

      Int_t                fVersion;                          // Configuration format version

   static const TString    fgkConfigurationFileName;        //! name of default configurations file

   IlcTriggerConfiguration&   operator=(const IlcTriggerConfiguration& des);
   IlcTriggerConfiguration( const IlcTriggerConfiguration& des );

   ClassDef( IlcTriggerConfiguration, 3 )  // Define a trigger configuration
};

#endif
