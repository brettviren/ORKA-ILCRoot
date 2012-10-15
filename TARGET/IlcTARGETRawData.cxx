////////////////////////////////////////////////
//  RawData classes for set:TARGET               //
////////////////////////////////////////////////

#include <string.h>

#include "IlcTARGETRawData.h"

ClassImp(IlcTARGETRawData)

ClassImp(IlcTARGETInStream)

//_____________________________________________________________________________

IlcTARGETInStream::IlcTARGETInStream()
{
  //default constructor
  fStreamLen=0;
  fInStream=0;
}
//_____________________________________________________________________________

IlcTARGETInStream::IlcTARGETInStream(UInt_t length)
{
  //
  // Creates a stream of unsigned chars
  //
  
  fStreamLen = length;
  fInStream = new UChar_t[length]; 
  
  ClearStream(); 
  
}

//_____________________________________________________________________________
IlcTARGETInStream::~IlcTARGETInStream()
{
  //destructor
  if (fInStream) delete[] fInStream;
}

//__________________________________________________________________________
IlcTARGETInStream::IlcTARGETInStream(const IlcTARGETInStream &source) : TObject(source){
  //     Copy Constructor 
  if(&source == this) return;
  this->fStreamLen = source.fStreamLen;
  this->fInStream = source.fInStream;
  return;
}

//_________________________________________________________________________
IlcTARGETInStream& 
  IlcTARGETInStream::operator=(const IlcTARGETInStream &source) {
  //    Assignment operator
  if(&source == this) return *this;
  this->fStreamLen = source.fStreamLen;
  this->fInStream = source.fInStream;
  return *this;
}

//_____________________________________________________________________________
void IlcTARGETInStream::ClearStream()
{
  //clear the array
  memset(fInStream,0,sizeof(UChar_t)*fStreamLen);
}


//_____________________________________________________________________________
Bool_t IlcTARGETInStream::CheckCount(UInt_t count) {
  //check boundaries
  if (count <= (UInt_t)fStreamLen) return kTRUE;
  else {
    Error("CheckCount", "actual size is %d, the necessary size is %d",fStreamLen,count);
    return kFALSE;
  }
}


ClassImp(IlcTARGETOutStream)
  
  //_______________________________________________________________________
  
  IlcTARGETOutStream::IlcTARGETOutStream() {
  //default constructor
  fStreamLen=0;
  fOutStream=0;
}

//__________________________________________________________________________

IlcTARGETOutStream::IlcTARGETOutStream(UInt_t length) {
  //
  // Creates a stream of unsigned chars
  //
  
  fStreamLen = length;
  fOutStream = new UInt_t[length];  
  ClearStream(); 
  
}

//_____________________________________________________________________________
IlcTARGETOutStream::~IlcTARGETOutStream()
{
  //destructor
  if (fOutStream) delete[] fOutStream;
}

//__________________________________________________________________________
IlcTARGETOutStream::IlcTARGETOutStream(const IlcTARGETOutStream &source):TObject(source){
  //     Copy Constructor 
  if(&source == this) return;
  this->fStreamLen = source.fStreamLen;
  this->fOutStream = source.fOutStream;
  return;
}

//_________________________________________________________________________
IlcTARGETOutStream& 
  IlcTARGETOutStream::operator=(const IlcTARGETOutStream &source) {
  //    Assignment operator
  if(&source == this) return *this;
  this->fStreamLen = source.fStreamLen;
  this->fOutStream = source.fOutStream;
  return *this;
}

//_____________________________________________________________________________
void IlcTARGETOutStream::ClearStream()
{
  // clear stream
  memset(fOutStream,0,sizeof(UInt_t)*fStreamLen);
}

//_____________________________________________________________________________
Bool_t IlcTARGETOutStream::CheckCount(UInt_t count)
{
  //check boundaries
  if (count < fStreamLen) return kTRUE;
  else {
    Error("CheckCount", "actual size is %d, the necessary size is %d",fStreamLen,count);
    return kFALSE;
  }
}
