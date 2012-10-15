#ifndef ILCRAWNULLDB_H
#define ILCRAWNULLDB_H
// @(#) $Id: IlcRawNullDB.h 32401 2009-05-15 14:01:03Z cvetan $
// Author: Fons Rademakers  26/11/99

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcRawNullDB                                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "IlcRawDB.h"


class IlcRawNullDB : public IlcRawDB {

public:
   IlcRawNullDB(IlcRawEventV2 *event,
		IlcESDEvent *esd,
		Int_t compress,
		const char* fileName,
		Int_t basketsize = 32000);
   ~IlcRawNullDB() { Close(); }

   Long64_t Close();

private:
   const char *GetFileName() const;

   ClassDef(IlcRawNullDB,0)  // Raw DB to /dev/null
};

#endif
