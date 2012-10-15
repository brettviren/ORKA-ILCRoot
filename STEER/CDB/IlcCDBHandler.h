#ifndef ILCCDBHANDLER_H
#define ILCCDBHANDLER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * * See cxx source for full Copyright notice */
/* $Id$ */

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//  The SAX XML file handler used by the OCDB Manager                     //
//  get the OCDB Folder <-> Run Range correspondance                      //
//                                                                        //
//  Chiara.Zampolli (Chiara.Zampolli@cern.ch)                             //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

#include <TObject.h>
class TString;

class IlcCDBHandler : public TObject {

public:

  IlcCDBHandler();
  IlcCDBHandler(Int_t run);
  IlcCDBHandler(const IlcCDBHandler &sh);
  virtual ~IlcCDBHandler();
  IlcCDBHandler &operator=(const IlcCDBHandler &sh);

  // functions to interface to TSAXHandler
  void          OnStartDocument();
  void          OnEndDocument();
  void          OnStartElement(const char *name, const TList *attributes);
  void          OnEndElement(const char *name);
  void          OnCharacters(const char *name);
  void          OnComment(const char *name);
  void          OnWarning(const char *name);
  void          OnError(const char *name);
  void          OnFatalError(const char *name);
  void          OnCdataBlock(const char *name, Int_t len);

  Int_t GetStartRunRange() const {return fStartRunRange;}
  Int_t GetEndRunRange() const {return fEndRunRange;}
  TString GetOCDBFolder() const {return fOCDBFolder;}
  void SetRun(Int_t run) {fRun=run;}

 private:
  Int_t fRun;              // run for which the LHC Period Folder has to be found 
  Int_t fStartRunRange;    // start run corresponding to the request 
  Int_t fEndRunRange;      // end run corresponding to the request 
  TString fOCDBFolder;     // OCDB folder corresponding to the request 

  ClassDef(IlcCDBHandler,0);    // The XML file handler for the OCDB
};
#endif

