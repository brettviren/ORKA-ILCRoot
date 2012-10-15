#ifndef IlcPVECDATreeEvent_H
#define IlcPVECDATreeEvent_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVECDATreeEvent.h 35905 2009-10-25 19:35:32Z kharlov $ */
// --
// --
// Implementation for TTree output in PVEC DA
// for calibrating energy by pi0 and MIP.
// --
// -- Author: Hisayuki Torii (Hiroshima Univ.)
// --

#include <iostream>
#include <Rtypes.h>
#include <ctime>
#include "IlcPVECDATreeCluster.h"

class IlcPVECDATreeEvent{

  friend std::ostream& operator<<(std::ostream& out,const IlcPVECDATreeEvent& event);

 public:
  
  IlcPVECDATreeEvent(): fTime(0), fNDigits(0), fDigits(0), fNClusters(0), fClusters(0){/* */};
  IlcPVECDATreeEvent(const IlcPVECDATreeEvent& evt);
  IlcPVECDATreeEvent& operator=(const IlcPVECDATreeEvent& evt);
  virtual ~IlcPVECDATreeEvent(){
    if( fNDigits > 0 ) delete[] fDigits;
    if( fNClusters > 0 ) delete[] fClusters;
  };
  time_t GetTime() const{return fTime;};
  void SetTime(time_t time){fTime=time;};
  int GetNDigits() const{ return fNDigits; };
  int GetNClusters() const{ return fNClusters; };
  IlcPVECDATreeCluster& GetCluster(int nclusters){
    return fClusters[nclusters];
  };
  IlcPVECDATreeDigit& GetDigit(int ndigits){
    return fDigits[ndigits];
  };
  bool Fill(float fenergy,int row,int col);
  bool Fill(IlcPVECDATreeDigit& digit);
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
  bool Clusterize(IlcPVECDATreeDigit& digit);

  time_t fTime;                     // Time information
  int fNDigits;                     // Number of digits in event
  IlcPVECDATreeDigit* fDigits;      //[fNDigits]
  int fNClusters;                   // Number of clusters in event
  IlcPVECDATreeCluster* fClusters;  //[fNClusters]

  ClassDef(IlcPVECDATreeEvent,1) // Simple Event Structure for PVEC DA
};
#endif

