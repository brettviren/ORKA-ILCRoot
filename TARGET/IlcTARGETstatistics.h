#ifndef ILCTARGETSTATISTICS_H
#define ILCTARGETSTATISTICS_H
//////////////////////////////////////////////////////////////////////////
//  Ilc TARGET first detector alignment program.                         //
//                                                                      //
// version: 0.0.0 Draft.                                                //
// Date: April 18 1999                                                  //
// By: Bjorn S. Nilsen                                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TObject.h"

class IlcTARGETstatistics : public TObject {
//

 public:
  IlcTARGETstatistics();
  IlcTARGETstatistics(Int_t order);
  IlcTARGETstatistics(IlcTARGETstatistics &source); // copy  constructor
  IlcTARGETstatistics& operator=(IlcTARGETstatistics &source); // operator=
  virtual ~IlcTARGETstatistics();
  void Reset();
  void AddValue(Double_t x,Double_t w);
  void AddValue(Double_t x){ Double_t weight=1.0; AddValue(x,weight);} // Default weight of 1
  void AddValue(Float_t x,Float_t w){AddValue((Double_t)x,(Double_t)w);} //float
  void AddValue(Float_t x){Float_t weight=1.0; AddValue(x,weight);} // floats default weight of 1
  Double_t GetNth(Int_t order);
  Double_t GetMean() {// returns the mean
    return GetNth(1);};
  Int_t GetN(){// returns the number of entries
    return fN;
  };
  Int_t GetOrder(){// returns the order of the moment of the distribution
    return fOrder;
  };
  Double_t GetXN(Int_t order){// returns X^N
    return fX[order-1];
  };
  Double_t GetWN(Int_t order){// returns W^N
    return fW[order-1];
  };
  Double_t GetRMS();
  Double_t GetErrorMean();
  Double_t GetErrorRMS();

 private:
  Int_t    fN;    //fN number of enetries
  Int_t    fOrder;//fOrder maximum allowed moment
  Double_t *fX;   //[fOrder] fX array of x moments
  Double_t *fW;   //[fOrder] fW array of weight by moment

  ClassDef(IlcTARGETstatistics,1)// A class to do simple statistics calculations
};
#endif
