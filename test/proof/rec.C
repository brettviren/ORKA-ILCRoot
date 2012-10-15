void rec(Int_t runNumber)
{
  gSystem->Load("libRAliEn.so");
  gSystem->Load("libNet.so");

  // Set the CDB storage location
  IlcCDBManager * man = IlcCDBManager::Instance();
  man->SetDefaultStorage("raw://");
  
  // Reconstruction settings
  IlcReconstruction rec;

  // QA options
  rec.SetRunQA(":") ;
  rec.SetRunGlobalQA(kFALSE);
  rec.SetQARefDefaultStorage("local://$ILC_ROOT/QAref") ;
  rec.SetRunPlaneEff(kFALSE);

  // IlcReconstruction settings
  rec.SetWriteESDfriend(kTRUE);
  rec.SetWriteAlignmentData();
  rec.SetInput(Form("raw://run%d",runNumber));
  rec.SetRunReconstruction("ALL -HLT");
  rec.SetUseTrackingErrorsForAlignment("ITS");

  rec.SetEventRange(0,10000);

  // switch off cleanESD
  rec.SetCleanESD(kFALSE);

  rec.SetOutput(Form("root_archive.zip#IlcESDs.root:IlcESDs.root,IlcESDfriends.root@dataset://run%d",runNumber));

  rec.Run();
}
