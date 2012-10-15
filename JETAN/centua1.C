void centua1(char* bin)
{
  gSystem->Load("$(ILC_ROOT)/lib/tgt_$(ILC_TARGET)/libJETAN");
  
  char dir[100];
  //sprintf(dir, "/data1/jgcn/partonic_events/unquenched/%s", bin);
  sprintf(dir, "/home/guest/ilc/data/cent1_nq/%s", bin);

  IlcJetESDReaderHeader *krh = new IlcJetESDReaderHeader(); // hijing
  //IlcJetKineReaderHeader *krh = new IlcJetKineReaderHeader(); // pythia
  krh->SetComment("");
  krh->SetDirectory(dir);
  krh->SetPattern("miniesd"); // hijing
  krh->SetFirstEvent(0);
  krh->SetLastEvent(100000);
  krh->SetPtCut(1.0);
  //krh->SetFastSimTPC(kFALSE); // pythia
  //krh->SetFastSimEMCAL(kFALSE); // pythia
 
  // define reader and set its header
  IlcJetESDReader *kr = new IlcJetESDReader(); // hijing
  //IlcJetKineReader *kr = new IlcJetKineReader(); // pythia
  kr->SetReaderHeader(krh);
 
  // define jet header
  IlcUA1JetHeader *jh=new IlcUA1JetHeader();
  jh->SetComment("UA1 jet code with radius 1");
  jh->SetMode(1);
  jh->SetRadius(0.4);
  jh->SetMinCellEt(0.);
  jh->SetEtSeed(4.);
  jh->SetLegoNbinPhi(420.);
  jh->SetLegoNbinEta(120.);
  jh->SetLegoEtaMin(-0.9);
  jh->SetLegoEtaMax(+0.9);
    
  // define jet finder. Set its header and reader
  IlcUA1JetFinder *jf = new IlcUA1JetFinder();
  jf->SetJetHeader(jh);
  jf->SetJetReader(kr);
  jf->SetPlotMode(kTRUE);
  jf->SetOutputFile("jets.root");
  // do the job
  jf->Run();
}
 
