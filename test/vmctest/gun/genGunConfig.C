/// $Id: genGunConfig.C 48253 2011-03-09 17:42:20Z ivana $
//
// IlcRoot Configuration for gun vmctest
//
// Extracted from G3 specific Config.C in test/gun.
// by I. Hrivnacova, IPN Orsay

Float_t EtaToTheta(Float_t arg);

IlcGenerator* genGunConfig()
{
  cout << "Running genGunConfig.C ... " << endl;

  //=======================================================================
  // Event generator
  //=======================================================================

  // The cocktail itself

  IlcGenCocktail *gener = new IlcGenCocktail();
  gener->SetPhiRange(0, 360);
  // Set pseudorapidity range from -8 to 8.
  Float_t thmin = EtaToTheta(8);   // theta min. <---> eta max
  Float_t thmax = EtaToTheta(-8);  // theta max. <---> eta min 
  gener->SetThetaRange(thmin,thmax);
  gener->SetOrigin(0, 0, 0);  //vertex position
  gener->SetSigma(0, 0, 0);   //Sigma in (X,Y,Z) (cm) on IP position


  // Particle guns for the barrel part (taken from RichConfig)

  IlcGenFixed *pG1=new IlcGenFixed(1);
  pG1->SetPart(kProton);
  pG1->SetMomentum(2.5);
  pG1->SetTheta(109.5-3);
  pG1->SetPhi(10);
  gener->AddGenerator(pG1,"g1",1);
  
  IlcGenFixed *pG2=new IlcGenFixed(1);
  pG2->SetPart(kPiPlus);
  pG2->SetMomentum(1.0);
  pG2->SetTheta( 90.0-3);
  pG2->SetPhi(10);
  gener->AddGenerator(pG2,"g2",1);

  IlcGenFixed *pG3=new IlcGenFixed(1);
  pG3->SetPart(kPiMinus);
  pG3->SetMomentum(1.5);
  pG3->SetTheta(109.5-3);
  pG3->SetPhi(30);
  gener->AddGenerator(pG3,"g3",1);
  
  IlcGenFixed *pG4=new IlcGenFixed(1);
  pG4->SetPart(kKPlus);
  pG4->SetMomentum(0.7);
  pG4->SetTheta( 90.0-3);
  pG4->SetPhi(30);
  gener->AddGenerator(pG4,"g4",1);
  
  IlcGenFixed *pG5=new IlcGenFixed(1);
  pG5->SetPart(kKMinus);
  pG5->SetMomentum(1.0);
  pG5->SetTheta( 70.0-3);
  pG5->SetPhi(30);
  gener->AddGenerator(pG5,"g5",1);
  
  IlcGenFixed *pG6=new IlcGenFixed(1);
  pG6->SetPart(kProtonBar);
  pG6->SetMomentum(2.5);
  pG6->SetTheta( 90.0-3);
  pG6->SetPhi(50);
  gener->AddGenerator(pG6,"g6",1);
  
  IlcGenFixed *pG7=new IlcGenFixed(1);
  pG7->SetPart(kPiMinus);
  pG7->SetMomentum(0.7);
  pG7->SetTheta( 70.0-3);
  pG7->SetPhi(50);
  gener->AddGenerator(pG7,"g7",1);

  // Electrons for TRD

  IlcGenFixed *pG8=new IlcGenFixed(1);
  pG8->SetPart(kElectron);
  pG8->SetMomentum(1.2);
  pG8->SetTheta( 95.0);
  pG8->SetPhi(190);
  gener->AddGenerator(pG8,"g8",1);

  IlcGenFixed *pG9=new IlcGenFixed(1);
  pG9->SetPart(kPositron);
  pG9->SetMomentum(1.2);
  pG9->SetTheta( 85.0);
  pG9->SetPhi(190);
  gener->AddGenerator(pG9,"g9",1);

  // PHOS

  IlcGenBox *gphos = new IlcGenBox(1);
  gphos->SetMomentumRange(10,11.);
  gphos->SetPhiRange(270.5,270.7);
  gphos->SetThetaRange(90.5,90.7);
  gphos->SetPart(kGamma);
  gener->AddGenerator(gphos,"GENBOX GAMMA for PHOS",1);

  // EMCAL

  IlcGenBox *gemcal = new IlcGenBox(1);
  gemcal->SetMomentumRange(10,11.);
  gemcal->SetPhiRange(90.5,199.5);
  gemcal->SetThetaRange(90.5,90.7);
  gemcal->SetPart(kGamma);
  gener->AddGenerator(gemcal,"GENBOX GAMMA for EMCAL",1);

  // MUON
  IlcGenBox * gmuon1 = new IlcGenBox(1);
  gmuon1->SetMomentumRange(20.,20.1);
  gmuon1->SetPhiRange(0., 360.);         
  gmuon1->SetThetaRange(171.000,178.001);
  gmuon1->SetPart(kMuonMinus);           // Muons
  gener->AddGenerator(gmuon1,"GENBOX MUON1",1);

  IlcGenBox * gmuon2 = new IlcGenBox(1);
  gmuon2->SetMomentumRange(20.,20.1);
  gmuon2->SetPhiRange(0., 360.);         
  gmuon2->SetThetaRange(171.000,178.001);
  gmuon2->SetPart(kMuonPlus);           // Muons
  gener->AddGenerator(gmuon2,"GENBOX MUON1",1);

  //TOF
  IlcGenFixed *gtof=new IlcGenFixed(1);
  gtof->SetPart(kProton);
  gtof->SetMomentum(2.5);
  gtof->SetTheta(95);
  gtof->SetPhi(340);
  gener->AddGenerator(gtof,"Proton for TOF",1);

  //FMD1
  IlcGenFixed *gfmd1=new IlcGenFixed(1);
  gfmd1->SetPart(kGamma);
  gfmd1->SetMomentum(25);
  gfmd1->SetTheta(1.8);
  gfmd1->SetPhi(10);
  gener->AddGenerator(gfmd1,"Gamma for FMD1",1);
  
  //FMD2i
  IlcGenFixed *gfmd2i=new IlcGenFixed(1);
  gfmd2i->SetPart(kPiPlus);
  gfmd2i->SetMomentum(1.5);
  gfmd2i->SetTheta(7.3);
  gfmd2i->SetPhi(20);
  gener->AddGenerator(gfmd2i,"Pi+ for FMD2i",1);
  
  //FMD2o
  IlcGenFixed *gfmd2o=new IlcGenFixed(1);
  gfmd2o->SetPart(kPiMinus);
  gfmd2o->SetMomentum(1.5);
  gfmd2o->SetTheta(16.1);
  gfmd2o->SetPhi(30);
  gener->AddGenerator(gfmd2o,"Pi- for FMD2o",1);
  
  //FMD3o
  IlcGenFixed *gfmd3o=new IlcGenFixed(1);
  gfmd3o->SetPart(kPiPlus);
  gfmd3o->SetMomentum(1.5);
  gfmd3o->SetTheta(163.9);
  gfmd3o->SetPhi(40);
  gener->AddGenerator(gfmd3o,"Pi+ for FMD3o",1);
  
  //FMD3i
  IlcGenFixed *gfmd3i=new IlcGenFixed(1);
  gfmd3i->SetPart(kPiMinus);
  gfmd3i->SetMomentum(1.5);
  gfmd3i->SetTheta(170.5);
  gfmd3i->SetPhi(50);
  gener->AddGenerator(gfmd3i,"Pi- for FMD3i",1);
  
  //VZERO C
  IlcGenFixed *gv0c=new IlcGenFixed(1);
  gv0c->SetPart(kPiPlus);
  gv0c->SetMomentum(1.5);
  gv0c->SetTheta(170);
  gv0c->SetPhi(50);
  gener->AddGenerator(gv0c,"Pi+ for V0C",1);
  
  //VZERO A
  IlcGenFixed *gv0a=new IlcGenFixed(1);
  gv0a->SetPart(kPiMinus);
  gv0a->SetMomentum(1.5);
  gv0a->SetTheta(1.5);
  gv0a->SetPhi(70);
  gener->AddGenerator(gv0a,"Pi- for V0A",1);


  //PMD
  IlcGenFixed *gpmd=new IlcGenFixed(1);
  gpmd->SetPart(kGamma);
  gpmd->SetMomentum(2);
  gpmd->SetTheta(12.6);
  gpmd->SetPhi(60);
  gener->AddGenerator(gpmd,"Gamma for PMD",1);

  //ZDC
  IlcGenFixed *gzdc1=new IlcGenFixed(1);
  gzdc1->SetPart(kProton);
  gzdc1->SetMomentum(700);
  gzdc1->SetTheta(0.6);
  gzdc1->SetPhi(60);
  gener->AddGenerator(gzdc1,"Proton for ZDC",1);

  IlcGenFixed *gzdc2=new IlcGenFixed(1);
  gzdc2->SetPart(kNeutron);
  gzdc2->SetMomentum(500);
  gzdc2->SetTheta(0.6);
  gzdc2->SetPhi(60);
  gener->AddGenerator(gzdc2,"Neutron for ZDC",1);

  //T0
  IlcGenFixed *gt0=new IlcGenFixed(1);
  gt0->SetPart(kPiPlus);
  gt0->SetMomentum(2);
  gt0->SetTheta(5.1);
  gt0->SetPhi(60);
  gener->AddGenerator(gt0,"Pi+ for T0",1);

  IlcGenFixed *gt01=new IlcGenFixed(1);
  gt01->SetPart(kPiMinus);
  gt01->SetMomentum(2);
  gt01->SetTheta(5.1);
  gt01->SetPhi(60);
  gener->AddGenerator(gt01,"Pi- for T0",1);


  //ACORDE
  IlcGenFixed *gacorde=new IlcGenFixed(1);
  gacorde->SetPart(kMuonPlus);
  gacorde->SetMomentum(20);
  gacorde->SetTheta(90.);
  gacorde->SetPhi(90);
  gener->AddGenerator(gacorde,"Muon+ for ACORDE",1);

  IlcGenFixed *gacorde1=new IlcGenFixed(1);
  gacorde1->SetPart(kMuonMinus);
  gacorde1->SetMomentum(20);
  gacorde1->SetTheta(90.);
  gacorde1->SetPhi(90);
  gener->AddGenerator(gacorde1,"Muon- for ACORDE",1);

  gener->Init();
  
  return gener;
  
  cout << "Running genGunConfig.C finished ... " << endl;
}

Float_t EtaToTheta(Float_t arg){
  return (180./TMath::Pi())*2.*atan(exp(-arg));
}
