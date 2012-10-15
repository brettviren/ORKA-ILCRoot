#ifndef IlcGenGeVSimEventHeader_h
#define IlcGenGeVSimEventHeader_h

//
// Event header for GeVSim event generator
// support event plane and elliptic flow
// in next release will suport full differential 
// directed and elliptic flow
//
// Sylwester Radomski, GSI
// mail: S.Radomski@gsi
// 31 Oct, 2002
//

#include "IlcGenEventHeader.h"


class IlcGenGeVSimEventHeader: public IlcGenEventHeader {

 public:
  
  //Constructors
  IlcGenGeVSimEventHeader();
  IlcGenGeVSimEventHeader(const char *name);
  ~IlcGenGeVSimEventHeader() {}

  //Getters
  Float_t GetEventPlane() const {return fEventPlane;}
  Float_t GetEllipticFlow() const {return fEllipticFlow;}

  //Setters
  void SetEventPlane(Float_t psi);
  void SetEllipticFlow(Float_t v2);

 private:
  
  Float_t fEventPlane;       // event plane in rad.
  Float_t fEllipticFlow;     // elliptic flow (fast solution)

 public:
  ClassDef(IlcGenGeVSimEventHeader, 1) // Event Header for GeVSim

};



#endif
