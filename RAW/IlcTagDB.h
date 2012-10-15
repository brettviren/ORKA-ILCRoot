#ifndef ILCTAGDB_H
#define ILCTAGDB_H
// @(#) $Id: IlcTagDB.h 23318 2008-01-14 12:43:28Z hristov $
// Author: Fons Rademakers  26/11/99

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcTagDB                                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TObject
#include <TObject.h>
#endif

#ifndef ROOT_TFile
#include <TFile.h>
#endif

#ifndef ROOT_TTree
#include <TTree.h>
#endif

#ifndef ROOT_TString
#include <TString.h>
#endif



// Forward class declarations
class IlcRawEventTag;


class IlcTagDB : public TObject {

public:
   IlcTagDB(IlcRawEventTag *eventTag, const char* fileName = NULL);
   virtual ~IlcTagDB() { Close(); }

   Bool_t          Create(const char* fileName = NULL);
   virtual void    Close();
   void            Fill() { fTree->Fill(); }
   Bool_t          FileFull()
            { return (fMaxSize >= 0) ? ((fTagDB->GetBytesWritten() > fMaxSize) ? kTRUE : kFALSE) : kFALSE; }

   Bool_t          NextFile(const char* fileName = NULL);

   void            SetMaxSize(Double_t maxSize) { fMaxSize = maxSize; }
   void            SetFS(const char* fs);

   Double_t           GetBytesWritten() const { return fTagDB->GetBytesWritten(); }
   TFile             *GetDB() const { return fTagDB; }
   const char        *GetDBName() const { return fTagDB->GetName(); }
   IlcRawEventTag    *GetEventTag() const { return fEventTag; }
   Int_t              GetEvents() const { return (Int_t) fTree->GetEntries(); }
   Float_t            GetCompressionFactor() const;

protected:
   TFile             *fTagDB;     // DB to store header information only (tag)
   TTree             *fTree;      // tree use to store header
   IlcRawEventTag    *fEventTag;  // pointer to event tag object via which data is stored
   Double_t           fMaxSize;   // maximum size in bytes of tag DB
   TString            fFS;        // tag DB file system location
   Bool_t             fDeleteFiles; // flag for deletion of files

   virtual const char *GetFileName() const;

private:
   IlcTagDB(const IlcTagDB& tagDB);
   IlcTagDB& operator = (const IlcTagDB& tagDB);

   ClassDef(IlcTagDB,0)  // Tag DB
};

#endif
