#ifndef ILCTENDER_H
#define ILCTENDER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTender.h 47550 2011-02-18 10:55:54Z mvala $ */
// Author: Andrei Gheata, 31/08/2009

//==============================================================================
//   IlcTender - Tender wagon providing access to ESD event and CDB.
//      The tender calls an arbitrary number of user algorithms that add or
//      correct information in ESD based on CDB info that was not available
//      during pass1 reconstruction.
//==============================================================================

#ifndef ILCANALYSISTASKSE_H
#include "IlcAnalysisTaskSE.h"
#endif

// #ifndef ILCESDINPUTHANDLER_H
// #include "IlcESDInputHandler.h"
// #endif
class IlcCDBManager;
class IlcESDEvent;
class IlcESDInputHandler;
class IlcTenderSupply;

class IlcTender : public IlcAnalysisTaskSE {

public:
enum ETenderFlags {
   kCheckEventSelection = BIT(18) // up to 18 used by IlcAnalysisTask
};
   
private:
  Int_t                     fRun;            //! Current run
  Bool_t                    fRunChanged;     //! Flag for run change.
  ULong_t                   fCDBkey;         //! Key to unlock CDB manager
  TString                   fDefaultStorage; // Default CDB storage
  IlcCDBManager            *fCDB;            //! Pointer to CDB manager
  IlcESDInputHandler       *fESDhandler;     //! Pointer to ESD input handler
  IlcESDEvent              *fESD;            //! Pointer to current ESD event
  TObjArray                *fSupplies;       // Array of tender supplies
  TObjArray                *fCDBSettings;    // Array with CDB configuration
  
  IlcTender(const IlcTender &other);
  IlcTender& operator=(const IlcTender &other);

public:  
  IlcTender();
  IlcTender(const char *name);
  virtual ~IlcTender();

  void                      AddSupply(IlcTenderSupply *supply);
  Int_t                     GetRun() const {return fRun;}
  IlcCDBManager            *GetCDBManager() const {return fCDB;}
  IlcESDInputHandler       *GetESDhandler() const {return fESDhandler;}
  IlcESDEvent              *GetEvent() const {return fESD;}
  TObjArray                *GetSupplies() const {return fSupplies;}
  void                      SetCheckEventSelection(Bool_t flag=kTRUE) {TObject::SetBit(kCheckEventSelection,flag);}
  Bool_t                    RunChanged() const {return fRunChanged;}
  // Configuration
  void                      SetDefaultCDBStorage(const char *dbString="local://$ILC_ROOT/OCDB");
  void SetESDhandler(IlcESDInputHandler*esdH) {fESDhandler = esdH;}

  // Run control
  virtual void              ConnectInputData(Option_t *option = "");
  virtual void              UserCreateOutputObjects();
//  virtual Bool_t            Notify() {return kTRUE;}
  virtual void              UserExec(Option_t *option);
    
  ClassDef(IlcTender,3)  // Class describing the tender car for ESD analysis
};
#endif
