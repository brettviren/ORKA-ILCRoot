/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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

/* $Id: IlcTARGETReconstructor.cxx,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// class for TARGET reconstruction                                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "Riostream.h"
#include "IlcTARGETReconstructor.h"
#include "IlcRun.h"
#include "IlcRunLoader.h"
#include "IlcRawReader.h"
#include "IlcTARGETDetTypeRec.h"
#include "IlcTARGETLoader.h"
#include "IlcTARGETtrackerMI.h"
#include "IlcTARGETtrackerSA.h"
#include "IlcTARGETVertexerIons.h"
#include "IlcTARGETVertexerFast.h"
#include "IlcTARGETVertexerPPZ.h"
#include "IlcTARGETVertexerZ.h"
#include "IlcESDEvent.h"
#include "IlcTARGETpidESD.h"
#include "IlcTARGETpidESD1.h"
#include "IlcTARGETpidESD2.h"
#include "IlcV0vertexer.h"
#include "IlcCascadeVertexer.h"

ClassImp(IlcTARGETReconstructor)

//___________________________________________________________________________
IlcTARGETReconstructor::IlcTARGETReconstructor() : IlcReconstructor(){
  // Default constructor
  fItsPID=0;
}
 //___________________________________________________________________________
IlcTARGETReconstructor::~IlcTARGETReconstructor(){
// destructor
  delete fItsPID;
} 
// // // // //______________________________________________________________________
// // // // IlcTARGETReconstructor::IlcTARGETReconstructor(const IlcTARGETReconstructor &ob) :IlcReconstructor(ob) {
// // // //   // Copy constructor
// // // //   // Copies are not allowed. The method is protected to avoid misuse.
// // // //   Error("IlcTARGETpidESD2","Copy constructor not allowed\n");
// // // // }

//______________________________________________________________________
IlcTARGETReconstructor& IlcTARGETReconstructor::operator=(const IlcTARGETReconstructor& /* ob */){
  // Assignment operator
  // Assignment is not allowed. The method is protected to avoid misuse.
  Error("= operator","Assignment operator not allowed\n");
  return *this;
}

//_____________________________________________________________________________
void IlcTARGETReconstructor::Reconstruct(IlcRunLoader* runLoader) const
{
// reconstruct clusters

  IlcTARGETLoader* loader = static_cast<IlcTARGETLoader*>(runLoader->GetLoader("TARGETLoader"));
  if (!loader) {
    Error("Reconstruct", "TARGET loader not found");
    return;
  }
  IlcTARGETDetTypeRec* rec = new IlcTARGETDetTypeRec(loader);
  rec->SetDefaults();

  loader->LoadRecPoints("RECREATE");
  loader->LoadDigits("READ");
  runLoader->LoadKinematics();
  TString option = GetOption();
  Bool_t clusfinder=kTRUE;   // Default: V2 cluster finder
  if(option.Contains("OrigCF"))clusfinder=kFALSE;

  Int_t nEvents = runLoader->GetNumberOfEvents();

  Int_t iEvent = runLoader->GetEventNumber();
  runLoader->GetEvent(runLoader->GetEventNumber());
  if(loader->TreeR()==0x0) loader->MakeTree("R");
  rec->MakeBranch("R");
  rec->SetTreeAddress();

    //dan-commento cout <<"chiama DigitsToRecPoints!!!"<<endl;
    rec->DigitsToRecPoints(iEvent,0,"SPD SSD",clusfinder);    

  loader->UnloadRecPoints();
  loader->UnloadDigits();
  runLoader->UnloadKinematics();
}

//_________________________________________________________________
void IlcTARGETReconstructor::Reconstruct(IlcRunLoader* runLoader, 
                                      IlcRawReader* rawReader) const
{
// reconstruct clusters

 
  IlcTARGETLoader* loader = static_cast<IlcTARGETLoader*>(runLoader->GetLoader("TARGETLoader"));
  if (!loader) {
    Error("Reconstruct", "TARGET loader not found");
    return;
  }

  IlcTARGETDetTypeRec* rec = new IlcTARGETDetTypeRec(loader);
  rec->SetDefaults();
  rec->SetDefaultClusterFindersV2(kTRUE);

  loader->LoadRecPoints("recreate");

  Int_t iEvent = 0;

  while(rawReader->NextEvent()) {
    runLoader->GetEvent(iEvent++);
    if(loader->TreeR()==0x0) loader->MakeTree("R");
    rec->DigitsToRecPoints(rawReader);
  }

  loader->UnloadRecPoints();
}

//_____________________________________________________________________________
IlcTracker* IlcTARGETReconstructor::CreateTracker(IlcRunLoader* runLoader)const
{
// create a TARGET tracker

  
  IlcTARGETgeom* geom = GetTARGETgeom(runLoader);
  TString selectedTracker = GetOption();
  IlcTracker* tracker;    
  if (selectedTracker.Contains("MI")) {
    tracker = new IlcTARGETtrackerMI(geom);
  }
  else {
    tracker =  new IlcTARGETtrackerSA(geom);  // inherits from IlcTARGETtrackerMI
  }

  TString selectedPIDmethod = GetOption();
  IlcTARGETLoader *loader = (IlcTARGETLoader*)runLoader->GetLoader("TARGETLoader");
  if (!loader) {
    Error("CreateTracker", "TARGET loader not found");
  }
  if(selectedPIDmethod.Contains("LandauFitPID")){
    loader->AdoptTARGETpid(new IlcTARGETpidESD2((IlcTARGETtrackerMI*)tracker,loader));
  }
  else{
    Double_t parTARGET[] = {34., 0.15, 10.};
    loader->AdoptTARGETpid(new IlcTARGETpidESD1(parTARGET));
  }
  return tracker;
  
}

//_____________________________________________________________________________
IlcTracker* IlcTARGETReconstructor::CreateTracker()const
{
// create a TARGET tracker

  IlcRunLoader *runLoader = IlcRunLoader::Instance();
  IlcTARGETgeom* geom = GetTARGETgeom(runLoader);
  TString selectedTracker = GetOption();
  IlcTracker* tracker;
  if (selectedTracker.Contains("MI")) {
    tracker = new IlcTARGETtrackerMI(geom);
  }
  else {
    tracker =  new IlcTARGETtrackerSA(geom);  // inherits from IlcTARGETtrackerMI
  }

  TString selectedPIDmethod = GetOption();
  IlcTARGETLoader *loader = (IlcTARGETLoader*)runLoader->GetLoader("TARGETLoader");
  if (!loader) {
    Error("CreateTracker", "TARGET loader not found");
  }
  if(selectedPIDmethod.Contains("LandauFitPID")){
    loader->AdoptTARGETpid(new IlcTARGETpidESD2((IlcTARGETtrackerMI*)tracker,loader));
  }
  else{
    Double_t parTARGET[] = {34., 0.15, 10.};
    loader->AdoptTARGETpid(new IlcTARGETpidESD1(parTARGET));
  }
  return tracker;
  
}

//_____________________________________________________________________________
IlcVertexer* IlcTARGETReconstructor::CreateVertexer(IlcRunLoader* /*runLoader*/) const
{
// create a TARGET vertexer

  TString selectedVertexer = GetOption();
  if(selectedVertexer.Contains("ions") || selectedVertexer.Contains("IONS")){
    Info("CreateVertexer","a IlcTARGETVertexerIons object has been selected\n");
    return new IlcTARGETVertexerIons("null");
  }
  if(selectedVertexer.Contains("smear") || selectedVertexer.Contains("SMEAR")){
    Double_t smear[3]={0.005,0.005,0.01};
    Info("CreateVertexer","a IlcTARGETVertexerFast object has been selected\n"); 
    return new IlcTARGETVertexerFast(smear);
  }
  if(selectedVertexer.Contains("ppz") || selectedVertexer.Contains("PPZ")){
    Info("CreateVertexer","a IlcTARGETVertexerPPZ object has been selected\n");
    return new IlcTARGETVertexerPPZ("null");
  }
  // by default an IlcTARGETVertexerZ object is instatiated
  Info("CreateVertexer","a IlcTARGETVertexerZ object has been selected\n");
  return new IlcTARGETVertexerZ("null");
}

//_____________________________________________________________________________
void IlcTARGETReconstructor::FillESD(IlcRunLoader* runLoader, 
				  IlcESDEvent* esd) const
{
// make PID, find V0s and cascade
  IlcTARGETLoader *loader = (IlcTARGETLoader*)runLoader->GetLoader("TARGETLoader");
  IlcTARGETpidESD *pidESD = 0;
  TString selectedPIDmethod = GetOption();
  if(selectedPIDmethod.Contains("LandauFitPID")){
    Info("FillESD","TARGET LandauFitPID option has been selected\n");
    pidESD=loader->GetTARGETpid();
  }
  else{
    Info("FillESD","TARGET default PID\n");
    pidESD=loader->GetTARGETpid();
  }
  if(pidESD!=0){
    pidESD->MakePID(esd);
  }
  else {
    Error("FillESD","!! cannot do the PID !!\n");
  }
  // V0 finding
  Double_t cuts[]={33,  // max. allowed chi2
		   0.0058,// min. allowed negative daughter's impact parameter
		   0.0058,// min. allowed positive daughter's impact parameter
		   6.626,// max. allowed DCA between the daughter tracks
		   0.99,// max. allowed cosine of V0's pointing angle
		   0.2,  // min. radius of the fiducial volume
		   999   // max. radius of the fiducial volume
  };

  IlcV0vertexer vtxer;
  vtxer.SetCuts(cuts);
// // // //   Double_t vtx[3], cvtx[6];
// // // //   esd->GetVertex()->GetXYZ(vtx);
// // // //   esd->GetVertex()->GetSigmaXYZ(cvtx);
// // // //   vtxer.SetVertex(vtx);
  vtxer.Tracks2V0vertices(esd);

  // cascade finding
  Double_t cts[]={33.,    // max. allowed chi2
		  0.0,   // min. allowed V0 impact parameter
		  0.008,  // window around the Lambda mass
		  0.0,  // min. allowed bachelor's impact parameter
		  8.0,   // max. allowed DCA between a V0 and a track
		  0.98, //max. allowed cosine of the cascade pointing angle
		  0.2,    // min. radius of the fiducial volume
		  999     // max. radius of the fiducial volume
  };

  IlcCascadeVertexer cvtxer;
  cvtxer.SetCuts(cts);
// // // //   cvtxer.SetVertex(vtx);
  cvtxer.V0sTracks2CascadeVertices(esd);
}


//_____________________________________________________________________________
IlcTARGETgeom* IlcTARGETReconstructor::GetTARGETgeom(IlcRunLoader* runLoader) const
{
// get the TARGET geometry

  if (!runLoader->GetIlcRun()) runLoader->LoadgIlc();
  if (!runLoader->GetIlcRun()) {
    Error("GetTARGETgeom", "couldn't get IlcRun object");
    return NULL;
  }
  IlcTARGETLoader *loader = (IlcTARGETLoader*)runLoader->GetLoader("TARGETLoader");
  IlcTARGETgeom* geom = (IlcTARGETgeom*)loader->GetTARGETgeom();
  if(!geom){
    Error("GetTARGETgeom","no TARGET geometry available");
    return NULL;
  }
  
  return geom;
}

