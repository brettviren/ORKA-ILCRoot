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

/* $Id: IlcRSTACKCalibData.cxx 53738 2011-12-20 16:49:27Z kharlov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// class for RSTACK calibration                                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TRandom.h"
#include "IlcLog.h"
#include "IlcRSTACKCalibData.h"
#include "IlcCDBManager.h"
#include "IlcCDBStorage.h"
#include "IlcCDBId.h"
#include "IlcCDBEntry.h"
#include "IlcRSTACKEmcCalibData.h"
#include "IlcRSTACKCpvCalibData.h"
#include "IlcRSTACKEmcBadChannelsMap.h"
#include "IlcCDBMetaData.h"

ClassImp(IlcRSTACKCalibData)

//________________________________________________________________
  IlcRSTACKCalibData::IlcRSTACKCalibData(): 
    TNamed(), 
    fCalibDataEmc(0x0), 
    fCalibDataCpv(0x0),
    fEmcBadChannelsMap(0x0),
    fEmcDataPath("RSTACK/Calib/EmcGainPedestals"),
    fCpvDataPath("RSTACK/Calib/CpvGainPedestals"),
    fEmcBadChannelsMapPath("RSTACK/Calib/EmcBadChannels")
{
  // Default constructor.
  // Open CDB entry, get EMC and CPV calibration data and bad channel map.
  // If EMC or CPV calibration data does not exist, stop the run
 
}

//________________________________________________________________
IlcRSTACKCalibData::IlcRSTACKCalibData(Int_t runNumber) :
  TNamed("RSTACKCalib","RSTACK Calibration Data Manager"),
  fCalibDataEmc(0x0), fCalibDataCpv(0x0), fEmcBadChannelsMap(0x0),
  fEmcDataPath("RSTACK/Calib/EmcGainPedestals"),
  fCpvDataPath("RSTACK/Calib/CpvGainPedestals"),
  fEmcBadChannelsMapPath("RSTACK/Calib/EmcBadChannels")
{
  // Constructor
  // Open CDB entry, get EMC and CPV calibration data and bad channel map.
  // If EMC or CPV calibration data does not exist, stop the run

  IlcCDBEntry* entryEmc = IlcCDBManager::Instance()->Get(fEmcDataPath.Data(),runNumber);
  if(entryEmc)
    fCalibDataEmc = (IlcRSTACKEmcCalibData*)entryEmc->GetObject();

  if(!fCalibDataEmc)
    IlcFatal("Calibration parameters for RSTACK EMC not found. Stop reconstruction!\n");
  
  IlcCDBEntry* entryCpv = IlcCDBManager::Instance()->Get(fCpvDataPath.Data(),runNumber);
  if(entryCpv)
    fCalibDataCpv = (IlcRSTACKCpvCalibData*)entryCpv->GetObject();

  if(!fCalibDataCpv)
    IlcFatal("Calibration parameters for RSTACK CPV not found. Stop reconstruction!\n");
  
  IlcCDBEntry* entryEmcBadMap = IlcCDBManager::Instance()->
    Get(fEmcBadChannelsMapPath.Data(),runNumber);
  if(entryEmcBadMap)
    fEmcBadChannelsMap = (IlcRSTACKEmcBadChannelsMap*)entryEmcBadMap->GetObject(); 

}

//________________________________________________________________
IlcRSTACKCalibData::IlcRSTACKCalibData(IlcRSTACKCalibData & RSTACKCDB) :
  TNamed(RSTACKCDB),
  fCalibDataEmc(RSTACKCDB.fCalibDataEmc),
  fCalibDataCpv(RSTACKCDB.fCalibDataCpv),
  fEmcBadChannelsMap(RSTACKCDB.fEmcBadChannelsMap),
  fEmcDataPath(RSTACKCDB.fEmcDataPath),
  fCpvDataPath(RSTACKCDB.fCpvDataPath),
  fEmcBadChannelsMapPath(RSTACKCDB.fEmcBadChannelsMapPath)
{
  // Copy constructor
}
//________________________________________________________________
IlcRSTACKCalibData::~IlcRSTACKCalibData()
{
  // Destructor
 
}

//________________________________________________________________
IlcRSTACKCalibData & IlcRSTACKCalibData::operator = (const IlcRSTACKCalibData & rhs)
{
  //Copy-assignment. Does not delete anything (see destructor)
  //compiler generated is ok, but ... because -Weffc++ and pointer
  //members we have to define it explicitly.
  TNamed::operator=(rhs);
  if (this != &rhs) {
    fCalibDataEmc = rhs.fCalibDataEmc;
    fCalibDataCpv = rhs.fCalibDataCpv;
    fEmcBadChannelsMap = rhs.fEmcBadChannelsMap;
    fEmcDataPath  = rhs.fEmcDataPath;
    fCpvDataPath  = rhs.fCpvDataPath;
    fEmcBadChannelsMapPath = rhs.fEmcBadChannelsMapPath;
  }
  else {
    IlcFatal("Self assignment!");
  }
  return *this;
}

//________________________________________________________________
void IlcRSTACKCalibData::Reset()
{
  // Set all pedestals to 0 and all ADC channels to 1,
  // and all channels are good (alive)

  fCalibDataEmc     ->Reset();
  fCalibDataCpv     ->Reset();
  fEmcBadChannelsMap->Reset();
}

//________________________________________________________________
void  IlcRSTACKCalibData::Print(Option_t *option) const
{
  // Print EMC and CPV calibration containers
  // Input: option="ped"  to print pedestals
  //        option="gain" to print calibration coefficients
  if (fCalibDataEmc) fCalibDataEmc->Print(option);
  if (fCalibDataCpv) fCalibDataCpv->Print(option);
}

//________________________________________________________________
void IlcRSTACKCalibData::CreateNew()
{
  // Create new EMC and CPV calibration containers with ideal coefficients

  if(fCalibDataEmc) delete fCalibDataEmc;
  fCalibDataEmc = new IlcRSTACKEmcCalibData("RSTACK-EMC");

  if(fCalibDataCpv) delete fCalibDataCpv;
  fCalibDataCpv = new IlcRSTACKCpvCalibData("RSTACK-CPV");

  if(fEmcBadChannelsMap) delete fEmcBadChannelsMap;
  fEmcBadChannelsMap = new IlcRSTACKEmcBadChannelsMap();

}

//________________________________________________________________
Bool_t IlcRSTACKCalibData::WriteEmc(Int_t firstRun, Int_t lastRun, IlcCDBMetaData *md)
{
  // Write EMC calibration container to CDB

  if(!fCalibDataEmc) return kFALSE;

  IlcCDBStorage* storage = IlcCDBManager::Instance()->GetSpecificStorage("RSTACK/*");
  if(!storage)
    storage = IlcCDBManager::Instance()->GetDefaultStorage();

  if(storage) { 
    IlcCDBId id(fEmcDataPath.Data(),firstRun,lastRun);
    storage->Put(fCalibDataEmc,id, md);
    return kTRUE;
  }
  else
    return kFALSE;

}

//________________________________________________________________
Bool_t IlcRSTACKCalibData::WriteCpv(Int_t firstRun, Int_t lastRun, IlcCDBMetaData *md)
{
  // Write CPV calibration container to CDB

  if(!fCalibDataCpv) return kFALSE;
  
  IlcCDBStorage* storage = IlcCDBManager::Instance()->GetSpecificStorage("RSTACK/*");
  if(!storage)
    storage = IlcCDBManager::Instance()->GetDefaultStorage();

  if(storage) { 
    IlcCDBId id(fCpvDataPath.Data(),firstRun,lastRun);
    storage->Put(fCalibDataCpv,id, md);
    return kTRUE;
  }
  else
    return kFALSE;

}


//________________________________________________________________
Bool_t IlcRSTACKCalibData::WriteEmcBadChannelsMap(Int_t firstRun,Int_t lastRun,IlcCDBMetaData *md)
{
  //Write EMC bad channels map into CDB.

  if(!fEmcBadChannelsMap) return kFALSE;
  
  IlcCDBStorage* storage = IlcCDBManager::Instance()->GetSpecificStorage("RSTACK/*");
  if(!storage)
    storage = IlcCDBManager::Instance()->GetDefaultStorage();

  if(storage) { 
    IlcCDBId id(fEmcBadChannelsMapPath.Data(),firstRun,lastRun);
    storage->Put(fEmcBadChannelsMap,id, md);
    return kTRUE;
  }
  else
    return kFALSE;
}

//________________________________________________________________
Float_t IlcRSTACKCalibData::GetADCchannelEmc(Int_t module, Int_t column, Int_t row) const
{
  // Return EMC calibration coefficient
  // for channel defined by (module,column,row)
  // module, column,raw should follow the internal RSTACK convention:
  // module 1:5, column 1:56, row 1:64
  // if CBD instance exists, the value is taken from CDB.
  // Otherwise it is an ideal one

  if(fCalibDataEmc) 
    return fCalibDataEmc->GetADCchannelEmc(module,column,row);
  else
    return 1.0; // default width of one EMC ADC channel in GeV
}

//________________________________________________________________
void IlcRSTACKCalibData::SetADCchannelEmc(Int_t module, Int_t column, Int_t row, Float_t value)
{
  // Set EMC calibration coefficient for (module,column,row)

  if(!fCalibDataEmc)
    fCalibDataEmc = new IlcRSTACKEmcCalibData("RSTACK-EMC");

  fCalibDataEmc->SetADCchannelEmc(module,column,row,value);
}

//________________________________________________________________
Float_t IlcRSTACKCalibData::GetADCpedestalEmc(Int_t module, Int_t column, Int_t row) const
{
  // Return EMC pedestal for channel defined by (module,column,row)
  // module, column,raw should follow the internal RSTACK convention:
  // module 1:5, column 1:56, row 1:64
  // if CBD instance exists, the value is taken from CDB.
  // Otherwise it is an ideal one

  if(fCalibDataEmc) 
    return fCalibDataEmc->GetADCpedestalEmc(module,column,row);
  else
    return 0.0; // default EMC ADC pedestal
}

//________________________________________________________________
void IlcRSTACKCalibData::SetADCpedestalEmc(Int_t module, Int_t column, Int_t row, Float_t value)
{
  // Set EMC pedestal for (module,column,row)

  if(!fCalibDataEmc)
    fCalibDataEmc = new IlcRSTACKEmcCalibData("RSTACK-EMC");

  fCalibDataEmc->SetADCpedestalEmc(module,column,row,value);
}

//________________________________________________________________
Float_t IlcRSTACKCalibData::GetHighLowRatioEmc(Int_t module, Int_t column, Int_t row) const
{
  // Return EMC calibration coefficient
  // for channel defined by (module,column,row)
  // module, column,raw should follow the internal RSTACK convention:
  // module 1:5, column 1:56, row 1:64
  // if CBD instance exists, the value is taken from CDB.
  // Otherwise it is an ideal one
 
  if(fCalibDataEmc)
    return fCalibDataEmc->GetHighLowRatioEmc(module,column,row);
  else
    return 1.0; // default width of one EMC ADC channel in GeV
}
 
//________________________________________________________________
void IlcRSTACKCalibData::SetHighLowRatioEmc(Int_t module, Int_t column, Int_t row, Float_t value)
{
  // Set EMC calibration coefficient for (module,column,row)
 
  if(!fCalibDataEmc)
    fCalibDataEmc = new IlcRSTACKEmcCalibData("RSTACK-EMC");
 
  fCalibDataEmc->SetHighLowRatioEmc(module,column,row,value);
}
 
//________________________________________________________________
Float_t IlcRSTACKCalibData::GetTimeShiftEmc(Int_t module, Int_t column, Int_t row) const
{ 
  // Return EMC calibration coefficient 
  // for channel defined by (module,column,row)                                
  // module, column,raw should follow the internal RSTACK convention:            
  // module 1:5, column 1:56, row 1:64 
  // if CBD instance exists, the value is taken from CDB. 
  // Otherwise it is an ideal one  
  
  if(fCalibDataEmc)
    return fCalibDataEmc->GetTimeShiftEmc(module,column,row);
  else
    return 1.0; // default width of one EMC ADC channel in GeV
}
 
//________________________________________________________________
void IlcRSTACKCalibData::SetTimeShiftEmc(Int_t module, Int_t column, Int_t row, Float_t value)
{
  // Set EMC calibration coefficient for (module,column,row)
 
  if(!fCalibDataEmc)
    fCalibDataEmc = new IlcRSTACKEmcCalibData("RSTACK-EMC");
 
  fCalibDataEmc->SetTimeShiftEmc(module,column,row,value);
}
//________________________________________________________________
Float_t IlcRSTACKCalibData::GetSampleTimeStep() const 
{
  //Get conversion coeff. from sample time step to seconds.
  //Negative value means that it is not used in reconstruction
  //but only in simulation of raw.
  if(fCalibDataEmc)
    return fCalibDataEmc->GetSampleTimeStep();
  else
    return 0.0; // default width of one EMC ADC channel in GeV
}
//________________________________________________________________
void   IlcRSTACKCalibData::SetSampleTimeStep(Float_t step)
{
  //Set conversion coeff. from sample time step to seconds.
  //Negative value means that it is not used in reconstruction
  //but only in simulation of raw.
  if(!fCalibDataEmc)
    fCalibDataEmc = new IlcRSTACKEmcCalibData("RSTACK-EMC");

  fCalibDataEmc->SetSampleTimeStep(step) ;
}
//________________________________________________________________
Int_t IlcRSTACKCalibData::GetAltroOffsetEmc(Int_t module, Int_t column, Int_t row) const
{
  // Return ALTRO pedestal coefficient
  // for channel defined by (module,column,row)
  // module, column,raw should follow the internal RSTACK convention:
  // module 1:5, column 1:56, row 1:64
  // if CBD instance exists, the value is taken from CDB.
  // Otherwise it is an ideal one
 
  if(fCalibDataEmc)
    return fCalibDataEmc->GetAltroOffsetEmc(module,column,row);
  else
    return 0; // default width of one EMC ADC channel in GeV
}
 
//________________________________________________________________
void IlcRSTACKCalibData::SetAltroOffsetEmc(Int_t module, Int_t column, Int_t row, Int_t value)
{
  // Set altro offset for (module,column,row)
 
  if(!fCalibDataEmc)
    fCalibDataEmc = new IlcRSTACKEmcCalibData("RSTACK-EMC");
 
  fCalibDataEmc->SetAltroOffsetEmc(module,column,row,value);
}

 
//________________________________________________________________
Float_t IlcRSTACKCalibData::GetADCchannelCpv(Int_t module, Int_t column, Int_t row) const
{
  // Return CPV calibration coefficient
  // for channel defined by (module,column,row)
  // module, column,raw should follow the internal CPV convention:
  // module 1:5, column 1:56, row 1:128
  // if CBD instance exists, the value is taken from CDB.
  // Otherwise it is an ideal one

  if(fCalibDataCpv) 
    return fCalibDataCpv->GetADCchannelCpv(module,column,row);
  else
    return 0.0012; // default width of one ADC channel in CPV arbitrary units
}

//________________________________________________________________
Float_t IlcRSTACKCalibData::GetADCpedestalCpv(Int_t module, Int_t column, Int_t row) const
{
  // Return CPV pedestal
  // for channel defined by (module,column,row)
  // module, column,raw should follow the internal CPV convention:
  // module 1:5, column 1:56, row 1:128
  // if CBD instance exists, the value is taken from CDB.
  // Otherwise it is an ideal one

  if(fCalibDataCpv) 
    return fCalibDataCpv->GetADCpedestalCpv(module,column,row);
  else
    return 0.012; // default CPV ADC pedestal
}

//________________________________________________________________
void IlcRSTACKCalibData::SetADCchannelCpv(Int_t module, Int_t column, Int_t row, Float_t value)
{
  // Set CPV calibration coefficient for (module,column,row)

  if(!fCalibDataCpv)
    fCalibDataCpv = new IlcRSTACKCpvCalibData("RSTACK-CPV");

  fCalibDataCpv->SetADCchannelCpv(module,column,row,value);
}

//________________________________________________________________
void IlcRSTACKCalibData::SetADCpedestalCpv(Int_t module, Int_t column, Int_t row, Float_t value)
{
  // Set CPV pedestal for (module,column,row)

  if(!fCalibDataCpv)
    fCalibDataCpv = new IlcRSTACKCpvCalibData("RSTACK-CPV");

  fCalibDataCpv->SetADCpedestalCpv(module,column,row,value);
}

//________________________________________________________________
void IlcRSTACKCalibData::RandomEmc(Float_t ccMin, Float_t ccMax)
{
  // Create decalibrated EMC with calibration coefficients and pedestals
  // randomly distributed within hard-coded limits
  // Default spread of calibration parameters is Cmax/Cmin = 4, (Cmax-Cmin)/2 = 1

  if(fCalibDataEmc) delete fCalibDataEmc;
  fCalibDataEmc = new IlcRSTACKEmcCalibData("RSTACK-EMC");

  TRandom rn;
  rn.SetSeed(0); //the seed is set to the current  machine clock
  
  Float_t adcChannelEmc,adcPedestalEmc;

  for(Int_t module=1; module<6; module++) {
    for(Int_t column=1; column<57; column++) {
      for(Int_t row=1; row<65; row++) {
        adcChannelEmc =rn.Uniform(ccMin,ccMax);
        adcPedestalEmc=rn.Uniform(0.0,0.0); // 0 spread of pedestals
        fCalibDataEmc->SetADCchannelEmc(module,column,row,adcChannelEmc);
        fCalibDataEmc->SetADCpedestalEmc(module,column,row,adcPedestalEmc);
      }
    }
  }

}

//________________________________________________________________
void IlcRSTACKCalibData::RandomCpv(Float_t ccMin, Float_t ccMax)
{
  // Create decalibrated CPV with calibration coefficients and pedestals
  // randomly distributed within hard-coded limits
  // Default spread of calibration parameters is  0.0012 +- 25%

  if(fCalibDataCpv) delete fCalibDataCpv;
  fCalibDataCpv = new IlcRSTACKCpvCalibData("RSTACK-CPV");

  TRandom rn;
  rn.SetSeed(0); //the seed is set to the current  machine clock
  
  Float_t adcChannelCpv,adcPedestalCpv;

  for(Int_t module=1; module<6; module++) {
    for(Int_t column=1; column<57; column++) {
      for(Int_t row=1; row<129; row++) {
	adcChannelCpv =rn.Uniform(ccMin,ccMax);
        adcPedestalCpv=rn.Uniform(0.0048,0.0192); // Ped[max]/Ped[min] = 4, <Ped> = 0.012
        fCalibDataCpv->SetADCchannelCpv(module,column,row,adcChannelCpv);
        fCalibDataCpv->SetADCpedestalCpv(module,column,row,adcPedestalCpv);
      }
    }
  }
}
//________________________________________________________________
Bool_t IlcRSTACKCalibData::IsBadChannelEmc(Int_t module, Int_t col, Int_t row) const
{
  //If no bad channels map found, channel considered good

  if(fEmcBadChannelsMap) 
    return fEmcBadChannelsMap->IsBadChannel(module,col,row);
  else
    return kFALSE;
}

//________________________________________________________________
Int_t IlcRSTACKCalibData::GetNumOfEmcBadChannels() const
{
  if(fEmcBadChannelsMap)
    return fEmcBadChannelsMap->GetNumOfBadChannels();
  else
    return 0;
}
//________________________________________________________________
void IlcRSTACKCalibData::EmcBadChannelIds(Int_t *badIds)
{
  //Fill array badIds by the Ids of EMC bad channels.
  //Array badIds of length GetNumOfBadChannels() should be prepared in advance. 

  if(fEmcBadChannelsMap)              
    fEmcBadChannelsMap->BadChannelIds(badIds);
}

//________________________________________________________________
Float_t IlcRSTACKCalibData::GetADCchannelEmcDecalib(Int_t module, Int_t column, Int_t row) const
{
  // Return random EMC (de)calibration factor O(1) for channel defined by (module,column,row). 
  // Used in simulation.
  
  // module, column,raw should follow the internal RSTACK convention:
  // module 1:5, column 1:56, row 1:64
  // if CBD instance exists, the value is taken from CDB.
  // Otherwise it is an ideal one (no decalibration).
  
  if(fCalibDataEmc) 
    return fCalibDataEmc->GetADCchannelEmcDecalib(module,column,row);
  else
    return 1.0; // no decalibration by default
}

//________________________________________________________________
void IlcRSTACKCalibData::SetADCchannelEmcDecalib(Int_t module, Int_t column, Int_t row, Float_t value)
{
  // Set EMC (de)calibration factor for (module,column,row).
  // Used in simulation.
  
  if(!fCalibDataEmc)
    fCalibDataEmc = new IlcRSTACKEmcCalibData("RSTACK-EMC");
  
  fCalibDataEmc->SetADCchannelEmcDecalib(module,column,row,value);
}
