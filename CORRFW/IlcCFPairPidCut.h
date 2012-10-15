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
//
// author : renaud.vernet@cern.ch
////////////////////////////////////////////////

#ifndef ILCCFPAIRPIDCUT_H
#define ILCCFPAIRPIDCUT_H

#include "IlcCFCutBase.h"
#include "IlcCFTrackCutPid.h"

//__________________________________________________________________________________
// CUT ON TRACK PID FOR V0 DAUGHTERS
//__________________________________________________________________________________

class IlcCFPairPidCut : public IlcCFCutBase
{
  public :
  IlcCFPairPidCut() ;
  IlcCFPairPidCut(const Char_t* name, const Char_t* title) ;
  IlcCFPairPidCut(const IlcCFPairPidCut& c) ;
  IlcCFPairPidCut& operator=(const IlcCFPairPidCut& c) ;
  virtual ~IlcCFPairPidCut() {delete fCutNeg; delete fCutPos; };

  virtual IlcCFTrackCutPid* GetNegCut() const {return fCutNeg;}
  virtual IlcCFTrackCutPid* GetPosCut() const {return fCutPos;}

  Bool_t IsSelected(TObject *obj); //boolean for detectors
  Bool_t IsSelected(TList* /*list*/) {return kTRUE;}
 private:
  IlcCFTrackCutPid* fCutNeg; // PID cut on negative daughter
  IlcCFTrackCutPid* fCutPos; // PID cut on positive daughter

  ClassDef(IlcCFPairPidCut,1);
};
#endif
    
