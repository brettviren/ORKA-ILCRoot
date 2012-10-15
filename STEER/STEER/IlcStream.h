#ifndef ILCSTREAM_H
#define ILCSTREAM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcStream.h 50615 2011-07-16 23:19:19Z hristov $ */

////////////////////////////////////////////////////////////////////////
//
//  Class to handle files on IO
//  Handles files and returns serial event number                  
//  Author: Jiri Chudoba (CERN), 2001
//
////////////////////////////////////////////////////////////////////////

// --- ROOT system ---
#include <TNamed.h>

// --- IlcRoot header files ---
class TObjArray;
class TFile;

class TString;

class IlcStream: public TNamed {

public:
  IlcStream();
  IlcStream(const char* foldername, Option_t *optioneventfoldername);
  IlcStream(const IlcStream &as);
  IlcStream & operator = (const IlcStream & as) 
    {as.Copy(*this); return *this;}
  virtual ~IlcStream();

  void       AddFile(const char *fileName);
  Bool_t     NextEventInStream();
  Bool_t     OpenNextFile();//returns kFALSE in case of failure
  Bool_t     ImportgIlc();
  void       ChangeMode(Option_t* option);     // reset READ or UPDATE mode
 
  const TString& GetFolderName() const{return fEventFolderName;}
  Int_t GetNInputFiles() const {return fFileNames->GetLast()+1;}
  TString GetFileName(Int_t order) const;
  void SetFolderName(const TString name) { fEventFolderName = name ; }
  Int_t GetCurrentEventNumber() const { return fLastEventSerialNr ; }
    
private:  

  void Copy(TObject & as) const;

  Int_t      fLastEventSerialNr;     // Serial number of last event
  Int_t      fLastEventNr;           // Number of last event
  Int_t      fCurrentFileIndex;      // Index of current file
  Int_t      fEvents;                //! nr. of events in the current file
  TString    fMode;                  // = 0 for READONLY, = 1 for READWRITE
  TObjArray* fFileNames;             // List of file names
  
  TString fEventFolderName; //Name of the folder where data for this stram will be mounted
  
  ClassDef(IlcStream,1)
};

#endif // ILCSTREAM_H
