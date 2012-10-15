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

#include <Riostream.h>

#include "IlcLHCClockPhase.h"
#include "IlcDCSValue.h"
#include "IlcLog.h"

ClassImp(IlcLHCClockPhase)

//______________________________________________________________________________
IlcLHCClockPhase::IlcLHCClockPhase():
  TObject(),
  fPhaseB1(),
  fPhaseB2()
{
  // default constructor
  // ...
  fPhaseB1.SetOwner();
  fPhaseB2.SetOwner();
}

//______________________________________________________________________________
void IlcLHCClockPhase::AddPhaseB1DP(UInt_t timestamp, Float_t phase)
{
  // Add a phase beam1 measurement
  // to the array of data-points

  fPhaseB1.AddLast(new IlcDCSValue(phase,timestamp));
}

//______________________________________________________________________________
void IlcLHCClockPhase::AddPhaseB2DP(UInt_t timestamp, Float_t phase)
{
  // Add a phase beam2 measurement
  // to the array of data-points

  fPhaseB2.AddLast(new IlcDCSValue(phase,timestamp));
}

//______________________________________________________________________________
const IlcDCSValue* IlcLHCClockPhase::GetPhaseB1DP(Int_t index) const
{
  // Get the value of the phase
  // The argument is the DP index
  IlcDCSValue *value = (IlcDCSValue*)fPhaseB1.At(index);
  if (!value) {
    IlcFatal(Form("Invalid index of the beam1 data point: %d (0 -> %d)",
		  index,fPhaseB1.GetEntries()));
    return NULL;
  }
  return value;
}

//______________________________________________________________________________
const IlcDCSValue* IlcLHCClockPhase::GetPhaseB2DP(Int_t index) const
{
  // Get the value of the phase
  // The argument is the DP index
  IlcDCSValue *value = (IlcDCSValue*)fPhaseB2.At(index);
  if (!value) {
    IlcFatal(Form("Invalid index of the beam2 data point: %d (0 -> %d)",
		  index,fPhaseB2.GetEntries()));
    return NULL;
  }
  return value;
}

//______________________________________________________________________________
Float_t IlcLHCClockPhase::GetMeanPhaseB1() const
{
  // Get mean beam1 phase shift.
  // The mean is calculated using all the data-points.
  Int_t n = 0;
  Float_t phase = 0;
  for(Int_t i = 0; i < fPhaseB1.GetEntries(); ++i) {
    IlcDCSValue *value = (IlcDCSValue*)fPhaseB1.At(i);
    if (value) {
      phase += value->GetFloat();
      ++n;
    }
  }
  if (n == 0) {
    IlcError("No beam1 measurements found! Assuming 0 phase shift!");
    return 0;
  }
  phase /= n;
  return phase;
}

//______________________________________________________________________________
Float_t IlcLHCClockPhase::GetMeanPhaseB2() const
{
  // Get mean beam2 phase shift.
  // The mean is calculated using all the data-points.
  Int_t n = 0;
  Float_t phase = 0;
  for(Int_t i = 0; i < fPhaseB2.GetEntries(); ++i) {
    IlcDCSValue *value = (IlcDCSValue*)fPhaseB2.At(i);
    if (value) {
      phase += value->GetFloat();
      ++n;
    }
  }
  if (n == 0) {
    IlcError("No beam2 measurements found! Assuming 0 phase shift!");
    return 0;
  }
  phase /= n;
  return phase;
}

//______________________________________________________________________________
Float_t IlcLHCClockPhase::GetMeanPhase() const
{
  // Get mean phase shift.
  // Beam1 and beam2 phases are calculated using
  // all beam1 and beam2 data-points. The two phases are
  // then averaged in order to get the mean value.
  return (GetMeanPhaseB1() + GetMeanPhaseB2())/2;
}

//______________________________________________________________________________
Float_t IlcLHCClockPhase::GetPhaseB1(UInt_t timestamp) const
{
  // Get beam1 phase shift using the
  // event time-stamp as an argument.
  // This methods loops over data-points and
  // returns the value of the closest (in time)
  // measurement found.
  Long64_t deltat = 0xffffffff;
  Float_t phase = 0;
  for(Int_t i = 0; i < fPhaseB1.GetEntries(); ++i) {
    IlcDCSValue *value = (IlcDCSValue*)fPhaseB1.At(i);
    if (value) {
      if (TMath::Abs((Long64_t)timestamp-(Long64_t)value->GetTimeStamp()) <= deltat) {
	phase = value->GetFloat();
	deltat = TMath::Abs((Long64_t)timestamp-(Long64_t)value->GetTimeStamp());
      }
    }
  }
  if (deltat == 0xffffffff) {
    IlcError(Form("Can't get the beam1 phase shift at time-stamp = %u",timestamp));
    return 0;
  }
  return phase;
}

//______________________________________________________________________________
Float_t IlcLHCClockPhase::GetPhaseB2(UInt_t timestamp) const
{
  // Get beam2 phase shift using the
  // event time-stamp as an argument.
  // This methods loops over data-points and
  // returns the value of the closest (in time)
  // measurement found.
  Long64_t deltat = 0xffffffff;
  Float_t phase = 0;
  for(Int_t i = 0; i < fPhaseB2.GetEntries(); ++i) {
    IlcDCSValue *value = (IlcDCSValue*)fPhaseB2.At(i);
    if (value) {
      if (TMath::Abs((Long64_t)timestamp-(Long64_t)value->GetTimeStamp()) <= deltat) {
	phase = value->GetFloat();
	deltat = TMath::Abs((Long64_t)timestamp-(Long64_t)value->GetTimeStamp());
      }
    }
  }
  if (deltat == 0xffffffff) {
    IlcError(Form("Can't get the beam2 phase shift at time-stamp = %u",timestamp));
    return 0;
  }
  return phase;
}

//______________________________________________________________________________
Float_t IlcLHCClockPhase::GetPhase(UInt_t timestamp) const
{
  // Get mean phase shift using the event time-stamp as
  // an argument.
  // Beam1 and beam2 phases are calculated using
  // the closest beam1 and beam2 data-points. The two phases are
  // then averaged in order to get the mean value.
  return (GetPhaseB1(timestamp) + GetPhaseB2(timestamp))/2;
}

//______________________________________________________________________________
void IlcLHCClockPhase::Print( const Option_t* ) const
{
  // Print all the data-points for beam1 and beam2
  // as well as the mean phases
  cout << "IlcLHCClockPhase object:" << endl;

  cout << "Beam1:" << endl;
  for(Int_t i = 0; i < fPhaseB1.GetEntries(); ++i) {
    IlcDCSValue *value = (IlcDCSValue*)fPhaseB1.At(i);
    if (value) cout << "TS=" << value->GetTimeStamp() << "  " << value->GetFloat() << endl;
  }
  cout << "Beam1 mean phase=" << GetMeanPhaseB1() << " ns" << endl;

  cout << "Beam2:" << endl;
  for(Int_t i = 0; i < fPhaseB2.GetEntries(); ++i) {
    IlcDCSValue *value = (IlcDCSValue*)fPhaseB2.At(i);
    if (value) cout << "TS=" << value->GetTimeStamp() << "  " << value->GetFloat() << endl;
  }
  cout << "Beam2 mean phase=" << GetMeanPhaseB2() << " ns" << endl;

  cout << "Mean phase (beam1 & beam2) =" << GetMeanPhase() << " ns" << endl;
}
