#ifndef ILCCFCUTBASE_H
#define ILCCFCUTBASE_H
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
// Base class for selecton classes for the correction framework 
// Inherits from IlcAnalysisCuts. It includes additional methods to handle QA 
// histograms and if needed, study the cut statistics & correlations 
// Author S.Arcelli
// silvia.Arcelli@cern.ch

#include <IlcAnalysisCuts.h>
class TBits;
class TList;
//___________________________________________________________________________
class IlcCFCutBase : public IlcAnalysisCuts
{
 public:
  IlcCFCutBase(); //default ctor
  IlcCFCutBase(const char* name, const char* title); //ctor
  IlcCFCutBase(const IlcCFCutBase& obj); //copy ctor  
  virtual ~IlcCFCutBase() {;} //dtor
  virtual Bool_t IsQAOn() const {return fIsQAOn;}; //QA flag getter
  virtual void SetQAOn(TList* list) {fIsQAOn=kTRUE; AddQAHistograms(list);} //QA flag setter
  virtual void  SetMCEventInfo(const TObject *) {} //Pass pointer to MC event
  virtual void SetRecEventInfo(const TObject *) {} //Pass pointer to reconstructed event
  
 protected:
  Bool_t fIsQAOn;//qa checking on/off
  virtual void AddQAHistograms(TList*) {;}; //QA Histos

  ClassDef(IlcCFCutBase, 1); // Base class for Correction Framework Cuts
};
 
#endif
