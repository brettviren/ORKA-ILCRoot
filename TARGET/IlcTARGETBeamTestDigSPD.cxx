////////////////////////////////////////////////////
//  Class to define                               //
//  SPD beam test raw 2 dig conv.                 //
//                                                //
//  Origin: Jan Conrad  Jan.Conrad@cern.ch        //
//                                                //
////////////////////////////////////////////////////

#include "IlcTARGETdigitSPD.h"
#include "IlcRawReader.h"
#include "IlcRawReader.h"
#include "IlcRawDataHeader.h"
#include "IlcTARGETRawStreamSPD.h"
#include "IlcTARGETBeamTestDigSPD.h"
#include "IlcTARGETgeom.h"
#include <TBranch.h>
#include <TTree.h>
#include <TClonesArray.h>
#include "IlcTARGETEventHeader.h"
ClassImp(IlcTARGETBeamTestDigSPD)



//_____________________________________________________________
  IlcTARGETBeamTestDigSPD::IlcTARGETBeamTestDigSPD(): IlcTARGETBeamTestDig()
{
  //
  // Constructor
  //

  
}

//_____________________________________________________________
  IlcTARGETBeamTestDigSPD::IlcTARGETBeamTestDigSPD(const Text_t* name, const Text_t* title): IlcTARGETBeamTestDig(name,title)
{
  //
  // Constructor
  //

 
}

//__________________________________________________________________
IlcTARGETBeamTestDigSPD::~IlcTARGETBeamTestDigSPD()
{
  //
  // Destructor
  //

 }


//_______________________________________________________________________
void IlcTARGETBeamTestDigSPD::Exec(Option_t* /*opt*/)
{
  //Reads raw data for SPD, fill SPD digits tree


  if(!fTARGETgeom){
    Error("Exec","fTARGETgeom is null!");
    return;
  }
  TBranch* branch = fTreeD->GetBranch("TARGETDigitsSPD");

  Int_t nsdd=0;
  Int_t nspd=0;
  Int_t nssd=0;
  for(Int_t nlay=1;nlay<=fTARGETgeom->GetNlayers();nlay++){
    for(Int_t nlad=1;nlad<=fTARGETgeom->GetNladders(nlay);nlad++){
      for(Int_t ndet=1;ndet<=fTARGETgeom->GetNdetectors(nlay);ndet++){
	Int_t index=fTARGETgeom->GetModuleIndex(nlay,nlad,ndet);
	if(fTARGETgeom->GetModuleTypeName(index)=="kSPD") nspd++;
	if(fTARGETgeom->GetModuleTypeName(index)=="kSDD") nsdd++;
	if(fTARGETgeom->GetModuleTypeName(index)=="kSSD") nssd++;
      }
    }
  }
  Int_t maxn=nspd+nsdd+nssd;

  TClonesArray** newdigits = new TClonesArray*[maxn];


  Int_t* idig = new Int_t[maxn];

  for (Int_t idet =0; idet <maxn;idet++){
     newdigits[idet]=new TClonesArray("IlcTARGETdigitSPD");
     idig[idet]=0;  
   }
  

  IlcTARGETRawStreamSPD str(fReader);

  fReader->SelectEquipment(17,211,211);

  while(str.Next()){  

    const IlcRawDataHeader* rdh = fReader->GetDataHeader();
    UChar_t blockAttributes = fReader->GetBlockAttributes();     
    UInt_t statusBits = fReader->GetStatusBits();     
    UInt_t orbitNumber = rdh->fEventID2; 			 
// // // //     UShort_t  bunchCross = rdh->fEventID1;      
      // UInt_t DataSize = rdh->fSize;				      
      //UChar_t L1TrigType = rdh->fL1TriggerType;			      
      //UInt_t MiniEvId = rdh->GetMiniEventID();			      
      // ULong64_t TriggerCL = rdh->GetTriggerClasses();     
      //ULong64_t ROI = rdh->GetROI();
      //      UChar_t Version =rdh->fVersion;                                 
      

    Int_t modID = str.GetModuleID();
    //    Int_t triggernumber = str.GetTriggerEventNumber();

    Int_t row = str.GetRow();
    Int_t col = str.GetColumn();

    const Int_t kdgt[3]={col,row,1};
    //    newdigits = new TClonesArray*[fBt->GetNSPD()];

    new ((*newdigits[modID])[idig[modID]]) IlcTARGETdigitSPD(kdgt);
    
    idig[modID]++;
    
      fTARGETHeader->SetOrbitNumber(0,orbitNumber);
      fTARGETHeader->SetStatusBits(0,statusBits);
      fTARGETHeader->SetBlockAttributes(0,blockAttributes);
// // // //       fTARGETHeader->SetBunchCross(0,bunchCross);
      //fTARGETHeader->SetTriggerClass(0,TriggerCL);
      //fTARGETHeader->SetSubDet(0,
      //fTARGETHeader->SetMiniEvId(0,MiniEvId);
      //fTARGETHeader->SetVersion(0,Version);
      //fTARGETHeader->SetSubDet(0,Subdets);
      //fTARGETHeader->SetL1TriggerType(0,L1TrigType);

   // fTARGETHeader->SetOrbitNumberSPD(OrbitNumber);
         //printf("Bunch Crossing  = %x\n ",BunchCross);
     if ( blockAttributes != 0x3a ) {
       Info("Exec","Block Attribs  = %x\n ",blockAttributes);
     }  
    
     
    
    } // while(str.Next());
    

    for(Int_t n=0;n<maxn;n++){
      branch->SetAddress(&newdigits[n]);
      branch->Fill(); 

   }
    
    fTreeD->SetEntries(maxn);
  
	
    fReader->Reset();
    fTreeD->AutoSave();
   

    for(Int_t n=0;n<maxn;n++){
      delete newdigits[n];
    }

    delete[] newdigits;
    delete[] idig;

}

  
