#ifndef IlcPVBARClusterizerv2_H
#define IlcPVBARClusterizerv2_H

// --- IlcRoot header files ---
#include "IlcPVBARClusterizerv1.h"

class IlcPVBARClusterizerv2 : public IlcPVBARClusterizerv1 {

public:

  IlcPVBARClusterizerv2();
  IlcPVBARClusterizerv2(IlcPVBARGeometry *geom);
  ~IlcPVBARClusterizerv2() {}
  
  Int_t AreNeighbours(IlcPVBARDigit* d1, IlcPVBARDigit* d2) const ;
  void GetNumberOfClustersFound(int* numb ) const;

  virtual void Digits2Clusters(Option_t* option);

private:
  IlcPVBARClusterizerv2(const IlcPVBARClusterizerv2 & clu) ;
  IlcPVBARClusterizerv2 & operator = (const IlcPVBARClusterizerv2 &rvalue);

  ClassDef(IlcPVBARClusterizerv2,2)
};

#endif
