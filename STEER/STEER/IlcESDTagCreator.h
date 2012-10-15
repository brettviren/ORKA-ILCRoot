#ifndef ILCESDTAGCREATOR_H
#define ILCESDTAGCREATOR_H
/*  See cxx source for full Copyright notice */


/* $Id: IlcESDTagCreator.h 50615 2011-07-16 23:19:19Z hristov $ */

//-------------------------------------------------------------------------
//                          Class IlcESDTagCreator
//   This is the IlcESDTagCreator class for the tag creation (post process)
//
//    Origin: Panos Christakoglou, UOA-CERN, Panos.Christakoglou@cern.ch
//-------------------------------------------------------------------------



//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                        IlcESDTagCreator                              //
//                                                                      //
//           Implementation of the tag creation mechanism.              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//ROOT
#include <TSystem.h>
class TChain;
class TList;
class TMap;

#include <IlcTagCreator.h>

class TFile;
class TGridResult;

class IlcGRPObject;
class IlcEventTag;

//___________________________________________________________________________
class IlcESDTagCreator : public IlcTagCreator {

 public:
  IlcESDTagCreator();
  ~IlcESDTagCreator(); 

  void CreateESDTags(Int_t fFirstEvent, Int_t fLastEvent, IlcGRPObject *grpData, ULong_t * qa, Bool_t * es, Int_t qalength, Int_t eslength);
  void CreateESDTagsFullRun(TTree *chain, IlcGRPObject *grpData, ULong_t * qa, Bool_t * es, Int_t qalength, Int_t eslength);
  void CreateESDRunTagSummary(TTree *chain);

  void SetInactiveBranches(const char* branches) {fBranches = branches;}

  Bool_t ReadGridCollection(TGridResult *result);
  Bool_t ReadLocalCollection(const char *localpath);
  Bool_t ReadCAFCollection(const char *filename);
  
 protected:  
  void CreateTag(TChain *chain, const char* type);
  void CreateTag(TFile* file, const char *guid, const char *md5, const char *turl, Long64_t size, Int_t Counter);
  void CreateTag(TFile* file, const char *filepath, Int_t Counter);

  void FillEventTag(TTree *chain, IlcEventTag *evTag, Int_t iEventNumber, IlcESDEvent *esd);

 private:
  void SwitchOffBranches() const;
  IlcESDTagCreator(const IlcESDTagCreator& creator);             
  IlcESDTagCreator& operator=(const IlcESDTagCreator& creator);
  
  TChain *fChain;     //chain of esd files
  TList  *fGUIDList;  //TList of guid TObjString
  TList  *fMD5List;   //TList of md5 TObjString
  TList  *fTURLList;  //TList of turl TObjString
  TString fBranches;  //List of branches to be switched off (separated by space
  MemInfo_t *meminfo; //mem info

  ClassDef(IlcESDTagCreator,0)  
};

#endif

