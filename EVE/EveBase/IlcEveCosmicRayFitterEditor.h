// $Id: IlcEveCosmicRayFitterEditor.h 24810 2008-03-31 16:33:43Z mtadel $
// Main authors: Matevz Tadel & Alja Mrak-Tadel: 2006, 2007

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

#ifndef ILCEVE_CosmicRayFitterEditor_H
#define ILCEVE_CosmicRayFitterEditor_H

#include <TGedFrame.h>

class TGCheckButton;
class TGNumberEntry;
class TGColorSelect;

class IlcEveCosmicRayFitter;

class IlcEveCosmicRayFitterEditor : public TGedFrame
{
public:
  IlcEveCosmicRayFitterEditor(const TGWindow* p=0, Int_t width=170, Int_t height=30, UInt_t options = kChildFrame, Pixel_t back=GetDefaultFrameBackground());
  virtual ~IlcEveCosmicRayFitterEditor() {}

  virtual void SetModel(TObject* obj);

  void DoStart();
  void DoFit();
  void DoReset();
  void DoStop();
  void DoGraph();

protected:
  IlcEveCosmicRayFitter* fM; // fModel dynamic-casted to IlcEveCosmicRayFitterEditor

  TGTextButton* fFit;    // button to fit selection
  TGTextButton* fReset;  // button to reset selection
  TGTextButton* fStart;  // button to connect to signal
  TGTextButton* fStop;   // button to disconnect from signal
  TGTextButton* fGraph;  // button to draw graph

private:
  IlcEveCosmicRayFitterEditor(const IlcEveCosmicRayFitterEditor&);            // Not implemented
  IlcEveCosmicRayFitterEditor& operator=(const IlcEveCosmicRayFitterEditor&); // Not implemented

  ClassDef(IlcEveCosmicRayFitterEditor, 0); // Editor for IlcEveCosmicRayFitter class.
};

#endif
