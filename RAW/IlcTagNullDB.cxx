// @(#) $Id: IlcTagNullDB.cxx 23318 2008-01-14 12:43:28Z hristov $
// Author: Fons Rademakers  26/11/99

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

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcTagNullDB                                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "IlcTagNullDB.h"


ClassImp(IlcTagNullDB)


//______________________________________________________________________________
IlcTagNullDB::IlcTagNullDB(IlcRawEventTag *eventTag) :
   IlcTagDB(eventTag, "/dev/null")
{
   // Create tag db writing to /dev/null.

}

//______________________________________________________________________________
const char *IlcTagNullDB::GetFileName() const
{
   // Return /dev/null as filename.

   return "/dev/null";
}

//______________________________________________________________________________
void IlcTagNullDB::Close()
{
   // Close null tag DB.

   if (!fTagDB) return;

   fTagDB->cd();

   // Write the tree.
   fTree->Write();

   // Close DB, this also deletes the fTree
   fTagDB->Close();

   delete fTagDB;
   fTagDB = 0;
}
