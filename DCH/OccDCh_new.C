Int_t OccDCh_new(Int_t num_events)
{

 Long_t numhit_det[21000000];
 Long_t num_ring[2100000];
 Long_t num_superlayer[2100000];
 Long_t ringtot[300000];
 Long_t num_wire_tot[2100000];
 Long_t  tot[300000];
   TFile file("Occ_Dch.root","RECREATE");
   TH2F *OccPlot=new TH2F("Occ 1","Drift Chamber Occupancy 1",100,0.,2200000.,100,0.,50.);
   TH1F *OccPlot2=new TH1F("Occ 2","Drift Chamber Occupancy 2",100,0.,50.);
   TH2F *OccPlot3=new TH2F("Occ 3","Drift Chamber Occupancy 3",100,0.,200.,100,0.,100.);
   TH2F *OccPlot4=new TH2F("Occ 4","Drift Chamber Occupancy 4",100,0.,200.,100,0.,5.);
   IlcRunLoader *rl = IlcRunLoader::Open("gilc.root");
   rl->LoadgIlc();
   rl->LoadHeader();
   rl->LoadKinematics();
   rl->LoadHits();
   rl->LoadTrackRefs();



  if (!rl) {
    cout << "<OccDCh> No RunLoader found" << endl;
    return 1;
  }

 // rl->GetEvent(event);

for (Int_t event=0;event<num_events;event++)
{

  for (Int_t i=0;i<2100000;i++) numhit_det[i]=0;
  Int_t countHits = 0;

  rl->GetEvent(event);

  
   IlcLoader *loader = (IlcLoader *)rl->GetLoader("DCHLoader");
  if (!loader) {
    cout << "<OccDCh> No DCHLoader found" << endl;
    return 2;
  }


   IlcDCH *dch=(IlcDCH*)rl->GetIlcRun()->GetDetector("DCH");
  if (!dch) {
    cout << "<OccDch> No DCH detector found" << endl;
    return 3;
  }
  IlcDCHgeometry *geo;
  if (dch) {
    geo = (IlcDCHgeometry *) dch->GetGeometry();
  }
  else {
    cout << "<OccDCh> No DCH geometry found" << endl;
    return 4;
  }

  TTree *hitTree = loader->TreeH();  
  if (!hitTree) {
    cout << "<OccDCh> No hit tree found" << endl;
    return 5;
  }



  Int_t nBytes    = 0;
  Int_t nTrack = (Int_t) hitTree->GetEntries();
  cout << "<OccDCh> Event: "<<event<< " -- Found " << nTrack 
       << " primary particles with hits" << endl;

  // Loop through all entries in the tree
  for (Int_t iTrack = 0; iTrack < nTrack; iTrack++) {

    //gIlc->ResetHits();
    nBytes += hitTree->GetEvent(iTrack);

    // Loop through the DCH hits  
    Int_t iHit = 0;
    IlcDCHhit *hit = (IlcDCHhit *) dch->FirstHit(-1);
    while (hit) {

     
      iHit++;

  //    Float_t x     = hit->X();
  //    Float_t y     = hit->Y();
  //    Float_t z     = hit->Z();
  //    Float_t q     = hit->GetCharge();
      Int_t   track = hit->Track();
      
      Int_t   status   = hit->GetStatus();
      Int_t   superlayer=hit->GetSuperlayer();
      Int_t   ring=hit->GetRing();
      Int_t   wire=hit->GetWire();
      //cout <<"superlayer:"<<superlayer<<endl;
      Long_t det=(Long_t)(superlayer*100000)+(ring*1000)+wire;
      //cout <<"det: "<<det<<endl;
      //cout <<"det1: "<<det<<endl;
      //cout <<"status: "<<status<<endl;
      //for (Int_t det2=0;det2<1000; det++)
      //   {
      //    if (det2==det) numhit_det[det]++;
      //   }
     
       
        
      //Int_t   plane = geo->GetPlane(det);
     //cout <<"track: "<<track<<endl;

      if ((status!=1) && (status!=2)) {  hit = (IlcDCHhit *) dch->NextHit(); continue;} 
      else
        {
         //cout <<"status: "<<status<<endl;  
         countHits++;
         numhit_det[det]++;
         num_ring[det]=ring;
         num_superlayer[det]=superlayer;
         //cout <<"num det: "<<numhit_det[det]<<endl;
        }
      
      hit = (IlcDCHhit *) dch->NextHit();
    }
  }
  cout << "<<OccDCh>> Found " << countHits << " hits in total" << endl;

    for (Int_t i=0;i<2100000;i++) {
    if (numhit_det[i]) {
     //cout <<"numero del superlayer:"<<num_superlayer[i]<<endl;
      //   Int_t num_wire=(80+((num_superlayer[i]-1)*20));
      // for (Int_t ringi=0;ringi<num_wire;ringi++) {
       
      // ringtot[(num_ring[i]-1)+10*(num_superlayer[i]-1)]++;
      //  num_wire_tot[(num_ring[i]-1)+10*(num_superlayer[i]-1)]=num_wire;
      // }
     OccPlot->Fill(i,numhit_det[i]);
     OccPlot2->Fill(numhit_det[i]);
    }
  }
  
    Int_t wire2=0;
  for (Int_t super=1;super<=20;super++)
    for (Int_t ringi=1;ringi<=10;ringi++)
      {
      
	if (event==0) { 
                      ringtot[(ringi-1)+10*super]=0;
                      num_wire_tot[(ringi-1)+10*super]=0;
	}
	Int_t num_wire=(80+((super-1)*20));
	for (Int_t wire=1;wire<=num_wire;wire++) {
          if ((ringi%2)!=0) wire2=2*wire-1; else wire2=2*wire;
             
	  if (numhit_det[(super*100000)+(ringi*1000)+wire2]){
	    ringtot[(ringi-1)+10*super]++;
            //cout<<"Entra qui"<<endl;
//cout <<"ringtot[(ringi-1)+10*super]: "<<ringtot[(ringi-1)+10*super]<<endl;
	    num_wire_tot[(ringi-1)+10*super]+=numhit_det[(super*100000)+(ringi*1000)+wire2];
	  }
	}
        //cout <<"num_wire_tot[ringi+10*super]: "<<num_wire_tot[(ringi-1)+10*super]<<endl;
        //cout <<"2) ringtot[(ringi-1)+10*super]: "<<ringtot[(ringi-1)+10*super]<<endl;

        if (event==num_events-1){
          
          //cout <<"(ringi+10*super): "<<((ringi-1)+10*super)<<" (ringtot[ringi+10*super]): "<<(ringtot[(ringi-1)+10*super])<<endl;
          //cout <<"num_wire:"<<num_wire<<endl;
          //cout <<"super: "<<super<<endl;
          //cout <<"ringi: "<<ringi<<endl;
	  OccPlot3->Fill(((ringi-1)+10*(super-1)), ((ringtot[(ringi-1)+10*super])/((Float_t)num_wire)*100.)/((Float_t)num_events));
          //cout <<"ringtot:"<<ringtot[(ringi-1)+10*super]<<endl;
          if (ringtot[(ringi-1)+10*super]==0) continue;
          OccPlot4->Fill(((ringi-1)+10*(super-1)),num_wire_tot[(ringi-1)+10*super]/((Float_t)ringtot[(ringi-1)+10*super]));
        }
      }    
    

  // for (Long_t j=0;j<300000; j++) {
  //  if (ringtot[j]) {
  //   tot[j]++;
     //  cout <<"j: "<<j<<endl;
     // cout <<"ringtot[j]: "<<ringtot[j];
  //  }
  // }



}
// for (Int_t k=0;k<300000;k++)
//   {
//     if (tot[k]){
       //cout <<"num_wire_tot[k]: "<<num_wire_tot[k]<<endl;
//   OccPlot3->Fill(k,((tot[k]/num_wire_tot[k])*100)/num_events);
//     }
//   } 
 //cout <<"totale hit calcolati: "<<tot<<endl;

  TCanvas *cHits = new TCanvas();
  cHits->Divide(4);
  cHits->cd(1);
  OccPlot->SetXTitle("ID cells number");
  OccPlot->SetYTitle("track number thru");
  OccPlot->Draw();
  cHits->cd(2);
  OccPlot2->SetXTitle("cells");
  OccPlot2->Draw();
  cHits->cd(3);
  OccPlot3->SetXTitle("rings");
  OccPlot3->SetYTitle("cell %");
  cHits->cd(4);
  OccPlot4->SetXTitle("rings");
  OccPlot4->SetYTitle("cell molteplicity");

  file.cd();
  OccPlot->Write();
  OccPlot2->Write();
  OccPlot3->Write();
  OccPlot4->Write();
  file.Close();
  return 0;
}
