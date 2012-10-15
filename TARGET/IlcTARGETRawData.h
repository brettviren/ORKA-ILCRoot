#ifndef ILCTARGETRAWDATA_H
#define ILCTARGETRAWDATA_H

////////////////////////////////////////////////
//  RawData classes for set:TARGET               //
////////////////////////////////////////////////

#include <TObject.h>



class IlcTARGETRawData: public TObject  {

  // most probably it should have a class IlcTARGETHeaderEvent as data member
   
 public:
    IlcTARGETRawData() {
      // constructor
    }
    virtual   ~IlcTARGETRawData() {
      // destructor
    }

    ClassDef(IlcTARGETRawData,1)     //RawData object for set:TARGET

};

//___________________________________________
class IlcTARGETInStream: public TObject{

 public:
  IlcTARGETInStream();
  IlcTARGETInStream(UInt_t length);
  virtual   ~IlcTARGETInStream();
  IlcTARGETInStream(const IlcTARGETInStream &source); // copy constructor
  IlcTARGETInStream& operator=(const IlcTARGETInStream &source); // ass. operator
    
  void ClearStream();
  Bool_t CheckCount(UInt_t count);
  UInt_t  StreamLength() {
    // stream length
    return fStreamLen;
  }
  UChar_t *Stream() {
    // stream
    return fInStream;
  }
  
protected:
  
  // input stream of unsigned chars
  
  UInt_t     fStreamLen;       // Length of the array
  UChar_t    *fInStream;        //[fStreamLen] Pointer to an array of input unsigned chararacters
  
  
  
  ClassDef(IlcTARGETInStream,1)     //Input Stream  object for set:TARGET
    };

//___________________________________________
class IlcTARGETOutStream: public TObject{
  
public:
  IlcTARGETOutStream();
  
  IlcTARGETOutStream(UInt_t length);
  virtual   ~IlcTARGETOutStream();
  IlcTARGETOutStream(const IlcTARGETOutStream &source); // copy constructor
  IlcTARGETOutStream& operator=(const IlcTARGETOutStream &source); // assignment operator
    
  void ClearStream();
  Bool_t CheckCount(UInt_t count);
  UInt_t  StreamLength() {
    // stream length
    return fStreamLen;
  }
  UInt_t *Stream() {
    // stream
    return fOutStream;
  }
  
protected:
  
  // output stream of unsigned chars
  
  UInt_t     fStreamLen;        // Length of the array
  UInt_t    *fOutStream;        //[fStreamLen] Pointer to an array of unsigned long
  
  

    ClassDef(IlcTARGETOutStream,1)     //Output Stream  object for set:TARGET
};



#endif
