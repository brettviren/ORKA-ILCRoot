// @(#)root/eve:$Id: IlcEveCascadeEditor.h 32027 2009-04-22 14:38:08Z mtadel $
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
 
//-----------------------------------------------------------------------------
// This code defines the Editor coming with the visualisation of cascades,
// within IlcEVE
//
// Origin :   Boris Hippolyte, IPHC (hippolyt@in2p3.fr)
// Modified : Antonin Maire, April 2009, IPHC (antonin.maire@cern.ch)
//-----------------------------------------------------------------------------
 

#ifndef ILCEVECASCADEEDITOR_H
#define ILCEVECASCADEEDITOR_H

#include "TGedFrame.h"

class TGButton;
class TGLabel;
//class TGCheckButton;
//class TGNumberEntry;
//class TGColorSelect;

class IlcEveCascade;

//______________________________________________________________________________
// Short description of IlcEveCascadeEditor
//

class IlcEveCascadeEditor : public TGedFrame
{
public:
  IlcEveCascadeEditor(const TGWindow* p=0, Int_t width=170, Int_t height=30,
                 UInt_t options=kChildFrame, Pixel_t back=GetDefaultFrameBackground());
  virtual ~IlcEveCascadeEditor() {}

  virtual void SetModel(TObject* obj);

  // Declare callback/slot methods
  // void DoXYZZ();
  void DisplayDetailed();
  void DisplayMassHyp();

protected:
  IlcEveCascade  *fM; //! Model object.

  TGLabel   *fInfoLabelRadius; //! label about transverse radius decay for the cascade
  TGLabel   *fInfoLabelDCA;    //! label about the DCA between Xi daughters
  TGLabel   *fInfoLabelCharge; //! label about the charge of the cascade
  TGLabel   *fInfoLabelPhi;    //! label about phi
  TGLabel   *fInfoLabelTheta;  //! label about theta
  TGLabel   *fInfoLabelPtot;   //! label about total momentum, Ptot
  TGLabel   *fInfoLabelPt;     //! label about transverse momentum, Pt
  TGLabel   *fInfoLabelEta;    //! label about pseudo-rapidity
  
  TGButton  *fXButtonDetailedView;  //! button to get the detailed view 
  TGButton  *fXButtonMassHyp;       //! button to printf the calculation of eff inv mass, under Xi and Omega hypotheses

private:
  IlcEveCascadeEditor(const IlcEveCascadeEditor&);            // Not implemented
  IlcEveCascadeEditor& operator=(const IlcEveCascadeEditor&); // Not implemented

  ClassDef(IlcEveCascadeEditor, 0); // GUI editor for IlcEveCascade.
};

#endif
