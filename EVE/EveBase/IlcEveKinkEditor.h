// $Id: IlcEveKinkEditor.h 31777 2009-04-01 17:44:10Z mtadel $
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

#ifndef IlcEveKinkEditor_H
#define IlcEveKinkEditor_H

#include "TGedFrame.h"

class TGButton;
class TGCheckButton;
class TGNumberEntry;
class TGColorSelect;

class IlcEveKink;

//______________________________________________________________________________
// Short description of IlcEveKinkEditor
//

class IlcEveKinkEditor : public TGedFrame
{
public:
  IlcEveKinkEditor(const TGWindow* p=0, Int_t width=170, Int_t height=30,
                 UInt_t options=kChildFrame, Pixel_t back=GetDefaultFrameBackground());
  virtual ~IlcEveKinkEditor() {}

  virtual void SetModel(TObject* obj);
  void DisplayDetailed();

protected:
  IlcEveKink  *fM; // Model object.

  TGLabel   *fInfoLabel0; // label
  TGLabel   *fInfoLabel1; // label
  TGLabel   *fInfoLabelDaughter; // label  
  
  TGButton  *fXButton;

private:
  IlcEveKinkEditor(const IlcEveKinkEditor&);            // Not implemented
  IlcEveKinkEditor& operator=(const IlcEveKinkEditor&); // Not implemented

  ClassDef(IlcEveKinkEditor, 0); // GUI editor for IlcEveKink.
};

#endif
