// $Id: IlcEveTrackFitter.cxx 47044 2011-02-03 10:46:29Z fca $
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

#include "IlcEveTrackFitter.h"
#include "IlcEveTrack.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TQObject.h"

#include "IlcRieman.h"
#include "IlcExternalTrackParam.h"

#include <TEveTrackPropagator.h>
#include <TEveVSDStructs.h>
#include <TEveManager.h>


//==============================================================================
//==============================================================================
// IlcEveTrackFitter
//==============================================================================

//______________________________________________________________________________
//
// IlcEveTrackFitter is an interface to TEvePointSet allowing IlcRieman fit.
// It builds a list of points by listening to selection signal of any
// object of type TEvePointSet. After selection the list is feeded to
// IlcRieman fitter, which returns helix parameters visualized with
// IlcEveTrack.
//

ClassImp(IlcEveTrackFitter)

IlcEveTrackFitter::IlcEveTrackFitter(const Text_t* name, Int_t nPoints) :
    TEvePointSet   (name, nPoints),

    fAlpha         (0),
    fRieman        (0),

    fConnected     (kFALSE),
    fSPMap         (),
    fTrackList     (0),

    fGraphPicked   (0),
    fGraphHelix    (0)
{
  // Constructor.

  SetMarkerColor(3);
  SetOwnIds(kFALSE);

  fTrackList = new TEveTrackList("Tracks");
  fTrackList->IncDenyDestroy();
  fTrackList->SetLineWidth(2);
  fTrackList->SetLineColor(8);
  fTrackList->GetPropagator()->SetEditPathMarks(kTRUE);
  AddElement(fTrackList);

  fGraphPicked = new TGraph();
  fGraphPicked->SetName("Selected points");
  fGraphPicked->SetMarkerColor(4);
  fGraphPicked->SetMarkerStyle(4);
  fGraphPicked->SetMarkerSize(2);

  fGraphHelix = new TGraphErrors();
  fGraphHelix->SetName("Fitted points");
  fGraphHelix->SetMarkerColor(2);
}

IlcEveTrackFitter::~IlcEveTrackFitter()
{
  // Destructor.

  if (fRieman) delete fRieman;

  fTrackList->DecDenyDestroy();
}

/******************************************************************************/

void IlcEveTrackFitter::DestroyElements()
{
  // Virtual method of base class TEveElement.
  // Preserves TEveTrackList object for fitted helices.

  TEveElement::DestroyElements();

  // fTrackList is destroyed because DenyDestroy is set.
  gEve->AddElement(fTrackList, this);
  fTrackList->DestroyElements();
}

/******************************************************************************/

void IlcEveTrackFitter::Start()
{
  // Clear existing point selection and maintain connection to the
  // TEvePointSet signal.

  Reset();
  if (fConnected == kFALSE)
  {
    TQObject::Connect("TEvePointSet", "PointSelected(Int_t)",
		      "IlcEveTrackFitter", this, "AddFitPoint(Int_t)");
    fConnected = kTRUE;
  }
}

void IlcEveTrackFitter::Stop()
{
  // Stop adding points for the fit.

  if (fConnected)
  {
    TQObject::Disconnect("TEvePointSet", "AddFitPoint(Int_t)");
    fConnected = kFALSE;
  }
}

void IlcEveTrackFitter::Reset(Int_t nPoints, Int_t nIntIds)
{
  // Reset selection.

  if (fRieman) fRieman->Reset();
  TEvePointSet::Reset(nPoints, nIntIds);
  fSPMap.clear();
}

/******************************************************************************/

void IlcEveTrackFitter::AddFitPoint(Int_t pointId)
{
  // Add or remove given point depending if exists in the map.

  Float_t x, y, z;

  TEvePointSet* ps = static_cast<TEvePointSet*>((TQObject*) gTQSender);

  PointMap_t::iterator g = fSPMap.find(Point_t(ps, pointId));
  if (g != fSPMap.end())
  {
    Int_t idx = g->second;
    if (idx != fLastPoint)
    {
      GetPoint(fLastPoint, x, y, z);
      SetPoint(idx, x, y, z);
    }
    fSPMap.erase(g);
    --fLastPoint;
  }
  else
  {
    fSPMap[Point_t(ps, pointId)] = Size();
    ps->GetPoint(pointId, x, y, z);
    SetNextPoint(x, y, z);
  }

  ResetBBox();
  ElementChanged(kTRUE, kTRUE);
}

/******************************************************************************/

void IlcEveTrackFitter::FitTrack()
{
  // Fit selected points with IlcRieman fitter.

  using namespace TMath;

  if (fRieman) delete fRieman;
  fRieman = new IlcRieman(Size());

  Float_t x, y, z;
  Int_t alphaIdx = 0;
  GetPoint(alphaIdx, x, y, z);
  Float_t minR2=x*x + y*y;
  for (Int_t i=0; i<=fLastPoint; i++)
  {
    GetPoint(i, x, y, z);
    Float_t cR2 = x*x + y*y;
    if (minR2 > cR2)
    {
      minR2 = cR2;
      alphaIdx = i;
    }
  }
  GetPoint(alphaIdx, x, y, z);
  fAlpha = ATan2(y, x);
  Float_t sin = Sin(-fAlpha);
  Float_t cos = Cos(-fAlpha);
  for (Int_t i = 0; i <= fLastPoint; ++i)
  {
    GetPoint(i, x, y, z);
    fRieman->AddPoint(cos*x - sin*y, cos*y + sin*x, z, 1, 1);
  }
  fRieman->Update();

  Double_t r = Sqrt(minR2);
  Double_t param[5];
  Double_t cov[15];
  fRieman->GetExternalParameters(r, param, cov);
  // curvature to pt
  param[4] /= TEveTrackPropagator::fgDefMagField*TEveTrackPropagator::fgkB2C;
  // sign in tang
  if (param[4] < 0) param[3] = -param[3];
  IlcExternalTrackParam trackParam(r, fAlpha, param, cov);
  trackParam.Print();

  // make track
  Double_t v0[3];
  trackParam.GetXYZAt(r, TEveTrackPropagator::fgDefMagField, v0);
  Double_t p0[3];
  trackParam.GetPxPyPzAt(r, TEveTrackPropagator::fgDefMagField, p0);
  TEveRecTrack rc;
  rc.fV.Set(v0);
  rc.fP.Set(p0);
  rc.fSign = trackParam.Charge();

  IlcEveTrack* track = new IlcEveTrack(&rc, fTrackList->GetPropagator());
  track->SetName(Form("track %f", fAlpha));

  track->MakeTrack();
  track->SetAttLineAttMarker(fTrackList);
  fTrackList->AddElement(track);
}


/******************************************************************************/

void IlcEveTrackFitter::DrawDebugGraph()
{
  // Draw graph of picked points and helix points.

  static const TEveException kEH("IlcEveTrackFitter::DrawRiemanGraph ");

  if (fRieman == 0)
    throw(kEH + "fitter not set.");

  Int_t nR = fRieman->GetN();
  fGraphPicked->Set(nR);
  fGraphHelix->Set(nR);

  Double_t* x  =  fRieman->GetX();
  Double_t* y  =  fRieman->GetY();
  Double_t* sy =  fRieman->GetSy();
  for (Int_t i=0; i<nR; i++)
  {
    fGraphPicked->SetPoint(i, x[i], y[i]);
    fGraphHelix->SetPoint (i, x[i], fRieman->GetYat(x[i]));
    fGraphHelix->SetPointError(i, 0.1, sy[i]); // now faked
  }

  if (gPad)
    gPad->Clear();

  fGraphPicked->Draw("AP");
  fGraphHelix->Draw("SAME P");
  gPad->GetCanvas()->SetTitle(Form("IlcRieman alpha: %f", fAlpha));
  gPad->Modified();
  gPad->Update();
}
