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

/* $Id: IlcDetectorTag.cxx 50615 2011-07-16 23:19:19Z hristov $ */

//-----------------------------------------------------------------
//           Implementation of the DetectorTag class
//   This is the class to deal with the tags in the detector level
//   Origin: Panos Christakoglou, UOA-CERN, Panos.Christakoglou@cern.ch
//-----------------------------------------------------------------

#include "TMath.h"

#include "IlcDetectorTag.h"
#include "IlcLog.h"
#include "TObjString.h"

ClassImp(IlcDetectorTag)

//___________________________________________________________________________
IlcDetectorTag::IlcDetectorTag() :
  TObject(),
  fMaskDAQ(0),
  fMaskReco(0),
  fDetectorValidityRange(),
  fDetectorStatus()
{
  // Default constructor
  for (int iter=0; iter<IlcDAQ::kHLTId; iter++) {
    fDetectorValidityRange[iter] = 0;
    fDetectorStatus[iter] = "";
  }
}

//___________________________________________________________________________
IlcDetectorTag::IlcDetectorTag(const IlcDetectorTag & detTag) :
  TObject(detTag),
  fMaskDAQ(detTag.fMaskDAQ),
  fMaskReco(detTag.fMaskReco),
  fDetectorValidityRange(),
  fDetectorStatus() 
{
  // DetectorTag copy constructor
  
  for (int iter=0; iter<IlcDAQ::kHLTId; iter++) {
    fDetectorValidityRange[iter] = detTag.GetDetectorValidityRange(iter);
    fDetectorStatus[iter] = detTag.GetDetectorStatus(iter);
  }
}

//___________________________________________________________________________
IlcDetectorTag & IlcDetectorTag::operator=(const IlcDetectorTag &detTag) {
  //DetectorTag assignment operator
  if (this != &detTag) {
    TObject::operator=(detTag);
    
    fMaskDAQ = detTag.fMaskDAQ;   
    fMaskReco = detTag.fMaskReco;   
    for (int iter=0; iter<IlcDAQ::kHLTId; iter++) {
      fDetectorValidityRange[iter] = detTag.GetDetectorValidityRange(iter);
      fDetectorStatus[iter] = detTag.GetDetectorStatus(iter);
    }
  }
  return *this;
}

//___________________________________________________________________________
IlcDetectorTag::~IlcDetectorTag() {
  // Destructor
}

void IlcDetectorTag::UpdateFromRunTable(IlcDetectorTag &detTag)
{
  for (int iter=0; iter<IlcDAQ::kHLTId; iter++) {
    fDetectorValidityRange[iter] = detTag.GetDetectorValidityRange(iter);
    fDetectorStatus[iter] = detTag.GetDetectorStatus(iter);
  }
}


//___________________________________________________________________________
// void IlcDetectorTag::SetDetectorConfiguration() {
//   //sets the detector configuration
//   if(fDetectors[0] == 1) {
//     SetITSSPD(); fDetectorArray->Add(new TObjString("SPD"));}
//   if(fDetectors[1] == 1) {
//     SetITSSDD(); fDetectorArray->Add(new TObjString("SDD"));}
//   if(fDetectors[2] == 1) {
//     SetITSSSD(); fDetectorArray->Add(new TObjString("SSD"));}
//   if(fDetectors[3] == 1) {
//     SetTPC(); fDetectorArray->Add(new TObjString("TPC"));}
//   if(fDetectors[4] == 1) {
//     SetTRD(); fDetectorArray->Add(new TObjString("TRD"));}
//   if(fDetectors[5] == 1) {
//     SetTOF(); fDetectorArray->Add(new TObjString("TOF"));}
//   if(fDetectors[6] == 1) {
//     SetHMPID();fDetectorArray->Add(new TObjString("HMPID"));}
//   if(fDetectors[7] == 1) {
//     SetPHOS(); fDetectorArray->Add(new TObjString("PHOS"));}
//   if(fDetectors[9] == 1) {
//     SetPMD(); fDetectorArray->Add(new TObjString("PMD"));}
//   if(fDetectors[10] == 1) {
//     SetMUON(); fDetectorArray->Add(new TObjString("MUON"));}
//   if(fDetectors[12] == 1) {
//     SetFMD(); fDetectorArray->Add(new TObjString("FMD"));}
//   if(fDetectors[13] == 1) {
//     SetTZERO(); fDetectorArray->Add(new TObjString("T0"));}
//   if(fDetectors[14] == 1) {
//     SetVZERO(); fDetectorArray->Add(new TObjString("VZERO"));}
//   if(fDetectors[15] == 1) {
//     SetZDC(); fDetectorArray->Add(new TObjString("ZDC"));}
//   if(fDetectors[18] == 1) {
//     SetEMCAL(); fDetectorArray->Add(new TObjString("EMCAL"));}
// }

//___________________________________________________________________________
void IlcDetectorTag::PrintDetectorMask() {
  //prints the detector mask
  IlcInfo( Form( "ITS-SPD: %d", GetITSSPD()) );
  IlcInfo( Form( "ITS-SDD: %d", GetITSSDD()) );
  IlcInfo( Form( "ITS-SSD: %d", GetITSSSD()) );
  IlcInfo( Form( "TPC: %d", GetTPC()) );
  IlcInfo( Form( "TRD: %d", GetTRD()) );
  IlcInfo( Form( "TOF: %d", GetTOF()) );
  IlcInfo( Form( "HMPID: %d", GetHMPID()) );
  IlcInfo( Form( "PHOS: %d", GetPHOS()) );
  IlcInfo( Form( "PMD: %d", GetPMD()) );
  IlcInfo( Form( "MUON: %d", GetMUON()) );
  IlcInfo( Form( "FMD: %d", GetFMD()) );
  IlcInfo( Form( "TZERO: %d", GetTZERO()) );
  IlcInfo( Form( "VZERO: %d", GetVZERO()) );
  IlcInfo( Form( "ZDC: %d", GetZDC()) );
  IlcInfo( Form( "EMCAL: %d", GetEMCAL()) );
}
