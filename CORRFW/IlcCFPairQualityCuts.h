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



#ifndef ILCCFPAIRQUILCTYCUTS_H
#define ILCCFPAIRQUILCTYCUTS_H

#include "IlcCFCutBase.h"
#include "IlcCFTrackQualityCuts.h"

class IlcESDEvent;

class IlcCFPairQualityCuts : public IlcCFCutBase
{
 public :
  IlcCFPairQualityCuts() ;
  IlcCFPairQualityCuts(Char_t* name, Char_t* title) ;
  IlcCFPairQualityCuts(const IlcCFPairQualityCuts& c) ;
  IlcCFPairQualityCuts& operator=(const IlcCFPairQualityCuts& c) ;
  virtual ~IlcCFPairQualityCuts() {delete fCutNeg; delete fCutPos; }

  Bool_t IsSelected(TObject* obj) ; 
  Bool_t IsSelected(TList* /*list*/) {return kTRUE;}

  virtual IlcCFTrackQualityCuts* GetNegCut() const {return fCutNeg;}
  virtual IlcCFTrackQualityCuts* GetPosCut() const {return fCutPos;}

 private :
  IlcCFTrackQualityCuts *fCutNeg ; // quality cut on negative daughter
  IlcCFTrackQualityCuts *fCutPos ; // quality cut on positive daughter

  ClassDef(IlcCFPairQualityCuts,2);
};

#endif
