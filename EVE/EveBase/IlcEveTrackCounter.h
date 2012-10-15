// @(#)root/eve:$Id: IlcEveTrackCounter.h 36489 2009-11-06 20:32:19Z mtadel $
// Author: Matevz Tadel 2007

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

#ifndef IlcEveTrackCounter_H
#define IlcEveTrackCounter_H

#include "TEveElement.h"
#include "TList.h"

class IlcEveTrack;
class IlcEveTracklet;
class TEveTrackList;

//______________________________________________________________________________
// Short description of IlcEveTrackCounter
//

class IlcEveTrackCounter :  public TEveElement, public TNamed
{
  friend class IlcEveTrackCounterEditor;

public:
  enum EClickAction_e { kCA_PrintTrackInfo, kCA_ToggleTrack };

  IlcEveTrackCounter(const Text_t* name="IlcEveTrackCounter", const Text_t* title="");
  virtual ~IlcEveTrackCounter();

  Int_t GetEventId() const { return fEventId; }
  void  SetEventId(Int_t id) { fEventId = id; }

  void Reset();

  void RegisterTracks(TEveTrackList* tlist, Bool_t goodTracks);
  void RegisterTracklets(TEveTrackList* tlist, Bool_t goodTracks);

  void DoTrackAction(IlcEveTrack* track);
  void DoTrackletAction(IlcEveTracklet* track);

  Int_t GetClickAction() const  { return fClickAction; }
  void  SetClickAction(Int_t a) { fClickAction = a; }

  void OutputEventTracks();
  void PrintEventTracks();

  Bool_t GetActive() const { return fActive; }
  void   SetActive(Bool_t a) { fActive = a; }

  static IlcEveTrackCounter* fgInstance;

  static Bool_t IsActive();

protected:
  Int_t fBadLineStyle;  // TEveLine-style used for secondary/bad tracks.
  Int_t fClickAction;   // Action to take when a track is ctrl-clicked.

  Int_t fEventId;       // Current event-id.

  Int_t fAllTracks;     // Counter of all tracks.
  Int_t fGoodTracks;    // Counter of good tracks.
  Int_t fAllTracklets;  // Counter of all tracklets.
  Int_t fGoodTracklets; // Counter of good tracklets.

  TList fTrackLists;    // List of track-lists registered for management.
  TList fTrackletLists; // List of tracklet-lists registered for management.

  Bool_t fActive;

private:
  IlcEveTrackCounter(const IlcEveTrackCounter&);            // Not implemented
  IlcEveTrackCounter& operator=(const IlcEveTrackCounter&); // Not implemented

  ClassDef(IlcEveTrackCounter, 0); // Class for selection of good/primary tracks with basic processing functionality.
};

#endif
