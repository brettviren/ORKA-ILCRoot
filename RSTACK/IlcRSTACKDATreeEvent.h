#ifndef IlcRSTACKDATreeEvent_H
#define IlcRSTACKDATreeEvent_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcRSTACKDATreeEvent.h 35905 2009-10-25 19:35:32Z kharlov $ */
// --
// --
// Implementation for TTree output in RSTACK DA
// for calibrating energy by pi0 and MIP.
// --
// -- Author: Hisayuki Torii (Hiroshima Univ.)
// --

#include <iostream>
#include <Rtypes.h>
#include <ctime>
#include "IlcRSTACKDATreeCluster.h"

class IlcRSTACKDATreeEvent{

  friend std::ostream& operator<<(std::ostream& out,const IlcRSTACKDATreeEvent& event);

 public:
  
  IlcRSTACKDATreeEvent(): fTime(0), fNDigits(0), fDigits(0), fNClusters(0), fClusters(0){/* */};
  IlcRSTACKDATreeEvent(const IlcRSTACKDATreeEvent& evt);
  IlcRSTACKDATreeEvent& operator=(const IlcRSTACKDATreeEvent& evt);
  virtual ~IlcRSTACKDATreeEvent(){
    if( fNDigits > 0 ) delete[] fDigits;
    if( fNClusters > 0 ) delete[] fClusters;
  };
  time_t GetTime() const{return fTime;};
  void SetTime(time_t time){fTime=time;};
  int GetNDigits() const{ return fNDigits; };
  int GetNClusters() const{ return fNClusters; };
  IlcRSTACKDATreeCluster& GetCluster(int nclusters){
    return fClusters[nclusters];
  };
  IlcRSTACKDATreeDigit& GetDigit(int ndigits){
    return fDigits[ndigits];
  };
  bool Fill(float fenergy,int row,int col);
  bool Fill(IlcRSTACKDATreeDigit& digit);
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
  bool Clusterize(IlcRSTACKDATreeDigit& digit);

  time_t fTime;                     // Time information
  int fNDigits;                     // Number of digits in event
  IlcRSTACKDATreeDigit* fDigits;      //[fNDigits]
  int fNClusters;                   // Number of clusters in event
  IlcRSTACKDATreeCluster* fClusters;  //[fNClusters]

  ClassDef(IlcRSTACKDATreeEvent,1) // Simple Event Structure for RSTACK DA
};
#endif

