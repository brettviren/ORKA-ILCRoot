#ifndef ILCDCHTRIGGER_H
#define ILCDCHTRIGGER_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH trigger class                                                        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TNamed.h>
#include <TObjArray.h>
#include <TClonesArray.h>

#include "IlcDCHgtuTrack.h"

class TTree;

class IlcDCHdigitsManager;
class IlcDCHdataArrayI;
class IlcDCHgeometry;
class IlcDCHcalibDB;
class IlcDCHCommonParam;

class IlcRunLoader;
class IlcRawReader;

class IlcDCHmcmTracklet;
class IlcDCHmcm;
class IlcDCHmodule;
class IlcDCHtrigParam;

class IlcDCHtrigger : public TNamed {

 public:  

  enum { kNMCM = 16, kMaxTrackletsPerMCM = 4, kMcmCol = 21 };

  IlcDCHtrigger();
  IlcDCHtrigger(const Text_t* name, const Text_t* title);
  IlcDCHtrigger(const IlcDCHtrigger &p);   
  virtual ~IlcDCHtrigger();

  IlcDCHtrigger &operator=(const IlcDCHtrigger &p); 
  virtual void Copy(TObject &p) const;

  void Init();

  void   SetRunLoader(IlcRunLoader *rl) { fRunLoader = rl; };
  Bool_t Open(const Char_t *name, Int_t nEvent = 0);
  Bool_t ReadDigits();
  Bool_t ReadDigits(IlcRawReader* rawReader);
  Bool_t MakeTracklets(Bool_t makeTracks = kFALSE);
  Bool_t WriteTracklets(Int_t det);
  Bool_t ReadTracklets(IlcRunLoader *rl);

  void   AddTracklet(Int_t det, Int_t row, Int_t seed, Int_t n);
  Bool_t TestTracklet(Int_t det, Int_t row, Int_t seed, Int_t n);
  TObjArray *Tracklets() { 
    if(!fTracklets) fTracklets = new TObjArray(400); return fTracklets; 
  };
  void   ResetTracklets() { if(fTracklets) fTracklets->Delete(); };
  void   SetMCMcoordinates(Int_t imcm);
  void   SetParameter(IlcDCHtrigParam *trigp) { fTrigParam = trigp; };
  IlcDCHtrigParam *GetParameter() const { return fTrigParam; };

  void   MakeTracks(Int_t det);

  IlcDCHgtuTrack *GetTrack(Int_t i) const {
    return (IlcDCHgtuTrack *)fTracks.UncheckedAt(i);
  }
  void AddTrack(const IlcDCHgtuTrack *t, Int_t det) {
    IlcDCHgtuTrack * track = new(fTracks[fTracks.GetEntriesFast()]) IlcDCHgtuTrack(*t);
    track->SetDetector(det);
  }
  Int_t GetNumberOfTracks() const {return fTracks.GetEntriesFast();}

  Int_t GetNPrimary() const { return fNPrimary; };

 protected:

  Float_t                fField;                       //! Magnetic field
  IlcDCHgeometry        *fGeo;                         //! DCH geometry
  IlcDCHcalibDB         *fCalib;                       //! Calibration DB
  IlcDCHCommonParam     *fCParam;                      //! Common parameters

  IlcDCHtrigParam       *fTrigParam;                   //! Trigger class parameters
  IlcRunLoader          *fRunLoader;                   //! Run Loader
  IlcDCHdigitsManager   *fDigitsManager;               //! DCH digits manager
  TTree                 *fTrackletTree;                //! Tree with tracklets
  TObjArray             *fTracklets;                   //! Array of tracklets

  Int_t                  fNROB;                        //! Number of ROBs in the current chamber
  IlcDCHmcm             *fMCM;                         //! Current MCM
  IlcDCHmcmTracklet     *fTrk;                         //! Current tracklet
  IlcDCHmcmTracklet     *fTrkTest;                     //! Test tracklet
  IlcDCHmodule          *fModule;                      //! Current module
  IlcDCHgtuTrack        *fGTUtrk;                      //! Current GTU track

  Int_t                  fNtracklets;                  //! Tracklets counter

  IlcDCHdataArrayI *fDigits;                           //! Array with digits
  IlcDCHdataArrayI *fTrack0;                           //! Track dictionary 0
  IlcDCHdataArrayI *fTrack1;                           //! Track dictionary 1
  IlcDCHdataArrayI *fTrack2;                           //! Track dictionary 2

  Int_t fNPrimary;                                     //! Number of primary tracks

  TClonesArray           fTracks;                      //! Array of GTU tracks

  ClassDef(IlcDCHtrigger,2)                            //  DCH trigger class

};

#endif
