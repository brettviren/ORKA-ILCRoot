#ifndef ILCFILETAG_H
#define ILCFILETAG_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcLHCTag.h 14745 2006-08-04 15:48:44Z panos $ */

//-------------------------------------------------------------------------
//                          Class IlcFileTag
//   This is the class to deal with the tags for the file level
//
//    Origin: Adam Kisiel, CERN, Adam.Kisiel@cern.ch
//-------------------------------------------------------------------------

#include <TObject.h>
#include <TString.h>
#include "TObjArray.h"
#include "IlcEventTag.h"

//______________________________________________________________________________
class IlcFileTag : public TObject {
 public:
  IlcFileTag();
  IlcFileTag(const IlcFileTag &tag);
  virtual ~IlcFileTag();
  
  IlcFileTag &operator=(const IlcFileTag &tag);

  //____________________________________________________//  
  void SetGUID(TString Pid) { fGUID = Pid; }
  void SetPath(TString Pid) { fPath = Pid; }
  void SetMD5(TString Pid) {fmd5 = Pid; }
  void SetTURL(TString Pid) {fturl = Pid; }
  void SetSize(Long64_t i) {fsize = i; }
  void AddEventTag(const IlcEventTag &t);

  void CopyFileInfo(const IlcFileTag &tag);

  //____________________________________________________//
  const char *GetGUID() const {return fGUID.Data();}
  const char *GetPath() const {return fPath.Data();}
  const char *GetMD5() const {return fmd5.Data();}
  const char *GetTURL() const {return fturl.Data();}
  Long64_t    GetSize() const {return fsize;}
  Int_t       GetNEvents() const {return fEventTags.GetEntries(); }
  const IlcEventTag *GetEventTag(Int_t iev) const {return (const IlcEventTag *)fEventTags.At(iev);}

  //____________________________________________________//
 private:
  TString   fGUID;		            //The unique identifier of the file
  TString   fPath;		            //The file's path (local storage)
  Long64_t  fsize;                          //the size of the file
  TString   fmd5;                           //another file identifier
  TString   fturl;                          //the file's url
  TObjArray fEventTags;                     //array with all event tags
  
  ClassDef(IlcFileTag,1)   //(ClassName, ClassVersion)
};
//______________________________________________________________________________

#endif
