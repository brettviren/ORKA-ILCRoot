// $Id: IlcEveKinkListEditor.h 31777 2009-04-01 17:44:10Z mtadel $
// Author: Paraskevi Ganoti: 2009

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

#ifndef IlcEveKinkListEditor_H
#define IlcEveKinkListEditor_H

#include "TGedFrame.h"

class TGButton;
class TGCheckButton;
class TGNumberEntry;
class TGColorSelect;
class TEveGDoubleValuator;
class TGComboBox;

class IlcEveKinkList;

//______________________________________________________________________________
// Short description of IlcEveKinkListEditor
//

class IlcEveKinkListEditor : public TGedFrame
{
public:
  IlcEveKinkListEditor(const TGWindow* p=0, Int_t width=170, Int_t height=30,
                     UInt_t options=kChildFrame, Pixel_t back=GetDefaultFrameBackground());
  virtual ~IlcEveKinkListEditor() {}

  virtual void SetModel(TObject* obj);

  // Declare callback/slot methods
  void DoMinMaxRCut();
  void DoMinMaxKinkAngleCut();
  void DoMinMaxPt();
  void DoMinMaxInvariantMass();
  void DoSelectDaugPid(Int_t rDaugPid);
  void DoCheckDaugPid();
  void DoSelectDaugProb();

protected:
  IlcEveKinkList            *fM; // Model object.

  // Declare widgets
  // TGSomeWidget*   fXYZZ;
  TEveGDoubleValuator* fMinMaxRCut;
  TEveGDoubleValuator* fMinMaxKinkAngleCut; 
  TEveGDoubleValuator* fMinMaxPt;
  TEveGDoubleValuator* fMinMaxInvariantMass;
  TGComboBox*          fDaughterSpecies; 
  TGCheckButton*       fDaughterCheckMaxPidProbability;
  TGNumberEntry*       fDaughterLevelPidProbability;  

private:
  IlcEveKinkListEditor(const IlcEveKinkListEditor&);            // Not implemented
  IlcEveKinkListEditor& operator=(const IlcEveKinkListEditor&); // Not implemented

  ClassDef(IlcEveKinkListEditor, 0); // GUI editor for IlcEveKinkList.
};

#endif
