#ifndef ILCRAWDATAARRAY_H
#define ILCRAWDATAARRAY_H

// Author: Cvetan Cheshkov  27/03/2007

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcRawDataArray                                                      //
// A container object which is used in order to write the sub-detector  //
// raw-data payloads into a separate branches                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TObject
#include <TObject.h>
#endif

class TObjArray;
class IlcRawData;

class IlcRawDataArray : public TObject {

public:
   IlcRawDataArray();
   IlcRawDataArray(Int_t n);
   virtual ~IlcRawDataArray();

   void ClearData();
   void Add(IlcRawData *data);

private:
   TObjArray *fRawDataArray; // Array containing raw-data payloads

   IlcRawDataArray(const IlcRawDataArray &);      // not implemented, usage causes
   IlcRawDataArray &operator=(const IlcRawDataArray &);  // link time error

   ClassDef(IlcRawDataArray,1)  // Ilc raw event buffer
};

#endif
