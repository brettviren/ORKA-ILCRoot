#ifndef IlcPVBARDATreeCluster_H
#define IlcPVBARDATreeCluster_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARDATreeCluster.h 35905 2009-10-25 19:35:32Z kharlov $ */

// --
// --
// Implementation for TTree output in PVBAR DA
// for calibrating energy by pi0 and MIP.
// --
// -- Author: Hisayuki Torii (Hiroshima Univ.)
// --

#include <iostream>
#include "IlcPVBARDATreeDigit.h"

class IlcPVBARDATreeCluster{

  friend std::ostream& operator<<(std::ostream& out,const IlcPVBARDATreeCluster& cluster);

 public:

  //IlcPVBARDATreeCluster():fEnergy(0),fX(0),fY(0),fZ(0),fNDigits(0),fDigits(0){/**/};
  //IlcPVBARDATreeCluster(float energy,float x,float y,float z):fEnergy(energy),fX(x),fY(y),fZ(z),fNDigits(0),fDigits(0){/**/};
  //void Set(float energy,float x,float y,float z){fEnergy=energy; fX=x; fY=y; fZ=z;};
  //float GetX(){ return fX; };
  //float GetY(){ return fY; };
  //float GetZ(){ return fZ; };
  IlcPVBARDATreeCluster():fEnergy(0),fRow(-100),fCol(-100),fNDigits(0),fDigits(0){/**/};
  IlcPVBARDATreeCluster(float energy,float row,float col):fEnergy(energy),fRow(row),fCol(col),fNDigits(0),fDigits(0){/**/};
  virtual ~IlcPVBARDATreeCluster(){ if(fNDigits>0) delete[] fDigits; };
  IlcPVBARDATreeCluster(const IlcPVBARDATreeCluster& cluster);
  IlcPVBARDATreeCluster& operator=(const IlcPVBARDATreeCluster& cluster);
  void Set(float energy,float row,float col){fEnergy=energy; fRow=row; fCol=col; };
  void SetEnergy(float energy){fEnergy=energy;};
  float GetEnergy() const{ return fEnergy; };
  float GetRow() const{ return fRow; };
  float GetCol() const{ return fCol; };
  bool CalculateProperty();
  int GetNDigits() const{ return fNDigits; };
  IlcPVBARDATreeDigit& GetDigit(int ndigit){
    return fDigits[ndigit];
  };
  IlcPVBARDATreeDigit& GetMaxDigit(){
    return fDigits[0];
  };
  bool Append(IlcPVBARDATreeDigit& digit);
  bool Append(IlcPVBARDATreeCluster& cluster);
  bool IsNeighbor(const IlcPVBARDATreeDigit& digit) const;
  bool IsNeighbor(const IlcPVBARDATreeCluster& cluster) const;
  void Reset();
  void Print(Option_t *option="") const;

 private:

  float fEnergy;               // Energy in GeV
  float fRow;                  // PVBAR Internal Coordinates, 0 - 63
  float fCol;                  // PVBAR Internal Coordinates, 0 - 55
  //float fX, fY, fZ;
  int fNDigits;                // Number of digits
  IlcPVBARDATreeDigit* fDigits; //[fNDigits]

  ClassDef(IlcPVBARDATreeCluster,1) // Simple Cluster Structure for PVBAR DA
};

#endif
