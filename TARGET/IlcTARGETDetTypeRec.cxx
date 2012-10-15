/***************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
 * Conributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/*
 $Id: IlcTARGETDetTypeRec.cxx,v 1.4 2008/11/06 23:57:35 cgatto Exp $
*/

////////////////////////////////////////////////////////////////////////
// This class defines the "Standard" reconstruction for the TARGET       // 
// detector.                                                          //
//                                                                    //
////////////////////////////////////////////////////////////////////////
#include "TObjArray.h"
#include "TTree.h"

#include "IlcCDBManager.h"
#include "IlcCDBEntry.h"
#include "IlcTARGETClusterFinder.h"
#include "IlcTARGETClusterFinderV2.h"
#include "IlcTARGETClusterFinderV2SPD.h"
#include "IlcTARGETClusterFinderV2SDD.h"
#include "IlcTARGETClusterFinderV2SSD.h"
#include "IlcTARGETClusterFinderSPD.h"
#include "IlcTARGETClusterFinderSDD.h"
#include "IlcTARGETClusterFinderSSD.h"
#include "IlcTARGETDetTypeRec.h"
#include "IlcTARGETParam.h"
#include "IlcTARGETRawCluster.h"
#include "IlcTARGETRawClusterSPD.h"
#include "IlcTARGETRawClusterSDD.h"
#include "IlcTARGETRawClusterSSD.h"
#include "IlcTARGETRecPoint.h"
#include "IlcTARGETCalibrationSDD.h"
#include "IlcTARGETsegmentationSPD.h"
#include "IlcTARGETsegmentationSPD2.h"
#include "IlcTARGETsegmentationSDD.h"
#include "IlcTARGETsegmentationSSD.h"
#include "IlcLog.h"


const Int_t IlcTARGETDetTypeRec::fgkNdettypes = 2;
const Int_t IlcTARGETDetTypeRec::fgkDefaultNModulesSPD = 192;
const Int_t IlcTARGETDetTypeRec::fgkDefaultNModulesSDD =   0;
const Int_t IlcTARGETDetTypeRec::fgkDefaultNModulesSSD =   (16*7)+(36*15)+(54*23)+(72*29)+(90*37)+(12*14);



ClassImp(IlcTARGETDetTypeRec)

//________________________________________________________________
IlcTARGETDetTypeRec::IlcTARGETDetTypeRec(): TObject(){
    // Default Constructor
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //    A properly zero-ed IlcTARGETDetTypeRec class.
  fReconstruction = 0;
  fSegmentation = 0;
fSegmentationSPD=0;

  fCalibration = 0;
  fPreProcess = 0;
  fPostProcess = 0;
  fDigits = 0;;
  for(Int_t i=0; i<3; i++){
    fClusterClassName[i]=0;
    fDigClassName[i]=0;
    fRecPointClassName[i]=0;
  }
  fNdtype = 0;
  fCtype = 0;
  fNMod = 0;
  fNctype = 0;
  fRecPoints = 0;
  fNRecPoints = 0;
  SelectVertexer(" ");
  fLoader = 0;
  fRunNumber = 0;
  fFirstcall = kTRUE;

}
//________________________________________________________________
IlcTARGETDetTypeRec::IlcTARGETDetTypeRec(IlcTARGETLoader *loader): TObject(){
    // Standard Constructor
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //   

  fReconstruction = new TObjArray(fgkNdettypes);
  fSegmentation = 0;
  fSegmentationSPD = 0;

  fCalibration = 0;
  fPreProcess = 0;
  fPostProcess = 0;
  fDigits = new TObjArray(fgkNdettypes);
  for(Int_t i=0; i<3; i++){
    fClusterClassName[i]=0;
    fDigClassName[i]=0;
    fRecPointClassName[i]=0;
  }

  fNdtype = new Int_t[fgkNdettypes];
  fCtype = new TObjArray(fgkNdettypes);
  fNctype = new Int_t[fgkNdettypes];
  fNMod = new Int_t [fgkNdettypes];
  fNMod[0]=fgkDefaultNModulesSPD;
  
  fNMod[1] = fgkDefaultNModulesSSD;
  fNMod[2] = fgkDefaultNModulesSDD;
  fRecPoints = new TClonesArray("IlcTARGETRecPoint",3000);
  fNRecPoints = 0;
  
  for(Int_t i=0;i<fgkNdettypes;i++){
    fNdtype[i]=0;
    fNctype[i]=0;
  }
  
  SelectVertexer(" ");
  fLoader = loader;
 
  SetRunNumber();
  fFirstcall = kTRUE;
 }
//______________________________________________________________________
IlcTARGETDetTypeRec::IlcTARGETDetTypeRec(const IlcTARGETDetTypeRec &/*rec*/):TObject(/*rec*/){
    // Copy constructor. 

  Error("Copy constructor","Copy constructor not allowed");
  
}
//______________________________________________________________________
IlcTARGETDetTypeRec& IlcTARGETDetTypeRec::operator=(const IlcTARGETDetTypeRec& /*source*/){
    // Assignment operator. This is a function which is not allowed to be
    // done.
    Error("operator=","Assignment operator not allowed\n");
    return *this; 
}

//_____________________________________________________________________
IlcTARGETDetTypeRec::~IlcTARGETDetTypeRec(){
 
  //Destructor
 
  if(fReconstruction){
    fReconstruction->Delete();
    delete fReconstruction;
    fReconstruction = 0;
  }
  if(fSegmentation){
    fSegmentation->Delete();
    delete fSegmentation;
    fSegmentation = 0;
  }
  if(fSegmentationSPD){
    fSegmentationSPD->Delete();
    delete fSegmentationSPD;
    fSegmentationSPD = 0;
  }

  if(fCalibration && fRunNumber<0){
    IlcTARGETresponse* rspd = ((IlcTARGETCalibration*)fCalibration->UncheckedAt(GetTARGETgeom()->GetStartSPD()))->GetResponse();    
    IlcTARGETresponse* rsdd = ((IlcTARGETCalibration*)fCalibration->UncheckedAt(GetTARGETgeom()->GetStartSSD()))->GetResponse();
    IlcTARGETresponse* rssd = ((IlcTARGETCalibration*)fCalibration->UncheckedAt(GetTARGETgeom()->GetStartSDD()))->GetResponse();
    if(rspd) delete rspd;
    if(rsdd) delete rsdd;
    if(rssd) delete rssd;
    fCalibration->Delete();
    delete fCalibration;
    fCalibration = 0;
  }
  if(fPreProcess) delete fPreProcess;
  if(fPostProcess) delete fPostProcess;

  if(fDigits){
    fDigits->Delete();
    delete fDigits;
    fDigits=0;
  }
  if(fRecPoints){
    fRecPoints->Delete();
    delete fRecPoints;
    fRecPoints=0;
  }
  if(fCtype) {
    fCtype->Delete();
    delete fCtype;
    fCtype = 0;
  }
  delete [] fNctype;
  delete [] fNdtype;
  delete [] fNMod;
  if(fLoader) delete fLoader;
  
}

//___________________________________________________________________
void IlcTARGETDetTypeRec::SetReconstructionModel(Int_t dettype,IlcTARGETClusterFinder *clf){

  //Set reconstruction model for detector type

  if(fReconstruction==0) fReconstruction = new TObjArray(fgkNdettypes);
  if(fReconstruction->UncheckedAt(dettype)!=0) delete fReconstruction->UncheckedAt(dettype);
  fReconstruction->AddAt(clf,dettype);
}
//______________________________________________________________________
IlcTARGETClusterFinder* IlcTARGETDetTypeRec::GetReconstructionModel(Int_t dettype){

  //Get reconstruction model for detector type
  if(fReconstruction==0)  {
    Warning("GetReconstructionModel","fReconstruction is 0!");
    return 0;     
  }
  return (IlcTARGETClusterFinder*)fReconstruction->UncheckedAt(dettype);
}

//______________________________________________________________________
void IlcTARGETDetTypeRec::SetSegmentationModel(Int_t dettype,IlcTARGETsegmentation *seg){
   
  //Set segmentation model for detector type
  
  if(fSegmentation==0) fSegmentation = new TObjArray(fgkNdettypes);
  if(fSegmentation->UncheckedAt(dettype)!=0) delete fSegmentation->UncheckedAt(dettype);
  fSegmentation->AddAt(seg,dettype);

}
//______________________________________________________________________
//______________________________________________________________________
void IlcTARGETDetTypeRec::SetSegmentationModelSPD(Int_t dettype,IlcTARGETsegmentation2 *seg){
   
  //Set segmentation model for detector type
  if(fSegmentationSPD==0) fSegmentationSPD = new TObjArray(fgkNdettypes);
  if(fSegmentationSPD->UncheckedAt(dettype)!=0) delete fSegmentationSPD->UncheckedAt(dettype);
  fSegmentationSPD->AddAt(seg,dettype);

}
//______________________________________________________________________
//______________________________________________________________________
IlcTARGETsegmentation2* IlcTARGETDetTypeRec::GetSegmentationModelSPD(Int_t dettype){

  //Get segmentation model for detector type
   
   if(fSegmentationSPD==0) {
     Warning("SegmentationModelSPD","fSegmentation is 0!");
     return 0; 
   } 
   return (IlcTARGETsegmentation2*)(fSegmentationSPD->UncheckedAt(dettype));

}
//_______________________________________________________________________

IlcTARGETsegmentation* IlcTARGETDetTypeRec::GetSegmentationModel(Int_t dettype){

  //Get segmentation model for detector type
   
   if(fSegmentation==0) {
     Warning("GetSegmentationModel","fSegmentation is 0!");
     return 0; 
   } 
   return (IlcTARGETsegmentation*)fSegmentation->UncheckedAt(dettype);

}
//_______________________________________________________________________
void IlcTARGETDetTypeRec::SetCalibrationModel(Int_t iMod, IlcTARGETCalibration *cal){

  //Set calibration (response) for the module iMod of type dettype
  if (fCalibration==0) {
    fCalibration = new TObjArray(GetTARGETgeom()->GetIndexMax());
    fCalibration->SetOwner(kTRUE);
    fCalibration->Clear();
  }

  if (fCalibration->UncheckedAt(iMod) != 0)
    delete (IlcTARGETCalibration*) fCalibration->UncheckedAt(iMod);
  fCalibration->AddAt(cal,iMod);

}
//_______________________________________________________________________
IlcTARGETCalibration* IlcTARGETDetTypeRec::GetCalibrationModel(Int_t iMod){
  
  //Get calibration model for module type
  
  if(fCalibration==0) {
    Warning("GetilcbrationModel","fCalibration is 0!");
    return 0; 
  }  

  return (IlcTARGETCalibration*)fCalibration->UncheckedAt(iMod);
}

//______________________________________________________________________
void IlcTARGETDetTypeRec::SetTreeAddress(){
    // Set branch address for the Trees.
 
  TTree *treeR = fLoader->TreeR();
  TTree *treeD = fLoader->TreeD();
 
  SetTreeAddressD(treeD);
  SetTreeAddressR(treeR);
}
//______________________________________________________________________
void IlcTARGETDetTypeRec::SetTreeAddressD(TTree *treeD){
    // Set branch address for the tree of digits.

    const char *det[4] = {"SPD","SSD","SDD","TARGET"};
    TBranch *branch;
    Char_t* digclass;
    Int_t i;
    char branchname[30];

    if(!treeD) return;
    if (fDigits == 0x0) fDigits = new TObjArray(fgkNdettypes);
    for (i=0; i<fgkNdettypes; i++) {
        digclass = GetDigitClassName(i);
        if(!(fDigits->UncheckedAt(i))) {
            fDigits->AddAt(new TClonesArray(digclass,1000),i);
        }else{
            ResetDigits(i);
        } 
        if (fgkNdettypes==3) sprintf(branchname,"%sDigits%s",det[3],det[i]);
        else  sprintf(branchname,"%sDigits%d",det[3],i+1);
        if (fDigits) {
            branch = treeD->GetBranch(branchname);
            //dan-commento cout <<"Settata la branch: "<<branchname<<endl;
            if (branch) branch->SetAddress(&((*fDigits)[i]));
        } 
    } 
}

//_______________________________________________________________________
TBranch* IlcTARGETDetTypeRec::MakeBranchInTree(TTree *tree, const char* name, 
                                       const char *classname, 
                                       void* address,Int_t size, 
                                       Int_t splitlevel, const char */*file*/)
{ 
//
// Makes branch in given tree and diverts them to a separate file
// 
//
//
    
  if (tree == 0x0) {
    Error("MakeBranchInTree","Making Branch %s Tree is NULL",name);
    return 0x0;
  }
  TBranch *branch = tree->GetBranch(name);
  if (branch) {  
    return branch;
  }
  if (classname){
    branch = tree->Branch(name,classname,address,size,splitlevel);
  }
  else {
    branch = tree->Bronch(name, "TClonesArray", address, size, splitlevel);
  }
  
  return branch;
}

//____________________________________________________________________
void IlcTARGETDetTypeRec::SetDefaults(){
  
  //Set defaults for segmentation and response

  if(!GetTARGETgeom()){
    Warning("SetDefaults","null pointer to IlcTARGETgeom !");
    return;
  }
  IlcTARGETParam *Param = new IlcTARGETParam(GetTARGETgeom()->GetDetVersion());
//   if (GetTARGETgeom()->GetDetVersion() == 3)   Param=new IlcTARGETParam(GetTARGETgeom()->GetDetVersion());
//     else if (GetTARGETgeom()->GetDetVersion() == 2) Param=new IlcTARGETParam(GetTARGETgeom()->GetDetVersion());
//       else Param=new IlcTARGETParam(GetTARGETgeom()->GetDetVersion());
  
 IlcTARGETsegmentation* seg;
  if(!GetCalibration()) {IlcFatal("Exit");exit(0);}  
  IlcTARGETsegmentation2* seg2;
  if(!GetCalibration()) {IlcFatal("Exit");exit(0);}  

  for(Int_t dettype=0;dettype<fgkNdettypes;dettype++){
    if(dettype==0){
      seg2 = new IlcTARGETsegmentationSPD2(GetTARGETgeom());
      SetSegmentationModelSPD(dettype,seg2);
      SetDigitClassName(dettype,"IlcTARGETdigitSPD");
      SetClusterClassName(dettype,"IlcTARGETRawClusterSPD");

    }
    if(dettype==1){
      IlcTARGETsegmentationSSD *seg22 = new IlcTARGETsegmentationSSD(GetTARGETgeom());

      for (Int_t lay=0; lay<Param->GetNlayersStrip()+Param->GetNendcapsStrip();lay++) seg22->SetAngles(lay,Param->GetStereoP(lay+Param->GetNlayers()+Param->GetNendcaps()),Param->GetStereoN(lay+Param->GetNlayers()+Param->GetNendcaps())); // strip angels rad P and N side.
      SetSegmentationModelSPD(dettype,seg22);
      SetDigitClassName(dettype,"IlcTARGETdigitSSD");
      SetClusterClassName(dettype,"IlcTARGETRawClusterSSD");

    }

    if(dettype==2){
      seg2 = new IlcTARGETsegmentationSDD(GetTARGETgeom(),0);
      //seg2->SetAnglesLay5(0.0075,0.0275); // strip angels rad P and N side.
      //seg2->SetAnglesLay6(0.0275,0.0075); // strip angels rad P and N side.
      SetSegmentationModelSPD(dettype,seg2);
      SetDigitClassName(dettype,"IlcTARGETdigitSDD");
      SetClusterClassName(dettype,"IlcTARGETRawClusterSDD");
    }

  }
  delete Param;
}
//______________________________________________________________________
Bool_t IlcTARGETDetTypeRec::GetCalibration() {
  // Get Default calibration if a storage is not defined.
  
IlcTARGETParam *Param = new IlcTARGETParam(GetTARGETgeom()->GetDetVersion());
//  if (GetTARGETgeom()->GetDetVersion() == 3) Param=new IlcTARGETParam(GetTARGETgeom()->GetDetVersion());
//     else if (GetTARGETgeom()->GetDetVersion() == 2) Param=new IlcTARGETParam(GetTARGETgeom()->GetDetVersion());
//       else Param=new IlcTARGETParam(GetTARGETgeom()->GetDetVersion());

//  fNMod[0] = fgkDefaultNModulesSPD;
  fNMod[0] = Param->GetNmodulesSPD();
  
  return kTRUE;

  if(!fFirstcall){
    IlcTARGETCalibration* cal = GetCalibrationModel(0);
    if(cal)return kTRUE;
  }
  else {
    fFirstcall = kFALSE;
  }

  SetRunNumber((Int_t)IlcCDBManager::Instance()->GetRun());
  Int_t run=GetRunNumber();
  if(run<0)run=0;   // if the run number is not yet set, use fake run # 0

  Bool_t origCacheStatus = IlcCDBManager::Instance()->GetCacheFlag();
  Bool_t isCacheActive = kTRUE;
  if(GetRunNumber()<0)isCacheActive=kFALSE;
  if (fCalibration==0) {
    fCalibration = new TObjArray(GetTARGETgeom()->GetIndexMax());
    fCalibration->SetOwner(isCacheActive);
    fCalibration->Clear();
  }

  IlcCDBManager::Instance()->SetCacheFlag(isCacheActive);

  IlcCDBEntry *entrySPD = IlcCDBManager::Instance()->Get("TARGET/Calib/CalibSPD", run);
  IlcCDBEntry *entrySDD = IlcCDBManager::Instance()->Get("TARGET/Calib/CalibSDD", run);
  IlcCDBEntry *entrySSD = IlcCDBManager::Instance()->Get("TARGET/Calib/CalibSSD", run);
  IlcCDBEntry *entry2SPD = IlcCDBManager::Instance()->Get("TARGET/Calib/RespSPD", run);
  IlcCDBEntry *entry2SDD = IlcCDBManager::Instance()->Get("TARGET/Calib/RespSDD", run);
  IlcCDBEntry *entry2SSD = IlcCDBManager::Instance()->Get("TARGET/Calib/RespSSD", run);

  if(!entrySPD || !entrySDD || !entrySSD || !entry2SPD || !entry2SDD || !entry2SSD){
  	IlcWarning("Calibration object retrieval failed! Dummy calibration will be used.");
	IlcCDBStorage *origStorage = IlcCDBManager::Instance()->GetDefaultStorage();
	IlcCDBManager::Instance()->SetDefaultStorage("local://$ILC_ROOT");
	
  	entrySPD = IlcCDBManager::Instance()->Get("TARGET/Calib/CalibSPD", run);
  	entrySDD = IlcCDBManager::Instance()->Get("TARGET/Calib/CalibSDD", run);
  	entrySSD = IlcCDBManager::Instance()->Get("TARGET/Calib/CalibSSD", run);
 	entry2SPD = IlcCDBManager::Instance()->Get("TARGET/Calib/RespSPD", run);
  	entry2SDD = IlcCDBManager::Instance()->Get("TARGET/Calib/RespSDD", run);
  	entry2SSD = IlcCDBManager::Instance()->Get("TARGET/Calib/RespSSD", run);
	
	IlcCDBManager::Instance()->SetDefaultStorage(origStorage);
  }

 
  TObjArray *calSPD = (TObjArray *)entrySPD->GetObject();
  if(!isCacheActive)entrySPD->SetObject(NULL);
  entrySPD->SetOwner(kTRUE);
 
  IlcTARGETresponseSPD *pSPD = (IlcTARGETresponseSPD*)entry2SPD->GetObject();
  if(!isCacheActive)entry2SPD->SetObject(NULL);
  entry2SPD->SetOwner(kTRUE);
    
  TObjArray *calSDD = (TObjArray *)entrySDD->GetObject();
  if(!isCacheActive)entrySDD->SetObject(NULL);
  entrySDD->SetOwner(kTRUE);
 
  IlcTARGETresponseSDD *pSDD = (IlcTARGETresponseSDD*)entry2SDD->GetObject();
  if(!isCacheActive)entry2SDD->SetObject(NULL);
  entry2SDD->SetOwner(kTRUE);

  TObjArray *calSSD = (TObjArray *)entrySSD->GetObject();
  if(!isCacheActive)entrySSD->SetObject(NULL);
  entrySSD->SetOwner(kTRUE);

  IlcTARGETresponseSSD *pSSD = (IlcTARGETresponseSSD*)entry2SSD->GetObject();
  if(!isCacheActive)entry2SSD->SetObject(NULL);
  entry2SSD->SetOwner(kTRUE);

  // DB entries are deleted. In this way metadeta objects are deleted as well
  if(!isCacheActive){
    delete entrySPD;
    delete entrySDD;
    delete entrySSD;
    delete entry2SPD;
    delete entry2SDD;
    delete entry2SSD;
  }

  IlcCDBManager::Instance()->SetCacheFlag(origCacheStatus);
  
  if ((!pSPD)||(!pSDD)||(!pSSD) || (!calSPD) || (!calSDD) || (!calSSD)) {
    IlcWarning("Can not get calibration from calibration database !");
    return kFALSE;
  }
// fNMod[0] = Param->GetNmodulesSPD();192;//8752;//192;
  fNMod[0] = Param->GetNmodulesSPD();


  fNMod[1] = 7504;
  fNMod[2] = 0;
  IlcInfo(Form("%i SPD, %i SSD and %i SDD in calibration database",
	       fNMod[0], fNMod[1], fNMod[2]));
  IlcTARGETCalibration* cal;
  for (Int_t i=0; i<fNMod[0]; i++) {
    cal = (IlcTARGETCalibration*) calSPD->UncheckedAt(i);
    cal->SetResponse((IlcTARGETresponse*)pSPD);
    SetCalibrationModel(i, cal);
 }
  for (Int_t i=0; i<fNMod[1]; i++) {
    cal = (IlcTARGETCalibration*) calSDD->UncheckedAt(i);
    cal->SetResponse((IlcTARGETresponse*)pSDD);
    Int_t iMod = i + fNMod[0];
    SetCalibrationModel(iMod, cal);
 }
  for (Int_t i=0; i<fNMod[2]; i++) {
    cal = (IlcTARGETCalibration*) calSSD->UncheckedAt(i);
    cal->SetResponse((IlcTARGETresponse*)pSSD);
    Int_t iMod = i + fNMod[0] + fNMod[1];
    SetCalibrationModel(iMod, cal);
 }
delete Param;
  return kTRUE;
}


//________________________________________________________________
void IlcTARGETDetTypeRec::SetDefaultClusterFinders(){
  
  //set defaults for standard cluster finder

  if(!GetTARGETgeom()){
    Warning("SetDefaults","null pointer to IlcTARGETgeom!");
    return;
  }

  IlcTARGETClusterFinder *clf; 

  MakeTreeC();
 
 for(Int_t dettype=0;dettype<fgkNdettypes;dettype++){
    //SPD
    if(dettype==0){
      if(!GetReconstructionModel(dettype)){
	TClonesArray *dig0 = DigitsAddress(0);
	TClonesArray *rec0 = ClustersAddress(0);
	clf = new IlcTARGETClusterFinderSPD(this,dig0,rec0);
	SetReconstructionModel(dettype,clf);

      }
    }
    //SSD
    if(dettype==1){
      if(!GetReconstructionModel(dettype)){
	TClonesArray* dig2 = DigitsAddress(1);
	clf = new IlcTARGETClusterFinderSSD(this,dig2);
	SetReconstructionModel(dettype,clf);
      }
    }


   
    //SDD
    if(dettype==2){
      if(!GetReconstructionModel(dettype)){
	TClonesArray *dig1 = DigitsAddress(2);
	TClonesArray *rec1 = ClustersAddress(2);
	clf = new IlcTARGETClusterFinderSDD(this,dig1,rec1);
	SetReconstructionModel(dettype,clf);
      }

    }
 }
 
  
}

//________________________________________________________________
void IlcTARGETDetTypeRec::SetDefaultClusterFindersV2(Bool_t rawdata){

  //Set defaults for cluster finder V2

  if(!GetTARGETgeom()){
    Warning("SetDefaults","Null pointer to IlcTARGETgeom !");
    return;
  }

  IlcTARGETClusterFinder *clf; 

  MakeTreeC();
  for(Int_t dettype=0;dettype<fgkNdettypes;dettype++){
    //SPD
    if(dettype==0){
      if(!GetReconstructionModel(dettype)){
	clf = new IlcTARGETClusterFinderV2SPD(this);
	clf->InitGeometry();
	if(!rawdata) clf->SetDigits(DigitsAddress(0));
	SetReconstructionModel(dettype,clf);

      }
    }

    //SSD
    if(dettype==1){
      if(!GetReconstructionModel(dettype)){
      //dan-commento cout <<"chiama qui la ricostruzione"<<endl;
	clf = new IlcTARGETClusterFinderV2SSD(this);
	clf->InitGeometry();
	if(!rawdata) clf->SetDigits(DigitsAddress(1));
	SetReconstructionModel(dettype,clf);
      }
    }

    //SDD
    if(dettype==2){
      if(!GetReconstructionModel(dettype)){
	clf = new IlcTARGETClusterFinderV2SDD(this);
	clf->InitGeometry();
	if(!rawdata) clf->SetDigits(DigitsAddress(2));
	SetReconstructionModel(dettype,clf);
      }

    }


 }
   
}
//______________________________________________________________________
void IlcTARGETDetTypeRec::MakeBranch(Option_t* option){

  //Creates branches for clusters and recpoints
  Bool_t cR = (strstr(option,"R")!=0);
  Bool_t cRF = (strstr(option,"RF")!=0);
  
  if(cRF)cR = kFALSE;

  if(cR) MakeBranchR(0);
  if(cRF) MakeBranchRF(0);

}

//_____________________________________________________________
void IlcTARGETDetTypeRec::MakeTreeC(){
  
  //Create a separate tree to store the clusters
  if(!fLoader){
    Warning("MakeTreeC","TARGET loader is null!");
    return;
  }
  if(fLoader->TreeC()== 0x0) fLoader->MakeTree("C");
  MakeBranchC();
}

//______________________________________________________________
void IlcTARGETDetTypeRec::MakeBranchC(){
  
  //Make branches in the tree of clusters

  if(!fLoader){
    Warning("MakeBranchC","TARGET loader is null!");
    return;
  }
  TTree* lTC = fLoader->TreeC();
  if(lTC==0x0){
    Error("MakeTreeC","Can not get TreeC from loader");
    return;
  }
  
  Int_t buffersize = 4000;
  Char_t branchname[30];
  Char_t* cluclass;
  const char *det[4]={"SPD","SSD","SDD","TARGET"};

  for(Int_t i=0;i<fgkNdettypes;i++){
    cluclass = GetClusterClassName(i);
    if(fCtype==0x0)  fCtype = new TObjArray(fgkNdettypes);
    if(!ClustersAddress(i)){
      fCtype->AddAt(new TClonesArray(cluclass,1000),i);
    }
    if(fgkNdettypes==3) sprintf(branchname,"%sClusters%s",det[3],det[i]);
    else sprintf(branchname,"%sClusters%d",det[3],i+1);
    if(fCtype && lTC){
      if(lTC->GetBranch(branchname)){
	Warning("MakeBranchC","Branch %s already exists in TreeC",branchname);
      } else{
	Info("MakeBranchC","Creating branch %s in TreeC",branchname);
	lTC->Branch(branchname,&((*fCtype)[i]),buffersize);
      }
    }

  }
  
}

//_______________________________________________________________
void IlcTARGETDetTypeRec::GetTreeC(Int_t event){
  
  //Get the clusters tree for this event and
  //sets the branch address


  if(!fLoader){
    Warning("GetTreeC","TARGET loader is null!");
    return;
  }
  
  Char_t branchname[30];
  const char *det[4] = {"SPD","SSD","SDD","TARGET"};
  TTree* lTC = fLoader->TreeC();
  
  ResetClusters();
  if(lTC) fLoader->CleanRawClusters();

  TBranch* branch;
  if(lTC){
    Char_t* cluclass;
    for(Int_t i=0;i<fgkNdettypes;i++){
      cluclass = GetClusterClassName(i);
      if(fCtype==0x0) fCtype = new TObjArray(fgkNdettypes);
      if(!fCtype->UncheckedAt(i)) fCtype->AddAt(new TClonesArray(cluclass,1000),i);
      if(fgkNdettypes==3) sprintf(branchname,"%sClusters%s",det[3],det[i]);
      else sprintf(branchname,"%sClusters%d",det[3],i+1);
      if(fCtype){
	branch = lTC->GetBranch(branchname);
	if(branch) branch->SetAddress(&((*fCtype)[i]));
      }
    }
  } else{
    Error("GetTreeC","cannot find clusters Tree for vent %d",event);
  }

}

//___________________________________________________________________
void IlcTARGETDetTypeRec::AddCluster(Int_t id, IlcTARGETRawCluster *c){

  // Adds a raw cluster to the list
  TClonesArray &lc = *((TClonesArray*)fCtype->UncheckedAt(id));  
  switch(id){
  case 0:
    new(lc[fNctype[id]++]) IlcTARGETRawClusterSPD(*((IlcTARGETRawClusterSPD*)c));
    break;
  case 1:
    new(lc[fNctype[id]++]) IlcTARGETRawClusterSSD(*((IlcTARGETRawClusterSSD*)c));
    break;
  case 2:
    new(lc[fNctype[id]++]) IlcTARGETRawClusterSDD(*((IlcTARGETRawClusterSDD*)c));
    break;
  } 
}
//___________________________________________________________________
void IlcTARGETDetTypeRec::ResetDigits(){
  // Reset number of digits and the digits array for the TARGET detector.
  
  if(!fDigits) return;
  for(Int_t i=0;i<fgkNdettypes;i++){
    ResetDigits(i);
  }
}
//___________________________________________________________________
void IlcTARGETDetTypeRec::ResetDigits(Int_t branch){
  // Reset number of digits and the digits array for this branch.
  
  if(fDigits->UncheckedAt(branch)) ((TClonesArray*)fDigits->UncheckedAt(branch))->Clear();
  if(fNdtype) fNdtype[branch]=0;

}

//__________________________________________________________________
void IlcTARGETDetTypeRec::ResetClusters(){

  //Resets number of clusters and the cluster array 
  for(Int_t i=0;i<fgkNdettypes;i++){
    ResetClusters(i);
  }
}

//__________________________________________________________________
void IlcTARGETDetTypeRec::ResetClusters(Int_t i){

  //Resets number of clusters and the cluster array for this branch

  if (fCtype->UncheckedAt(i))    ((TClonesArray*)fCtype->UncheckedAt(i))->Clear();
  if (fNctype)  fNctype[i]=0;
}
//__________________________________________________________________
void IlcTARGETDetTypeRec::MakeBranchR(const char *file, Option_t *opt){

  //Creates tree branches for recpoints
  // Inputs:
  //      cont char *file  File name where RecPoints branch is to be written
  //                       to. If blank it write the SDigits to the same
  //                       file in which the Hits were found.

  if(!fLoader){
    Warning("MakeBranchR","TARGET loader is null!");
    return;
  }

  Int_t buffsz = 4000;
  char branchname[30];

  // only one branch for rec points for all detector types
  Bool_t oFast= (strstr(opt,"Fast")!=0);
  
  Char_t detname[10] = "TARGET";
 
  
  if(oFast){
    sprintf(branchname,"%sRecPointsF",detname);
  } else {
    sprintf(branchname,"%sRecPoints",detname);
  }
  
  if(!fRecPoints)fRecPoints = new TClonesArray("IlcTARGETRecPoint",1000);
  if (fLoader->TreeR()) {
    if(fRecPoints==0x0) fRecPoints = new TClonesArray("IlcTARGETRecPoint",
						      1000);
    MakeBranchInTree(fLoader->TreeR(),branchname,0,&fRecPoints,buffsz,99,file);
  } // end if

  
}
//______________________________________________________________________
void IlcTARGETDetTypeRec::SetTreeAddressR(TTree *treeR){
    // Set branch address for the Reconstructed points Trees.
    // Inputs:
    //      TTree *treeR   Tree containing the RecPoints.
    // Outputs:
    //      none.
    // Return:

    char branchname[30];
    Char_t namedet[10]="TARGET";

    if(!treeR) return;
    if(fRecPoints==0x0) fRecPoints = new TClonesArray("IlcTARGETRecPoint",1000);
    TBranch *branch;
    sprintf(branchname,"%sRecPoints",namedet);
    branch = treeR->GetBranch(branchname);
    if (branch) {
      branch->SetAddress(&fRecPoints);
    }else {
      sprintf(branchname,"%sRecPointsF",namedet);
      branch = treeR->GetBranch(branchname);
      if (branch) {
	branch->SetAddress(&fRecPoints);
      }

    }
}
//____________________________________________________________________
void IlcTARGETDetTypeRec::AddRecPoint(const IlcTARGETRecPoint &r){
    // Add a reconstructed space point to the list
    // Inputs:
    //      const IlcTARGETRecPoint &r RecPoint class to be added to the tree
    //                              of reconstructed points TreeR.
    // Outputs:
    //      none.
    // Return:
    //      none.

    TClonesArray &lrecp = *fRecPoints;
    new(lrecp[fNRecPoints++]) IlcTARGETRecPoint(r);
}

//______________________________________________________________________
void IlcTARGETDetTypeRec::DigitsToRecPoints(Int_t evNumber,Int_t lastentry,Option_t *opt, Bool_t v2){
  // cluster finding and reconstruction of space points
  // the condition below will disappear when the geom class will be
  // initialized for all versions - for the moment it is only for v5 !
  // 7 is the SDD beam test version
  // Inputs:
  //      Int_t evNumber   Event number to be processed.
  //      Int_t lastentry  Offset for module when not all of the modules
  //                       are processed.
  //      Option_t *opt    String indicating which TARGET sub-detectors should
  //                       be processed. If ="All" then all of the TARGET
  //                       sub detectors are processed.

  if(!GetTARGETgeom()){
    Warning("DigitsToRecPoints","Null pointer to IlcTARGETgeom !");
    return;
  }
  if(!fLoader){
    Warning("DigitsToRecPoints","TARGET loader is null!");
    return;
  }

  const char *all = strstr(opt,"All");
  const char *det[3] = {strstr(opt,"SPD"),strstr(opt,"SSD"),
                        strstr(opt,"SDD")};
  if(!v2) {
    SetDefaultClusterFinders();
    IlcInfo("Original cluster finder has been selected\n");
  }
  else   { 

    //dan-commento cout <<"chiama il SetDefaultClusterFinderV2"<<endl;
    SetDefaultClusterFindersV2();
    IlcInfo("V2 cluster finder has been selected \n");
  }

  TTree *treeC=fLoader->TreeC();
  if(!treeC){
    MakeTreeC();
    MakeBranchC();
  }
  IlcTARGETClusterFinder *rec     = 0;
  Int_t lay,lad,dete,s=0,s2=0,id,module,first=0;
  for(module=0;module<GetTARGETgeom()->GetIndexMax();module++){
//    if ((module <192) || (module>7503)) continue;
  //cout <<"modulo per la ricostruzione locale: "<<module<<endl;
//      if (module > 95) continue;//Dan Endcap
      id       = GetTARGETgeom()->GetModuleType(module);
      if (!all && !det[id]) {//dan-commento cout <<"Chiama il continue: det[id]: "<<det[id]<<endl; 
       continue;}
      if(det[id]) {/*cout <<"first: "<<first<<endl;*/ first = GetTARGETgeom()->GetStartDet(id);}
      rec = (IlcTARGETClusterFinder*)GetReconstructionModel(id);
      TClonesArray *itsDigits  = DigitsAddress(id);
      if (!rec) {
          Error("DigitsToRecPoints",
                "The reconstruction class was not instanciated! event=%d",
                evNumber);
          exit(1);
      } 
      ResetDigits();
      TTree *lTD = fLoader->TreeD();
      if (all) {
          lTD->GetEvent(lastentry+module);
      }else {
          lTD->GetEvent(lastentry+(module));
      }
      Int_t ndigits = itsDigits->GetEntriesFast();
//      if (id ==0) {s++; cout <<"s: "<<s<<endl; cout <<"numero digits: "<<ndigits<<endl;}
//      if (id==1)  {s2++; cout <<"s2: "<<s2<<endl; cout <<"numero digits: "<<ndigits<<endl;}
      GetTARGETgeom()->GetModuleId(module,lay,lad,dete);

      if(ndigits>0){
        
	rec->SetDetTypeRec(this);
 	rec->SetDigits(DigitsAddress(id));
 	rec->SetClusters(ClustersAddress(id));
      //dan-commento cout <<"lay: "<<lay<<" lad: "<<lad<<" det: "<<dete<<endl;
         //dan-commento cout <<"!!!!!!!!!rec: "<<rec<<endl;
         //dan-commento cout <<"module: "<<module<<endl;
 	rec->FindRawClusters(module);
      } // end if
      fLoader->TreeR()->Fill();
      ResetRecPoints();
      treeC->Fill();
      ResetClusters();
  } 
      
  fLoader->WriteRecPoints("OVERWRITE");
  fLoader->WriteRawClusters("OVERWRITE");
}
//______________________________________________________________________
void IlcTARGETDetTypeRec::DigitsToRecPoints(IlcRawReader* rawReader){
  // cluster finding and reconstruction of space points
  // the condition below will disappear when the geom class will be
  // initialized for all versions - for the moment it is only for v5 !
  // 7 is the SDD beam test version
  // Inputs:
  //      Int_t evNumber   Event number to be processed.
  //      Int_t lastentry  Offset for module when not all of the modules
  //                       are processed.
  //      Option_t *opt    String indicating which TARGET sub-detectors should
  //                       be processed. If ="All" then all of the TARGET
  //                       sub detectors are processed.
  // Outputs:
  //      none.
  // Return:
  //      none.
  if(!GetTARGETgeom()){
    Warning("DigitsToRecPoints","Null pointer to IlcTARGETgeom !");
    return;
  }
  if(!fLoader){
    Warning("DigitsToRecPoints","TARGET loader is null!");
    return;
  }

  
  IlcTARGETClusterFinderV2 *rec     = 0;
  Int_t id=0;

  if(!fLoader->TreeR()) fLoader->MakeTree("R");
  TTree* cTree = fLoader->TreeR();
  TClonesArray *array=new TClonesArray("IlcTARGETRecPoint",1000);
  cTree->Branch("TARGETRecPoints",&array);
  delete array;
 
  TClonesArray** clusters = new TClonesArray*[GetTARGETgeom()->GetIndexMax()]; 
  for (Int_t iModule = 0; iModule < GetTARGETgeom()->GetIndexMax(); iModule++) {
    clusters[iModule] = NULL;
  }
  for(id=0;id<3;id++){
    rec = (IlcTARGETClusterFinderV2*)GetReconstructionModel(id);
    rec->SetDetTypeRec(this);
    if (!rec) {
      Error("DigitsToRecPoints",
	    "The reconstruction class was not instanciated");
      exit(1);
    } 
    rec->RawdataToClusters(rawReader,clusters);    
  } 
  Int_t nClusters =0;
  for(Int_t iModule=0;iModule<GetTARGETgeom()->GetIndexMax();iModule++){
    array = clusters[iModule];
    if(!array){
      Error("DigitsToRecPoints","data for module %d missing!",iModule);
      array = new TClonesArray("IlcTARGETRecPoint");
    }
    cTree->SetBranchAddress("TARGETRecPoints",&array);
    cTree->Fill();
    nClusters+=array->GetEntriesFast();
    delete array;
  }
  fLoader->WriteRecPoints("OVERWRITE");

  delete[] clusters;
  Info("DigitsToRecPoints", "total number of found recpoints in TARGET: %d\n", 
       nClusters);
  
}


