#ifndef ILCHEADER_H
#define ILCHEADER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcHeader.h 54196 2012-01-27 15:15:29Z hristov $ */

//-----------------------------------------------------------------------
//     Simulation event header class
//     Collaborates with IlcRun, IlcStack, and IlcGenReaderTreeK classes
//     Many other classes depend on it
//-----------------------------------------------------------------------

#include <TObject.h>
#include <time.h>

class IlcStack;
class IlcGenEventHeader;
class IlcDetectorEventHeader;
class TObjArray;

class IlcHeader : public TObject {
public:
  IlcHeader();
  IlcHeader(const IlcHeader& head);
  IlcHeader(Int_t run, Int_t event);
  IlcHeader(Int_t run, Int_t eventSerialNr, Int_t evNrInRun);
  virtual ~IlcHeader();
  

  virtual void Reset(Int_t run, Int_t event);
  virtual void Reset(Int_t run, Int_t eventSerialNr, Int_t evNrInRun);

  virtual  void  SetRun(Int_t run) {fRun = run;}
  virtual  Int_t GetRun() const {return fRun;}
  
  virtual  void  SetNprimary(Int_t nprimary) {fNprimary = nprimary;}
  virtual  Int_t GetNprimary()   const {return fNprimary;}
  virtual  Int_t GetNsecondary() const {return fNtrack-fNprimary;}
  
  virtual  void  SetNvertex(Int_t vertex) {fNvertex = vertex;}
  virtual  Int_t GetNvertex() const {return fNvertex;}
  
  virtual  void  SetNtrack(Int_t ntrack) {fNtrack = ntrack;}
  virtual  Int_t GetNtrack() const {return fNtrack;}
  
  virtual  void  SetEvent(Int_t event) {fEvent = event;}
  virtual  Int_t GetEvent() const {return fEvent;}

  virtual  void  SetEventNrInRun(Int_t event) {fEventNrInRun = event;}
  virtual  Int_t GetEventNrInRun() const {return fEventNrInRun;}

  virtual  IlcStack* Stack() const;
  virtual  void SetStack(IlcStack* stack);

  virtual  void SetGenEventHeader(IlcGenEventHeader* header);
  virtual  IlcGenEventHeader*  GenEventHeader() const;

  virtual void AddDetectorEventHeader(IlcDetectorEventHeader* header);
  virtual IlcDetectorEventHeader* GetDetectorEventHeader(const char *name) const;
  
  virtual  void SetTimeStamp(time_t timeStamp) {fTimeStamp = timeStamp;}
  virtual  time_t GetTimeStamp() const {return fTimeStamp;}
  
  virtual void Print(const char *opt=0) const;

  IlcHeader& operator=(const IlcHeader& head) 
    {head.Copy(*this); return *this;}
  
protected:

  void Copy(TObject& head) const;

  Int_t         fRun;               //Run number
  Int_t         fNvertex;           //Number of vertices
  Int_t         fNprimary;          //Number of primary tracks
  Int_t         fNtrack;            //Number of tracks
  Int_t         fEvent;             //Event number (serial in the file)
  Int_t         fEventNrInRun;      //Unique Event number within the run
  time_t        fTimeStamp;         //Event time-stamp
  IlcStack     *fStack;             //Pointer to stack
  IlcGenEventHeader* fGenHeader;    //Event Header for Generator
  TObjArray*         fDetHeaders;   //Event Headers for detector specific information 

  ClassDef(IlcHeader,4) //Ilc event header    
};

#endif
