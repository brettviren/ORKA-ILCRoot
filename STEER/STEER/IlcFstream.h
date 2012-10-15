#ifndef ILCFSTREAM_H
#define ILCFSTREAM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//-------------------------------------------------------------------------
// This is the class which is to be used during the writing of
// simulated raw data (DDL files format).
// It is using the root functionality in order to deal correctly
// with little/big endian issue. By convention the detector raw
// data payload is stored always with little endian (this corresponds
// to the real life situation when the detector data is coming from
// the hardware).
//-------------------------------------------------------------------------

#include <TObject.h>

class IlcFstream : public TObject {
public:
  IlcFstream();
  IlcFstream(const char *fileName);
  virtual ~IlcFstream();

  void   Seekp(UInt_t position);
  UInt_t Tellp();
  void   WriteBuffer(const char *buffer, UInt_t size, Bool_t force = kFALSE);

private:

  IlcFstream(const IlcFstream &source);
  IlcFstream &operator =(const IlcFstream& source);

  UInt_t Swap(UInt_t x);

  fstream *fFile;       // Output file stream
  UInt_t  *fBuffer;     // Pointer to the internal buffer
  UInt_t   fBufferSize; // Internal buffer size

  ClassDef(IlcFstream,0)
};

#endif
