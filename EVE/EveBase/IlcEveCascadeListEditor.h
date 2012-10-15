// @(#)root/eve:$Id: IlcEveCascadeListEditor.h 32027 2009-04-22 14:38:08Z mtadel $
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

#ifndef ILCEVECASCADELISTEDITOR_H
#define ILCEVECASCADELISTEDITOR_H

//------------------------------------------------------------------------------
// This code defines the List Editor coming with the visualisation of cascades,
// within IlcEVE
//
// Origin :   Boris Hippolyte, IPHC (hippolyt@in2p3.fr)
// Modified : Antonin Maire, April 2009, IPHC (antonin.maire@cern.ch)
//------------------------------------------------------------------------------


//class TGButton;
//class TGCheckButton;
//class TGNumberEntry;
//class TGColorSelect;
class TEveGDoubleValuator;
class TGComboBox;

class IlcEveCascadeList;

#include "TGedFrame.h"

//______________________________________________________________________________
// Short description of IlcEveCascadeListEditor
//

class IlcEveCascadeListEditor : public TGedFrame
{
public:
  IlcEveCascadeListEditor(const TGWindow* p=0, Int_t width=170, Int_t height=30,
                     UInt_t options=kChildFrame, Pixel_t back=GetDefaultFrameBackground());
  virtual ~IlcEveCascadeListEditor() {}

  virtual void SetModel(TObject* obj);

  // Declare callback/slot methods
  void DoMinMaxRCut();
  void DoMinMaxDaughterDCA();
  void DoMinMaxPt();
  
  void DoSelectInvMassHyp(Int_t rInvMassHyp);
  void DoMinMaxInvariantMass();

protected:
  IlcEveCascadeList    *fM; //! Model object.

  // Declare widgets
  // TGSomeWidget*   fXYZZ;
  TGComboBox*          fCascadeSpecies;		//! Box meant to choose the mass hyp. to be applied : Xi or Omega ?
  
  TEveGDoubleValuator* fMinMaxRCut;		//! Transverse radius range targeted by the user
  TEveGDoubleValuator* fMinMaxDaughterDCA;	//! DCA (between Xi daughters) range targeted by the user
  TEveGDoubleValuator* fMinMaxPt;		//! Pt range targeted by the user
  TEveGDoubleValuator* fMinMaxInvariantMass;	//! Inv Mass range targeted by the user

private:
  IlcEveCascadeListEditor(const IlcEveCascadeListEditor&);            // Not implemented
  IlcEveCascadeListEditor& operator=(const IlcEveCascadeListEditor&); // Not implemented

  ClassDef(IlcEveCascadeListEditor, 1); // GUI editor for IlcEveCascadeList.
};

#endif
