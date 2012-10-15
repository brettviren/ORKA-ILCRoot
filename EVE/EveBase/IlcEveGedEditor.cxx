// $Id: IlcEveGedEditor.cxx 41294 2010-05-17 18:26:59Z mtadel $
// Author: Matevz Tadel 2009

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

#include "IlcEveGedEditor.h"
#include <TGButton.h>


//==============================================================================
// IlcEveGedNameFrame
//==============================================================================

//______________________________________________________________________________
// Full description of IlcEveGedNameFrame
//

ClassImp(IlcEveGedNameFrame)

//______________________________________________________________________________
IlcEveGedNameFrame::IlcEveGedNameFrame(const TGWindow *p) :
  TGedFrame(p),
  fB(0)
{
  // Constructor.

  fB = new TGTextButton(this);
  AddFrame(fB, new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));
}

//______________________________________________________________________________
void IlcEveGedNameFrame::SetModel(TObject* obj)
{
  // Set model object.

  if (obj)
    fB->SetText(Form("%s [%s]", obj->GetName(), obj->ClassName()));
  else
    fB->SetText("No object selected");
}


//==============================================================================
// IlcEveGedEditor
//==============================================================================

//______________________________________________________________________________
// Full description of IlcEveGedEditor
//

ClassImp(IlcEveGedEditor)

//______________________________________________________________________________
IlcEveGedEditor::IlcEveGedEditor() :
  TEveGedEditor()
{
  // Constructor.

  // Remove old name-frame -- it is created in TGedEditor constructor
  // so virtuals are not active yet.
  fTabContainer->RemoveAll();

  // Replace with a new one.
  TGedFrame* nf = CreateNameFrame(fTabContainer, "Style");
  nf->SetGedEditor(this);
  nf->SetModelClass(0);
  fTabContainer->AddFrame(nf, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
}

//______________________________________________________________________________
TGedFrame* IlcEveGedEditor::CreateNameFrame(const TGWindow* parent, const char* /*tab_name*/)
{
  // Create name-frame for a tab.

  return new IlcEveGedNameFrame(parent);
}
