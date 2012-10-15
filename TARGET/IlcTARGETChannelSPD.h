#ifndef ILCTARGETCHANNELSPD_H
#define ILCTARGETCHANNELSPD_H

/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETChannelSPD.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////
// IlcTARGETChannelSPD declaration by P. Nilsson 2005
// AUTHOR/CONTACT: Paul.Nilsson@cern.ch
//
// Objects of this class are stored in a TObjArray and should be
// interpreted as "bad" channels, i.e. either noisy or dead channels
// depending on where they are stored
///////////////////////////////////////////////////////////////////////////

#include <TObject.h>

class IlcTARGETChannelSPD: public TObject {

 public:

  IlcTARGETChannelSPD(void);    			            // Default constructor
  IlcTARGETChannelSPD(Int_t column, Int_t row);    // Constructor for already existing "bad" channel
  IlcTARGETChannelSPD(const IlcTARGETChannelSPD &ch);             // Copy constructor
  virtual ~IlcTARGETChannelSPD(void) { };                      // Default destructor
  IlcTARGETChannelSPD& operator=(const IlcTARGETChannelSPD &ch);  // Assignment operator
  Bool_t operator==(const IlcTARGETChannelSPD &channel) const; // Equivalence operator

  // Getters and setters
  Int_t GetColumn(void) const { return fColumn; };          // Get column
  Int_t GetRow(void) const { return fRow; };                // Get row
  void SetColumn(Int_t c) { fColumn = c; };                 // Set column
  void SetRow(Int_t r) { fRow = r; };                       // Set row

 protected:

  Int_t fColumn;                                            // SPD column (real range [0,31], but not checked)
  Int_t fRow;                                               // SPD row (real range [0,255] but not checked)

  ClassDef(IlcTARGETChannelSPD,1)
};

#endif
