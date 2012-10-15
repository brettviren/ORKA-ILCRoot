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

/* $Id: IlcVEventHandler.cxx 58336 2012-08-28 15:07:04Z hristov $ */

//-------------------------------------------------------------------------
//     Event IO handler base class
//     Author: Andreas Morsch, CERN
//-------------------------------------------------------------------------

#include "IlcVEventHandler.h"

ClassImp(IlcVEventHandler)

//______________________________________________________________________________
IlcVEventHandler::IlcVEventHandler() :
  TNamed()
{
  // default constructor
}

//______________________________________________________________________________
IlcVEventHandler::~IlcVEventHandler() 
{
// destructor
}

//______________________________________________________________________________
IlcVEventHandler::IlcVEventHandler(const char* name, const char* title):
    TNamed(name, title)
{
}
//______________________________________________________________________________
void IlcVEventHandler::Lock()
{
// Security lock. This is to detect NORMAL user errors and not really to
// protect against intentional hacks.
   if (IsLocked()) return;
   TObject::SetBit(kHandlerLocked, kTRUE);
}

//______________________________________________________________________________
void IlcVEventHandler::UnLock()
{
// Verbose unlocking. Hackers will be punished ;-) ... 
   if (!IsLocked()) return;
   TObject::SetBit(kHandlerLocked, kFALSE);
}

//______________________________________________________________________________
void IlcVEventHandler::Changed()
{
// All critical setters pass through the Changed method that throws an exception 
// in case the lock was set.
   if (IsLocked()) Fatal("Changed","Critical setter of a handler called in locked mode");
}
