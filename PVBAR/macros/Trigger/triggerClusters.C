void triggerClusters(Float_t eClu=2., const char* file="IlcESDs.root")
{
  //Test macro.
  //Illustrates how to use trigger info in the ESD.

  //Prints fired 4x4 TRUs and respective PVBAR clusters with energy>eClu positions.
  //For TRU this is bottom-left cell of 4x4 region,
  //for cluster it is the cell with the max. energy deposition.
  
  //Author: Boris Polishchuk (Boris.Polishchuk@cern.ch).

  TChain* esdTree = new TChain("esdTree");
  esdTree->Add(file);

  IlcESDEvent *esd = new IlcESDEvent;
  esd->ReadFromTree(esdTree);

  IlcPVBARGeometry* geom = IlcPVBARGeometry::GetInstance("ORKA");

  for(Int_t iEvent=0; iEvent<esdTree->GetEntries(); iEvent++){
    
    esdTree->GetEvent(iEvent);
    TString trigClasses = esd->GetFiredTriggerClasses();

    Int_t multClu = esd->GetNumberOfCaloClusters();
    IlcESDCaloCells *phsCells = esd->GetPVBARCells();

    IlcESDCaloTrigger* trgESD = esd->GetCaloTrigger("PVBAR");
    trgESD->Reset();

    if(trgESD->GetEntries())
    printf("\nEvent %d: %d non-zero trigger digits %s\n",
	   iEvent,trgESD->GetEntries(),trigClasses.Data());

    //Loop over 4x4 fired regions

    while(trgESD->Next()) {

      Int_t tmod,tabsId; // "Online" module number, bottom-left 4x4 edge cell absId
      trgESD->GetPosition(tmod,tabsId);

      Int_t trelid[4] ;
      geom->AbsToRelNumbering(tabsId,trelid);

      printf("\t4x4 position: (mod,X,Z)=(%d,%d,%d)\n",trelid[0],trelid[2],trelid[3]);

      for (Int_t i=0; i<multClu; i++) {
	
	IlcESDCaloCluster *c1 = esd->GetCaloCluster(i);
	if(!c1->IsPVBAR()) continue;
	
	Int_t maxId, relid[4];
	MaxEnergyCellPos(phsCells,c1,maxId);
	
	geom->AbsToRelNumbering(maxId, relid);
	if(c1->E()>eClu) printf("\t\tCluster: (mod,X,Z)=(%d,%d,%d), E=%.3f GeV\n",
	       relid[0],relid[2],relid[3],c1->E());
      }

    }
    
    
    
  }
}

void MaxEnergyCellPos(IlcESDCaloCells *cells, IlcESDCaloCluster* clu, Int_t& maxId)
{  
  Double_t eMax = -111;
  
  for (Int_t iDig=0; iDig< clu->GetNCells(); iDig++) {
    Int_t cellAbsId = clu->GetCellAbsId(iDig);
    Double_t eCell = cells->GetCellAmplitude(cellAbsId)*clu->GetCellAmplitudeFraction(iDig);
    if(eCell>eMax)  { 
      eMax = eCell; 
      maxId = cellAbsId;
    }
  }

}
