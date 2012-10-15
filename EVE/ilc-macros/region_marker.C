// $Id: region_marker.C 54257 2012-01-30 20:52:05Z quark $
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
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TPolyMarker3D.h>
#include <TEveManager.h>
#include <TEveElement.h>
#include <TEveUtil.h>
#endif


void region_marker(Float_t a=10, Float_t b=10, Float_t c=20,
		   Float_t x=0, Float_t y=0, Float_t z=0)
{
  TPolyMarker3D* mark = new TPolyMarker3D(8);
  mark->SetName("Origin marker");
  mark->SetMarkerStyle(3);
  mark->SetMarkerColor(6);
  mark->SetPoint(0, x+a, y+b, z+c);
  mark->SetPoint(1, x+a, y-b, z+c);
  mark->SetPoint(2, x-a, y-b, z+c);
  mark->SetPoint(3, x-a, y+b, z+c);

  mark->SetPoint(4, x+a, y+b, z-c);
  mark->SetPoint(5, x+a, y-b, z-c);
  mark->SetPoint(6, x-a, y+b, z-c);
  mark->SetPoint(7, x-a, y-b, z-c);
  Color_t* colp = TEveUtil::FindColorVar(mark, "fMarkerColor");
  TEveElementObjectPtr* rnrEl = new TEveElementObjectPtr(mark, *colp);
  gEve->AddGlobalElement(rnrEl);
  gEve->Redraw3D();
}
