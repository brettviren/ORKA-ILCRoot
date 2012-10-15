#ifndef ILCDCHMATRIX_H
#define ILCDCHMATRIX_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHmatrix.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

#include <TObject.h>

class TObjArray;

class IlcDCHpixel;

///////////////////////////////////////////////////////
//                                                   //
//  Stores the pixel-information of one DCH chamber  //
//                                                   //
///////////////////////////////////////////////////////

class IlcDCHmatrix : public TObject {

public:

  IlcDCHmatrix();
  IlcDCHmatrix(Int_t nRow, Int_t nCol, Int_t nTime, Int_t iSec, Int_t iCha, Int_t iPla);
  IlcDCHmatrix(const IlcDCHmatrix &m);
  virtual ~IlcDCHmatrix();
  IlcDCHmatrix &operator=(const IlcDCHmatrix &m);

  virtual void         AddSignal(Int_t iRow, Int_t iCol, Int_t iTime, Float_t signal);
  virtual Bool_t       AddTrack(Int_t iRow, Int_t iCol, Int_t iTime, Int_t track);

  virtual void         Copy(TObject &m) const;
  virtual void         Draw(Option_t *opt = " ");
  virtual void         DrawRow(Int_t iRow);
  virtual void         DrawCol(Int_t iCol);
  virtual void         DrawTime(Int_t iTime);
  virtual void         ProjRow();  
  virtual void         ProjCol();  
  virtual void         ProjTime();  

  virtual void         SetSignal(Int_t iRow, Int_t iCol, Int_t iTime, Float_t signal);
  virtual void         SetTrack(Int_t iRow, Int_t iCol, Int_t iTime
                              , Int_t iTrack, Int_t track);

  virtual Float_t      GetSignal(Int_t iRow, Int_t iCol, Int_t iTime) const;
  virtual Int_t        GetTrack(Int_t iRow, Int_t iCol, Int_t iTime, Int_t iTrack) const;

  virtual Int_t        GetSector() const  { return fSector;  };
  virtual Int_t        GetChamber() const { return fChamber; };
  virtual Int_t        GetPlane() const   { return fPlane;   };

protected:

  virtual Int_t        GetIndex(Int_t iRow, Int_t iCol, Int_t iTime) const;
  virtual IlcDCHpixel *GetPixel(Int_t iRow, Int_t iCol, Int_t iTime) const;

  Int_t         fRow;            // Number of pad-rows
  Int_t         fCol;            // Number of pad-columns
  Int_t         fTime;           // Number of time buckets
  Int_t         fPixel;          // Number of pixels
  Int_t         fSector;         // Sector number
  Int_t         fChamber;        // Chamber number
  Int_t         fPlane;          // Plane number
  TObjArray    *fPixelArray;     // Array of pixels

  ClassDef(IlcDCHmatrix,1)       // The DCH detector matrix for one readout chamber

};

#endif
