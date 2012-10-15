Int_t GetRecPoints(Char_t DirgIlcRec[100],Double_t x[100000],Double_t y[100000],
		   Double_t z[100000],Double_t ey[100000],Double_t ez[100000],
		   Int_t layers[100000],Int_t ladders[100000],Int_t module[100000],Int_t label[100000], 
		   Int_t npointsevent[1000],int& nevent) 
{
  
  int npoints=0;   
  IlcRunLoader* rl = IlcRunLoader::Open(DirgIlcRec);
  if (rl == 0x0){cerr<<"Can not open session"<<endl;return 0;}
  IlcTARGETLoader *itsl = (IlcTARGETLoader*)rl->GetLoader("TARGETLoader");
  if (itsl == 0x0){cerr<<"Can not get TARGET Loader"<<endl;return 0;}
  if (itsl->LoadHits()){cerr<<"Error occured while loading hits"<<endl;return 0;}
  if (itsl->LoadRecPoints()){cerr<<"Error occured while loading digits"<<endl;return 0;}
  if (rl->LoadgIlc()){cerr<<"Error occured while l"<<endl;return 0;}

  rl->LoadKinematics();
  rl->LoadTrackRefs();
  rl->LoadHeader();
  rl->LoadHits();

  gIlc=rl->GetIlcRun();
  if (!gIlc) {cerr<<"Can't get gIlc !\n";return 0;}

  TDirectory *cwd = gDirectory;

  IlcTARGETgeom *gm=0;
  rl->CdGAFile();
  IlcTARGETvROOT2 *TARGET = (IlcTARGETvROOT2*)gIlc->GetDetector("TARGET"); 
  gm=(IlcTARGETgeom *)gDirectory->Get("IlcTARGETgeom");//TARGET->GetTARGETgeom();

  TStopwatch timer;

  timer.Start(); cwd->cd(); 

  int n = rl->GetNumberOfEvents();  
  nevent=n;
  TClonesArray* clits=new TClonesArray("IlcTARGETRecPoint",100);
  struct xyz{double x,y,z;int n;};
  xyz clpoint;

  bool onlyOne=false;
  TBranch * br=0;
  int nsec=0;

  for(Int_t i=0;i<n;i++){
    printf("Processing event %d\n",i);
    rl->GetEvent(i);
    br = itsl->TreeR()->GetBranch("TARGETRecPoints");
    br->SetAddress(&clits);
    
    nsec=Int_t(itsl->TreeR()->GetEntries());
    for (Int_t ns=0; ns<nsec; ns++) {
      br->GetEntry(ns);
      int nclust =clits->GetEntriesFast();
      for (Int_t nc=0;nc<nclust;nc++) {
	IlcTARGETRecPoint* cli=(IlcTARGETRecPoint*)(clits->At(nc));
	int layer=cli->GetLayer(),ladder=cli->GetDetectorIndex();
        int mod=cli->GetModule();
	//int ind=gm->GetModuleIndex(layer+1,ladder+1,det+1);
	
	double xyzG[3]={cli->GetDetLocalX(),0,cli->GetDetLocalZ()};
// 	double xm,ym,zm;
// 	gm->GetTrans(layer+1,ladder+1,det+1,xm,ym,zm);
// 	float xyzLf[3]={0,TMath::Hypot(xm,ym),0};
// 	gm->TrackingV2ToDetL(ind,cli->GetY(),cli->GetZ(),xyzLf[0],xyzLf[2]);
// 	double xyzL[3]={xyzLf[0],xyzLf[1],xyzLf[2]};
// 	double xyzG[3];
// 	gm->LtoG(layer+1,ladder+1,1,xyzL,xyzG);
	
// 	if((cli->GetLabel(0)>=0&&!onlyOne)||(cli->GetLabel(0)==0&&cli->GetLabel(1)<0)){
	  
	  x[npoints]=xyzG[0];
	  y[npoints]=xyzG[1];
	  z[npoints]=xyzG[2];
	  
	  ey[npoints]=TMath::Sqrt(cli->GetSigmaY2());
	  ez[npoints]=TMath::Sqrt(cli->GetSigmaZ2());
	  
	  layers[npoints]=layer;
	  ladders[npoints]=ladder;
          module[npoints]=mod;
	  label[npoints]=cli->GetLabel(0);
	  
	  npoints++;
	  
// 	}
      }
    }
    npointsevent[i]=npoints;
    cout<<"Recpoints: "<<npoints<<endl;
    timer.Stop(); timer.Print();
  }
  
  rl->CleanFolders();
  rl->RemoveEventFolder();
  rl->UnloadAll();

  return npoints;
  
}



void compareTARGET(Char_t DirRecpoints[100],Char_t DirFastRecpoints[100])
{

  Double_t xFast[100000], yFast[100000], zFast[100000], eyFast[100000], ezFast[100000], 
    xSlow[100000],ySlow[100000],zSlow[100000],eySlow[100000],ezSlow[100000];
  
  Int_t nFastPoints[1000];
  Int_t nPoints[1000];
  Int_t Fastlayers[100000],Fastladders[100000],Fastmodules[100000],Fastlabels[100000];
  Int_t layers[100000],ladders[100000],labels[100000],modules[100000];
  int nevents;
  Double_t diffx[100000],diffz[100000];
  Char_t DirFastRecpoints2[100];
  Char_t DirRecpoints2[100];
  
  TH1F *hdiffx=new TH1F("diffx","Deviation Global Y",800,-100.,100.);
  TH1F *hdiffz=new TH1F("diffz","Deviation Global Z",800,-2000.,2000.);

  TH1F *hdiffpullx=new TH1F("diffpullx","Deviation Global Pull Y",50000,-150.,150.);
  TH1F *hdiffpullz=new TH1F("diffpullz","Deviation Global Pull Z",50000,-150.,150.);

  sprintf(DirFastRecpoints2,"%s/gilc.root",DirFastRecpoints);
  sprintf(DirRecpoints2,"%s/gilc.root",DirRecpoints);

  cout <<"Loading Fast Recpoints..."<<endl;
  
  if (!(GetRecPoints(DirFastRecpoints2,xFast,yFast,zFast,eyFast,ezFast,
		     Fastlayers,Fastladders,Fastmodules,Fastlabels,nFastPoints,
		     nevents))) {
    cout <<"Error getting FastRecPoints"<<endl; 
    exit(1);
  } 

  cout <<"Loading Slow Recpoints..."<<endl;

  if (!(GetRecPoints(DirRecpoints2,xSlow,ySlow,zSlow,eySlow,ezSlow,
		     layers,ladders,modules,labels,nPoints,
		     nevents)))  {
    cout <<"Error getting RecPoints"<<endl; 
    exit(1);
  }

  for(int nev=0;nev<nevents;nev++){
    for (Int_t i=(nev?nPoints[nev-1]:0);i<nPoints[nev];i++){
      for (Int_t j=(nev?nFastPoints[nev-1]:0);j<nFastPoints[nev];j++){
	
	if ((labels[i]==Fastlabels[j])&&(modules[i]==Fastmodules[j])) {
//          cout <<"zFast[j]: "<<zFast[j]<<"--- zSlow[i]: "<<zSlow[i]<<endl;
//	  if (labels[i]<=0) {cout <<"la label less a zero"<<endl;}
	  
	  diffx[i]=(xFast[j]-xSlow[i])*10000.;
	  diffz[i]=(zFast[j]-zSlow[i])*10000.;
	  
	 /* if (TMath::Abs(diffx[i])>=50.)   
	    {
             cout <<"diffx["<<i<<"]: "<<diffx[i]<<" labels[i] "<<labels[i]<<" "<<Fastlabels[j]<<endl; 
             continue;
            }
	  
	  if (TMath::Abs(diffz[i])>=3000.)   
	    {
             cout <<"diffz["<<i<<"]: "<<diffz[i]<<" labels[i] "<<labels[i]<<" "<<Fastlabels[j]<<endl;
             continue;
            }*/
	  
	  
	  hdiffx->Fill(diffx[i]);
	  hdiffz->Fill(diffz[i]);
	  hdiffpullx->Fill(diffx[i]/eySlow[i]/10000);
	  hdiffpullz->Fill(diffz[i]/ezSlow[i]/10000);
	}
      }
      
    }
  }

  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(1111);
  TCanvas *c=new TCanvas();

  c->Divide(1,2);
  c->cd(1);
  hdiffx->Fit("gaus");
  hdiffx->Draw();
  c->cd(2);
  hdiffz->Fit("gaus");
  hdiffz->Draw();
  /*c->cd(3);
  hdiffpullx->Fit("gaus");
  hdiffpullx->Draw();
  c->cd(4);
  hdiffpullz->Fit("gaus");
  hdiffpullz->Draw();
  */TFile *file= new TFile("FastVsSlow.root","RECREATE");
  file->cd();
  c->Write();
  c->Close();

//c.Update();
}

