#ifndef ILCFILEMERGER_H
#define ILCFILEMERGER_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//////////////////////////////////////////////////////////////////////////
//
//  Utilities for file merging
//
//////////////////////////////////////////////////////////////////////////

class TObjString;

#include "TNamed.h"

class IlcFileMerger : public TNamed
{

 public:
  IlcFileMerger();
  IlcFileMerger(const char* name);
  void Merge(TFile* fileIn, TObjArray * array);

  void IterTXT( const char * fileList,  const char* outputFileName,Bool_t dontOverwrite=kFALSE);
  void IterAlien(const char* outputDir, const char* outputFileName = "CalibObjects.root" , const char* pattern = "IlcESDfriends_v1.root", Bool_t dontOverwrite=kFALSE);
  void IterList(const TList* namesList, const char* outputFileName, Bool_t dontOverwrite=kFALSE);
  //
  void StoreResults(TObjArray * array, const char* outputFileName);
  void StoreSeparateResults(TObjArray * array, const char* outputFileName);
  //
  Bool_t IsAccepted(TString name);
  void AddReject(const char *reject);
  void AddAccept(const char *accept);
  void SetNoTrees(Bool_t v=kTRUE)        {fNoTrees = v;}
  Bool_t IsNoTrees()               const {return fNoTrees;}
  void SetMaxFilesOpen(Int_t n)          {fMaxFilesOpen = n<3 ? 3 : n;}
  Int_t GetMaxFilesOpen()          const {return fMaxFilesOpen;}
protected:
  int AddFile(TList* sourcelist, std::string entry);
  int MergeRootfile( TDirectory *target, TList *sourceNames);
  int OpenNextChunks(const TList* namesList, TList* filesList, Int_t from, Int_t to);
protected:
  TObjArray * fRejectMask;  // mask of the objects to be rejected
  TObjArray * fAcceptMask;    // mask of the objects to be accepted
  Int_t       fMaxFilesOpen;  // max number of open files
  Bool_t      fNoTrees;       // do we merge trees
private:
  IlcFileMerger(const IlcFileMerger&);
  IlcFileMerger& operator=(const IlcFileMerger& other);
  
  ClassDef(IlcFileMerger, 2); // File merger utilities for IlcRoot
};

#endif
