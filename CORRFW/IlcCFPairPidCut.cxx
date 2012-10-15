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

////////////////////////////////////////////////
// Class to define PID cuts on a pair of tracks
// The track pair object to use is IlcCFPair
// author : renaud.vernet@cern.ch
////////////////////////////////////////////////

#include "IlcCFPairPidCut.h"
#include "IlcCFPair.h"

ClassImp(IlcCFPairPidCut)

//__________________________________________________________________________________
IlcCFPairPidCut::IlcCFPairPidCut() :
  IlcCFCutBase(),
  fCutNeg(new IlcCFTrackCutPid()),
  fCutPos(new IlcCFTrackCutPid())
{
  //
  // Default constructor
  //
}
//__________________________________________________________________________________
IlcCFPairPidCut::IlcCFPairPidCut(const Char_t* name, const Char_t* title) :
  IlcCFCutBase(name,title),
  fCutNeg(new IlcCFTrackCutPid(name,title)),
  fCutPos(new IlcCFTrackCutPid(name,title))
{
  //
  // Constructor
  //
}
//__________________________________________________________________________________
IlcCFPairPidCut::IlcCFPairPidCut(const IlcCFPairPidCut& c) :
  IlcCFCutBase(c),
  fCutNeg(c.fCutNeg),
  fCutPos(c.fCutPos)
{
  //
  // copy constructor
  //
}
//__________________________________________________________________________________
IlcCFPairPidCut& IlcCFPairPidCut::operator=(const IlcCFPairPidCut& c)
{
  //
  // Assignment operator
  //
  if (this != &c) {
    IlcCFCutBase::operator=(c) ;
    fCutNeg = c.fCutNeg ;
    fCutPos = c.fCutPos ;
  }
  return *this;
}

//__________________________________________________________________________________
Bool_t IlcCFPairPidCut::IsSelected(TObject* obj) {
  //
  // loops over decisions of single cuts and returns if the track is accepted
  //

  IlcCFPair* pair = dynamic_cast<IlcCFPair*>(obj);
  if (!pair) return kFALSE ;
  TString className(pair->ClassName());
  if (className.CompareTo("IlcCFPair") != 0) {
    Error("IsSelected","obj must point to a IlcCFPair !");
    return kFALSE ;
  }

  IlcVParticle* tneg = pair->GetNeg();
  IlcVParticle* tpos = pair->GetPos();

  if (!tneg || !tpos) return kFALSE ;
  if ( ! fCutNeg->IsSelected(tneg) || ! fCutPos->IsSelected(tpos) ) return kFALSE ;
  return kTRUE ;
}
