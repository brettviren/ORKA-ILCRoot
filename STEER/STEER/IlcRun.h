#ifndef ILCRUN_H
#define ILCRUN_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcRun.h 50615 2011-07-16 23:19:19Z hristov $ */

//
// General configuration class for Simulation and Reconstruction
// Basic driver for IlcRoot runs
// Containing pointers to data elements for IlcRoot
//

#include <TSystem.h>

class TRandom;

#include "IlcRunLoader.h"
class IlcGenEventHeader;
class IlcGenerator;
class IlcHeader;
class IlcMC;
class IlcMagF;
class IlcStack;


class IlcRun : public TNamed {

public:
   // Creators - distructors
   IlcRun();
   IlcRun(const char *name, const char *title);
   virtual ~IlcRun();

   TObjArray     *Detectors() const {return fModules;}
   TObjArray     *Modules() const {return fModules;}
   void           AddModule(IlcModule* mod);
   Int_t          GetEvNumber() const;
   void           SetEventNrInRun(Int_t event) {fEventNrInRun=event;}
   Int_t          GetEventNrInRun() const {return fEventNrInRun;}
   Int_t          GetNdets() const {return fNdets;}
   IlcModule     *GetModule(const char *name) const;
   IlcDetector   *GetDetector(const char *name) const;
   Int_t          GetModuleID(const char *name) const;
   virtual  const char *GetBaseFile() const 
    {return fBaseFileName.Data();}
   virtual  Int_t GetEvent(Int_t event);
   virtual  void  SetConfigFunction(const char * config="Config();")
   {fConfigFunction=config;}
   virtual  const char *GetConfigFunction() const 
   {return fConfigFunction.Data();}
   virtual  void  SetGenEventHeader(IlcGenEventHeader* header);
   IlcMC*         GetMCApp() const {return fMCApp;}
   virtual  void  Hits2Digits(const char *detector=0); 
   virtual  void  Hits2SDigits(const char *detector=0)   {Tree2Tree("S",detector);}
   virtual  void  SDigits2Digits(const char *detector=0) {Tree2Tree("D",detector);}
   virtual  void  Digits2Reco(const char *detector=0)    {Tree2Tree("R",detector);}
   Bool_t         IsFolder() const {return kTRUE;}
   virtual  void  SetBaseFile(const char *filename="gilc.root");
   //
   // End of MC Application

   void SetRunLoader(IlcRunLoader* rloader);

  virtual  void Announce() const;
   
  virtual  void  InitLoaders(); //prepares run (i.e. creates getters)

protected:
  virtual  void  Tree2Tree(Option_t *option, const char *detector=0);
  Int_t          fEventNrInRun;      //! Current unique event number in run
  TObjArray     *fModules;           //  List of Detectors
  IlcMC         *fMCApp;             //  Pointer to virtual MC Application
  Int_t          fNdets;             //  Number of detectors
  TString        fConfigFunction;    //  Configuration file to be executed
  TString        fBaseFileName;      //  Name of the base root file
  IlcRunLoader  *fRunLoader;         //! run getter - written as a separate object
private:
  IlcRun(const IlcRun&); // Not implemented
  IlcRun& operator = (const IlcRun&); // Not implemented

  ClassDef(IlcRun,15)      //Supervisor class for all Ilc detectors
};
 
R__EXTERN  IlcRun *gIlc;
  
#endif
