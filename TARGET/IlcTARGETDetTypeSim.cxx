/***************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 ******************************************F********************************/

/*
 $Id: IlcTARGETDetTypeSim.cxx,v 1.5 2008/11/07 01:49:49 cgatto Exp $
*/


/////////////////////////////////////////////////////////////////////
// Base simulation functions for TARGET                               //
//                                                                 //
//                                                                 //
/////////////////////////////////////////////////////////////////////          
#include <TStopwatch.h>
#include "TBranch.h"
#include "TClonesArray.h"
#include "TObjArray.h"
#include "TTree.h"

#include "IlcRun.h"
#include "IlcLoader.h"

#include "IlcCDBManager.h"
#include "IlcCDBId.h"
#include "IlcCDBStorage.h"
#include "IlcCDBEntry.h"
#include "IlcCDBMetaData.h"

#include "IlcTARGETdigit.h"
#include "IlcTARGETdigitSPD.h"
#include "IlcTARGETdigitSDD.h"
#include "IlcTARGETdigitSSD.h"
#include "IlcTARGETDetTypeSim.h"
#include "IlcTARGETpListItem.h"
#include "IlcTARGETresponseSDD.h"
#include "IlcTARGETCalibrationSDD.h"
#include "IlcTARGETCalibrationSSD.h"
#include "IlcTARGETsegmentationSPD.h"
#include "IlcTARGETsegmentationSPD2.h"
#include "IlcTARGETsegmentationSDD.h"
#include "IlcTARGETsegmentationSSD.h"
#include "IlcTARGETsimulation.h"
#include "IlcTARGETsimulationSPD.h"
#include "IlcTARGETsimulationSDD.h"
#include "IlcTARGETsimulationSSD.h"
#include "IlcTARGETsimulationFastPoints.h"



ClassImp(IlcTARGETDetTypeSim)

//----------------------------------------------------------------------
IlcTARGETDetTypeSim::IlcTARGETDetTypeSim():
TObject(),
fGeom(),         //
fGeom2(),
fSimulation(),   // [NDet]
fSegmentation(), // [NDet]
fSegmentation2(), // [NDet]
//fSegmentationSPD(), // [NDet]
fCalibration(),     // [NMod]
fPreProcess(),   // [] e.g. Fill fHitModule with hits
fPostProcess(),  // [] e.g. Wright Raw data
fNSDigits(0),    //! number of SDigits
fSDigits(),      //! [NMod][NSDigits]
fNDigits(0),     //! number of Digits
fDigits(),       //! [NMod][NDigits]
fHitClassName(), // String with Hit class name.
fSDigClassName(),// String with SDigit class name.
fDigClassName(){ // String with digit class name.
    // Default Constructor
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //    A properly zero-ed IlcTARGETDetTypeSim class.
}
void IlcTARGETDetTypeSim::Init() {
//----------------------------------------------------------------------
  IlcTARGETParam *Param;
  IlcInfo("Called");
//cout <<"****************IlcTARGETDetTypeSim: Minor Version QUA: "<<fDetVersion<<endl;
  if (fDetVersion == 3) {fgkNdettypes = 2; Param=new IlcTARGETParam(fDetVersion); }
  else if (fDetVersion == 2) {fgkNdettypes = 1; Param=new IlcTARGETParam(fDetVersion); }
  else if (fDetVersion == 1) {fgkNdettypes = 1; Param=new IlcTARGETParam(fDetVersion); }
  
  fgkDefaultNModulesSPD =  Param->GetNmodulesSPD();
 
 
  fgkDefaultNModulesSDD =  0;
  fgkDefaultNModulesSSD =  Param->GetNmodulesSSD();
  //dan-commento cout <<"tipo di tecnologie: "<<fgkNdettypes<<endl;
  //dan-commento cout <<"numero SPD: "<<fgkDefaultNModulesSPD<<endl;
  //dan-commento cout <<"numero SSD: "<<fgkDefaultNModulesSSD<<endl;
  //fGeom = 0;
  //fGeom2 = 0;
  fSimulation = new TObjArray(fgkNdettypes);
  fSegmentation2 = new TObjArray(fgkNdettypes);
  fSegmentation = new TObjArray(fgkNdettypes);
  fSegmentation->SetOwner(kTRUE);
  fSegmentation2->SetOwner(kTRUE);
  fCalibration = 0;
  fPreProcess = 0;
  fPostProcess = 0;
  fNSDigits = 0;
  fSDigits = new TClonesArray("IlcTARGETpListItem",1000);
  fDigits = new TObjArray(fgkNdettypes);
  fNDigits = new Int_t[fgkNdettypes];
  fLoader = 0;
  fNMod[0] = fgkDefaultNModulesSPD;
  fNMod[1] = fgkDefaultNModulesSSD;
  fNMod[2] = fgkDefaultNModulesSDD;
  SetRunNumber();
  fFirstcall = kTRUE;

  delete Param;
}

IlcTARGETDetTypeSim::~IlcTARGETDetTypeSim(){
    // Destructor
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //    Nothing.
  
    if(fSimulation){
      fSimulation->Delete();
      delete fSimulation;
      fSimulation = 0;
    }
    
    if(fSegmentation){
      fSegmentation->Delete();
      delete fSegmentation;
      fSegmentation = 0;
    }

    if(fSegmentation2){
      fSegmentation2->Delete();
      delete fSegmentation2;
      fSegmentation2 = 0;
    }
    
    if (fCalibration){
      for(int i=0;i<fCalibration->GetEntries();i++){
        IlcTARGETCalibration *ar=(IlcTARGETCalibration*)fCalibration->UncheckedAt(i);
	delete ar;
      }
    }

    if(fGeom) delete fGeom;
    if(fGeom2) delete fGeom2;
    
    if(fPreProcess){
      fPreProcess->Delete();
      delete fPreProcess;
      fPreProcess = 0;
    }
    
    if(fPostProcess){
      fPostProcess->Delete();
      delete fPostProcess;
      fPostProcess = 0;
    }

    if(fNDigits) delete [] fNDigits;

    if (fLoader)
      {
	fLoader->GetModulesFolder()->Remove(this);
      }
    
    if (fSDigits) {
      fSDigits->Delete();
      delete fSDigits;
      fSDigits=0;
    }
    if (fDigits) {
      fDigits->Delete();
      delete fDigits;
      fDigits=0;
    }
  
}
//----------------------------------------------------------------------
IlcTARGETDetTypeSim::IlcTARGETDetTypeSim(const IlcTARGETDetTypeSim &source) : TObject(source){
    // Copy Constructor for object IlcTARGETDetTypeSim not allowed
  if(this==&source) return;
  Error("Copy constructor",
	"You are not allowed to make a copy of the IlcTARGETDetTypeSim");
  exit(1);

        
}
//----------------------------------------------------------------------
IlcTARGETDetTypeSim& IlcTARGETDetTypeSim::operator=(const IlcTARGETDetTypeSim &source){
    // The = operator for object IlcTARGETDetTypeSim
 
    if(&source==this) return *this;
    Error("operator=","You are not allowed to make a copy of the IlcTARGETDetTypeSIm");
    exit(1);
    return *this;
}


//______________________________________________________________________
void IlcTARGETDetTypeSim::SetSimulationModel(Int_t dettype,IlcTARGETsimulation *sim){

  //Set simulation model for detector type

  if(fSimulation==0) fSimulation = new TObjArray(fgkNdettypes);
  fSimulation->AddAt(sim,dettype);
}
//______________________________________________________________________
IlcTARGETsimulation* IlcTARGETDetTypeSim::GetSimulationModel(Int_t dettype){

  //Get simulation model for detector type
  if(fSimulation==0)  {
    Warning("GetSimulationModel","fSimulation is 0!");
    return 0;     
  }
  return (IlcTARGETsimulation*)(fSimulation->UncheckedAt(dettype));
}
//______________________________________________________________________
IlcTARGETsimulation* IlcTARGETDetTypeSim::GetSimulationModelByModule(Int_t module){

  //Get simulation model by module number
  if(fGeom==0) {
    Warning("GetSimulationModelByModule","fGeom is 0!");
    return 0;
  }
  
  return GetSimulationModel(fGeom->GetModuleType(module));
}
//_______________________________________________________________________
void IlcTARGETDetTypeSim::SetDefaultSegmentation(Int_t idet){
  // Set default segmentation model objects
  IlcTARGETsegmentation *seg;
  IlcTARGETsegmentation2 *seg2;
  if(fSegmentation==0x0){
    fSegmentation = new TObjArray(fgkNdettypes);
    fSegmentation->SetOwner(kTRUE);
  //cout <<"fgNdettypes: "<<fgkNdettypes<< " fSegmentation" <<fSegmentation<<endl;
  }
  if(fSegmentation2==0x0){
    fSegmentation2 = new TObjArray(fgkNdettypes);
    fSegmentation2->SetOwner(kTRUE);
  //cout <<"fgNdettypes: "<<fgkNdettypes<< " fSegmentation2" <<fSegmentation2<<endl;
  }


  if(GetSegmentationModel2(idet))delete (IlcTARGETsegmentation2*)fSegmentation2->UncheckedAt(idet);
  if(GetSegmentationModel(idet))delete (IlcTARGETsegmentation*)fSegmentation->UncheckedAt(idet);

  //if(GetSegmentationModelSPD(idet))delete (IlcTARGETsegmentation2*)fSegmentationSPD->At(idet);

  if(idet==0){
    seg2 = new IlcTARGETsegmentationSPD2(fGeom2);
    SetSegmentationModel2(idet,seg2);
  }
  else if(idet==1){
    seg2 = new IlcTARGETsegmentationSSD(fGeom2);
    SetSegmentationModel2(idet,seg2);
  }
  else {
    //IlcTARGETCalibration* res = GetCalibrationModel(fGeom->GetStartSDD());
    seg2 = new IlcTARGETsegmentationSDD(fGeom2,0);
  SetSegmentationModel2(idet,seg2);
  }


}

//______________________________________________________________________
void IlcTARGETDetTypeSim::SetSegmentationModel(Int_t dettype,IlcTARGETsegmentation *seg){
   
  //Set segmentation model for detector type
  if(fSegmentation==0x0){
    fSegmentation = new TObjArray(fgkNdettypes);
    fSegmentation->SetOwner(kTRUE);
  }
  fSegmentation->AddAt(seg,dettype);

}
//______________________________________________________________________
void IlcTARGETDetTypeSim::SetSegmentationModel2(Int_t dettype,IlcTARGETsegmentation2 *seg){
   
  //Set segmentation model for detector type
  if(fSegmentation2==0x0){
    fSegmentation2 = new TObjArray(fgkNdettypes);
    fSegmentation2->SetOwner(kTRUE);
  }
  fSegmentation2->AddAt(seg,dettype);

}
//______________________________________________________________________
IlcTARGETsegmentation* IlcTARGETDetTypeSim::GetSegmentationModel(Int_t dettype){

  //Get segmentation model for detector type
   
   if(fSegmentation==0) {
     Warning("GetSegmentationModel","fSegmentation is 0!");
     return 0; 
   } 
   return (IlcTARGETsegmentation*)(fSegmentation->UncheckedAt(dettype));

}
//______________________________________________________________________
IlcTARGETsegmentation2* IlcTARGETDetTypeSim::GetSegmentationModel2(Int_t dettype){

  //Get segmentation model for detector type
   
   if(fSegmentation2==0) {
     Warning("SmentationModelSPD","fSegmentation is 0!");
     return 0; 
   } 
   return (IlcTARGETsegmentation2*)(fSegmentation2->UncheckedAt(dettype));

}
//_______________________________________________________________________
IlcTARGETsegmentation* IlcTARGETDetTypeSim::GetSegmentationModelByModule(Int_t module){
  
  //Get segmentation model by module number
   if(fGeom==0){
     Warning("GetSegmentationModelByModule","fGeom is 0!");
     return 0;
   }     
   return GetSegmentationModel(fGeom->GetModuleType(module));

}
//_______________________________________________________________________
void IlcTARGETDetTypeSim::CreateCalibrationArray() {

  //Create the container of calibration functions with correct size
  if (fCalibration) {
    Warning("CreateCalibration","pointer to calibration object exists\n");
    fCalibration->Delete();
    delete fCalibration;
  }

  Int_t nModTot = fGeom->GetIndexMax();
  fCalibration = new TObjArray(nModTot);
  fCalibration->SetOwner(kTRUE);
  fCalibration->Clear();

}
//_______________________________________________________________________
void IlcTARGETDetTypeSim::SetCalibrationModel(Int_t iMod, IlcTARGETCalibration *resp){

  //Set response model for modules

  if (fCalibration==0) CreateCalibrationArray();
 
  if (fCalibration->UncheckedAt(iMod)!=0)
    delete (IlcTARGETCalibration*) fCalibration->UncheckedAt(iMod);
  fCalibration->AddAt(resp, iMod);

}

//______________________________________________________________________
void IlcTARGETDetTypeSim::ResetCalibrationArray(){

  //resets response array
  if(fCalibration && fRunNumber<0){  // if fRunNumber<0 fCalibration is owner
    IlcTARGETresponse* rspd = ((IlcTARGETCalibration*)fCalibration->UncheckedAt(fGeom->GetStartSPD()))->GetResponse();
    IlcTARGETresponse* rsdd = ((IlcTARGETCalibration*)fCalibration->UncheckedAt(fGeom->GetStartSDD()))->GetResponse();
    IlcTARGETresponse* rssd = ((IlcTARGETCalibration*)fCalibration->UncheckedAt(fGeom->GetStartSSD()))->GetResponse();
    if(rspd) delete rspd;
    if(rsdd) delete rsdd;
    if(rssd) delete rssd;
    fCalibration->Clear();
  }
  else if (fCalibration && fRunNumber>=0){
    fCalibration->Clear();
  }
}
//______________________________________________________________________
void IlcTARGETDetTypeSim::ResetSegmentation(){
 
 //Resets segmentation array

  if(fSegmentation) fSegmentation->Clear();
  if(fSegmentation2) fSegmentation2->Clear();

}

//_______________________________________________________________________
IlcTARGETCalibration* IlcTARGETDetTypeSim::GetCalibrationModel(Int_t iMod){
   //Get response model for module number iMod 
 
  if(fCalibration==0) {
    IlcError("fCalibration is 0!");
    return 0; 
  }

  return (IlcTARGETCalibration*)(fCalibration->UncheckedAt(iMod));

}
//_______________________________________________________________________
void IlcTARGETDetTypeSim::SetDefaults(){

  //Set defaults for segmentation and response

  if(fGeom==0){
    Warning("SetDefaults","fGeom is 0!");
    return;
  }
  if(fGeom2==0){
    Warning("SetDefaults","fGeom is 0!");
    return;
  }

  if (fCalibration==0) {
    CreateCalibrationArray();
  }
  ResetSegmentation();

  //if(!GetCalibration()){IlcFatal("Exit"); exit(0);}

  for(Int_t idet=0;idet<fgkNdettypes;idet++){
    //SPD
    if(idet==0){
      if(!GetSegmentationModel2(idet)) SetDefaultSegmentation(idet);
	SetDigitClassName(idet,"IlcTARGETdigitSPD");
        //cout <<"settato Digit class name for SPD"<<endl;
    }
    //SDD
    if(idet==1){
       //cout <<"forse il problema Ã¨ qui"<<endl;
      if(!GetSegmentationModel2(idet))SetDefaultSegmentation(idet);
	SetDigitClassName(idet,"IlcTARGETdigitSSD");
        //cout <<"forse non Ã¨ qui"<<endl;
        //cout <<"in DetSim no problemi"<<endl;
        //cout <<"settato Digit class name for SSD"<<endl;
    }
    //SSD
    if(idet==2){
      if(!GetSegmentationModel2(idet)) SetDefaultSegmentation(idet);
      //IlcTARGETCalibrationSDD* rsp = (IlcTARGETCalibrationSDD*)GetCalibrationModel(fGeom->GetStartSDD());
      //const char *kopt = ((IlcTARGETresponseSDD*)rsp->GetResponse())->ZeroSuppOption();
      //if((!strstr(kopt,"2D"))&&(!strstr(kopt,"1D"))) {
	//SetDigitClassName(idet,"IlcTARGETdigit");
      //}
      //else {
	SetDigitClassName(idet,"IlcTARGETdigitSDD");
    //  }
    
    }

  }
}

//______________________________________________________________________
Bool_t IlcTARGETDetTypeSim::GetCalibration() {
  // Get Default calibration if a storage is not defined.
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
  if(GetRunNumber()<0){
    isCacheActive=kFALSE;
    fCalibration->SetOwner(kTRUE);
  }
  else{
    fCalibration->SetOwner(kFALSE);
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

  if ((!pSPD)||(!pSDD)||(!pSSD)) {
    IlcWarning("Can not get calibration from calibration database !");
    return kFALSE;
  }
  if ((! calSPD)||(! calSSD)||(! calSDD)) {
    IlcWarning("Can not get calibration from calibration database !");
    return kFALSE;
  }
  
  IlcTARGETParam *Param;

//if (DetVersion !=3 )cout <<"in IlcTARGETDetTypeSim: Minor Version: "<<DetVersion<<endl;
if (fDetVersion == 3) {fgkNdettypes = 2; Param=new IlcTARGETParam(fDetVersion); }
else if (fDetVersion == 2) {fgkNdettypes = 1; Param=new IlcTARGETParam(fDetVersion); }
else if (fDetVersion == 1) {fgkNdettypes = 1; Param=new IlcTARGETParam(fDetVersion); }
//cout <<"****************IlcTARGETDetTypeSim: Minor Version: "<<fDetVersion<<endl;
//cout <<"****************IlcTARGETDetTypeSim  - Param->GetNmodulesSPD(): "<<Param->GetNmodulesSPD()<<endl;
  
//  fNMod[0] = 192;//8752;//192;
  fNMod[0] = Param->GetNmodulesSPD();
  fNMod[1] =  7504;
  fNMod[2] =0;
  IlcInfo(Form("%i SPD, %i SSD and %i SDD in calibration database",
	       fNMod[0], fNMod[1], fNMod[2]));
  IlcTARGETCalibration* cal;
  for (Int_t i=0; i<fNMod[0]; i++) {
    cal = (IlcTARGETCalibration*) calSPD->UncheckedAt(i);
    cal->SetResponse(pSPD);
    SetCalibrationModel(i, cal);
 }
  for (Int_t i=0; i<fNMod[1]; i++) {
    cal = (IlcTARGETCalibration*) calSSD->UncheckedAt(i);
    cal->SetResponse(pSSD);
    Int_t iMod = i + fNMod[0];
    SetCalibrationModel(iMod, cal);
 }
  for (Int_t i=0; i<fNMod[2]; i++) {
    cal = (IlcTARGETCalibration*) calSDD->UncheckedAt(i);
    cal->SetResponse(pSDD);
    Int_t iMod = i + fNMod[0] + fNMod[1];
    SetCalibrationModel(iMod, cal);
 }
delete Param;
  return kTRUE;

}



//_______________________________________________________________________
void IlcTARGETDetTypeSim::SetDefaultSimulation(){

  //Set default simulation for detector type

  if(fGeom2==0){
    Warning("SetDefaultSimulation","fGeom is 0!");
    return;
  }
  if(fCalibration==0){
    Warning("SetDefaultSimulation","fCalibration is 0!");
    return;
  }
  if(fSegmentation==0){
    Warning("SetDefaultSimulation","fSegmentation is 0!");
    for(Int_t i=0;i<fgkNdettypes;i++) SetDefaultSegmentation(i);
  }
  if(fSegmentation2==0){
    Warning("SetDefaultSimulation","fSegmentation is 0!");
    for(Int_t i=0;i<fgkNdettypes;i++) SetDefaultSegmentation(i);
  }

  else{
    for(Int_t i=0;i<fgkNdettypes;i++){
      if(!GetSegmentationModel(i)){
	//Warning("SetDefaultSimulation","Segmentation not defined for det %d - Default taken\n!",i);
	SetDefaultSegmentation(i);
      }
    }
  }
  IlcTARGETsimulation* sim;

  for(Int_t idet=0;idet<fgkNdettypes;idet++){ 
   //SPD
    if(idet==0){
      sim = GetSimulationModel(idet);
 
      if(!sim){
	sim = new IlcTARGETsimulationSPD(this);
	SetSimulationModel(idet,sim);
      }
    }
    
    //SSD
    //SDD
    if(idet==1){
      sim = GetSimulationModel(idet);
      if(!sim){
	sim = new IlcTARGETsimulationSSD(this);
	SetSimulationModel(idet,sim);
      }

    if(idet==2){
      sim = GetSimulationModel(idet);
      if(!sim){
	sim =new IlcTARGETsimulationSDD();
	SetSimulationModel(idet,sim);
      }

    }


      
    }

  }
}




//___________________________________________________________________
void IlcTARGETDetTypeSim::SetTreeAddressS(TTree* treeS, Char_t* name){
  // Set branch address for the TARGET summable digits Trees.
  
  char branchname[30];

  if(!treeS){
    return;
  }
  if (fSDigits ==  0x0){
    fSDigits = new TClonesArray("IlcTARGETpListItem",1000);
  }
  TBranch *branch;
  sprintf(branchname,"%s",name);
  branch = treeS->GetBranch(branchname);
  if (branch) branch->SetAddress(&fSDigits);

}
//___________________________________________________________________
void IlcTARGETDetTypeSim::SetTreeAddressD(TTree* treeD, Char_t* name){
  // Set branch address for the digit Trees.
  
  const char *det[3] = {"SPD","SSD","SDD"};
  TBranch *branch;
  
  char branchname[30];
  
  if(!treeD){
    return;
  }
  if(!fDigits){
    fDigits = new TObjArray(fgkNdettypes); 
  }
  for(Int_t i=0;i<fgkNdettypes;i++){
    Char_t* digclass = GetDigitClassName(i);
    if(digclass==0x0){
      if(i==0) SetDigitClassName(i,"IlcTARGETdigitSPD");
      if(i==1) SetDigitClassName(i,"IlcTARGETdigitSSD");
      if(i==2) SetDigitClassName(i,"IlcTARGETdigitSDD");
      digclass = GetDigitClassName(i);
    }
    TString classn = digclass;
    if(!(fDigits->UncheckedAt(i))){
      fDigits->AddAt(new TClonesArray(classn.Data(),1000),i);
    }else{
      ResetDigits(i);
    }
    
    if(fgkNdettypes==3) sprintf(branchname,"%sDigits%s",name,det[i]);
    else sprintf(branchname,"%sDigits%d",name,i+1);
    if(fDigits){
      branch = treeD->GetBranch(branchname);
      if(branch) branch->SetAddress(&((*fDigits)[i]));
    }
  }

}
//___________________________________________________________________
void IlcTARGETDetTypeSim::ResetDigits(){
  // Reset number of digits and the digits array for the TARGET detector.
  

  if(!fDigits){
    Error("ResetDigits","fDigits is null!");
    return;
  }
  for(Int_t i=0;i<fgkNdettypes;i++){
    ResetDigits(i);
  }
}
//___________________________________________________________________
void IlcTARGETDetTypeSim::ResetDigits(Int_t branch){
  // Reset number of digits and the digits array for this branch.

  if(fDigits->UncheckedAt(branch)){
    ((TClonesArray*)fDigits->UncheckedAt(branch))->Clear();
  }
  if(fNDigits) fNDigits[branch]=0;

}



//_______________________________________________________________________
void IlcTARGETDetTypeSim::SDigitsToDigits(Option_t* opt, Char_t* name){
  // Standard Summable digits to Digits function.
  if(!fGeom){
    Warning("SDigitsToDigits","fGeom is null!!");
    return;
  }
  
  const char *all = strstr(opt,"All");
  const char *det[3] = {strstr(opt,"SPD"),strstr(opt,"SSD"),
			strstr(opt,"SDD")};
  if( !det[0] && !det[1] && !det[2] ) all = "All";
  else all = 0;
  static Bool_t setDef = kTRUE;
  if(setDef) SetDefaultSimulation();
  setDef = kFALSE;
  
  IlcTARGETsimulation *sim =0;
  TTree* trees = fLoader->TreeS();
  if( !(trees && GetSDigits()) ){
    Error("SDigits2Digits","Error: No trees or SDigits. Returning.");
    return;
  } 
  sprintf(name,"%s",name);
  TBranch* brchSDigits = trees->GetBranch(name);
  
  Int_t id;
  for(Int_t module=0;module<fGeom->GetIndexMax();module++){
     //if (module>95) continue; //dan Endcap
     id = fGeom->GetModuleType(module);
    if (!all && !det[id]) continue;
    sim = (IlcTARGETsimulation*)GetSimulationModel(id);

    if(!sim){
      Error("SDigit2Digits","The simulation class was not "
	    "instanciated for module %d type %s!",module,
	    fGeom->GetModuleTypeName(module));
      exit(1);
    }
    sim->InitSimulationModule(module,gIlc->GetEvNumber());
    
    fSDigits->Clear();
    brchSDigits->GetEvent(module);
    
    sim->AddSDigitsToModule(fSDigits,0);
    sim->FinishSDigitiseModule();
    fLoader->TreeD()->Fill();
    ResetDigits();
  }

  fLoader->TreeD()->GetEntries();
  fLoader->TreeD()->AutoSave();
  fLoader->TreeD()->Reset();

}



//_________________________________________________________
void IlcTARGETDetTypeSim::AddSumDigit(IlcTARGETpListItem &sdig){
  
  //Adds the module full of summable digits to the summable digits tree.
  TClonesArray &lsdig = *fSDigits;
  new(lsdig[fNSDigits++]) IlcTARGETpListItem(sdig);
}
//__________________________________________________________
void IlcTARGETDetTypeSim::AddRealDigit(Int_t branch, Int_t *digits){
  //   Add a real digit - as coming from data.
  TClonesArray &ldigits = *((TClonesArray*)fDigits->UncheckedAt(branch));
  new(ldigits[fNDigits[branch]++]) IlcTARGETdigit(digits); 
}
//__________________________________________________________
void IlcTARGETDetTypeSim::AddSimDigit(Int_t branch, IlcTARGETdigit* d){
  
  //    Add a simulated digit.
  TClonesArray &ldigits = *((TClonesArray*)fDigits->UncheckedAt(branch));
  switch(branch){
  case 0:
    new(ldigits[fNDigits[branch]++]) IlcTARGETdigitSPD(*((IlcTARGETdigitSPD*)d));
    break;
  case 1:
    new(ldigits[fNDigits[branch]++]) IlcTARGETdigitSSD(*((IlcTARGETdigitSSD*)d));
    break;
  case 2:
    new(ldigits[fNDigits[branch]++]) IlcTARGETdigitSDD(*((IlcTARGETdigitSDD*)d));
    break;
  } 
  
}

//______________________________________________________________________
void IlcTARGETDetTypeSim::AddSimDigit(Int_t branch,Float_t phys,Int_t *digits,
				   Int_t *tracks,Int_t *hits,Float_t *charges){
  //   Add a simulated digit to the list.

                                     TClonesArray &ldigits = *((TClonesArray*)fDigits->UncheckedAt(branch));
  IlcTARGETCalibrationSDD *resp = 0;
  switch(branch){
  case 0:
    new(ldigits[fNDigits[branch]++]) IlcTARGETdigitSPD(digits,tracks,hits);
    break;
  case 1:

    new(ldigits[fNDigits[branch]++]) IlcTARGETdigitSSD(digits,tracks,hits);
    break;

  case 2:
    //dan-commento cout <<"Sto aggiungendo il digit per SSD"<<endl;
    resp = (IlcTARGETCalibrationSDD*)GetCalibrationModel(fGeom->GetStartSDD());
    new(ldigits[fNDigits[branch]++]) IlcTARGETdigitSDD(phys,digits,tracks,
						   hits,charges,resp);
    break;
  } 
}



//______________________________________________________________________
void IlcTARGETDetTypeSim::StoreCalibration(Int_t firstRun, Int_t lastRun, IlcCDBMetaData &md) {

  // Store calibration in the calibration database

  // The database must be created in an external piece of code (i.e. 
  // a configuration macro )

  if(!IlcCDBManager::Instance()->IsDefaultStorageSet()) {
    IlcWarning("No storage set! Will use dummy one");
    IlcCDBManager::Instance()->SetDefaultStorage("local://$ILC_ROOT");
  }

  if (!fCalibration) {
    IlcError("IlcTARGETCalibration classes are not defined - nothing done");
    return;
  }
  IlcCDBId idRespSPD("TARGET/Calib/CalibSPD",firstRun, lastRun);
  IlcCDBId idRespSDD("TARGET/Calib/CalibSDD",firstRun, lastRun);
  IlcCDBId idRespSSD("TARGET/Calib/CalibSSD",firstRun, lastRun);

  TObjArray respSPD(fNMod[0]);
  TObjArray respSDD(fNMod[1]-fNMod[0]);
  TObjArray respSSD(fNMod[2]-fNMod[1]);
  respSPD.SetOwner(kFALSE);
  respSSD.SetOwner(kFALSE);
  respSSD.SetOwner(kFALSE);

  Int_t *index=new Int_t[fgkNdettypes];
  for (Int_t i = 0; i<fgkNdettypes; i++ ) {
    index[i] = 0;
    for (Int_t j = 0; j<=i; j++ )
      index[i]+=fNMod[j];
  }

  for (Int_t i = 0; i<index[0]; i++ )
    respSPD.Add(fCalibration->UncheckedAt(i));

  for (Int_t i = index[0]; i<index[1]; i++ )
    respSDD.Add(fCalibration->UncheckedAt(i));

  for (Int_t i = index[1]; i<index[2]; i++ )
    respSSD.Add(fCalibration->UncheckedAt(i));

  IlcCDBManager::Instance()->Put(&respSPD, idRespSPD, &md);
  IlcCDBManager::Instance()->Put(&respSDD, idRespSDD, &md);
  IlcCDBManager::Instance()->Put(&respSSD, idRespSSD, &md);

  delete index;

}


