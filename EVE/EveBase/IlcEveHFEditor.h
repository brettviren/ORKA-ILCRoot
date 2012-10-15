// @(#)root/eve:$Id: IlcEveHFEditor.h 32622 2009-05-29 17:55:58Z mtadel $
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

#ifndef IlcEveHFEditor_H
#define IlcEveHFEditor_H

#include "TGedFrame.h"

class TGButton;
class TGCheckButton;
class TGNumberEntry;
class TGColorSelect;

class IlcEveHF;

//______________________________________________________________________________
// Short description of IlcEveHFEditor
//

class IlcEveHFEditor : public TGedFrame
{
public:
  IlcEveHFEditor(const TGWindow* p=0, Int_t width=170, Int_t height=30,
                 UInt_t options=kChildFrame, Pixel_t back=GetDefaultFrameBackground());
  virtual ~IlcEveHFEditor() {}

  virtual void SetModel(TObject* obj);

  // Declare callback/slot methods
  // void DoXYZZ();
  void DisplayDetailed();

protected:
  IlcEveHF  *fM; // Model object.

  Int_t     fnProng;

  TGLabel   *fInfoLabel0; // label
  TGLabel   *fInfoLabel1; // label
  TGLabel   *fInfoLabel2; // label

  TGButton  *fXButton;

private:
  IlcEveHFEditor(const IlcEveHFEditor&);            // Not implemented
  IlcEveHFEditor& operator=(const IlcEveHFEditor&); // Not implemented

  ClassDef(IlcEveHFEditor, 0); // GUI editor for IlcEveHF.
};

#endif
