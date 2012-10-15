#ifndef ILCDCHSEGMENTID_H
#define ILCDCHSEGMENTID_H

/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHsegmentID.h,v */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//     Base class for a detector segment                                     // 
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TObject.h>

class IlcDCHsegmentID : public TObject {

 public:

  IlcDCHsegmentID();
  IlcDCHsegmentID(Int_t index);
  virtual ~IlcDCHsegmentID();

          Int_t  GetID() const      { return fSegmentID;    }
  virtual Int_t  GetSize() const    { return sizeof(*this); }

          void   SetID(Int_t index) { fSegmentID = index;} 

 protected:

  Int_t fSegmentID;           // Identification number of a segment

  ClassDef(IlcDCHsegmentID,1) // Detector segment base class

};
   
#endif 

