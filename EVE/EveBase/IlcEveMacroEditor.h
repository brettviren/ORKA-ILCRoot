// @(#)root/eve:$Id: IlcEveMacroEditor.h 27973 2008-08-14 14:25:16Z mtadel $
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

#ifndef IlcEveMacroEditor_H
#define IlcEveMacroEditor_H

#include "TGedFrame.h"

class IlcEveMacro;

class TGCheckButton;
class TGTextEntry;
class TGComboBox;

//______________________________________________________________________________
// Short description of IlcEveMacroEditor
//

class IlcEveMacroEditor : public TGedFrame
{
public:
  IlcEveMacroEditor(const TGWindow* p=0, Int_t width=170, Int_t height=30,
		    UInt_t options=kChildFrame, Pixel_t back=GetDefaultFrameBackground());
  virtual ~IlcEveMacroEditor() {}

  virtual void SetModel(TObject* obj);

  void DoSources(Int_t v);
  void DoTags();
  void DoMacro();
  void DoFunc();
  void DoArgs();
  void DoActive();

protected:
  IlcEveMacro           *fM; // Model object.

  TGComboBox            *fSources;
  TGTextEntry           *fTags;
  TGTextEntry           *fMacro;
  TGTextEntry           *fFunc;
  TGTextEntry           *fArgs;
  TGCheckButton         *fActive;

  TGHorizontalFrame* MkHFrame(TGCompositeFrame* p=0);
  TGLabel*           MkLabel (TGCompositeFrame* p, const char* txt, Int_t width,
			      Int_t lo=0, Int_t ro=0, Int_t to=2, Int_t bo=0);

private:
  IlcEveMacroEditor(const IlcEveMacroEditor&);            // Not implemented
  IlcEveMacroEditor& operator=(const IlcEveMacroEditor&); // Not implemented

  ClassDef(IlcEveMacroEditor, 0); // GUI editor for IlcEveMacro.
};

#endif
