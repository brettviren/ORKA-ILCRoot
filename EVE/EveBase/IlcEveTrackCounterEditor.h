// @(#)root/eve:$Id: IlcEveTrackCounterEditor.h 58244 2012-08-21 12:57:19Z hristov $
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

#ifndef IlcEveTrackCounterEditor_H
#define IlcEveTrackCounterEditor_H

#include "TGedFrame.h"
#include <fstream>

class TGComboBox;
class TGLabel;
class TGNumberEntry;

class IlcEveTrackCounter;

using std::ofstream;

//______________________________________________________________________________
// Short description of IlcEveTrackCounterEditor
//

class IlcEveTrackCounterEditor : public TGedFrame
{
public:
   IlcEveTrackCounterEditor(const TGWindow* p=0, Int_t width=170, Int_t height=30,
                            UInt_t options = kChildFrame, Pixel_t back=GetDefaultFrameBackground());
   virtual ~IlcEveTrackCounterEditor();

   void UpdateModel();

   virtual void SetModel(TObject* obj);

   void DoActivate();
   void DoDeactivate();

   void DoPrev();
   void DoNext();
   void DoSetEvent();

   void DoPrintReport();
   void DoFileReport();
   void DoShowHistos();

   void DoClickAction(Int_t);
   void DoEventCategorization(Int_t);

protected:
   IlcEveTrackCounter *fM; // Model object.

   TGCompositeFrame *fAF;  // Active frame.
   TGCompositeFrame *fDF;  // Non-active frame.

   TGComboBox       *fClickAction;
   TGLabel          *fInfoLabelTracks;
   TGLabel          *fInfoLabelTracklets;
   TGNumberEntry    *fEventId;

   int               fEventCat;
   ofstream         *fScanSummaryFile;
   
private:
   IlcEveTrackCounterEditor(const IlcEveTrackCounterEditor&);            // Not implemented
   IlcEveTrackCounterEditor& operator=(const IlcEveTrackCounterEditor&); // Not implemented

   ClassDef(IlcEveTrackCounterEditor, 0); // GUI editor for IlcEveTrackCounter.
};

#endif
