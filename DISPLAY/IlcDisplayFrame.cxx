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
// ILC DISPLAY FRAME CLASS                                           //
// Author: Mayeul   ROUSSELET                                          //
// e-mail: Mayeul.Rousselet@cern.ch                                    //
// Last update:26/08/2003                                              //
/////////////////////////////////////////////////////////////////////////

#include <time.h>

#include <TGFrame.h>
#include <TGTab.h>
#include <TObjArray.h>
#include <TStopwatch.h>
#include <TPad.h>
#include <TVirtualX.h>
#include <TCanvas.h>
#if ROOT_VERSION_CODE>= 331523
#include <TView3D.h>
#else
#include <TView.h>
#endif
#include <TParticle.h>
#include <TGeometry.h>
#include <TGeoManager.h>
#include <TNode.h>

#include "IlcModuleInfo.h"
#include "IlcDisplayHLT.h"
#include "IlcDisplay2.h"
#include "IlcModule.h"
#include "IlcDetector.h"
#include "IlcPoints.h"
#include "IlcRun.h"


ClassImp(IlcDisplayFrame)

//_____________________________________________________________
IlcDisplayFrame::IlcDisplayFrame(const TGWindow *p, UInt_t w, UInt_t h)
{
  // Constructor
  fClipMin=-20;
  fClipMax=20;
  fPreviousW=0;
  fPreviousH=0;
  fRange = 500;
  fPolyMarkers = new TObjArray(1000);
  
  fMainFrame = new TGCompositeFrame(p,w,h);
  fMainTab = new TGTab(fMainFrame, w, h);
  fFrame1 = fMainTab->AddTab("Main View");
  fMainEmbeddedCanvas = new TRootEmbeddedCanvas("Main12",fFrame1,w,h,kFixedWidth);
  fFrame1->AddFrame(fMainEmbeddedCanvas,new TGLayoutHints( kLHintsTop | kLHintsLeft|kLHintsExpandX| kLHintsExpandY, 0, 0, 0, 0));
  fMainCanvas = fMainEmbeddedCanvas->GetCanvas();
  fMainCanvas->SetFillColor(1);
  fMainCanvas->SetBorderMode(0);
  fMainCanvas->cd();
  fMainCanvas->SetFixedAspectRatio();
  fMainCanvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)","IlcDisplayFrame",this,"ExecuteEvent(Int_t,Int_t,Int_t,TObject*)");
  //fView = new TView(1);
  //DoView(kIdbFRONTVIEW);
  
  gIlcDisplay2->SetCurrentView(kIdbFRONTVIEW);	
  
  fFrame2 = fMainTab->AddTab("No detector");
  fSelectionEmbeddedCanvas = new TRootEmbeddedCanvas("Selection",fFrame2,w,h);
  fSelectionCanvas = fSelectionEmbeddedCanvas->GetCanvas();
  fSelectionCanvas->SetFillColor(1);
  fSelectionCanvas->SetBorderMode(0);
  fSelectionCanvas->cd();
  fFrame2->AddFrame(fSelectionEmbeddedCanvas,new TGLayoutHints( kLHintsTop | kLHintsLeft|kLHintsExpandX| kLHintsExpandY, 0, 0, 0, 0));
  fMainFrame->AddFrame(fMainTab,new TGLayoutHints( kLHintsTop | kLHintsLeft|kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  fAllViews = kFALSE;
  fMainFrame->MapSubwindows();
  fMainFrame->MapWindow();

  fGeometry = new TGeometry("IlcGeom","Gilc Geometry for Hits");

}

//_____________________________________________________________
IlcDisplayFrame::~IlcDisplayFrame(void)
{
  // Destructor
  delete fMainTab;
  delete fSelectionEmbeddedCanvas;
  delete fMainEmbeddedCanvas;
  delete fFrame1;
  delete fFrame2;
  delete fMainCanvas;
  delete fSelectionCanvas;
  delete fPoints2;
  delete fPoints;
  delete fModules;
  delete fMainFrame;
  delete [] fActivePoints;
  delete [] fClustersPos;
  fGeometry->Delete();
  delete fGeometry;
}

//_____________________________________________________________
void IlcDisplayFrame::DoView(Int_t view)
{
  // Draws selected view
  Int_t x,y;
  char vname[16];
  y=fMainFrame->GetDefaultHeight();
  x=fMainFrame->GetDefaultWidth();
  gIlcDisplay2->SetCurrentView(view);
  switch(view){
  case kIdbALLVIEW:{
    fAllViews=kTRUE;
    strcpy(vname,"All views");	
    fMainCanvas->cd();
    gPad->Clear();
    fMainCanvas->SetFillColor(15);
    fMainCanvas->Divide(2,2,0.005,0.005,1);
    
    fMainCanvas->cd(1);
    gIlcDisplay2->SetZoomFactor(.008);
    Draw(80,170,70,0);
    gIlcDisplay2->SetZoomFactor(5.5);
    
    fMainCanvas->cd(2);
    gIlcDisplay2->SetCurrentView(kIdbTOPVIEW);
    Draw(90,-90,90,0);
    
    fMainCanvas->cd(3);		
    gIlcDisplay2->SetCurrentView(kIdbSIDEVIEW);
    Draw(90,180,90,0);
    
    fMainCanvas->cd(4);
    gIlcDisplay2->SetCurrentView(kIdbFRONTVIEW);
    Draw(0,-90,0,0);
    
    //fMainCanvas->cd();
    
  }
    break;
  case kIdbTOPVIEW:{
    strcpy(vname,"Top view  ");
    fAllViews=kFALSE;
    fMainCanvas->cd();
    gPad->SetFillColor(1);
    gPad->Clear();
    gPad->Draw();
    Draw(90,-90,90);
  }
    break;
  case kIdbSIDEVIEW:{
    strcpy(vname,"Side view");
    fAllViews=kFALSE;	
    fMainCanvas->cd();		
    gPad->SetFillColor(1);
    gPad->Clear();
    gPad->Draw();
    Draw(90,180,90);
  }
    break;
  case kIdbFRONTVIEW:{
    strcpy(vname,"Front view");
    fAllViews=kFALSE;	
    fMainCanvas->cd();
    gPad->SetFillColor(1);
    gPad->Clear();
    gPad->Draw();
    
    Draw(0,-90,0);
  }
    break;
  default: break;
  }
  (fMainTab->GetTabTab(0))->SetText(new TGString(vname));
}

//_____________________________________________________________
void IlcDisplayFrame::DrawDetector(const char *name)
{
  // Draws detector
  (fMainTab->GetTabTab(1))->SetText(new TGString(name));
}


//_______________________________________________________________________
TGeometry *IlcDisplayFrame::GetGeometry(Bool_t check)
{


  if(check){
    if(fGeometry)
      fGeometry->Delete();
    fGeometry = new TGeometry("IlcGeom","Gilc Geometry for Hits");
  }
  else {
    if(!fGeometry){
      fGeometry = new TGeometry("IlcGeom","Gilc Geometry for Hits");
    }
    else {
      if(fGeometry)
// 	fGeometry->Delete();
      fGeometry = new TGeometry("IlcGeom","Gilc Geometry for Hits");
    }
  }
  
  TIter next(fModules);
  IlcModule *detector;
  while((detector = dynamic_cast<IlcModule*>(next()))) {
//     detector->Print();
    if(!(gIlcDisplay2->GetModuleInfo()->IsEnabled(detector->GetName()))) continue;
//     printf("%s\n", detector->GetName());
    TList *dnodes=detector->BuildGeometry2();
    Int_t j;
    TNode *node, *node1;
    if (!dnodes) continue;
    for ( j=0; j<dnodes->GetSize(); j++) {
      node = dynamic_cast<TNode*>(dnodes->At(j));
      node1 = fGeometry->GetNode(node->GetName());
      dnodes->Remove(node);
    }
  }
  return fGeometry;
}


//_____________________________________________________________
void IlcDisplayFrame::EnableDetector(const char *name)
{
  // Enables detector
  IlcModule *module = dynamic_cast<IlcModule*>(gIlc->Modules()->FindObject(name));
  if(!module) return;
  gIlcDisplay2->GetModuleInfo()->Enable((char*)name);
// // // //   module->Enable();
}

//_____________________________________________________________
void IlcDisplayFrame::DisableDetector(const char *name)
{
  // Disables detector
  IlcModule *module = dynamic_cast<IlcModule*>(gIlc->Modules()->FindObject(name));
  if(!module) return;
  gIlcDisplay2->GetModuleInfo()->Disable((char*)name);
// // // //   module->Disable();
}

//_____________________________________________________________
void IlcDisplayFrame::Draw(Float_t theta, Float_t phi, Float_t psi, Bool_t check)
{
  // Draws everything???
  //clock_t t1,t2;
  time_t t1,t2;
  //t1 = clock();
//   TStopwatch timer;
//   timer.Start();
//   time(&t1);
  gPad->SetCursor(kWatch);
  gPad->SetEditable(kTRUE);
  gPad->SetFillColor(1);
//   gPad->Clear();
  
  Int_t iret;
  
#if ROOT_VERSION_CODE>= 331523
  Double_t rmin[]={-1,-1,-1};
  Double_t rmax[]={ 1, 1, 1};
  TView *view = new TView3D(1,rmin,rmax);
#else
  TView *view = new TView(1);
#endif
  TGDimension dim=((TGCanvas*)fMainEmbeddedCanvas)->GetViewPort()->GetDefaultSize();
  Float_t aspectRatio = dim.fWidth/(Float_t) dim.fHeight;
  //printf("Dimension %d %d",dim.fWidth,dim.fHeight);
  if(gIlcDisplay2->GetCurrentView()==kIdbFRONTVIEW){
    view->SetRange(-fRange*aspectRatio,-fRange,-fRange,fRange*aspectRatio,fRange,fRange);
  }
  if(gIlcDisplay2->GetCurrentView()==kIdbTOPVIEW){
    view->SetRange(-fRange,-fRange,-fRange*aspectRatio,fRange,fRange,fRange*aspectRatio);
  }
  if(gIlcDisplay2->GetCurrentView()==kIdbSIDEVIEW){
    view->SetRange(-fRange,-fRange,-fRange*aspectRatio,fRange,fRange,fRange*aspectRatio);
  }
  
// // // //   gIlc->GetGeometry()->Draw("same");

  GetGeometry(check)->Draw("same");

  if(gIlcDisplay2->IsEnabled(kHits)) DrawHits();
  if(gIlcDisplay2->IsEnabled(kClusters)) fClusters->Draw();
  if(gIlcDisplay2->IsEnabled(kHLT)) fHLT->Draw();
  
  gIlcDisplay2->AppendPad();
  view->SetView(phi,theta,psi,iret);
  
  view->ZoomView(gPad,gIlcDisplay2->GetZoomFactor());
  //t2 = clock();
//   time(&t2);
  //	printf("\nDrawn in....%f sec", ((double)t2-t1)/(10000*CLK_TCK));
//   printf("\nDrawn in....%f sec", difftime(t2,t1));
//   timer.Stop();
//   timer.Print("m");
}

//_____________________________________________________________
void IlcDisplayFrame::DrawHits()
{

  // Draws hits
  IlcPoints *p;
  TObjArray *ppoints = gIlcDisplay2->Points();
  if(!ppoints) return;
//   printf("\n 4 nb hits %d",ppoints->GetEntriesFast());
  for(Int_t i=0;i<ppoints->GetEntriesFast();i++){
    if(fActivePoints[i]){
//       printf("\n ActivePoints %d %d", i, fActivePoints[i]);
      p=dynamic_cast<IlcPoints *>(ppoints->UncheckedAt(i));
      if(!p) continue;
      p->Draw();
    }
  }


// // //   // Draws hits
// // //   IlcPoints *p;
// // //   if(!fPoints2) return;
// // //   printf("\n 4 nb hits %d",fPoints2->GetEntries());
// // //   for(Int_t i=0;i<fPoints2->GetEntries();i++){
// // //     if(fActivePoints[i]){
// // //       printf("\n ActivePoints %d %d", i, fActivePoints[i]);
// // //       p=dynamic_cast<IlcPoints *>(fPoints2->UncheckedAt(i));	
// // //       if(!p) continue;
// // //       p->Draw();
// // //     }
// // //   }


}

//_____________________________________________________________
void IlcDisplayFrame::LoadEnabledModules()
{
  // Loads enabled modules
  clock_t t1,t2;
  t1=clock(); 
  TIter next(gIlc->Modules());
  IlcModule *module;
  fModules = new TObjArray(0,32);
  while((module = dynamic_cast <IlcModule*> (next()))){
    if(!module) continue;
    if(!module->IsActive()) continue;
    fModules->AddLast(module);
  }
  t2=clock();
  fNbModules = fModules->GetEntriesFast();
  //	printf("\nModules loaded in.....%f sec", ((double)t2-t1)/(10000*CLK_TCK));
}

//_____________________________________________________________
void IlcDisplayFrame::LoadClusters(Int_t nevent)
{
  // Loads clusters
  fClusters = new IlcDisplayClusters();
  fClusters->LoadClusters("ITS TPC",nevent);
}

//_____________________________________________________________
void IlcDisplayFrame::LoadHLTClusters(Int_t nevent)
{
  // Loads HLT clusters
  fHLT = new IlcDisplayHLT();
  fHLT->LoadHLT("TPC",nevent);
}
	
//_____________________________________________________________
void IlcDisplayFrame::LoadHits()
{
  // Loads hits
  clock_t t1,t2;

  t1=clock();

//   fPoints2 = new TObjArray(0,1000);
  TObjArray *ppoints = gIlcDisplay2->Points();
  if(!ppoints) {
    return;
  }
//   printf("\n 1 nb hits %d\n",ppoints->GetEntriesFast());
//   for(Int_t j=0;j<ppoints->GetEntriesFast();j++){
//     if(!ppoints->UncheckedAt(j)) continue;
//     fPoints2->AddLast((ppoints->UncheckedAt(j)));
//   }
//   printf("\n 2 nb hits %d",ppoints->GetEntriesFast());
  fActivePoints = new Bool_t[ppoints->GetEntriesFast()];
  for(Int_t k=0;k<ppoints->GetEntriesFast();k++){
    fActivePoints[k]=kTRUE;
  }
//   printf("\n 3 nb hits %d",ppoints->GetEntriesFast());
  t2=clock();

  return;

// // // //   fPoints2 = new TObjArray(0,1000);
// // // //   IlcModule *module;
// // // //   TObjArray *points;
// // // //   for(Int_t i=0;i<fNbModules;i++){
// // // //     module = dynamic_cast<IlcModule*>(fModules->UncheckedAt(i));
// // // //     if(!module) continue;
// // // //     points = module->Points();
// // // //     module->Print();
// // // //     if(!points) {
// // // //       continue;
// // // //     }
// // // //     for(Int_t j=0;j<points->GetEntriesFast();j++){
// // // //       if(!points->UncheckedAt(j)) continue;
// // // //       fPoints2->AddLast((points->UncheckedAt(j)));
// // // //     }	
// // // //   }
// // // //   fActivePoints = new Bool_t[fPoints2->GetEntries()];
// // // //   for(Int_t k=0;k<fPoints2->GetEntriesFast();k++){
// // // //     fActivePoints[k]=kTRUE;
// // // //   }
// // // //   printf("\n nb hits %d",fPoints2->GetEntries());
// // // //   t2=clock();
  //	printf("\nPoints loaded in....%f sec", ((double)t2-t1)/(10000*CLK_TCK));
}

//_____________________________________________________________
void IlcDisplayFrame::ApplyCuts()
{

  TObjArray *ppoints = gIlcDisplay2->Points();
  // Applies cuts
  clock_t t1,t2;
  t1=clock();
  
  Float_t		*pxyz;
  Float_t		r,theta,eta,cutmin,cutmax,etamin,etamax,pmom,smin,smax;
  Int_t		nbhits=0;
  IlcPoints *pm;
  TParticle *particle;
  
  //Get momentum cut
  smin = gIlcDisplay2->GetMomentumMin();
  smax = gIlcDisplay2->GetMomentumMax();
  cutmin = 2.0*smin;
  if(smax<0.98) 	cutmax = 2.0*smax;
  else 			cutmax = 100000;
  
  //Get rapidity cut
  smax = gIlcDisplay2->GetRapidityMax();
  smin = gIlcDisplay2->GetRapidityMin();
  //etamin = 1.5*(2*smin-1);
  //etamax = 1.5*(2*smax-1);
  etamin = smin;
  etamax = smax;
  if(smin<-1.46) etamin = -1000;
  if(smax>1.46) etamax = 1000;
  
  
  if(!ppoints) return;
  for(Int_t i=0;i<ppoints->GetEntriesFast();i++){
    pm = dynamic_cast<IlcPoints*>(ppoints->UncheckedAt(i));
    if(!pm) {
      fActivePoints[i]=kFALSE;
      continue;
    }

    if(!(gIlcDisplay2->GetModuleInfo()->IsEnabled(pm->GetDetector()->GetName()))){
      fActivePoints[i]=kFALSE;
      continue;
    }

    particle = pm->GetParticle();
    if(!particle) {
      fActivePoints[i]=kFALSE;
      continue;
    }
    pmom = particle->P();
    if(pmom < cutmin) {
      fActivePoints[i]=kFALSE;
      continue;
    }
    if(pmom > cutmax) {
      fActivePoints[i]=kFALSE;
      continue;
    }
    pxyz = pm->GetP();
    r = TMath::Sqrt(pxyz[0]*pxyz[0]+pxyz[1]*pxyz[1]);
    theta = TMath::ATan2(r,TMath::Abs(pxyz[2]));
    if(theta) eta = -TMath::Log(TMath::Abs(TMath::Tan(0.5*theta)));
    else eta = 1e10;
    if(pxyz[2] < 0) eta = -eta;
    if((eta < etamin) || (eta > etamax)) {
      fActivePoints[i]=kFALSE;
      continue;
    }
    fActivePoints[i]=kTRUE;
    //pm->Draw();
    nbhits += pm->GetN();
  }
  gIlcDisplay2->SetNbHits(nbhits);
  t2=clock();
  //	printf("\nCuts applied in....%f sec", ((double)t2-t1)/(10000*CLK_TCK));
  gIlcDisplay2->SetNbParticles(GetNbActivePoints()); 
}

//_____________________________________________________________
Int_t IlcDisplayFrame::GetNbActivePoints() const
{
  // Returns the number of active points
  Int_t ans=0;
  TObjArray *ppoints = gIlcDisplay2->Points();
  for(Int_t i=0;i<ppoints->GetEntries();i++){
    if(fActivePoints[i]) ans++;
  }
  return ans;
}
//_____________________________________________________________
void IlcDisplayFrame::DrawX3d()
{
  // Draws using X3d
  TPad *pad = dynamic_cast<TPad*>(gPad);
  pad->cd();
  TView *view = pad->GetView();
  if(!view) return;
  pad->GetViewer3D("x3d");
}

//_____________________________________________________________
void IlcDisplayFrame::SavePadGIF(const char *file)
{
  // Save the current pad in a GIF file
  if(!gPad){
    printf("\nThere is no active pad");
    return;
  }
  gPad->SaveAs(file);
}

//_____________________________________________________________
void IlcDisplayFrame::DrawGL()
{
  // Draws using GL
  TPad *pad = dynamic_cast<TPad*>(gPad);
  pad->cd();
  TView *view = pad->GetView();
  if(!view) return;
  pad->GetViewer3D("ogl");
}

//_____________________________________________________________
void IlcDisplayFrame::ExecuteEvent(Int_t event, Int_t px,Int_t py,TObject *)
{
  static Float_t x0,y0,x1,y1;
  static Int_t pxold,pyold;
  static Int_t px0,py0;
  static Int_t linedrawn;
  Float_t temp;
  
  
  switch(event){
  case kMouseMotion:{
    
    IlcPoints *p=dynamic_cast<IlcPoints*> (gPad->GetSelected());
    if(p){
      gIlcDisplay2->SetStatusBar(p->GetName(),1);
      gIlcDisplay2->SetStatusBar(p->GetDetector()->GetName(),2);
    }
  }
    break;
  default:break;
  }	
  
  if((!gIlcDisplay2->GetZoomMode())&&(gPad->GetView())){
    gPad->GetView()->ExecuteRotateView(event,px,py);
    return;
  }
  
  
  
  if(gIlcDisplay2->GetZoomMode()==kTRUE){
    switch(event){
      
    case kButton1Down:{
      gVirtualX->SetLineColor(-1);
      gPad->TAttLine::Modify();
      x0 = gPad->AbsPixeltoX(px);
      y0 = gPad->AbsPixeltoY(py);
      px0 = px;
      py0 = py;
      pxold = px;
      pyold = py;
      linedrawn = 0;
    }
      break;
    case kButton1Motion:{
      if(linedrawn) gVirtualX->DrawBox(px0,py0,pxold,pyold,TVirtualX::kHollow);
      pxold = px;
      pyold = py;
      linedrawn = 1;
      gVirtualX->DrawBox(px0,py0,pxold,pyold,TVirtualX::kHollow);
    }
      break;
      
    case kButton1Up:{
      gPad->GetCanvas()->FeedbackMode(kFALSE);
      if(px == px0) break;
      if(py == py0) break;
      x1 = gPad->AbsPixeltoX(px);
      y1 = gPad->AbsPixeltoY(py);
      if(x1<x0) { 
	temp = x0;
	x0 = x1;
	x1 = temp;
      }
      if(y1<y0) {
	temp = y0;
	y0 = y1;
	y1 = temp;
      }
      printf("\nBox (%f,%f)-(%f,%f)",x0,y0,x1,y1);
      gPad->SetEditable(kTRUE);
      //gPad->Range(x0,y0,x1,y1);
      gPad->SetEditable(kFALSE);
      //gPad->Range(0.5,0.5,1,1);
      //gIlcDisplay2->SetZoomFactor(1);
      gPad->Modified(kTRUE);
      gIlcDisplay2->Draw();	
      gIlcDisplay2->SetZoomMode(kFALSE);
      gPad->SetEditable(kTRUE);
    }
      break; 
    default: break;		
    }		
  }
}
