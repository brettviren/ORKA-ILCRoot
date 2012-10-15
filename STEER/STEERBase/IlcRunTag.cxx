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

/* $Id: IlcRunTag.cxx 53510 2011-12-10 22:29:56Z shahoian $ */

//-----------------------------------------------------------------
//           Implementation of the RunTag class
//   This is the class to deal with the tags in the run level
//   Origin: Panos Christakoglou, UOA-CERN, Panos.Christakoglou@cern.ch
//-----------------------------------------------------------------

#include <stdlib.h>
#include "IlcRunTag.h"
#include "IlcDetectorTag.h"
#include "IlcEventTag.h"

ClassImp(IlcRunTag)

//___________________________________________________________________________
  IlcRunTag::IlcRunTag() :
    TObject(),
    fIlcRunId(-1),
    fIlcMagneticField(0.0),
    fIlcDipoleField(0.0),
    fIlcRunStartTime(0),
    fIlcRunStopTime(0),
    fIlcrootVersion(0),
    fRootVersion(0),
    fGeant3Version(0),
    fLHCPeriod(0),
    fRecPass(0),
    fProductionName(0),
    fIlcRunValidated(0),
    fIlcRunGlobalQuality(0),
    fIlcBeamEnergy(0.0),
    fIlcBeamType(0),
    fIlcCalibrationVersion(0),
    fIlcDataType(0),
    //    fNumEvents(0),
    fNumFiles(0),
    fBeamTriggers(0),
    fCollisionTriggers(0),
    fEmptyTriggers(0),
    fASideTriggers(0),
    fCSideTriggers(0),
    fHMTriggers(0),
    fMuonTriggers(0),
    fCollisionRate(0.0),
    fMeanVertex(0.0),
    fVertexQuality(0.0),
    fNumDetectors(0),
    fFileTags(100),
    fDetectorTag(),
    fLHCTag(), 
    fActiveTriggerClasses(""),
    fQA(),  
    fQALength(0), 
    fQAArray(NULL), 
    fESLength(0), 
    fEventSpecies(NULL)
{
  //Default constructor
}

//___________________________________________________________________________
IlcRunTag::~IlcRunTag() {
  //Destructor
  //  fEventTag.Delete();
  if ( fQAArray ) 
    delete [] fQAArray ; 
  if ( fEventSpecies )
    delete [] fEventSpecies ; 
  fFileTags.Delete();
}

//___________________________________________________________________________
IlcRunTag::IlcRunTag(const IlcRunTag& tag):
TObject(),
fIlcRunId(tag.fIlcRunId),
fIlcMagneticField(tag.fIlcMagneticField),
fIlcDipoleField(tag.fIlcDipoleField),
fIlcRunStartTime(tag.fIlcRunStartTime),
fIlcRunStopTime(tag.fIlcRunStopTime),
fIlcrootVersion(tag.fIlcrootVersion),
fRootVersion(tag.fRootVersion),
fGeant3Version(tag.fGeant3Version),
fLHCPeriod(tag.fLHCPeriod),
fRecPass(tag.fRecPass),
fProductionName(tag.fProductionName),
fIlcRunValidated(tag.fIlcRunValidated),
fIlcRunGlobalQuality(tag.fIlcRunGlobalQuality),
fIlcBeamEnergy(tag.fIlcBeamEnergy),
fIlcBeamType(tag.fIlcBeamType),
fIlcCalibrationVersion(tag.fIlcCalibrationVersion),
fIlcDataType(tag.fIlcDataType),
//fNumEvents(tag.fNumEvents),
fNumFiles(0),
fBeamTriggers(tag.fBeamTriggers),
fCollisionTriggers(tag.fCollisionTriggers),
fEmptyTriggers(tag.fEmptyTriggers),
fASideTriggers(tag.fASideTriggers),
fCSideTriggers(tag.fCSideTriggers),
fHMTriggers(tag.fHMTriggers),
fMuonTriggers(tag.fMuonTriggers),
fCollisionRate(tag.fCollisionRate),
fMeanVertex(tag.fMeanVertex),
fVertexQuality(tag.fVertexQuality),
fNumDetectors(tag.fNumDetectors),
fFileTags(100),
fDetectorTag(tag.fDetectorTag),
fLHCTag(tag.fLHCTag), 
fActiveTriggerClasses(tag.fActiveTriggerClasses),
fQA(tag.fQA),
fQALength(tag.fQALength),
fQAArray(NULL), 
fESLength(tag.fESLength),
fEventSpecies(NULL)
{
  //copy constructor
  if (fQALength == 0 ) 
    fQAArray = NULL ; 
  else {
    fQAArray = new ULong_t[fQALength] ; 
    memcpy(fQAArray, tag.fQAArray, fQALength*sizeof(ULong_t)) ;
  }
  if (fESLength == 0 ) 
    fEventSpecies = NULL ; 
  else {
    fEventSpecies = new Bool_t[fESLength] ; 
    memcpy(fEventSpecies, tag.fEventSpecies, fESLength*sizeof(Bool_t)) ;
  }
  for (int ifl=0; ifl<tag.fNumFiles; ifl++) {
    AddFileTag(new IlcFileTag(*tag.GetFileTag(ifl)));
  }
    

}

//___________________________________________________________________________
IlcRunTag& IlcRunTag::operator = (const IlcRunTag& tag) {
//assignment operator
  if(&tag != this) {
    fIlcRunId               = tag.fIlcRunId ; 
    fIlcMagneticField       = tag.fIlcMagneticField ;
    fIlcDipoleField         = tag.fIlcDipoleField ;
    fIlcRunStartTime        = tag.fIlcRunStartTime ; 
    fIlcRunStopTime         = tag.fIlcRunStopTime ; 
    fIlcrootVersion           = tag.fIlcrootVersion ; 
    fRootVersion              = tag.fRootVersion ;
    fGeant3Version            = tag.fGeant3Version ; 
    fLHCPeriod                = tag.fLHCPeriod ; 
    fRecPass                  = tag.fRecPass ; 
    fProductionName           = tag.fProductionName ; 
    fIlcRunValidated        = tag.fIlcRunValidated ; 
    fIlcRunGlobalQuality    = tag.fIlcRunGlobalQuality ; 
    fIlcBeamEnergy          = tag.fIlcBeamEnergy ;
    fIlcBeamType            = tag.fIlcBeamType ; 
    fIlcCalibrationVersion  = tag.fIlcCalibrationVersion ; 
    fIlcDataType            = tag.fIlcDataType ; 
    //    fNumEvents                = tag.fNumEvents ;
    fBeamTriggers             = tag.fBeamTriggers;
    fCollisionTriggers	      = tag.fCollisionTriggers;
    fEmptyTriggers	      = tag.fEmptyTriggers;
    fASideTriggers	      = tag.fASideTriggers;
    fCSideTriggers	      = tag.fCSideTriggers;
    fHMTriggers               = tag.fHMTriggers;
    fMuonTriggers             = tag.fMuonTriggers;
    fCollisionRate	      = tag.fCollisionRate;
    fMeanVertex		      = tag.fMeanVertex;
    fVertexQuality	      = tag.fVertexQuality;
    fNumDetectors             = tag.fNumDetectors ; 
    fDetectorTag              = tag.fDetectorTag ;
    fLHCTag                   = tag.fLHCTag ;  
    fActiveTriggerClasses     = tag.fActiveTriggerClasses;
    fQA                       = tag.fQA ;      
    fQALength                 = tag.fQALength ; 
    if (fQAArray) 
      delete [] fQAArray ; 
    if (fQALength == 0 ) 
      fQAArray = NULL ; 
    else {
      fQAArray = new ULong_t[fQALength] ; 
      memcpy(fQAArray, tag.fQAArray, fQALength*sizeof(ULong_t)) ;
    }
    fESLength                 = tag.fESLength ; 
    if (fEventSpecies)
      delete [] fEventSpecies ; 
    if (fESLength == 0 ) 
      fEventSpecies = NULL ; 
    else {
      fEventSpecies = new Bool_t[fESLength] ; 
      memcpy(fEventSpecies, tag.fEventSpecies, fESLength*sizeof(Bool_t)) ;
    }
    for (int ifl=0; ifl<tag.fNumFiles; ifl++) {
      AddFileTag(new IlcFileTag(*tag.GetFileTag(ifl)));
    }
//     for (int ifile=0; ifile<tag.GetFileTags()->GetEntries(); ifile++)
//       AddFileTag(*((IlcFileTag *) tag.GetFileTags()->At(ifile)));
  }
  return *this ; 
}

//___________________________________________________________________________
Bool_t IlcRunTag::Add(const IlcRunTag* tag) 
{
  // merge two pieces
  if (fIlcRunId==-1) { // empty
    *this = *tag;
    return kTRUE;
  }
  if (fIlcRunId != tag->fIlcRunId) {    
    IlcWarning(Form("Run IDs are different: %d %d",fIlcRunId,tag->fIlcRunId)); 
    return kFALSE;
  }
  // real merging
  fBeamTriggers       += tag->fBeamTriggers;
  fCollisionTriggers  += tag->fCollisionTriggers;
  fEmptyTriggers      += tag->fEmptyTriggers;
  fASideTriggers      += tag->fASideTriggers;
  fCSideTriggers      += tag->fCSideTriggers;
  fHMTriggers         += tag->fHMTriggers;
  fMuonTriggers       += tag->fMuonTriggers;
  IlcFileTag* eftag = 0;
  //
  for (int i=0;i<tag->fNumFiles;i++) {
    IlcFileTag* tftag = (IlcFileTag*)tag->GetFileTag(i);
    if (fNumFiles>0) eftag = (IlcFileTag*)GetFileTag(fNumFiles-1);
    else { 
      eftag = new IlcFileTag(); 
      eftag->SetMD5("");
      eftag->SetTURL("");
      eftag->SetSize(0);
      AddFileTag(eftag);
    }
    int nev = tftag->GetNEvents();
    for (int j=0;j<nev;j++) {
      IlcEventTag* tge = (IlcEventTag*)tftag->GetEventTag(j);
      if (tge) eftag->AddEventTag(*tge);
    }
  }
  
  return kTRUE; 
}

//___________________________________________________________________________
void IlcRunTag::CopyStandardContent(IlcRunTag *oldtag) {
  //function that copies the run, lhc and detector levels
  SetRunId(oldtag->GetRunId());
  SetMagneticField(oldtag->GetMagneticField());
  SetDipoleField(oldtag->GetDipoleField());
  SetRunStartTime(oldtag->GetRunStartTime());
  SetRunStopTime(oldtag->GetRunStopTime());
  SetIlcrootVersion(oldtag->GetIlcrootVersion());
  SetRootVersion(oldtag->GetRootVersion());
  SetGeant3Version(oldtag->GetGeant3Version());
  SetLHCPeriod(oldtag->GetLHCPeriod());
  SetReconstructionPass(oldtag->GetReconstructionPass());
  SetProductionName(oldtag->GetProductionName());
  SetRunValidation(oldtag->GetRunValidation());
  SetRunQuality(oldtag->GetRunQuality());
  SetBeamEnergy(oldtag->GetBeamEnergy());
  SetBeamType(oldtag->GetBeamType());
  SetCalibVersion(oldtag->GetCalibVersion());
  SetDataType(oldtag->GetDataType());
  SetBeamTriggers(oldtag->GetBeamTriggers());
  SetCollisionTriggers(oldtag->GetCollisionTriggers());
  SetEmptyTriggers(oldtag->GetEmptyTriggers());
  SetASideTriggers(oldtag->GetASideTriggers());
  SetCSideTriggers(oldtag->GetCSideTriggers());
  SetHMTriggers(oldtag->GetHMTriggers());
  SetMuonTriggers(oldtag->GetMuonTriggers());
  SetCollisionRate(oldtag->GetCollisionRate());
  SetMeanVertex(oldtag->GetMeanVertex());
  SetVertexQuality(oldtag->GetVertexQuality());
  SetLHCTag(oldtag->GetLHCTag()->GetLuminosity(),oldtag->GetLHCTag()->GetLHCState());
  SetDetectorTag(oldtag->GetDetectorTags()->GetIntDetectorMaskDAQ(), oldtag->GetDetectorTags()->GetIntDetectorMaskReco());
  SetActiveTriggerClasses(oldtag->GetActiveTriggerClasses());
  SetQA(*(oldtag->GetQA())) ;  	
  SetQAArray(oldtag->GetQAArray(), oldtag->GetQALength()) ;  
  SetEventSpecies(oldtag->GetEventSpecies(), oldtag->GetESLength()) ;  
  for (int ifile=0; ifile<oldtag->GetNFiles(); ifile++) {
    IlcFileTag *ntag = new IlcFileTag();
    ntag->CopyFileInfo((const IlcFileTag &) *(oldtag->GetFileTag(ifile)));
    AddFileTag(ntag);
  }
}

void IlcRunTag::UpdateFromRunTable(IlcRunTag *tabtag)
{
  SetBeamTriggers(tabtag->GetBeamTriggers());
  SetCollisionTriggers(tabtag->GetCollisionTriggers());
  SetEmptyTriggers(tabtag->GetEmptyTriggers());
  SetASideTriggers(tabtag->GetASideTriggers());
  SetCSideTriggers(tabtag->GetCSideTriggers());
  SetHMTriggers(tabtag->GetHMTriggers());
  SetMuonTriggers(tabtag->GetMuonTriggers());
  SetCollisionRate(tabtag->GetCollisionRate());
  SetMeanVertex(tabtag->GetMeanVertex());
  SetVertexQuality(tabtag->GetVertexQuality());
  SetRunQuality(tabtag->GetRunQuality());
  fLHCTag.UpdateFromRunTable(*tabtag->GetLHCTag());
  fDetectorTag.UpdateFromRunTable(*tabtag->GetDetectorTags());
}

//___________________________________________________________________________
void IlcRunTag::SetQAArray(ULong_t * qa, Int_t qalength) {
  //Setter for the qa bits 
  if (qa && qalength > 0) {
    fQALength = qalength ; 
    if (fQAArray) 
      delete [] fQAArray ; 
    fQAArray = new ULong_t[qalength] ; 
    memcpy(fQAArray, qa, qalength*sizeof(ULong_t)) ;
  }
}

//___________________________________________________________________________
void IlcRunTag::SetEventSpecies(Bool_t * es, Int_t eslength) {
  //setter for the eventspecices 
  if (es && eslength >0 ) {
    fESLength = eslength ; 
    if (fEventSpecies) 
      delete [] fEventSpecies ; 
    fEventSpecies = new Bool_t[eslength] ;
    memcpy(fEventSpecies, es, eslength*sizeof(Bool_t)) ; 
  }
}


//___________________________________________________________________________
void IlcRunTag::SetLHCTag(Float_t lumin, TString type) {
  //Setter for the LHC tags
  fLHCTag.SetLuminosity(lumin);
  fLHCTag.SetLHCState(type);
}

//___________________________________________________________________________
void IlcRunTag::SetDetectorTag(UInt_t mask, UInt_t maskReco) {
  //Setter for the detector tags
  fDetectorTag.SetDetectorMaskDAQ(mask);
  if (maskReco == 0)
    fDetectorTag.SetDetectorMaskReco(mask);
  else
    fDetectorTag.SetDetectorMaskReco(maskReco);

  int ndet = 0;
  for (int iter=0; iter<32; iter++)  
    ndet += (mask & (1 << iter)) > 0;
  
  fNumDetectors = ndet;
}

//___________________________________________________________________________
void IlcRunTag::AddEventTag(const IlcEventTag & EvTag) {
  //Adds an entry to the event tag TClonesArray
  ((IlcFileTag *) fFileTags[fNumFiles-1])->AddEventTag(EvTag);
  //  new(fEventTag[fNumEvents++]) IlcEventTag(EvTag);
}

void IlcRunTag::AddFileTag(IlcFileTag *t) {
  //Adds an entry for each file tag
  if (fNumFiles == fFileTags.GetSize()-1) fFileTags.Expand(fFileTags.GetSize()*2);
  //  new(fFileTags[fNumFiles++]) IlcFileTag(t);
  fFileTags[fNumFiles++] = t;
}

//___________________________________________________________________________
void IlcRunTag::Clear(const char *) {
  //Resets the number of events and detectors
  //  fEventTag.Delete();
  //  fNumEvents = 0;
  fFileTags.Delete();
  fNumFiles = 0;
  if ( fQAArray ) {
    delete [] fQAArray ;
    fQAArray = 0x0;
  } 
  fQALength=0;
  if ( fEventSpecies ) {
    delete [] fEventSpecies ;
    fEventSpecies = 0x0;
  } 
  fESLength=0;
}

const IlcEventTag* IlcRunTag::GetEventTag(int evt) const
{
  int curev = evt;
  int curf = 0;

  if (evt >= GetNEvents()) return 0;
 
  while (curev >= ((IlcFileTag *) fFileTags[curf])->GetNEvents()) {
    curev -= ((IlcFileTag *) fFileTags[curf])->GetNEvents();
    curf++;
  }
  return ((IlcFileTag *) fFileTags[curf])->GetEventTag(curev);
}

IlcFileTag *IlcRunTag::GetFileTagForEvent(int evt) 
{
  // Returns FileTag in which the given event is
  int curev = evt;
  int curf = 0;

  if (evt >= GetNEvents()) return 0;
 
  while (curev >= ((IlcFileTag *) fFileTags[curf])->GetNEvents()) {
    curev -= ((IlcFileTag *) fFileTags[curf])->GetNEvents();
    curf++;
  }
  return (IlcFileTag *) fFileTags[curf];
}

Int_t       IlcRunTag::GetNEvents() const
{
  Int_t evtot = 0;
  for (int iter=0; iter<fNumFiles; iter++)
    evtot += ((IlcFileTag *) fFileTags[iter])->GetNEvents();

  return evtot;
}

Int_t      IlcRunTag::GetFileId(const char *guid)
{
  for (int iter=0; iter<fNumFiles; iter++) {
    if (!strcmp(((IlcFileTag *) fFileTags[iter])->GetGUID(), guid))
      return iter;
  }
  return -1;
}

