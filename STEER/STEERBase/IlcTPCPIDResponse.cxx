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

//-----------------------------------------------------------------
//           Implementation of the TPC PID class
// Very naive one... Should be made better by the detector experts...
//      Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch
// With many additions and modifications suggested by
//      Alexander Kalweit, GSI, alexander.philipp.kalweit@cern.ch
//      Dariusz Miskowiec, GSI, D.Miskowiec@gsi.de
//-----------------------------------------------------------------

#include <TGraph.h>
#include <TObjArray.h>
#include <TSpline.h>

#include "IlcExternalTrackParam.h"

#include "IlcTPCPIDResponse.h"

ClassImp(IlcTPCPIDResponse)

//_________________________________________________________________________
IlcTPCPIDResponse::IlcTPCPIDResponse():
  fMIP(50.),
  fRes0(0.07),
  fResN2(0.),
  fKp1(0.0283086),
  fKp2(2.63394e+01),
  fKp3(5.04114e-11),
  fKp4(2.12543),
  fKp5(4.88663),
  fUseDatabase(kFALSE),
  fResponseFunctions(IlcPID::kUnknown+1)
{
  //
  //  The default constructor
  //
}

//_________________________________________________________________________
IlcTPCPIDResponse::IlcTPCPIDResponse(const Double_t *param):
  fMIP(param[0]),
  fRes0(param[1]),
  fResN2(param[2]),
  fKp1(0.0283086),
  fKp2(2.63394e+01),
  fKp3(5.04114e-11),
  fKp4(2.12543),
  fKp5(4.88663),
  fUseDatabase(kFALSE),
  fResponseFunctions(IlcPID::kUnknown+1)
{
  //
  //  The main constructor
  //
}

//_________________________________________________________________________
Double_t IlcTPCPIDResponse::Bethe(Double_t betaGamma) const {
  //
  // This is the Bethe-Bloch function normalised to 1 at the minimum
  // WARNING
  // Simulated and reconstructed Bethe-Bloch differs
  //           Simulated  curve is the dNprim/dx
  //           Reconstructed is proportianal dNtot/dx
  // Temporary fix for production -  Simple linear correction function
  // Future    2 Bethe Bloch formulas needed
  //           1. for simulation
  //           2. for reconstructed PID
  //
  
//   const Float_t kmeanCorrection =0.1;
  Double_t bb=
    IlcExternalTrackParam::BetheBlochAleph(betaGamma,fKp1,fKp2,fKp3,fKp4,fKp5);
  return bb*fMIP;
}

//_________________________________________________________________________
void IlcTPCPIDResponse::SetBetheBlochParameters(Double_t kp1,
                             Double_t kp2,
                             Double_t kp3,
                             Double_t kp4,
                             Double_t kp5) {
  //
  // Set the parameters of the ALEPH Bethe-Bloch formula
  //
  fKp1=kp1;
  fKp2=kp2;
  fKp3=kp3;
  fKp4=kp4;
  fKp5=kp5;
}
//_________________________________________________________________________
void IlcTPCPIDResponse::SetSigma(Float_t res0, Float_t resN2) {
  //
  // Set the relative resolution  sigma_rel = res0 * sqrt(1+resN2/npoint)
  //
  fRes0 = res0;
  fResN2 = resN2;
}

//_________________________________________________________________________
Double_t IlcTPCPIDResponse::GetExpectedSignal(const Float_t mom,
					      IlcPID::EParticleType n) const {
  //
  // Calculates the expected PID signal as the function of 
  // the information stored in the track, for the specified particle type 
  //  
  // At the moment, these signals are just the results of calling the 
  // Bethe-Bloch formula. 
  // This can be improved. By taking into account the number of
  // assigned clusters and/or the track dip angle, for example.  
  //
  
  Double_t mass=IlcPID::ParticleMass(n);
  if (!fUseDatabase||fResponseFunctions.GetEntriesFast()>IlcPID::kUnknown) return Bethe(mom/mass);
  //
  TSpline3 * responseFunction = (TSpline3 *) fResponseFunctions.UncheckedAt(n);
  if (!responseFunction) return Bethe(mom/mass);
  return fMIP*responseFunction->Eval(mom/mass);

}

//_________________________________________________________________________
Double_t IlcTPCPIDResponse::GetExpectedSigma(const Float_t mom, 
					     const Int_t nPoints,
                                         IlcPID::EParticleType n) const {
  //
  // Calculates the expected sigma of the PID signal as the function of 
  // the information stored in the track, for the specified particle type 
  //  
  //
  
  if (nPoints != 0) 
    return GetExpectedSignal(mom,n)*fRes0*sqrt(1. + fResN2/nPoints);
  else
    return GetExpectedSignal(mom,n)*fRes0;
}
