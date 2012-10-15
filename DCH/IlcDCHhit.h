#ifndef ILCDCHHIT_H
#define ILCDCHHIT_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHhit.h,v */

////////////////////////////////////////////////
//  Hit class for the DCH                     //
////////////////////////////////////////////////

#include "IlcHit.h"

//_____________________________________________________________________________
class IlcDCHhit : public IlcHit {

 public:

  IlcDCHhit();
  IlcDCHhit(Int_t shunt, Int_t track, UShort_t superlayer, UShort_t Celring, UShort_t wire, ULong_t celid, UShort_t trkStat, Float_t *hits, UShort_t ClMult,Int_t status=0);
  virtual ~IlcDCHhit();
	  
	  
          Int_t  GetSuperlayer() const         { return  fSuperlayer; }
          Int_t  GetCelRing()    const         { return  fCelRing; }
          Int_t  GetWire()       const         { return  fWire; }
          ULong_t GetCelId()      const         { return  fCelId; }
          Int_t  GetClMult()     const         { return  fClMult; }
          Int_t  GetTrkStat()    const         { return  fTrkStat; }
          void   SetTrkStat(UShort_t ltrkstat) { fTrkStat = ltrkstat; }
          void   SetCelId  (ULong_t cid)       { fCelId  = cid; }
          void   SetclMult (UShort_t mult)     { fClMult = mult; }
	  Int_t  GetStatus()     const         { return fStatus; }
         Float_t GetTrackTime()  const         { return fTime;}
 private:
  
  UShort_t     fSuperlayer;           
  UShort_t     fCelRing;         // Cel ring number (from 0 to nring-1 ) (nring is the number of trapezium rings)
  UShort_t     fWire;
  UShort_t     fTrkStat;         // flag for track (0 if is a hit inside a volume, 1 if is entrering, 02 if is exiting)
  ULong_t      fCelId;
  UShort_t     fClMult;          // Cluster Pairs Number 
  Int_t        fStatus; 
  Float_t      fTime;            //track time
  ClassDef(IlcDCHhit,2)    // Hit for the Cluster Counting Drift Chamber

};

#endif
