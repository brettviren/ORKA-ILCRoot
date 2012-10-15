/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
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

/* $Id: IlcHoughFilter.cxx 32401 2009-05-15 14:01:03Z cvetan $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// high level filter algorithm for TPC using a hough transformation          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TStopwatch.h>

#include "IlcHLTStandardIncludes.h"
#include "IlcHLTLogging.h"
#include "IlcHLTTransform.h"
#include "IlcHLTHough.h"
#include "IlcLog.h"
#include <IlcHLTITSclusterer.h>
#include <IlcHLTITSVertexerZ.h>
#include <IlcHLTITStracker.h>

#include "IlcHoughFilter.h"

#include "IlcRawReaderRoot.h"
#include <IlcMagF.h>
#include <IlcKalmanTrack.h>
#include <IlcITSgeom.h>
#include <IlcESDVertex.h>

ClassImp(IlcHoughFilter)

//_____________________________________________________________________________
IlcHoughFilter::IlcHoughFilter():
fPtmin(0),
fITSgeom(NULL)
{
// default constructor

  // Init debug level
  IlcHLTLog::fgLevel = IlcHLTLog::kError;
  if (IlcDebugLevel() > 0) IlcHLTLog::fgLevel = IlcHLTLog::kWarning;
  if (IlcDebugLevel() > 1) IlcHLTLog::fgLevel = IlcHLTLog::kInformational;
  if (IlcDebugLevel() > 2) IlcHLTLog::fgLevel = IlcHLTLog::kDebug;

  // Init TPC HLT geometry
  const char *path = gSystem->Getenv("ILC_ROOT");
  Char_t pathname[1024];
  strcpy(pathname,path);
  strcat(pathname,"/HLT/src");
  if (!IlcHLTTransform::Init(pathname, kFALSE))
    IlcError("HLT initialization failed!");

  // Init magnetic field
  IlcMagF* field = (IlcMagF*)TGeoGlobalMagField::Instance();
  IlcTracker::SetFieldMap(field,kTRUE);
  fPtmin = 0.1*IlcHLTTransform::GetSolenoidField();

  // Init ITS geometry
  fITSgeom = new IlcITSgeom();
  fITSgeom->ReadNewFile("$ILC_ROOT/ITS/ITSgeometry_vPPRasymmFMD.det");
  if (!fITSgeom)
    IlcError("ITS geometry not found!");

  // Init PID
  IlcPID pid;
}

//_____________________________________________________________________________
Bool_t IlcHoughFilter::Filter(IlcRawVEvent* event, IlcESDEvent* esd)
{
  // Run fast online reconstruction
  // based on the HLT tracking algorithms

  TStopwatch globaltimer;
  globaltimer.Start();

  TStopwatch timer;
  timer.Start();

  TTree *treeITSclusters = new TTree("TreeL3ITSclusters"," "); //make a tree
  treeITSclusters->SetDirectory(0);

  RunITSclusterer(event,treeITSclusters);
  RunITSvertexer(esd,treeITSclusters);
  RunTPCtracking(event,esd);
  RunITStracking(esd,treeITSclusters);
  delete treeITSclusters;

  IlcInfo(Form("Event filter has finished in %f seconds\n\n\n\n\n\n",globaltimer.RealTime()));

  return kTRUE;
}

//_____________________________________________________________________________
void IlcHoughFilter::RunITSclusterer(IlcRawVEvent* event, TTree *treeClusters)
{
  // Run ITS Clusterer
  // The clusters are stored in a tree

  TStopwatch timer;
  timer.Start();

  IlcHLTITSclusterer clusterer(0);
  IlcRawReader *itsrawreader=new IlcRawReaderRoot(event);
  clusterer.Digits2Clusters(itsrawreader,treeClusters);
  delete itsrawreader;
  IlcInfo(Form("ITS clusterer has finished in %f seconds\n",timer.RealTime()));

}


//_____________________________________________________________________________
void IlcHoughFilter::RunITSvertexer(IlcESDEvent* esd, TTree *treeClusters)
{
  // Run SPD vertexerZ
  // Store the result in the ESD

  TStopwatch timer;
  timer.Start();

  IlcHLTITSVertexerZ vertexer;
  IlcESDVertex *vertex = vertexer.FindVertexForCurrentEvent(fITSgeom,treeClusters);
  esd->SetVertex(vertex);
  IlcInfo(Form("ITS vertexer has finished in %f seconds\n",timer.RealTime()));

}

//_____________________________________________________________________________
void IlcHoughFilter::RunTPCtracking(IlcRawVEvent* event, IlcESDEvent* esd)
{
  // Run hough transform tracking in TPC
  // The z of the vertex is taken from the ESD
  // The result of the tracking is stored in the ESD
  TStopwatch timer;
  timer.Start();

  const IlcESDVertex *vertex = esd->GetVertex();
  Float_t zvertex = vertex->GetZv();

  IlcHLTHough *hough1 = new IlcHLTHough();
    
  hough1->SetThreshold(4);
  hough1->CalcTransformerParams(fPtmin);
  hough1->SetPeakThreshold(70,-1);
  hough1->Init(100,4,event,zvertex);
  hough1->SetAddHistograms();

  IlcHLTHough *hough2 = new IlcHLTHough();
  
  hough2->SetThreshold(4);
  hough2->CalcTransformerParams(fPtmin);
  hough2->SetPeakThreshold(70,-1);
  hough2->Init(100,4,event,zvertex);
  hough2->SetAddHistograms();

  Int_t nglobaltracks = 0;
  /* In case we run HLT code in 2 threads */
  hough1->StartProcessInThread(0,17);
  hough2->StartProcessInThread(18,35);

  if(hough1->WaitForThreadFinish())
    ::Fatal("IlcHLTHough::WaitForThreadFinish"," Can not join the required thread! ");
  if(hough2->WaitForThreadFinish())
    ::Fatal("IlcHLTHough::WaitForThreadFinish"," Can not join the required thread! ");

    /* In case we run HLT code in the main thread
    for(Int_t slice=0; slice<=17; slice++)
      {
	hough1->ReadData(slice,0);
	hough1->Transform();
	hough1->AddAllHistogramsRows();
	hough1->FindTrackCandidatesRow();
	hough1->AddTracks();
      }
    for(Int_t slice=18; slice<=35; slice++)
      {
	hough2->ReadData(slice,0);
	hough2->Transform();
	hough2->AddAllHistogramsRows();
	hough2->FindTrackCandidatesRow();
	hough2->AddTracks();
      }
    */

  nglobaltracks += hough1->FillESD(esd);
  nglobaltracks += hough2->FillESD(esd);

    /* In case we want to debug the ESD
    gSystem->MakeDirectory("hough1");
    hough1->WriteTracks("./hough1");
    gSystem->MakeDirectory("hough2");
    hough2->WriteTracks("./hough2");
    */

  delete hough1;
  delete hough2;

  IlcInfo(Form("\nHough Transformer has found %d TPC tracks in %f seconds\n\n", nglobaltracks,timer.RealTime()));

}

//_____________________________________________________________________________
void IlcHoughFilter::RunITStracking(IlcESDEvent* esd, TTree *treeClusters)
{
  // Run the ITS tracker
  // The tracks from the HT TPC tracking are used as seeds
  // The prologated tracks are updated in the ESD
  TStopwatch timer;
  timer.Start();

  Double_t vtxPos[3];
  Double_t vtxErr[3]={0.005,0.005,0.010};
  const IlcESDVertex *vertex = esd->GetVertex();
  vertex->GetXYZ(vtxPos);

  IlcHLTITStracker itsTracker(0);
  itsTracker.SetVertex(vtxPos,vtxErr);

  itsTracker.LoadClusters(treeClusters);
  itsTracker.Clusters2Tracks(esd);
  itsTracker.UnloadClusters();
  IlcInfo(Form("ITS tracker has finished in %f seconds\n",timer.RealTime()));

}
