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

//
// Select points from TEvePointSet(clusters, hits, etc.) with Alt+mouse-left
// click action.
//
// In IlcEvetrackFitEditor press "Fit" button to make track fit on the
// selected points. To fit new track, press "Reset".

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TEveManager.h>
#include <TEveSelection.h>

#include <IlcEveTrackFitter.h>
#include <IlcEveCosmicRayFitter.h>
#endif

void track_fitter(Int_t mode = 1)
{
  gEve->GetSelection()->SetPickToSelect(1);
  gEve->GetHighlight()->SetPickToSelect(0);

  if (mode == 0)
  {
    // helix fit
    IlcEveTrackFitter* t = new IlcEveTrackFitter();
    gEve->AddElement(t);
    t->Start();
  }
  else
  {
    // straight line fit
    IlcEveCosmicRayFitter* t = new IlcEveCosmicRayFitter();
    gEve->AddElement(t);
    t->Start();
  }
}
