/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ALICE Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/////////////////////////////////////////////////////////////////////////
// ILC SHUTTER FRAME CLASS                                           //
// Author: Mayeul   ROUSSELET                                          //
// e-mail: Mayeul.Rousselet@cern.ch                                    //
// Last update:26/08/2003                                              //
/////////////////////////////////////////////////////////////////////////

#include <TGFrame.h>
#include <TGLayout.h>
#include <TGShutter.h>

#include "IlcDetectorFrame.h"
#include "IlcDisplay2.h"
#include "IlcShutterItem.h"

#include "IlcShutterFrame.h"


ClassImp(IlcShutterFrame)

//_____________________________________________________________
IlcShutterFrame::IlcShutterFrame(TGCompositeFrame *p, UInt_t /*w*/, UInt_t h)
{
  // Constructor
  fShutter = new TGShutter(p,kSunkenFrame);
  fLayout = new TGLayoutHints(kLHintsExpandY | kLHintsTop | kLHintsLeft);
  fMainFrame = (TGCompositeFrame *) fShutter;
  
  //Event Shutter
  IlcShutterItem *item = new IlcShutterItem(fShutter,"Event",kIdsEVENT);
  
  item->AddPictureButton("next.xpm","Show next event",kIdbNextEVENT);
  item->AddPictureButton("prev.xpm","Show previous event",kIdbPrevEVENT);
  
  //View Shutter
  item = new IlcShutterItem(fShutter,"View",kIdsVIEW);
  item->AddPictureButton("top.xpm","Top view",kIdbTOPVIEW);
  item->AddPictureButton("side.xpm","Side view",kIdbSIDEVIEW);
  item->AddPictureButton("front.xpm","Front view",kIdbFRONTVIEW);
  item->AddPictureButton("four.xpm","Four views",kIdbALLVIEW);
  
  //Detector Shutter
  item = new IlcShutterItem(fShutter,"Detectors",kIdsDETECTORS);
  TGCompositeFrame *frame = item->GetShutterItemFrame();
  fDetectorFrameLayout = new TGLayoutHints( kLHintsTop | kLHintsLeft| kLHintsExpandX | kLHintsCenterX,5,5,5,5);
  fDetectorFrame = new IlcDetectorFrame(frame,200,200,item->GetShutterItem()->GetDefaultFrameBackground());
  frame->AddFrame(fDetectorFrame->GetDetectorFrame(),fDetectorFrameLayout);
  
  //Options Shutter
  item = new IlcShutterItem(fShutter,"Options",kIdsOPTIONS);
  item->AddCheckButton("Display Hits",kIdbCheckHITS);
  item->AddCheckButton("Display Clusters",kIdbCheckCLUSTERS);
  item->AddCheckButton("Display HLT Clusters",kIdbCheckHLT);
  //	item->AddCheckButton("Display Tracks",kIdbCheckTRACKS);
  
  fMainFrame->Resize(150,h);
}

//_____________________________________________________________
IlcShutterFrame::~IlcShutterFrame(void)
{
  // Destructor
  delete fLayout;
  delete fShutter;
  delete fMainFrame;
  delete fDetectorFrame;
  delete fDetectorFrameLayout;
}

