#ifndef ILCAODEXTENSION_H
#define ILCAODEXTENSION_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAODExtension.h 50615 2011-07-16 23:19:19Z hristov $ */

//-------------------------------------------------------------------------
//     Support class for AOD extensions. This is created by the user analysis
//     that requires a separate file for some AOD branches. The name of the 
//     IlcAODExtension object is the file name where the AOD branches will be
//     stored.
//     Author: Andrei Gheata, CERN
//-------------------------------------------------------------------------

#ifndef ROOT_TNamed
#  include "TNamed.h"
#endif

class IlcAODBranchReplicator;
class IlcAODEvent;
class TFile;
class TList;
class TMap;
class TTree;

class IlcAODExtension : public TNamed {
  
public:
  
  enum EIlcAODExtensionFlags {
    kFilteredAOD      = BIT(14),
    kDropUnspecifiedBranches = BIT(15)
  };
  
  IlcAODExtension();
  IlcAODExtension(const char* name, const char* title, Bool_t isfilter=kFALSE);
  virtual ~IlcAODExtension();
  void                 AddBranch(const char* cname, void* addobj);
  Bool_t               FinishEvent();
  Int_t                GetNtotal() const         {return fNtotal;}
  Int_t                GetNpassed() const        {return fNpassed;}
  const char*          GetOutputFileName() const {return TNamed::GetName();}
  IlcAODEvent*         GetAOD() const            {return fAODEvent;}
  TTree*               GetTree() const           {return fTreeE;}
  Bool_t               Init(Option_t *option);
  Bool_t               IsFilteredAOD() const     {return TObject::TestBit(kFilteredAOD);}
  Bool_t               IsEventSelected() const   {return fSelected;}
  void                 SelectEvent(Bool_t flag=kTRUE)  {fSelected = flag;}
  void                 SetEvent(IlcAODEvent* event);
  void                 SetOutputFileName(const char* fname) {TNamed::SetName(fname);}
  Bool_t               TerminateIO();
  
  void Print(Option_t* opt="") const;
  
  // Branches not specified in any FilterBranch call will be dropped by default
  void DropUnspecifiedBranches() { TObject::SetBit(kDropUnspecifiedBranches); }
  
  // Branches not specified in any FilterBranch call will be kept by default
  void KeepUnspecifiedBranches() { TObject::ResetBit(kDropUnspecifiedBranches); }
  
  void FilterBranch(const char* branchName, IlcAODBranchReplicator* replicator=0x0);
  
  /* Use DisableReferences if and only if the output AOD contains no TRef or TRefArray,
   otherwise the produced AOD won't be valid.
   */
  void DisableReferences() { fEnableReferences=kFALSE; }
  
  void EnableReferences() { fEnableReferences=kTRUE; }
  
  void AddAODtoTreeUserInfo();
  
private:
  IlcAODExtension(const IlcAODExtension&);             // Not implemented
  IlcAODExtension& operator=(const IlcAODExtension&);  // Not implemented
  
private:
  IlcAODEvent             *fAODEvent;               //! Pointer to the AOD event
  TTree                   *fTreeE;                  //! tree for AOD persistency
  TFile                   *fFileE;                  //! Output file
  Int_t                    fNtotal;                 //! Number of processed events
  Int_t                    fNpassed;                //! Number of events that passed the filter
  Bool_t                   fSelected;               //! Select current event for filtered AOD's. Made false at event start.
  
  TMap*                    fRepFiMap; // which branch(es) to filter out / and or replicate
  TList*                   fRepFiList; // list of unique filter/replicator
  
  Bool_t                   fEnableReferences; // whether or not to enable the TRefTable branch
  TList*                   fObjectList; //! internal list of which objects to keep 
  
  ClassDef(IlcAODExtension, 2) // Support for extra AOD branches in a separate AOD file
};

#endif
