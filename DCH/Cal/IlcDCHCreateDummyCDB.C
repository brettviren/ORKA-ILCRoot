#if !defined( __CINT__) || defined(__MAKECINT__)

#include <iostream>

#include <IlcCDBManager.h>
#include <IlcCDBStorage.h>
#include <IlcCDBEntry.h>
#include <IlcCDBMetaData.h>

#include "IlcTRDgeometry.h"

#include "IlcTRDCalROC.h"
#include "IlcTRDCalChamberPos.h"
#include "IlcTRDCalStackPos.h"
#include "IlcTRDCalSuperModulePos.h"
#include "IlcTRDCalPad.h"
#include "IlcTRDCalDet.h"
#include "IlcTRDCalGlobals.h"

#include "IlcTRDCalSuperModuleStatus.h"
#include "IlcTRDCalChamberStatus.h"
#include "IlcTRDCalMCMStatus.h"
#include "IlcTRDCalPadStatus.h"
#include "IlcTRDCalSingleChamberStatus.h"

#include "IlcTRDCalPIDLQ.h"
#include "IlcTRDCalMonitoring.h"

#endif

// run number for the dummy file
const Int_t gkDummyRun = 0;
IlcCDBStorage* gStorLoc = 0;

TObject* CreatePadObject(const char* shortName, const char* description, Float_t value)
{
  IlcTRDCalPad *calPad = new IlcTRDCalPad(shortName, description);
  for (Int_t det=0; det<IlcTRDgeometry::kNdet; ++det)
  {
    IlcTRDCalROC *calROC = calPad->GetCalROC(det);
    for (Int_t channel=0; channel<calROC->GetNchannels(); ++channel)
      calROC->SetValue(channel, value);
  }
  return calPad;
}

TObject* CreateDetObject(const char* shortName, const char* description, Float_t value)
{
  IlcTRDCalDet *object = new IlcTRDCalDet(shortName, description);
  for (Int_t det=0; det<IlcTRDgeometry::kNdet; ++det)
    object->SetValue(det, value);
  return object;
}

TObject* CreateGlobalsObject()
{
  IlcTRDCalGlobals *object = new IlcTRDCalGlobals("Globals", "Global TRD calibration parameters");

  object->SetSamplingFrequency(10.0);
  object->SetNumberOfTimeBins(22);
  
  return object;
}

TObject* CreateChamberObject()
{
  IlcTRDCalChamberPos *object = new IlcTRDCalChamberPos("Chamber", "TRD chamber positions");
  
  for (Int_t det=0; det<IlcTRDgeometry::kNdet; ++det)
  {
    object->SetPos(det, 0, 0, 0);
    object->SetRot(det, 0, 0, 0);
  }
  
  return object;
}

TObject* CreateStackObject()
{
  IlcTRDCalStackPos *object = new IlcTRDCalStackPos("Stack", "TRD stack positions");

  for (Int_t sect=0; sect<IlcTRDgeometry::kNsect; ++sect)
  {
    for (Int_t chamber=0; chamber<IlcTRDgeometry::kNcham; ++chamber)
    {
      object->SetPos(chamber, sect, 0, 0, 0);
      object->SetRot(chamber, sect, 0, 0, 0);
    }
  }

  return object;
}

TObject* CreateSuperModuleObject()
{
  IlcTRDCalSuperModulePos *object = new IlcTRDCalSuperModulePos("Stack", "TRD supermodule positions");

  for (Int_t sect=0; sect<IlcTRDgeometry::kNsect; ++sect)
  {
    object->SetPos(sect, 0, 0, 0);
    object->SetRot(sect, 0, 0, 0);
  }

  return object;
}


TObject* CreatePRFWidthObject()
{
  IlcTRDCalPad *calPad = new IlcTRDCalPad("PRFWidth","PRFWidth");
  for (Int_t plane=0; plane<IlcTRDgeometry::kNplan; ++plane)
  {
    Float_t value = 0;
    switch (plane)
    {
      case 0: value = 0.515; break;
      case 1: value = 0.502; break;
      case 2: value = 0.491; break;
      case 3: value = 0.481; break;
      case 4: value = 0.471; break;
      case 5: value = 0.463; break;
      default: cout << "CreatePRFWidthObject: UNEXPECTED" << endl; return 0;
    }
    for (Int_t chamber=0; chamber<IlcTRDgeometry::kNcham; ++chamber)
    {
      for (Int_t sector=0; sector<IlcTRDgeometry::kNsect; ++sector)
      {
        IlcTRDCalROC *calROC = calPad->GetCalROC(plane, chamber, sector);
        for (Int_t channel=0; channel<calROC->GetNchannels(); ++channel)
          calROC->SetValue(channel, value);
      }
    }
  }
      
  return calPad;
}

IlcTRDCalSuperModuleStatus* CreateSuperModuleStatusObject()
{
  IlcTRDCalSuperModuleStatus* obj = new IlcTRDCalSuperModuleStatus("supermodulestatus", "supermodulestatus");

  for (Int_t i=0; i<IlcTRDgeometry::kNsect; ++i)
    obj->SetStatus(i, IlcTRDCalSuperModuleStatus::kInstalled);

  return obj;
}

IlcTRDCalChamberStatus* CreateChamberStatusObject()
{
  IlcTRDCalChamberStatus* obj = new IlcTRDCalChamberStatus("chamberstatus", "chamberstatus");

  for (Int_t i=0; i<IlcTRDgeometry::kNdet; ++i)
    obj->SetStatus(i, IlcTRDCalChamberStatus::kInstalled);

  return obj;
}

IlcTRDCalMCMStatus* CreateMCMStatusObject()
{
  IlcTRDCalMCMStatus* obj = new IlcTRDCalMCMStatus("mcmstatus", "mcmstatus");

  return obj;
}

IlcTRDCalPadStatus* CreatePadStatusObject()
{
  IlcTRDCalPadStatus* obj = new IlcTRDCalPadStatus("padstatus", "padstatus");

  return obj;
}

IlcTRDCalPIDLQ* CreatePIDLQObject()
{
  IlcTRDCalPIDLQ* pid = new IlcTRDCalPIDLQ("pidobject", "pidobject");
  pid->ReadData("$ILC_ROOT/TRD/TRDdEdxHistogramsV1.root");
  pid->SetMeanChargeRatio(1.0); // The factor is the ratio of Mean of pi charge dist.
                    // for the New TRD code divided by the Mean of pi charge
                    // dist. given in IlcTRDCalPIDLQ object
  
  return pid;
}

IlcTRDCalMonitoring* CreateMonitoringObject()
{
  IlcTRDCalMonitoring* obj = new IlcTRDCalMonitoring();

  return obj;
}

IlcCDBMetaData* CreateMetaObject(const char* objectClassName)
{
  IlcCDBMetaData *md1= new IlcCDBMetaData(); 
  md1->SetObjectClassName(objectClassName);
  md1->SetResponsible("Jan Fiete Grosse-Oetringhaus");
  md1->SetBeamPeriod(1);
  md1->SetIlcRootVersion("05-06-00"); //root version
  md1->SetComment("The dummy values in this calibration file are for testing only");
  
  return md1;
}

void StoreObject(const char* cdbPath, TObject* object, IlcCDBMetaData* metaData)
{
  IlcCDBId id1(cdbPath, gkDummyRun, gkDummyRun); 
  gStorLoc->Put(object, id1, metaData); 
}
    

void IlcTRDCreateDummyCDB()
{
  cout << endl << "TRD :: Creating dummy CDB with event number " << gkDummyRun << endl;
  
  IlcCDBManager *man = IlcCDBManager::Instance();
  gStorLoc = man->GetStorage("local://$ILC_ROOT");
  if (!gStorLoc)
    return;

  TObject* obj = 0;
  IlcCDBMetaData* metaData = 0;
  
  metaData = CreateMetaObject("IlcTRDCalPad");
  
  obj = CreatePadObject("LocalVdrift","TRD drift velocities (local variations)", 1);
  StoreObject("TRD/Calib/LocalVdrift", obj, metaData);
  
  obj = CreatePadObject("LocalT0","T0 (local variations)", 1);
  StoreObject("TRD/Calib/LocalT0", obj, metaData);

  obj = CreatePadObject("GainFactor","GainFactor (local variations)", 1);
  StoreObject("TRD/Calib/LocalGainFactor", obj, metaData);

  obj = CreatePRFWidthObject();
  StoreObject("TRD/Calib/PRFWidth", obj, metaData);

  metaData = CreateMetaObject("IlcTRDCalDet");
  
  obj = CreateDetObject("ChamberVdrift","TRD drift velocities (detector value)", 1.5);
  StoreObject("TRD/Calib/ChamberVdrift", obj, metaData);
  
  obj = CreateDetObject("ChamberT0","T0 (detector value)", 0);
  StoreObject("TRD/Calib/ChamberT0", obj, metaData);
  
  obj = CreateDetObject("ChamberGainFactor","GainFactor (detector value)", 1);
  StoreObject("TRD/Calib/ChamberGainFactor", obj, metaData);
  
  metaData = CreateMetaObject("IlcTRDCalGlobals");
  obj = CreateGlobalsObject();
  StoreObject("TRD/Calib/Globals", obj, metaData);
  
  metaData = CreateMetaObject("IlcTRDCalChamberPos");
  obj = CreateChamberObject();
  StoreObject("TRD/Calib/ChamberPos", obj, metaData);

  metaData = CreateMetaObject("IlcTRDCalStackPos");
  obj = CreateStackObject();
  StoreObject("TRD/Calib/StackPos", obj, metaData);

  metaData = CreateMetaObject("IlcTRDCalSuperModulePos");
  obj = CreateSuperModuleObject();
  StoreObject("TRD/Calib/SuperModulePos", obj, metaData);

  metaData = CreateMetaObject("IlcTRDCalSuperModuleStatus");
  obj = CreateSuperModuleStatusObject();
  StoreObject("TRD/Calib/SuperModuleStatus", obj, metaData);

  metaData = CreateMetaObject("IlcTRDCalChamberStatus");
  obj = CreateChamberStatusObject();
  StoreObject("TRD/Calib/ChamberStatus", obj, metaData);

  metaData = CreateMetaObject("IlcTRDCalMCMStatus");
  obj = CreateMCMStatusObject();
  StoreObject("TRD/Calib/MCMStatus", obj, metaData);

  metaData = CreateMetaObject("IlcTRDCalPadStatus");
  obj = CreatePadStatusObject();
  StoreObject("TRD/Calib/PadStatus", obj, metaData);

  metaData = CreateMetaObject("IlcTRDCalPIDLQ");
  obj = CreatePIDLQObject();
  StoreObject("TRD/Calib/PIDLQ", obj, metaData);

  metaData = CreateMetaObject("IlcTRDCalMonitoring");
  obj = CreateMonitoringObject();
  StoreObject("TRD/Calib/MonitoringData", obj, metaData);
}
