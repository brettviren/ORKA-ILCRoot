#if !defined(__CINT__) || defined(__MAKECINT__)


#include <TVector.h>
#include <TMath.h>
#include "Riostream.h"
#include <TROOT.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <TObjArray.h>
#include <TTree.h>
#include <TArray.h>
#include <TCollection.h>
#include "IlcRun.h"
#include "IlcCluster.h"

#include "IlcDCH.h"
#include "IlcComplexCluster.h"
#include "IlcESD.h"
#include "IlcKink.h"
#include "IlcV0.h"
#include "IlcHelix.h"
#include "IlcRunLoader.h"
#include "IlcDCHReconstructor.h"
#include "IlcDCHpolyTrack.h"
#include "IlcDCHseed.h" 
#include "IlcDCHtracker.h"
#include "TStopwatch.h"
#include "IlcDCHReconstructor.h"
#include "IlcPID.h"
#include "TTreeStream.h"
#include "IlcAlignObj.h"
#include "IlcTrackPointArray.h"
#include "IlcDCHRecoParam.h"
#include "IlcDCHParam.h"
#include "IlcDCHwireposition.h"

#include <map>

#endif

/********************How to use********************

put the macros in the folder of signal
check the path of th the RecPoint file of the background 
compile it using:

gSystem->AddIncludePath("-I$ILC_ROOT/include -I$ILC_ROOT/DCH "); 
gROOT->LoadMacro("$ILC_ROOT/DCH/mergeDCHcluster.C+");  

to execute:

mergeDCHcluster("path/name of the RecPoint fil of the background file")



********************How to use**********************/

using namespace std;

void mergeDCHcluster(char *file="./DCH.RecPoints_Bck.root")

{



  IlcRunLoader* rl = IlcRunLoader::Open("gilc.root");

  
  if (rl == 0x0){cerr<<"Can not open session"<<endl;return;}
  if (rl->LoadgIlc()){cerr<<"Error occured while l"<<endl;return ;}
 
  gIlc=rl->GetIlcRun();  
  if (!gIlc) {cerr<<"Can't get gIlc !\n";return ;}
  
  IlcLoader *dchl = (IlcLoader*)rl->GetLoader("DCHLoader");
  if (dchl == 0x0) {  cerr<<"Can not get DCH Loader"<<endl; return ; }
  
  if (dchl->LoadRecPoints()) {cerr<<"Error occured while loading digits"<<endl;return ; }
  IlcDCH *dchdet=(IlcDCH*)gIlc->GetDetector("DCH");
  IlcDCHParam *param=dchdet->GetParam();


  TClonesArray* clrow=new TClonesArray("IlcDCHcluster",100);
  TClonesArray* clrow_bck=new TClonesArray("IlcDCHcluster",100);
  
  TClonesArray clNew("IlcDCHcluster",100);//TClonesArray per new file TClonesArray clNew("IlcDCHcluster",100);//TClonesArray per new file

  cout<<"BackGround File: "<<file<<endl;
  TFile *file_bck = TFile::Open(file); //Open the BackgroundFile
  
  if(!file_bck) { 
    cout<<"BackGround File not present "<<endl;
    cout<<"The macro will be closed "<<endl;
    return;
  }
  
  //output file
  TFile myFile("DCH.RecPoints_new.root","recreate");
  
//architecture of the output file: TreeR with a Branch named DCHcluster
  TClonesArray *pclNew=&clNew;

  Int_t Label[3]; //array used to store the Label of the track befor its shift of 10000000

  for(int a=0;a<3;a++)Label[a]=-1;//Reset the array

  ULong_t cellID=0; //used to take the "Det" univoque number of illumined cell in the DCH
  
  int n = rl->GetNumberOfEvents(); //after the opening of the current session, I take the number of events
  
  for(Int_t i=0;i<n;i++){ 
    //events of signal or background, depend if we are working in signal folder and the external file is the background file or vice versa
    
    printf("Processing event %d \n",i);
    rl->GetEvent(i);
    
    //make in the new output file (myFile) the same architecture of the existing files File->Folder(called Event)->TreeR->Branch 
    myFile.mkdir(Form("Event%i",i));
    myFile.cd(Form("Event%i",i));
  
    TTree myTree("TreeR","Reconstructed Points Container"); 
    myTree.Branch("DCHcluster",&pclNew);

    TString treename="Event";treename+=i;treename+="/TreeR";
    TTree* tr_bck=(TTree*)file_bck->Get(treename);
  
    if(!tr_bck)  {
      //TTree *ttmp = (TTree*)gROOT->FindObject("TreeR");
      //pcl1=(IlcDCHcluster*)ttmp;
      //myTree.Write();
      cout<<"Event "<<i<<" not present in the external file: "<<file<<endl;
      //       continue;
    }
  
    cout<<"Doing the Merge for the event: "<<i<<endl;
  
    TBranch * br = dchl->TreeR()->GetBranch("DCHcluster");
    br->SetAddress(&clrow);
    
  
    Int_t nSign=Int_t(dchl->TreeR()->GetEntries());

    std::map <int,std::vector<IlcDCHcluster* > > mrecs; 
    //definition of a new map(called mrecs), it will have 2 list of object IlcDCHcluster, one for the signal and one for the background
    
    for (Int_t ns=0; ns<nSign; ns++) {//loop over entries for signal
      br->GetEntry(ns);

      int nclust =clrow->GetEntriesFast();
      for(int nc=0;nc<nclust;nc++){//Signal
	IlcDCHcluster* cl=(IlcDCHcluster*)(clrow->At(nc));
	
	if(!cl) continue;

	cellID=cl->GetId();
	mrecs[cellID].push_back(new IlcDCHcluster(*cl));
      }
    }//end loop over entries of signal

    if(tr_bck){
      TBranch * br_bck = tr_bck->GetBranch("DCHcluster");
      br_bck->SetAddress(&clrow_bck);
    
      Int_t nBck=Int_t(tr_bck->GetEntries());
      
      for (Int_t ns_bck=0; ns_bck<nBck; ns_bck++){//loop over entries for Background
	br_bck->GetEntry(ns_bck);
	
	int nclust_bck =clrow_bck->GetEntriesFast();
	
	for (int nc_bck=0;nc_bck<nclust_bck;nc_bck++) {//Background cluster
	  IlcDCHcluster *cl_bck=(IlcDCHcluster*)(clrow_bck->At(nc_bck));
	  
	  if(!cl_bck) continue;
	  
	  cellID=cl_bck->GetId();
	  for(int a=0;a<3;a++){
	    Label[a]=cl_bck->GetLabel(a);
	    if (Label[a]>=0){
	      Label[a]+=10000000;
	      cl_bck->SetLabel(Label[a],a);
	    }
	  }
	  
	  mrecs[cellID].push_back(new IlcDCHcluster(*cl_bck));
	}
      }//end loop over entries of background
    }

    for(std::map<int,std::vector<IlcDCHcluster* > >::iterator it=mrecs.begin();it!=mrecs.end();it++){
      int svec=(*it).second.size();
      
      if((*it).second.size()<=2){
	for(int i=0;i<svec;i++) {
	  IlcDCHcluster *pippo=dynamic_cast<IlcDCHcluster*>((*it).second[i]);
	  new (clNew[clNew.GetEntriesFast()]) IlcDCHcluster(*pippo);
	}
      }
      
      Double_t Imp[svec];
      Int_t Min[svec];
      
      if((*it).second.size()>2){
	
	for(int i=0;i<svec;i++) {
	  Imp[i]=(*it).second[i]->GetImP();
	}
		
	TMath::Sort(svec,Imp,Min,0);
  	
	IlcDCHcluster *pippo=dynamic_cast<IlcDCHcluster*>((*it).second[Min[0]]);
	new (clNew[clNew.GetEntriesFast()]) IlcDCHcluster(*pippo);
	  
		
	IlcDCHcluster *goofy=dynamic_cast<IlcDCHcluster*>((*it).second[Min[1]]);
	if(goofy->GetSigmaImP2()<1.01*param->GetSigmaFrsTrk()*param->GetSigmaFrsTrk()){
	  goofy->SetImP(goofy->GetImP()+gRandom->Gaus()*
			TMath::Sqrt(param->GetSigmaSecTrk()*param->GetSigmaSecTrk()-
				    goofy->GetSigmaImP2()));
	  goofy->SetSigmaImP2(param->GetSigmaSecTrk()*param->GetSigmaSecTrk());
	}
	new (clNew[clNew.GetEntriesFast()]) IlcDCHcluster(*(*it).second[Min[1]]);		
      }//end second check over the dimension of the maps
      for(int i=0;i<svec;i++) delete (*it).second[i];
	
    }//end check in the map
    cout<<"Number of merged clusters: "<<clNew.GetEntriesFast()<<endl;
    myTree.Fill();
    myTree.Write(); 
    
    clNew.Delete();
      
  }//end loop over signal events 
 
  myFile.cd();
  myFile.Close();
}

