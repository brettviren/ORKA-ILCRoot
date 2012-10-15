#ifndef ILCDCHDIGVXDMANAGER_H
#define ILCDCHDIGVXDMANAGER_H

/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHdigitsManager.h,v */

/////////////////////////////////////////////////////////////
//  Manages the DCH digits                                 //
/////////////////////////////////////////////////////////////

#include <TObject.h>

class TFile;
class TTree;

class IlcDCHsegmentArray;
class IlcDCHdataArrayI;
class IlcDCHdigit;

class IlcDCHdigitsManager : public TObject {

 public:

  enum { kNDict = 3 };

  IlcDCHdigitsManager();
  IlcDCHdigitsManager(const IlcDCHdigitsManager &m);
  virtual ~IlcDCHdigitsManager();
  IlcDCHdigitsManager &operator=(const IlcDCHdigitsManager &m);

  virtual void                CreateArrays();
  virtual void                ResetArrays();
  virtual void                Copy(TObject &m) const;

  virtual Bool_t              MakeBranch(TTree *tree);
          
  virtual Bool_t              ReadDigits(TTree *tree);
  virtual Bool_t              WriteDigits();

  virtual void                SetRaw();
  virtual void                SetEvent(Int_t evt)          { fEvent   = evt; };
  virtual void                SetDebug(Int_t v = 1)        { fDebug   = v;   };
  virtual void                SetSDigits(Int_t v = 1)      { fSDigits = v;   };

  virtual Bool_t              IsRaw() const                { return fIsRaw;         };
  static  Int_t               NDict()                      { return fgkNDict;       }; 

  virtual IlcDCHsegmentArray *GetDigits() const            { return fDigits;        };
  virtual IlcDCHsegmentArray *GetDictionary(Int_t i) const { return fDictionary[i]; };

          IlcDCHdigit        *GetDigit(Int_t row, Int_t col, Int_t time, Int_t det) const;
          Int_t               GetTrack(Int_t track, Int_t row, Int_t col
                                     , Int_t time, Int_t det) const;

          IlcDCHdataArrayI   *GetDigits(Int_t det) const;
          IlcDCHdataArrayI   *GetDictionary(Int_t det, Int_t i) const;
          Int_t               GetTrack(Int_t track, IlcDCHdigit *Digit) const;
          Short_t             GetDigitAmp(Int_t row, Int_t col, Int_t time, Int_t det) const;

 protected:

  static const Int_t  fgkNDict;            //  Number of track dictionary arrays

  Int_t               fEvent;              //  Event number

  TTree              *fTree;               //! Tree for the digits arrays

  IlcDCHsegmentArray *fDigits;             //! Digits data array
  IlcDCHsegmentArray *fDictionary[kNDict]; //! Track dictionary data array

  Bool_t              fIsRaw;              //  Flag indicating raw digits
  Bool_t              fSDigits;            //  Switch for the summable digits
  Int_t               fDebug;              //  Debug flag

  ClassDef(IlcDCHdigitsManager,4)          //  Manages the DCH digits

};

#endif
