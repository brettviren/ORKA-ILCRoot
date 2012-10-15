#ifndef ILCTARGETBADCHANNELSAUXSPD_H
#define ILCTARGETBADCHANNELSAUXSPD_H

/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETBadChannelsAuxSPD.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $ */

/////////////////////////////////////////////////////////
// IlcTARGETBadChannelsAuxSPD declaration by P. Nilsson 2005
// AUTHOR/CONTACT: Paul.Nilsson@cern.ch                  
//
// Auxiliary algorithms for the SPD
/////////////////////////////////////////////////////////


#include <TObjArray.h>
#include <TString.h>
#include "IlcTARGETChannelSPD.h"
#include "IlcTARGETdigitSPD.h"

class IlcTARGETBadChannelsAuxSPD {

 public:

  IlcTARGETBadChannelsAuxSPD(void);    			      // Default constructor
  virtual ~IlcTARGETBadChannelsAuxSPD(void) { };                 // Default destructor

  // General algorithms
  Bool_t Diff(TObjArray *&in1, TObjArray *&in2, TObjArray *&out1, TObjArray *&out2) const; // Diff algorithm
  Bool_t Find(IlcTARGETChannelSPD *&channel, TObjArray *&array) const; // Find a channel in the array
  Bool_t Find(IlcTARGETdigitSPD *&digit, TObjArray *&array) const;     // Find a digit in the array
  Int_t GetNumberOfBadChannels(Int_t* &array, Int_t* &indexArray, Int_t size) const; // Get the number of bad channels

  // Converters
  IlcTARGETdigitSPD* CreateDigitFromChannel(const IlcTARGETChannelSPD *&channel) const; // Create a digit from a channel
  IlcTARGETChannelSPD* CreateChannelFromDigit(const IlcTARGETdigitSPD *&digit) const;   // Create a channel from a digit

  // Miscellanious
  Bool_t CreateHTMLReport(char *name, Int_t* &array, Int_t* &indexArray,    // Create an HTML report
			  Int_t indexSize, TString *buffer, Bool_t tags);

 protected:

  ClassDef(IlcTARGETBadChannelsAuxSPD,1)
};

#endif
