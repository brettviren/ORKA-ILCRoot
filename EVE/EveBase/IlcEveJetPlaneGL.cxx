// $Id: IlcEveJetPlaneGL.cxx 31253 2009-03-02 10:29:56Z mtadel $
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

#include "IlcEveJetPlaneGL.h"
#include "IlcEveJetPlane.h"

#include <TGLRnrCtx.h>
#include <TGLSelectRecord.h>
#include <TGLIncludes.h>

#include "TGLUtil.h"
#include "TGLAxis.h"

#include <TColor.h>
#include <TStyle.h>
#include <TROOT.h>

//==============================================================================
//==============================================================================
// IlcEveJetPlaneGL
//==============================================================================

//______________________________________________________________________________
//
// GL renderer for IlcEveJetPlane.

ClassImp(IlcEveJetPlaneGL)

IlcEveJetPlaneGL::IlcEveJetPlaneGL() : TGLObject(), fM(0)
{
  // Constructor.

  fDLCache = kFALSE; // Disable display list -- axis pain.
}

/******************************************************************************/

Bool_t IlcEveJetPlaneGL::SetModel(TObject* obj, const Option_t* /*opt*/)
{
  // Set model object.

  if (SetModelCheckClass(obj, IlcEveJetPlane::Class())) {
    fM = dynamic_cast<IlcEveJetPlane*>(obj);
    return kTRUE;
  }
  return kFALSE;
}

void IlcEveJetPlaneGL::SetBBox()
{
  // Set bounding box.

  SetAxisAlignedBBox(((IlcEveJetPlane*)fExternalObj)->AssertBBox());
}

/******************************************************************************/

void IlcEveJetPlaneGL::DirectDraw(TGLRnrCtx& rnrCtx) const
{
  // Render the object.

  Float_t minEta = (fM->fMinEta)*(fM->fEtaScale);
  Float_t maxEta = (fM->fMaxEta)*(fM->fEtaScale);
  Float_t minPhi = (fM->fMinPhi)*(fM->fPhiScale) - 350;
  Float_t maxPhi = (fM->fMaxPhi)*(fM->fPhiScale) - 350;
  Float_t phiCoord, etaCoord, dPhi, dEta;

  // Show frame for Eta-Phi coordinates

  glBegin(GL_LINE_LOOP);
  glVertex3f(minEta, minPhi, 0);
  glVertex3f(maxEta, minPhi, 0);
  glVertex3f(maxEta, maxPhi, 0);
  glVertex3f(minEta, maxPhi, 0);
  glEnd();

  if (rnrCtx.Selection() == kFALSE && rnrCtx.Highlight() == kFALSE)
  {

    // Show grid in Eta-Phi coordinates

    dPhi = (maxPhi-minPhi)/(fM->fNPhiDiv - 1);
    dEta = (maxEta-minEta)/(fM->fNEtaDiv - 1);

    for (Int_t count = 1; count < fM->fNPhiDiv-1; ++count)
    {
      phiCoord = minPhi + count*dPhi;
      glBegin(GL_LINES);
      glVertex3f( minEta, phiCoord, 0);
      glVertex3f( maxEta, phiCoord, 0);
      glEnd();
    }

    for (Int_t count = 1; count < fM->fNEtaDiv-1; ++count)
    {
      etaCoord = minEta + count*dEta;
      glBegin(GL_LINES);
      glVertex3f(etaCoord, minPhi, 0);
      glVertex3f(etaCoord, maxPhi, 0);
      glEnd();
    }

    // Show axis tick marks and labels

    {
      TGLCapabilitySwitch lightsOff(GL_LIGHTING, false);

      TGLAxis ap;
      ap.SetLineColor(fM->fGridColor);
      ap.SetTextColor(fM->fGridColor);
      TGLVector3 start, end;

      start.Set(minEta, minPhi, 0);
      end.Set(maxEta, minPhi, 0);
      ap.PaintGLAxis(start.CArr(), end.CArr(), fM->fMinEta, fM->fMaxEta, 205);

      start.Set(maxEta, minPhi, 0);
      end.Set(maxEta, maxPhi, 0);
      ap.PaintGLAxis(start.CArr(), end.CArr(), fM->fMinPhi, fM->fMaxPhi, 205);
    }

  }
}
