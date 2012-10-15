// $Id: IlcEveTrack.cxx 47349 2011-02-13 17:26:41Z mtadel $
// Author: Matevz Tadel 2009

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

#include "IlcEveTrack.h"

#include "IlcESDtrack.h"
#include "IlcAODTrack.h"

#include <TROOT.h>
#include <TMath.h>

//______________________________________________________________________________
// Full description of IlcEveTrack
//

ClassImp(IlcEveTrack)

//______________________________________________________________________________
IlcEveTrack::IlcEveTrack() :
  TEveTrack()
{
  // Constructor.
}

//______________________________________________________________________________
IlcEveTrack::IlcEveTrack(TParticle* t, Int_t label, TEveTrackPropagator* prop) :
  TEveTrack(t, label, prop)
{
  // Constructor.
}

//______________________________________________________________________________
IlcEveTrack::IlcEveTrack(TEveMCTrack*  t, TEveTrackPropagator* prop) :
  TEveTrack(t, prop)
{
  // Constructor.
}

//______________________________________________________________________________
IlcEveTrack::IlcEveTrack(TEveRecTrack* t, TEveTrackPropagator* prop) :
  TEveTrack(t, prop)
{
  // Constructor.
}

//______________________________________________________________________________
IlcEveTrack::IlcEveTrack(IlcESDtrack* t, TEveTrackPropagator* prop) :
  TEveTrack()
{
  // Constructor.

  Double_t buf[3];
  t->GetXYZ(buf);    fV.Set(buf);
  t->GetPxPyPz(buf); fP.Set(buf);

  Double_t ep = t->GetP(), mc = t->GetMass();
  fBeta = ep/TMath::Sqrt(ep*ep + mc*mc);
  // fPdg = 0; // ??? Use PID ?
  fCharge = TMath::Nint(t->GetSign());
  
  fLabel = t->GetLabel();
  fIndex = t->GetID();
  // fStatus = (Int_t) t->GetStatus(); // RRRR Uncomment for root-5.26.

  SetPropagator(prop);
}

//______________________________________________________________________________
IlcEveTrack::IlcEveTrack(IlcAODTrack* t, TEveTrackPropagator* prop) :
  TEveTrack()
{
  // Constructor.

  Double_t buf[3];

  t->GetXYZ(buf); fV.Set(buf);
  t->PxPyPz(buf); fP.Set(buf);

  // fBeta = 0; // Unknown, no mass function
  // fPdg = 0;  // ??? Use PID ?
  fCharge= t->Charge();
  
  fLabel = t->GetLabel();
  fIndex = t->GetID();
  // fStatus = (Int_t) t->GetStatus(); // RRRR Uncomment for root-5.26.

  SetPropagator(prop);
}

//______________________________________________________________________________
IlcEveTrack::IlcEveTrack(const IlcEveTrack& t) :
  TEveTrack(t)
{
  // Copy constructor.
}

//______________________________________________________________________________
IlcEveTrack::~IlcEveTrack()
{
  // Destructor.
}

//______________________________________________________________________________
void IlcEveTrack::SetStartParams(const IlcExternalTrackParam* tp)
{
  // Set the initial vertex / momentum of eve track from 'tp'.

  Double_t buf[3];

  tp->GetXYZ(buf); fV.Set(buf);
  tp->PxPyPz(buf); fP.Set(buf);
}

/******************************************************************************/

//______________________________________________________________________________
void IlcEveTrack::ImportHits()
{
  // Import hits with same label as the track.
  // Uses macro "hits_from_label.C".

  TEveUtil::LoadMacro("hits_from_label.C");
  gROOT->ProcessLine(Form("hits_from_label(%d, (TEveElement*)%p);",
                          fLabel, this));
}

//______________________________________________________________________________
void IlcEveTrack::ImportClustersFromLabel()
{
  // Import clusters with same label as the track.
  // Uses macro "clusters_from_label.C".

  TEveUtil::LoadMacro("clusters_from_label.C");
  gROOT->ProcessLine(Form("clusters_from_label(%d, (TEveElement*)%p);",
                          fLabel, this));
}

//______________________________________________________________________________
void IlcEveTrack::ImportClustersFromIndex()
{
  // Import clusters marked with same reconstructed track index as the track.
  // Uses macro "clusters_from_index.C".

  static const TEveException kEH("IlcEveTrack::ImportClustersFromIndex ");

  if (fIndex == kMinInt)
    throw kEH + "index not set.";

  TEveUtil::LoadMacro("clusters_from_index.C");
  gROOT->ProcessLine(Form("clusters_from_index(%d, (TEveElement*)%p);",
                          fIndex, this));
}

/******************************************************************************/

//______________________________________________________________________________
void IlcEveTrack::ImportKine()
{
   // Import kinematics of the track's label recursively.
   // Uses macro "kine_tracks.C".

   static const TEveException kEH("IlcEveTrack::ImportKine ");

   if (fLabel == kMinInt)
      throw kEH + "label not set.";

   Int_t label;
   if (fLabel < 0) {
      Warning(kEH, "label negative, taking absolute value.");
      label = -fLabel;
   } else {
      label = fLabel;
   }

   TEveUtil::LoadMacro("kine_tracks.C");
   gROOT->ProcessLine(Form("kine_track(%d, kTRUE, kTRUE, kTRUE, kTRUE, (TEveElement*)%p);",
                           label, this));

}

//______________________________________________________________________________
void IlcEveTrack::ImportKineWithArgs(Bool_t importMother, Bool_t importDaugters,
                                     Bool_t colorPdg,     Bool_t recurse)
{
  // Import kinematics of the track's label. Arguments steer the
  // import process:
  //   importMother     import particle with track's label
  //   importDaugters   import direct daughters of label
  //   colorPdg         color kinematics by PDG code
  //   recurse          recursive import of daughters' daughters
  // Uses macro "kine_tracks.C".

  static const TEveException kEH("IlcEveTrack::ImportKineWithArgs ");

  if (fLabel == kMinInt)
    throw kEH + "label not set.";

  Int_t label;
  if (fLabel < 0) {
    Warning(kEH, "label negative, taking absolute value.");
    label = -fLabel;
  } else {
    label = fLabel;
  }

  TEveUtil::LoadMacro("kine_tracks.C");
  gROOT->ProcessLine(Form("kine_track(%d, %d, %d, %d, %d, (TEveElement*)%p);",
                          label, importMother, importDaugters, colorPdg, recurse, this));
}

//______________________________________________________________________________
void IlcEveTrack::PrintKineStack()
{
  // Print kinematics pertaining to track's label.
  // Uses macro "print_kine_from_label.C".

  static const TEveException kEH("IlcEveTrack::PrintKineStack ");

  if (fLabel == kMinInt)
    throw kEH + "label not set.";

  Int_t label;
  if (fLabel < 0) {
    Warning(kEH, "label negative, taking absolute value.");
    label = -fLabel;
  } else {
    label = fLabel;
  }

  TEveUtil::LoadMacro("print_kine_from_label.C");
  gROOT->ProcessLine(Form("print_kine_from_label(%d);", label));
}

//______________________________________________________________________________
void IlcEveTrack::SecSelected(TEveTrack* track)
{
  // Emits "SecSelected(TEveTrack*)" signal.
  // Called from TEveTrackGL on secondary-selection.

  Emit("SecSelected(TEveTrack*)", (Long_t)track);
  SecSelectedTrack((IlcEveTrack*) track);
}

//______________________________________________________________________________
void IlcEveTrack::SecSelectedTrack(IlcEveTrack* track)
{
  // Emits "SecSelectedTrack(IlcEveTrack*)" signal.

  Emit("SecSelectedTrack(IlcEveTrack*)", (Long_t)track);
}

//______________________________________________________________________________
IlcESDtrack* IlcEveTrack::GetESDTrack() const
{
  // Return source object dyn-casted to IlcESDtrack.

  return dynamic_cast<IlcESDtrack*>(GetSourceObject());
}

//______________________________________________________________________________
IlcAODTrack* IlcEveTrack::GetAODTrack() const
{
  // Return source object dyn-casted to IlcAODTrack.

  return dynamic_cast<IlcAODTrack*>(GetSourceObject());
}
