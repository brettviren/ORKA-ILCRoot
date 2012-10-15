#ifndef ILCTARGETBADCHANNELSSPD_H
#define ILCTARGETBADCHANNELSSPD_H

/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETBadChannelsSPD.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////
// IlcTARGETBadChannelsSPD declaration by P. Nilsson 2005
// AUTHOR/CONTACT: Paul.Nilsson@cern.ch
//
// The class is used by the IlcTARGETPreprocessorSPD class to store the
// final noisy and dead channel objects in the calibration database for
// the SPD.
//
// (See the source file for more information)
///////////////////////////////////////////////////////////////////////////


#include "TObjArray.h"
#include "IlcTARGETChannelSPD.h"

class IlcTARGETBadChannelsSPD: public TObject {

 public:

  IlcTARGETBadChannelsSPD(void);		                // Default constructor
  IlcTARGETBadChannelsSPD(const IlcTARGETBadChannelsSPD &bc); // Default copy constructor
  virtual ~IlcTARGETBadChannelsSPD(void);                  // Default destructor
  IlcTARGETBadChannelsSPD& operator=(const IlcTARGETBadChannelsSPD& bc); // Assignment operator

  void Put(Int_t* &array, const Int_t &arraySize,       // Add new arrays to the collection
	   Int_t* &index, const Int_t &indexSize);
  Bool_t Get(Int_t* &array, Int_t* &index) const;       // Retrieve the stored arrays (true if non empty arrays)

  Int_t GetIndexArraySize(void) const                   // Return the size of the index array
    { return fIndexArraySize; };
  Int_t GetBadChannelsArraySize(void) const             // Return the size of the bad channels array
    { return fBadChannelsArraySize; };

  Int_t* CreateModuleArray(Int_t module) const;   // Create an array with sequential data for a given module
  Int_t GetModuleArraySize(Int_t module) const    // Return array size for a given module
    { return (2*fBadChannelsArray[fIndexArray[module]] + 1); };

  TObjArray* CreateModuleObjArray(Int_t module) const; // Create a TObjArray with data for a given module
  Int_t GetModuleObjArraySize(Int_t module) const      // Return TObjArray size for a given module
    { return (fBadChannelsArray[fIndexArray[module]]); };

 protected:

  Int_t fIndexArraySize;                                // Size of the index array
  Int_t fBadChannelsArraySize;                          // Size of the bad channels array
  Int_t *fIndexArray;                                   //[fIndexArraySize]
  Int_t *fBadChannelsArray;                             //[fBadChannelsArraySize]

  ClassDef(IlcTARGETBadChannelsSPD,1)
};

#endif
