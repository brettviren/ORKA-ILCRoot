// @(#)root/eve:$Id: IlcEveEventManagerEditor.h 37200 2009-11-24 19:42:51Z mtadel $
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

#ifndef IlcEveEventManagerEditor_H
#define IlcEveEventManagerEditor_H

#include "TGedFrame.h"
#include "TGComboBox.h"
#include <TObjString.h>

class IlcEveEventManager;
class TEveGValuator;
class TGButton;
class TGCheckButton;
class TGTextButton;
class TGTextView;
class TGNumberEntry;
class TGLabel;

//==============================================================================
// IlcEveEventManagerEditor
//==============================================================================

//______________________________________________________________________________
// Short description of IlcEveEventManagerEditor
//

class IlcEveEventManagerEditor : public TGedFrame
{
public:
   IlcEveEventManagerEditor(const TGWindow* p=0, Int_t width=170, Int_t height=30,
         UInt_t options=kChildFrame, Pixel_t back=GetDefaultFrameBackground());
   virtual ~IlcEveEventManagerEditor() {}

   virtual void SetModel(TObject* obj);

   void DumpEventInfo();

protected:
   IlcEveEventManager  *fM;             // Model object.

   TGTextButton        *fDumpEventInfo; // Button that dumps event-info into a file.
   TGTextView          *fEventInfo;     // Text box with event info.

private:
   IlcEveEventManagerEditor(const IlcEveEventManagerEditor&);            // Not implemented
   IlcEveEventManagerEditor& operator=(const IlcEveEventManagerEditor&); // Not implemented

   ClassDef(IlcEveEventManagerEditor, 0); // GUI editor for IlcEveEventManager.
};


//==============================================================================
// IlcEveEventManagerWindow
//==============================================================================

//______________________________________________________________________________
// Short description of IlcEveEventManagerWindow
//

class IlcEveEventManagerWindow : public TGMainFrame
{
public:
  IlcEveEventManagerWindow(IlcEveEventManager* mgr);
  virtual ~IlcEveEventManagerWindow();

  void DoFirstEvent();
  void DoPrevEvent();
  void DoNextEvent();
  void DoLastEvent();

  void DoSetEvent();

  void DoRefresh();
  void DoSetAutoLoad();
  void DoSetAutoLoadTime();
  void DoSetTrigSel();

  void Update();

protected:
  IlcEveEventManager   *fM;            // Model object.

  TGTextButton         *fFirstEvent;   // Go to first event
  TGTextButton         *fPrevEvent;    // Go to prev event
  TGTextButton         *fNextEvent;    // Go to next event
  TGTextButton         *fLastEvent;    // Go to last event
  TGTextButton         *fRefresh;      // Refresh event-file state

  TGNumberEntry        *fEventId;      // Display/edit current event id
  TGLabel              *fInfoLabel;    // Display last available event id

  TGCheckButton        *fAutoLoad;     // Check-box for automatic loading of events
  TEveGValuator        *fAutoLoadTime; // Time for automatic loading of events

  TGComboBox           *fTrigSel;      // Trigger selection combo box

  TGTextView           *fEventInfo;    // Text box with event info

  TGTextButton* MkTxtButton(TGCompositeFrame* p, const char* txt, Int_t width=0,
			    Int_t lo=0, Int_t ro=0, Int_t to=0, Int_t bo=0);
  TGLabel* MkLabel(TGCompositeFrame* p, const char* txt, Int_t width,
		   Int_t lo=0, Int_t ro=0, Int_t to=2, Int_t bo=0);

private:
  IlcEveEventManagerWindow(const IlcEveEventManagerWindow&);            // Not implemented
  IlcEveEventManagerWindow& operator=(const IlcEveEventManagerWindow&); // Not implemented

  ClassDef(IlcEveEventManagerWindow, 0); // GUI window for IlcEveEventManager.
};

#endif
