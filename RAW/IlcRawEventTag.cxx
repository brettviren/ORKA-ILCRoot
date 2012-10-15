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

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// The IlcRawEventTag class handles the raw-data event-oriented tag         //
// information. One object for each raw-data event is stored in a ROOT      //
// tree inside the file controled by IlcTagDB class.                        //
// For the moment the tag information includes the raw-data event header +  //
// the raw-data file GUID and the event index.                              //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#include "IlcRawEventTag.h"
#include "IlcRawEventHeaderBase.h"

ClassImp(IlcRawEventTag)

//______________________________________________________________________________
IlcRawEventTag::IlcRawEventTag() :
  fHeader(NULL),
  fGUID(""),
  fEvent(-1)
{
  // Default constructor
}

//___________________________________________________________________________
IlcRawEventTag::IlcRawEventTag(const IlcRawEventTag & tag) :
  TObject(tag),
  fHeader(tag.fHeader),
  fGUID(tag.fGUID),
  fEvent(tag.fEvent)
{
  // copy constructor
}

//___________________________________________________________________________
IlcRawEventTag & IlcRawEventTag::operator=(const IlcRawEventTag &tag) {
  // assignment operator
  if (this != &tag) {
    TObject::operator=(tag);

    SetHeader(tag.GetHeader());
    SetGUID(tag.GetGUID());
    SetEventNumber(tag.GetEventNumber());
  }
  return *this;
}
