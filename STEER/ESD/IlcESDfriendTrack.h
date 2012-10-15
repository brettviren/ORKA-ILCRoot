#ifndef ILCESDFRIENDTRACK_H
#define ILCESDFRIENDTRACK_H

//-------------------------------------------------------------------------
//                     Class IlcESDfriendTrack
//               This class contains ESD track additions
//       Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch 
//-------------------------------------------------------------------------

#include <TObject.h>
#include <TClonesArray.h>
#include <IlcExternalTrackParam.h>

class IlcTrackPointArray;
class IlcKalmanTrack;
class TObjArrray;
//_____________________________________________________________________________
class IlcESDfriendTrack : public TObject {
public:
  enum {
    kMaxITScluster=12,
    kMaxVXDcluster=38,
    kMaxTPCcluster=160,
    kMaxTRDcluster=180
  };
  IlcESDfriendTrack();
  IlcESDfriendTrack(const IlcESDfriendTrack &t);
  virtual ~IlcESDfriendTrack();

  void Set1P(Float_t p) {f1P=p;}
  void SetTrackPointArray(IlcTrackPointArray *points) {
    fPoints=points;
  }
  Float_t Get1P() const  {return f1P;}
  Int_t *GetITSindices() {return fITSindex;}
  Int_t *GetVXDindices() {return fVXDindex;}
  Int_t *GetTPCindices() {return fTPCindex;}
  Int_t *GetTRDindices() {return fTRDindex;}
  const IlcTrackPointArray *GetTrackPointArray() const {return fPoints;}

  void SetITStrack(IlcKalmanTrack *t) {fITStrack=t;}
  void SetVXDtrack(IlcKalmanTrack *t) {fVXDtrack=t;}
  void SetTRDtrack(IlcKalmanTrack *t) {fTRDtrack=t;}
  IlcKalmanTrack *GetTRDtrack() {return fTRDtrack;}
  IlcKalmanTrack *GetITStrack() {return fITStrack;}
  IlcKalmanTrack *GetVXDtrack() {return fVXDtrack;}
  void AddCalibObject(TObject * calibObject); 
  TObject * GetCalibObject(Int_t index);
  //
  // parameters backup
  void SetTPCOut(const IlcExternalTrackParam &param);
  void SetITSOut(const IlcExternalTrackParam &param);
  void SetVXDOut(const IlcExternalTrackParam &param);
  void SetTRDIn(const IlcExternalTrackParam  &param);
  //
  
  const IlcExternalTrackParam * GetTPCOut() const {return  fTPCOut;} 
  const IlcExternalTrackParam * GetITSOut() const {return fITSOut;} 
  const IlcExternalTrackParam * GetVXDOut() const {return fVXDOut;}
  const IlcExternalTrackParam * GetTRDIn()  const {return fTRDIn;} 

  void SetITSIndices(Int_t* indices, Int_t n);
  void SetVXDIndices(Int_t* indices, Int_t n);
  void SetTPCIndices(Int_t* indices, Int_t n);
  void SetTRDIndices(Int_t* indices, Int_t n);

  Int_t GetMaxITScluster() {return fnMaxITScluster;}
  Int_t GetMaxVXDcluster() {return fnMaxVXDcluster;}
  Int_t GetMaxTPCcluster() {return fnMaxTPCcluster;}
  Int_t GetMaxTRDcluster() {return fnMaxTRDcluster;}
  
  // bit manipulation for filtering
  void SetSkipBit(Bool_t skip){SetBit(23,skip);}
  Bool_t TestSkipBit() {return TestBit(23);}

protected:
  Float_t f1P;                     // 1/P (1/(GeV/c))
  Int_t fnMaxITScluster; // Max number of ITS clusters
  Int_t fnMaxVXDcluster; // Max number of VXD clusters
  Int_t fnMaxTPCcluster; // Max number of TPC clusters
  Int_t fnMaxTRDcluster; // Max number of TRD clusters
  Int_t* fITSindex; //[fnMaxITScluster] indices of the ITS clusters 
  Int_t* fVXDindex; //[fnMaxVXDcluster] indices of the VXD clusters
  Int_t* fTPCindex; //[fnMaxTPCcluster] indices of the TPC clusters
  Int_t* fTRDindex; //[fnMaxTRDcluster] indices of the TRD clusters

  IlcTrackPointArray *fPoints;//Array of track space points in the global frame
  TObjArray      *fCalibContainer; //Array of objects for calibration    
  IlcKalmanTrack *fITStrack; //! pointer to the ITS track (debug purposes) 
  IlcKalmanTrack *fVXDtrack; //! pointer to the VXD track (debug purposes)
  IlcKalmanTrack *fTRDtrack; //! pointer to the TRD track (debug purposes) 
  //
  //
  IlcExternalTrackParam * fTPCOut; // tpc outer parameters
  IlcExternalTrackParam * fITSOut; // its outer parameters
  IlcExternalTrackParam * fVXDOut; // VXD outer parameters
  IlcExternalTrackParam * fTRDIn;  // trd inner parameters

private:
  IlcESDfriendTrack &operator=(const IlcESDfriendTrack & /* t */) {return *this;}

  ClassDef(IlcESDfriendTrack,6) //ESD friend track
};

#endif


