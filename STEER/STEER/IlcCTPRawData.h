/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////
// Class used to store the CTP (trigger) DDL raw data      //
/////////////////////////////////////////////////////////////

#ifndef IlcTPCDDLRAWDATA_H
#define IlcTPCDDLRAWDATA_H


class IlcCTPRawData:public TObject{
 public:
  IlcCTPRawData();                                       // default constructor
  virtual ~IlcCTPRawData() {;}                           // destructor
  IlcCTPRawData(const IlcCTPRawData &source);            // copy constructor
  IlcCTPRawData& operator=(const IlcCTPRawData &source); // assignment operator
  void  RawData();  //This method is used to create the slides (sequence of files)

  ClassDef(IlcCTPRawData,1)
};
    
#endif
