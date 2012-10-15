#ifndef ILCRAWDATAARRAYV2_H
#define ILCRAWDATAARRAYV2_H

// Author: Cvetan Cheshkov  27/03/2007

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcRawDataArrayV2                                                    //
// A container object which is used in order to write the sub-detector  //
// raw-data payloads into a separate branches                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TObject
#include <TObject.h>
#endif
#include <TClonesArray.h>

class IlcRawData;

class IlcRawDataArrayV2 : public TObject {

public:
   IlcRawDataArrayV2();
   IlcRawDataArrayV2(Int_t n);
   virtual ~IlcRawDataArrayV2();

   void ClearData();
   IlcRawData *Add();

private:
   TClonesArray  fRawDataArray; // Array containing raw-data payloads
   Int_t         fNAlloc;       //!

   IlcRawDataArrayV2(const IlcRawDataArrayV2 &);      // not implemented, usage causes
   IlcRawDataArrayV2 &operator=(const IlcRawDataArrayV2 &);  // link time error

   ClassDef(IlcRawDataArrayV2,1)  // Ilc raw event buffer
};

#endif
