#ifndef ILCRAWROOTDDB_H
#define ILCRAWROOTDDB_H
// @(#) $Id: IlcRawRootdDB.h 32401 2009-05-15 14:01:03Z cvetan $
// Author: Fons Rademakers  26/11/99

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcRawRootdDB                                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "IlcRawDB.h"


class IlcRawRootdDB : public IlcRawDB {

public:
   IlcRawRootdDB(IlcRawEventV2 *event,
		 IlcESDEvent *esd,
		 Int_t compress,
		 const char* fileName = NULL,
		 Int_t basketsize = 32000);
   ~IlcRawRootdDB() { Close(); }

   Long64_t Close();

private:
   const char *GetFileName() const;

   ClassDef(IlcRawRootdDB,0)  // Raw DB via rootd
};

#endif
