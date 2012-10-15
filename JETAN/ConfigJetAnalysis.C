IlcJetFinder*  ConfigJetAnalysis()
{
    //
    // Configuration goes here
    // 
    printf("ConfigJetAnalysis() \n");
    IlcJetESDReaderHeader *jrh = new IlcJetESDReaderHeader();
    jrh->SetComment("Testing");
    jrh->SetFirstEvent(0);
    jrh->SetLastEvent(1000);
    jrh->SetPtCut(0.);
    jrh->SetReadSignalOnly(kFALSE);
    // Define reader and set its header
    IlcJetESDReader *er = new IlcJetESDReader();
    er->SetReaderHeader(jrh);
   
 
    // Define jet header
    IlcUA1JetHeaderV1 *jh=new IlcUA1JetHeaderV1();
    jh->SetComment("UA1 jet code with default parameters");
    jh->BackgMode(0);
    jh->SetRadius(1.0);
    jh->SetEtSeed(2.);
    jh->SetLegoNbinPhi(420.);
    jh->SetLegoNbinEta(120.);
    jh->SetLegoEtaMin(-1.9);
    jh->SetLegoEtaMax(+1.9);  
    jh->SetMinJetEt(5.);
    
    // Define jet finder. Set its header and reader
    jetFinder = new IlcUA1JetFinderV1();
    jetFinder->SetJetHeader(jh);
    jetFinder->SetJetReader(er);
    /*
    jetFinder->SetPlotMode(kTRUE);
    jetFinder->SetOutputFile("jets.root");
    */
    //
    return jetFinder;
}
