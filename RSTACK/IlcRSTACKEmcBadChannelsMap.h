#ifndef ILCRSTACKEMCBADCHANNELSMAP
#define ILCRSTACKEMCBADCHANNELSMAP
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                          */

/* $Id: IlcRSTACKEmcBadChannelsMap.h 40123 2010-03-31 22:18:54Z kharlov $ */

// This class keeps the EMC bad channels map 
// (bad means dead or noisy).

#include "TObject.h"

class IlcRSTACKEmcBadChannelsMap : public TObject {

public:

  IlcRSTACKEmcBadChannelsMap();
  IlcRSTACKEmcBadChannelsMap(const IlcRSTACKEmcBadChannelsMap &map);
  IlcRSTACKEmcBadChannelsMap& operator= (const IlcRSTACKEmcBadChannelsMap &map);
  ~IlcRSTACKEmcBadChannelsMap() {}

  void  SetBadChannel(Int_t module, Int_t col, Int_t row);
  Bool_t IsBadChannel(Int_t module, Int_t col, Int_t row) const { return fBadChannelEmc[module-1][col-1][row-1]; }
  Int_t GetNumOfBadChannels() const {  return fBads; }
  void BadChannelIds(Int_t *badIds=0);
  void Reset();

private:
  
  Bool_t fBadChannelEmc[5][56][64]; //[mod][col][row]
  Int_t fBads;

  ClassDef(IlcRSTACKEmcBadChannelsMap,2)

};

#endif
