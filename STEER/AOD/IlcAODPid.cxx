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

/* $Id: IlcAODPid.cxx 58259 2012-08-22 07:07:20Z hristov $ */

//-------------------------------------------------------------------------
//     AOD Pid class to store additional pid information
//     Author: Annalisa Mastroserio
//-------------------------------------------------------------------------

#include "IlcAODPid.h"
#include "IlcESDtrack.h"
#include "IlcLog.h"
#include "IlcPID.h"
#include "IlcTPCdEdxInfo.h"

ClassImp(IlcAODPid)


//______________________________________________________________________________
IlcAODPid::IlcAODPid():
    fITSsignal(0), 
    fVXDsignal(0),
    fTPCsignal(0),
    fTPCsignalN(0),
    fTPCmomentum(0),
    fTRDnSlices(0),
    fTRDntls(0),
    fTRDslices(0x0),
    fTRDChi2(0x0),
    fTOFesdsignal(0),
    fTPCdEdxInfo(0)
{
  // default constructor
    for(Int_t i=0; i<kSPECIES; i++) fIntTime[i]   = 0; 
    for(Int_t i=0; i<5; i++) fTOFpidResolution[i] = 0.;
    for(Int_t i=0; i<6; i++) {
      fTRDmomentum[i]      = 0.;
      fTRDncls[i]          = 0;
    }
    for(Int_t i=0; i<4; i++) fITSdEdxSamples[i]   = 0.;
    for(Int_t i=0; i<4; i++) fVXDdEdxSamples[i]   = 0.;
}

//______________________________________________________________________________
IlcAODPid::~IlcAODPid() 
{
  delete [] fTRDslices;
  fTRDslices = 0;
  delete fTPCdEdxInfo;
  // destructor
}


//______________________________________________________________________________
IlcAODPid::IlcAODPid(const IlcAODPid& pid) : 
  TObject(pid),
  fITSsignal(pid.fITSsignal), 
  fVXDsignal(pid.fVXDsignal),
  fTPCsignal(pid.fTPCsignal),
  fTPCsignalN(pid.fTPCsignalN),
  fTPCmomentum(pid.fTPCmomentum),
  fTRDnSlices(pid.fTRDnSlices),
  fTRDntls(pid.fTRDntls),
  fTRDslices(0x0),
  fTRDChi2(pid.fTRDChi2),
  fTOFesdsignal(pid.fTOFesdsignal),
  fTPCdEdxInfo(0x0)
{
  // Copy constructor
  SetTRDsignal(fTRDnSlices, pid.fTRDslices);
    for(Int_t i=0; i<kSPECIES; i++) fIntTime[i]=pid.fIntTime[i];

    for(Int_t i=0; i<6; i++){ 
      fTRDmomentum[i]=pid.fTRDmomentum[i];
      fTRDncls[i] = 0;
    }

    for(Int_t i=0; i<5; i++) fTOFpidResolution[i]=pid.fTOFpidResolution[i];

    for(Int_t i=0; i<4; i++) fITSdEdxSamples[i]=pid.fITSdEdxSamples[i];

    for(Int_t i=0; i<4; i++) fVXDdEdxSamples[i]=pid.fVXDdEdxSamples[i];

    if (pid.fTPCdEdxInfo) fTPCdEdxInfo=new IlcTPCdEdxInfo(*pid.fTPCdEdxInfo);
}

//______________________________________________________________________________
IlcAODPid& IlcAODPid::operator=(const IlcAODPid& pid)
{
  // Assignment operator
  if(this!=&pid) {
    // copy stuff
    TObject::operator=(pid);

    fITSsignal   = pid.fITSsignal; 
    for (Int_t i = 0; i < 4; i++) fITSdEdxSamples[i]=pid.fITSdEdxSamples[i];
    fVXDsignal   = pid.fVXDsignal;
    for (Int_t i = 0; i < 4; i++) fVXDdEdxSamples[i]=pid.fVXDdEdxSamples[i];
    fTPCsignal   = pid.fTPCsignal;
    fTPCsignalN  = pid.fTPCsignalN;
    fTPCmomentum = pid.fTPCmomentum;


    if(fTRDnSlices != pid.fTRDnSlices) {
      // only delete if number changed or is 0
      delete [] fTRDslices;
      fTRDslices = 0;
      fTRDnSlices = pid.fTRDnSlices;
      if(pid.fTRDnSlices > 0) fTRDslices = new Double32_t[fTRDnSlices];
    }

    if (fTRDslices && pid.fTRDslices)
      memcpy(fTRDslices, pid.fTRDslices, fTRDnSlices*sizeof(Double32_t));

    fTRDntls = pid.fTRDntls;
    for(Int_t i = 0; i < 6; i++){ 
	fTRDmomentum[i] = pid.fTRDmomentum[i];
	fTRDncls[i]     = pid.fTRDncls[i];
    }

    fTRDChi2 = pid.fTRDChi2;

    fTOFesdsignal=pid.fTOFesdsignal;
    for (Int_t i = 0; i < 5; i++) fTOFpidResolution[i]=pid.fTOFpidResolution[i];
    for (Int_t i = 0; i < 5; i++) fIntTime[i]=pid.fIntTime[i];
    
     SetTPCdEdxInfo(pid.fTPCdEdxInfo);
  }

  return *this;
}
//_______________________________________________________________________________
void IlcAODPid::GetIntegratedTimes(Double_t timeint[kSPECIES]) const
{
 // Returns the array with integrated times for each particle hypothesis
for(Int_t i=0; i<kSPECIES; i++) timeint[i]=fIntTime[i];
}
//_______________________________________________________________________________
void IlcAODPid::SetIntegratedTimes(Double_t timeint[kSPECIES])
{
 // Returns the array with integrated times for each particle hypothesis
for(Int_t i=0; i<kSPECIES; i++) fIntTime[i]=timeint[i];
}
//______________________________________________________________________________
void IlcAODPid::SetTOFpidResolution(Double_t tofPIDres[5])
{
  for (Int_t i=0; i<5; i++) fTOFpidResolution[i]=tofPIDres[i];

}
//______________________________________________________________________________
void IlcAODPid::GetTOFpidResolution(Double_t tofRes[5]) const
{
  for (Int_t i=0; i<5; i++) tofRes[i]=fTOFpidResolution[i];
}

//______________________________________________________________________________
void IlcAODPid::SetITSdEdxSamples(const Double_t s[4])
{
  //
  // Set the 4 values of dE/dx from individual ITS layers that are read from ESD
  //  
  for (Int_t i=0; i<4; i++) fITSdEdxSamples[i]=s[i];
}
//______________________________________________________________________________
void IlcAODPid::GetITSdEdxSamples(Double_t s[4]) const
{
  //
  // Get the 4 values of dE/dx from individual ITS layers that are read from ESD
  //  
  for (Int_t i=0; i<4; i++) s[i]=fITSdEdxSamples[i];
}
//______________________________________________________________________________
void IlcAODPid::SetVXDdEdxSamples(const Double_t s[4])
{
  //
  // Set the 4 values of dE/dx from individual VXD layers that are read from ESD
  //
  for (Int_t i=0; i<4; i++) fVXDdEdxSamples[i]=s[i];
}
//______________________________________________________________________________
void IlcAODPid::GetVXDdEdxSamples(Double_t s[4]) const
{
  //
  // Get the 4 values of dE/dx from individual VXD layers that are read from ESD
  //
  for (Int_t i=0; i<4; i++) s[i]=fVXDdEdxSamples[i];
}
//______________________________________________________________________________
void IlcAODPid::SetTPCdEdxInfo(IlcTPCdEdxInfo * dEdxInfo)
{
  //
  // Set TPC dEdx info
  //
  if (dEdxInfo==0x0){
    delete fTPCdEdxInfo;
    fTPCdEdxInfo=0x0;
    return;
  }
  if (!fTPCdEdxInfo) fTPCdEdxInfo=new IlcTPCdEdxInfo;
  (*fTPCdEdxInfo)=(*dEdxInfo);
}

