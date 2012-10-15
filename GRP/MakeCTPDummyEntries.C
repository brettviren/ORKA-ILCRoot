void MakeCTPDummyEntries(){

	// Example macro to put in OCDB the dummy entries for CTP configuration and scalers
	// The entries are at present taken from $ILC_ROOT 
	// Should be used to test the GRP preprocessor 

	IlcCDBManager *man = IlcCDBManager::Instance();
	man->SetDefaultStorage("local://$ILC_ROOT/OCDB");
	Char_t * filenameConfig = gSystem->ExpandPathName("$ILC_ROOT/GRP/CTP/stdln.cfg");
	Char_t * filenameScalers = gSystem->ExpandPathName("$ILC_ROOT/GRP/CTP/stdln.cnt");
        Char_t * filenameCTPtime = gSystem->ExpandPathName("$ILC_ROOT/GRP/CTP/stdln.tim");

	IlcTriggerConfiguration *runcfg = IlcTriggerConfiguration::LoadConfiguration(filenameConfig);
	IlcTriggerRunScalers *scalers = IlcTriggerRunScalers::ReadScalers(filenameScalers);
	IlcCTPTimeParams *ctptime = IlcCTPTimeParams::LoadCTPTimeParams(filenameCTPtime);

	// CTP configuration
	IlcCDBMetaData* metaconfig = new IlcCDBMetaData();
	metaconfig->SetResponsible("Roman Lietava");
	metaconfig->SetComment("Dummy CTP configuration for standalone runs");
	IlcCDBId idconfig("GRP/CTP/DummyConfig",0,IlcCDBRunRange::Infinity());
	man->Put(runcfg,idconfig, metaconfig);

	// CTP scalers
	IlcCDBMetaData* metascalers = new IlcCDBMetaData();
	metascalers->SetResponsible("Roman Lietava");
	metascalers->SetComment("Dummy CTP scalers for standalone runs");
	IlcCDBId idscalers("GRP/CTP/DummyScalers",0,IlcCDBRunRange::Infinity());
	man->Put(scalers,idscalers, metascalers);

	// CTP time parameters
	IlcCDBMetaData* metactptime = new IlcCDBMetaData();
	metactptime->SetResponsible("Roman Lietava");
	metactptime->SetComment("Dummy CTP time params for standalone runs");
	IlcCDBId idctptime("GRP/CTP/DummyCTPtime",0,IlcCDBRunRange::Infinity());
	man->Put(ctptime,idctptime, metactptime);

	// CTP LTU configuration

	TObjArray* ltuarray = new TObjArray();
	ltuarray->SetOwner(1);
	IlcLTUConfig* ltu;
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("ITSSPD"),14000.,16459.,13);
	ltuarray->AddAtAndExpand(ltu,0);
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("ITSSDD"),3126.,16459.,8);
	ltuarray->AddAtAndExpand(ltu,1);
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("ITSSSD"),3126.,16459.,17);
	ltuarray->AddAtAndExpand(ltu,2);
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("TPC"),3126.,16459.,15);
	ltuarray->AddAtAndExpand(ltu,3);
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("TRD"),3126.,16459.,17);
	ltuarray->AddAtAndExpand(ltu,4);
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("TOF"),3126.,16459.,14);
	ltuarray->AddAtAndExpand(ltu,5);
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("HMPID"),3126.,16459.,19);
	ltuarray->AddAtAndExpand(ltu,6);
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("PHOS"),3126.,16459.,19);
	ltuarray->AddAtAndExpand(ltu,7);
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("CPV"),3126.,16459.,16);
	ltuarray->AddAtAndExpand(ltu,8);
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("PMD"),3126.,16459.,22);
	ltuarray->AddAtAndExpand(ltu,9);
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("MUONTRK"),3126.,16459.,8);
	ltuarray->AddAtAndExpand(ltu,10);
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("MUONTRG"),3126.,16459.,11);
	ltuarray->AddAtAndExpand(ltu,11);
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("FMD"),3126.,16459.,17);
	ltuarray->AddAtAndExpand(ltu,12);
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("T0"),3126.,16459.,15);
	ltuarray->AddAtAndExpand(ltu,13);
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("VZERO"),2000.,16459.,12);
	ltuarray->AddAtAndExpand(ltu,14);
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("ZDC"),3126.,16459.,17);
	ltuarray->AddAtAndExpand(ltu,15);
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("ACORDE"),16126.,22459.,18);
	ltuarray->AddAtAndExpand(ltu,16);
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("EMCAL"),3126.,16459.,19);
	ltuarray->AddAtAndExpand(ltu,17);
	ltu = new IlcLTUConfig((Char_t)IlcDAQ::DetectorID("DAQ_TEST"),3126.,16459.,10);
	ltuarray->AddAtAndExpand(ltu,18);

	IlcCDBMetaData* md = new IlcCDBMetaData();
	md->SetResponsible("Roman Lietava");
	md->SetComment("Example of (dummy -> default settings from 23/11/2010) entry for the detectors' LTU config");
	IlcCDBId id("GRP/CTP/DummyLTUConfig",0,IlcCDBRunRange::Infinity());
	man->Put(ltuarray,id, md);
	// check if ok
	/*
	man->SetRun(0);
	TObjArray* ltuarrayR = (TObjArray*) man->Get("GRP/CTP/LTUConfig")->GetObject();
	cout << "Array size: " << ltuarrayR->GetEntriesFast() << endl;
        for(Int_t i=0;i<ltuarrayR->GetEntriesFast();i++){
	  if(ltu=(IlcLTUConfig*) ltuarrayR->At(i)) ltu->Print();
	  else cout << "--------------------->Empty position " << i << endl;
	}
	*/

	return;
}
