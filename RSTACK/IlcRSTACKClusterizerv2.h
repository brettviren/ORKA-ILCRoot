#ifndef IlcRSTACKClusterizerv2_H
#define IlcRSTACKClusterizerv2_H

// --- IlcRoot header files ---
#include "IlcRSTACKClusterizerv1.h"

class IlcRSTACKClusterizerv2 : public IlcRSTACKClusterizerv1 {

public:

  IlcRSTACKClusterizerv2();
  IlcRSTACKClusterizerv2(IlcRSTACKGeometry *geom);
  ~IlcRSTACKClusterizerv2() {}
  
  Int_t AreNeighbours(IlcRSTACKDigit* d1, IlcRSTACKDigit* d2) const ;
  void GetNumberOfClustersFound(int* numb ) const;

  virtual void Digits2Clusters(Option_t* option);

private:
  IlcRSTACKClusterizerv2(const IlcRSTACKClusterizerv2 & clu) ;
  IlcRSTACKClusterizerv2 & operator = (const IlcRSTACKClusterizerv2 &rvalue);

  ClassDef(IlcRSTACKClusterizerv2,2)
};

#endif
