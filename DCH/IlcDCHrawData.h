#ifndef ILCDCHRAWDATA_H
#define ILCDCHRAWDATA_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHrawData.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Converts DCH digits into a raw data stream                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TObject.h"

class TTree;
class IlcDCHdigitsManager;
class IlcRawReader;

class IlcDCHrawData : public TObject {

 public:

  IlcDCHrawData();
  IlcDCHrawData(const IlcDCHrawData &r);
  virtual ~IlcDCHrawData();
  IlcDCHrawData &operator=(const IlcDCHrawData &r);

  virtual void                 Copy(TObject &r) const;

  virtual Bool_t               Digits2Raw(TTree *digits);
  virtual IlcDCHdigitsManager* Raw2Digits(IlcRawReader* rawReader);
  virtual void                 SetDebug(Int_t v = 1) { fDebug = v; };

 protected:

  Int_t                fDebug;          //  Debug level

  ClassDef(IlcDCHrawData,1)             //  DCH raw data class

};
#endif
