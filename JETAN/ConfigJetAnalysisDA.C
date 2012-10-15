IlcJetFinder*  ConfigJetAnalysis()
{
    //
    // Configuration goes here
    // 
    printf("ConfigJetAnalysis() \n");
    IlcJetAODReaderHeader *jrh = new IlcJetAODReaderHeader();
    jrh->SetComment("AOD Reader");
    jrh->SetPtCut(0.);
    jrh->SetTestFilterMask(1<<0);
    // Define reader and set its header
    IlcJetAODReader *er = new IlcJetAODReader();
    er->SetReaderHeader(jrh);
   
 
    // Define jet header
    IlcDAJetHeader *jh=new IlcDAJetHeader();
    jh->SetComment("DA jet code with default parameters");
    jh->SelectJets(kTRUE);
    jh->SetNclust(10);

    // Define jet finder. Set its header and reader
    jetFinder = new IlcDAJetFinder();
    jetFinder->SetJetHeader(jh);
    jetFinder->SetJetReader(er);
    //
    return jetFinder;
}
