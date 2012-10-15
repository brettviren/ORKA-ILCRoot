#ifndef ILCTAGNULLDB_H
#define ILCTAGNULLDB_H
// @(#) $Id: IlcTagNullDB.h 23318 2008-01-14 12:43:28Z hristov $
// Author: Fons Rademakers  26/11/99

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcTagNullDB                                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "IlcTagDB.h"


class IlcTagNullDB : public IlcTagDB {

public:
   IlcTagNullDB(IlcRawEventTag *eventTag);
   ~IlcTagNullDB() { Close(); }

   void Close();

private:
   const char *GetFileName() const;

   ClassDef(IlcTagNullDB,0)   // Tag DB to /dev/null
};

#endif
