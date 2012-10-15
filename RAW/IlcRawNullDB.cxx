// @(#) $Id: IlcRawNullDB.cxx 32401 2009-05-15 14:01:03Z cvetan $
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
// IlcRawNullDB                                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "IlcRawNullDB.h"


ClassImp(IlcRawNullDB)


//______________________________________________________________________________
IlcRawNullDB::IlcRawNullDB(IlcRawEventV2 *event,
			   IlcESDEvent *esd,
			   Int_t compress,
			   const char* fileName,Int_t basketsize)
   : IlcRawDB(event, esd, compress, fileName, basketsize)
{
   // Create a new raw DB that will wrtie to /dev/null.

}

//______________________________________________________________________________
const char *IlcRawNullDB::GetFileName() const
{
   // Return /dev/null as filename.

   return "/dev/null";
}

//______________________________________________________________________________
Long64_t IlcRawNullDB::Close()
{
   // Close raw RFIO DB.

   if (!fRawDB) return 0;

   if (!fRawDB->IsOpen()) return 0;

   fRawDB->cd();

   // Write the tree.
   Bool_t error = kFALSE;
   if (fTree)
     if (fTree->Write() == 0)
       error = kTRUE;
   if (fESDTree)
     if (fESDTree->Write() == 0)
       error = kTRUE;

   // Close DB, this also deletes the fTree
   fRawDB->Close();

   fTree = NULL;

   Long64_t filesize = fRawDB->GetEND();

   delete fRawDB;
   fRawDB = 0;
   if(!error)
     return filesize;
   else
     return -1;
}
