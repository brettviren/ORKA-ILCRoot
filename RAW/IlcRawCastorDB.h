#ifndef ILCRAWCASTORDB_H
#define ILCRAWCASTORDB_H
// @(#) $Id: IlcRawCastorDB.h 32401 2009-05-15 14:01:03Z cvetan $
// Author: Fons Rademakers  26/11/99

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcRawCastorDB                                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "IlcRawDB.h"


class IlcRawCastorDB : public IlcRawDB {

public:
   IlcRawCastorDB(IlcRawEventV2 *event,
		  IlcESDEvent *esd,
		  Int_t compress,
		  const char* fileName = NULL,
		  Int_t basketsize = 32000);
   ~IlcRawCastorDB() { Close(); }

   const char *GetOpenOption() const { return "-RECREATE"; }
   Int_t       GetNetopt() const { return 0; }
   Long64_t    Close();

private:
   const char *GetFileName() const;

   ClassDef(IlcRawCastorDB,0)  // Raw DB via CASTOR and rootd
};

#endif
