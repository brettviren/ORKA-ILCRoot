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
// ILC EVENT DISPLAY CLASS                                           //
// Author: Mayeul   ROUSSELET                                          //
// e-mail: Mayeul.Rousselet@cern.ch                                    //
// Last update:26/08/2003                                              //
/////////////////////////////////////////////////////////////////////////

#define do_mc

//standard modules
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//ROOT

#include <TTree.h>
#include <TGLayout.h>
#include <TObjArray.h>
#include <TSystem.h>
#include <TGWindow.h>
#include <TEnv.h>
#include <TPad.h>
#include <TTree.h>

//IlcRoot Module
#include "IlcModule.h"
#include "IlcDetector.h"
#include "IlcLoader.h"
#include "IlcRun.h"
#include "IlcMC.h"

#include "IlcModuleInfo.h"
#include "IlcSliderFrame.h"
#include "IlcShutterFrame.h"
#include "IlcDisplayFrame.h"
#include "IlcInfoFrame.h"
#include "IlcDetectorFrame.h"
#include "IlcMenu.h"

#include "IlcPoints.h"
#include "IlcHit.h"

#include "IlcDisplay2.h"


 IlcDisplay2 *gIlcDisplay2;


ClassImp(IlcDisplay2)
//_____________________________________________________________
IlcDisplay2::IlcDisplay2(const TGWindow *p, UInt_t w, UInt_t h)
			:TObject()
{
  //  Constructor
  //gIlc->SetDisplay(this);
  gIlcDisplay2=this;
  fSliderUpdate = kFALSE;
  fZoomMode = kFALSE;
  fZoomStep = 1.2;
  fZoomFactor = 5.5;
  fNbParticles = 0;
  fEventNumber = 0;
  fNbHits = 0;
  fSliderStep = 0.01;
  fClustersLoaded = kFALSE;
  fHitsLoaded = kFALSE;
  fHLTLoaded = kFALSE;
  fTracksLoaded = kFALSE;
  fMode =0;
  fPoints = 0;

  FindModules();
  
  fIconsPath = new char[256];
  strcpy(fIconsPath,gSystem->Getenv("ILC_ROOT"));
  strcat(fIconsPath,"/DISPLAY/icons/");
  LoadFromRC();
  fMainFrame = new TGMainFrame(p,w,h,kVerticalFrame);
  fSubFrame = new TGCompositeFrame(fMainFrame,w,h,kHorizontalFrame);
  fLeftFrame = new TGCompositeFrame(fSubFrame,150,h,kVerticalFrame|kFixedWidth);
  fRightFrame = new TGCompositeFrame(fSubFrame,600,h,kVerticalFrame);
  //fMainFrame->Connect("ProcessedEvent(Event_t*)", "IlcDisplay2", this,"HandleMouseWheel(Event_t*)");
  fMainFrame->Connect("ProcessedEvent(Event_t*)", "IlcDisplay2",this,"HandleResize(Event_t*)");
  //MenuBar
  fMenu = new IlcMenu(fMainFrame,1,1,kRaisedFrame|kHorizontalFrame);
  
  //Slider Frame
  fSliderFrameLayout = new TGLayoutHints( kLHintsBottom| kLHintsRight| kLHintsExpandX | kLHintsCenterX, 2, 2, 2, 2);
  fSliderFrame  = new IlcSliderFrame(fRightFrame,600,150);
  fRightFrame->AddFrame(fSliderFrame->GetSliderFrame(),fSliderFrameLayout);
  
  //Info Frame
  fInfoFrameLayout = new TGLayoutHints( kLHintsTop | kLHintsLeft  | kLHintsExpandX  ,0,0,0,0);
  fInfoFrame = new IlcInfoFrame(fLeftFrame,150,200);
  fLeftFrame->AddFrame(fInfoFrame->GetInfoFrame(),fInfoFrameLayout);
  
  
  //Shutter Frame
  fShutterFrameLayout = new TGLayoutHints( kLHintsTop | kLHintsLeft | kLHintsExpandY |kLHintsExpandX, 0, 0, 5, 0);
  fShutterFrame = new IlcShutterFrame(fLeftFrame,150,300);
  fLeftFrame->AddFrame(fShutterFrame->GetShutterFrame(),fShutterFrameLayout);
  
  //Display Frame
  fDisplayFrameLayout = new TGLayoutHints( kLHintsTop | kLHintsRight | kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2); 
  fDisplayFrame = new IlcDisplayFrame(fRightFrame, w-150,w-110);
  fRightFrame->AddFrame(fDisplayFrame->GetDisplayFrame(),fDisplayFrameLayout);
  fDisplayFrame->GetDisplayFrame()->Connect("ProcessedEvent(Event_t*)", "IlcDisplay2", this,"HandleMouseWheel(Event_t*)");
  
  
  fLeftFrame->Layout();
  
  fSubFrame->AddFrame(fLeftFrame, new TGLayoutHints( kLHintsBottom | kLHintsLeft | kLHintsExpandY, 5, 5, 2, 2));
  fSubFrame->AddFrame(fRightFrame, new TGLayoutHints( kLHintsBottom | kLHintsRight | kLHintsExpandX | kLHintsExpandY, 5, 5, 2, 2));
  
  Int_t parts[] = {45,45,10};
  fStatusBar = new TGStatusBar(fMainFrame,50,10,kHorizontalFrame);
  fStatusBar->SetParts(parts,3);
  fStatusBar->SetText("IlcDisplay v2.0",0);
  fMainFrame->AddFrame(fStatusBar,new TGLayoutHints(kLHintsBottom | kLHintsExpandX,0,0,0,0));
  
  fMainFrame->AddFrame(fSubFrame,new TGLayoutHints( kLHintsBottom | kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 5, 5, 2, 2));
  fMainFrame->SetWindowName("Ilc Display");
  
  fMainFrame->MapSubwindows();
  fMainFrame->MapWindow();
  LoadSettings();

	
  fMainFrame->Resize(w-10,h);
  fMainFrame->Resize(w,h);
  fMainFrame->SetWMSizeHints(500,500,1280, 1200,1,1);
}

//_____________________________________________________________
IlcDisplay2::~IlcDisplay2(void)
{
  //Destructor
  delete fModules;
  delete [] fEnabledModules;
  delete fModuleInfo;
  
  delete fSliderFrameLayout;
  delete fSliderFrame;	
  delete fDisplayFrameLayout;
  delete fDisplayFrame;
  //	delete fZoomFrameLayout;
  //	delete fZoomFrame;
  delete fShutterFrameLayout;
  delete fShutterFrame;
  delete fInfoFrameLayout;
  delete fInfoFrame;
  delete fDetectorFrameLayout;
  delete fDetectorFrame;
  
  delete fSubFrame;
  delete fLeftFrame;
  delete fRightFrame;
  delete fMainFrame;
  delete fIlcDisplay2rc;
  
  delete fMenu;
  delete fStatusBar;
}

//_____________________________________________________________
void IlcDisplay2::CloseWindow(void)
{
  // Deletes the current display
  delete this;
}

//_____________________________________________________________
void IlcDisplay2::LoadFromRC()
{
  // Load the environment settings from .ilcdisplayrc file
  TEnv *rc=new TEnv(".ilcdisplayrc");
  SetSliderUpdate(rc->GetValue("IlcDisplay.SliderUpdate",kFALSE));
  SetZoomStep(rc->GetValue("IlcDisplay.ZoomStep",1.2));
  SetSliderStep(rc->GetValue("IlcDisplay.SliderStep",0.01));
  char c[128];
  fRawDataPath = new char[128];
  strcpy(c,gSystem->Getenv("ILC_ROOT"));
  sprintf(fRawDataPath,"%s%s",c,rc->GetValue("IlcDisplay.RawDataPath","/raw"));
  printf("\nRaw data path %s",fRawDataPath);
}

//_____________________________________________________________
void IlcDisplay2::SaveToRC() const
{
  // Saves the environment settings in .ilcdisplayrc file
  TEnv *rc=new TEnv(".ilcdisplayrc");
  rc->SetValue("IlcDisplay.SliderUpdate",GetSliderUpdate());
  rc->SetValue("IlcDisplay.ZoomStep",GetZoomStep());
  rc->SetValue("IlcDisplay.SliderStep",GetSliderStep());
  rc->SetValue("IlcDisplay.RawDataPath","/raw");
  rc->SaveLevel(kEnvLocal);
  rc->Save();
}

//_____________________________________________________________
void IlcDisplay2::DoSaveSettings(void)
{
  // Saves the environment settings for the slider frame and display
  fSliderFrame->SaveToRC();
  SaveToRC();
}

//_____________________________________________________________
void IlcDisplay2::LoadSettings()
{
  // Loads settings
  LoadFromRC();
}

//_____________________________________________________________
void IlcDisplay2::Draw(Option_t */*options*/)
{
  // Draws display frame
  fDisplayFrame->DoView(fCurrentView);
}

//_____________________________________________________________
void IlcDisplay2::DrawX3d()
{
  // Draws display frame using X3d
  fDisplayFrame->DrawX3d();
}

//_____________________________________________________________
void IlcDisplay2::DrawGL()
{
  // Draws display frame using GL
  fDisplayFrame->DrawGL();
}

//_____________________________________________________________
void IlcDisplay2::ShowNextEvent(Int_t delta)
{
  //Load the next event
  clock_t t1,t2;
  t1=clock();
  Int_t newEvent=0;
  if(delta!=0){
    gIlc->Clear();
    newEvent = fEventNumber + delta;
    if( newEvent < 0) return;
    gIlc->GetEvent(newEvent);
    fEventNumber += delta;
    //		if(!gIlc->TreeH()) return;
// // //     if(!((TTree*)(detector->GetLoader())->TreeH())) return;
    

    
  }
  if(IsEnabled(kHits)) LoadHits();
  if(IsEnabled(kClusters)) LoadClusters(newEvent);
  if(IsEnabled(kHLT)) LoadHLTClusters(newEvent);
  t2=clock();
  //	printf("\nEvent loaded in....%f sec", ((double)t2-t1)/(10000*CLK_TCK));
  Update(kmMODULES);
}

//_____________________________________________________________
void IlcDisplay2::FindModules()
{
  // Find the modules used for the simulation and assign 
  // these modules to the array fModules
  fModules = new TObjArray;
  TObjArray *modules = gIlc->Modules();
  IlcModule *mod;
  Int_t nbm = 0;
  for(Int_t i=0;i<modules->GetEntriesFast();i++){
    mod = (IlcModule *) modules->At(i);
    if(!mod) continue;
    const char *avoid = strstr("BODY MAG ABSO DIPO HALL FRAME SHIL PIPE",mod->GetName());
    if(avoid) continue;
    fModules->AddLast(mod);
    nbm++;
  }
  fEnabledModules = new Bool_t[nbm];
  fNbModules = nbm;
  fModuleInfo = new IlcModuleInfo(nbm);
  for(Int_t j=0;j<fModules->GetEntriesFast();j++){
    fModuleInfo->Add(fModules->At(j)->GetName(),j);
    fEnabledModules[j]=kTRUE;
  }
}

//_____________________________________________________________
void IlcDisplay2::LoadHits()
{
  //Load the detected hits from each detector to memory

  if(fPoints)
    fPoints->Delete();


  // // //   gIlc->ResetPoints();
  TIter next(gIlc->Modules());
  IlcModule *module;
  Int_t ntracks = gIlc->GetMCApp()->GetNtrack();
  if (fPoints == 0) fPoints = new TObjArray(ntracks*GetNbModules());
  for (Int_t track=0; track<ntracks*GetNbModules();track++)
    fPoints->AddAtAndExpand(0, track);

  while((module = (IlcModule*)next()))
  {
    IlcDetector* detector = dynamic_cast<IlcDetector*>(module);
    if(detector) detector->SetTreeAddress();
  }
  next.Reset();
  for (Int_t track=0; track<ntracks;track++) {
// // //     gIlc->ResetHits();
    while((module = (IlcModule*)next())) {
      IlcDetector* detector = dynamic_cast<IlcDetector*>(module);
      if(detector)
	{

	  (TTree*)(detector->GetLoader())->TreeH()->GetEvent(track);
// 	  detector->TreeH()->GetEvent(track);
// 	  detector->LoadPoints(track);
	  LoadPoints(detector,track);
// 	  detector->Print();
	}
    }
    next.Reset();
  }
  fHitsLoaded = kTRUE;
}

//_____________________________________________________________
void IlcDisplay2::LoadPoints(IlcDetector* detector, Int_t track)
{
  //
  // Store x, y, z of all hits in memory
  //

  // detector->Print();
// // // // //   if(gIlcDisplay2->GetModuleInfo()->IsEnabled(detector->GetName()))
// // // // //     printf("%d %s\n", track, detector->GetName());
// // // // // 
// // // // //   if(!(gIlcDisplay2->GetModuleInfo()->IsEnabled(detector->GetName()))) return;

  if (detector->Hits() == 0)
  {
//     if(!strcmp(detector->GetName(), "EMCAL"))
      IlcError(Form("fHits == 0. Name is %s track %d",detector->GetName(), track));
    return;
  }
  //
  Int_t nhits = detector->Hits()->GetEntriesFast();
//   if(!strcmp(detector->GetName(), "EMCAL"))
//   IlcInfo(Form("0 %s has %d hits Id: %d",detector->GetName(), nhits, fModuleInfo->Id(detector->GetName())));
  
  if (nhits == 0)
  {
    //    Error("LoadPoints","nhits == 0. Name is %s",GetName());
    return;
  }

  Int_t tracks = gIlc->GetMCApp()->GetNtrack();
//   if (fPoints == 0) fPoints = new TObjArray(tracks);
  IlcHit *ahit;
  //
  Int_t *ntrk=new Int_t[tracks];
  Int_t *limi=new Int_t[tracks];
  Float_t **coor=new Float_t*[tracks];
  for(Int_t i=0;i<tracks;i++) {
    ntrk[i]=0;
    coor[i]=0;
    limi[i]=0;
  }
  //
  IlcPoints *points = 0;
  Float_t *fp=0;
  Int_t trk;
  Int_t chunk=nhits/4+1;
  //
  // Loop over all the hits and store their position
  for (Int_t hit=0;hit<nhits;hit++) {
    ahit = dynamic_cast<IlcHit*>(detector->Hits()->UncheckedAt(hit));
    trk=ahit->GetTrack();
    if(trk>tracks) {
      IlcError(Form("Found track number %d, max track %d",trk, tracks));
      continue;
    }
    if(ntrk[trk]==limi[trk])
    {
      //
      // Initialise a new track
      fp=new Float_t[3*(limi[trk]+chunk)];
      if(coor[trk])
      {
	memcpy(fp,coor[trk],sizeof(Float_t)*3*limi[trk]);
	delete [] coor[trk];
      }
      limi[trk]+=chunk;
      coor[trk] = fp;
    }
    else
    {
      fp = coor[trk];
    }
    fp[3*ntrk[trk]  ] = ahit->X();
    fp[3*ntrk[trk]+1] = ahit->Y();
    fp[3*ntrk[trk]+2] = ahit->Z();
    ntrk[trk]++;
  }
  //
  for(trk=0; trk<tracks; ++trk) {
    if(ntrk[trk]) {
      points = new IlcPoints();
//       points->SetMarkerColor(GetMarkerColor());
//       points->SetMarkerSize(GetMarkerSize());
//       points->SetDetector(this);
      points->SetMarkerColor(2);
      points->SetMarkerSize(1);
      points->SetDetector(detector);
      points->SetParticle(trk);
//       points->SetPolyMarker(ntrk[trk],coor[trk],GetMarkerStyle());
      points->SetPolyMarker(ntrk[trk],coor[trk],7);
      points->SetMarkerStyle(6);
//       IlcInfo(Form("%s\t%d\t%d\t%d\t%d", detector->GetName(), tracks, trk, fModuleInfo->Id(detector->GetName()), trk+tracks*(fModuleInfo->Id(detector->GetName())-1000)));
      fPoints->AddAtAndExpand(points,trk+tracks*(fModuleInfo->Id(detector->GetName())-1000) );
      delete [] coor[trk];
      coor[trk]=0;
    }
  }
  delete [] coor;
  delete [] ntrk;
  delete [] limi;


}

//_____________________________________________________________
void IlcDisplay2::LoadClusters(Int_t nevent)
{
  //clock_t t1,t2;
  fDisplayFrame->LoadClusters(nevent);
  fClustersLoaded = kTRUE;
  //	printf("\nClusters loaded in....%f sec", ((double)t2-t1)/(10000*CLK_TCK));
}

//_____________________________________________________________
void IlcDisplay2::LoadHLTClusters(Int_t nevent)
{
  // Loads HLT clusters
  fDisplayFrame->LoadHLTClusters(nevent);
  fHLTLoaded = kTRUE;
}

//_____________________________________________________________
void IlcDisplay2::Enable(Int_t m)
{
  // Enables the given mode m
  if(m==kHits){
    if((fMode&kHits)==kHits) return;
    fMode = kHits|fMode;
    /*if(!fHitsLoaded)*/ LoadHits();
    Update(kmPOINTS);
  }
  if(m==kClusters){
    if((fMode&kClusters)==kClusters) return;
    fMode = kClusters|fMode;
    if(!fClustersLoaded) LoadClusters(fEventNumber);
    Update();
  }
  if(m==kHLT){
    if((fMode&kHLT)==kHLT) return;
    fMode = kHLT|fMode;
    if(!fHLTLoaded) {
      LoadHLTClusters(fEventNumber);
    }
    Update();
  }
  if(m==kTracks){
    if((fMode&kTracks)==kTracks) return;
    fMode = kTracks|fMode;
    Update();
  }
}

//_____________________________________________________________
void IlcDisplay2::Disable(Int_t m)
{
  // Disables the given mode m
  if(m==kHits){
    fMode = fMode|kHits;
    fMode = fMode^kHits;
  }
  if(m==kClusters){
    fMode = fMode|kClusters;
    fMode = fMode^kClusters;
  }
  if(m==kHLT){
    fMode = fMode|kHLT;
    fMode = fMode^kHLT;
  }
  if(m==kTracks){
    fMode = fMode|kTracks;
    fMode = fMode^kTracks;
  }
  Update();
}

//_____________________________________________________________
Bool_t IlcDisplay2::IsEnabled(Int_t m) const
{
  // Checks if the mode m is enabled
  if(m==kHits){
    if((fMode&kHits)==kHits) return kTRUE;
    return kFALSE;
  }
  if(m==kClusters){
    if((fMode&kClusters)==kClusters) return kTRUE;
    return kFALSE;
  }
  if(m==kHLT){
    if((fMode&kHLT)==kHLT) return kTRUE;
    return kFALSE;
  }
  if(m==kTracks){
    if((fMode&kTracks)==kTracks) return kTRUE;
    return kFALSE;
  }
  return kFALSE;
}

//_____________________________________________________________
void IlcDisplay2::HandleMouseWheel(Event_t *event)
{
  //Handle mouve event, not working yet
  if(event->fType != kButtonPress && event->fType != kButtonRelease) return;
  
  if(event->fCode == kButton4){
    fZoomFactor *= fZoomStep;
    Draw();
  }
  
  if(event->fCode == kButton5){
    fZoomFactor /= fZoomStep;
    Draw();
  }
}

//_____________________________________________________________
void IlcDisplay2::HandleResize(Event_t *event)
{
  // Handle resize event
  switch(event->fType){
  case kConfigureNotify:{
    Draw();
  }
    break;
  default:break;
  }
}	

//_____________________________________________________________
void IlcDisplay2::Update(Int_t tag)
{
  // Update the view, if loading only the modified data from the previous
  // changes, the integer tag indicates the kind of modification
  if(tag==kmMODULES){
    LoadEnabledModules();
    if(((fMode)&kHits)==kHits){
      LoadEnabledHits();
      ApplyCuts();
    }
  }
  if(tag==kmCUTS){
    if(((fMode)&kHits)==kHits)ApplyCuts();
  }
  if(tag==kmPOINTS){
    if(((fMode)&kHits)==kHits){
      LoadEnabledHits();
      ApplyCuts();
    }
  }
  Draw();
  fInfoFrame->Update();
}


