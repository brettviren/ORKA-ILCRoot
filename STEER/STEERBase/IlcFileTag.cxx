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

/* $Id: IlcFileTag.cxx 14745 2006-08-04 15:48:44Z panos $ */

//-----------------------------------------------------------------
//           Implementation of the FileTag class
//   This is the class to deal with the tags in the file level
//   Origin: Adam Kisiel, CERN, Adam.Kisiel@cern.ch
//-----------------------------------------------------------------

#include "IlcFileTag.h"
#include <stdlib.h>

ClassImp(IlcFileTag)

//___________________________________________________________________________
IlcFileTag::IlcFileTag() : 
  TObject(),  
  fGUID(""),
  fPath(""),
  fsize(0),
  fmd5(""),
  fturl(""),
  fEventTags(1000)
{
  // IlcFileTag default constructor
  
}

IlcFileTag::IlcFileTag(const IlcFileTag &tag):
  TObject(tag),
  fGUID(tag.fGUID),
  fPath(tag.fPath),
  fsize(tag.fsize),
  fmd5(tag.fmd5),
  fturl(tag.fturl),
  fEventTags(10000)
{
  for (int iev=0; iev<tag.GetNEvents(); iev++)
    AddEventTag(*(tag.GetEventTag(iev)));
}

IlcFileTag &IlcFileTag::operator=(const IlcFileTag &tag)
{
  if (this != &tag) {
    TObject::operator=(tag);

    SetGUID(tag.GetGUID());
    SetPath(tag.GetPath());
    SetSize(tag.GetSize());
    SetMD5(tag.GetMD5());
    SetTURL(tag.GetTURL());
    
    for (int iev=0; iev<tag.GetNEvents(); iev++)
      AddEventTag(*(tag.GetEventTag(iev)));
  }

  return *this;
}

//___________________________________________________________________________
IlcFileTag::~IlcFileTag() {
  // IlcEventTag destructor
  //  fEventTag.Delete();
  fEventTags.Delete();
}

//___________________________________________________________________________
void IlcFileTag::AddEventTag(const IlcEventTag & EvTag) {
  //Adds an entry to the event tag TClonesArray

  fEventTags.Add(new IlcEventTag(EvTag));
}

void IlcFileTag::CopyFileInfo(const IlcFileTag &tag)
{
  SetGUID(tag.GetGUID());
  SetPath(tag.GetPath());
  SetSize(tag.GetSize());
  SetMD5(tag.GetMD5());
  SetTURL(tag.GetTURL());
}
