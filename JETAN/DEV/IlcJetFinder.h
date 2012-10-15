#ifndef ILCJETFINDER_H
#define ILCJETFINDER_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
 
//---------------------------------------------------------------------
// Jet finder base class
// manages the search for jets 
// Authors: jgcn@mda.cinvestav.mx
//          andreas.morsch@cern.ch
//          magali.estienne@subatech.in2p3.fr
//          alexandre.shabetai@cern.ch
//---------------------------------------------------------------------

#include "IlcJetCalTrk.h"
#include "IlcAODJet.h"
#include "IlcJetHeader.h"
#include "IlcAODJetEventBackground.h"
class IlcAODEvent;

class IlcJetFinder : public TObject 
{
 public:
  IlcJetFinder();
  virtual ~IlcJetFinder();

  // Getters
  virtual IlcJetCalTrkEvent* GetCalTrkEvent() const {return fCalTrkEvent;}
  virtual IlcJetHeader *GetJetHeader() const {return fHeader;}
  virtual IlcAODJetEventBackground* GetEventBackground() const {return fAODEvBkg;}
  // Setters
  virtual void              SetCalTrkEvent(IlcJetCalTrkEvent& event) {fCalTrkEvent = &event;}
  virtual void              SetJetHeader(IlcJetHeader* h) {fHeader=h;}
  virtual void              SetEventBackground(IlcAODJetEventBackground* bkg) {fAODEvBkg = bkg;}
  // Others
  virtual void              AddJet(IlcAODJet jet);
  virtual void              WriteHeaderToFile();
  virtual void		    WriteHeader();
  // the following have to be implemented for each specific finder
  virtual void              Init() {}
  virtual void              Reset() {fNAODjets = 0;}
  virtual void              FindJets() {}
  virtual void              ComputeBkgs() {}
  virtual void              CreateOutputObjects(TList * const /*histos*/) {} // Used by CDF for histo storage

  // some methods to allow steering from the outside
  virtual Bool_t            ProcessEvent();
  virtual void              ConnectAOD(const IlcAODEvent* aod);
  virtual void              ConnectAODNonStd(IlcAODEvent* aod,const char* bname);
  virtual void              AddHistosToList(TList */*list*/) {}

 protected:
  IlcJetFinder(const IlcJetFinder& rJetFinder);
  IlcJetFinder& operator = (const IlcJetFinder& rhsf);
  IlcJetHeader*             fHeader;         //  pointer to header
  TClonesArray*             fAODjets;        //! reconstructed jets
  Int_t                     fNAODjets;       //  number of reconstructed jets
  IlcAODJetEventBackground* fAODEvBkg;       //! bkg object to be store
  Int_t                     fDebug;          //  debug option, set through the header
  IlcJetCalTrkEvent*        fCalTrkEvent;    //  pointer to IlcJetCalTrkEvent object
 
  ClassDef(IlcJetFinder,3)                   //  base class for any jet finder

};

#endif
