#if !defined( __CINT__) || defined(__MAKECINT__)

#include <iostream>

#include "IlcTRDcalibDB.h"
#include "IlcCDBManager.h"
#include <TStopwatch.h>
#include <TRandom.h>

extern TRandom* gRandom;

#endif

#define BENCHMARK(code, comment) \
  timer.Reset(); timer.Start(); \
  for (Int_t i=0; i<NUMBER; ++i) { \
  code \
  } \
  timer.Stop(); \
  cerr   << "Tested " << NUMBER << " times: " << comment << ". Time/call: " << timer.CpuTime() / NUMBER << endl; \
  timer.Print();

#define NUMBER 100000

void IlcTRDbenchmarkCalibDB()
{
  TStopwatch timer;
  IlcCDBManager::Instance()->SetDefaultStorage("local://$ILC_ROOT");
  IlcTRDcalibDB* calib = IlcTRDcalibDB::Instance();
  if (!calib)
  {
    cerr << "calibDB singleton has already been terminated." << endl;
    return;
  }

  Float_t xyz[3];
  
  calib->SetRun(1);
  calib->SetRun(0);
  calib->GetNumberOfTimeBins();
  
  BENCHMARK(calib->GetNumberOfTimeBins();, "GetNumberOfTimeBins");
  BENCHMARK(calib->GetChamberPos(1, xyz);, "GetChamberPos");
  BENCHMARK(calib->GetVdrift((Int_t) (gRandom->Uniform() * 500), (Int_t) (gRandom->Uniform() * 100), (Int_t) (gRandom->Uniform() * 10));, "GetVdrift");

  #undef NUMBER
  #define NUMBER 10000
  BENCHMARK(calib->SetRun(1); calib->SetRun(0); calib->GetNumberOfTimeBins();, "GetNumberOfTimeBins with invilcdating");
  BENCHMARK(calib->SetRun(1); calib->SetRun(0); calib->GetChamberPos(1, xyz);, "GetChamberPos with invilcdating");
  #undef NUMBER
  #define NUMBER 200
  BENCHMARK(calib->SetRun(1); calib->SetRun(0); calib->GetVdrift(1, 1, 1);, "GetVdrift with invilcdating");
  
  IlcTRDcalibDB::Terminate();
}
