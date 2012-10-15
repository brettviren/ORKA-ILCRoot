#ifndef ILCTAGANALYSIS_H
#define ILCTAGANALYSIS_H
/*  See cxx source for full Copyright notice */


/* $Id: IlcTagAnalysis.h 38128 2010-01-06 14:44:54Z hristov $ */

//-------------------------------------------------------------------------
//                          Class IlcTagAnalysis
//   This is the IlcTagAnalysis class for the tag analysis
//
//    Origin: Panos Christakoglou, UOA-CERN, Panos.Christakoglou@cern.ch
//-------------------------------------------------------------------------



//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                        IlcTagAnalysis                                //
//                                                                      //
//           Implementation of the tag analysis mechanism.              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//ROOT
#include <TObject.h>
#include <TString.h>
#include <TEntryList.h>

class IlcEventTag;
class TChain;
class IlcEventTagCuts;
class IlcDetectorTagCuts;
class IlcLHCTagCuts;
class IlcRunTagCuts;
class TGridResult;
class TTreeFormula;

//____________________________________________________//
class IlcTagAnalysis : public TObject {
 public:
  IlcTagAnalysis();
  IlcTagAnalysis(const char* type);
  ~IlcTagAnalysis(); 
  
  void SetType(const char* type) {fAnalysisType = type;}
  const char* GetType() {return fAnalysisType.Data();}
  Bool_t AddTagsFile(const char *alienUrl, Bool_t checkFile=kTRUE);
  void ChainLocalTags(const char *dirname);
  TChain *ChainGridTags(TGridResult *result);
  
  TChain *QueryTags(IlcRunTagCuts *runTagCuts, 
		    IlcLHCTagCuts *lhcTagCuts, 
		    IlcDetectorTagCuts *detTagCuts, 
		    IlcEventTagCuts *evTagCuts);
  TChain *QueryTags(const char *fRunCut, 
		    const char *fLHCCut, 
		    const char *fDetectorCut, 
		    const char *fEventCut);  

  Bool_t CreateXMLCollection(const char* name, 
			     IlcRunTagCuts *runTagCuts, 
			     IlcLHCTagCuts *lhcTagCuts, 
			     IlcDetectorTagCuts *detTagCuts, 
			     IlcEventTagCuts *evTagCuts);
  Bool_t CreateXMLCollection(const char* name, 
			     const char *fRunCut, 
			     const char *fLHCCut, 
			     const char *fDetectorCut, 
			     const char *fEventCut);

  TChain *GetInputChain(const char* system, const char *wn);
  
  static TChain* CreateChainFromCollection(const char* collectionname, const char* treename);
  
  TEntryList *GetGlobalList() {return fGlobalList;}
  //____________________________________________________//
 protected:
  TGridResult *ftagresult;    //the results from the tag grid query     
  TString      fTagDirName;   //the location of the locally stored tags
  TChain      *fChain;        //tag chain 
  TString      fAnalysisType; //define the type of analysis (esd or aod)
  TEntryList  *fGlobalList;   //global TEntryList
  
  //____________________________________________________//
 private:
  IlcTagAnalysis(const IlcTagAnalysis & source);
  IlcTagAnalysis & operator=(const IlcTagAnalysis & source);
       
  ClassDef(IlcTagAnalysis,0)  
};

#endif

