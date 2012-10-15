#ifndef ILCRAWDATA_H
#define ILCRAWDATA_H
// @(#) $Id: IlcRawData.h 32401 2009-05-15 14:01:03Z cvetan $
// Author: Fons Rademakers  26/11/99

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcRawData                                                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TObject
#include <TObject.h>
#endif


class IlcRawData : public TObject {

public:
   IlcRawData();
   virtual ~IlcRawData() { if (fOwner) delete [] fRawData; }

   inline void SetBuffer(void *buf, Int_t size);
   Int_t       GetSize() const { return fSize; }
   void       *GetBuffer() { return fRawData; }

private:
   Int_t   fSize;         // number of raw data bytes
   char   *fRawData;      //[fSize] raw event data
   Bool_t  fOwner;        //!if true object owns fRawData buffer

   IlcRawData(const IlcRawData &);
   IlcRawData &operator=(const IlcRawData &);

   ClassDef(IlcRawData,2)  // Ilc raw event buffer
};

void IlcRawData::SetBuffer(void *buf, Int_t size)
{
   if (fOwner) delete [] fRawData;
   fRawData = (char *) buf;
   fSize    = size;
   fOwner   = kFALSE;
}

#endif
