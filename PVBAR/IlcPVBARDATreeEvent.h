#ifndef IlcPVBARDATreeEvent_H
#define IlcPVBARDATreeEvent_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARDATreeEvent.h 35905 2009-10-25 19:35:32Z kharlov $ */
// --
// --
// Implementation for TTree output in PVBAR DA
// for calibrating energy by pi0 and MIP.
// --
// -- Author: Hisayuki Torii (Hiroshima Univ.)
// --

#include <iostream>
#include <Rtypes.h>
#include <ctime>
#include "IlcPVBARDATreeCluster.h"

class IlcPVBARDATreeEvent{

  friend std::ostream& operator<<(std::ostream& out,const IlcPVBARDATreeEvent& event);

 public:
  
  IlcPVBARDATreeEvent(): fTime(0), fNDigits(0), fDigits(0), fNClusters(0), fClusters(0){/* */};
  IlcPVBARDATreeEvent(const IlcPVBARDATreeEvent& evt);
  IlcPVBARDATreeEvent& operator=(const IlcPVBARDATreeEvent& evt);
  virtual ~IlcPVBARDATreeEvent(){
    if( fNDigits > 0 ) delete[] fDigits;
    if( fNClusters > 0 ) delete[] fClusters;
  };
  time_t GetTime() const{return fTime;};
  void SetTime(time_t time){fTime=time;};
  int GetNDigits() const{ return fNDigits; };
  int GetNClusters() const{ return fNClusters; };
  IlcPVBARDATreeCluster& GetCluster(int nclusters){
    return fClusters[nclusters];
  };
  IlcPVBARDATreeDigit& GetDigit(int ndigits){
    return fDigits[ndigits];
  };
  bool Fill(float fenergy,int row,int col);
  bool Fill(IlcPVBARDATreeDigit& digit);
  bool ExecuteClustering();
  void Reset(){
    if( fNDigits > 0 ) delete[] fDigits;
    if( fNClusters > 0 ) delete[] fClusters;
    fTime = 0;
    fNDigits = 0;
    fNClusters = 0;
  };
  void Print(Option_t *option="") const;

 private:
  bool Clusterize(IlcPVBARDATreeDigit& digit);

  time_t fTime;                     // Time information
  int fNDigits;                     // Number of digits in event
  IlcPVBARDATreeDigit* fDigits;      //[fNDigits]
  int fNClusters;                   // Number of clusters in event
  IlcPVBARDATreeCluster* fClusters;  //[fNClusters]

  ClassDef(IlcPVBARDATreeEvent,1) // Simple Event Structure for PVBAR DA
};
#endif

