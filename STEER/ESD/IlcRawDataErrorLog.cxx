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

/////////////////////////////////////////////////////////////////////
//   Implementation of IlcRawDataErrorLog class                    //
//                                                                 //
// class IlcRawDataErrorLog                                        //
// This is a class for logging raw-data related errors.            //
// It is used to record and retrieve of the errors                 //
// during the reading and reconstruction of raw-data and ESD       //
// analysis.                                                       //
// Further description of the methods and functionality are given  //
// inline.                                                         //
//                                                                 //
// cvetan.cheshkov@cern.ch                                         //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#include "IlcRawDataErrorLog.h"

#include <Riostream.h>

ClassImp(IlcRawDataErrorLog)

//_____________________________________________________________________________
IlcRawDataErrorLog::IlcRawDataErrorLog() :
  TNamed(),
  fEventNumber(-1),
  fDdlID(-1),
  fErrorLevel(IlcRawDataErrorLog::kMinor),
  fErrorCode(0),
  fCount(0)
{
  // Default constructor
}

//_____________________________________________________________________________
IlcRawDataErrorLog::IlcRawDataErrorLog(Int_t eventNumber, Int_t ddlId,
				       ERawDataErrorLevel errorLevel,
				       Int_t errorCode,
				       const char *message) :
  TNamed(message,""),
  fEventNumber(eventNumber),
  fDdlID(ddlId),
  fErrorLevel(errorLevel),
  fErrorCode(errorCode),
  fCount(1)
{
  // Constructor that specifies
  // the event number, ddl id, error type and
  // custom message related to the error
}

//___________________________________________________________________________
IlcRawDataErrorLog::IlcRawDataErrorLog(const IlcRawDataErrorLog & source) :
  TNamed(source),
  fEventNumber(source.fEventNumber),
  fDdlID(source.fDdlID),
  fErrorLevel(source.fErrorLevel),
  fErrorCode(source.fErrorCode),
  fCount(source.fCount)
{
  // Copy constructor
}

//___________________________________________________________________________
IlcRawDataErrorLog & IlcRawDataErrorLog::operator=(const IlcRawDataErrorLog &source)
{
  // assignment operator
  if (this != &source) {
    TNamed::operator=(source);

    fEventNumber = source.GetEventNumber();
    fDdlID       = source.GetDdlID();
    fErrorLevel  = source.GetErrorLevel();
    fErrorCode   = source.GetErrorCode();
    fCount       = source.GetCount();
  }
  return *this;
}

void IlcRawDataErrorLog::Copy(TObject &obj) const {
  
  // this overwrites the virtual TOBject::Copy()
  // to allow run time copying without casting
  // in IlcESDEvent

  if(this==&obj)return;
  IlcRawDataErrorLog *robj = dynamic_cast<IlcRawDataErrorLog*>(&obj);
  if(!robj)return; // not an IlcRawDataErrorLog
  *robj = *this;

}

//_____________________________________________________________________________
Int_t IlcRawDataErrorLog::Compare(const TObject *obj) const
{
  // Compare the event numbers and DDL IDs
  // of two error log objects.
  // Used in the sorting of raw data error logs
  // during the raw data reading and reconstruction
  Int_t eventNumber = ((IlcRawDataErrorLog*)obj)->GetEventNumber();
  Int_t ddlID = ((IlcRawDataErrorLog*)obj)->GetDdlID();

  if (fEventNumber == eventNumber) {
    if (fDdlID == ddlID)
      return 0;
    else
      return ((fDdlID > ddlID) ? 1 : -1);
  }
  else
    return ((fEventNumber > eventNumber) ? 1 : -1);
}

//_____________________________________________________________________________
const char*
IlcRawDataErrorLog::GetErrorLevelAsString() const
{
  switch ( GetErrorLevel() )
  {
    case kMinor:
      return "MINOR";
      break;
    case kMajor:
      return "MAJOR";
      break;
    case kFatal:
      return "FATAL";
      break;
    default:
      return "INVALID";
      break;
  }
  
}

//_____________________________________________________________________________
void
IlcRawDataErrorLog::Print(Option_t*) const
{
  cout << Form("EventNumber %10d DdlID %5d ErrorLevel %10s ErrorCode %4d Occurence %5d",
               GetEventNumber(),GetDdlID(),
               GetErrorLevelAsString(),
               GetErrorCode(),
               GetCount()) << endl;
  cout << "    " << GetMessage() << endl;
}
