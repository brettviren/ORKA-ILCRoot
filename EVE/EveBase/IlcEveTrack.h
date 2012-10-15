// $Id: IlcEveTrack.h 37913 2009-12-15 13:45:53Z mtadel $
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

#ifndef IlcEveTrack_H
#define IlcEveTrack_H

#include <TEveTrack.h>

class IlcExternalTrackParam;
class IlcESDtrack;
class IlcAODTrack;

//______________________________________________________________________________
// Short description of IlcEveTrack
//

class IlcEveTrack : public TEveTrack
{
public:
  IlcEveTrack();
  IlcEveTrack(TParticle* t, Int_t label, TEveTrackPropagator* prop=0);
  IlcEveTrack(TEveMCTrack*  t, TEveTrackPropagator* prop=0);
  IlcEveTrack(TEveRecTrack* t, TEveTrackPropagator* prop=0);
  IlcEveTrack(IlcESDtrack*  t, TEveTrackPropagator* prop=0);
  IlcEveTrack(IlcAODTrack*  t, TEveTrackPropagator* prop=0);
  IlcEveTrack(const IlcEveTrack& t);
  virtual ~IlcEveTrack();

  void SetStartParams(const IlcExternalTrackParam* tp);

  void ImportHits();      // *MENU*
  
  void ImportClustersFromLabel(); // *MENU*
  void ImportClustersFromIndex(); // *MENU*

  void ImportKine();              // *MENU*
  void ImportKineWithArgs(Bool_t importMother=kTRUE, Bool_t impDaugters=kTRUE,
			  Bool_t colorPdg    =kTRUE, Bool_t recurse    =kTRUE); // *MENU*
  void PrintKineStack();          // *MENU*

  virtual void SecSelected(TEveTrack*);        // *SIGNAL*
  virtual void SecSelectedTrack(IlcEveTrack*); // *SIGNAL*

  IlcESDtrack* GetESDTrack() const;
  IlcAODTrack* GetAODTrack() const;

protected:

private:
  IlcEveTrack& operator=(const IlcEveTrack&); // Not implemented

  ClassDef(IlcEveTrack, 0); // Short description.
};

#endif
