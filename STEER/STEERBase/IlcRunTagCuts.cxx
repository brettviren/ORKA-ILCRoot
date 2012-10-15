/**************************************************************************
 * Author: Panos Christakoglou.                                           *
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

/* $Id: IlcRunTagCuts.cxx 50615 2011-07-16 23:19:19Z hristov $ */

//-----------------------------------------------------------------
//                   IlcRunTagCuts class
//   This is the class to deal with the run tag level cuts
//   Origin: Panos Christakoglou, UOA-CERN, Panos.Christakoglou@cern.ch
//-----------------------------------------------------------------

class IlcLog;
class IlcESD;

#include "IlcRunTag.h"
#include "IlcRunTagCuts.h"

ClassImp(IlcRunTagCuts)


//___________________________________________________________________________
IlcRunTagCuts::IlcRunTagCuts() :
  TObject(),
  fIlcRunId(-1),                  
  fIlcRunIdFlag(kFALSE),              
  fIlcMagneticField(-1.),          
  fIlcMagneticFieldFlag(kFALSE),      
  fIlcDipoleField(-1.),
  fIlcDipoleFieldFlag(kFALSE),
  fIlcRunStartTimeMin(-1),        
  fIlcRunStartTimeMax(-1),        
  fIlcRunStartTimeFlag(kFALSE),       
  fIlcRunStopTimeMin(-1),         
  fIlcRunStopTimeMax(-1),         
  fIlcRunStopTimeFlag(kFALSE),        
  fIlcrootVersion(""),              
  fIlcrootVersionFlag(kFALSE),          
  fRootVersion(""),                 
  fRootVersionFlag(kFALSE),             
  fGeant3Version(""),               
  fGeant3VersionFlag(kFALSE),   
  fLHCPeriod(""),
  fLHCPeriodFlag(kFALSE),
  fRecPass(""),
  fRecPassFlag(kFALSE),
  fProdName(""),
  fProdNameFlag(kFALSE),
  fIlcRunValidation(0),             
  fIlcRunValidationFlag(kFALSE),         
  fIlcRunQualities(""),
  fIlcRunQualitiesFlag(kFALSE),
  fIlcBeamEnergy(-1),             
  fIlcBeamEnergyFlag(kFALSE),         
  fIlcBeamType(""),               
  fIlcBeamTypeFlag(kFALSE),           
  fIlcCalibrationVersion(-1),    
  fIlcCalibrationVersionFlag(kFALSE),
  fIlcDataType(-1),                
  fIlcDataTypeFlag(kFALSE),
  fBeamTriggerRange(),
  fBeamTriggerFlag(kFALSE),
  fCollisionTriggerRange(),
  fCollisionTriggerFlag(kFALSE),
  fEmptyTriggerRange(),
  fEmptyTriggerFlag(kFALSE),
  fASideTriggerRange(),
  fASideTriggerFlag(kFALSE),
  fCSideTriggerRange(),
  fCSideTriggerFlag(kFALSE),
  fHMTriggerRange(),
  fHMTriggerFlag(kFALSE),
  fMuonTriggerRange(),
  fMuonTriggerFlag(kFALSE),
  fCollisionRateRange(),
  fCollisionRateFlag(kFALSE),
  fMeanVertexRange(),
  fMeanVertexFlag(kFALSE),
  fVertexQualityRange(),
  fVertexQualityFlag(kFALSE)
{
  //Default constructor which calls the Reset method.
}

//___________________________________________________________________________
IlcRunTagCuts::~IlcRunTagCuts() {  
  //Defaut destructor.
}

//___________________________________________________________________________
void IlcRunTagCuts::Reset() {
  //Sets dummy values to every private member.
  fIlcRunId = -1;                  
  fIlcRunIdFlag = kFALSE;              
  fIlcMagneticField = -1.;          
  fIlcMagneticFieldFlag = kFALSE;      
  fIlcDipoleField = -1.;
  fIlcDipoleFieldFlag = kFALSE;
  fIlcRunStartTimeMin = -1;        
  fIlcRunStartTimeMax = -1;        
  fIlcRunStartTimeFlag = kFALSE;       
  fIlcRunStopTimeMin = -1;         
  fIlcRunStopTimeMax = -1;         
  fIlcRunStopTimeFlag = kFALSE;        
  fIlcrootVersion = "";              
  fIlcrootVersionFlag = kFALSE;          
  fRootVersion = "";                 
  fRootVersionFlag = kFALSE;             
  fGeant3Version = "";               
  fGeant3VersionFlag = kFALSE;           
  fLHCPeriod = "";
  fLHCPeriodFlag = kFALSE;
  fRecPass = "";
  fRecPassFlag = kFALSE;
  fProdName = "";
  fProdNameFlag = kFALSE;
  fIlcRunValidation = 0;             
  fIlcRunValidationFlag = kFALSE;         
  fIlcRunQualities = "";
  fIlcRunQualitiesFlag = kFALSE;
  fIlcBeamEnergy = -1;             
  fIlcBeamEnergyFlag = kFALSE;         
  fIlcBeamType = "";               
  fIlcBeamTypeFlag = kFALSE;           
  fIlcCalibrationVersion = -1;    
  fIlcCalibrationVersionFlag = kFALSE;
  fIlcDataType = -1;                
  fIlcDataTypeFlag = kFALSE;           
  fBeamTriggerRange[0] = 0;
  fBeamTriggerRange[1] = 0;
  fBeamTriggerFlag = kFALSE;
  fCollisionTriggerRange[0] = 0;
  fCollisionTriggerRange[1] = 0;
  fCollisionTriggerFlag = kFALSE;
  fEmptyTriggerRange[0] = 0;
  fEmptyTriggerRange[1] = 0;
  fEmptyTriggerFlag = kFALSE;
  fASideTriggerRange[0] = 0;
  fASideTriggerRange[1] = 0;
  fASideTriggerFlag = kFALSE;
  fCSideTriggerRange[0] = 0;
  fCSideTriggerRange[1] = 0;
  fCSideTriggerFlag = kFALSE;
  fHMTriggerRange[0] = 0;
  fHMTriggerRange[1] = 0;
  fHMTriggerFlag = kFALSE;
  fMuonTriggerRange[0] = 0;
  fMuonTriggerRange[1] = 0;
  fMuonTriggerFlag = kFALSE;
  fCollisionRateRange[0] = 0;
  fCollisionRateRange[1] = 0;
  fCollisionRateFlag = kFALSE;
  fMeanVertexRange[0] = 0;
  fMeanVertexRange[1] = 0;
  fMeanVertexFlag = kFALSE;
  fVertexQualityRange[0] = 0;
  fVertexQualityRange[1] = 0;
  fVertexQualityFlag = kFALSE;
}

void IlcRunTagCuts::AddRunQualityValue(Int_t qval)
{
  // Adds to the list of selected run qualities
  fIlcRunQualities += qval;
  fIlcRunQualities += " ";
  fIlcRunQualitiesFlag = kTRUE;
}

//___________________________________________________________________________
Bool_t IlcRunTagCuts::IsAccepted(IlcRunTag *RunTag) const {
  //Returns true if the event is accepted otherwise false.
  if(fIlcRunIdFlag)
    if((RunTag->GetRunId() != fIlcRunId))
      return kFALSE;
  if(fIlcMagneticFieldFlag)
    if((RunTag->GetMagneticField() != fIlcMagneticField))
      return kFALSE;
  if(fIlcDipoleFieldFlag)
    if((RunTag->GetDipoleField() != fIlcDipoleField))
      return kFALSE;
  if(fIlcRunStartTimeFlag)
    if((RunTag->GetRunStartTime() < fIlcRunStartTimeMin) || (RunTag->GetRunStartTime() > fIlcRunStartTimeMax))
      return kFALSE;
  if(fIlcRunStopTimeFlag)
    if((RunTag->GetRunStopTime() < fIlcRunStopTimeMin) || (RunTag->GetRunStopTime() > fIlcRunStopTimeMax))
      return kFALSE;
  if(fIlcrootVersionFlag)
    if((RunTag->GetIlcrootVersion() != fIlcrootVersion))
      return kFALSE;
  if(fRootVersionFlag)
    if((RunTag->GetRootVersion() != fRootVersion))
      return kFALSE;
  if(fGeant3VersionFlag)
    if((RunTag->GetGeant3Version() != fGeant3Version))
      return kFALSE;
  if(fLHCPeriodFlag)
    if((RunTag->GetLHCPeriod() != fLHCPeriod))
      return kFALSE;
  if(fRecPassFlag)
    if((RunTag->GetReconstructionPass() != fRecPass))
      return kFALSE;
  if(fProdNameFlag)
    if((RunTag->GetProductionName() != fProdName))
      return kFALSE;
  if(fIlcRunValidationFlag)
    if(RunTag->GetRunValidation())
      return kFALSE;
  if (fIlcRunQualitiesFlag) {
    TObjArray *tQualities = fIlcRunQualities.Tokenize(" ");
    Bool_t tQual = kFALSE;

    TString tRQual = "";
    tRQual += RunTag->GetRunQuality();

    for (int iqual=0; iqual<tQualities->GetEntries(); iqual++)
      if (((TObjString *) tQualities->At(iqual))->GetString().Contains(tRQual))
	tQual = kTRUE;
	//      if (EvTag->GetFiredTriggerClasses().Contains(((TObjString *) tClasses->At(iqual))->GetString()))
  
    tQualities->Delete();
    delete tQualities;

    if (!tQual)
      return kFALSE;
  }
  if(fIlcBeamEnergyFlag)
    if(RunTag->GetBeamEnergy() != fIlcBeamEnergy)
      return kFALSE;
  if(fIlcBeamTypeFlag)
    if(RunTag->GetBeamType() != fIlcBeamType)
      return kFALSE;
  if(fIlcCalibrationVersionFlag)
    if(RunTag->GetBeamEnergy() != fIlcBeamEnergy)
      return kFALSE;
  if(fIlcDataTypeFlag)
    if(RunTag->GetDataType() != fIlcDataType)
      return kFALSE;
  if (fBeamTriggerFlag)
    if ((RunTag->GetBeamTriggers() < fBeamTriggerRange[0]) || (RunTag->GetBeamTriggers() > fBeamTriggerRange[1]))
      return kFALSE;
  if (fCollisionTriggerFlag)
    if ((RunTag->GetCollisionTriggers() < fCollisionTriggerRange[0]) || (RunTag->GetCollisionTriggers() > fCollisionTriggerRange[1]))
      return kFALSE;
  if (fEmptyTriggerFlag)
    if ((RunTag->GetEmptyTriggers() < fEmptyTriggerRange[0]) || (RunTag->GetEmptyTriggers() > fEmptyTriggerRange[1]))
      return kFALSE;
  if (fCSideTriggerFlag)
    if ((RunTag->GetCSideTriggers() < fCSideTriggerRange[0]) || (RunTag->GetCSideTriggers() > fCSideTriggerRange[1]))
      return kFALSE;
  if (fASideTriggerFlag)
    if ((RunTag->GetASideTriggers() < fASideTriggerRange[0]) || (RunTag->GetASideTriggers() > fASideTriggerRange[1]))
      return kFALSE;
  if (fHMTriggerFlag)
    if ((RunTag->GetHMTriggers() < fHMTriggerRange[0]) || (RunTag->GetHMTriggers() > fHMTriggerRange[1]))
      return kFALSE;
  if (fMuonTriggerFlag)
    if ((RunTag->GetMuonTriggers() < fMuonTriggerRange[0]) || (RunTag->GetMuonTriggers() > fMuonTriggerRange[1]))
      return kFALSE;
  if (fCollisionRateFlag)
    if ((RunTag->GetCollisionRate() < fCollisionRateRange[0]) || (RunTag->GetCollisionRate() > fCollisionRateRange[1]))
      return kFALSE;
  if (fMeanVertexFlag)
    if ((RunTag->GetMeanVertex() < fMeanVertexRange[0]) || (RunTag->GetMeanVertex() > fMeanVertexRange[1]))
      return kFALSE;
  if (fVertexQualityFlag)
    if ((RunTag->GetVertexQuality() < fVertexQualityRange[0]) || (RunTag->GetVertexQuality() > fVertexQualityRange[1]))
      return kFALSE;

  return kTRUE;
}
