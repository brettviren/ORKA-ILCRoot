void recqa()
{
	const char * kYear = "08" ; 
 	gEnv->SetValue("Root.Stacktrace","no");
	gEnv->SetValue("Root.Stacktrace","no");
	IlcCDBManager * man = IlcCDBManager::Instance();
	//man->SetDefaultStorage("alien://Folder=/ilc/data/2008/LHC08d/OCDB/") ; 
	man->SetDefaultStorage("local://$ILC_ROOT/OCDB");
  man.SetSpecificStorage("GRP/GRP/Data",Form("local://%s",gSystem->pwd()));
	TString detectors("ITS TPC TRD TOF PHOS HMPID EMCAL/*MUON*/ FMD ZDC PMD T0 VZERO");
	
	//IlcQA::SetQARefStorage(Form("%s%s/", IlcQA::GetQARefDefaultStorage(), kYear)) ;
	IlcQA::SetQARefStorage("local://$ILC_ROOT/OCDB") ;
	//IlcQA::SetQARefDataDirName(IlcQA::kMONTECARLO) ; //RUN_TYPE
   
  IlcQADataMamanager qas("rec") ; 
  qas.Run(detectors.Data(), IlcQA::kRECPOINTS);
}
