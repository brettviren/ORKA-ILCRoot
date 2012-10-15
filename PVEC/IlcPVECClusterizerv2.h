#ifndef IlcPVECClusterizerv2_H
#define IlcPVECClusterizerv2_H

// --- IlcRoot header files ---
#include "IlcPVECClusterizerv1.h"

class IlcPVECClusterizerv2 : public IlcPVECClusterizerv1 {

public:

  IlcPVECClusterizerv2();
  IlcPVECClusterizerv2(IlcPVECGeometry *geom);
  ~IlcPVECClusterizerv2() {}
  
  Int_t AreNeighbours(IlcPVECDigit* d1, IlcPVECDigit* d2) const ;
  void GetNumberOfClustersFound(int* numb ) const;

  virtual void Digits2Clusters(Option_t* option);

private:
  IlcPVECClusterizerv2(const IlcPVECClusterizerv2 & clu) ;
  IlcPVECClusterizerv2 & operator = (const IlcPVECClusterizerv2 &rvalue);

  ClassDef(IlcPVECClusterizerv2,2)
};

#endif
