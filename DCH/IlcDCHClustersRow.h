#ifndef ILCTPCCLUSTERROW_H
#define ILCTPCCLUSTERROW_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHClustersRow.h,v 1.1 2008/04/22 09:43:37 tassiell Exp $ */

////////////////////////////////////////////////
//  Manager class for TPC   clusters                   //
////////////////////////////////////////////////


#include "IlcClusters.h"

class TObject;


class IlcTPCClustersRow : public IlcClusters{
public:
  IlcTPCClustersRow();
  virtual TObject *InsertCluster(const TObject *c);
  virtual TObject *Append();  //create new object return pointer to this object

public:
  
  ClassDef(IlcTPCClustersRow,1) // Cluster manager 
};  
#endif //ILCTPCCLUSTERROW_H







