void MakeLHCClockPhaseEntry(const char *cdbStorage = "local://$ILC_ROOT/OCDB")
{
  // Example macro to put in OCDB the default (=0) LHC-clock phase
  // It is valid fro runs from 0 to inf
  // The timestamp range is also inf (we store the first and last value for
  // each beam)
  IlcCDBManager *man = IlcCDBManager::Instance();
  man->SetDefaultStorage(cdbStorage);

  IlcLHCClockPhase phaseObj;

  phaseObj.AddPhaseB1DP(0,0.);
  phaseObj.AddPhaseB2DP(0,0.);

  phaseObj.AddPhaseB1DP(2147483647,0.);
  phaseObj.AddPhaseB2DP(2147483647,0.);

  IlcCDBMetaData* metadata = new IlcCDBMetaData();
  metadata->SetResponsible("Cvetan Cheshkov");
  metadata->SetComment("Default LHC-clock phase object");
  IlcCDBId id("GRP/Calib/LHCClockPhase",0,IlcCDBRunRange::Infinity());

  man->Put(&phaseObj,id,metadata);

  return;
}
