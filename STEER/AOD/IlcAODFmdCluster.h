#ifndef IlcAODFmdCluster_H
#define IlcAODFmdCluster_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAODFmdCluster.h 50615 2011-07-16 23:19:19Z hristov $ */

//-------------------------------------------------------------------------
//     AOD FMD cluster class
//     Author: Markus Oldenburg, CERN
//-------------------------------------------------------------------------

#include <TRef.h>

#include "IlcAODCluster.h"
#include "IlcAODTrack.h"

class IlcAODFmdCluster : public IlcAODCluster {

 public:
  
  IlcAODFmdCluster();
  IlcAODFmdCluster(Int_t id,
		   Int_t nLabel,
		   Int_t *label,
		   Double_t energy,
		   Double_t x[3],
		   Double_t pid[9],
		   Char_t ttype=kUndef,
		   IlcAODVertex *prodVertex=NULL,
		   IlcAODTrack *primTrack=NULL);

   IlcAODFmdCluster(Int_t id,
		    Int_t nLabel,
		    Int_t *label,
		    Float_t energy,
		    Float_t x[3],
		    Float_t pid[9],
		    Char_t ttype=kUndef,
		    IlcAODVertex *prodVertex=NULL,
		    IlcAODTrack *primTrack=NULL);

  virtual ~IlcAODFmdCluster();
  IlcAODFmdCluster(const IlcAODFmdCluster& trk); 
  IlcAODFmdCluster& operator=(const IlcAODFmdCluster& trk);

  IlcAODVertex *GetProdVertex() const { return (IlcAODVertex*)fProdVertex.GetObject(); }
  IlcAODTrack *GetPrimTrack() const { return (IlcAODTrack*)fPrimTrack.GetObject(); }
  
  void SetProdVertex(IlcAODVertex *vertex) { fProdVertex = vertex; }
  void SetPrimTrack(IlcAODTrack *ptrack) { fPrimTrack = ptrack; }


 private :

  TRef fProdVertex;     // vertex of origin
  TRef fPrimTrack;      // primary track associated with this cluster

  ClassDef(IlcAODFmdCluster,1);
};

#endif
