#ifndef IlcRSTACKDATreeCluster_H
#define IlcRSTACKDATreeCluster_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcRSTACKDATreeCluster.h 35905 2009-10-25 19:35:32Z kharlov $ */

// --
// --
// Implementation for TTree output in RSTACK DA
// for calibrating energy by pi0 and MIP.
// --
// -- Author: Hisayuki Torii (Hiroshima Univ.)
// --

#include <iostream>
#include "IlcRSTACKDATreeDigit.h"

class IlcRSTACKDATreeCluster{

  friend std::ostream& operator<<(std::ostream& out,const IlcRSTACKDATreeCluster& cluster);

 public:

  //IlcRSTACKDATreeCluster():fEnergy(0),fX(0),fY(0),fZ(0),fNDigits(0),fDigits(0){/**/};
  //IlcRSTACKDATreeCluster(float energy,float x,float y,float z):fEnergy(energy),fX(x),fY(y),fZ(z),fNDigits(0),fDigits(0){/**/};
  //void Set(float energy,float x,float y,float z){fEnergy=energy; fX=x; fY=y; fZ=z;};
  //float GetX(){ return fX; };
  //float GetY(){ return fY; };
  //float GetZ(){ return fZ; };
  IlcRSTACKDATreeCluster():fEnergy(0),fRow(-100),fCol(-100),fNDigits(0),fDigits(0){/**/};
  IlcRSTACKDATreeCluster(float energy,float row,float col):fEnergy(energy),fRow(row),fCol(col),fNDigits(0),fDigits(0){/**/};
  virtual ~IlcRSTACKDATreeCluster(){ if(fNDigits>0) delete[] fDigits; };
  IlcRSTACKDATreeCluster(const IlcRSTACKDATreeCluster& cluster);
  IlcRSTACKDATreeCluster& operator=(const IlcRSTACKDATreeCluster& cluster);
  void Set(float energy,float row,float col){fEnergy=energy; fRow=row; fCol=col; };
  void SetEnergy(float energy){fEnergy=energy;};
  float GetEnergy() const{ return fEnergy; };
  float GetRow() const{ return fRow; };
  float GetCol() const{ return fCol; };
  bool CalculateProperty();
  int GetNDigits() const{ return fNDigits; };
  IlcRSTACKDATreeDigit& GetDigit(int ndigit){
    return fDigits[ndigit];
  };
  IlcRSTACKDATreeDigit& GetMaxDigit(){
    return fDigits[0];
  };
  bool Append(IlcRSTACKDATreeDigit& digit);
  bool Append(IlcRSTACKDATreeCluster& cluster);
  bool IsNeighbor(const IlcRSTACKDATreeDigit& digit) const;
  bool IsNeighbor(const IlcRSTACKDATreeCluster& cluster) const;
  void Reset();
  void Print(Option_t *option="") const;

 private:

  float fEnergy;               // Energy in GeV
  float fRow;                  // RSTACK Internal Coordinates, 0 - 63
  float fCol;                  // RSTACK Internal Coordinates, 0 - 55
  //float fX, fY, fZ;
  int fNDigits;                // Number of digits
  IlcRSTACKDATreeDigit* fDigits; //[fNDigits]

  ClassDef(IlcRSTACKDATreeCluster,1) // Simple Cluster Structure for RSTACK DA
};

#endif
