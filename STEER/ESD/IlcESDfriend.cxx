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

//-------------------------------------------------------------------------
//               Implementation of the IlcESDfriend class
//  This class contains some additional to the ESD information like
//  the clusters associated to tracks.
//      Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch
//-------------------------------------------------------------------------

#include "IlcESDfriend.h"
#include "IlcESDVZEROfriend.h"
#include "IlcESDTZEROfriend.h"

ClassImp(IlcESDfriend)

IlcESDfriend::IlcESDfriend(): TObject(), fTracks("IlcESDfriendTrack",1),
  fESDVZEROfriend(NULL),
  fESDTZEROfriend(NULL)

{
 //
 // Default constructor
 //
}

IlcESDfriend::IlcESDfriend(const IlcESDfriend &f) :
  TObject(f),
  fTracks(f.fTracks),
  fESDVZEROfriend(f.fESDVZEROfriend ? new IlcESDVZEROfriend(*f.fESDVZEROfriend) : NULL),
  fESDTZEROfriend(f.fESDTZEROfriend ? new IlcESDTZEROfriend(*f.fESDTZEROfriend) : NULL)
{
 //
 // Copy constructor
 //
}

IlcESDfriend& IlcESDfriend::operator=(const IlcESDfriend& esd)
{
    
    // Assignment operator
    if(&esd == this) return *this;
    TObject::operator=(esd);
    fTracks = esd.fTracks;

    delete fESDVZEROfriend;
    fESDVZEROfriend = new IlcESDVZEROfriend(*esd.fESDVZEROfriend);

    delete fESDTZEROfriend;
    fESDTZEROfriend = new IlcESDTZEROfriend(*esd.fESDTZEROfriend);
 
 
 
    return *this;
}



IlcESDfriend::~IlcESDfriend() {
  //
  // Destructor
  //
  fTracks.Delete();
  delete fESDVZEROfriend;
  delete fESDTZEROfriend;
}



void IlcESDfriend::SetVZEROfriend(IlcESDVZEROfriend * obj)
{
  //
  // Set the VZERO friend data object
  // (complete raw data)
  if (!fESDVZEROfriend) fESDVZEROfriend = new IlcESDVZEROfriend();
  if (obj) *fESDVZEROfriend = *obj;
}
void IlcESDfriend::SetTZEROfriend(IlcESDTZEROfriend * obj)
{
  //
  // Set the TZERO friend data object
  // (complete raw data)
  if (!fESDTZEROfriend) fESDTZEROfriend = new IlcESDTZEROfriend();
  if (obj) *fESDTZEROfriend = *obj;
}
