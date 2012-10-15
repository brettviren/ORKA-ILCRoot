void MakeCTPLTUConfigEntry(const char *cdbStorage = "local://$ILC_ROOT/OCDB"){

	// Example macro to put in OCDB an LTU Config entry
	// Since it is just an example macro, the IlcLTUConfig members will be set as 
	// follows for all the detectors:
	// fFineDelay1 = 0;
	// fFineDelay2 = 0;
	// fBCDelaysAdd = 0;

	IlcCDBManager *man = IlcCDBManager::Instance();
	man->SetDefaultStorage(cdbStorage);

	TObjArray* ltuarray = new TObjArray();
	ltuarray->SetOwner(1);
	IlcLTUConfig* ltu;
	for(Int_t i = 0; i<IlcDAQ::kNDetectors-2; i++){
		const char* name = IlcDAQ::DetectorName(i);
		ltu = new IlcLTUConfig((UChar_t)IlcDAQ::DetectorID(name),0.,0.,0.);
		ltuarray->AddAtAndExpand(ltu,i);
	}

	IlcCDBMetaData* md = new IlcCDBMetaData();
	md->SetResponsible("Roman Lietava");
	md->SetComment("Example of (dummy -> everything set to 0) entry for the detectors' LTU config");
	IlcCDBId id("GRP/CTP/LTUConfig",0,IlcCDBRunRange::Infinity());
	man->Put(ltuarray,id, md);

	return;
}
