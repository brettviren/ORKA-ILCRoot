// Macro to make a tag file for a full run directly from ESDs and the GRP
// Takes as parameters a text file with the location of all ESD files
// for a given run (can be modified to work on any chain of ESDs)
// Second argument is the run number, for which the GRP will be retrieved
// Suitable for a GRID job - runs with pure ROOT to minimize memory 
// consumption but needs ILCROOT package location to load only necessary
// IlcRoot libraries.
//
// Author: Adam.Kisiel@cern.ch

class IlcGRPObject;

IlcGRPObject *GetGRPForRun(Int_t runno) {
  IlcCDBManager* man = IlcCDBManager::Instance();
  man->SetDefaultStorage("raw://");
  man->SetRun(runno);
  IlcCDBEntry* e = man->Get("GRP/GRP/Data");
  IlcGRPObject* o = (IlcGRPObject*)e->GetObject();
  
  return o;
}

void ReMakeTagsFullRun(const char *esdfile, Int_t runno)
{
  gSystem->Load("libProofPlayer.so");
  gSystem->Load("libVMC.so");  
  gSystem->Load("libSTEERBase.so");
  gSystem->Load("libESD.so");
  gSystem->Load("libAOD.so");
  gSystem->Load("libANALYSIS.so");  
  gSystem->Load("libANALYSISilc.so");
  gSystem->Load("libGui.so");
  gSystem->Load("libCDB.so");

  gSystem->Load("libMinuit.so");
  gSystem->Load("libRAWDatabase.so");

  gSystem->Load("libSTEER.so");  
  gSystem->Load("libXMLParser.so");

  TGrid::Connect("alien://");


  IlcGRPObject *grpo = GetGRPForRun(runno);
  
  IlcESDTagCreator *tagCre = new IlcESDTagCreator();
  tagCre->SetStorage(0);


  TChain *chain = new TChain("esdTree");;
  ifstream *istr = new ifstream(esdfile);

  char fname[2000];
  char pname[2000];
  while (!istr->eof()) {
    fname[0] = '\0';
    (*istr) >> fname;
    if (strlen(fname) > 10) {
      sprintf(pname, "alien://%s", fname);
      chain->Add(pname);
    }
  }

  tagCre->CreateESDTagsFullRun(chain, grpo, 0, 0, 0, 0);  
}
