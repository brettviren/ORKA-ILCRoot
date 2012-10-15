#ifndef ILCJETREADER_H
#define ILCJETREADER_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */
 
// Jet reader base class
// manages the reading of input for jet algorithms
// Authors: jgcn@mda.cinvestav.mx
//          Magali Estienne <magali.estienne@subatech.in2p3.fr>  
//          alexandre.shabetai@cern.ch

#include <TObject.h>

class IlcJetReaderHeader;
class IlcJetFillCalTrkEvent;
class IlcJetCalTrkEvent;
class IlcVEvent;
class IlcMCEvent;

class IlcJetReader : public TObject 
{
 public: 
  IlcJetReader();
  virtual ~IlcJetReader();

  // Getters
  IlcJetCalTrkEvent*        GetCalTrkEvent()       const {return fCalTrkEvent;}
  IlcJetReaderHeader*       GetReaderHeader()      const {return fReaderHeader;}
  
  // Setters
  void                      SetReaderHeader(IlcJetReaderHeader* header)      {fReaderHeader = header;}  

  // Others
  void                      SetInputEvent(const TObject* esd, const TObject* aod, const IlcMCEvent* mc);
  void                      InitTasks();
  Bool_t                    CreateTasks();
  Bool_t                    ExecTasks();
  Bool_t                    ProcessEvent();
  void                      WriteRHeaderToFile() const;
  void                      WriteReaderHeader();
 
 protected:
  IlcJetReader(const IlcJetReader& rJetReader);
  IlcJetReader& operator = (const IlcJetReader& rhsr);

  IlcJetCalTrkEvent*        fCalTrkEvent;                     //! Pointer to calTrkEvent
  IlcJetFillCalTrkEvent*    fFillEvent;                       //! Pointer to IlcJetFillCalTrkEvent
  IlcJetReaderHeader*       fReaderHeader;                    //  Pointer to header
  IlcJetFillCalTrkEvent*    fFillEventwTrks;                  //  For charged particle task
  Int_t                     fDebug;                           //  Debug option
  IlcVEvent*                fVEvent;                          //! Input event
  IlcMCEvent*               fMCEvent;			      //! MC Event;
  Int_t                     fOpt;                             //  Detector config
  
  ClassDef(IlcJetReader,2)                                    // jet reader class

};
 
#endif
