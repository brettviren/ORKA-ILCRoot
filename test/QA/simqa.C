void simqa()
{
	const char * kYear = "08" ; 
	gEnv->SetValue("Root.Stacktrace","no");
	gEnv->SetValue("Root.Stacktrace","no");
	IlcCDBManager * man = IlcCDBManager::Instance();
	//man->SetDefaultStorage("alien://Folder=/ilc/data/2008/LHC08d/OCDB/");
	man->SetDefaultStorage("local://$ILC_ROOT/OCDB");
	TString detectors("ITS TPC TRD TOF PHOS HMPID EMCAL MUON FMD PMD ZDC T0 VZERO"); 
	
	//IlcQA::SetQARefStorage(Form("%s%s/", IlcQA::GetQARefDefaultStorage(), kYear)) ;
	IlcQA::SetQARefStorage("local://$ILC_ROOT/QAref") ;
  

	IlcQAManager *  qas = IlcQAManager::QAManager("sim") ; 
	qas->SetDefaultStorage("local://$ILC_ROOT/QAref");
        qas->SetEventSpecie(IlcRecoParam::kLowMult); 
	qas->Run(detectors.Data(), IlcQA::kHITS);
	qas->Run(detectors.Data(), IlcQA::kSDIGITS);
	qas->Run(detectors.Data(), IlcQA::kDIGITS);
}
