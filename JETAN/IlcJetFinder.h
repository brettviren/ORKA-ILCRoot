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
//---------------------------------------------------------------------

//#include <Riostream.h>
//#include <vector>

#include <TObject.h>
#include "IlcAODJet.h"
#include "IlcJetHeader.h"
#include "IlcJetReader.h"
#include "IlcAODJetEventBackground.h"

class TChain;
class TTree;
class IlcAODEvent;
class IlcJetReaderHeader;

class IlcJetFinder : public TObject 
{
 public:
  IlcJetFinder();
  virtual ~IlcJetFinder();

  // Getters
  virtual IlcJetReader *GetReader() const {return fReader;}
  virtual IlcJetHeader *GetHeader() const {return fHeader;}
  virtual IlcAODJetEventBackground* GetEventBackground() const {return fAODEvBkg;}
  // Setters
  virtual void          SetJetReader(IlcJetReader* r) {fReader=r;}
  virtual void          SetJetHeader(IlcJetHeader* h) {fHeader=h;}
  virtual void          SetEventBackground(IlcAODJetEventBackground* bkg) {fAODEvBkg = bkg;}
  // Others
  virtual void          AddJet(IlcAODJet jet);
  virtual void          WriteRHeaderToFile();  
  // the following have to be implemented for each specific finder
  virtual void          Init() {}
  virtual void          InitTask(TChain* /*tree*/) {}
  virtual void          Reset() {fNAODjets = 0;}
  virtual void          FindJets() {}
  virtual void          FindJetsC(){}
  virtual void          WriteJHeaderToFile() const {}
  // some methods to allow steering from the outside
  virtual Bool_t        ProcessEvent();
  virtual Bool_t        ProcessEvent2();
  virtual void          ConnectTree(TTree* tree, TObject* data);
  virtual void          ConnectAOD(const IlcAODEvent* aod);
  virtual void          ConnectAODNonStd(IlcAODEvent* aod,const char* bname);
  virtual void          WriteHeaders();

 protected:
  IlcJetFinder(const IlcJetFinder& rJetFinder);
  IlcJetFinder& operator = (const IlcJetFinder& rhsf);
  IlcJetReader*             fReader;         //  pointer to reader
  IlcJetHeader*             fHeader;         //  pointer to header
  TClonesArray*             fAODjets;        //! reconstructed jets
  Int_t                     fNAODjets;       //! number of reconstructed jets
  IlcAODJetEventBackground* fAODEvBkg;       //! bkg object to be store
  Int_t                     fDebug;          //  debug option, set through the header

  ClassDef(IlcJetFinder,2)
};

#endif
