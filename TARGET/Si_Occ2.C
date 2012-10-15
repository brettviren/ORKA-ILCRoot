void Si_Occ2(Char_t DirgIlcRec[100]) 
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

  Double_t endcap1_width=4.78815;
  Double_t endcap2_width=4.78815;
  Double_t endcap3_width=4.78815;
  Double_t endcap4_width=5.04697;
  Double_t endcap5_width=0.71952;
  Double_t endcap6_width=1.94373;
  Double_t endcap7_width=3.01524;
  
  IlcTARGETgeom *gm=0;
  rl->CdGAFile();
  IlcTARGETvROOT2 *TARGET = (IlcTARGETvROOT2*)gIlc->GetDetector("TARGET"); 
  gm=(IlcTARGETgeom *)gDirectory->Get("IlcTARGETgeom");//TARGET->GetTARGETgeom();

  TStopwatch timer;

  timer.Start(); cwd->cd(); 

  int n = rl->GetNumberOfEvents();  
  //nevent=n;

  TClonesArray* clits=new TClonesArray("IlcTARGETRecPoint",100);


  TBranch * br=0;
  int nsec=0;
  Double_t  modules[7672];
  TH3F *glob_plot=new TH3F("glob_plot","Global coordinates", 400, -200000., 200000., 400, -200000., 200000., 400, -200000.,200.);
  
  TH1D *n_events=new TH1D("n_events","total events", 2000, 0., 5.);
  TH1D *n_entries=new TH1D("n_entries","total entries", 2000, 0., 5.);
  TH1D *barrels_endcaps=new TH1D("barrels_endcaps","Barrels Endcaps", 20, 0., 2.);
  TH1F *field=new TH1F("field","Magnetic Field Intensity", 200, 0., 100.);
  
    TH2F *local_plot1=new TH2F("Endcap 1","Endcap 1 --- Hits Map", 2000, -200000., 200000., 40, -200000., 200000.);
    TH2F *local_plot2=new TH2F("Endcap 2","Endcap 2 --- Hits Map", 2000, -200000., 200000., 60, -300000., 300000.);  
    TH2F *local_plot3=new TH2F("Endcap 3","Endcap 3 --- Hits Map", 4000, -400000., 400000., 100, -500000., 500000.);
    TH2F *local_plot4=new TH2F("Endcap 4","Endcap 4 --- Hits Map", 4000, -400000., 400000., 120, -600000., 600000.);
    TH2F *local_plot5=new TH2F("Endcap 5","Endcap 5 --- Hits Map", 500, -50000., 50000., 20, -100000., 100000.);
    TH2F *local_plot6=new TH2F("Endcap 6","Endcap 6 --- Hits Map", 500, -50000., 50000., 10, -50000., 50000.);
    TH2F *local_plot7=new TH2F("Endcap 7","Endcap 7 --- Hits Map", 500, -50000., 50000., 10, -50000., 50000.);
    
    TH2F *local_plot1_cut=new TH2F("local_plot1_cut","Occupancy Endcap 1", 2000, -200000., 200000., 40, -200000., 200000.);
    TH2F *local_plot2_cut=new TH2F("local_plot2_cut","Occupancy Endcap 2", 2000, -200000., 200000., 60, -300000., 300000.);  
    TH2F *local_plot3_cut=new TH2F("local_plot3_cut","Occupancy Endcap 3", 4000, -400000., 400000., 100, -500000., 500000.);
    TH2F *local_plot4_cut=new TH2F("local_plot4_cut","Occupancy Endcap 4", 4000, -400000., 400000., 120, -600000., 600000.);
    TH2F *local_plot5_cut=new TH2F("local_plot5_cut","Occupancy Endcap 5", 500, -50000., 50000., 20, -100000., 100000.);
    TH2F *local_plot6_cut=new TH2F("local_plot6_cut","Occupancy Endcap 6", 500, -50000., 50000., 10, -50000., 50000.);
    TH2F *local_plot7_cut=new TH2F("local_plot7_cut","Occupancy Endcap 7", 500, -50000., 50000., 10, -50000., 50000.);

    
    /*TH2F *local_plot1_cut= new TH2F();
    TH2F *local_plot2_cut= new TH2F();
    TH2F *local_plot3_cut= new TH2F();
    TH2F *local_plot4_cut= new TH2F();
    TH2F *local_plot5_cut= new TH2F();
    TH2F *local_plot6_cut= new TH2F();
    TH2F *local_plot7_cut= new TH2F();
    
    local_plot1_cut=local_plot1;
    local_plot2_cut=local_plot2;
    local_plot3_cut=local_plot3;
    local_plot4_cut=local_plot4;
    local_plot5_cut=local_plot5;
    local_plot6_cut=local_plot6;
    local_plot7_cut=local_plot7;*/

  
  for(Int_t i=0;i<n;i++){
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
        n_events->Fill(0);
        n_entries->Fill(0);
        npoints++;
        IlcTARGETRecPoint* cli=(IlcTARGETRecPoint*)(clits->At(nc));
        int layer=cli->GetLayer();
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
        if ((layer<18) && (layer>=13)) barrels_endcaps->Fill(0);
        if ((layer>=18) && (layer<=31)) barrels_endcaps->Fill(1);
        if ((layer==18) || (layer==25)) {
          if ((xyzL[0]<=endcap1_width) && (xyzL[0]>=-endcap1_width)) local_plot1_cut->Fill(xyzL[0]*10000.,xyzL[2]*10000.);
          local_plot1->Fill(xyzL[0]*10000.,xyzL[2]*10000.);
        }
        
        if ((layer==19) || (layer==26)) {
          if ((xyzL[0]<=endcap2_width) && (xyzL[0]>=-endcap2_width)) local_plot2_cut->Fill(xyzL[0]*10000.,xyzL[2]*10000.);
          local_plot2->Fill(xyzL[0]*10000.,xyzL[2]*10000.);
        }
        
        if ((layer==20) || (layer==27)) {
          if ((xyzL[0]<=endcap3_width) && (xyzL[0]>=-endcap3_width)) local_plot3_cut->Fill(xyzL[0]*10000.,xyzL[2]*10000.);
          local_plot3->Fill(xyzL[0]*10000.,xyzL[2]*10000.);
        }
        
        if ((layer==21) || (layer==28)) {
          if ((xyzL[0]<=endcap4_width) && (xyzL[0]>=-endcap4_width)) local_plot4_cut->Fill(xyzL[0]*10000.,xyzL[2]*10000.);
          local_plot4->Fill(xyzL[0]*10000.,xyzL[2]*10000.);
        }
        
        if ((layer==22) || (layer==29)) {
          if ((xyzL[0]<=endcap5_width) && (xyzL[0]>=-endcap5_width)) local_plot5_cut->Fill(xyzL[0]*10000.,xyzL[2]*10000.);
          local_plot5->Fill(xyzL[0]*10000.,xyzL[2]*10000.);
        }
        
        if ((layer==23) || (layer==30)) {
          if ((xyzL[0]<=endcap6_width) && (xyzL[0]>=-endcap6_width))
           local_plot6_cut->Fill(xyzL[0]*10000.,xyzL[2]*10000.);
           local_plot6->Fill(xyzL[0]*10000.,xyzL[2]*10000.);
        }
        
        if ((layer==24) || (layer==31)) {
          if ((xyzL[0]<=endcap7_width) && (xyzL[0]>=-endcap7_width))
           local_plot7_cut->Fill(xyzL[0]*10000.,xyzL[2]*10000.);
           local_plot7->Fill(xyzL[0]*10000.,xyzL[2]*10000.);
          
        }
        
        glob_plot->Fill(xyzG[0],xyzG[1],xyzG[2]);
      }
    }
  }
  
  //n_entries->Fill(npoints);
  //n_events->Fill(n);
  Double_t x[3]={1.,1.,1.};
  Double_t B[3];
  gIlc->Field(x,B);
  //cout <<"campo: "<<gIlc->Field()->SolenoidField()/10.<<endl;
  cout <<"campo: "<<B[2]<<endl;
  field->Fill(B[2]/10.);
  
  rl->CleanFolders();
  rl->RemoveEventFolder();
  rl->UnloadAll();
//  TCanvas *canv=new TCanvas();
  gStyle->SetOptStat(1111);
  gStyle->SetOptFit(111);
  

//  canv->Divide(7,2);
//  glob_plot->Draw();
  
  TFile *fileocc=new TFile("Endcap_occupancy.root","RECREATE");
  fileocc->cd();
  
  //cout <<"n: "<<n<<endl;
  TH1D *local_plot1_prY=local_plot1_cut->ProjectionY();
  local_plot1_prY->SetTitle("Endcap 1 --- Integrated Occupancy of 4 strips");
  local_plot1_prY->SetName("Endcap 1");
  local_plot1_prY->SetStats(kFALSE);
  TH1D *local_plot1_prX=local_plot1_cut->ProjectionX();
  local_plot1_prX->SetTitle("Endcap 1 --- Integrated Occupancy of 4 strips");
  local_plot1_prX->SetName("Endcap 1");
  local_plot1_cut->Write();
  local_plot1_prY->Scale(1./(n*24.*endcap1_width*2.*10000./200.));
  local_plot1_prY->Write();
  local_plot1_prX->Scale(1./n/24.);
  local_plot1_prX->Fit("pol0","","",-endcap1_width*10000.,endcap1_width*10000.);
  local_plot1_prX->Write();
  local_plot1->Write();
  
  TH1D *local_plot2_prY=local_plot2_cut->ProjectionY();
  local_plot2_prY->SetTitle("Endcap 2 --- Integrated Occupancy of 4 strips");
  local_plot2_prY->SetName("Endcap 2");
  local_plot2_prY->SetStats(kFALSE);
  TH1D *local_plot2_prX=local_plot2_cut->ProjectionX();
  local_plot2_prX->SetTitle("Endcap 2 --- Integrated Occupancy of 4 strips");
  local_plot2_prX->SetName("Endcap 2");
  local_plot2_cut->Write();
  local_plot2_prY->Scale(1./(n*24.*endcap2_width*2.*10000./200.));
  local_plot2_prY->Write();
  local_plot2_prX->Scale(1./n/24.);
  local_plot2_prX->Fit("pol0","","",-endcap2_width*10000.,endcap2_width*10000.);
  local_plot2_prX->Write();
  local_plot2->Write();
  

  TH1D *local_plot3_prY=local_plot3_cut->ProjectionY();
  local_plot3_prY->SetTitle("Endcap 3 --- Integrated Occupancy of 4 strips");
  local_plot3_prY->SetName("Endcap 3");
  local_plot3_prY->SetStats(kFALSE);
  TH1D *local_plot3_prX=local_plot3_cut->ProjectionX();
  local_plot3_prX->SetTitle("Endcap 3 --- Integrated Occupancy of 4 strips");
  local_plot3_prX->SetName("Endcap 3");
  local_plot3_cut->Write();
  local_plot3_prY->Scale(1./(n*24.*endcap3_width*2.*10000./200.));
  local_plot3_prY->Write();
  local_plot3_prX->Scale(1./n/24.);
  local_plot3_prX->Fit("pol0","","",-endcap3_width*10000.,endcap3_width*10000.);
  local_plot3_prX->Write();
  local_plot3->Write();
  
  TH1D *local_plot4_prY=local_plot4_cut->ProjectionY();
  local_plot4_prY->SetTitle("Endcap 4 --- Integrated Occupancy of 4 strips");
  local_plot4_prY->SetName("Endcap 4");
  local_plot4_prY->SetStats(kFALSE);
  TH1D *local_plot4_prX=local_plot4_cut->ProjectionX();
  local_plot4_prX->SetTitle("Endcap 4 --- Integrated Occupancy of 4 strips");
  local_plot4_prX->SetName("Endcap 4");
  local_plot4_cut->Write();
  local_plot4_prY->Scale(1./(n*24.*endcap4_width*2.*10000./200.));
  local_plot4_prY->Write();
  local_plot4_prX->Scale(1./n/24.);
  local_plot4_prX->Fit("pol0","","",-endcap4_width*10000.,endcap4_width*10000.);
  local_plot4_prX->Write();
  local_plot4->Write();

  TH1D *local_plot5_prY=local_plot5_cut->ProjectionY();
  local_plot5_prY->SetTitle("Endcap 5 --- Integrated Occupancy of 4 strips");
  local_plot5_prY->SetName("Endcap 5");
  local_plot5_prY->SetStats(kFALSE);
  TH1D *local_plot5_prX=local_plot5_cut->ProjectionX();
  local_plot5_prX->SetTitle("Endcap 5 --- Integrated Occupancy of 4 strips");
  local_plot5_prX->SetName("Endcap 5");
  local_plot5_cut->Write();
  local_plot5_prY->Scale(1./(n*24.*endcap5_width*2.*10000./200.));
  local_plot5_prY->Write();
  local_plot5_prX->Scale(1./n/24.);
  local_plot5_prX->Fit("pol0","","",-endcap5_width*10000.,endcap5_width*10000.);
  local_plot5_prX->Write();
  local_plot5->Write();

  TH1D *local_plot6_prY=local_plot6_cut->ProjectionY();
  local_plot6_prY->SetTitle("Endcap 6 --- Integrated Occupancy of 4 strips");
  local_plot6_prY->SetName("Endcap 6");
  local_plot6_prY->SetStats(kFALSE);
  TH1D *local_plot6_prX=local_plot6_cut->ProjectionX();
  local_plot6_prX->SetTitle("Endcap 6 --- Integrated Occupancy of 4 strips");
  local_plot6_prX->SetName("Endcap 6");
  local_plot6_cut->Write();
  local_plot6_prY->Scale(1./(n*24.*endcap6_width*2.*10000./200.));
  local_plot6_prY->Write();
  local_plot6_prX->Scale(1./n/24.);
  local_plot6_prX->Fit("pol0","","",-endcap6_width*10000.,endcap6_width*10000.);
  local_plot6_prX->Write();
  local_plot6->Write();

  TH1D *local_plot7_prY=local_plot7_cut->ProjectionY();
  local_plot7_prY->SetTitle("Endcap 7 --- Integrated Occupancy of 4 strips");
  local_plot7_prY->SetName("Endcap 7");
  local_plot7_prY->SetStats(kFALSE);
  TH1D *local_plot7_prX=local_plot7_cut->ProjectionX();
  local_plot7_prX->SetTitle("Endcap 7 --- Integrated Occupancy of 4 strips");
  local_plot7_prX->SetName("Endcap 7");
  local_plot7_cut->Write();
  local_plot7_prY->Scale(1./(n*24.*endcap7_width*2.*10000./200.));
  local_plot7_prY->Write();
  local_plot7_prX->Scale(1./n/24.);
  local_plot7_prX->Fit("pol0","","",-endcap7_width*10000.,endcap7_width*10000.);
  local_plot7_prX->Write();
  local_plot7->Write();
  
  n_events->Write();
  n_entries->Write();
  barrels_endcaps->Write();
  field->Write();
  
  fileocc->Close();

  n_events->Delete();
  n_entries->Delete();
  barrels_endcaps->Delete();
  field->Delete();
  local_plot1->Delete();
  local_plot2->Delete();
  local_plot3->Delete();
  local_plot4->Delete();
  local_plot5->Delete();
  local_plot6->Delete();
  local_plot7->Delete();
  local_plot1_cut->Delete();
  local_plot2_cut->Delete();
  local_plot3_cut->Delete();
  local_plot4_cut->Delete();
  local_plot5_cut->Delete();
  local_plot6_cut->Delete();
  local_plot7_cut->Delete();
}



