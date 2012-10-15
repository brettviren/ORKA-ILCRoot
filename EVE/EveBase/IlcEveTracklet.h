// $Id: IlcEveTracklet.h 36365 2009-11-04 11:11:01Z mtadel $
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

#ifndef IlcEveTracklet_H
#define IlcEveTracklet_H

#include "IlcEveTrack.h"

class IlcVVertex;

//______________________________________________________________________________
// IlcEveTracklet is a representation of SPD tracklet.
// It inherits from IlcEveTrack to allow for common functionality
// regarding track counting.

class IlcEveTracklet : public IlcEveTrack
{
public:
  IlcEveTracklet(Int_t index, const IlcVVertex* pv, Float_t theta, Float_t phi, TEveTrackPropagator* prop=0);
  virtual ~IlcEveTracklet() {}

  virtual void MakeTrack(Bool_t recurse=kTRUE);

  virtual void SecSelected(TEveTrack*);              // *SIGNAL*
  virtual void SecSelectedTracklet(IlcEveTracklet*); // *SIGNAL*

  // ----------------------------------------------------------------

  static Float_t GetDefaultRadius();
  static void    SetDefaultRadius(Float_t r);

protected:
  static Float_t fgDefaultRadius;

private:
  IlcEveTracklet(const IlcEveTracklet&);            // Not implemented
  IlcEveTracklet& operator=(const IlcEveTracklet&); // Not implemented

  ClassDef(IlcEveTracklet, 0); // Short description.
};

#endif
