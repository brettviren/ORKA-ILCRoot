#ifndef ILCRAWEVENTTAG_H
#define ILCRAWEVENTTAG_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// The IlcRawEventTag class handles the raw-data event-oriented tag         //
// information. One object for each raw-data event is stored in a ROOT      //
// tree inside the file controled by IlcTagDB class.                        //
// For the moment the tag information includes the raw-data event header +  //
// the raw-data file GUID and the event index.                              //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#include <TObject.h>
#include <TString.h>

class IlcRawEventHeaderBase;

class IlcRawEventTag: public TObject {
 public:

  IlcRawEventTag();
  virtual ~IlcRawEventTag() {};

  void             SetHeader(IlcRawEventHeaderBase *header) { fHeader = header; }
  void             SetGUID(const char *guid) { fGUID = guid; }
  void             SetEventNumber(Int_t event) { fEvent = event; }

  
  IlcRawEventHeaderBase *GetHeader() const { return fHeader; }
  const char *     GetGUID() const { return fGUID.Data(); }
  Int_t            GetEventNumber() const { return fEvent; }

 private:

   IlcRawEventTag(const IlcRawEventTag& tag);
   IlcRawEventTag& operator = (const IlcRawEventTag& tag);

   IlcRawEventHeaderBase *fHeader;    // raw data event header
   TString                fGUID;      // GUID of the raw data file
   Int_t                  fEvent;     // raw data event number inside the file

   ClassDef(IlcRawEventTag,1)  // Raw data event tag
};

#endif
