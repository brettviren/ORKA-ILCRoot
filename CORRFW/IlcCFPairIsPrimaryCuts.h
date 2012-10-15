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

///////////////////////////////////////////////////////
// Class to handle primariness criteria in track pairs
// The track pair object to use is IlcCFPair
// author : renaud.vernet@cern.ch
///////////////////////////////////////////////////////

#ifndef ILCCFPAIRISPRIMARYCUTS_H
#define ILCCFPAIRISPRIMARYCUTS_H

#include "IlcCFCutBase.h"
#include "IlcCFTrackIsPrimaryCuts.h"

class IlcESDEvent;

class IlcCFPairIsPrimaryCuts : public IlcCFCutBase
{
 public :
  IlcCFPairIsPrimaryCuts() ;
  IlcCFPairIsPrimaryCuts(Char_t* name, Char_t* title) ;
  IlcCFPairIsPrimaryCuts(const IlcCFPairIsPrimaryCuts& c) ;
  IlcCFPairIsPrimaryCuts& operator=(const IlcCFPairIsPrimaryCuts& c) ;
  virtual ~IlcCFPairIsPrimaryCuts() {delete fCutNeg; delete fCutPos; }

  Bool_t IsSelected(TObject* obj) ;
  Bool_t IsSelected(TList* /*list*/) {return kTRUE;}
  virtual IlcCFTrackIsPrimaryCuts* GetNegCut() const {return fCutNeg;}
  virtual IlcCFTrackIsPrimaryCuts* GetPosCut() const {return fCutPos;}

  ClassDef(IlcCFPairIsPrimaryCuts,2);

 private :
  IlcCFTrackIsPrimaryCuts *fCutNeg ; // isprimary cut on negative daughter
  IlcCFTrackIsPrimaryCuts *fCutPos ; // isprimary cut on positive daughter

};

#endif
