#ifndef ILCAODTAGCREATOR_H
#define ILCAODTAGCREATOR_H
/*  See cxx source for full Copyright notice */


/* $Id: IlcAODTagCreator.h 50615 2011-07-16 23:19:19Z hristov $ */

//-------------------------------------------------------------------------
//                          Class IlcAODTagCreator
//   This is the IlcAODTagCreator class for the tag creation (post process)
//
//    Origin: Panos Christakoglou, UOA-CERN, Panos.Christakoglou@cern.ch
//-------------------------------------------------------------------------



//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                        IlcAODTagCreator                              //
//                                                                      //
//           Implementation of the tag creation mechanism.              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//ROOT
//#include <TObject.h>

#include <IlcTagCreator.h>

class TChain;
class TTree;
class TFile;
class TGridResult;

class IlcAODEvent;
class IlcRunTag;
class IlcEventTag;
class IlcFileTag;

//___________________________________________________________________________
class IlcAODTagCreator : public IlcTagCreator {

 public:
  IlcAODTagCreator();
  ~IlcAODTagCreator(); 

  void CreateAODTags(Int_t fFirstEvent, Int_t fLastEvent, TList *grpList);

  Bool_t ReadGridCollection(TGridResult *result);
  Bool_t ReadLocalCollection(const char *localpath, const char* pattern = "IlcAOD.root");
  Bool_t ReadCAFCollection(const char *filename);
  void FillEventTag(IlcAODEvent* aod, IlcEventTag* evtTag);  
 protected:  
  void CreateTag(TChain *chain, const char* type);
  void CreateTags(const char* type = "");

  
 private:
  IlcAODTagCreator(const IlcAODTagCreator& creator);             
  IlcAODTagCreator& operator=(const IlcAODTagCreator& creator);    
 private:
  TChain       *fChain;     //! Chain of esd files
  IlcAODEvent  *fAODEvent;  //! AOD Event 
  TTree        *fTreeT;     //! Tag Tree
  IlcRunTag    *fRunTag;    //! Run tag
  TChain       *fTreeTEsd;  //! ESD tag Tree
  IlcRunTag    *fRunTagEsd; //! ESD run tag
  
  ClassDef(IlcAODTagCreator,0)  
};

#endif

