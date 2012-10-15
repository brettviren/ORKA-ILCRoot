#ifndef IlcPVECDATreeCluster_H
#define IlcPVECDATreeCluster_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVECDATreeCluster.h 35905 2009-10-25 19:35:32Z kharlov $ */

// --
// --
// Implementation for TTree output in PVEC DA
// for calibrating energy by pi0 and MIP.
// --
// -- Author: Hisayuki Torii (Hiroshima Univ.)
// --

#include <iostream>
#include "IlcPVECDATreeDigit.h"

class IlcPVECDATreeCluster{

  friend std::ostream& operator<<(std::ostream& out,const IlcPVECDATreeCluster& cluster);

 public:

  //IlcPVECDATreeCluster():fEnergy(0),fX(0),fY(0),fZ(0),fNDigits(0),fDigits(0){/**/};
  //IlcPVECDATreeCluster(float energy,float x,float y,float z):fEnergy(energy),fX(x),fY(y),fZ(z),fNDigits(0),fDigits(0){/**/};
  //void Set(float energy,float x,float y,float z){fEnergy=energy; fX=x; fY=y; fZ=z;};
  //float GetX(){ return fX; };
  //float GetY(){ return fY; };
  //float GetZ(){ return fZ; };
  IlcPVECDATreeCluster():fEnergy(0),fRow(-100),fCol(-100),fNDigits(0),fDigits(0){/**/};
  IlcPVECDATreeCluster(float energy,float row,float col):fEnergy(energy),fRow(row),fCol(col),fNDigits(0),fDigits(0){/**/};
  virtual ~IlcPVECDATreeCluster(){ if(fNDigits>0) delete[] fDigits; };
  IlcPVECDATreeCluster(const IlcPVECDATreeCluster& cluster);
  IlcPVECDATreeCluster& operator=(const IlcPVECDATreeCluster& cluster);
  void Set(float energy,float row,float col){fEnergy=energy; fRow=row; fCol=col; };
  void SetEnergy(float energy){fEnergy=energy;};
  float GetEnergy() const{ return fEnergy; };
  float GetRow() const{ return fRow; };
  float GetCol() const{ return fCol; };
  bool CalculateProperty();
  int GetNDigits() const{ return fNDigits; };
  IlcPVECDATreeDigit& GetDigit(int ndigit){
    return fDigits[ndigit];
  };
  IlcPVECDATreeDigit& GetMaxDigit(){
    return fDigits[0];
  };
  bool Append(IlcPVECDATreeDigit& digit);
  bool Append(IlcPVECDATreeCluster& cluster);
  bool IsNeighbor(const IlcPVECDATreeDigit& digit) const;
  bool IsNeighbor(const IlcPVECDATreeCluster& cluster) const;
  void Reset();
  void Print(Option_t *option="") const;

 private:

  float fEnergy;               // Energy in GeV
  float fRow;                  // PVEC Internal Coordinates, 0 - 63
  float fCol;                  // PVEC Internal Coordinates, 0 - 55
  //float fX, fY, fZ;
  int fNDigits;                // Number of digits
  IlcPVECDATreeDigit* fDigits; //[fNDigits]

  ClassDef(IlcPVECDATreeCluster,1) // Simple Cluster Structure for PVEC DA
};

#endif
