#ifndef ILC_DCS_VALUE_H
#define ILC_DCS_VALUE_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCSValue.h 50615 2011-07-16 23:19:19Z hristov $ */

//
// This class represents the value(s) of a DCS data point at a given timestamp
//

#include <TObject.h>

class IlcDCSValue : public TObject {
public:
  enum Type {
    kInvalid = 0,

    kBool = 1,
    kChar = 2,
    kInt = 3,
    kUInt = 4,
    kFloat = 5
  };

  IlcDCSValue();
  IlcDCSValue(const IlcDCSValue& c);

  virtual ~IlcDCSValue();

  IlcDCSValue& operator=(const IlcDCSValue& c);
  virtual void Copy(TObject& c) const;

  IlcDCSValue(Bool_t value, UInt_t timeStamp);
  IlcDCSValue(Char_t value, UInt_t timeStamp);
  IlcDCSValue(Int_t value, UInt_t timeStamp);
  IlcDCSValue(UInt_t value, UInt_t timeStamp);
  IlcDCSValue(Float_t value, UInt_t timeStamp);

  Bool_t GetBool() const;
  Char_t GetChar() const;
  Int_t GetInt() const;
  UInt_t GetUInt() const;
  Float_t GetFloat() const;

  operator Bool_t() const { return GetBool(); }
  operator Char_t() const { return GetChar(); }
  operator Int_t() const { return GetInt(); }
  operator UInt_t() const { return GetUInt(); }
  operator Float_t() const { return GetFloat(); }

  Type GetType() const { return fType; }

  UInt_t GetTimeStamp() const { return fTimeStamp; }
  void SetTimeStamp(UInt_t timeStamp) { fTimeStamp = timeStamp; }

  Int_t GetSize() const;

  const Char_t* ToString() const;
  void Print(Option_t* /*opt*/) const;


/**********************************************/

  Int_t Compare(const TObject* obj) const
    { if( fTimeStamp < ((IlcDCSValue *)obj )->fTimeStamp ) return -1;
      if( fTimeStamp == ((IlcDCSValue *)obj )->fTimeStamp ) return 0;
      return 1;
    }

  Bool_t IsSortable() const { return kTRUE; }

/*********************************************/


protected:
  void Init();

  Type fType;           // type of the value stored

  Bool_t fBool;         // bool value
  Char_t fChar;         // char value
  Int_t fInt;           // int value
  UInt_t fUInt;         // uint value
  Float_t fFloat;       // float value

  UInt_t fTimeStamp;    // timestamp of this value

	ClassDef(IlcDCSValue, 2);
};

#endif
