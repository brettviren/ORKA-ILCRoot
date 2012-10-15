#ifndef ILCTPCCLUSTERSARRAY_H
#define ILCTPCCLUSTERSARRAY_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHClustersArray.h,v 1.1 2008/04/22 09:43:37 tassiell Exp $ */

////////////////////////////////////////////////
//  Manager class for TPC   clusters                   //
////////////////////////////////////////////////


#include "IlcClustersArray.h"
 




class IlcTPCClustersRow;

class IlcTPCClustersArray : public IlcClustersArray {
public:
  IlcTPCClustersArray();
  virtual ~IlcTPCClustersArray();
  IlcTPCClustersRow * GetRow(Int_t sector,Int_t row);  
  IlcTPCClustersRow * CreateRow(Int_t sector, Int_t row); //
  IlcTPCClustersRow * LoadRow(Int_t sector,Int_t row);
  Bool_t StoreRow(Int_t sector,Int_t row);
  Bool_t ClearRow(Int_t sector,Int_t row);
  Bool_t Setup(const IlcDetectorParam *param);     
  //construct array  according parameters in fParam   
  Bool_t  Update(); //blabla 
  IlcSegmentID * NewSegment(); //create new segment - IlcTPCClustersRow
protected:
  //void MakeTree(); 
 
private:   
  ClassDef(IlcTPCClustersArray,1) // Cluster manager 
};
  
#endif //ILCTPCCLUSTERSARRAY_H
