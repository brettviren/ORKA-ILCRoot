/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Class to generate DCS data base entries                                  //
//  Author: Haavard Helstrup                                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////





// TTimeStamp startTime(2006,10,18,0,0,0,0,kFALSE)
// TTimeStamp endTime(2006,10,19,0,0,0,0,kFALSE)
// Int_t run=2546
// IlcDCSGenDB db
// db->SetDefaultStorage("local:///afs/cern.ch/ilc/tpctest/IlcRoot/HEAD");
// db->SetSpecificStorage("local:///afs/cern.ch/ilc/tpctest/Calib/");
// db->Init(run,"TPC/Config/Pressure","TPC/*/*")
// db->MakeCalib("PressureSensor.txt","DCSMap.root",startTime,endTime,firstRun,lastRun,"TPC/Calib/Pressure")


#include "IlcDCSGenDB.h"
#include "IlcLog.h"
#include "ARVersion.h"

const Int_t kBeamPeriod=2;

ClassImp(IlcDCSGenDB)

//______________________________________________________________________________________________

IlcDCSGenDB::IlcDCSGenDB():
   fFirstRun(0),
   fLastRun(0),
   fSpecificStorage(0),
   fDefaultStorage(0),
   fSensor(0),
   fStorLoc(0),
   fMetaData(0),
   fConfTree(0)
//
//  standard constructor
//
{}

//______________________________________________________________________________________________

IlcDCSGenDB::IlcDCSGenDB(const char* defaultStorage, const char* specificStorage):
   fFirstRun(0),
   fLastRun(0),
   fSpecificStorage(specificStorage),
   fDefaultStorage(defaultStorage),
   fSensor(0),
   fStorLoc(0),
   fMetaData(0),
   fConfTree(0)
//
//  special constructor
//
{}

//______________________________________________________________________________________________

IlcDCSGenDB::IlcDCSGenDB(const IlcDCSGenDB& org):
  TObject(org),
  fFirstRun(org.fFirstRun),
  fLastRun(org.fLastRun),
  fSpecificStorage(org.fSpecificStorage),
  fDefaultStorage(org.fDefaultStorage),
  fSensor(0),
  fStorLoc(0),
  fMetaData(0),
  fConfTree(0)
{
//
//  Copy constructor
//

 IlcError("copy constructor not implemented");

}

//______________________________________________________________________________________________
IlcDCSGenDB::~IlcDCSGenDB(){
//
// destructor
//
   delete fSensor;
   delete fMetaData;
   delete fConfTree;
}

//______________________________________________________________________________________________
IlcDCSGenDB& IlcDCSGenDB::operator= (const IlcDCSGenDB& /*org*/ )
{
 //
 // assignment operator
 //
 IlcError("assignment operator not implemented");
 return *this;

}

//______________________________________________________________________________________________

void IlcDCSGenDB::MakeCalib(const char *list, const char *mapDCS,
                             const TTimeStamp& startTime,
			     const TTimeStamp& endTime,
			     Int_t firstRun, Int_t lastRun, const char *calibDir )
{

   // Generate calibration entry from DCS map
   // Configuration read from ASCII file specified by list
   
   TClonesArray *arr = ReadList(list);
   fSensor = new IlcDCSSensorArray(arr);
   fSensor->SetStartTime(startTime);
   fSensor->SetEndTime(endTime);
   TMap* map = SetGraphFile(mapDCS);
   if (map) {
     fSensor->MakeSplineFit(map);
   }
   delete map;
   map=0;
   mapDCS=0;

   SetFirstRun(firstRun);
   SetLastRun(lastRun);

   StoreObject(calibDir, fSensor, fMetaData);
}

//______________________________________________________________________________________________
void IlcDCSGenDB::MakeConfig(const char *file, Int_t firstRun, Int_t lastRun, const char *confDir )
{
   //
   // Store Configuration file to OCDB
   //

   TTree *tree = ReadListTree(file);
   SetConfTree(tree);
   SetFirstRun(firstRun);
   SetLastRun(lastRun);

   StoreObject(confDir, fConfTree, fMetaData);
}




//______________________________________________________________________________________________
IlcCDBMetaData* IlcDCSGenDB::CreateMetaObject(const char* objectClassName)
{
  IlcCDBMetaData *md1= new IlcCDBMetaData();
  md1->SetObjectClassName(objectClassName);
  md1->SetResponsible("Haavard Helstrup");
  md1->SetBeamPeriod(kBeamPeriod);
  md1->SetIlcRootVersion(ILCROOT_SVN_BRANCH);

  return md1;
}

//______________________________________________________________________________________________
void IlcDCSGenDB::StoreObject(const char* cdbPath, TObject* object, IlcCDBMetaData* metaData)
{

  IlcCDBId id1(cdbPath, fFirstRun, fLastRun);
  if (fStorLoc) fStorLoc->Put(object, id1, metaData);
}

//______________________________________________________________________________________________
void IlcDCSGenDB::Init(Int_t run, const char *configDir, 
                                  const char *specificDir,
				  const char *sensorClass)
{

   fMetaData = CreateMetaObject(sensorClass);
   IlcCDBManager *man = IlcCDBManager::Instance();
   man->SetDefaultStorage(fDefaultStorage);
   man->SetRun(run);
   man->SetSpecificStorage(specificDir,fSpecificStorage);
   IlcCDBEntry *config = man->Get(configDir);
   if (config) fConfTree = (TTree*)config->GetObject();
   fStorLoc = man->GetStorage(fSpecificStorage);
   if (!fStorLoc)    return;

   /*Bool_t cdbCache = */IlcCDBManager::Instance()->GetCacheFlag(); // save cache status
   IlcCDBManager::Instance()->SetCacheFlag(kTRUE); // activate CDB cache
   

}

//______________________________________________________________________________________________


//_____________________________________________________________________________
TMap* IlcDCSGenDB::SetGraphFile(const char *fname)
{
  //
  // Read DCS maps from file given by fname
  //
  TFile file(fname);
  TMap * map = (TMap*)file.Get("DCSMap");
  return map;
}

//______________________________________________________________________________________________

TClonesArray * IlcDCSGenDB::ReadList(const char *fname, const char *title) {
  //
  // read values from ascii file
  //
  TTree* tree = new TTree(title,title);
  tree->ReadFile(fname,"");
  TClonesArray *arr = IlcDCSSensor::ReadTree(tree);
  delete tree;
  return arr;
}

//______________________________________________________________________________________________

TTree * IlcDCSGenDB::ReadListTree(const char *fname, const char *title) {
  //
  // read values from ascii file
  //
  TTree* tree = new TTree(title,title);
  tree->ReadFile(fname,"");
  TClonesArray *arr = IlcDCSSensor::ReadTree(tree);
  arr->Delete();
  delete arr;
  return tree;
}



