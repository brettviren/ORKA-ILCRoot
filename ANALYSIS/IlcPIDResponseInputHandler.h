//
// Class IlcPIDResponseInputHandler
//
// IlcPIDResponseInputHandler
// TODO example
// authors:
//        Jens Wiechula (jens.wiechula@cern.ch)
//        Martin Vala (martin.vala@cern.ch)
//

#ifndef ILCPIDRESPONSEINPUTHANDLER_H
#define ILCPIDRESPONSEINPUTHANDLER_H

#include "IlcInputEventHandler.h"
class IlcPIDResponse;
class IlcMultiInputEventHandler;

class IlcPIDResponseInputHandler : public IlcInputEventHandler {
  
public:
  IlcPIDResponseInputHandler(const char *name = "PIDResoponseIH");
  virtual ~IlcPIDResponseInputHandler();
  
   // From the interface
  virtual Bool_t  Init(Option_t *opt);
  virtual Bool_t  Init(TTree *tree, Option_t *opt);
  virtual Bool_t  BeginEvent(Long64_t entry);
  virtual Bool_t  FinishEvent();
  virtual Bool_t  Notify();
  virtual Bool_t  Notify(const char *path);
  virtual Bool_t  GetEntry();
  
  void SetIsMC(Bool_t isMC=kTRUE) { fIsMC=isMC; }
private:
  
  Bool_t fIsMC;                        //  If we run on MC data
  
  IlcPIDResponse *fPIDResponse;        //! PID response Handler
  Int_t   fRun;                        //! current run number
  Int_t   fOldRun;                     //! current run number
  Int_t   fRecoPass;                   //! reconstruction pass
  
  IlcMultiInputEventHandler *fMCurrentMutliIH;  //! input handler
  
  //
  void SetRecoInfo();
  
  
  IlcPIDResponseInputHandler(const IlcPIDResponseInputHandler& handler);
  IlcPIDResponseInputHandler &operator=(const IlcPIDResponseInputHandler &handler);
  
  ClassDef(IlcPIDResponseInputHandler, 1)
    
};

#endif
