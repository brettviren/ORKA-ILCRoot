////////////////////////////////////////////////////
//  Class to manage the                           //
//  TARGET beam test conversion from rawdata         //
//  to digits. It executes the digitization for   //
//  SPD, SDD and SSD.                             //
//                                                //
//                                                //
//  Origin:  E. Crescio crescio@to.infn.it        //
//           J. Conrad  Jan.Conrad@cern.ch        //
////////////////////////////////////////////////////
#include <TClonesArray.h>
#include <TTree.h>

#include "IlcHeader.h"
#include "IlcRun.h"
#include "IlcRunLoader.h"
#include "IlcTARGETEventHeader.h"
#include "IlcTARGETLoader.h"
#include "IlcTARGETBeamTestDigSDD.h"
#include "IlcTARGETBeamTestDigSPD.h"
#include "IlcTARGETBeamTestDigSSD.h"
#include "IlcTARGETBeamTestDigitizer.h"
#include "IlcRawReaderDate.h"
#include "IlcRawReaderRoot.h"

const TString IlcTARGETBeamTestDigitizer::fgkDefaultDigitsFileName="TARGET.Digits.root";  

ClassImp(IlcTARGETBeamTestDigitizer)


//_____________________________________________________________
IlcTARGETBeamTestDigitizer::IlcTARGETBeamTestDigitizer():TTask() 
{  
  //
  // Default constructor
  //
  fRunLoader = 0;
  fLoader =0;
  fEvIn=0;
  fEvFin=0;
  fFlagHeader=kTRUE;
  fDATEEvType=7;
  fRunNumber=-1;
  SetFlagInit();
  SetOptDate();
  fPeriod=kNov04;
} 

//_____________________________________________________________
  IlcTARGETBeamTestDigitizer::IlcTARGETBeamTestDigitizer(const Text_t* name, const Text_t* title, Char_t* opt,const char* filename):TTask(name,title) 
{  
  //
  // Standard constructor 
  //
  fRunLoader=0;
  fLoader=0;
  fEvIn=0;
  fEvFin=0;
  fDATEEvType=7;
  fFlagHeader=kTRUE;
  fRunNumber=-1;
  SetOptDate();

  TString choice(opt);
  Bool_t aug04 = choice.Contains("Aug04");
  Bool_t nov04 = choice.Contains("Nov04");
  if(aug04) fPeriod=kAug04;
  if(nov04) fPeriod=kNov04;
  Init(filename);
 } 

//_____________________________________________________________
  IlcTARGETBeamTestDigitizer::IlcTARGETBeamTestDigitizer(const Text_t* name, const Text_t* title, Int_t run, Char_t* opt,const char* filename):TTask(name,title) 

{  
  //
  // Constructor 
  //
  fRunLoader=0;
  fLoader=0;
  fEvIn=0;
  fEvFin=0;
  fDATEEvType=7;
  fFlagHeader=kTRUE;
  fRunNumber=run;
  SetOptDate();
  TString choice(opt);
  Bool_t aug04 = choice.Contains("Aug04");
  Bool_t nov04 = choice.Contains("Nov04");
  if(aug04) fPeriod=kAug04;
  if(nov04) fPeriod=kNov04;

  Init(filename);
 } 

//___________________________________________________________
void IlcTARGETBeamTestDigitizer::Init(const char* filename){

  //
  //Initialization of run loader and its loader 
  //creation of gilc.root
  //


  fRunLoader = IlcRunLoader::Open(filename,IlcConfig::GetDefaultEventFolderName(),"update");
  if (fRunLoader == 0x0)
    {
      Error("IlcTARGETBeamTestDigitizer","Can not load the session",filename);
      return;
    }
  fRunLoader->LoadgIlc();
  gIlc = fRunLoader->GetIlcRun();
  
  if(!gIlc) {
    Error("IlcTARGETBeamTestDigitizer","gIlc not found on file. Aborting.");
    return;
  } 
  fRunLoader->MakeTree("E");  
  fLoader = (IlcTARGETLoader*)fRunLoader->GetLoader("TARGETLoader");

  fDigitsFileName=fgkDefaultDigitsFileName;
  this->Add(new IlcTARGETBeamTestDigSPD("DigSPD","SPD Digitization")); 
  this->Add(new IlcTARGETBeamTestDigSDD("DigSDD","SDD Digitization")); 
  this->Add(new IlcTARGETBeamTestDigSSD("DigSSD","SSD Digitization"));

  SetFlagInit(kTRUE);
}


//______________________________________________________________________
IlcTARGETBeamTestDigitizer::IlcTARGETBeamTestDigitizer(const IlcTARGETBeamTestDigitizer &bt):TTask(bt){
  // Copy constructor. 
  //not allowed
  if(this==&bt) return;
  Error("Copy constructor",
	"You are not allowed to make a copy of the IlcTARGETBeamTestDigitizer");
  exit(1);

}
//______________________________________________________________________
IlcTARGETBeamTestDigitizer& IlcTARGETBeamTestDigitizer::operator=(const IlcTARGETBeamTestDigitizer &source){
    // Assignment operator. This is a function which is not allowed to be
    // done to the TARGET beam test digitizer. It exits with an error.
    // Inputs:
    if(this==&source) return *this;
    Error("operator=","You are not allowed to make a copy of the IlcTARGETBeamTestDigitizer");
    exit(1);
    return *this; //fake return
}


//______________________________________________________________
IlcTARGETBeamTestDigitizer::~IlcTARGETBeamTestDigitizer(){

  //Destructor
  //  if(fBt) delete fBt;
  if(fLoader) delete fLoader;
  if(fHeader) delete fHeader;
} 


//_____________________________________________________________
void IlcTARGETBeamTestDigitizer::SetNumberOfEventsPerFile(Int_t nev)
{
  //Sets number of events per file

  if(fRunLoader) fRunLoader->SetNumberOfEventsPerFile(nev);
  else Warning("SetNumberOfEventsPerFile","fRunLoader is 0");
}


//____________________________________________________
void IlcTARGETBeamTestDigitizer::ExecDigitization(){

  // Execution of digitisation for SPD,SDD and SSD

  if(!GetFlagInit()){
    Warning("ExecDigitization()","Run Init() please..");
    return;
  }
  fLoader->SetDigitsFileName(fDigitsFileName);
  fLoader->LoadDigits("recreate");
 
  IlcRawReader* rd;

  if(GetOptDate()) rd = new IlcRawReaderDate(fRawdataFileName,fEvIn);
  else rd = new IlcRawReaderRoot(fRawdataFileName,fEvIn);

  IlcHeader* header = fRunLoader->GetHeader();
  Int_t iev=fEvIn-1;

  
  IlcTARGETBeamTestDigSDD* digSDD = (IlcTARGETBeamTestDigSDD*)fTasks->FindObject("DigSDD");
  IlcTARGETBeamTestDigSPD* digSPD = (IlcTARGETBeamTestDigSPD*)fTasks->FindObject("DigSPD");
  IlcTARGETBeamTestDigSSD* digSSD = (IlcTARGETBeamTestDigSSD*)fTasks->FindObject("DigSSD");


  do{
    iev++;
    if(fEvFin!=0){
      if(iev>fEvFin) break;
    } 
    IlcTARGETEventHeader* itsh = new IlcTARGETEventHeader("TARGETHeader");
    fRunLoader->SetEventNumber(iev);
   
    rd->RequireHeader(fFlagHeader);
// // // //     rd->SelectEvents(fDATEEvType);
 
    digSDD->SetRawReader(rd);
    digSPD->SetRawReader(rd);
    digSSD->SetRawReader(rd);
    
    if(fLoader->TreeD() == 0x0) fLoader->MakeTree("D");

    TTree* treeD = (TTree*)fLoader->TreeD();
   
    // Make branches outside the dig-classes

    TClonesArray* digitsSPD = new TClonesArray("IlcTARGETdigitSPD",1000);
    treeD->Branch("TARGETDigitsSPD",&digitsSPD);
 
    TClonesArray* digitsSDD = new TClonesArray("IlcTARGETdigitSDD",1000);
    treeD->Branch("TARGETDigitsSDD",&digitsSDD);
   
    TClonesArray* digitsSSD = new TClonesArray("IlcTARGETdigitSSD",1000);
    treeD->Branch("TARGETDigitsSSD",&digitsSSD);


    digSSD->SetTree(treeD);
    digSDD->SetTree(treeD);
    digSPD->SetTree(treeD);
    
    IlcTARGETgeom* geom = fLoader->GetTARGETgeom();

    digSSD->SetTARGETgeom(geom);
    digSDD->SetTARGETgeom(geom);
    digSPD->SetTARGETgeom(geom);
    
    digSSD->SetTARGETEventHeader(itsh);
    digSDD->SetTARGETEventHeader(itsh);
    digSPD->SetTARGETEventHeader(itsh);

    digSDD->SetBtPeriod(GetBeamTestPeriod());
    if(GetBeamTestPeriod()==1)digSDD->SetThreshold(16);
    else digSDD->SetThreshold(0);
    ExecuteTask(0);  

    header->SetEventNrInRun(iev);
    header->SetEvent(iev);
    header->SetRun(fRunNumber);
    fRunLoader->GetHeader()->AddDetectorEventHeader(itsh);
    fRunLoader->TreeE()->Fill();
    header->Reset(fRunNumber,iev);
    
    delete digitsSPD;
    delete digitsSDD;
    delete digitsSSD;

   }while(rd->NextEvent());

  
  fRunLoader->WriteHeader("OVERWRITE");
  fRunLoader->WriteRunLoader("OVERWRITE");

  delete rd;
  fLoader->UnloadDigits();
  fLoader->UnloadRawClusters();
  fRunLoader->UnloadHeader();

  
}



//_______________________________________________
void IlcTARGETBeamTestDigitizer:: SetActive(const TString& subdet,Bool_t value){

  //Sets active sub-tasks (detectors)
  
  Bool_t sdd = subdet.Contains("SDD");
  Bool_t spd = subdet.Contains("SPD");
  Bool_t ssd = subdet.Contains("SSD");

  if(sdd){
  IlcTARGETBeamTestDigSDD* digSDD = (IlcTARGETBeamTestDigSDD*)fTasks->FindObject("DigSDD");
  digSDD->SetActive(value);
  }
  
 if(spd){
  IlcTARGETBeamTestDigSPD* digSPD = (IlcTARGETBeamTestDigSPD*)fTasks->FindObject("DigSPD");
  digSPD->SetActive(value);
  
  }
 
  if(ssd){
  IlcTARGETBeamTestDigSSD* digSSD = (IlcTARGETBeamTestDigSSD*)fTasks->FindObject("DigSSD");
  digSSD->SetActive(value);
  
  }



}

