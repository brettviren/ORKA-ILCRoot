// @(#)root/eve:$Id: IlcEveV0Editor.h 30515 2008-12-31 17:03:39Z mtadel $
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

#ifndef IlcEveV0Editor_H
#define IlcEveV0Editor_H

#include "TGedFrame.h"

class TGButton;
class TGCheckButton;
class TGNumberEntry;
class TGColorSelect;

class IlcEveV0;

//______________________________________________________________________________
// Short description of IlcEveV0Editor
//

class IlcEveV0Editor : public TGedFrame
{
public:
  IlcEveV0Editor(const TGWindow* p=0, Int_t width=170, Int_t height=30,
                 UInt_t options=kChildFrame, Pixel_t back=GetDefaultFrameBackground());
  virtual ~IlcEveV0Editor() {}

  virtual void SetModel(TObject* obj);

  // Declare callback/slot methods
  // void DoXYZZ();
  void DisplayDetailed();

protected:
  IlcEveV0  *fM; // Model object.

  TGLabel   *fInfoLabel0; // label
  TGLabel   *fInfoLabel1; // label
  TGLabel   *fInfoLabelNegDaughter; // label
  TGLabel   *fInfoLabelPosDaughter; // label

  TGButton  *fXButton;

private:
  IlcEveV0Editor(const IlcEveV0Editor&);            // Not implemented
  IlcEveV0Editor& operator=(const IlcEveV0Editor&); // Not implemented

  ClassDef(IlcEveV0Editor, 0); // GUI editor for IlcEveV0.
};

#endif
