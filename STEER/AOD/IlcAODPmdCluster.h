#ifndef IlcAODPmdCluster_H
#define IlcAODPmdCluster_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAODPmdCluster.h 50615 2011-07-16 23:19:19Z hristov $ */

//-------------------------------------------------------------------------
//     AOD PMD cluster class
//     Author: Markus Oldenburg, CERN
//-------------------------------------------------------------------------

#include <TRef.h>

#include "IlcAODCluster.h"
class IlcAODPmdCluster : public IlcAODCluster {

 public:
  
  IlcAODPmdCluster();
  IlcAODPmdCluster(Int_t id,
		   Int_t nLabel,
		   Int_t *label,
		   Double_t energy,
		   Double_t x[3],
		   Double_t pid[13],
		   Char_t ttype=kUndef,
		   UInt_t selectInfo=0,
		   IlcAODPmdCluster* assoc=NULL);
  
  IlcAODPmdCluster(Int_t id,
		   Int_t nLabel,
		   Int_t *label,
		   Float_t energy,
		   Float_t x[3],
		   Float_t pid[13],
		   Char_t ttype=kUndef,
		   UInt_t selectInfo=0,
		   IlcAODPmdCluster* assoc=NULL);
  
  virtual ~IlcAODPmdCluster();
  IlcAODPmdCluster(const IlcAODPmdCluster& clus); 
  IlcAODPmdCluster& operator=(const IlcAODPmdCluster& clus);

  IlcAODPmdCluster *GetAssocCluster() const { return (IlcAODPmdCluster*)fAssocCluster.GetObject(); }
  void SetAssocCluster(IlcAODPmdCluster *cluster) { fAssocCluster = cluster; }

 private :

  TRef fAssocCluster;   // cluster of other layer associated with this cluster

  ClassDef(IlcAODPmdCluster,1);
};

#endif
