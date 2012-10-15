// @(#)root/eve:$Id: IlcEveV0ListEditor.h 30515 2008-12-31 17:03:39Z mtadel $
// Author: Matevz Tadel 2007

/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 * See http://ilcinfo.cern.ch/Offline/IlcRoot/License.html for          *
 * full copyright notice.                                                 *
 **************************************************************************/

#ifndef IlcEveV0ListEditor_H
#define IlcEveV0ListEditor_H

#include "TGedFrame.h"

class TGButton;
class TGCheckButton;
class TGNumberEntry;
class TGColorSelect;
class TEveGDoubleValuator;
class TGComboBox;

class IlcEveV0List;

//______________________________________________________________________________
// Short description of IlcEveV0ListEditor
//

class IlcEveV0ListEditor : public TGedFrame
{
public:
  IlcEveV0ListEditor(const TGWindow* p=0, Int_t width=170, Int_t height=30,
                     UInt_t options=kChildFrame, Pixel_t back=GetDefaultFrameBackground());
  virtual ~IlcEveV0ListEditor() {}

  virtual void SetModel(TObject* obj);

  // Declare callback/slot methods
  void DoMinMaxRCut();
  void DoMinMaxDaughterDCA();
  void DoMinMaxPt();
  void DoSelectNegPid(Int_t rNegPid);
  void DoCheckNegPid();
  void DoSelectNegProb();
  void DoSelectPosPid(Int_t rPosPid);
  void DoCheckPosPid();
  void DoSelectPosProb();
  void DoMinMaxInvariantMass();

protected:
  IlcEveV0List            *fM; // Model object.

  // Declare widgets
  // TGSomeWidget*   fXYZZ;
  TEveGDoubleValuator* fMinMaxRCut;
  TEveGDoubleValuator* fMinMaxDaughterDCA;
  TEveGDoubleValuator* fMinMaxPt;
  TGComboBox*          fNegativeSpecies;
  TGComboBox*          fPositiveSpecies;
  TGCheckButton*       fNegativeCheckMaxPidProbability;
  TGCheckButton*       fPositiveCheckMaxPidProbability;
  TGNumberEntry*       fNegativeLevelPidProbability;
  TGNumberEntry*       fPositiveLevelPidProbability;
  TEveGDoubleValuator* fMinMaxInvariantMass;

private:
  IlcEveV0ListEditor(const IlcEveV0ListEditor&);            // Not implemented
  IlcEveV0ListEditor& operator=(const IlcEveV0ListEditor&); // Not implemented

  ClassDef(IlcEveV0ListEditor, 0); // GUI editor for IlcEveV0List.
};

#endif
