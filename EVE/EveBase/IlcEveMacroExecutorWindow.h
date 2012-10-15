// @(#)root/eve:$Id: IlcEveMacroExecutorWindow.h 28011 2008-08-15 15:12:51Z mtadel $
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

#ifndef IlcEveMacroExecutorWindow_H
#define IlcEveMacroExecutorWindow_H

#include "TGFrame.h"

#include <vector>

class IlcEveMacroExecutor;
class IlcEveMacro;
class IlcEveMEWEditor;

class TGLabel;
class TGListBox;
class TGTextEntry;

//______________________________________________________________________________
// Short description of IlcEveMacroExecutorWindow
//

class IlcEveMacroExecutorWindow : public TGMainFrame
{
public:
  IlcEveMacroExecutorWindow(IlcEveMacroExecutor* master);
  virtual ~IlcEveMacroExecutorWindow();

  void PopulateMacros(Bool_t keep_selected=kTRUE);

  void SetActiveStateOfShownMacros(Bool_t active);

  void NewEventLoaded();

  void DoEnableAll()  { SetActiveStateOfShownMacros(kTRUE);  }
  void DoDisableAll() { SetActiveStateOfShownMacros(kFALSE); }
  void DoReloadEvent();
  void DoSelectTags();
  void DoMacroSelected(Int_t mid);

protected:
  IlcEveMacroExecutor *fM;

  TGCompositeFrame *fMainFrame;
  TGCompositeFrame *fCtrlFrame;
  TGListBox        *fListBox;
  IlcEveMEWEditor  *fEditor;

  TGTextEntry      *fSelectTags;

  std::vector<IlcEveMacro*> fBoxContents;

  TGHorizontalFrame* MkHFrame(TGCompositeFrame* p=0);
  TGLabel*           MkLabel (TGCompositeFrame* p, const char* txt, Int_t width,
			      Int_t lo=0, Int_t ro=0, Int_t to=2, Int_t bo=0);

private:
  IlcEveMacroExecutorWindow(const IlcEveMacroExecutorWindow&);            // Not implemented
  IlcEveMacroExecutorWindow& operator=(const IlcEveMacroExecutorWindow&); // Not implemented

  ClassDef(IlcEveMacroExecutorWindow, 0); // Short description.
};

#endif
