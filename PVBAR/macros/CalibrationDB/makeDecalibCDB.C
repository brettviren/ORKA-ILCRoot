void makeDecalibCDB(Int_t firstRun, Int_t lastRun, Float_t decalib = 0.065)
{
  //Generates a random decalibration factors O(1)
  //to be applied in the anchor run simulations with raw:// .
  //If decalib<0, no decalibration generated, all factors=1.
  
  //Run range is [firstRun,lastRun] and gaussian sigma = decalib.
  //Author: Boris Polishchuk.
  
  IlcCDBManager::Instance()->SetDefaultStorage("raw://");
  IlcCDBManager::Instance()->SetRun(firstRun);

  TString emcPath("PVBAR/Calib/EmcGainPedestals");
  IlcCDBEntry* entryEmc = IlcCDBManager::Instance()->Get(emcPath.Data(),-1);

  IlcPVBAREmcCalibData* clb=0;

  if(entryEmc) clb = (IlcPVBAREmcCalibData*)entryEmc->GetObject();
  else { printf("CDB entry not found. Exit.\n"); return; }
  
  if(!clb) { printf("Calibration parameters for PVBAR EMC not found.\n"); return; }
  
  printf("\t\tEMC calibration object found: FirstRun=%d LastRun=%d Version=%d SubVersion=%d\n",
         entryEmc->GetId().GetFirstRun(), entryEmc->GetId().GetLastRun(),
         entryEmc->GetId().GetVersion(),entryEmc->GetId().GetSubVersion());
  
  
  TRandom rn;
  rn.SetSeed(0); //the seed is set to the current  machine clock

  Float_t adcChannelEmc;

  for(Int_t module=1; module<6; module++) {
    for(Int_t column=1; column<57; column++) {
      for(Int_t row=1; row<65; row++) {

	if(decalib<0.) adcChannelEmc = 1.;
	else
	  adcChannelEmc =rn.Gaus(1.,decalib);

        clb->SetADCchannelEmcDecalib(module,column,row,adcChannelEmc);
      }
    }
  }

  IlcCDBManager::Instance()->SetDefaultStorage("local://./");
  IlcCDBStorage* storage = IlcCDBManager::Instance()->GetDefaultStorage();
  
  IlcCDBMetaData *md = new IlcCDBMetaData();
  IlcCDBId id(emcPath.Data(),firstRun,lastRun);
  storage->Put(clb,id, md);

}
