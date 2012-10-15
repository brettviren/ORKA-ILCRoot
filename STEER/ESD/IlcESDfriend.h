#ifndef ILCESDFRIEND_H
#define ILCESDFRIEND_H

//-------------------------------------------------------------------------
//                     Class IlcESDfriend
//               This class contains ESD additions
//       Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch 
//-------------------------------------------------------------------------

#include <TObject.h>
#include <TClonesArray.h>

#include "IlcESDfriendTrack.h"

class IlcESDVZEROfriend;
class IlcESDTZEROfriend;

//_____________________________________________________________________________
class IlcESDfriend : public TObject {
public:
  IlcESDfriend();
  IlcESDfriend(const IlcESDfriend &);
  IlcESDfriend& operator=(const IlcESDfriend& esd);  
  virtual ~IlcESDfriend();

  Int_t GetNumberOfTracks() const {return fTracks.GetEntriesFast();}
  IlcESDfriendTrack *GetTrack(Int_t i) const {
     return (IlcESDfriendTrack *)fTracks.At(i);
  }
  Int_t GetEntriesInTracks() const {return fTracks.GetEntries();}
  void AddTrack(const IlcESDfriendTrack *t) {
     new(fTracks[fTracks.GetEntriesFast()]) IlcESDfriendTrack(*t);
  }

  void AddTrackAt(const IlcESDfriendTrack *t, Int_t i) {
     new(fTracks[i]) IlcESDfriendTrack(*t);
  }

  void SetVZEROfriend(IlcESDVZEROfriend * obj);
  IlcESDVZEROfriend *GetVZEROfriend(){ return fESDVZEROfriend; }
  void SetTZEROfriend(IlcESDTZEROfriend * obj);
  IlcESDTZEROfriend *GetTZEROfriend(){ return fESDTZEROfriend; }

  void Ls(){
	  return fTracks.ls();
  }

  // bit manipulation for filtering
  void SetSkipBit(Bool_t skip){SetBit(23,skip);}
  Bool_t TestSkipBit() {return TestBit(23);}

protected:
  TClonesArray fTracks;    // ESD friend tracks
  IlcESDVZEROfriend *fESDVZEROfriend; // VZERO object containing complete raw data
  IlcESDTZEROfriend *fESDTZEROfriend; // TZERO calibration object

  ClassDef(IlcESDfriend,3) // ESD friend
};

#endif


