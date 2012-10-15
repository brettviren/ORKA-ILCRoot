#ifndef ILCDCHMODULE_H
#define ILCDCHMODULE_H

#include <TObjArray.h>

class IlcDCHgtuTrack;
class IlcDCHltuTracklet;
class IlcDCHtrigParam;

class IlcDCHmodule : public TObject {

 public:

  enum { kNplan = 6, kNmaxZchan = 100, kNsubZchan = 16, kNmaxTrk = 12 };

  IlcDCHmodule(IlcDCHtrigParam *trigp);
  virtual ~IlcDCHmodule();
  IlcDCHmodule &operator=(const IlcDCHmodule &m);
  virtual void Copy(TObject &m) const;

  void Reset();

  void AddTracklet(Int_t det, 
		   Int_t row, 
		   Float_t rowz,
		   Float_t slope, 
		   Float_t offset, 
		   Float_t time, 
		   Int_t ncl,
		   Int_t label,
		   Float_t q);
  
  TObjArray *Tracklets() { 
    if(!fTracklets) fTracklets = new TObjArray(400); return fTracklets; 
  };

  void ResetTracklets() { if(fTracklets) fTracklets->Delete(); };
  void SortTracklets()  { if(fTracklets) fTracklets->Sort(); };
  IlcDCHltuTracklet *GetTracklet(Int_t pos) const;
  void RemoveMultipleTracklets();
  void RemoveTracklet(Int_t pos);
  Int_t GetNtracklets() const {
    if (fTracklets) return fTracklets->GetEntriesFast();
    return 0;
  };
  void AddTrack();
  TObjArray *Tracks() { 
    if(!fTracks) fTracks = new TObjArray(400); return fTracks; 
  };

  void  ResetTracks();
  void  SortTracks()  { if(fTracks) fTracks->Sort(); };
  IlcDCHgtuTrack *GetTrack(Int_t pos) const;
  void  RemoveMultipleTracks();
  void  RemoveTrack(Int_t pos);
  Int_t GetNtracks() const {
    if (fTracks) return fTracks->GetEntriesFast();
    return 0;
  };
  void  SortZ(Int_t cha);
  void  InitZLUT();
  void  FindTracks();
  void  FindTracksCombi(Int_t zchan);

 protected:

  Float_t             fXprojPlane;                  //! X (time) coordinate of the
                                                    //  projection plane
  Float_t             fField;                       //! Magnetic field
  TObjArray          *fTracklets;                   //! Array of LTU tracklets
  TObjArray          *fTracks;                      //! Array of GTU tracks

  Int_t fZnchan[kNplan][kNsubZchan];                //! number of LTU tracklets in each 
                                                    //  subchannel
  Int_t fZtrkid[kNplan][kNmaxZchan][kNsubZchan];    //! list of LTU tracklet id's for 
                                                    //  each subchannel

  Float_t  fDeltaY;                                 // Y (offset) matching window in the GTU
  Float_t  fDeltaS;                                 // Slope matching window in the GTU

  IlcDCHltuTracklet  *fLTUtrk;                      //! Current LTU tracklet
  IlcDCHgtuTrack     *fGTUtrk;                      //! Current GTU track

  ClassDef(IlcDCHmodule,2)

};

#endif
