////////////////////////////////////////////////////////////////////////
//
// name: IlcPVBARHits2SDigits
// date: 4.4.2002
// last update: 4.4.2002
// author: Jiri Chudoba
// version: 1.0
//
// description: 
//       creates sdigits for digits for PVBAR.
//       stores sdigits in separate file (or in the source file
//       with hits). Stores gIlc object and copies TE to the
//       file with sdigits.
//
// input:
//        TString fileNameHits ... input file with hits
//        TString fileNameSDigits ... output file with sdigits
//
// History:
//
// 04.04.02 - first version
// 
////////////////////////////////////////////////////////////////////////


Int_t IlcPVBARHits2SDigits(TString
fileNameSDigits="PVBAR.sdigits.root",TString fileNameHits="rfio:gilc.root")

{
  TFile *fileSDigits;
  fileSDigits = Init(fileNameSDigits, fileNameHits);
  if (!fileSDigits) return 1;

  IlcPVBARSDigitizer *sdPVBAR = new IlcPVBARSDigitizer(fileNameHits.Data(),"PVBAR");

  TStopwatch timer;
  timer.Start();

  gIlc->MakeTree("S",fileSDigits);
  sdPVBAR->ExecuteTask("deb all");

  timer.Stop(); 
  timer.Print();

  fileSDigits->Close();
  delete fileSDigits;

}
 

////////////////////////////////////////////////////////////////////////
TFile* Init(TString fileNameSDigits, TString fileNameHits) {
// open input file, read in gIlc, prepare output file
  if (gIlc) delete gIlc;
  gIlc = 0;

  Bool_t sameFiles = kFALSE;
  if (fileNameSDigits == fileNameHits || fileNameSDigits == "") sameFiles = kTRUE;

  TString fileMode = "read";
  if (sameFiles) fileMode = "update";

  TFile *fileHits =  OpenFile(fileNameHits.Data(),fileMode.Data());
  if (!fileHits) return 0;
  if (!ImportgIlc(fileHits)) return 0;
  if (!sameFiles) return gIlc->InitTreeFile("S",fileNameSDigits.Data());
  return fileHits;

}

////////////////////////////////////////////////////////////////////////
TFile* OpenFile(TString fileName, TString fileMode) {
// open file fileName
  TFile *file = TFile::Open(fileName.Data(),fileMode.Data());
  if (!file->IsOpen()) {
    cerr<<"Can't open "<<fileName.Data()<<" !\n";
    return 0;
  }
  return file;
}

////////////////////////////////////////////////////////////////////////
Bool_t ImportgIlc(TFile *file) {
// read in gIlc object from the file
  gIlc = (IlcRun*)file->Get("gIlc");
  if (!gIlc)  return kFALSE;
  return kTRUE;
}
////////////////////////////////////////////////////////////////////////
