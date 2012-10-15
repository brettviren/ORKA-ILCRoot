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


///////////////////////////////////////////////////////////////////////////
//          ----   CORRECTION FRAMEWORK   ----
// Class to cut on the number of IlcTrackReference's 
// for each detector. Applies on pair of tracks (IlcCFPair)
///////////////////////////////////////////////////////////////////////////
// author : R. Vernet (renaud.vernet@cern.ch)
///////////////////////////////////////////////////////////////////////////


#ifndef ILCCFPAIRACCEPTANCECUTS_H
#define ILCCFPAIRACCEPTANCECUTS_H

#include "IlcCFAcceptanceCuts.h"
#include "IlcCFCutBase.h"

class IlcMCEvent;
class TBits;

class IlcCFPairAcceptanceCuts : public IlcCFCutBase
{
 public :
  IlcCFPairAcceptanceCuts() ;
  IlcCFPairAcceptanceCuts(const Char_t* name, const Char_t* title) ;
  IlcCFPairAcceptanceCuts(const IlcCFPairAcceptanceCuts& c) ;
  IlcCFPairAcceptanceCuts& operator=(const IlcCFPairAcceptanceCuts& c) ;
  virtual ~IlcCFPairAcceptanceCuts() {delete fCutNeg; delete fCutPos; }
  Bool_t IsSelected(TObject* obj) ;
  Bool_t IsSelected(TList* /*list*/) {return kTRUE;}
  virtual void SetMCEventInfo(const TObject *mcInfo) ;
  virtual IlcCFAcceptanceCuts* GetNegCut() const {return fCutNeg;}
  virtual IlcCFAcceptanceCuts* GetPosCut() const {return fCutPos;}

  enum {
    kNCuts=2
  };

 protected:
  IlcMCEvent          *fMCInfo ; // global event information
  IlcCFAcceptanceCuts *fCutNeg ; // acceptance cut on negative daughter
  IlcCFAcceptanceCuts *fCutPos ; // acceptance cut on positive daughter
  TBits               *fBitmap ; // cut bitmap    

 private:
  void SelectionBitMap(TObject* obj);

  ClassDef(IlcCFPairAcceptanceCuts,1);
};

#endif
