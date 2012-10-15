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
// Class to cut on V0 topology
//   -> support for :
//                    DCA between V0 daughters
//                    V0 daughter impact parameters wrt primary vertex
//                    cosine of V0 pointing angle
//
///////////////////////////////////////////////////////////////////////////
// author : R. Vernet (renaud.vernet@cern.ch)
///////////////////////////////////////////////////////////////////////////

#ifndef ILCCFV0TOPOCUTS_H
#define ILCCFV0TOPOCUTS_H

#include "IlcCFCutBase.h"

class TObject;
class IlcVEvent;

class IlcCFV0TopoCuts : public IlcCFCutBase 
{
 public :
  IlcCFV0TopoCuts () ;
  IlcCFV0TopoCuts (const Char_t* name, const Char_t* title) ;
  IlcCFV0TopoCuts (const IlcCFV0TopoCuts& c) ;
  IlcCFV0TopoCuts& operator=(const IlcCFV0TopoCuts& c) ;
  virtual ~IlcCFV0TopoCuts() { } ;
  Bool_t IsSelected(TObject* v0) ;
  Bool_t IsSelected(TList* /*list*/) {return kTRUE;}
  void   SetRecEventInfo(const TObject* evt) {fEvent = (IlcVEvent*)evt;}
  void   SetMaxDcaDaughters (Double32_t dca)  {fMaxDcaDaughters = dca;}
  void   SetMinDcaNeg       (Double32_t dca)  {fMinDcaNeg = dca;}
  void   SetMinDcaPos       (Double32_t dca)  {fMinDcaPos = dca;}
  void   SetMinCosPointAngle(Double32_t cos)  {fMinCosP   = cos;}
  
 private :
  Double32_t   fMaxDcaDaughters ; // max. dca between V0 daughters
  Double32_t   fMinDcaNeg ;       // min impact parameter (aka dca to prim. vertex) of neg. daughter
  Double32_t   fMinDcaPos ;       // min impact parameter (aka dca to prim. vertex) of pos. daughter
  Double32_t   fMinCosP ;         // min cosine of pointing angle
  IlcVEvent*   fEvent;            // pointer to current event (needed for cuts related to PV position)
  
  ClassDef(IlcCFV0TopoCuts,0);
};

#endif
