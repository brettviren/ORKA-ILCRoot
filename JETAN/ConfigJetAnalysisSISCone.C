IlcJetFinder*  ConfigJetAnalysis()
{
  //
  // Configuration goes here
  //
  printf("ConfigJetAnalysis() \n");
  // Define the grids
  IlcJetGrid *grid = new IlcJetGrid(419,119,0.,2*TMath::Pi(),-0.9,0.9);
  grid->SetGridType(1);
  grid->InitParams(80.*TMath::Pi()/180,190.*TMath::Pi()/180,-0.7,0.7);
  grid->SetMatrixIndexes();
  grid->SetIndexIJ();
  IlcJetGrid *grid2 = new IlcJetGrid(131,95,80.*TMath::Pi()/180.,190.*TMath::Pi()/180.,-0.7,0.7);
  grid2->SetGridType(0);
  grid2->SetMatrixIndexes();
  grid2->SetIndexIJ();
  
  // Define ESD reader header
  IlcJetESDReaderHeader *jrh = new IlcJetESDReaderHeader();
  jrh->SetComment("Testing");
  jrh->SetReadSignalOnly(kFALSE);
  // Detector options: 0 = Charged particles only (MomentumArray)
  //                   1 = Charged particles only (UnitArray)
  //                   2 = Neutral cells only (UnitArray)
  //                   3 = Charged particles + neutral cells (UnitArray)
  jrh->SetDetector(0);
  jrh->SetDebug(0);
  jrh->SetFiducialEta(-0.9,0.9);
  jrh->SetFiducialPhi(0,2*TMath::Pi());

  // Define reader and set its header
  IlcJetESDReader *er = new IlcJetESDReader();
  er->SetReaderHeader(jrh);
  er->SetTPCGrid(grid);
  er->SetEMCalGrid(grid2);

  // Define jet finder header
  IlcSISConeJetHeader * jh = new IlcSISConeJetHeader();
  //siscone parameters
  jh->SetConeRadius(1.0);                   // cone radius
  jh->SetOverlapThreshold(0.75);            // overlap parameter, between 0 and 1 excluded!! 0.75 value is advised
  jh->SetMinJetPt(20);                      // Ptmin of jets
  //do you want to subtract BG (0 = no, 1 = yes)
  jh->SetBGMode(0);
  //to determine jets area (for BG subtraction)
  jh->SetAreaTypeNumber(4);                 // from 1 to 4 : 1 = active_area, 2 = active_area_explicit_ghosts, 3 = one_ghost_passive_area, 4 = passive_area

  // Define jet finder
  IlcSISConeJetFinder *jetFinder = new IlcSISConeJetFinder();
  jetFinder->SetJetHeader(jh);
  jetFinder->SetJetReader(er);

  return jetFinder;
}





