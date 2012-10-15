void Si_Occ(Char_t DirgIlcRec[100]) 
{
  
  int npoints=0;   
  Char_t DirFastRecpoints2[100];
  Double_t local[3], global[3];
  sprintf(DirFastRecpoints2,"%s/gilc.root",DirgIlcRec);
  IlcRunLoader* rl = IlcRunLoader::Open(DirFastRecpoints2);
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


  TBranch * br=0;
  int nsec=0;
Int_t  Fastlayer0;
Int_t  Fastlayer1;
Int_t  Fastlayer2;
Int_t  Fastlayer3;
Int_t  Fastlayer4;
Double_t  modules[7696];
  TH1F *hlayer0=new TH1F("layer1","Occupancy Layer 1",9,0.,8.);
  TH1F *hlayer1=new TH1F("layer2","Occupancy Layer 2",9,0.,8.);
  TH1F *hlayer2=new TH1F("layer3","Occupancy Layer 3",9,0.,8.);
  TH1F *hlayer3=new TH1F("layer4","Occupancy Layer 4",9,0.,8.);
  TH1F *hlayer4=new TH1F("layer_5","Occupancy Layer 5",9,0.,8.);
  TH1F *hlayer_full=new TH1F("layer_full","Occupancy total",9,0.,8.);
  TH1F *hmodules=new TH1F("module_full","Occupancy tiles",22,0.,21.);
  TH3F *glob_plot=new TH3F("glob_plot","Global coordinates", 10, -100., 100., 10, -100., 100., 10, -100.,100.);

  

  
  for(Int_t i=0;i<n;i++){
    Fastlayer0=0;
    Fastlayer1=0;
    Fastlayer2=0;
    Fastlayer3=0;
    Fastlayer4=0;
    for (Int_t j=0;j<7504;j++) modules[j]=0.;
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
	int layer=0;
        int ladder=0;
        int det=0;
        int module=cli->GetModule();
        //cout <<"module: "<<module<<endl;
        //gm->GetModuleId(module,layer,ladder,det);
        //cout <<"layer: "<<layer<<" ladder: "<<ladder<<" det: "<<det<<endl;
 	//double xm,ym,zm;
 	//gm->GetTrans(layer,ladder,det,xm,ym,zm);
 	double xyzL[3]={cli->GetDetLocalX(),0.0,cli->GetDetLocalZ()};
 	//gm->TrackingV2ToDetL(module,cli->GetY(),cli->GetZ(),xyzLf[0],xyzLf[2]);
 	//double xyzL[3]={xyzLf[0],xyzLf[1],xyzLf[2]};
 	double xyzG[3];
 	gm->LtoG(module,xyzL,xyzG);
        glob_plot->Fill(xyzG[0],xyzG[1],xyzG[2]);
        //cout <<"layer: "<<layer<<endl;
        if (layer==13) { Fastlayer0++; /*cout <<"Fastlayer0: "<<Fastlayer0<<endl;*/}
        if (layer==14) { Fastlayer1++; /*cout <<"Fastlayer1: "<<Fastlayer1<<endl;*/}
        if (layer==15) { Fastlayer2++; /*cout <<"Fastlayer2: "<<Fastlayer2<<endl;*/}
        if (layer==16) { Fastlayer3++; /*cout <<"Fastlayer3: "<<Fastlayer3<<endl;*/}
        if (layer==17) { Fastlayer4++; /*cout <<"Fastlayer4: "<<Fastlayer4<<endl;*/}
        for (Int_t imodule=192;imodule<7504;imodule++) if (module==imodule) modules[imodule]++;
        npoints++;
      }
    }
  
    timer.Stop(); timer.Print();
    hlayer0->Fill(Fastlayer0);
    hlayer1->Fill(Fastlayer1);
    hlayer2->Fill(Fastlayer2);
    hlayer3->Fill(Fastlayer3);
    hlayer4->Fill(Fastlayer4);
    hlayer_full->Fill(Fastlayer0);
    hlayer_full->Fill(Fastlayer1);
    hlayer_full->Fill(Fastlayer2);
    hlayer_full->Fill(Fastlayer3);
    hlayer_full->Fill(Fastlayer4);
    
    for (Int_t s=192;s<7504;s++) if (modules[s]>0) hmodules->Fill(modules[s]);
  }
  rl->CleanFolders();
  rl->RemoveEventFolder();
  rl->UnloadAll();
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(1111);
  hmodules->Scale(1./hmodules->GetEntries()*100.);
  TCanvas *c=new TCanvas();

  c->Divide(1,2);
  c->cd(1);
  hmodules->Draw();
  c->cd(2);
  glob_plot->Draw();
  /*c->cd(3);
  hlayer2->Draw();
  c->cd(4);
  hlayer3->Draw();
  c->cd(5);
  hlayer4->Draw();
  c->cd(6);
  hlayer_full->Draw();
  c->cd(7);
  c->cd();
  hmodules->Draw();*/

  
//  return npoints;
  
}



