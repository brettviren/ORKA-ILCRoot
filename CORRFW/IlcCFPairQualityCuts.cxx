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


///////////////////////////////////////////////
// Class to handle track quality in track pairs
// The track pair object to use is IlcCFPair
// author : renaud.vernet@cern.ch
///////////////////////////////////////////////

#include "IlcCFPairQualityCuts.h"
#include "IlcCFPair.h"

ClassImp(IlcCFPairQualityCuts)

//__________________________________________________________________________________
IlcCFPairQualityCuts::IlcCFPairQualityCuts() :
  IlcCFCutBase(),
  fCutNeg(new IlcCFTrackQualityCuts()),
  fCutPos(new IlcCFTrackQualityCuts())
{
  //
  // Default constructor
  //
}
//__________________________________________________________________________________
IlcCFPairQualityCuts::IlcCFPairQualityCuts(Char_t* name, Char_t* title) :
  IlcCFCutBase(name,title),
  fCutNeg(new IlcCFTrackQualityCuts(name,title)),
  fCutPos(new IlcCFTrackQualityCuts(name,title))
{
  //
  // Named Constructor
  //
}
//__________________________________________________________________________________
IlcCFPairQualityCuts::IlcCFPairQualityCuts(const IlcCFPairQualityCuts& c) :
  IlcCFCutBase(c),
  fCutNeg(c.fCutNeg),
  fCutPos(c.fCutPos)
{
  //
  // copy constructor
  //
}
//__________________________________________________________________________________
IlcCFPairQualityCuts& IlcCFPairQualityCuts::operator=(const IlcCFPairQualityCuts& c)
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
Bool_t IlcCFPairQualityCuts::IsSelected(TObject* obj) {
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

  if ( ! fCutNeg->IsSelected((TObject*)tneg) || ! fCutPos->IsSelected((TObject*)tpos) ) return kFALSE ;

  return kTRUE ;
}
