// $Id: IlcEveBeamsInfoEditor.h 43215 2010-08-30 12:48:52Z mtadel $
// Author: Stefano Carrazza 2010, CERN, stefano.carrazza@cern.ch

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

#ifndef ILCEVEBEAMSINFOEDITOR_H
#define ILCEVEBEAMSINFOEDITOR_H

#include "TGedFrame.h"

class TGButton;
class TGColorSelect;
class TGComboBox;
class TGCheckButton;
class TGGroupFrame;
class TGNumberEntry;
class TGTextButton;

class IlcEveBeamsInfo;

//______________________________________________________________________________
// Short description of IlcEveBeamsInfoEditor
//

class IlcEveBeamsInfoEditor : public TGedFrame
{
public:
   IlcEveBeamsInfoEditor(const TGWindow* p=0, Int_t width=170, Int_t height=30,
         UInt_t options=kChildFrame, Pixel_t back=GetDefaultFrameBackground());
   virtual ~IlcEveBeamsInfoEditor() {}

   virtual void SetModel(TObject* obj);

   // Set Methods
   void SetAlpha();

   // Slot methods
   void SelectEventSelection(Int_t id);
   void ShowEventSelection();
   void ShowPrevEvent();
   void ShowNextEvent();
   void SwitchDataType();

protected:
   IlcEveBeamsInfo *fM;              // Model object.

private:
   TGCheckButton   *fIsMC;           // activating mc selection
   TGGroupFrame    *fEventSelection; // event selection group box
   TGCheckButton   *fShowEvents;     // display information checkbox
   TGComboBox      *fSelect;         // combo box display
   TGTextButton    *fButtonPrev;     // previous event selection
   TGTextButton    *fButtonNext;     // next event selection
   TGGroupFrame    *fSetAlpha;       // set alpha for overlay buttons
   TGNumberEntry   *fAlpha;          // alpha value

   IlcEveBeamsInfoEditor(const IlcEveBeamsInfoEditor&);            // Not implemented
   IlcEveBeamsInfoEditor& operator=(const IlcEveBeamsInfoEditor&); // Not implemented

   ClassDef(IlcEveBeamsInfoEditor, 0); // GUI editor for IlcEveBeamsInfo.
};

#endif
