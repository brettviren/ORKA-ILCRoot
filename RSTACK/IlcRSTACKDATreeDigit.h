#ifndef IlcRSTACKDATreeDigit_H
#define IlcRSTACKDATreeDigit_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcRSTACKDATreeDigit.h 35905 2009-10-25 19:35:32Z kharlov $ */

// --
// --
// Implementation for TTree output in RSTACK DA
// for calibrating energy by pi0 and MIP.
// --
// -- Author: Hisayuki Torii (Hiroshima Univ.)
// --

class IlcRSTACKDATreeDigit {

  friend std::ostream& operator<<(std::ostream& out,const IlcRSTACKDATreeDigit& digit);

 public:

  IlcRSTACKDATreeDigit():fEnergy(0),fAbsId(-1){/**/};
  IlcRSTACKDATreeDigit(float energy,int row,int col):fEnergy(energy),fAbsId(row*56+col){/**/};
  IlcRSTACKDATreeDigit(float energy,short absid):fEnergy(energy),fAbsId(absid){/**/};
  virtual ~IlcRSTACKDATreeDigit(){/**/};
  void Set(float energy,int row,int col){fEnergy=energy; fAbsId = row*56 + col;};
  void Set(float energy,short absid){fEnergy=energy; fAbsId = absid;};
  void SetEnergy(float energy){ fEnergy=energy; };
  float GetEnergy() const{return fEnergy;};
  int GetCol() const{return fAbsId % 56; };     // 0 - 55 : col( z ) number
  int GetRow() const{return (int) fAbsId/56; }; // 0 - 63 : row( x ) number
  short GetAbsId() const{return fAbsId; };
  short GetDigitId() const{return fAbsId; };
  bool IsValid() const { if( fAbsId>=0 && fAbsId < 3584 ) return true; return false; };
  bool IsNeighbor(const IlcRSTACKDATreeDigit &digit) const;
  void Print(Option_t *opt="") const;

  //IlcRSTACKDATreeDigit(float energy,int mod,int row,int col):fEnergy(energy),fAbsId(mod*3584+row*56+col){/**/};
  //void Set(float energy,int mod,int row,int col){
  //fEnergy=energy; fAbsId = mod * 3584 + row*56 + col;};
  //int GetMod(){return (int)(fAbsId/3584); };      // 0 -  4 : module number
  //int GetCol(){return (fAbsId%3584) % 56; };      // 0 - 55 : col( z ) number
  //int GetRow(){return (int)(fAbsId%3584) / 56; }; // 0 - 63 : row( x ) number
  //bool IsValid(){ if( fAbsId>=0 && fAbsId < 17920 ) return true; return false; };

 private:

  float fEnergy; // Energy in GeV
  short fAbsId;  // col + row * 56 [+mod*3584 in some cases]

  ClassDef(IlcRSTACKDATreeDigit,1) // Simple Digit Structure for RSTACK DA
};

#endif
