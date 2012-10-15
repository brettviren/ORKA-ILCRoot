#ifndef ILCXMLCOLLECTION_H
#define ILCXMLCOLLECTION_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcXMLCollection.h 58023 2012-07-31 16:29:45Z hristov $ */

//-------------------------------------------------------------------------
//                          Class IlcXMLCollection
//   This is the class that creates XML collections after querying the tags
//
//    Origin: Panos Christakoglou, UOA-CERN, Panos.Christakoglou@cern.ch
//-------------------------------------------------------------------------

#include <TObject.h>
#include "TGridCollection.h"
#include <Riostream.h>
#include <TString.h>

class TMap;
class TIter;
class TEntryList;

using std::ofstream;
//___________________________________________________________________________
class IlcXMLCollection : public TGridCollection {
 
 public:
  IlcXMLCollection();
  IlcXMLCollection(const char *localCollectionFile);
  IlcXMLCollection(const IlcXMLCollection& collection);
  
  virtual ~IlcXMLCollection();
  
  //____________________________________________________//
  Bool_t WriteHeader();
  Bool_t WriteSummary(Int_t aTotal, Int_t aAccepted, Int_t aRejRun, Int_t aRejLHC, Int_t aRejDet, Int_t aRejEvt);
  Bool_t WriteBody(Int_t counter, const char* guid, const char *lfn, const char *turl, TEntryList *fEntryList);
  Bool_t WriteBody(Int_t counter, const char* guid, const char *lfn, const char *turl, TEntryList *fEntryListm, Int_t accSum, Int_t rejSum);
  Bool_t Export();

  void SetCollectionName(const char* name) {fCollectionName = name;}
  
  //____________________________________________________//
  const char* GetCollectionName() const {return fCollectionName.Data();}

  //____________________________________________________//
  void        Reset();
  TMap       *Next();
  Bool_t      Remove(TMap *map);
  const char *GetTURL(const char *name);
  const char *GetLFN(const char *name);
  const char *GetGUID(const char *name);
  const char *GetCutSumm();
  TEntryList *GetEventList(const char *filename) const;
  TEntryList *GetEntryList(const char *filename) { return GetEventList(filename); }
  Bool_t      OverlapCollection(TGridCollection * comparator);
  Bool_t      GetCollectionSummary(Int_t  *aTot, Int_t  *aAcc, Int_t  *aRejRun, Int_t  *aRejLHC, Int_t  *aRejDet, Int_t  *aRejEvt) const;

  static IlcXMLCollection *Open(const char *localcollectionfile);

  //____________________________________________________//
 protected:
  virtual void ParseXML();

  TString  fXmlFile;        // collection XML file
  TList   *fEventList;      // list with event file maps
  TIter   *fEventListIter;  // event file list iterator
  TMap    *fCurrent;        // current event file map
  TString  fCollectionName;   //the name of the xml file
  ofstream fout; // The output stream

  // List summary information
  Int_t    fTotalEvents;    // Total no. of events in collection
  Int_t    fAcceptedEvents; // No. of accepted events in collection
  Int_t    fRejectedRun;    // No. of events rejected by Run cuts
  Int_t    fRejectedLHC;    // No. of events rejected by LHC cuts
  Int_t    fRejectedDet;    // No. of events rejected by Detector cuts
  Int_t    fRejectedEvt;    // No. of events rejected by Event cuts
  
  IlcXMLCollection & operator=(const IlcXMLCollection & ) {return *this;}

  ClassDef(IlcXMLCollection,0)  //(ClassName, ClassVersion)
};
//___________________________________________________________________________

#endif
