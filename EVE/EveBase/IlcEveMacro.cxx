// @(#)root/eve:$Id: IlcEveMacro.cxx 30345 2008-12-10 17:04:51Z mtadel $
// Author: Matevz Tadel 2007

/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 * See http://ilcinfo.cern.ch/Offline/IlcRoot/License.html for          *
 * full copyright notice.                                                 *
 **************************************************************************/

#include "IlcEveMacro.h"

//______________________________________________________________________________
//
// Member fSources is a bitfield, but we do not have a widget
// that can show/edit this (a combo-box with a check-box for each
// entry). So ... use a single value for now,

ClassImp(IlcEveMacro)

//______________________________________________________________________________
IlcEveMacro::IlcEveMacro(Int_t src, const TString& tags, const TString& mac,
			 const TString& foo, const TString& args, Bool_t act) :
  TObject(),
  fSources(src), fTags(tags), fMacro (mac),
  fFunc   (foo), fArgs(args), fActive(act),
  fExecStatus(kNotRun), fExecExcString(), fExecResult(0)
{
  // Constructor.
}

/******************************************************************************/

void IlcEveMacro::ResetExecState()
{
  // Reset exec variables into state as if the macro has not been run.

  fExecStatus      = kNotRun;
  fExecExcString = "";
  fExecResult      = 0;
}

void IlcEveMacro::SetExecNoData()
{
  // Set last execution state to 'NoData'.

  fExecStatus = kNoData;
}


void IlcEveMacro::SetExecOK(TEveElement* result)
{
  // Set last execution state to 'OK' and register result.

  fExecStatus = kOK;
  fExecResult = result;
}


void IlcEveMacro::SetExecException(const TString& exception)
{
  // Set last execution state to 'Exception' and store the exception string.

  fExecStatus    = kException;
  fExecExcString = exception;
}

void IlcEveMacro::SetExecError()
{
  // Set last execution state to 'Error'.

  fExecStatus = kError;
}

/******************************************************************************/

TString IlcEveMacro::FormForExec() const
{
  // Return string suitable for execution.

  return fFunc + "(" + fArgs + ");";
}

TString IlcEveMacro::FormForDisplay() const
{
  // Return string suitable for display.

  return TString::Format
    (" %c %-22s  %-30s  %-30s  %-s", fActive ? 'x' : ' ',
     fMacro.Data(), fFunc.Data(), fArgs.Data(), fTags.Data());
}
