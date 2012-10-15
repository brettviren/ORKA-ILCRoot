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
    jrh->SetPtCut(.0);
    jrh->SetReadSignalOnly(kFALSE);
    jrh->SetReadBkgdOnly(kFALSE);
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
    
    // Define jet header
    IlcUA1JetHeaderV1 *jh=new IlcUA1JetHeaderV1();
    jh->SetComment("UA1 jet code with default parameters");
    jh->BackgMode(0);
    jh->SetRadius(1.0);
    jh->SetEtSeed(2.);
    jh->SetLegoNbinPhi(420.);
    jh->SetLegoNbinEta(120.);
    jh->SetLegoEtaMin(-0.9);
    jh->SetLegoEtaMax(+0.9);  
    jh->SetMinJetEt(10.);
    
    // Define jet finder. Set its header and reader
    jetFinder = new IlcUA1JetFinderV2();
    jetFinder->SetJetHeader(jh);
    jetFinder->SetJetReader(er);
    jetFinder->SetPlotMode(kTRUE);
    
    return jetFinder;
}
