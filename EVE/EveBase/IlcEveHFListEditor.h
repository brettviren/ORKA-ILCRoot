// @(#)root/eve:$Id: IlcEveHFListEditor.h 32622 2009-05-29 17:55:58Z mtadel $
// Main author: Davide Caffarri 2009

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

#ifndef IlcEveHFListEditor_H
#define IlcEveHFListEditor_H

#include "TGedFrame.h"

class TGButton;
class TGCheckButton;
class TGNumberEntry;
class TGColorSelect;
class TEveGValuator;
class TEveGDoubleValuator;
class TGComboBox;

class IlcEveHFList;

//______________________________________________________________________________
// Short description of IlcEveHFListEditor
//

class IlcEveHFListEditor : public TGedFrame
{
public:
  IlcEveHFListEditor(const TGWindow* p=0, Int_t width=170, Int_t height=30,
                     UInt_t options=kChildFrame, Pixel_t back=GetDefaultFrameBackground());
  virtual ~IlcEveHFListEditor() {}

  virtual void SetModel(TObject* obj);

  // Declare callback/slot methods
  void DoMinMaxPt();
  void DoMinMaxCosPointingAngle();
  void DoMinMaxInvMass();


protected:
  IlcEveHFList            *fM; // Model object.

  // Declare widgets

  TEveGDoubleValuator* fMinMaxPt;
  TEveGDoubleValuator* fMinMaxCosPointingAngle;
  TEveGDoubleValuator* fValueInvMass;

private:
  IlcEveHFListEditor(const IlcEveHFListEditor&);            // Not implemented
  IlcEveHFListEditor& operator=(const IlcEveHFListEditor&); // Not implemented

  ClassDef(IlcEveHFListEditor, 0); // GUI editor for IlcEveV0List.
};

#endif
