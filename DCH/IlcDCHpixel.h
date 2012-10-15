#ifndef ILCDCHPIXEL_H
#define ILCDCHPIXEL_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHpixel.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

#include <TObject.h>

//////////////////////////////////////////////////////
//  Stores the information for one detector pixel   //
//////////////////////////////////////////////////////

class IlcDCHpixel : public TObject {

public:

  IlcDCHpixel();
  virtual ~IlcDCHpixel();

  virtual void    Copy(TObject &p) const;

  static  Int_t   NTrackPixel()                  { return fgkNTrackPixel; };

  virtual void    SetSignal(Float_t signal)      { fSignal   = signal; };
  virtual void    SetTrack(Int_t i, Int_t track) { fTrack[i] = track;  };

  virtual Float_t GetSignal() const              { return fSignal;     };
  virtual Int_t   GetTrack(Int_t i) const        { return fTrack[i];   };

protected:

  enum { kNTrackPixel = 3 };
  static const Int_t fgkNTrackPixel;       // Maximal number of stored tracks

  Float_t      fSignal;                    // Signal sum
  Int_t        fTrack[kNTrackPixel];       // Tracks contributing to this pixel

  ClassDef(IlcDCHpixel,1)                  // Information for one detector pixel   

};

#endif
