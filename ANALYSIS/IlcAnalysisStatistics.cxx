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

/* $Id: IlcAnalysisStatistics.cxx 58023 2012-07-31 16:29:45Z hristov $ */
// Author: Andrei Gheata, 20/12/2010

//==============================================================================
// IlcAnalysisStatistics - basic class for storing statistics for the processed
//   events. The object is mergeable and can be used for general purpose. In case
//   a IlcAnalysisTaskStat is used, this will set the global statistics object
//   to the analysis manager and will update it for the accepted events.
//==============================================================================

#include "IlcAnalysisStatistics.h"

#include "Riostream.h"
#include "TCollection.h"

#include "IlcVEvent.h"

using std::cout;
using std::endl;
ClassImp(IlcAnalysisStatistics)

//______________________________________________________________________________
IlcAnalysisStatistics::IlcAnalysisStatistics(const IlcAnalysisStatistics &other)
      :TNamed(other),
       fNinput(other.fNinput),
       fNprocessed(other.fNprocessed),
       fNfailed(other.fNfailed),
       fNaccepted(other.fNaccepted),
       fOfflineMask(other.fOfflineMask)
{
// Copy constructor.
}

//______________________________________________________________________________
IlcAnalysisStatistics &IlcAnalysisStatistics::operator=(const IlcAnalysisStatistics &other)
{
// Assignment.
  if (&other == this) return *this;
  fNinput     = other.fNinput;
  fNprocessed = other.fNprocessed;
  fNfailed    = other.fNfailed;
  fNaccepted  = other.fNaccepted;
  fOfflineMask = other.fOfflineMask;
  return *this;
}

//______________________________________________________________________________
Long64_t IlcAnalysisStatistics::Merge(TCollection* list)
{
// Merge statistics objets from list on top of this.
  TIter next(list);
  IlcAnalysisStatistics *current;
  Long64_t count = 1;
  while ((current = (IlcAnalysisStatistics*)next())) {
    fNinput     += current->GetNinput();
    fNprocessed += current->GetNprocessed();
    fNfailed    += current->GetNfailed();
    fNaccepted  += current->GetNaccepted();
    current++;
  }
  return count;
}

//______________________________________________________________________________
void IlcAnalysisStatistics::Print(const Option_t *) const
{
// Print info about the processed statistics.
  cout << "### Input events                 : " << fNinput << endl;
  cout << "### Processed events w/o errors  : " << fNprocessed << endl;
  cout << "### Failed events                : " << fNfailed << endl;
  cout << "### Accepted events for mask: " << GetMaskAsString(fOfflineMask) << ": " << fNaccepted << endl;
}

//______________________________________________________________________________
const char *IlcAnalysisStatistics::GetMaskAsString(UInt_t mask)
{
// Returns a string corresponding to the offline mask.
   static TString smask;
   smask = "ALL EVT.";
   if (!mask) return smask.Data();
   smask.Clear();
   if (mask & IlcVEvent::kMB)   smask = "MB";
   if (mask & IlcVEvent::kMUON) {
      if (!smask.IsNull()) smask += " | ";
      smask += "MUON";
   }
   if (mask & IlcVEvent::kHighMult) {
      if (!smask.IsNull()) smask += " | ";
      smask += "HighMult";
   }
   if (mask & IlcVEvent::kUserDefined) {
      if (!smask.IsNull()) smask += " | ";
      smask += "UserDefined";
   }
   if (mask ==  IlcVEvent::kAny) smask = "ANY";
   return smask.Data();
}
   
